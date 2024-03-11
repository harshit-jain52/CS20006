#include "TreeIndex.h"

// TreeNode

TreeNode::TreeNode() : left(NULL), right(NULL) {}
TreeNode::TreeNode(vector<int> v, TreeNode *l, TreeNode *r) : left(l), right(r), indices(v) {}
TreeNode::~TreeNode()
{
    delete left;
    delete right;
}

// TreeIndex

TreeIndex::TreeIndex(VectorDataset &vds, int minsz) : dataset(vds), MinSize(minsz), root_(NULL) {}
TreeIndex::~TreeIndex() { delete root_; }

TreeIndex *TreeIndex::GetInstance()
{
    if (Instance == nullptr)
        throw invalid_argument("Constuct a tree index first");

    return Instance;
}

void TreeIndex::MakeTreeWrapper()
{
    delete root_;
    int sz = dataset.getDatasetSize();
    vector<int> ind(sz);
    for (int i = 0; i < sz; i++)
        ind[i] = i;

    root_ = new TreeNode(ind);
    MakeTree(root_);
}

void TreeIndex::MakeTree(TreeNode *root)
{
    auto rule = ChooseRule(root->indices);
    root->dir = rule.first;
    root->split = rule.second;

    vector<int> leftvec, rightvec;
    int numDVs = root->indices.size();
    for (int i = 0; i < numDVs; i++)
    {
        if (dataset.getVector(root->indices[i]) * root->dir <= root->split)
            leftvec.push_back(root->indices[i]);
        else
            rightvec.push_back(root->indices[i]);
    }

    if (leftvec.size() < MinSize || rightvec.size() < MinSize)
        return;

    root->left = new TreeNode(leftvec);
    root->right = new TreeNode(rightvec);
    MakeTree(root->left);
    MakeTree(root->right);
}

void TreeIndex::AddData(const DataVector &newVector)
{
    dataset.addVector(newVector);
    MakeTreeWrapper();
}

void TreeIndex::RemoveData(const DataVector &vector)
{
    bool isRemoved = dataset.removeVector(vector);
    if (!isRemoved)
        return;
    MakeTreeWrapper();
}

VectorDataset TreeIndex::SearchWrapper(const DataVector &point, int k)
{
    priority_queue<pair<double, int>> pq;
    Search(root_, point, pq, k);
    VectorDataset ANN(k);

    int i = 1;
    while (!pq.empty())
    {
        ANN.setVector(k - i, dataset.getVector(pq.top().second));
        pq.pop();
        i++;
    }

    return ANN;
}

void TreeIndex::Search(TreeNode *root, const DataVector &point, priority_queue<pair<double, int>> &kNearest, int k)
{
    if (root == NULL)
        return;

    if (root->left == NULL && root->right == NULL)
    {
        for (int idx : root->indices)
        {
            double neigh_dist = dataset.getVector(idx).dist(point);
            if (kNearest.size() < k)
            {
                kNearest.push({neigh_dist, idx});
                continue;
            }

            if (kNearest.top().first > neigh_dist)
            {
                kNearest.pop();
                kNearest.push({neigh_dist, idx});
            }
        }

        return;
    }

    if (root->dir * point <= root->split)
    {
        Search(root->left, point, kNearest, k);
        if (kNearest.size() < k || abs(root->dir * point) < kNearest.top().first)
            Search(root->right, point, kNearest, k);
    }
    else
    {
        Search(root->right, point, kNearest, k);
        if (kNearest.size() < k || abs(root->dir * point) <= kNearest.top().first)
            Search(root->left, point, kNearest, k);
    }
}

// KDTreeIndex

KDTreeIndex::KDTreeIndex(VectorDataset &vds, int minsz) : TreeIndex(vds, minsz) {}

KDTreeIndex::~KDTreeIndex() {}

TreeIndex *KDTreeIndex::GetInstance(VectorDataset &vds, int minsz)
{
    if (Instance == nullptr)
        Instance = new KDTreeIndex(vds, minsz);

    return Instance;
}

pair<DataVector, int> KDTreeIndex::ChooseRule(const vector<int> &ind)
{
    int numDVs = ind.size(), dim = dataset.getVector(0).getDimension();

    // Find coordinate with the maximum spread
    vector<double> upperRange(dim, INT_MIN), lowerRange(dim, INT_MAX);
    for (int i = 0; i < numDVs; i++)
    {
        DataVector temp = dataset.getVector(ind[i]);
        for (int j = 0; j < dim; j++)
        {
            upperRange[j] = max(upperRange[j], temp.getCoordinate(j));
            lowerRange[j] = min(lowerRange[j], temp.getCoordinate(j));
        }
    }

    double maxSpread = -1, coord = 0;
    for (int j = 0; j < dim; j++)
    {
        double spread = upperRange[j] - lowerRange[j];
        if (spread > maxSpread)
        {
            maxSpread = spread;
            coord = j;
        }
    }

    // Calculate median
    vector<double> allCoord(numDVs);
    for (int i = 0; i < numDVs; i++)
        allCoord[i] = dataset.getVector(ind[i]).getCoordinate(coord);

    sort(allCoord.begin(), allCoord.end());

    DataVector dir(dim);
    dir.setCoordinate(coord, 1);
    double median = allCoord[numDVs / 2];

    return {dir, median};
}

// RPTreeIndex

RPTreeIndex::RPTreeIndex(VectorDataset &vds, int minsz) : TreeIndex(vds, minsz) {}

RPTreeIndex::~RPTreeIndex() {}

TreeIndex *RPTreeIndex::GetInstance(VectorDataset &vds, int minsz)
{
    if (Instance == nullptr)
        Instance = new RPTreeIndex(vds, minsz);

    return Instance;
}

pair<DataVector, int> RPTreeIndex::ChooseRule(const vector<int> &ind)
{
    int numDVs = ind.size(), dim = dataset.getVector(0).getDimension();

    // Generate Random Vector
    vector<double> random_v(dim);
    for (int j = 0; j < dim; j++)
        random_v[j] = 2 * ((double)rand() / RAND_MAX) - 1;

    // Normalize the generated vector
    double norm = DataVector(random_v).norm();
    for (int j = 0; j < dim; j++)
        random_v[j] /= norm;
    DataVector dir(random_v);

    // Calculate median and delta
    DataVector x = dataset.getVector(ind[rand() % numDVs]); // Choose a random vector from set
    vector<double> allCoord(numDVs);
    double maxdist = 0;

    for (int i = 0; i < numDVs; i++)
    {
        maxdist = max(maxdist, x.dist(dataset.getVector(ind[i])));
        allCoord[i] = dataset.getVector(ind[i]) * dir;
    }

    sort(allCoord.begin(), allCoord.end());

    double delta = (2 * ((double)rand() / RAND_MAX) - 1) * 6 * maxdist / sqrt(dim);
    return {dir, allCoord[numDVs / 2] + delta};
}

#include <utility>
#include <algorithm>
#include <queue>
#include <climits>
#include <random>
#include "VectorDataset.h"
using namespace std;

/*
STRUCTURE and IMPLEMENTATION

TreeNode class: Helper class representing each node of ANN Index structure

*public attributes:
- *left & *right : Pointers to children nodes
- indices : collection of indices of datavectors represented by the node
- dir : splitting plane at the node
- split : split-value at the node

*member functions:
- DataVector() : Constructors
- ~DataVector() : Destructor
-----------------------------------------------------------------------------------------------------
TreeIndex class: Base class defining the basic functionalities of ANN index

*protected attributes:
- dataset : Dataset to search in
- root_ : Root of the tree structure
- MinSize : Minimum size of a leaf
- Instance : static attribute to facilitate functionality of Singleton class

*protected member functions:
- TreeIndex() : Constructor
- MakeTree() : Core tree-building algorithm
- Search() : To search for K nearest neighbours in the tree structure

*public member funcitons:
- ~TreeIndex() : Destructor
- GetInstance() : static function to access Instance or raise error if not created
- MakeTreeWrapper() : Wrapper for MakeTree() function
- SearchWrapper() : Wrapper for Search() function
- AddData() : Add a vector to the dataset
- RemoveData() : Remove a vector from the dataset
- ChooseRule() : pure virtual function that dispatches to the ChooseRule of appropriate subclass
-----------------------------------------------------------------------------------------------------
KDTreeIndex class: class derived from TreeIndex

*public member functions:
GetInstance() : static function to access Instance or to create one (pointing to KDTreeIndex object) if not created
ChooseRule() : subroutine that defines the nature of split in KDTreeIndex structure
    -pick a coordinate with the largest spread
    -calculate median value for the coordinate

*private member functions:
KDTreeIndex() : Constructor
~KDTreeIndex() : Destructor
-----------------------------------------------------------------------------------------------------
RPTreeIndex class: class derived from TreeIndex

*public member functions:
GetInstance() : static function to access Instance or to create one (pointing to RPTreeIndex object) if not created
ChooseRule() : subroutine that defines the nature of split in RPTreeIndex structure:
    -choose a random unit direction
    -pick any vector (say x) from set of vectors and calculate distance between
     x and y (where y is the farthest vector from x)
    -calculate median and delta using formula given in problem statement

*private member functions:
RPTreeIndex() : Constructor
~RPTreeIndex() : Destructor
*/

class TreeNode
{
public:
    TreeNode *left;
    TreeNode *right;
    vector<int> indices;
    DataVector dir;
    double split;

    TreeNode();
    TreeNode(vector<int>, TreeNode * = NULL, TreeNode * = NULL);
    ~TreeNode();
};

class TreeIndex
{
protected:
    VectorDataset dataset;
    TreeNode *root_;
    int MinSize;
    static TreeIndex *Instance;

    TreeIndex(VectorDataset &, int);
    void MakeTree(TreeNode *);
    void Search(TreeNode *, const DataVector &, priority_queue<pair<double, int>> &, int);

public:
    virtual ~TreeIndex();
    static TreeIndex *GetInstance();
    void MakeTreeWrapper();
    VectorDataset SearchWrapper(const DataVector &, int);
    void AddData(const DataVector &);
    void RemoveData(const DataVector &);
    virtual pair<DataVector, int> ChooseRule(const vector<int> &) = 0;
};

class KDTreeIndex : public TreeIndex
{
public:
    static TreeIndex *GetInstance(VectorDataset &, int);
    pair<DataVector, int> ChooseRule(const vector<int> &) override;

private:
    KDTreeIndex(VectorDataset &, int);
    ~KDTreeIndex();
};

class RPTreeIndex : public TreeIndex
{
public:
    static TreeIndex *GetInstance(VectorDataset &, int);
    pair<DataVector, int> ChooseRule(const vector<int> &) override;

private:
    RPTreeIndex(VectorDataset &, int);
    ~RPTreeIndex();
};

enum ANN_Index
{
    KD = 1,
    RP = 2
};
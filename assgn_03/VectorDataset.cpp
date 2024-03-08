#include "VectorDataset.h"

VectorDataset::VectorDataset(int size) : dataset_(size, 0) {}
VectorDataset::VectorDataset(const VectorDataset &other) : dataset_(other.dataset_) {}

VectorDataset::~VectorDataset() {}

int VectorDataset::getDatasetSize() const
{
    return (int)dataset_.size();
}

void VectorDataset::setDatasetSize(int size)
{
    dataset_.resize(size, 0);
}

DataVector VectorDataset::getVector(int i)
{
    if (i >= dataset_.size())
    {
        std::cout << "Out of bounds of dataset" << std::endl;
        exit(0);
    }
    return dataset_[i];
}

void VectorDataset::setVector(int i, DataVector datav)
{
    if (i >= dataset_.size())
    {
        std::cout << "Out of bounds of dataset" << std::endl;
        exit(0);
    }
    dataset_[i] = datav;
}

int VectorDataset::findVector(const DataVector &datav)
{
    int sz = dataset_.size();
    for (int i = 0; i < sz; i++)
        if (dataset_[i] == datav)
            return i;

    return -1;
}

void VectorDataset::addVector(const DataVector &datav)
{
    if (dataset_.size() > 0 && datav.getDimension() != dataset_[0].getDimension())
    {
        std::cout << "addVector: Can't add vector of different dimension to the dataset" << std::endl;
        exit(0);
    }
    int sz = dataset_.size();
    dataset_.resize(sz + 1);
    dataset_[sz] = datav;
}

bool VectorDataset::removeVector(const DataVector &datav)
{
    int idx = findVector(datav);
    if (idx == -1)
        return false;

    int sz = dataset_.size();
    for (int i = idx; i < sz - 1; i++)
        dataset_[i] = dataset_[i + 1];
    dataset_.resize(sz - 1);

    return true;
}
void VectorDataset::ReadDataset(const std::string &CSVfile)
{
    std::ifstream input{CSVfile};
    std::string line;
    std::getline(input, line); // ignore header
    for (; std::getline(input, line);)
    {
        std::stringstream ss(line);
        std::vector<std::string> row;

        for (std::string data; std::getline(ss, data, ',');)
            row.push_back(std::move(data));

        int dim = row.size();
        DataVector dv(dim);
        for (int i = 0; i < dim; i++)
            dv.setCoordinate(i, std::stod(row[i]));

        dataset_.push_back(dv);
    }

    input.close();
}

std::ostream &operator<<(std::ostream &stream, const VectorDataset &other)
{
    int dim = other.getDatasetSize();
    for (int i = 0; i < dim; i++)
    {
        stream << other.dataset_[i] << "\n";
    }
    return stream;
}
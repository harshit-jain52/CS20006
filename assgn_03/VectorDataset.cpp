#include "VectorDataset.h"

VectorDataset::VectorDataset(int size) : dataset_(size, 0) {}
VectorDataset::VectorDataset(const VectorDataset &other) : dataset_(other.dataset_) {}

VectorDataset::~VectorDataset() {}

int VectorDataset::getDatasetSize() const
{
    return (int)dataset_.size();
}

void VectorDataset::setDatsetSize(int size)
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
#include "DataVector.h"

DataVector::DataVector(int dim) : dimension_(dim), v_(dim, 0) {}
DataVector::DataVector(std::vector<double> &v) : dimension_(v.size()), v_(v) {}
DataVector::DataVector(const DataVector &other) : dimension_(other.dimension_), v_(other.v_) {}

DataVector::~DataVector() {}

int DataVector::getDimension() const
{
    return dimension_;
}

void DataVector::setDimension(int dim)
{
    dimension_ = dim;
    v_.resize(dim, 0);
}

DataVector &DataVector::operator=(const DataVector &other)
{
    if (this != &other)
    {
        dimension_ = other.dimension_;
        v_ = other.v_;
    }

    return *this;
}

DataVector DataVector::operator+(const DataVector &other)
{
    if (dimension_ != other.dimension_)
    {
        std::cout << "+ : Can't add vectors of different dimension_" << std::endl;
        exit(0);
    }

    DataVector newVec(dimension_);
    for (int i = 0; i < dimension_; i++)
        newVec.v_[i] = v_[i] + other.v_[i];

    return newVec;
}

DataVector DataVector::operator-(const DataVector &other)
{
    if (dimension_ != other.dimension_)
    {
        std::cout << "- : Can't subtract vectors of different dimension_" << std::endl;
        exit(0);
    }

    DataVector newVec(dimension_);
    for (int i = 0; i < dimension_; i++)
        newVec.v_[i] = v_[i] - other.v_[i];

    return newVec;
}

double DataVector::operator*(const DataVector &other)
{
    if (dimension_ != other.dimension_)
    {
        std::cout << "* : Can't multiply vectors of different dimension_" << std::endl;
        exit(0);
    }

    double dotProduct = 0;
    for (int i = 0; i < dimension_; i++)
        dotProduct += v_[i] * other.v_[i];

    return dotProduct;
}

double DataVector::norm()
{
    return sqrtl(*this * *this);
}

double DataVector::dist(const DataVector &other)
{
    if (dimension_ != other.dimension_)
    {
        std::cout << "dist : Can't compute distance between vectors of different dimension_" << std::endl;
        exit(0);
    }

    return (*this - other).norm();
}

double DataVector::getCoordinate(int i)
{
    if (i >= v_.size())
    {
        std::cout << "Out of dimensions of data vector" << std::endl;
        exit(0);
    }
    return v_[i];
}

void DataVector::setCoordinate(int i, double coord)
{
    if (i >= v_.size())
    {
        std::cout << "Out of dimensions of data vector" << std::endl;
        exit(0);
    }

    v_[i] = coord;
}

bool DataVector::operator==(const DataVector &other)
{
    if (dimension_ != other.dimension_)
        return false;

    for (int i = 0; i < dimension_; i++)
        if (v_[i] != other.v_[i])
            return false;

    return true;
}

std::ostream &operator<<(std::ostream &stream, const DataVector &other)
{
    int dim = other.getDimension();
    for (int i = 0; i < dim; i++)
    {
        stream << other.v_[i];
        if (i < dim - 1)
            stream << ",";
    }
    return stream;
}

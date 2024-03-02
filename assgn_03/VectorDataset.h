#include "DataVector.h"
#include <fstream>
#include <sstream>

/*
STRUCTURE and IMPLEMENTATION

VectorDataset class: represents a dataset (collection of vectors)

*private attributes:
- dataset_ : Collection of DataVector

*member functions:
- VectorDataset() : Constructor & Copy Constructor
- ~VectorDataset() : Destructor
- getDatasetSize() & setDatasetSize() : To access and change size of dataset
- getVector() & setVector() : To access and change a vector in dataset by index (of dataset_)
- ReadDataset() : To create a dataset by reading a given CSV file

*operator overloads:
<< : (friend function) To print the dateset vectors, seperated by newline (facilitates writing dataset into CSV file)
*/

class VectorDataset
{
private:
    std::vector<DataVector> dataset_;

public:
    VectorDataset(int = 0);                                                 // Constructor
    VectorDataset(const VectorDataset &);                                   // Copy Constructor
    ~VectorDataset();                                                       // Destructor
    int getDatasetSize() const;                                             // Access size of dataset
    void setDatasetSize(int);                                               // Change size of dataset
    DataVector getVector(int);                                              // Access a vector in dataset
    void setVector(int, DataVector);                                        // Change a vector in dataset
    void ReadDataset(const std::string &);                                  // Create dataset from a CSV file
    friend std::ostream &operator<<(std::ostream &, VectorDataset const &); // Print Dataset
};
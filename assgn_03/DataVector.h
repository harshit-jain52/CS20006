#include <iostream>
#include <vector>
#include <cmath>

/*
STRUCTURE and IMPLEMENTATION

DataVector class: represents a vector, containing data in the form of its coordinates

*private attributes:
- dimension_ : Stores the dimension of the vector
- v_ : Collection of values of coordinates constituting the vector

*member functions:
- DataVector() : Constructor & Copy Constructor
- ~DataVector(): Destructor
- getDimension() & setDimension() : To access and change dimension_
- getCoordinate() & setCoordinate() : To access and change a coordinate in vector by index (of v_)
- norm() : To calculate length of the vector
- dist() : To calculate distance betweeen two vectors

*operator overloads:
= : Copy Assignment
+ : To add two vectors, coordinate by coordinate
- : To subtract two vectors, coordinate by coordinate
* : To find the dot product of two vectors
==: To compare two vectors for equality
<< : (friend function) To print the vector coordinates, seperated by commas (facilitates writing vector into CSV file)
*/

class DataVector
{
private:
    int dimension_;
    std::vector<double> v_;

public:
    DataVector(int = 0);                                                 // Constructor
    DataVector(std::vector<double> &);                                   // Constructor
    DataVector(const DataVector &);                                      // Copy Constructor
    ~DataVector();                                                       // Destructor
    DataVector &operator=(const DataVector &);                           // Copy Assignment Operator
    int getDimension() const;                                            // Access Dimension
    void setDimension(int = 0);                                          // Change Dimension
    double getCoordinate(int);                                           // Access a coordinate in Vector
    void setCoordinate(int, double);                                     // Change a coordinate in Vector
    DataVector operator+(const DataVector &);                            // Vector Addition
    DataVector operator-(const DataVector &);                            // Vector Subtraction
    double operator*(const DataVector &);                                // Dot Product
    double norm();                                                       // Length of Vector
    double dist(const DataVector &);                                     // Distance between two Vectors
    bool operator==(const DataVector &);                                 // Compare two Vectors
    friend std::ostream &operator<<(std::ostream &, DataVector const &); // Print Vector
};
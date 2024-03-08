#include <chrono>
#include "TreeIndex.h"
TreeIndex *TreeIndex::Instance = nullptr;

int main()
{
    srand(time(0));

    chrono::time_point<chrono::steady_clock> start, end;
    chrono::duration<double> elapsed_seconds;

    // Read and Create Test Dataset
    start = chrono::steady_clock::now();
    VectorDataset testData;
    testData.ReadDataset("fmnist-test.csv");
    int testsz = testData.getDatasetSize();
    end = chrono::steady_clock::now();
    elapsed_seconds = end - start;
    cout << "Reading Test Data: " << elapsed_seconds.count() << "sec\tSize: " << testsz << endl;

    // Read and Create Train Dataset
    start = chrono::steady_clock::now();
    VectorDataset trainData;
    trainData.ReadDataset("fmnist-train.csv");
    int trainsz = trainData.getDatasetSize();
    end = chrono::steady_clock::now();
    elapsed_seconds = end - start;
    cout << "Reading Train Data: " << elapsed_seconds.count() << "sec\tSize: " << trainsz << endl;

    // Create Tree Index
    int choice = 1, minSize = 10;
    cout << "1. KD Tree\n2. RP Tree\nEnter Choice: ";
    cin >> choice;
    cout << "Enter minimum size of leaf node: ";
    cin >> minSize;
    start = chrono::steady_clock::now();
    if (choice == KD)
        KDTreeIndex::GetInstance(trainData, minSize)->MakeTreeWrapper();
    else if (choice == RP)
        RPTreeIndex::GetInstance(trainData, minSize)->MakeTreeWrapper();
    else
        throw invalid_argument("Invalid Choice");

    end = chrono::steady_clock::now();
    elapsed_seconds = end - start;
    cout << "Making Tree Index: " << elapsed_seconds.count() << "sec" << endl;

    int k;
    cout << "Enter k: ";
    cin >> k;

    // Invalid value of k
    if (k <= 0)
        throw invalid_argument("k must be positive");

    if (k > trainsz)
        throw out_of_range("k can't be greater than the training-dataset size");

    int n;
    cout << "Find KNN from how many vectors? n: ";
    cin >> n;

    // Invalid value of n
    if (n <= 0)
        throw invalid_argument("n must be positive");
    if (n > testsz)
        throw out_of_range("n can't be greater than the testing-dataset size");

    // Find k nearest neighbours of each vector and write them onto a corresponding CSV file
    cout << "Computing KNN from first " << n << " test vectors" << endl;
    start = chrono::steady_clock::now();
    for (int i = 0; i < n; i++)
    {
        cout << "Test Vector " << i + 1 << "\n";
        ofstream output;
        output.open("testvector" + to_string(i + 1) + ".csv");
        output << TreeIndex::GetInstance()->SearchWrapper(testData.getVector(i), k);
        output.close();
    }
    end = chrono::steady_clock::now();
    elapsed_seconds = end - start;
    cout << "Time for training " << n << " vectors: " << elapsed_seconds.count() << "sec\n";

    delete TreeIndex::GetInstance();
}

/*
compile using command:
g++ DataVector.cpp VectorDataset.cpp TreeIndex.cpp nearestneighbour.cpp
*/
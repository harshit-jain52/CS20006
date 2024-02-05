#include "VectorDataset.h"
#include <chrono>
#include <queue>
#include <utility>

// Return a dataset containing k nearest neighbours of the given vector
VectorDataset knearestneighbour(DataVector testv, VectorDataset &traindata, int k)
{
    std::priority_queue<std::pair<double, int>> pq;
    int datasetsize = traindata.getDatasetSize();

    VectorDataset KNN(k);

    for (int i = 0; i < datasetsize; i++)
    {
        double neigh_dist = testv.dist(traindata.getVector(i));
        if (pq.size() < k)
        {
            pq.push({neigh_dist, i});
            continue;
        }

        if (pq.top().first > neigh_dist)
        {
            pq.pop();
            pq.push({neigh_dist, i});
        }
    }

    int i = 1;
    while (!pq.empty())
    {
        KNN.setVector(k - i, traindata.getVector(pq.top().second));
        pq.pop();
        i++;
    }

    return KNN;
}

int main()
{
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;

    // Read and Create Test Dataset
    start = std::chrono::steady_clock::now();
    VectorDataset testData;
    testData.ReadDataset("fmnist-test.csv");
    int testsz = testData.getDatasetSize();
    end = std::chrono::steady_clock::now();
    elapsed_seconds = end - start;
    std::cout << "Reading Test Data: " << elapsed_seconds.count() << "sec\tSize: " << testsz << std::endl;

    // Read and Create Train Dataset
    start = std::chrono::steady_clock::now();
    VectorDataset trainData;
    trainData.ReadDataset("fmnist-train.csv");
    int trainsz = trainData.getDatasetSize();
    end = std::chrono::steady_clock::now();
    elapsed_seconds = end - start;
    std::cout << "Reading Train Data: " << elapsed_seconds.count() << "sec\tSize: " << trainsz << std::endl;

    int k;
    std::cout << "Enter k: ";
    std::cin >> k;

    // Invalid value of k
    if (k <= 0)
    {
        std::cout << "k must be positive" << std::endl;
        exit(0);
    }
    if (k > trainsz)
    {
        std::cout << "k can't be greater than the training-dataset size: " << trainsz << std::endl;
        exit(0);
    }

    // Find k nearest neighbours of each vector and write them onto a corresponding CSV file

    int n;
    std::cout << "Find KNN from how many vectors? ";
    std::cin >> n;

    // Invalid value of n
    if (n <= 0)
    {
        std::cout << "n must be positive" << std::endl;
        exit(0);
    }
    if (n > testsz)
    {
        std::cout << "k can't be greater than the testing-dataset size: " << testsz << std::endl;
        exit(0);
    }

    std::cout << "Computing KNN from first " << n << " test vectors" << std::endl;
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < n; i++)
    {
        std::cout << "Test Vector " << i + 1 << "\n";
        std::ofstream output;
        output.open("testvector" + std::to_string(i + 1) + ".csv");
        output << knearestneighbour(testData.getVector(i), trainData, k);
        output.close();
    }
    end = std::chrono::steady_clock::now();
    elapsed_seconds = end - start;
    std::cout << "Time for training " << n << " vectors: " << elapsed_seconds.count() << "sec\n";
}

/*
compile using command:
g++ DataVector.cpp VectorDataset.cpp nearestneighbour.cpp
*/
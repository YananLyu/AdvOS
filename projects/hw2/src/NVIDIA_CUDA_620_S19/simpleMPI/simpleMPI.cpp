/*
 * Copyright 1993-2015 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */


/* Simple example demonstrating how to use MPI with CUDA
*
*  Generate some random numbers on one node.
*  Dispatch them to all nodes.
*  Compute their square root on each node's GPU.
*  Compute the average of the results using MPI.
*
*  simpleMPI.cpp: main program, compiled with mpicxx on linux/Mac platforms
*                 on Windows, please download the Microsoft HPC Pack SDK 2008
*/

// MPI include
#include <mpi.h>

// System includes
#include <iostream>
#include<unistd.h>

using std::cout;
using std::cerr;
using std::endl;

// User include
#include "simpleMPI.h"

// Error handling macros
#define MPI_CHECK(call) \
    if((call) != MPI_SUCCESS) { \
        cerr << "MPI error calling \""#call"\"\n"; \
        my_abort(-1); }


// Host code
// No CUDA here, only MPI
int main(int argc, char *argv[])
{
    // Initialize MPI state
    MPI_CHECK(MPI_Init(&argc, &argv));

    // Get our MPI node number and node count
    int commSize, commRank;
    MPI_CHECK(MPI_Comm_size(MPI_COMM_WORLD, &commSize));
    MPI_CHECK(MPI_Comm_rank(MPI_COMM_WORLD, &commRank));

    // Dimensions of the dataset
    int dataSizeTotal = 1 << 20 ; // 1M
    int dataSizePerNode = dataSizeTotal / commSize ;

    int blockSize = 256;
    int gridSize = (dataSizePerNode + blockSize - 1) / blockSize;

    // Generate some random numbers on the root node (node 0)
    float *dataRootA = NULL;
    float *dataRootB = NULL;  // TODO: new array

    if (commRank == 0)  // Are we the root node?
    {
        cout << "Running on " << commSize << " nodes" << endl;
        dataRootA = new float[dataSizeTotal];
	dataRootB = new float[dataSizeTotal]; // TODO
        initData(dataRootA, dataSizeTotal); // TODO
	initData(dataRootB, dataSizeTotal);
    }

    // Allocate a buffer on each node
    float *dataNodeA = new float[dataSizePerNode];
    float *dataNodeB = new float[dataSizePerNode]; // TODO

    // Dispatch a portion of the input data to each node
    MPI_CHECK(MPI_Scatter(dataRootA,
                          dataSizePerNode,
                          MPI_FLOAT,
                          dataNodeA,
                          dataSizePerNode,
                          MPI_FLOAT,
                          0,
                          MPI_COMM_WORLD));
    MPI_CHECK(MPI_Scatter(dataRootB, // TODO
                          dataSizePerNode,
                          MPI_FLOAT,
                          dataNodeB,
                          dataSizePerNode,
                          MPI_FLOAT,
                          0,
                          MPI_COMM_WORLD));

    if (commRank == 0)
    {
        // No need for root data any more
        delete [] dataRootA;
	delete [] dataRootB;  //TODO
    }

    // On each node, run computation on GPU
    float *resultNode = NULL;
    computeGPU(dataNodeA, dataNodeB, resultNode, blockSize, gridSize);
  
    // Reduction to the root node, computing the sum of output elements
    float EucNode = sum(resultNode, dataSizePerNode);  // TODO
    
    float EucRoot;  // TODO
   
    char hostname[256];
    gethostname(hostname, 256);
    cout << "hostname: " << hostname << " local summation: " << EucNode << endl;
    MPI_CHECK(MPI_Reduce(&EucNode, &EucRoot, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD));  //TODO

    delete [] dataNodeA;
    delete [] dataNodeB;
 /*   if (commRank == 0)
    {
        float average = (sumRootA + sumRootB) / dataSizeTotal;
        cout << "Average of square roots is: " << average << endl;
    }
*/
    // Cleanup

    MPI_CHECK(MPI_Finalize());

    if (commRank == 0)
    {
        cout << "PASSED\n";
    }

    return 0;
}

// Shut down MPI cleanly if something goes wrong
void my_abort(int err)
{
    cout << "Test FAILED\n";
    MPI_Abort(MPI_COMM_WORLD, err);
}



#include <iostream>
#include <cuda.h>
#include <cuda_runtime.h>
using namespace std;

__global__ void add(int *A, int *B, int *C, int size) //kernel function executed on gpu
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x; //calculates global index of thread

    if (tid < size)
    {
        C[tid] = A[tid] + B[tid];
    }
}

void userInput(int *vector, int size)
{
    cout << "Enter " << size << " elements for the vector:\n";
    for (int i = 0; i < size; i++)
    {
        cout << "Element " << i + 1 << ": ";
        cin >> vector[i];
    }
}

void print(int *vector, int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << vector[i] << " ";
    }
    cout << endl;
}

int main()
{
    int N;
    int *A, *B, *C;
    cout << "Enter the size of the vector" << endl;
    cin >> N;

    int vectorSize = N;
    size_t vectorBytes = vectorSize * sizeof(int);

    A = new int[vectorSize];
    B = new int[vectorSize];
    C = new int[vectorSize];

    userInput(A, vectorSize);
    userInput(B, vectorSize);

    cout << "Vector A: ";
    print(A, N);
    cout << "Vector B: ";
    print(B, N);

    int *X, *Y, *Z;

    cudaMalloc(&X, vectorBytes);
    cudaMalloc(&Y, vectorBytes);
    cudaMalloc(&Z, vectorBytes);

    cudaMemcpy(X, A, vectorBytes, cudaMemcpyHostToDevice);
    cudaMemcpy(Y, B, vectorBytes, cudaMemcpyHostToDevice);

    // Sequential execution
    double sequential_start = clock();
    for (int i = 0; i < N; i++)
    {
        C[i] = A[i] + B[i];
    }
    double sequential_end = clock();
    // Print vector sequential
    cout << "\nAddition of vectors when added sequentially - ";
    print(C, N);
    
    // Print execution times
    double seq_time = (sequential_end - sequential_start) / CLOCKS_PER_SEC * 1000.0;
    cout << "Sequential Time is - " << seq_time << "ms\n\n";

    // Parallel execution
    int threadsPerBlock = 256;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;
    double parallel_start = clock();
    add<<<blocksPerGrid, threadsPerBlock>>>(X, Y, Z, N);
    cudaDeviceSynchronize();
    double parallel_end = clock();

    // Copy data back from device to host (optional for verification)
    cudaMemcpy(C, Z, vectorBytes, cudaMemcpyDeviceToHost);
    // Print vector parallel
    cout << "\nAddition of vectors when added parallely - ";
    print(C, N);
    
    // Print execution times
    double par_time = (parallel_end - parallel_start) / CLOCKS_PER_SEC * 1000.0;
    cout << "Parallel Time is - " << par_time << "ms";

    delete[] A;
    delete[] B;
    delete[] C;

    cudaFree(X);
    cudaFree(Y);
    cudaFree(Z);

    return 0;
}

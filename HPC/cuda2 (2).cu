#include <iostream>
#include <cuda.h>
#include <vector>
#include <cuda_runtime.h>
using namespace std;

// CUDA code to multiply matrices
__global__ void multiply(int* A, int* B, int* C, int size) {
    // Uses thread indices and block indices to compute each element
    //computes the row and column indices of the element to be computed using the thread indices
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < size && col < size) {
        int sum = 0;
        for (int i = 0; i < size; i++) {
            sum += A[row * size + i] * B[i * size + col];//computes the dot product of the rowth row of matrix A and the colth column of matrix B
        }
        C[row * size + col] = sum;
    }
}

void initialize(vector<vector<int>>& matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            cout << "Enter the elements of the matrix (" << size << "x" << size << "):\n";
            cin >> matrix[row][col];
        }
    }
}

void print(int* matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            cout << matrix[row * size + col] << " ";
        }
        cout << '\n';
    }
    cout << '\n';
}

int main() {
    int* A, * B, * C;

    int N;
    cout << "Enter the size of the square matrices: ";
    cin >> N;

    vector<vector<int>> A_vec(N, vector<int>(N));
    vector<vector<int>> B_vec(N, vector<int>(N));
    vector<vector<int>> C_vec(N, vector<int>(N));

    initialize(A_vec, N);
    initialize(B_vec, N);
    cout << "Matrix A: \n";
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            cout << A_vec[row][col] << " ";
        }
        cout << '\n';
    }
    cout << '\n';

    cout << "Matrix B: \n";
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            cout << B_vec[row][col] << " ";
        }
        cout << '\n';
    }
    cout << '\n';

    int matrixSize = N * N;
    size_t matrixBytes = matrixSize * sizeof(int);

    A = new int[matrixSize];
    B = new int[matrixSize];
    C = new int[matrixSize];

    // Copy data from vectors to arrays
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = A_vec[i][j];
            B[i * N + j] = B_vec[i][j];
        }
    }

    int* X, * Y, * Z;
    // Allocate space
    cudaMalloc(&X, matrixBytes);
    cudaMalloc(&Y, matrixBytes);
    cudaMalloc(&Z, matrixBytes);

    // Copy values from A to X
    cudaMemcpy(X, A, matrixBytes, cudaMemcpyHostToDevice);

    // Copy values from A to X and B to Y
    cudaMemcpy(Y, B, matrixBytes, cudaMemcpyHostToDevice);

    // Threads per CTA dimension
    int THREADS = 2;

    // Blocks per grid dimension (assumes THREADS divides N evenly)
    int BLOCKS = N / THREADS;

    // Use dim3 structs for block  and grid dimensions
    dim3 threads(THREADS, THREADS);
    dim3 blocks(BLOCKS, BLOCKS);

    // Start timer for parallel multiplication
    double parallel_start = clock();

    // Launch kernel
    multiply<<<blocks, threads>>>(X, Y, Z, N);

    // End timer for parallel multiplication
    double parallel_end = clock();
    double par_time = (parallel_end - parallel_start) / CLOCKS_PER_SEC * 1000.0;

    // Copy result matrix from device to host
    cudaMemcpy(C, Z, matrixBytes, cudaMemcpyDeviceToHost);

    cout << "Parallel Matrix Multiplication of matrix A and B: \n";
    print(C, N);
    cout << "Parallel Multiplication Time: " << par_time << " ms\n\n" << endl;


    // Start timer for sequential multiplication
    double sequential_start = clock();

    // Perform sequential multiplication
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A_vec[i][k] * B_vec[k][j];
            }
            C_vec[i][j] = sum;
        }
    }

    // End timer for sequential multiplication
    double sequential_end = clock();
    double seq_time = (sequential_end - sequential_start) / CLOCKS_PER_SEC * 1000.0;

    cout << "Sequential Matrix Multiplication of matrix A and B: \n";
    print(C, N);
    cout << "Sequential Multiplication Time: " << seq_time << " ms" << endl;

    delete[] A;
    delete[] B;
    delete[] C;

    cudaFree(X);
    cudaFree(Y);
    cudaFree(Z);

    return 0;
}

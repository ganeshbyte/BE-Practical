
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#define N 1024 // Define the size of vectors and matrices

// CUDA kernel for vector addition
__global__ void vectorAdd(int *a, int *b, int *c) {
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    if (index < N)
        c[index] = a[index] + b[index];
}

// CUDA kernel for matrix multiplication
__global__ void matrixMul(int *a, int *b, int *c) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int sum = 0;
    if (row < N && col < N) {
        for (int k = 0; k < N; ++k) {
            sum += a[row * N + k] * b[k * N + col];
        }
        c[row * N + col] = sum;
    }
}

int main() {
    int *a, *b, *c; // Host vectors and matrices
    int *d_a, *d_b, *d_c; // Device vectors and matrices

    // Allocate memory for host vectors and matrices
    a = (int*)malloc(N * N * sizeof(int));
    b = (int*)malloc(N * N * sizeof(int));
    c = (int*)malloc(N * N * sizeof(int));

    // Initialize host vectors and matrices
    for (int i = 0; i < N * N; ++i) {
        a[i] = 1;
        b[i] = 2;
    }

    // Allocate memory for device vectors and matrices
    cudaMalloc((void**)&d_a, N * N * sizeof(int));
    cudaMalloc((void**)&d_b, N * N * sizeof(int));
    cudaMalloc((void**)&d_c, N * N * sizeof(int));

    // Copy host vectors and matrices to device
    cudaMemcpy(d_a, a, N * N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, N * N * sizeof(int), cudaMemcpyHostToDevice);

    // Define block and grid dimensions for vector addition
    dim3 block_dim(256);
    dim3 grid_dim((N + block_dim.x - 1) / block_dim.x, 1);

    // Launch vector addition kernel
    vectorAdd<<<grid_dim, block_dim>>>(d_a, d_b, d_c);

    // Copy result back to host for vector addition
    cudaMemcpy(c, d_c, N * N * sizeof(int), cudaMemcpyDeviceToHost);

    // Print result of vector addition
    printf("Vector Addition Result:\n");
    for (int i = 0; i < N * N; ++i) {
        printf("%d ", c[i]);
    }
    printf("\n");

    // Define block and grid dimensions for matrix multiplication
    dim3 block_dim_matrix(16, 16);
    dim3 grid_dim_matrix((N + block_dim_matrix.x - 1) / block_dim_matrix.x, (N + block_dim_matrix.y - 1) / block_dim_matrix.y);

    // Launch matrix multiplication kernel
    matrixMul<<<grid_dim_matrix, block_dim_matrix>>>(d_a, d_b, d_c);

    // Copy result back to host for matrix multiplication
    cudaMemcpy(c, d_c, N * N * sizeof(int), cudaMemcpyDeviceToHost);

    // Print result of matrix multiplication
    printf("Matrix Multiplication Result:\n");
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%d ", c[i * N + j]);
        }
        printf("\n");
    }

    // Free device memory
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    // Free host memory
    free(a);
    free(b);
    free(c);

    return 0;
}

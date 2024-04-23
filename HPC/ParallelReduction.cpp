#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Function to find the minimum element in an array using parallel reduction
int parallelMin(const vector<int>& arr) {
    int min_val = arr[0];

    #pragma omp parallel for reduction(min:min_val)
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }

    return min_val;
}

// Function to find the maximum element in an array using parallel reduction
int parallelMax(const vector<int>& arr) {
    int max_val = arr[0];

    #pragma omp parallel for reduction(max:max_val)
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }

    return max_val;
}

// Function to compute the sum of elements in an array using parallel reduction
int parallelSum(const vector<int>& arr) {
    int sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < arr.size(); ++i) {
        sum += arr[i];
    }

    return sum;
}

// Function to compute the average of elements in an array using parallel reduction
double parallelAverage(const vector<int>& arr) {
    int sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < arr.size(); ++i) {
        sum += arr[i];
    }

    return (double)(sum) / arr.size();
}

int main() {
    vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
    cout << "Array: "; 
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    cout << "Minimum: " << parallelMin(arr) << endl;
    cout << "Maximum: " << parallelMax(arr) << endl;
    cout << "Sum: " << parallelSum(arr) << endl;
    cout << "Average: " << parallelAverage(arr) << endl;

    return 0;
}
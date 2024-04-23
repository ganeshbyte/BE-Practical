#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i) {
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; ++j) {
        R[j] = arr[m + 1 + j];
    }

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped = true;
    while (swapped) {
        swapped = false;

        #pragma omp parallel for shared(arr, swapped) 
        for (int i = 0; i < n - 1; ++i) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
    }
}

void parallelMergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelMergeSort(arr, l, m);
            }
            #pragma omp section
            {
                parallelMergeSort(arr, m + 1, r);
            }
        }
        merge(arr, l, m, r);
    }
}

int main() {
    vector<int> arr_bubble = {64, 34, 25, 12, 22, 11, 90, 20, 32, 56, 5, 40};
    vector<int> arr_merge = {64, 34, 25, 12, 22, 11, 90, 20, 32, 56, 5, 40};
    int n = arr_bubble.size();

    cout << "Original array for Bubble Sort: ";
    for (int num : arr_bubble) {
        cout << num << " ";
    }
    cout << endl;

    parallelBubbleSort(arr_bubble);

    cout << "Sorted array using Bubble Sort: ";
    for (int num : arr_bubble) {
        cout << num << " ";
    }
    cout << endl;

    cout << "Original array for Merge Sort: ";
    for (int num : arr_merge) {
        cout << num << " ";
    }
    cout << endl;

    parallelMergeSort(arr_merge, 0, n - 1);

    cout << "Sorted array using Merge Sort: ";
    for (int num : arr_merge) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}
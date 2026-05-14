#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm> // reverse
using namespace std;

using Clock = chrono::high_resolution_clock;

// Estructura para métricas
struct Metrics {
    long long comparisons = 0;
    long long swaps = 0;
};

// ======================= BUBBLE SORT =======================
void bubbleSort(vector<int>& arr, Metrics& m) {
    int n = arr.size();
    bool swapped;

    for (int i = 0; i < n - 1; i++) {
        swapped = false;

        for (int j = 0; j < n - i - 1; j++) {
            m.comparisons++;
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                m.swaps++;
                swapped = true;
            }
        }

        if (!swapped) break;
    }
}

// ======================= INSERTION SORT =======================
void insertionSort(vector<int>& arr, Metrics& m) {
    int n = arr.size();

    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0) {
            m.comparisons++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                m.swaps++;
                j--;
            } else break;
        }

        arr[j + 1] = key;
    }
}

// ======================= MERGE SORT =======================
void merge(vector<int>& arr, int left, int mid, int right, Metrics& m) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        m.comparisons++;
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
        m.swaps++;
    }

    while (i < n1) {
        arr[k++] = L[i++];
        m.swaps++;
    }

    while (j < n2) {
        arr[k++] = R[j++];
        m.swaps++;
    }
}

void mergeSortRec(vector<int>& arr, int left, int right, Metrics& m) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSortRec(arr, left, mid, m);
        mergeSortRec(arr, mid + 1, right, m);
        merge(arr, left, mid, right, m);
    }
}

void mergeSort(vector<int>& arr, Metrics& m) {
    mergeSortRec(arr, 0, arr.size() - 1, m);
}

// ======================= QUICK SORT =======================
int partition(vector<int>& arr, int low, int high, Metrics& m) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        m.comparisons++;
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
            m.swaps++;
        }
    }

    swap(arr[i + 1], arr[high]);
    m.swaps++;
    return i + 1;
}

void quickSortRec(vector<int>& arr, int low, int high, Metrics& m) {
    if (low < high) {
        int pi = partition(arr, low, high, m);
        quickSortRec(arr, low, pi - 1, m);
        quickSortRec(arr, pi + 1, high, m);
    }
}

void quickSort(vector<int>& arr, Metrics& m) {
    quickSortRec(arr, 0, arr.size() - 1, m);
}

// ======================= TEST =======================
void test(vector<int> data,
          void(*func)(vector<int>&, Metrics&),
          const string& name) {

    vector<int> arr = data;
    Metrics m;

    auto t1 = Clock::now();
    func(arr, m);
    auto t2 = Clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << name
         << " | Tiempo: " << duration << " μs"
         << " | Comparaciones: " << m.comparisons
         << " | Intercambios: " << m.swaps << "\n";
}

// ======================= MAIN =======================
int main() {
    srand(time(0));

    int n = 1000;

    vector<int> randomData(n);
    for (int i = 0; i < n; i++) randomData[i] = rand() % 10000;

    vector<int> sortedData = randomData;
    sort(sortedData.begin(), sortedData.end());

    vector<int> reverseData = sortedData;
    reverse(reverseData.begin(), reverseData.end());

    cout << "===== DATOS ALEATORIOS =====\n";
    test(randomData, bubbleSort,   "Bubble Sort   ");
    test(randomData, insertionSort,"Insertion Sort");
    test(randomData, mergeSort,    "Merge Sort    ");
    test(randomData, quickSort,    "Quick Sort    ");

    cout << "\n===== DATOS ORDENADOS =====\n";
    test(sortedData, bubbleSort,   "Bubble Sort   ");
    test(sortedData, insertionSort,"Insertion Sort");
    test(sortedData, mergeSort,    "Merge Sort    ");
    test(sortedData, quickSort,    "Quick Sort    ");

    cout << "\n===== DATOS INVERSOS =====\n";
    test(reverseData, bubbleSort,   "Bubble Sort   ");
    test(reverseData, insertionSort,"Insertion Sort");
    test(reverseData, mergeSort,    "Merge Sort    ");
    test(reverseData, quickSort,    "Quick Sort    ");

    return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ===================== COUNTING SORT =====================
void countingSort(vector<int>& arr, int maxVal) {
    vector<int> count(maxVal + 1, 0);
    vector<int> output(arr.size());

    for (int num : arr) count[num]++;

    for (int i = 1; i <= maxVal; i++)
        count[i] += count[i - 1];

    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }

    arr = output;
}

// ===================== RADIX SORT =====================
void countingSortRadix(vector<int>& arr, int exp) {
    int n = arr.size();
    vector<int> output(n);
    int count[10] = {0};

    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    arr = output;
}

void radixSort(vector<int>& arr) {
    int maxVal = *max_element(arr.begin(), arr.end());

    for (int exp = 1; maxVal / exp > 0; exp *= 10)
        countingSortRadix(arr, exp);
}

// ===================== BUCKET SORT =====================
void bucketSort(vector<float>& arr) {
    int n = arr.size();
    vector<vector<float>> buckets(n);

    for (float num : arr) {
        int idx = n * num;
        buckets[idx].push_back(num);
    }

    for (int i = 0; i < n; i++)
        sort(buckets[i].begin(), buckets[i].end());

    int index = 0;
    for (int i = 0; i < n; i++)
        for (float num : buckets[i])
            arr[index++] = num;
}

// ===================== PIGEONHOLE SORT =====================
void pigeonholeSort(vector<int>& arr) {
    int minVal = *min_element(arr.begin(), arr.end());
    int maxVal = *max_element(arr.begin(), arr.end());
    int size = maxVal - minVal + 1;

    vector<int> holes(size, 0);

    for (int num : arr)
        holes[num - minVal]++;

    int index = 0;
    for (int i = 0; i < size; i++)
        while (holes[i]--)
            arr[index++] = i + minVal;
}

// ===================== FLASH SORT =====================
void flashSort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;

    int minVal = arr[0], maxIdx = 0;

    for (int i = 1; i < n; i++) {
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > arr[maxIdx]) maxIdx = i;
    }

    if (minVal == arr[maxIdx]) return;

    int m = int(0.45 * n);
    vector<int> L(m, 0);

    for (int i = 0; i < n; i++) {
        int k = (m - 1) * (arr[i] - minVal) / (arr[maxIdx] - minVal);
        L[k]++;
    }

    for (int i = 1; i < m; i++)
        L[i] += L[i - 1];

    swap(arr[maxIdx], arr[0]);

    int move = 0, j = 0, k = m - 1;

    while (move < n - 1) {
        while (j > L[k] - 1) {
            j++;
            k = (m - 1) * (arr[j] - minVal) / (arr[maxIdx] - minVal);
        }

        int flash = arr[j];

        while (j != L[k]) {
            k = (m - 1) * (flash - minVal) / (arr[maxIdx] - minVal);
            int temp = arr[L[k] - 1];
            arr[L[k] - 1] = flash;
            flash = temp;
            L[k]--;
            move++;
        }
    }

    // Inserción final
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// ===================== TIMSORT (SIMPLIFICADO) =====================
const int RUN = 32;

void insertionSort(vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int temp = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > temp) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);

    int i = 0, j = 0, k = l;

    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j])
            arr[k++] = left[i++];
        else
            arr[k++] = right[j++];
    }

    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}

void timSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; i += RUN)
        insertionSort(arr, i, min(i + RUN - 1, n - 1));

    for (int size = RUN; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = min((left + 2 * size - 1), (n - 1));

            if (mid < right)
                merge(arr, left, mid, right);
        }
    }
}

// ===================== MAIN =====================
int main() {

    vector<int> arr1 = {5, 2, 1, 7, 3, 2, 5, 0};
    countingSort(arr1, 7);

    cout << "Counting Sort: ";
    for (int x : arr1) cout << x << " ";
    cout << endl;

    vector<int> arr2 = {170, 45, 75, 90, 802, 24, 2, 66};
    radixSort(arr2);

    cout << "Radix Sort: ";
    for (int x : arr2) cout << x << " ";
    cout << endl;

    vector<int> arr3 = {8, 3, 2, 7, 4, 6, 8};
    pigeonholeSort(arr3);

    cout << "Pigeonhole Sort: ";
    for (int x : arr3) cout << x << " ";
    cout << endl;

    vector<int> arr4 = {4, 2, 7, 3, 1, 6};
    flashSort(arr4);

    cout << "Flash Sort: ";
    for (int x : arr4) cout << x << " ";
    cout << endl;

    vector<int> arr5 = {5, 21, 7, 23, 19};
    timSort(arr5);

    cout << "TimSort: ";
    for (int x : arr5) cout << x << " ";
    cout << endl;

    vector<float> arr6 = {0.42, 0.32, 0.23, 0.52, 0.25, 0.47};
    bucketSort(arr6);

    cout << "Bucket Sort: ";
    for (float x : arr6) cout << x << " ";
    cout << endl;

    return 0;
}
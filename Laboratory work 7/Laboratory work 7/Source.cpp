#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

// Последовательная сортировка Шелла
void shellSort(vector<double>& arr) {
    int size = arr.size();

    for (int gap = size / 2; gap > 0; gap /= 2) {
        for (int currentIndex = gap; currentIndex < size; currentIndex++) {
            double currentValue = arr[currentIndex];
            int previousIndex;

            for (previousIndex = currentIndex; previousIndex >= gap && arr[previousIndex - gap] > currentValue; previousIndex -= gap) {
                arr[previousIndex] = arr[previousIndex - gap];
            }

            arr[previousIndex] = currentValue;
        }
    }
}

void insertSort(vector<double>& arr, int start, int gap) {
    for (int currentIndex = start + gap; currentIndex < arr.size(); currentIndex += gap) {
        double currentValue = arr[currentIndex];
        int previousIndex = currentIndex;

        while (previousIndex >= gap && arr[previousIndex - gap] > currentValue) {
            arr[previousIndex] = arr[previousIndex - gap];
            previousIndex -= gap;
        }

        arr[previousIndex] = currentValue;
    }
}

// Параллельная сортировка Шелла
void parallelShellSort(vector<double>& arr) {
    int size = arr.size();

    for (int gap = size / 2; gap > 0; gap /= 2) {
        #pragma omp parallel for
        for (int i = 0; i < gap; ++i) {
            insertSort(arr, i, gap);
        }
    }
}

bool isSorted(const vector<double>& arr) {
    for (int i = 0; i < arr.size() - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            cout << "Массив не отсортирован!" << endl;
            return false;
        }
    }

    cout << "Массив отсортирован!" << endl;
    return true;
}

int main() {
    setlocale(LC_ALL, "Rus");

    const int size = 10000000;
    vector<double> arr(size);
    vector<double> arrCopy(size);

    srand(unsigned(time(nullptr)));
    for (int i = 0; i < size; i++) {
        arr[i] = static_cast<double>(rand()) / RAND_MAX;
    }

    arrCopy = arr;

    double start = omp_get_wtime();
    shellSort(arrCopy);
    double end = omp_get_wtime();
    cout << "Время последовательной сортировки Шелла: " << end - start << " секунд\n";
    isSorted(arrCopy);

    start = omp_get_wtime();
    parallelShellSort(arr);
    end = omp_get_wtime();
    cout << "Время параллельной сортировки Шелла: " << end - start << " секунд\n";
    isSorted(arr);

    return 0;
}
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

// Последовательная сортировка Шелла
void shellSort(vector<double>& arr) {
    int size = arr.size();

    // Начинаем с большого шага, затем уменьшаем его
    for (int gap = size / 2; gap > 0; gap /= 2) {
        // Применяем сортировку вставками для каждого подмассива
        for (int currentIndex = gap; currentIndex < size; currentIndex++) {
            double currentValue = arr[currentIndex];
            int previousIndex;

            // Сдвигаем элементы, пока не найдем правильную позицию для currentValue
            for (previousIndex = currentIndex; previousIndex >= gap && arr[previousIndex - gap] > currentValue; previousIndex -= gap) {
                arr[previousIndex] = arr[previousIndex - gap];
            }

            // Вставляем currentValue на правильную позицию
            arr[previousIndex] = currentValue;
        }
    }
}

// Параллельная сортировка Шелла
void parallelShellSort(vector<double>& arr) {
    int size = arr.size();

    // Начинаем с большого шага, затем уменьшаем его
    for (int gap = size / 2; gap > 0; gap /= 2) {
        // Анализатор кода Visual Studio не способен анализировать параллельные регионы OpenMP,
        // так как они обрабатываются на этапе выполнения, а не компиляции - поэтому получаем предупреждение
        #pragma omp parallel for
        for (int currentIndex = gap; currentIndex < size; currentIndex++) {
            double currentValue = arr[currentIndex];
            int previousIndex;

            // Сдвигаем элементы, пока не найдем правильную позицию для currentValue
            for (previousIndex = currentIndex; previousIndex >= gap && arr[previousIndex - gap] > currentValue; previousIndex -= gap) {
                arr[previousIndex] = arr[previousIndex - gap];
            }

            // Вставляем currentValue на правильную позицию
            arr[previousIndex] = currentValue;
        }
    }
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

    start = omp_get_wtime();
    parallelShellSort(arr);
    end = omp_get_wtime();
    cout << "Время параллельной сортировки Шелла: " << end - start << " секунд\n";

    return 0;
}
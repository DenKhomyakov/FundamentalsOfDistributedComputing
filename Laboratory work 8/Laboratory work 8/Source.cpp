#include <iostream>
#include <cmath>
#include <omp.h>

using namespace std;

double f(double x) {
    return 3 - 2 * x - x * x;
}

double integrate(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        double x = a + i * h;
        sum += f(x);
    }

    return sum * h;
}

double integrateParallel(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;

    // Анализатор кода Visual Studio не способен анализировать параллельные регионы OpenMP,
    // так как они обрабатываются на этапе выполнения, а не компиляции - поэтому получаем предупреждение
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++) {
        cout << omp_get_num_threads() << endl;
        double x = a + i * h;
        sum += f(x);
    }

    return sum * h;
}

int main() {
    setlocale(LC_ALL, "Rus");

    double a = -2.0;
    double b = 1.0;
    int n = 10000000000;

    double start;
    double end;

    start = omp_get_wtime();
    double sequenceResult = integrate(a, b, n);
    end = omp_get_wtime();
    cout << "Результат последовательного вычисления: " << sequenceResult << endl;
    cout << "Последовательное время: " << end - start << " секунд" << endl;

    start = omp_get_wtime();
    double parallelResult = integrateParallel(a, b, n);
    end = omp_get_wtime();
    cout << "Результат параллельного вычисления: " << parallelResult << endl;
    cout << "Параллельное время: " << end - start << " секунд" << endl;

    return 0;
}
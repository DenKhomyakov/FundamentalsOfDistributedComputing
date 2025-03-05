#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

#define MATRIX_SIZE 1000

void fillMatrix(double** matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = static_cast<double>(rand()) / RAND_MAX;
        }
    }
}

// schedule (static) - итерации цикла распределяются между потоками статически (равномерно)
void multiplyStatic(double** matrix1, double** matrix2, double** resultMatrix, int matrixSize, int chunkSize) {
    #pragma warning(disable : 6993)
    #pragma omp parallel for schedule(static, chunkSize)
    for (int i = 0; i < matrixSize; i++) {

        for (int j = 0; j < matrixSize; j++) {
            resultMatrix[i][j] = 0;

            for (int k = 0; k < matrixSize; k++) {
                resultMatrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

// schedule (dynamic) - итерации распределяются динамически (потоки берут новые итерации по мере завершения предыдущих)
void multiplyDynamic(double** matrix1, double** matrix2, double** resultMatrix, int matrixSize, int chunkSize) {
    #pragma omp parallel for schedule(dynamic, chunkSize)
    for (int i = 0; i < matrixSize; i++) {

        for (int j = 0; j < matrixSize; j++) {
            resultMatrix[i][j] = 0;

            for (int k = 0; k < matrixSize; k++) {
                resultMatrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

// schedule (guided) - итерации распределяются динамически, но размер блока уменьшается по мере выполнения
void multiplyGuided(double** matrix1, double** matrix2, double** resultMatrix, int matrixSize, int chunkSize) {
    #pragma omp parallel for schedule(guided, chunkSize)
    for (int i = 0; i < matrixSize; i++) {

        for (int j = 0; j < matrixSize; j++) {
            resultMatrix[i][j] = 0;

            for (int k = 0; k < matrixSize; k++) {
                resultMatrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

// schedule (runtime) - OpenMP сам выбирает стратегию (тип распределения и размер блока)
void multiplyRuntime(double** matrix1, double** matrix2, double** resultMatrix, int matrixSize) {
    #pragma omp parallel for schedule(runtime)
    for (int i = 0; i < matrixSize; i++) {

        for (int j = 0; j < matrixSize; j++) {
            resultMatrix[i][j] = 0;

            for (int k = 0; k < matrixSize; k++) {
                resultMatrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

int main() {
    setlocale(LC_ALL, "Rus");

    double** matrix1 = new double* [MATRIX_SIZE];
    double** matrix2 = new double* [MATRIX_SIZE];
    double** resultMatrix = new double* [MATRIX_SIZE];

    for (int i = 0; i < MATRIX_SIZE; i++) {
        matrix1[i] = new double[MATRIX_SIZE];
        matrix2[i] = new double[MATRIX_SIZE];
        resultMatrix[i] = new double[MATRIX_SIZE];
    }

    srand(time(nullptr));
    fillMatrix(matrix1, MATRIX_SIZE);
    fillMatrix(matrix2, MATRIX_SIZE);

    const char* scheduleTypes[] = { "static", "dynamic", "guided", "runtime" };
    int numThreadsOptions[] = { 1, 2, 4, 8 };
    int chunkSize = 100;

    for (const char* scheduleType : scheduleTypes) {
        for (int numThreads : numThreadsOptions) {
            omp_set_num_threads(numThreads);

            double start = omp_get_wtime();

            if (strcmp(scheduleType, "static") == 0) {
                multiplyStatic(matrix1, matrix2, resultMatrix, MATRIX_SIZE, chunkSize);
            } else if (strcmp(scheduleType, "dynamic") == 0) {
                multiplyDynamic(matrix1, matrix2, resultMatrix, MATRIX_SIZE, chunkSize);
            } else if (strcmp(scheduleType, "guided") == 0) {
                multiplyGuided(matrix1, matrix2, resultMatrix, MATRIX_SIZE, chunkSize);
            } else if (strcmp(scheduleType, "runtime") == 0) {
                multiplyRuntime(matrix1, matrix2, resultMatrix, MATRIX_SIZE);
            }

            double end = omp_get_wtime();
            cout << "Schedule: " << scheduleType << ", Потоки: " << numThreads << ", Время: " << end - start << " секунд" << endl;
        }

        cout << endl;
    }

    for (int i = 0; i < MATRIX_SIZE; i++) {
        delete[] matrix1[i];
        delete[] matrix2[i];
        delete[] resultMatrix[i];
    }
    delete[] matrix1;
    delete[] matrix2;
    delete[] resultMatrix;

    return 0;
}
#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

int main() {
	setlocale(LC_ALL, "Rus");

	const int vectorSize{ 1000000 };

	vector<double> vector1(vectorSize, 1.0);
	vector<double> vector2(vectorSize, 2.0);

	double scalarValue{ 0.0 };


	// ƒиректива указывает на то, что цикл for должен быть выполнен параллельно
	// reduction(+:scalarValue) означает, что переменна€ scalarValue будет суммироватьс€ в каждом потоке,
	// а затем результаты из всех потоков будут объединены (сложены) в конце
	#pragma omp parallel for reduction(+:scalarValue)
	for (int i{}; i < vectorSize; ++i) {
		//cout << omp_get_num_threads() << endl;
		scalarValue += vector1[i] * vector2[i];
	}

	cout << "—кал€рное произведение: " << scalarValue << endl;

	return 0;
}
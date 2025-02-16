#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <random>

using namespace std;

/*
ѕри использовании rand() возникает проблема: генератор случайных числе ициниализируетс€ один раз в главном потоке при помощи srand()
ѕоскольку потоки запускаютс€ практически одновременно, они могут получать одинаковые значени€ от rand(), что приводит к дублированию
последовательностей
*/

mutex mtx;
vector<double> resultArray;

void generateArrayNumber(int start, int end, double maxValue) {
	random_device entropy;	// »сточник хаоса (энтропии) дл€ инициализации генератора
	mt19937 generator(entropy());	// √енератор случайных чисел (вихрь ћерсенна)
	uniform_real_distribution<> distribution(0.0, maxValue); // –аспределение дл€ вещественных чисел

	for (int i{ start }; i < end; ++i) {
		double pseudorandomValue = distribution(generator);
		//double pseudorandomValue = (static_cast<double>(rand()) / RAND_MAX) * maxValue;

		lock_guard<mutex> lock(mtx);
		resultArray[i] = pseudorandomValue;
	}
}

int main() {
	setlocale(LC_ALL, "Rus");

	cout << "¬ведите количество чисел (n): ";
	int n{};
	cin >> n;

	cout << "¬ведите количество потоков (k): ";
	int k{};
	cin >> k;

	cout << "¬ведите максимальное значение дл€ генерации: ";
	double maxValue{};
	cin >> maxValue;

	resultArray.resize(n);

	//srand(static_cast<unsigned int>(time(nullptr)));

	vector<thread> threads;
	int blockSize{ n / k };
	int remainder{ n % k };

	for (int i{}; i < k; ++i) {
		int start = i * blockSize;
		int end = start + blockSize;

		if (i == k - 1) {
			end += remainder;
		}

		threads.emplace_back(generateArrayNumber, start, end, maxValue);
	}

	for (auto& th : threads) {
		th.join();
	}

	cout << "¬аш массив: " << endl;
	for (auto& number : resultArray) {
		cout << number << " ";
	}
	cout << endl;

	return 0;
}
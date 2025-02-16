#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <random>

using namespace std;

/*
��� ������������� rand() ��������� ��������: ��������� ��������� ����� ���������������� ���� ��� � ������� ������ ��� ������ srand()
��������� ������ ����������� ����������� ������������, ��� ����� �������� ���������� �������� �� rand(), ��� �������� � ������������
�������������������
*/

mutex mtx;
vector<double> resultArray;

void generateArrayNumber(int start, int end, double maxValue) {
	random_device entropy;	// �������� ����� (��������) ��� ������������� ����������
	mt19937 generator(entropy());	// ��������� ��������� ����� (����� ��������)
	uniform_real_distribution<> distribution(0.0, maxValue); // ������������� ��� ������������ �����

	for (int i{ start }; i < end; ++i) {
		double pseudorandomValue = distribution(generator);
		//double pseudorandomValue = (static_cast<double>(rand()) / RAND_MAX) * maxValue;

		lock_guard<mutex> lock(mtx);
		resultArray[i] = pseudorandomValue;
	}
}

int main() {
	setlocale(LC_ALL, "Rus");

	cout << "������� ���������� ����� (n): ";
	int n{};
	cin >> n;

	cout << "������� ���������� ������� (k): ";
	int k{};
	cin >> k;

	cout << "������� ������������ �������� ��� ���������: ";
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

	cout << "��� ������: " << endl;
	for (auto& number : resultArray) {
		cout << number << " ";
	}
	cout << endl;

	return 0;
}
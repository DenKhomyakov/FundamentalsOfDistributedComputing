#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex mtx;

void finalCoutdown(int number, int threadNumber) {
	for (int i{ number }; i >= 0; --i) {
        mtx.lock();
        cout << "Поток № " << threadNumber << " - число " << i << endl;
        mtx.unlock();

        this_thread::sleep_for(1000ms);
	}
}

int main() {
	setlocale(LC_ALL, "Rus");

	cout << "Введите число: ";
	int number{ 0 };
	cin >> number;

    thread thread1(finalCoutdown, number, 1);
    thread thread2(finalCoutdown, number, 2);
    thread thread3(finalCoutdown, number, 3);

    thread1.join();
    thread2.join();
    thread3.join();

	return 0;
}
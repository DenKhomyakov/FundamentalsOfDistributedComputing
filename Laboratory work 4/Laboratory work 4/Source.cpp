#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

using namespace std;

class BarrierSynchronization {
private:
	mutex barrierMutex;
	condition_variable conditionVarriable;
	size_t threadCount;
	size_t count;
	size_t generation;

public:
	explicit BarrierSynchronization(size_t threadCount) : threadCount(threadCount), count(0), generation(0) {}

	void waiting() {
		unique_lock<mutex> lock(barrierMutex);
		auto localGeneration = generation;

		if (++count == threadCount) {
			count = 0;
			++generation;

			conditionVarriable.notify_all();
		} else {
			while (generation == localGeneration) {
				conditionVarriable.wait(lock);
			}
		}
	}
};

mutex coutMutex;

void threadFunction(BarrierSynchronization& barrierSynchronization, int id) {
	{
		lock_guard<mutex> guard(coutMutex);
		cout << "Поток " << id << " выполняет работу" << endl;
	}
	this_thread::sleep_for(100ms);

	{
		lock_guard<mutex> guard(coutMutex);
		cout << "Поток " << id << " ожидает на барьере" << endl;
	}
	barrierSynchronization.waiting();

	{
		lock_guard<mutex> guard(coutMutex);
		cout << "Поток " << id << " прошёл барьер" << endl;
	}
}

int main() {
	setlocale(LC_ALL, "Rus");

	const size_t threadsNumber = 5;
	BarrierSynchronization barrierSynchronization(threadsNumber);

	vector<thread> threads;

	for (size_t i{}; i < threadsNumber; ++i) {
		threads.emplace_back(threadFunction, ref(barrierSynchronization), i);
	}

	for (auto& th : threads) {
		th.join();
	}

	return 0;
}
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>

using namespace std;

template <typename T>
class ThreadSafeQueue {
private:
    queue<T> safeQueue;
    mutable mutex headMutex;
    mutable mutex tailMutex;
    condition_variable conditionalVariable;

public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;

    void push(const T& value) {
        lock_guard<mutex> lock(tailMutex);
        safeQueue.push(value);
        conditionalVariable.notify_one();
    }

    bool pop(T& value) {
        unique_lock<mutex> lock(headMutex);

        while (safeQueue.empty()) {
            conditionalVariable.wait(lock);
        }

        value = safeQueue.front();
        safeQueue.pop();

        return true;
    }

    T findMax() const {
        lock_guard<mutex> headLock(headMutex);
        lock_guard<mutex> tailLock(tailMutex);

        if (safeQueue.empty()) {
            throw runtime_error("������� �����");
        }

        T maxElement = safeQueue.front();
        queue<T> temp = safeQueue;

        while (!temp.empty()) {
            if (temp.front() > maxElement) {
                maxElement = temp.front();
            }

            temp.pop();
        }

        return maxElement;
    }

    T findMin() const {
        lock_guard<mutex> headLock(headMutex);
        lock_guard<mutex> tailLock(tailMutex);

        if (safeQueue.empty()) {
            throw runtime_error("������� �����");
        }

        T minElement = safeQueue.front();
        queue<T> temp = safeQueue;

        while (!temp.empty()) {
            if (temp.front() < minElement) {
                minElement = temp.front();
            }

            temp.pop();
        }

        return minElement;
    }

    void print() const {
        lock_guard<mutex> headLock(headMutex);
        lock_guard<mutex> tailLock(tailMutex);

        queue<T> temp = safeQueue;

        while (!temp.empty()) {
            cout << temp.front() << " ";
            temp.pop();
        }

        cout << endl;
    }
};

mutex coutMutex;

void producer(ThreadSafeQueue<int>& safeQueue, int id) {
    for (int i{}; i < 10; ++i) {
        safeQueue.push(i + id * 10);

        {
            lock_guard<mutex> lock(coutMutex);
            cout << "������������� " << id << " �������: " << i + id * 10 << endl;
        }

        this_thread::sleep_for(100ms);
    }
}

void consumer(ThreadSafeQueue<int>& safeQueue, int id) {
    int value;

    for (int i{}; i < 10; ++i) {
        if (safeQueue.pop(value)) {
            {
                lock_guard<mutex> lock(coutMutex);
                cout << "����������� " << id << " �����: " << value << endl;
            }
        }
    }
}

int main() {
    setlocale(LC_ALL, "Rus");

    ThreadSafeQueue<int> safeQueue;

    // ����������� ����������� ������������� ������ ��������� ������� � ������� � ������ �������
    thread producerThread1(producer, ref(safeQueue), 1);
    thread producerThread2(producer, ref(safeQueue), 2);
    thread consumerThread1(consumer, ref(safeQueue), 1);
    thread consumerThread2(consumer, ref(safeQueue), 2);

    producerThread1.join();
    producerThread2.join();
    consumerThread1.join();
    consumerThread2.join();

    cout << endl;

    safeQueue.push(17);
    safeQueue.push(-4);
    safeQueue.push(3);
    safeQueue.push(105);

    try {
        cout << "������������ �������: " << safeQueue.findMax() << endl;
        cout << "����������� �������: " << safeQueue.findMin() << endl;
    } catch (const runtime_error& ex) {
        cout << "������: " << ex.what() << endl;
    }

    cout << endl;

    cout << "���������� �������: ";
    safeQueue.print();

    cout << endl;

    safeQueue.push(15);
    cout << "������� ����� ���������� ��������: ";
    safeQueue.print();
    cout << endl;

    int poppedValue;
    if (safeQueue.pop(poppedValue)) {
        cout << "����������� �������: " << poppedValue << endl;
    }
    cout << "������� ����� �������� ��������: ";
    safeQueue.print();
    cout << endl;

    cout << "������������ ������ ��� ������ ��������:" << endl;
    ThreadSafeQueue<int> errorSafeQueue;

    try {
        cout << "������������ �������: " << errorSafeQueue.findMax() << endl;
        cout << "����������� �������: " << errorSafeQueue.findMin() << endl;
    } catch (const runtime_error& ex) {
        cout << "������: " << ex.what() << endl;
    }

    cout << endl;

    return 0;
}
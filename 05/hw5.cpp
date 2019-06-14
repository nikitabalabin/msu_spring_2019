#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>

std::mutex m;
std::condition_variable cv;

const int N = 1000000;
static std::atomic<int64_t> i {0};

void ping()
{
	std::unique_lock<std::mutex> lock(m);
	while (i < N)
	{
		while (i % 2)
			cv.wait(lock);

		std::cout << "ping" << std::endl;

		cv.notify_one();
		i++;
	}
}


void pong()
{
	std::unique_lock<std::mutex> lock(m);
	while (i < N)
	{
		while (!(i % 2))
			cv.wait(lock);

		std::cout << "pong" << std::endl;

		cv.notify_one();
		i++;
	}
}

int main()
{
	std::thread threadPing(ping), threadPong(pong);
	threadPing.join();
	threadPong.join();

	return 0;
}
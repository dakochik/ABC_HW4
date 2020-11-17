#include <utility>
#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <shared_mutex>

long long highestScore = NULL;
std::pair<unsigned long, unsigned long> result;
std::vector<long> array;
std::shared_timed_mutex mtx;

void threadFun(unsigned long sth)
{
	unsigned long i = sth;
	for (long y = i + 1; y < array.size(); ++y)
	{
		long long sum = array[i];
		for (unsigned int j = i + 1; j <= y; ++j)
		{
			sum += static_cast<long long>(array[j]) * static_cast<long long>(pow(-1, j - i));
		}
		std::unique_lock<std::shared_timed_mutex> lock(mtx);
		if ((highestScore == NULL) || (sum > highestScore))
		{
			result = std::pair<int, int>(i, y);
			highestScore = sum;
		}
	}
}

void countIndex(std::vector<long> array)
{
	std::vector<std::thread> threads;
	for (unsigned long i = 0; i < array.size(); i++)
	{
		std::thread thr(threadFun, i);
		threads.emplace_back(std::move(thr));
	}
	for (auto& thr : threads) {
		thr.join();
	}
}

int main(int argc, char* argv[])
{
	std::string vecStr;

	std::ifstream in;
	in.open(argv[1]);
	while (std::getline(in, vecStr))
	{
		array.push_back(std::stol(vecStr));
	}
	in.close();

	countIndex(array);

	std::ofstream out;
	out.open(argv[2]);
	out << result.first << std::endl;
	out << result.second << std::endl;
	out.close();
}

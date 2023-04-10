#include <iostream>
#include <cstdint>
#include <string>
#include <chrono>
#include <fstream>
#include <cstdlib>

#include "test.h"

using namespace std::chrono;

class PerformanceTest : public Test {
private:
	static constexpr uint64_t TEST_MAX = 1024 * 64;

	void get_test(uint64_t max)
	{
		for (uint64_t i = 0; i < max; ++i) {
			store.get(i);
		}
	}

	void put_test(uint64_t max, uint64_t size)
	{
		for (uint64_t i = 0; i < max; ++i) {
			store.put(i, std::string(size, 's'));
		}
	}

	void del_test(uint64_t max)
	{
		for (uint64_t i = 0; i < max; ++i) {
			store.del(i);
		}
	}

	void sub_conventional_test(uint64_t size, std::fstream& fs)
	{
		steady_clock::time_point start, end;
		duration<double> time_span;

		store.reset();
		constexpr uint64_t test_count = TEST_MAX;

		std::cout << "For value size " << size << " :" << std::endl;
		fs << size << '\t';

		std::cout << "--Put--" << std::endl;
		start = steady_clock::now();
		put_test(test_count, size);
		end = steady_clock::now();
		time_span = duration_cast<duration<double>>(end - start);
		std::cout << test_count << " puts takes " << time_span.count() << "s." << std::endl;
		std::cout << "Average delay : " << time_span.count() / test_count << "s." << std::endl;
		std::cout << "Throughput : " << test_count / time_span.count() << "/s." << std::endl;
		fs << time_span.count() / test_count << '\t' << test_count / time_span.count() << '\t';

		std::cout << "--Get--" << std::endl;
		start = steady_clock::now();
		get_test(test_count);
		end = steady_clock::now();
		time_span = duration_cast<duration<double>>(end - start);
		std::cout << test_count << " gets takes " << time_span.count() << "s." << std::endl;
		std::cout << "Average delay : " << time_span.count() / test_count << "s." << std::endl;
		std::cout << "Throughput : " << test_count / time_span.count() << "/s." << std::endl;
		fs << time_span.count() / test_count << '\t' << test_count / time_span.count() << '\t';

		std::cout << "--Delete--" << std::endl;
		start = steady_clock::now();
		del_test(test_count);
		end = steady_clock::now();
		time_span = duration_cast<duration<double>>(end - start);
		std::cout << test_count << " deletes takes " << time_span.count() << "s." << std::endl;
		std::cout << "Average delay : " << time_span.count() / test_count << "s." << std::endl;
		std::cout << "Throughput : " << test_count / time_span.count() << "/s." << std::endl;
		fs << time_span.count() / test_count << '\t' << test_count / time_span.count() << std::endl;
		
		std::cout << std::endl;
	}

	void conventional_test() 
	{
		std::cout << "<<Conventional Test>>" << std::endl;

		std::fstream fs;
		fs.open("./conventional_test.txt", std::ios_base::out | std::ios_base::trunc);
		if (!fs.is_open()) {
			std::cout << "Failed to open file!" << std::endl;
			return;
		}

		//store.reset();

		fs << "Size\tPut-Delay\tPut-Throughput\tGet-Delay\tGet-Throughput\tDel-Delay\tDel-Throughput" << std::endl;
		for (uint64_t i = 0; i < 16; ++i) {
			sub_conventional_test(0x1ull << i, fs);
		}

		fs.close();
		std::cout << "Test data has been write to ./conventional_test.txt" << std::endl;
	}

	void cache_test()
	{
		std::cout << "<<Cache Test>>" << std::endl;

		store.reset();

		std::cout << "Preparing..." << std::endl;
		for (uint64_t i = 0; i < TEST_MAX; ++i) {
			store.put(i << 6, std::string(256, 's'));
		}

		steady_clock::time_point start, end;
		duration<double> time_span;
		constexpr uint64_t get_count = 1024;

		std::cout << "Testing..." << std::endl;
		start = steady_clock::now();
		for (uint64_t i = 0; i < get_count; ++i) {
			store.get(i);
		}
		end = steady_clock::now();
		time_span = duration_cast<duration<double>>(end - start);
		std::cout << get_count << " gets takes " << time_span.count() << "s." << std::endl;
		std::cout << "Average delay : " << time_span.count() / get_count << "s." << std::endl;
	}

	void compaction_test()
	{
		std::cout << "<<Compaction Test>>" << std::endl;

		std::fstream fs;
		fs.open("./compaction_test.txt", std::ios_base::out | std::ios_base::trunc);
		if (!fs.is_open()) {
			std::cout << "Failed to open file!" << std::endl;
			return;
		}

		constexpr uint64_t value_size = TEST_MAX / 2;

		store.reset();
		std::cout << "Second\tThroughput" << std::endl;
		fs << "Second\tThroughput" << std::endl;

		steady_clock::time_point start, end;
		duration<double> time_span;
		uint64_t key = 0;
		double time_count = 0;
		for (uint64_t i = 0; i < 200; ++i) {
			uint64_t throughput = 0;
			while (time_count < 1) {
				start = steady_clock::now();
				store.put(++key, std::string(value_size, 's'));
				end = steady_clock::now();
				time_span = duration_cast<duration<double>>(end - start);
				time_count += time_span.count();
				++throughput;
			}
			time_count -= 1;
			std::cout << i << '\t' << throughput << std::endl;
			fs << i << '\t' << throughput << std::endl;
		}

		fs.close();
		std::cout << "Test data has been write to ./compaction_test.txt" << std::endl;
	}

public:
	PerformanceTest(const std::string& dir, bool v = true) : Test(dir, v)
	{
	}

	void start_test(void* args = NULL) override
	{
		int mode = 0;
		if (args) {
			mode = *static_cast<int*>(args);
		}

		switch (mode) {
		case 0:
			conventional_test();
			break;
		case 1:
			cache_test();
			break;
		case 2:
			compaction_test();
			break;
		default:
			break;
		}
	}
};

int main(int argc, char* argv[])
{
	int mode = 0;
	if (argc == 2)
	{
		if (std::string(argv[1]) == "-g")
			mode = 1;
		else if (std::string(argv[1]) == "-p")
			mode = 2;
	}

	PerformanceTest test("./data");

	test.start_test(static_cast<void*>(&mode));

	return 0;
}
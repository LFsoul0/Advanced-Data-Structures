#pragma once
#include <cstdint>

class BloomFilter
{
protected:
	static constexpr uint64_t SIZE = 10240;

private:
	char seg[SIZE];

public:
	BloomFilter();
	~BloomFilter();

	void put(uint64_t key);
	bool check(uint64_t key);
};


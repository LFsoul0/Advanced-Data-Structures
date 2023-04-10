#include "BloomFilter.h"
#include "MurmurHash3.h"

BloomFilter::BloomFilter() 
	: seg{ 0 }
{
}

BloomFilter::~BloomFilter()
{
}

void BloomFilter::put(uint64_t key)
{
	uint32_t hash[4]{ 0 };
	MurmurHash3_x64_128(&key, sizeof(key), 1, hash);
	for (uint32_t idx : hash) {
		seg[(idx / 8) % SIZE] |= (0x1 << idx % 8);
	}
}

bool BloomFilter::check(uint64_t key)
{
	uint32_t hash[4]{ 0 };
	MurmurHash3_x64_128(&key, sizeof(key), 1, hash);

	bool exist = true;
	for (uint32_t idx : hash) {
		exist = exist && (seg[(idx / 8) % SIZE] & (0x1 << idx % 8));
	}

	return exist;
}
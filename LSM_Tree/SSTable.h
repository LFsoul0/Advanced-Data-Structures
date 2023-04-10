#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "KVNode.h"
#include "BloomFilter.h"

class SSTable
{
protected:
	static uint64_t currStamp;

	struct Header
	{
		uint64_t stamp;
		uint64_t size;
		uint64_t min, max;
	};

	struct IdxNode
	{
		uint64_t key;
		uint32_t offset;
	};

private:
	Header header;
	BloomFilter* filter;
	IdxNode* idxSeg;
	std::string fileName;

	uint64_t binarySearch(uint64_t key, IdxNode* _seg, uint64_t _size) const;

public:
	static constexpr uint32_t MEM_MAX = 2 * 1024 * 1024;
	static constexpr uint32_t MEM_BASE = sizeof(Header) + sizeof(BloomFilter);

	SSTable(const std::vector<KVNode<uint64_t, std::string> *>& data, const std::string& _fileName, 
		const uint64_t* assignedStamp = nullptr);
	SSTable(const std::string& _fileName);
	~SSTable();

	std::string get(uint64_t key) const;

	uint64_t stamp() const;
	uint64_t size() const;
	uint64_t minimum() const;
	uint64_t maximum() const;
	const std::string& file() const;

	uint64_t toVector(std::vector<KVNode<uint64_t, std::string>*>& ret);
};


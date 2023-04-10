#pragma once

#include "kvstore_api.h"
#include "Skiplist.h"
#include "SSTable.h"
#include <string>
#include <vector>


class KVStore : public KVStoreAPI {
protected:
	static constexpr char delMark[] = "~DELETED~";

private:
	std::string dir;
	uint32_t memSize;
	Skiplist<uint64_t, std::string> memTable;
	std::vector<std::vector<SSTable *>> sstCache;

	void saveMem();
	void compaction(uint32_t lv = 0);

public:
	KVStore(const std::string &dir);

	~KVStore();

	void put(uint64_t key, const std::string &s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;

};

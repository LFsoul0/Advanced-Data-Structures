#include "kvstore.h"
#include "utils.h"
#include <chrono>

KVStore::KVStore(const std::string &_dir)
	: KVStoreAPI(_dir), dir(_dir)
{
	memSize = SSTable::MEM_BASE;

	// scan file 
	uint32_t lv = 0;
	while (utils::dirExists(dir + '/' + "Level-" + std::to_string(lv))) {
		std::string lvPath = dir + '/' + "Level-" + std::to_string(lv);
		std::vector<std::string> fileList;
		utils::scanDir(lvPath, fileList);

		sstCache.push_back(std::vector<SSTable*>());
		for (std::string& f : fileList) {
			sstCache[lv].push_back(new SSTable(lvPath + '/' + f));
		}

		++lv;
	}
}

KVStore::~KVStore()
{
	// save memTable
	if (!memTable.empty()) {
		saveMem();
	}

	// delete cache
	for (std::vector<SSTable*>& lv : sstCache) {
		for (SSTable*& sst : lv) {
			delete sst;
		}
	}
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
	// assume memSize
	uint32_t _memSize = memSize;
	_memSize += sizeof(key);
	_memSize += (s.length() + 1) * sizeof(char);

	// if oversize, write SSTable
	if (_memSize > SSTable::MEM_MAX) {
		saveMem();

		_memSize = memSize;
		_memSize += sizeof(key);
		_memSize += (s.length() + 1) * sizeof(char);
	}

	// put key
	std::string* oldVal = nullptr;
	memTable.put(key, s, &oldVal);

	// if replaced
	if (oldVal) {
		_memSize -= sizeof(key);
		_memSize -= (oldVal->length() + 1) * sizeof(char);
		delete oldVal;
	}

	// update memSize
	memSize = _memSize;
}

/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
	std::string ret = "";

	// search in memTable
	std::string *s = memTable.get(key);
	if (s) {
		ret = *s;
	}
	else {
		// search in SSTable
		uint32_t _size = sstCache.size();
		for (uint32_t lv = 0; lv < _size; ++lv) {
			for (uint32_t i = sstCache[lv].size(); i > 0; --i) {
				ret = sstCache[lv][i - 1]->get(key);
				if (ret != "") break;
			}
			if (ret != "") break;
		}
	}

	if (ret == delMark) return "";
	return ret;
}

/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
	bool scc = (get(key) != "");

	if (scc) {
		put(key, delMark);
	}

	return scc;
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
	// clear memTable
	memTable.clear();
	memSize = SSTable::MEM_BASE;

	// clear sstCache
	for (std::vector<SSTable*>& lv : sstCache) {
		for (SSTable*& sst : lv) {
			delete sst;
  		}
	}
	sstCache.clear();

	// clear file 
	uint32_t lv = 0;
	while (utils::dirExists(dir + '/' + "Level-" + std::to_string(lv))) {
		std::string lvPath = dir + '/' + "Level-" + std::to_string(lv);
		std::vector<std::string> fileList;
		utils::scanDir(lvPath, fileList);

		for (std::string f : fileList) {
			utils::rmfile((lvPath + '/' + f).c_str());
		}

		utils::rmdir(lvPath.c_str());
		++lv;
	}
}

void KVStore::saveMem() 
{
	// check dir
	std::string levelDir = dir + '/' + "Level-0";
	if (!utils::dirExists(levelDir)) {
		utils::mkdir(levelDir.c_str());
	}

	// write SSTable
	if (sstCache.size() == 0) {
		sstCache.push_back(std::vector<SSTable*>());
	}
	std::vector<KVNode<uint64_t, std::string>*> data;
	memTable.toVector(data);
	sstCache[0].push_back(new SSTable(data, levelDir + '/' + 
		std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + ".sst"));

	// clear memTable
	memTable.clear();
	memSize = SSTable::MEM_BASE;

	// compation
	compaction();
}

void KVStore::compaction(uint32_t lv)
{
	// check level existence and its size
	if (lv >= sstCache.size() || sstCache[lv].size() <= (UINT32_C(0x1) << (lv + 1)))
		return;

	// from SSTables to vector of KVNode
	bool lastLevel = (lv + 2 >= sstCache.size());
	uint32_t compactNum = lv ? sstCache[lv].size() - (UINT32_C(0x1) << (lv + 1)) : sstCache[lv].size();
	std::vector<KVNode<uint64_t, std::string>*> data;
	std::vector<std::vector<KVNode<uint64_t, std::string>*>> vecSet;
	uint64_t _stamp = 0;
	{	
		// (from current level)
		uint64_t _min = UINT64_MAX;
		uint64_t _max = 0;
		for (uint32_t i = 0; i < compactNum; ++i) {
			std::vector<KVNode<uint64_t, std::string>*> vec;
			sstCache[lv][i]->toVector(vec);
			vecSet.push_back(vec);
			_min = sstCache[lv][i]->minimum() < _min ? sstCache[lv][i]->minimum() : _min;
			_max = sstCache[lv][i]->maximum() > _max ? sstCache[lv][i]->maximum() : _max;
			_stamp = sstCache[lv][i]->stamp() > _stamp ? sstCache[lv][i]->stamp() : _stamp;
			utils::rmfile(sstCache[lv][i]->file().c_str());
			delete sstCache[lv][i];
		}
		sstCache[lv].erase(sstCache[lv].begin(), sstCache[lv].begin() + compactNum);

		// (from next level)
		if (lv + 1 < sstCache.size()) {
			for (uint32_t i = sstCache[lv + 1].size(); i > 0; --i) {
				if (sstCache[lv + 1][i - 1]->minimum() > _max || sstCache[lv + 1][i - 1]->maximum() < _min)
					continue;

				// if interval intersecting
				std::vector<KVNode<uint64_t, std::string>*> vec;
				sstCache[lv + 1][i - 1]->toVector(vec);
				vecSet.insert(vecSet.begin(), vec);
				_stamp = sstCache[lv + 1][i - 1]->stamp() > _stamp ? sstCache[lv + 1][i - 1]->stamp() : _stamp;
				utils::rmfile(sstCache[lv + 1][i - 1]->file().c_str());
				delete sstCache[lv + 1][i - 1];
				sstCache[lv + 1].erase(sstCache[lv + 1].begin() + i - 1);
			}
		}

		// (merge sort)
		for (std::vector<KVNode<uint64_t, std::string>*>& vec : vecSet) {
			uint32_t p = 0;
			for (KVNode<uint64_t, std::string>*& node : vec) {
				uint64_t _size = data.size();
				while (p < _size && node->key > data[p]->key) ++p;

				// remove old value
				if (p < _size && node->key == data[p]->key) {
					delete data[p];
					data.erase(data.begin() + p);
				}
				// remove delMark if lastLevel
				if (lastLevel && node->val == delMark) {
					delete node;
					continue;
				}

				data.insert(data.begin() + p, node);
			}
		}
	}

	// from vector of KVNode to new SSTables
	std::string levelDir = dir + '/' + "Level-" + std::to_string(lv + 1);
	if (!utils::dirExists(levelDir)) {
		utils::mkdir(levelDir.c_str());
	}
	if (lv + 1 >= sstCache.size()) {
		sstCache.push_back(std::vector<SSTable*>());
	}
	while (!data.empty()) {
		uint32_t p = 0, _size = data.size();
		uint32_t _memSize = SSTable::MEM_BASE;

		while (p < _size) {
			_memSize += sizeof(data[p]->key);
			_memSize += (data[p]->val.length() + 1) * sizeof(char);

			if (_memSize > SSTable::MEM_MAX)
				break;

			++p;
		}

		std::vector<KVNode<uint64_t, std::string>*> subset(data.begin(), data.begin() + p);
		sstCache[lv + 1].push_back(new SSTable(subset, levelDir + '/' +
			std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + ".sst", &_stamp));
		for (uint32_t i = 0; i < p; ++i) delete data[i];
		data.erase(data.begin(), data.begin() + p);
	}

	// compact next level
	compaction(lv + 1);
}

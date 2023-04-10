#include "SSTable.h"
#include <fstream>

uint64_t SSTable::currStamp = 0;

SSTable::SSTable(const std::vector<KVNode<uint64_t, std::string>*>& data, const std::string& _fileName, 
	const uint64_t* assignedStamp)
	: fileName(_fileName)
{	
	// cache
	if (assignedStamp) {
		header.stamp = *assignedStamp;
		currStamp = header.stamp > currStamp ? header.stamp : currStamp;
	}
	else {
		header.stamp = ++currStamp;
	}
	header.size = data.size();
	header.min = UINT64_MAX;
	header.max = 0;
	filter = new BloomFilter;
	idxSeg = new IdxNode[header.size];
	uint32_t _offset = MEM_BASE + header.size * 12;

	for (uint64_t i = 0; i < header.size; ++i) {
		uint64_t _key = data[i]->key;

		header.min = _key < header.min ? _key : header.min;
		header.max = _key > header.max ? _key : header.max;

		filter->put(_key);
		idxSeg[i].key = _key;
		idxSeg[i].offset = _offset;

		_offset += (data[i]->val.length() + 1) * sizeof(char);
	}
	
	// write to file
	std::fstream fs;
	fs.open(_fileName, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);

	uint64_t safe_size = 0;
	fs.write((const char*)&header, sizeof(Header));
	fs.seekp(sizeof(header.stamp), std::ios_base::beg);
	fs.write((const char*)&safe_size, sizeof(safe_size));
	fs.seekp(sizeof(Header), std::ios_base::beg);

	fs.write((const char*)filter, sizeof(BloomFilter));
	for (uint64_t i = 0; i < header.size; ++i) {
		fs.write((const char*)&idxSeg[i].key, sizeof(IdxNode::key));
		fs.write((const char*)&idxSeg[i].offset, sizeof(IdxNode::offset));
	}
	for (uint64_t i = 0; i < header.size; ++i) {
		fs.write(data[i]->val.c_str(), (data[i]->val.length() + 1) * sizeof(char));
	}

	fs.seekp(sizeof(header.stamp), std::ios_base::beg);
	fs.write((const char*)&header.size, sizeof(header.size));

	fs.close();
}

SSTable::SSTable(const std::string& _fileName)
	: fileName(_fileName)
{
	// read file
	std::fstream fs;
	fs.open(_fileName, std::ios_base::in | std::ios_base::binary);

	fs.read((char*)&header, sizeof(Header));
	currStamp = header.stamp > currStamp ? header.stamp : currStamp;
	filter = new BloomFilter;
	idxSeg = new IdxNode[header.size];

	fs.read((char*)filter, sizeof(BloomFilter));
	for (uint64_t i = 0; i < header.size; ++i) {
		fs.read((char*)&idxSeg[i].key, sizeof(IdxNode::key));
		fs.read((char*)&idxSeg[i].offset, sizeof(IdxNode::offset));
	}

	fs.close();
}

SSTable::~SSTable()
{
	delete filter;
	delete[] idxSeg;
}

std::string SSTable::get(uint64_t key) const
{
	// interval check
	if (key < header.min || key > header.max)
		return "";

	// filter
	if (!filter->check(key)) 
		return "";
	
	// idx search
	uint64_t target = binarySearch(key, idxSeg, header.size);
	if (target == UINT64_MAX)
		return "";

	// read file
	std::fstream fs;
	fs.open(fileName, std::ios_base::in | std::ios_base::binary);

	uint32_t len = 0;
	if (target == header.size - 1) {
		fs.seekg(0, std::ios_base::end);
		len = (uint32_t)fs.tellg() - idxSeg[target].offset;
	}
	else {
		len = idxSeg[target + 1].offset - idxSeg[target].offset;
	}

	char* buf = new char[len];
	fs.seekg(idxSeg[target].offset, std::ios_base::beg);
	fs.read(buf, len);
	std::string ret(buf);
	delete[] buf;

	fs.close();
	return ret;
}

uint64_t SSTable::binarySearch(uint64_t key, IdxNode* _seg, uint64_t _size) const
{
	if (!_size)
		return UINT64_MAX;

	uint64_t mid = _size / 2;
	if (_seg[mid].key == key)
		return mid;

	if (_seg[mid].key > key)
		return binarySearch(key, _seg, mid);
	else {
		uint64_t ret = binarySearch(key, _seg + mid + 1, _size - mid - 1);
		return ret == UINT64_MAX ? UINT64_MAX : ret + mid + 1;
	}
}

uint64_t SSTable::stamp() const
{
	return header.stamp;
}

uint64_t SSTable::size() const
{
	return header.size;
}

uint64_t SSTable::minimum() const
{
	return header.min;
}

uint64_t SSTable::maximum() const
{
	return header.max;
}

const std::string& SSTable::file() const
{
	return fileName;
}

uint64_t SSTable::toVector(std::vector<KVNode<uint64_t, std::string>*>& ret)
{
	// read file
	std::fstream fs;
	fs.open(fileName, std::ios_base::in | std::ios_base::binary);

	for (uint64_t i = 0; i < header.size; ++i) {
		uint32_t len = 0;
		if (i == header.size - 1) {
			fs.seekg(0, std::ios_base::end);
			len = (uint32_t)fs.tellg() - idxSeg[i].offset;
		}
		else {
			len = idxSeg[i + 1].offset - idxSeg[i].offset;
		}

		char* buf = new char[len];
		fs.seekg(idxSeg[i].offset, std::ios_base::beg);
		fs.read(buf, len);
		ret.push_back(new KVNode<uint64_t, std::string>(idxSeg[i].key, std::string(buf)));
		delete[] buf;
	}

	fs.close();

	return header.size;
}
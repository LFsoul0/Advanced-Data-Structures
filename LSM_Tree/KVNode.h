#pragma once

template<typename K, typename V>
struct KVNode {
	K key;
	V val;
	KVNode(const K& _key, const V& _val) : key(_key), val(_val) {}
	KVNode() {}
};
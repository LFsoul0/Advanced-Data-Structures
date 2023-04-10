#pragma once

#include <set>
#include <QString>

using namespace std;

template <typename K, typename V>
struct Dictionary {
    struct Node {
        K key;
        V* val;

        Node(K k) : key(k) { }
        Node(K k, V v) : key(k), val(new V(v)) { }
        bool operator<(const Node& other) const { return key < other.key; }
        bool operator>(const Node& other) const { return key > other.key; }
        bool operator<=(const Node& other) const { return key <= other.key; }
        bool operator>=(const Node& other) const { return key >= other.key; }
        bool operator==(const Node& other) const { return key == other.key; }
        bool operator!=(const Node& other) const { return key != other.key; }
    };

    set<Node> dSet;

    int size() {
        return dSet.size();
    }
    void clear() {
        dSet.clear();
    }
    bool put(K k, V v) {
        const auto ret = dSet.insert(Node(k, v));
        if (!ret.second) {
            dSet.erase(ret.first);
            dSet.insert(Node(k, v));
        }
        return true;
    }
    V* get(K k) {
        const auto tar = dSet.find(Node(k));
        if (tar == dSet.end())
            return nullptr;
        else
            return tar->val;
    }
    bool remove(K k) {
        return dSet.erase(Node(k));
    }

    QString toString() {
        QString s;

        for (auto it = dSet.begin(); it != dSet.end(); it++) {
            s += it->key;
            s += ": ";
            s += it->val->getTypeString();
            s += " = ";
            s += it->val->getValString();
            s += "\n";
        }

        return s;
    }
};

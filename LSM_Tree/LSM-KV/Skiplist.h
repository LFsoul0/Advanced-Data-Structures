#pragma once
#include <vector>
#include "KVNode.h"

template<typename K, typename V>
class Skiplist {
protected:
    struct Node : public KVNode<K, V> {
        Node* right, * down;
        Node(Node* right, Node* down, K key, V val) : right(right), down(down), KVNode<K, V>(key, val) {}
        Node() : right(nullptr), down(nullptr) {}
    };

private:
    void replace(Node* p, const V& val) {
        while (p) {
            p->val = val;
            p = p->down;
        }
    }

    void clear(Node* n) {
        while (n) {
            Node* next = n->down;
            while (n) {
                Node* tmp = n->right;
                delete n;
                n = tmp;
            }
            n = next;
        }
    }

public:
    Node* head;
    Skiplist() {
        head = new Node(); 
    }

    ~Skiplist() {
        clear(head);
    }

    void clear() {
        clear(head);
        head = new Node();
    }

    bool empty() const {
        Node* tmp = head;
        while (tmp->down) tmp = tmp->down;

        return !tmp->right;
    }

    size_t size() const {
        size_t n = 0;
        Node* tmp = head;
        while (tmp->down) tmp = tmp->down;

        while (tmp->right) {
            tmp = tmp->right;
            ++n;
        }

        return n;
    }

    V* get(const K& key) const {
        Node* p = head;
        while (true) {
            while (p->right && p->right->key < key) p = p->right;
            if (p->right && p->right->key == key) return &p->right->val;
            if (!p->down) return nullptr;
            p = p->down;
        }
    }

    void put(const K& key, const V& val, V** oldVal = nullptr) {
        std::vector<Node*> pathList;
        Node* p = head;
        while (p) {
            while (p->right && p->right->key < key) p = p->right;
            if (p->right && p->right->key == key) {
                if (oldVal) {
                    *oldVal = new V(p->right->val);
                }
                replace(p->right, val);
                return;
            }
            pathList.push_back(p);
            p = p->down;
        }

        bool insertUp = true;
        Node* downNode = nullptr;
        while (insertUp && pathList.size() > 0) {   
            Node* insert = pathList.back();
            pathList.pop_back();
            insert->right = new Node(insert->right, downNode, key, val); 
            downNode = insert->right;    
            insertUp = (rand() & 1);   
        }
        if (insertUp) {  
            Node* oldHead = head;
            head = new Node();
            head->right = new Node(nullptr, downNode, key, val);
            head->down = oldHead;
        }
    }

    bool remove(const K& key, V** oldVal = nullptr) {
        Node* p = head;
        while (true) {
            while (p->right && p->right->key < key) p = p->right;
            if (p->right && p->right->key == key) break;
            if (!p->down) return false;
            p = p->down;
        }

        if (oldVal) {
            *oldVal = new V(p->right->val);
        }

        Node* tmp = p->right;
        while (true) {
            p->right = tmp->right;
            Node* next = tmp->down;
            delete tmp;

            if (!next) break;
            p = p->down;
            while (p->right != next) p = p->right;
            tmp = next;
        }

        while (head->down && !head->right) {
            tmp = head;
            head = tmp->down;
            delete tmp;
        }

        return true;
    }

    size_t toVector(std::vector<KVNode<K, V>*>& ret) {
        Node* tmp = head;
        while (tmp->down) tmp = tmp->down;

        size_t n = 0;
        while (tmp->right) {
            tmp = tmp->right;
            ret.push_back(tmp);
            ++n;
        }

        return n;
    }
};

#pragma once

#include <QVector>
#include <QList>
#include <QColor>

template<typename K, typename V>
struct Node {
    Node<K, V>* right, * down;
    K key;
    V val;
    Node(Node<K, V>* right, Node<K, V>* down, const K& key, const V& val) : right(right), down(down), key(key), val(val) {}
    Node() : right(nullptr), down(nullptr), key(0) {}
};

template<typename K, typename V>
class Skiplist {
public:
    Node<K, V>* head;
    Skiplist() {
        head = new Node<K, V>();
    }

    ~Skiplist() {
        clear(head);
    }

    size_t size() const {
        size_t n = 0;
        Node<K, V>* tmp = head;
        while (tmp->down) tmp = tmp->down;

        while (tmp->right) {
            tmp = tmp->right;
            ++n;
        }

        return n;
    }

    void clear() {
        clear(head);
        head = new Node<K, V>();
    }

    void clear(Node<K, V>* n) {
        if (n == nullptr) return;

        clear(n->down);
        clearline(n);
    }

    void clearline(Node<K, V>* n) {
        if (n == nullptr) return;

        clearline(n->right);
        delete n;
    }

    Node<K,V>* get(const K& key) {
        Node<K, V>* p = head;
        while (true) {
            while (p->right && p->right->key <= key) p = p->right;
            if (p != head && p->key == key) {
                while(p->down) p = p->down;
                return p;
            }
            if (!p->down) return nullptr;
            p = p->down;
        }
    }

    void put(const K& key, const V& val) {
        remove(key);

        QVector<Node<K, V>*> pathList;
        Node<K, V>* p = head;
        while (p) {
            while (p->right && p->right->key < key) {
                p = p->right;
            }
            pathList.push_back(p);
            p = p->down;
        }

        bool insertUp = true;
        Node<K, V>* downNode = nullptr;
        while (insertUp && pathList.size() > 0) {
            Node<K, V>* insert = pathList.back();
            pathList.pop_back();
            insert->right = new Node<K, V>(insert->right, downNode, key, val);
            downNode = insert->right;
            insertUp = (rand() & 1);
        }
        if (insertUp) {
            Node<K, V>* oldHead = head;
            head = new Node<K, V>();
            head->right = new Node<K, V>(NULL, downNode, key, val);
            head->down = oldHead;
        }
    }

    bool remove(const K& key) {
        Node<K, V>* p = head;
        while (true) {
            while (p->right && p->right->key < key) p = p->right;
            if (p->right && p->right->key == key) break;
            if (!p->down) return false;
            p = p->down;
        }

        Node<K, V>* tmp = p->right;
        while (true) {
            p->right = tmp->right;
            Node<K, V>* next = tmp->down;
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

    QString toString(QList<QPair<int, QColor>>* highlights = nullptr, Node<K, V>* debug = nullptr) const {
        Node<K, V>* p = head;
        while (p->down) p = p->down;

        QString s;
        while (p->right) {
            p = p->right;
            s.append(QString::number(p->key));
            if (highlights && p == debug) {
                highlights->append(QPair<int, QColor>(s.length() - 1, QColor(100, 255, 100)));
            }
            else if (highlights && !p->val.isValid()) {
                highlights->append(QPair<int, QColor>(s.length() - 1, QColor(255, 100, 100)));
            }
            s.append(" ");
            s.append(p->val.toString());
            s.append("\n");
        }

        return s;
    }

    Node<K, V>* getHead() const {
        Node<K, V>* p = head;
        while (p->down) p = p->down;

        return p->right;
    }
};

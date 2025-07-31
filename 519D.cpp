#include <iostream>
#include <string>
#include <vector>
#include <functional>  


namespace std {
    template<>
    struct hash<std::pair<char, long long>> {
        size_t operator()(const std::pair<char, long long>& p) const {
            size_t h1 = hash<char>{}(p.first);
            size_t h2 = hash<long long>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };
}

template<typename K, typename V>
class Node {
public:
    K key;
    V value;
    Node* next;
    Node(const K& key, const V& value) : key(key), value(value), next(nullptr) {}
};

template<typename K, typename V>
class HashTable {
    static const int TABLE_SIZE = 262147; 
    Node<K, V>* dataMap[TABLE_SIZE];

    int hash(const K& key) {
        std::hash<K> hasher;
        size_t hashed = hasher(key);
        return (hashed * 23) % TABLE_SIZE;
    }

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) dataMap[i] = nullptr;
    }

    ~HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node<K, V>* temp = dataMap[i];
            while (temp) {
                Node<K, V>* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
        }
    }

    void set(const K& key, const V& value) {
        int index = hash(key);
        Node<K, V>* temp = dataMap[index];
        while (temp != nullptr) {
            if (temp->key == key) {
                temp->value = value; 
                return;
            }
            temp = temp->next;
        }
        
        Node<K, V>* newNode = new Node<K, V>(key, value);
        newNode->next = dataMap[index];
        dataMap[index] = newNode;
    }

    V get(const K& key) {
        int index = hash(key);
        Node<K, V>* temp = dataMap[index];
        while (temp != nullptr) {
            if (temp->key == key) {
                return temp->value;
            }
            temp = temp->next;
        }
        return V(); 
    }

    bool contains(const K& key) {
        int index = hash(key);
        Node<K, V>* temp = dataMap[index];
        while (temp != nullptr) {
            if (temp->key == key) return true;
            temp = temp->next;
        }
        return false;
    }
};

int main() {
    
    HashTable<char, int> letterValues;
    for (int i = 0; i < 26; ++i) {
        int val; std::cin >> val;
        letterValues.set('a' + i, val);
    }

   
    std::string s;
    std::cin >> s;
    int n = (int)s.length();

    
    std::vector<long long> prefixSum(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        prefixSum[i + 1] = prefixSum[i] + letterValues.get(s[i]);
    }

    
    HashTable<std::pair<char, long long>, int> prefixCounts;

    long long result = 0;

    for (int i = 0; i < n; ++i) {
        std::pair<char, long long> keyBefore(s[i], prefixSum[i]);
    
        result += prefixCounts.get(keyBefore);

        std::pair<char, long long> keyAfter(s[i], prefixSum[i + 1]);
        int oldCount = prefixCounts.get(keyAfter);
        prefixCounts.set(keyAfter, oldCount + 1);
    }

    std::cout << result << "\n";

    return 0;
}

#ifndef LRUCACHE_H_
#define LRUCACHE_H_

#include <unordered_map>
#include <list>
#include <cstdio>

namespace lru {

typedef int K;
typedef int V;

// template <typename K, typename V>
class LRUCache {
    typedef std::list<std::pair<K, V*>> CacheList;
    typedef std::unordered_map<K, CacheList::iterator> CacheMap;

private:
    size_t _limit;
    CacheList _list;
    CacheMap _to_list_iter;
    void _remove_least_recent();

public:
    LRUCache(size_t cache_size) : _limit(cache_size) {}
    ~LRUCache() = default;
    LRUCache(const LRUCache& o) = delete;
    LRUCache(LRUCache&& o) = default;

    void put(const K& key, V* ptr);
    V* get(const K& key);
    bool has(const K& key);
    void del(const K& key);

}; // class LRUCache

void LRUCache::_remove_least_recent() {
    del(_list.rbegin()->first);
}

V* LRUCache::get(const K& key) {
    printf("accessing %d\n", key);
    auto iter = _to_list_iter[key];
    auto pointer = iter->second;
    _list.erase(iter);
    _list.push_front({key, pointer});
    _to_list_iter[key] = _list.begin();
    return _list.begin()->second;
}

void LRUCache::put(const K& key, V* ptr) {
    if (has(key)) {
        del(key);
    }
    if (_list.size() >= _limit) {
        _remove_least_recent();
    }
    printf("update %d with %p(%d)\n", key, ptr, *ptr);
    _list.push_front({key, ptr});
    _to_list_iter[key] = _list.begin();
}

void LRUCache::del(const K& key) {
    if (!has(key)) {
        printf("%d doesn't exits, fail removing", key);
        return;
    }
    printf("removing %d\n", key);
    auto iter = _to_list_iter[key];
    _list.erase(iter);
    _to_list_iter.erase(key);
}

bool LRUCache::has(const K& key) {
    return _to_list_iter.find(key) != _to_list_iter.end();
}


} // namespace lru


#endif // LRUCACHE_H_


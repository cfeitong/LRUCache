#ifndef LRUCACHE_H_
#define LRUCACHE_H_

#include <unordered_map>
#include <list>

namespace lru {


template <typename K, typename P>
class LRUCache {
    typedef std::list<std::pair<K, P>> CacheList;
    typedef std::unordered_map<K, typename CacheList::iterator> CacheMap;

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

    void put(const K& key, P ptr);
    P get(const K& key);
    bool has(const K& key);
    void del(const K& key);

}; // class LRUCache

template <typename K, typename P>
void LRUCache<K, P>::_remove_least_recent() {
    del(_list.rbegin()->first);
}

template <typename K, typename P>
P LRUCache<K, P>::get(const K& key) {
    auto iter = _to_list_iter[key];
    auto pointer = iter->second;
    _list.erase(iter);
    _list.push_front({key, pointer});
    _to_list_iter[key] = _list.begin();
    return _list.begin()->second;
}

template <typename K, typename P>
void LRUCache<K, P>::put(const K& key, P ptr) {
    if (has(key)) {
        del(key);
    }
    if (_list.size() >= _limit) {
        _remove_least_recent();
    }
    _list.push_front({key, ptr});
    _to_list_iter[key] = _list.begin();
}

template <typename K, typename P>
void LRUCache<K, P>::del(const K& key) {
    if (!has(key)) {
        return;
    }
    auto iter = _to_list_iter[key];
    _list.erase(iter);
    _to_list_iter.erase(key);
}

template <typename K, typename P>
bool LRUCache<K, P>::has(const K& key) {
    return _to_list_iter.find(key) != _to_list_iter.end();
}


} // namespace lru


#endif // LRUCACHE_H_


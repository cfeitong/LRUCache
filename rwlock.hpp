#ifndef RWLOCK_HPP_
#define RWLOCK_HPP_

#include <mutex>
#include <condition_variable>

namespace lru {

class RWLock {
private:
    std::mutex _mutex;
    std::condition_variable _cond_var;
    size_t _readers_waiting;
    int _status; // -1 for one writer, n(n>=0) for n readers
    bool _writer_waiting;
public:
    RWLock() : _readers_waiting(0), _status(0), _writer_waiting(false) {}
    RWLock(const RWLock& o) = delete;
    RWLock(RWLock&& o) = default;

    void reader_lock();
    void writer_lock();
    void reader_unlock();
    void writer_unlock();
};

void RWLock::reader_lock() {
    std::unique_lock<std::mutex> lock_(_mutex);
    _readers_waiting++;
    _cond_var.wait(lock_, [=]() { return _writer_waiting == false && _status >= 0; });
    _readers_waiting--;
    _status++;
}

void RWLock::writer_lock() {
    std::unique_lock<std::mutex> lock_(_mutex);
    _writer_waiting = true;
    _cond_var.wait(lock_, [=]() { return _readers_waiting == 0; });
    _writer_waiting = false;
    _status--;
}

void RWLock::reader_unlock() {
    std::unique_lock<std::mutex> lock_(_mutex);
    if (_status <= 0) { // no reader or one writer
        return;
    }
    if (--_status == 0) {
        _cond_var.notify_one();
    }
}

void RWLock::writer_unlock() {
    std::unique_lock<std::mutex> lock_(_mutex);
    if (_status >= 0) { // no writer
        return;
    }
    if (++_status >= 0) {
        _cond_var.notify_all();
    }
}

}

#endif // RWLOCK_HPP_
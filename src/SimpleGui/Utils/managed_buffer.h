#pragma once

#include <deque>

template <typename T, size_t maxSize>
  class managed_buffer {
    std::deque<T> data;

public:

    size_t size() const { return data.size(); }
    void clear() { data.clear(); }
    bool empty() const { return data.empty(); }
    const T& front() const { return data.front(); }
    const T& back() const { return data.back(); }


    void push(const T& value) {
        if (data.size() == maxSize) {
            data.pop_front();
        }
        data.push_back(value);
    }

    void pop_first() {
        if (!data.empty()) {
            data.pop_front();
        }
    }

    void pop_last() {
        if (!data.empty()) {
            data.pop_back();
        }
    }

    managed_buffer& operator+=(const T& value) {
        if (data.size() == maxSize) {
            data.pop_front();
        }
        data.push_back(value);

        return this;
    }

    T &operator[](int index) {
        return data.at(index);
    }

    const std::deque<T>& get() const { return data; }
};

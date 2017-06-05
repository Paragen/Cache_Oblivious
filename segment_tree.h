
#ifndef CACHE_OBLIVIOUS_SEGMENT_TREE_H
#define CACHE_OBLIVIOUS_SEGMENT_TREE_H

#include <vector>
#include <cstddef>
#include <assert.h>
#include <cmath>

template<class T>
class segment_tree {
    std::vector<T> data;

    T (*func)(const T &, const T &);

    size_t size;

    template<class Iter>
    void build(Iter bi, Iter ei, size_t number);

    T get(size_t lborder, size_t rborder, size_t number, size_t lcurrent, size_t rcurrent);

    void update(size_t position, const T &value, size_t number, size_t lcurrent, size_t rcurrent);

public:
    template<class Iter>
    segment_tree(Iter bi, Iter ei, T(&func)(const T &, const T &));

    T get(size_t lborder, size_t rborder);

    void update(size_t position, const T &value);
};

template<class T>
template<class Iter>
segment_tree<T>::segment_tree(Iter bi, Iter ei, T(&func)(const T &, const T &)):func(func) {
    size_t size = ei - bi;
    this->size = size;
    size_t tmp = pow(2, ceil(log(size) / log(2)));
    data.resize(2 * tmp);
    build(bi, ei, 1);
}

template<class T>
template<class Iter>
void segment_tree<T>::build(Iter bi, Iter ei, size_t number) {
    if (bi + 1 == ei) {
        data[number] = *bi;

    } else {
        auto diff = ei - bi;
        diff = (diff + 1) >> 1;
        build(bi, bi + diff, number << 1);
        build(bi + diff, ei, number << 1 | 1);
        data[number] = func(data[number << 1], data[number << 1 | 1]);
    }
}

template<class T>
T segment_tree<T>::get(size_t lborder, size_t rborder, size_t number, size_t lcurrent, size_t rcurrent) {
    if (lcurrent == lborder && rborder == rcurrent) {
        return data[number];
    }

    auto mid = lcurrent + ((rcurrent - lcurrent) >> 1);
    if (mid < lborder) {
        return get(lborder, rborder, number << 1 | 1, mid + 1, rcurrent);
    }

    if (mid + 1 > rborder) {
        return get(lborder, rborder, number << 1, lcurrent, mid);
    }

    return func(get(lborder, mid, number << 1, lcurrent, mid),
                get(mid + 1, rborder, number << 1 | 1, mid + 1, rcurrent));
}

template<class T>
void segment_tree<T>::update(size_t position, const T &value, size_t number, size_t lcurrent, size_t rcurrent) {
    if (lcurrent == rcurrent) {
        data[number] = value;
    } else {
        auto mid = lcurrent + ((rcurrent - lcurrent) >> 1);
        if (position <= mid) {
            update(position, value, number << 1, lcurrent, mid);
        } else {
            update(position, value, number << 1 | 1, mid + 1, rcurrent);
        }

        data[number] = func(data[number << 1], data[number << 1 | 1]);
    }

}

template<class T>
T segment_tree<T>::get(size_t lborder, size_t rborder) {
    assert(lborder <= rborder);
    return get(lborder, rborder, 1, 0, size - 1);
}

template<class T>
void segment_tree<T>::update(size_t position, const T &value) {
    update(position, value, 1, 0, size - 1);
}

#endif //CACHE_OBLIVIOUS_SEGMENT_TREE_H

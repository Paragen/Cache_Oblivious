
#include <cstddef>
#include <vector>
#include <assert.h>
#include <tgmath.h>

#ifndef CACHE_OBLIVIOUS_MATRIX_H
#define CACHE_OBLIVIOUS_MATRIX_H

template<class T, size_t N, size_t M>
class matrix {
    T _matrix[N][M];
    const unsigned cache = (unsigned)std::sqrt(2 * 1024 / sizeof(T));
    void transpose_part(size_t left, size_t right, size_t up, size_t down, matrix& dest);
public:
    matrix();
    template<class Iter>
    matrix(Iter bi, Iter ei);
    matrix(const T& val);

    T* operator[](size_t index);
    matrix<T, M, N> transpose();
    matrix<T, M, N> oblivious_transpose();
};

template<class T, size_t N, size_t M>
matrix<T, N, M>::matrix(){}

template<class T, size_t N, size_t M>
template<class Iter>
matrix<T, N, M>::matrix(Iter bi, Iter ei) {
    assert(ei - bi == N * M);
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            _matrix[i][j] = *(bi++);
        }
    }
}

template<class T, size_t N, size_t M>
matrix<T, N, M>::matrix(const T &val) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            _matrix[i][j] = val;
        }
    }
}


template<class T, size_t N, size_t M>
T *matrix<T, N, M>::operator[](size_t index) {
    return _matrix[index];
}


template<class T, size_t N, size_t M>
matrix<T, M, N> matrix<T, N, M>::transpose() {
    matrix<T, M, N> answer;
    for(size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            answer[j][i] = _matrix[i][j];
        }
    }
    return answer;
}

template<class T, size_t N, size_t M>
matrix<T, M, N> matrix<T, N, M>::oblivious_transpose() {
    matrix<T, M, N> answer;
    transpose_part(0, N, 0, M, answer);
    return answer;
}

template<class T, size_t N, size_t M>
void matrix<T, N, M>::transpose_part(size_t left, size_t right, size_t up, size_t down, matrix& dest) {
    size_t width = right - left, height = down - up;

    if (width <= cache && height <= cache) {
        for (auto i = left; i < right; ++i) {
            for (auto j = up; j < down; ++j) {
                dest[i][j] = _matrix[j][i];
            }
        }

        return;
    }

    if (width < height) {
        auto tmp = height >> 1;
        transpose_part(left, right, up , up + tmp, dest);
        transpose_part(left, right, up + tmp, down, dest);
    } else {
        auto tmp = width >> 1;
        transpose_part(left, left + tmp, up , down, dest);
        transpose_part(left + tmp, right, up , down, dest);
    }
}


#endif //CACHE_OBLIVIOUS_MATRIX_H

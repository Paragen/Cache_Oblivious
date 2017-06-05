#pragma comment(linker,"/STACK:67100000")
#include <iostream>
#include <vector>
#include "matrix.h"
#include <cstdlib>
#include <ctime>

namespace test_matrix {

    void test_correctness() {
        std::vector<int> vec;
        for (size_t i = 0; i < 10000; ++i) {
            vec.push_back(rand());
        }
        matrix<int, 100, 100> mx1(vec.begin(), vec.end()), mx2(vec.begin(), vec.end());
        auto tr1 = mx1.transpose();
        auto tr2 = mx2.oblivious_transpose();

        for (size_t i = 0; i < 100 ; ++i) {
            for (size_t j = 0; j < 100; ++j) {
                assert(tr1[i][j] == tr2[i][j]);
            }
        }
    }

    const size_t size = 100;

    void test_matrix() {
        std::cout << "----------- Correctness ------------\n";
        test_correctness();
        std::cout << "----------- All correct ------------\n";

        std::cout << "----------- Performance ------------\n";
        matrix<int, size, size> mx(1);
        auto start = std::clock();
        mx.transpose();
        double f1 = (double)(clock() - start) / CLOCKS_PER_SEC;
        std::cout << "Common transpose done in: " << f1 << "seconds.\n";
        start = clock();
        mx.oblivious_transpose();
        double f2 = (double)(clock() - start) / CLOCKS_PER_SEC;
        std::cout << "Cache oblivious transpose done in: " << f2 << "seconds.\n";
        std::cout << "Common / Cache oblivious : " <<  f1/f2 ;
    }
}
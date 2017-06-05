#include <iostream>
#include <vector>
#include "segment_tree.h"
#include "ho_segment_tree.h"
#include <cstdlib>

namespace test_segment_tree {
    int max(const int& first, const int& second) {
        return first > second ? first : second;
    }
    void test_correctness() {
        std::vector<int> vec2;
        const size_t sz = 100000;
        int tmp;
        for (int i = 0; i < sz; ++i) {
            vec2.push_back(rand());
        }

        ho_segment_tree tree1(vec2.data(), vec2.data() + vec2.size(), &max);
        segment_tree<int> tree2(vec2.begin(), vec2.end(), max);

        for (int i = 0; i < 1000; ++i) {
            size_t first = rand() % (sz - 1);
            size_t second = sz - first;
            second = first + (rand() % second);
            assert(tree1.get(first, second) == tree2.get(first, second));
        }

        for (int i = 0; i < sz; ++i) {
            tmp = rand() % 10000;
            tree1.update(i, tmp);
            tree2.update(i, tmp);
        }

        for (int i = 0; i < 1000; ++i) {
            size_t first = rand() % (sz - 1);
            size_t second = sz - first;
            second = first + (rand() % second);
            assert(tree1.get(first, second) == tree2.get(first, second));
        }
        }

        void test_segment_tree(size_t size) {
            std::cout << "----------- Correctness ------------\n";
            test_correctness();
            std::cout << "----------- All correct ------------\n";

            std::cout << "----------- Performance ------------\n";
            int tmp;
            std::vector<int> vec2;
            size_t count = size / 100;
            for (int i = 0; i < size; ++i) {
                vec2.push_back(rand());
            }
            auto start = clock();
            ho_segment_tree tree11(vec2.data(), vec2.data() + vec2.size(), &max);

            for (int i = 0; i < count; ++i) {
                size_t first = rand() % (size - 1);
                size_t second = size - first;
                second = first + (rand() % second);
                tree11.get(first, second);
            }

            for (int i = 0; i < size; ++i) {
                tmp = rand();
                tree11.update(i, tmp);
            }

            for (int i = 0; i < count; ++i) {
                size_t first = rand() % (size - 1);
                size_t second = size - first;
                second = first + (rand() % second);
                tree11.get(first, second);
            }
            double f1 =  (double) (clock() - start) / CLOCKS_PER_SEC;
            std::cout << "CO segment tree ^_^ done in : " << f1 << std::endl;

            start  = clock();
            segment_tree<int> tree22(vec2.begin(), vec2.end(), max);

            for (int i = 0; i < count; ++i) {
                size_t first = rand() % (size - 1);
                size_t second = size - first;
                second = first + (rand() % second);
                tree22.get(first, second);
            }


            for (int i = 0; i < size; ++i) {
                tmp = rand();
                tree22.update(i, tmp);
            }

            for (int i = 0; i < count; ++i) {
                size_t first = rand() % (size - 1);
                size_t second = size - first;
                second = first + (rand() % second);
                tree22.get(first, second);
            }
            double f2 =  (double) (clock() - start) / CLOCKS_PER_SEC;
            std::cout << "segment tree done in : " << f2 << std::endl << "Common / CO : " <<  f2 / f1 << std::endl;

        }
    }
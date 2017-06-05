

#ifndef CACHE_OBLIVIOUS_HO_SEGMENT_TREE_H
#define CACHE_OBLIVIOUS_HO_SEGMENT_TREE_H

#include <vector>
#include <cstddef>
#include <cmath>
#include <map>

typedef int value_type;
const unsigned cache = 2 * 1024 / sizeof(value_type);
const size_t block_deep = (size_t) std::ceil(std::log(cache) / std::log(2));
const size_t last_level_quantity = (size_t) pow(2, block_deep);

class ho_segment_tree {
    size_t left, right;
    std::vector<value_type> data;
    std::map<size_t, ho_segment_tree> children;

    value_type (*func)(const value_type &, const value_type &);


    ho_segment_tree(size_t left, size_t right, value_type (*func)(const value_type &, const value_type &)) : left(left),
                                                                                                             right(right),
                                                                                                             func(func) {
        data.resize(2 * last_level_quantity);
    }

public:
    ho_segment_tree() {};

    ho_segment_tree(value_type *left_pointer, value_type *right_pointer,
                    value_type (*func)(const value_type &, const value_type &));

    value_type get(size_t left_border, size_t right_border);

    void update(size_t position, value_type value);

private:
    value_type build(value_type *left_pointer, value_type *right_pointer);

    value_type
    build(value_type *left_pointer, value_type *right_pointer, size_t left_current, size_t right_current, size_t deep,
          size_t number) {
        if (left_pointer + 1 >= right_pointer) {
            return data[number] = *left_pointer;

        } else {
            auto mid = left_current + ((right_current - left_current) >> 1);
            auto diff = right_pointer - left_pointer;
            diff = (diff + 1) >> 1;
            if (deep + 1 < block_deep) {

                build(left_pointer, left_pointer + diff, left_current, mid, deep + 1, number << 1);
                build(left_pointer + diff, right_pointer, mid + 1, right_current, deep + 1, number << 1 | 1);
                return data[number] = func(data[number << 1], data[number << 1 | 1]);
            } else {
                children[left_current] = ho_segment_tree(left_current, mid, func);
                children[mid + 1] = ho_segment_tree(mid + 1, right_current, func);
                return data[number] = func(children[left_current].build(left_pointer, left_pointer + diff),
                                           children[mid + 1].build(left_pointer + diff, right_pointer));

            }
        }
    }

    value_type get(size_t left_border, size_t right_border, size_t left_current, size_t right_current, size_t deep,
                   size_t number) {
        if (left_border == left_current && right_border == right_current) {
            return data[number];
        }
        auto mid = left_current + ((right_current - left_current) >> 1);
        if (deep + 1 >= block_deep) {

            if (right_border <= mid) {
                return children[left_current].get(left_border, right_border);
            } else if (left_border > mid) {
                return children[mid + 1].get(left_border, right_border);
            } else {
                return func(children[left_current].get(left_border, right_border),
                            children[mid + 1].get(left_border, right_border));
            }

        } else {


            if (mid < left_border) {
                return get(left_border, right_border, mid + 1, right_current, deep + 1, number << 1 | 1);
            }

            if (mid + 1 > right_border) {
                return get(left_border, right_border, left_current, mid, deep + 1, number << 1);
            }

            return func(get(left_border, mid, left_current, mid, deep + 1, number << 1),
                        get(mid + 1, right_border, mid + 1, right_current, deep + 1, number << 1 | 1));
        }
    }


    void
    update(size_t position, value_type value, size_t left_current, size_t right_current, size_t deep, size_t number) {

        if (left_current == right_current) {
            data[number] = value;
            return;
        }
        auto mid = left_current + ((right_current - left_current) >> 1);
        if (deep + 1 >= block_deep) {

            children[position > mid ? mid + 1 : left_current].update(position, value);

            data[number] = func(children[left_current].data[1], children[mid + 1].data[1]);

        } else {

            if (position <= mid) {
                update(position, value, left_current, mid, deep + 1, number << 1);
            } else {
                update(position, value, mid + 1, right_current, deep + 1, number << 1 | 1);
            }

            data[number] = func(data[number << 1], data[number << 1 | 1]);
        }

    }

};

value_type ho_segment_tree::build(value_type *left_pointer, value_type *right_pointer) {
    return build(left_pointer, right_pointer, left, right, 0, 1);
}

value_type ho_segment_tree::get(size_t left_border, size_t right_border) {
    if (left_border < left) {
        left_border = left;
    }
    if (right_border > right) {
        right_border = right;
    }

    return get(left_border, right_border, left, right, 0, 1);
}

void ho_segment_tree::update(size_t position, value_type value) {
    update(position, value, left, right, 0, 1);
}

ho_segment_tree::ho_segment_tree(value_type *left_pointer, value_type *right_pointer,
                                 value_type (*func)(const value_type &, const value_type &)) : func(func) {
    left = 0;
    right = right_pointer - left_pointer - 1;
    data.resize(2 * last_level_quantity);
    build(left_pointer, right_pointer);
}


#endif //CACHE_OBLIVIOUS_HO_SEGMENT_TREE_H

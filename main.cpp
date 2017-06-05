#include "test_matrix.h"
#include "test_segment_tree.h"
#include <string>
using std::string;


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: [matrix|tree] [|size]\n";
        return 1;
    }
    string mod(argv[1]);
    size_t size = std::stoi(string(argv[2]));
    if (mod == "matrix") {
        test_matrix::test_matrix();
    } else if (mod == "tree") {
        test_segment_tree::test_segment_tree(size);
    } else {
        return 1;
    }
}
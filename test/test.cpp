#include <iostream>
#include <tarlib.hpp>

using namespace std;

const std::string kFile = "E:/User/dd.tar";

int main(int argc, char const *argv[]) {
    Tar tar(kFile);
    tar.write_file("E:/User/qwe/");

    return 0;
}

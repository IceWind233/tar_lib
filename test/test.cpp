#include <iostream>
#include <tarlib.hpp>

using namespace std;

const std::string kFile = "E:/User/dd.tar";

int main(int argc, char const *argv[]) {
    Tar tar(kFile);
    auto size = tar.get_all_size();
    for (auto i = 0; i < tar.length(); ++i) {
	    cout << tar.get_name(i) << " " << size[i] << endl;
	}

    return 0;
}

#include <iostream>
#include <boost/filesystem.hpp>
using std::cout;
int main()
{
    namespace fs = boost::filesystem;

    fs::path path_to_file("/home/cppcode/gitrep/leetcode/BinarySearch.js");

    cout << "as is: " << path_to_file.parent_path().string() << "\n";
    cout << "normalized: " << path_to_file.parent_path().normalize().string() << "\n";
    return 0;
}

// compile comand g++ -std=c++17 -o getDirectory.out getDirectory.cpp -lboost_filesystem -lboost_system
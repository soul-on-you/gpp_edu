#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <iostream>
#include <string>
#include <fstream>
#include <experimental/filesystem>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;

int main()
{
    std::string path = "bin1";

    cout << "size of file '" << path << "' = " << std::experimental::filesystem::file_size(path) << std::endl;

    return EXIT_SUCCESS;
}
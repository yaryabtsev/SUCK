#include <dirent.h>
#include <fstream>
#include <cstring>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

unsigned long long int reverse(unsigned long long int code, unsigned char len) {
    unsigned long long re_code = 0;
    for (short i = 0; i < len; i++, code /= 2) {
        re_code *= 2;
        re_code += code % 2;
    }
    return re_code;
}

#include "Huffman.cpp"
#include "UnHuffman.cpp"
#include "LZ77.cpp"
#include "UnLZ77.cpp"
#include <cmath>
#include <chrono>

namespace {
    std::vector<std::pair<std::string, std::string>> GetDirectoryFiles(const std::string &dir) {
        std::vector<std::pair<std::string, std::string>> files;
        std::shared_ptr<DIR> directory_ptr(opendir(dir.c_str()), [](DIR *dir) { dir && closedir(dir); });
        struct dirent *dirent_ptr;
        if (!directory_ptr) {
            std::cout << "Error opening : " << std::strerror(errno) << dir << std::endl;
            return files;
        }
        while ((dirent_ptr = readdir(directory_ptr.get())) != nullptr)
            if (!dirent_ptr->d_type) {
                int i, n = dirent_ptr->d_namlen;
                for (i = n - 1; i >= 0 && dirent_ptr->d_name[i] != '.'; i--);
                if (dirent_ptr->d_name[i] != '.' || i == 0 || i == n - 1)continue;
                files.emplace_back(std::string(dirent_ptr->d_name).substr(0, i),
                                   std::string(dirent_ptr->d_name).substr(i + 1, n));
            }
        return files;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    const std::string directory_path = "../DATA/";
    const std::vector<std::pair<std::string, std::string>> &files = GetDirectoryFiles(directory_path);
    short i;
//    std::chrono::time_point<std::chrono::system_clock> start, end;
    for (const std::pair<std::string, std::string> &file : files) {
        std::cout << file.first << '.' << file.second << std::endl;

//        start = std::chrono::system_clock::now();
//        for (i = 0; i < 10; i++)
        Huffman(directory_path, file);
//        end = std::chrono::system_clock::now();
//        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 10 << '\t';

//        start = std::chrono::system_clock::now();
//        for (i = 0; i < 10; i++)
        UnHuffman(directory_path, {file.first, "haff"});
//        end = std::chrono::system_clock::now();
//        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 10 << '\n';
//        for (i = 5; i <= 20; i *= 2) {
//            LZ77(directory_path, file, i);
////            break;
//            UnLZ77(directory_path, {file.first, "lz" + std::to_string(i)}, i);
//            break;
//        }
//
//        break;
    }
    return 0;
}

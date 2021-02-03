#include "UnHuffman.h"

UnHuffman::UnHuffman(std::string path, std::pair<std::string, std::string> file) {
    fin.open(path + file.first + '.' + file.second, std::ios::in | std::ios::binary);
    if (!fin.is_open())
        throw std::ios_base::failure("file not open, first time");
    fout.open(path + file.first + '.' + "unhaff", std::ios::out | std::ios::binary);
    loadAlfavit();
    codesGen();
    std::sort(codes, codes + size, [](symbol x, symbol y) { return x.code < y.code; });
//    std::cout << "ok\n";
    short i;
    for (i = 0; i < size; i++) {
        letters[i] = codes[i].code;
//        std::cout /*<< char(codes[i].symbol) << ' '*/ << int(codes[i].symbol) << '\t' << int(codes[i].len) << ' '
//                                                      << codes[i].code << '\n';
    }
    unhuff();
}

void UnHuffman::loadAlfavit() {
    char n[1];
    fin.read(n, 1);
    int m;
    size = (unsigned char) n[0];
    if (!size)size += 256;
    //std::cout << size << '\n';

    char buf[512];
    std::string content;
    if ((m = fin.read(buf, 2 * size).gcount()) > 0)
        for (short i = 0; i < m / 2; i++) {
            codes[i] = {(unsigned char) buf[i * 2], 0, (unsigned char) buf[i * 2 + 1]};
            // std::cout << int(codes[i].symbol) << '\t' << int(codes[i].len) << '\n';
        }
//    std::cout << "ok\n";
}

void UnHuffman::codesGen() {

    unsigned long long code = 1;
    short i, j;
    for (i = 1; i < size; i++, code++) {
        for (j = 0; j < codes[i].len - codes[i - 1].len; j++, code *= 2);
        codes[i].code = code;
    }
//    std::cout << "ok\n";
}

void UnHuffman::unhuff() {
    char n[1];
    fin.read(n, 1);
    int half_byte = (unsigned char) n[0];
    //std::cout << half_byte << '\n';
    char buf[512];
    int N;
    unsigned long long code = 0;
    short code_len = 0;
    unsigned char nb;
    char len;
    short left = 0;
    while ((N = fin.read(buf, sizeof(buf)).gcount()) > 0)
        for (int i = 0; i < N; i++) {
            len = 8;
            nb = reverse(buf[i], 8);
            while (half_byte-- > 0) {
                nb /= 2;
                len--;
            }
            while (len > 0) {
                //std::cout << nb % 2;
                code *= 2;
                code += nb % 2;
                code_len++;
                left = std::lower_bound(letters + left, letters + size, code) - letters;
                //std::cout <<code_len<<' '<< code << '\t' << left << ' ' << letters[left] << '\n';
                if (code == codes[left].code && code_len == codes[left].len) {
                    fout.write(new char[1]{char(codes[left].symbol)}, 1);
                    code = 0;
                    code_len = 0;
                    left = 0;
                }
                nb /= 2;
                len--;
            }
        }
}



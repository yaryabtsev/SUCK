#include <algorithm>
#include "LZ77.h"

LZ77::LZ77(std::string path, std::pair<std::string, std::string> file, const short window) {
    num = window;
    ahead = window / 5 * 1024;
    buffer = ahead * 4;
    fin.open(path + file.first + '.' + file.second, std::ios::in | std::ios::binary);
    if (!fin.is_open())
        throw std::ios_base::failure("file not open");
    fout.open(path + file.first + (file.second == ("lz" + std::to_string(window)) ? "*" : "") + '.' + "lz" +
              std::to_string(window), std::ios::out | std::ios::binary);
    size = (2 * (ahead + buffer));
    buf = new char[size];
    begin = 0;
    int n = fin.read(buf, size).gcount();
    end = std::min(size, n) * 8;
    int k;
    is_distance = false;
    while (end != pos) {
        //std::cout << begin << ' ' << pos << ' ' << end << '\n';
        k = write();
        pos += k;
        pos %= (size * 8);
        if (is_distance) {
            if ((begin / (size * 4)) != ((begin + k) / (size * 4))) {
                n = fin.read(buf + (begin / (size * 4)) * (ahead + buffer), (ahead + buffer)).gcount();
                end = ((begin / (size * 4)) * (ahead + buffer) + n) * 8;
            }
            begin += k;
            begin %= (size * 8);
            size -= k;
            size += (n * 8);
        } else if ((pos - begin) % size * 8 >= 8 * buffer) {
            is_distance = true;
            begin = (pos + 8 * (2 * ahead + buffer)) % size;
        }
    }
    if (len_nb) {
        while (len_nb++ != 8)
            nb *= 2;
        fout.write(new char[1]{char(nb)}, 1);
    }
    delete[]buf;
    fin.close();
    fout.close();
}


int LZ77::write() {
    lz nword = {0, 0, getBit(pos)};

    int j;
    for (int i = begin; i < pos; i++) {
        j = 0;
        while (j < ahead * 8 && pos + j != end - 1 && getBit(i + j) == getBit(pos + j))
            j++;
        if (nword.y < j)
            nword = {short(pos - i), j, getBit(pos + j)};
    }
//    fout.write(reinterpret_cast<char *>(&nword.x), sizeof nword.x);
//    fout.write(reinterpret_cast<char *>(&nword.y), sizeof nword.y);
//    fout.write(new char[1]{nword.c}, 1);
    std::cout << nword.x << '\t' << nword.y << '\t' << int(nword.c) << '\n';
    unsigned long long bit;
    int a = nword.x;
    for (j = 15; j >= 13; j -= 2, a = nword.y) {
        bit = 1 << (j + num / 10 /*+ 1*/);
        for (; bit; bit >>= 1) {
            nb *= 2;
            nb += ((a & bit) ? 1 : 0);
            //std::cout<<nb%2;
            len_nb += 1;
            if (len_nb == 8) {
                len_nb = 0;
                fout.write(new char[1]{char(nb)}, 1);
                //std::cout << int((unsigned char) nb) << ' ';
                nb = 0;
            }
        }
        //std::cout<<' ';
    }

    nb *= 2;
    nb += (nword.c ? 1 : 0);
    //std::cout << nb % 2 << '\n';
    len_nb += 1;
    if (len_nb == 8) {
        len_nb = 0;
        fout.write(new char[1]{char(nb)}, 1);
        nb = 0;
    }
    return nword.y + 1;
}

bool LZ77::getBit(int idx) {
    idx %= (size * 8);
    return (buf[idx / 8] & (1 << (7 - (idx % 8))));
}

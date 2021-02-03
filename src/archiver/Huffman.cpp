

#include "Huffman.h"

Huffman::Huffman(const std::string path, const std::pair<std::string, std::string> file) {
    fin.open(path + file.first + '.' + file.second, std::ios::in | std::ios::binary);
    if (!fin.is_open())
        throw std::ios_base::failure("file not open, first time");
    fout.open(path + file.first + (file.second == "haff" ? "*" : "") + '.' + "haff", std::ios::out | std::ios::binary);
    short i, j;
    for (i = -1; ++i < 256; codes[i] = {0, (unsigned char) i, 0, (unsigned char) 0});
    count();
    std::sort(codes, codes + 256, [](symbol x, symbol y) { return x.count < y.count; });
    for (i = 255; i >= 0 && codes[i].count; i--)
        addChance(i, codes[i].count);
    minimize();
    std::sort(codes, codes + 256, [](symbol x, symbol y) {
        return (x.len < y.len ? true : (x.len == y.len ? x.symbol < y.symbol : false));
    });
    unsigned long long code = 1;
    for (i = 0; i < 256 && !codes[i].len; i++);
    //std::cout << 256 - i << '\n';
    fout.write(new char[1]{char(256 - i)}, 1);
    //std::cout << char(codes[i].symbol) << ' ' << int(codes[i].symbol) << '\t' << codes[i].count << '\t'
    //          << int(codes[i].len) << ' ' << codes[i].code << '\n';
    fout.write(new char[2]{char(codes[i].symbol), char(codes[i].len)}, 2);
    half_byte = codes[i].len * codes[i].count;
    for (i += 1; i < 256; i++) {
        for (j = 0; j < codes[i].len - codes[i - 1].len; j++, code *= 2);
        codes[i].code = reverse(code++, codes[i].len);
        //std::cout << char(codes[i].symbol) << ' ' << int(codes[i].symbol) << '\t' << codes[i].count << '\t'
        //          << int(codes[i].len) << ' ' << codes[i].code << '\n';
        fout.write(new char[2]{char(codes[i].symbol), char(codes[i].len)}, 2);
        half_byte += codes[i].len * codes[i].count;
    }
    half_byte %= 8;
    half_byte = 8 - half_byte;
    //std::cout << int(half_byte) << '\n';
    fout.write(new char[1]{char(half_byte)}, 1);
    std::sort(codes, codes + 256, [](symbol x, symbol y) { return x.symbol < y.symbol; });
    fin.clear();
    fin.seekg(0, std::ios_base::beg);
    write();
    fin.close();


    fout.close();
    for (i = 0; i < 256; i++)
        delete[]sums[i];
    delete new_node;

}

void Huffman::write() {
    char buf[512];
    std::string content;
    short n;
    unsigned long long curr_ch;
    int len = half_byte;
    char new_ch = 0;
    short i, j;
    while ((n = fin.read(buf, sizeof(buf)).gcount()) > 0)
        for (i = 0; i < n; i++) {
            curr_ch = codes[(unsigned char) buf[i]].code, j = codes[(unsigned char) buf[i]].len;
            //std::cout << curr_ch << '\t' << j << '\n';
            for (; j; j--, len++) {
                if (len == 8) {
                    len = 0;
                    fout.write(new char[1]{new_ch}, 1);
                    //std::cout << int(new_ch) << '\t';
                    new_ch = 0;
                }
                new_ch *= 2;
                new_ch += curr_ch % 2;
                curr_ch /= 2;
                //std::cout << int(new_ch) << '\t' << len << '\n';
            }
        }
    while (len++ != 8)
        new_ch *= 2;
    fout.write(new char[1]{new_ch}, 1);
    //std::cout << int(new_ch) << '\n';
}


void Huffman::count() {
    char buf[512];
    int n;
    while ((n = fin.read(buf, sizeof(buf)).gcount()) > 0)
        for (int i = 0; i < n; codes[(unsigned char) buf[i++]].count += 1);

}

void Huffman::addChance(short idx, unsigned long chance) {
    size++;
    sums[size] = new node(chance, idx, nullptr, nullptr);
}

void Huffman::minimize() {
    int n = size;
    while (n > 0) {
        inc(sums[n]);
        inc(sums[n - 1]);
        new_node = new node(sums[n]->sum + sums[n - 1]->sum, -1, sums[n - 1], sums[n]);
        std::swap(sums[n - 1], new_node);

        n--;
        int i = n;
        while (i > 0 && sums[i]->sum > sums[i - 1]->sum) {
            std::swap(sums[i], sums[i - 1]);
            i--;
        }
    }
}

Huffman::node::node(unsigned long sum, int idx, Huffman::node *left = nullptr, Huffman::node *right = nullptr) {
    this->sum = sum;
    this->idx = idx;
    this->left = left;
    this->right = right;
}

#include "UnLZ77.h"

UnLZ77::UnLZ77(std::string path, std::pair<std::string, std::string> file, const short num) {

    fin.open(path + file.first + '.' + file.second, std::ios::in | std::ios::binary);
    if (!fin.is_open())
        throw std::ios_base::failure("file not open");
    fout.open(path + file.first + (file.second == ("unlz" + std::to_string(num)) ? "*" : "") + '.' + "unlz" +
              std::to_string(num), std::ios::out | std::ios::binary);
    int n = 16 + 14 + 1 + (num / 10) * 2;
    buf = new char[8 * n];
    int m;
    short i, j, idx = 0, block;
    int len1, len2, len3;
    len1 = 16 + (num / 10);
    len2 = len1 + 14 + (num / 10);
    len3 = len2 + 1;
    int num1, num2;
    bool num3;
    int _num = 0;
    std::cout << "===\n";
    while ((m = fin.read(buf, 8 * n).gcount()) > 0) {
        for (i = 0; i < m; i++)
            buf[i] = reverse(buf[i], 8);
        for (i = 0; i < m; i++) {
            for (j = 0; j < 8; j++) {
                idx++;
                _num *= 2;
                _num += ((buf[i] % 2 == 0) ? 0 : 1);
                buf[i] /= 2;
                if (idx == len1) {
                    num1 = _num;
                    _num = 0;
                } else if (idx == len2) {
                    num2 = _num;
                    _num = 0;
                } else if (idx == len3) {
                    num3 = _num;
                    _num = 0;
                    idx = 0;
                    std::cout << num1 << '\t' << num2 << '\t' << int(num3) << '\n';
                }
            }
        }

    }

    fin.close();
    fout.close();
    delete[]buf;
}
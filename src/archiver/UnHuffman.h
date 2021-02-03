
class UnHuffman {

public:
    UnHuffman(const std::string path, const std::pair<std::string, std::string> file);
private:
    std::ifstream fin;
    std::ofstream fout;
    struct symbol {
        unsigned char symbol;
        unsigned long long code = 0;
        unsigned char len;
    } codes[256];
    unsigned long long letters[256];
    void loadAlfavit();
    short size;
    void codesGen();


    void unhuff();
};


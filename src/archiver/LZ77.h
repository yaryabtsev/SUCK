
class LZ77 {

public:
    LZ77(const std::string path, const std::pair<std::string, std::string> file, const short buffer);

private:
    std::ifstream fin;
    std::ofstream fout;
    int buffer;
    int ahead;
    struct lz {
        int x, y;
        bool c;
    };
    bool is_distance;
    char *buf = nullptr;
    int pos = 0;
    int len, begin=0, end;
    int size=0;
    int write();
    short num=5;
    bool getBit(int pos);
    unsigned char nb=0;
    short len_nb=0;
};


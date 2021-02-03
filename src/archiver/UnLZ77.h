
class UnLZ77 {

public:
    UnLZ77(const std::string path, std::pair<std::string, std::string> file, short buffer);
private:
    std::ifstream fin;
    std::ofstream fout;
    char *buf = nullptr;
};


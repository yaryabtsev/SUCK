
class Huffman {
public:

    Huffman(const std::string path, const std::pair<std::string, std::string> file);

    class node {
    public:
        node(unsigned long sum, int idx, node *left, node *right);

        unsigned long sum;
        short idx;
        node *left = nullptr;
        node *right = nullptr;
    };

private:
    std::ifstream fin;
    std::ofstream fout;
    short size = -1;
    node *sums[256];
    node *new_node = nullptr;
    unsigned char half_byte;

    void count();


    void inc(const node *nd) {
        if (nd->idx == -1) {
            inc(nd->left);
            inc(nd->right);
        } else
            codes[nd->idx].len += 1;
    }

    struct symbol {
        unsigned long count;
        unsigned char symbol;
        unsigned long long code = 0;
        unsigned char len;
    } codes[256];

    void addChance(short idx, unsigned long chance);

    void minimize();

    void write();
};
#include <iostream>
#include <string>
#include <cstdint>
#include <algorithm>

void error() { throw std::invalid_argument("error"); }

class Calculator {
private:
    const std::string str;
    const size_t size;
    size_t i;

    int64_t item(size_t j) {
        std::string num;
        if (str[j] == '-') {
            num += '-';
            ++j;
        }
        while(j < size && isdigit(str[j]))
            num += str[j++];
        i = j;
        if (num == "-" || num == "") error();
        return std::stoi(num);
    }
    
    int64_t term(size_t j) {
        int64_t a = item(j);
        while (i < size) {
            char b = str[i];
            if (b == '*') {
                int64_t c = item(++i);
                a *= c;
            }
            else if (b == '/') {
                int64_t c = item(++i);
                if (c != 0)
                    a /= c;
                else error();
            }
            else if (b == '+' || b == '-')
                break;
            else
                error();
        }
        return a;
    }
    
    int64_t expr(size_t j) {
        int64_t a = term(j);
        while (i < size) {
            char b = str[i];
            if (b == '+') {
                int64_t c = term(++i);
                a += c;
            }
            else if (b == '-') {
                int64_t c = term(++i);
                a -= c;
            }
            else if (b == '*' || b == '/')
                break;
            else error();
        }
        return a;
    }
        
public:
    Calculator(const std::string& s)
        : str(s), size(s.size()), i(0) {}
        
    int64_t eval() { return expr(0); }
};

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "error" << std::endl;
        return 1;
    }
    std::string str = argv[1];
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    Calculator c(str);
    try {
        std::cout << c.eval() << std::endl;
    } 
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;;
        return 1;
    }
    return 0;
}

#include <iostream>
#include <cmath>
#include <vector>
#include "numbers.dat"

const int maxval = 99999;

bool is_prime(int n) {
    if (n == 2)
        return true;
    if (n == 1 || n % 2 == 0)
        return false;
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0)
            return false;
    }
    return true;
}

int bin(int n) {
    int l = 0;
    int r = Size - 1;
    int m = 0;
    while (l <= r) {
        m = floor((l + r) / 2);
        if (Data[m] < n)
            l = m + 1;
        else if (Data[m] > n)
            r = m - 1;
        else
            return m;
    }
    return -1;
}

int main(int argc, char** argv) {
    if (argc < 3 || argc % 2 == 0)
        return 0;
    std::vector<int> v(argc - 1);
    for (int i = 0; i < argc - 1; ++i) {
        v[i] = std::atoi(argv[i + 1]);
    }
    for (int i = 0; i < argc - 1; i += 2) {
        if (v[i] > v[i + 1]) {
            std::cout << 0 << std::endl;
            continue;
        }
        int num = 0, left = bin(v[i]), right = bin(v[i + 1]);
        if (left == -1 || right == -1) {
            std::cout << 0 << std::endl;
            continue;
        }
        while (left != 0) {
            if (Data[left - 1] == v[i])
                --left;
            else
                break;
        }
        while (right != Size - 1) {
            if (Data[right + 1] == v[i+1])
                ++right;
            else
                break;
        }
        for (int j = left; j <= right; ++j) {
            if (is_prime(Data[j]))
                num += 1;
        }
        std::cout << num << std::endl;
    }
    return 0;
}

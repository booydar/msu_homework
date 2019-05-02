#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <cstdint>
#include <iterator>
#include <thread>

size_t size = 10;
size_t M, N = 1;
std::mutex mutex;

void read(std::ifstream& in, std::vector<uint64_t>& v);
void clear(size_t k);
size_t merge(size_t a, size_t b);


int main() {
    std::vector<uint64_t> v(size);
    std::ifstream in("numbers.txt", std::ios::binary);
    std::thread t1(read, std::ref(in), std::ref(v));
    std::thread t2(read, std::ref(in), std::ref(v));
    t1.join();
    t2.join();
    M = N;
    if (N > 2) {
        size_t j = merge(1, 2);
        for (int i = 3; i <= N; ++i)
            j = merge(j, i);
    }
    std::cout << "Result in file " << M - 1 << ".txt" << "\n";
    return 0;
}

void read(std::ifstream& in, std::vector<uint64_t>& v) {
    while(in.peek() != EOF) {
        size_t i = 0;
        uint64_t x;
        mutex.lock();
        while (i < size && in >> x) {
            v[i] = x;
            ++i;
        }
        mutex.unlock();
        if (i < size)
            std::sort(begin(v), begin(v) + i);
        else
            std::sort(begin(v), end(v));
        mutex.lock();
        std::ofstream out(std::to_string(N) + ".txt", std::ios::binary);
        for (size_t j = 0; j < i; ++j)
            out << v[j] << " ";
        ++N;
        mutex.unlock();
    }
}

void clear(size_t k) {
    std::string s = std::to_string(k) + ".txt";
    const char* file = s.c_str();
    std::remove(file);
}

size_t merge(size_t a, size_t b) {
    std::ofstream merged(std::to_string(M) + ".txt", std::ios::binary);
    std::ifstream in1(std::to_string(a) + ".txt", std::ios::binary);
    std::ifstream in2(std::to_string(b) + ".txt", std::ios::binary);
    std::merge(std::istream_iterator<uint64_t>(in1),
              std::istream_iterator<uint64_t>(), 
              std::istream_iterator<uint64_t>(in2),
              std::istream_iterator<uint64_t>(),
              std::ostream_iterator<uint64_t>(merged, " "));
    clear(a);
    clear(b);
    return M++;
}

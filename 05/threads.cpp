#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>

std::mutex mutex;
std::condition_variable cv;
bool ping = 0;
bool pong = 1;

void print(const std::string& s, bool& j) {
    for (int i = 0; i < 500000; ++i) {
        std::unique_lock<std::mutex> lock(mutex);
        if (j == ping) {
            while (!pong)
                cv.wait(lock);
            std::cout << s << "\n";
            ping = !ping;
            pong = !pong;
            cv.notify_one();
        } else {
            while (!ping)
                cv.wait(lock);
            std::cout << s << "\n";
            ping = !ping;
            pong = !pong;
            cv.notify_one();
        }
    }
}

int main() {
    std::thread ping_t(print, "ping", std::ref(ping));
    std::thread pong_t(print, "pong", std::ref(pong));
    ping_t.join();
    pong_t.join();
    return 0;
}

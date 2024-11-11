// System.h
#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <chrono>
#include <thread>

class System {
public:
    void myFunction() {
        std::cout << "60초가 지났습니다! 특정 함수가 실행됩니다." << std::endl;
    }

    void startTimer() {
        auto start = std::chrono::steady_clock::now();
        while (true) {
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed_seconds = now - start;

            if (elapsed_seconds.count() >= 60) {
                myFunction();
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
};

#endif // SYSTEM_H

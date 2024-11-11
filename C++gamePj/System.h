// System.h
#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include <codecvt>
#include <locale>

using namespace std;

class System {
public:
    // 폰트와 텍스트 변수
    sf::Font font;
    sf::Text elapsedText;

    // 생성자
    System() {
        // 폰트 로드
        if (!font.loadFromFile("gulim.ttc")) { // 폰트 파일 경로를 적절히 수정하세요
            cerr << "폰트를 로드할 수 없습니다." << endl;
            exit(-1); // 프로그램 종료
        }

        // 텍스트 설정
        elapsedText.setFont(font);
        elapsedText.setCharacterSize(24); // 문자 크기
        elapsedText.setFillColor(sf::Color::White); // 텍스트 색상
        elapsedText.setPosition(10.f, 10.f); // 텍스트 위치
    }

    void myFunction() {
        wcout << L"60초가 지났습니다! 특정 함수가 실행됩니다." << endl;
    }

    void startTimer() {
        auto start = chrono::steady_clock::now();
        while (true) {
            auto now = chrono::steady_clock::now();
            chrono::duration<double> elapsed_seconds = now - start;

            // 와이드 문자열로 경과 시간 설정
            wstring elapsedTime = L"경과 시간: " + to_wstring(static_cast<int>(elapsed_seconds.count())) + L"초";
            elapsedText.setString(convertToString(elapsedTime)); // 변환 후 설정

            if (elapsed_seconds.count() >= 60) {
                myFunction();
                break; // 루프 종료
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    // 텍스트를 반환하는 메서드
    sf::Text getElapsedText() {
        return elapsedText;
    }

private:
    // wstring을 string으로 변환하는 메서드
    std::string convertToString(const std::wstring& wstr) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    }
};

#endif // SYSTEM_H

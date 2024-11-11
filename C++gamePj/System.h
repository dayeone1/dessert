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
    // ��Ʈ�� �ؽ�Ʈ ����
    sf::Font font;
    sf::Text elapsedText;

    // ������
    System() {
        // ��Ʈ �ε�
        if (!font.loadFromFile("gulim.ttc")) { // ��Ʈ ���� ��θ� ������ �����ϼ���
            cerr << "��Ʈ�� �ε��� �� �����ϴ�." << endl;
            exit(-1); // ���α׷� ����
        }

        // �ؽ�Ʈ ����
        elapsedText.setFont(font);
        elapsedText.setCharacterSize(24); // ���� ũ��
        elapsedText.setFillColor(sf::Color::White); // �ؽ�Ʈ ����
        elapsedText.setPosition(10.f, 10.f); // �ؽ�Ʈ ��ġ
    }

    void myFunction() {
        wcout << L"60�ʰ� �������ϴ�! Ư�� �Լ��� ����˴ϴ�." << endl;
    }

    void startTimer() {
        auto start = chrono::steady_clock::now();
        while (true) {
            auto now = chrono::steady_clock::now();
            chrono::duration<double> elapsed_seconds = now - start;

            // ���̵� ���ڿ��� ��� �ð� ����
            wstring elapsedTime = L"��� �ð�: " + to_wstring(static_cast<int>(elapsed_seconds.count())) + L"��";
            elapsedText.setString(convertToString(elapsedTime)); // ��ȯ �� ����

            if (elapsed_seconds.count() >= 60) {
                myFunction();
                break; // ���� ����
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    // �ؽ�Ʈ�� ��ȯ�ϴ� �޼���
    sf::Text getElapsedText() {
        return elapsedText;
    }

private:
    // wstring�� string���� ��ȯ�ϴ� �޼���
    std::string convertToString(const std::wstring& wstr) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    }
};

#endif // SYSTEM_H

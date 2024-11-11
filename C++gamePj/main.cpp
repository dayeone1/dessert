#include <SFML/Graphics.hpp>
#include "System.h" // System.h 헤더 파일 포함

const int WINDOW_WIDTH = 1400;
const int WINDOW_HEIGHT = 600;

int main() {
    // 창 생성
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML 게임");

    // 도형 생성 (예: 원)
    sf::CircleShape circle(50); // 반지름 50의 원
    circle.setFillColor(sf::Color::Green); // 색상 설정
    circle.setPosition(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 50); // 중앙에 위치

    // System 클래스 인스턴스 생성
    System system;

    // 타이머를 별도의 스레드에서 실행
    std::thread timerThread(&System::startTimer, &system);

    // 창이 열려 있는 동안 루프 실행
    while (window.isOpen()) {
        // 이벤트 처리
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close(); // 창 닫기 이벤트 처리
        }

        // 화면 지우기
        window.clear(sf::Color::Black);

        // 도형 그리기
        window.draw(circle);

        // 화면에 그리기
        window.display();
    }

    // 타이머 스레드 종료 대기
    timerThread.join();

    return 0;
}

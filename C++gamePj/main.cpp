#include <SFML/Graphics.hpp>
#include "System.h" // System.h ��� ���� ����

const int WINDOW_WIDTH = 1400;
const int WINDOW_HEIGHT = 600;

int main() {
    // â ����
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML ����");

    // ���� ���� (��: ��)
    sf::CircleShape circle(50); // ������ 50�� ��
    circle.setFillColor(sf::Color::Green); // ���� ����
    circle.setPosition(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 50); // �߾ӿ� ��ġ

    // System Ŭ���� �ν��Ͻ� ����
    System system;

    // Ÿ�̸Ӹ� ������ �����忡�� ����
    std::thread timerThread(&System::startTimer, &system);

    // â�� ���� �ִ� ���� ���� ����
    while (window.isOpen()) {
        // �̺�Ʈ ó��
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close(); // â �ݱ� �̺�Ʈ ó��
        }

        // ȭ�� �����
        window.clear(sf::Color::Black);

        // ���� �׸���
        window.draw(circle);

        // ȭ�鿡 �׸���
        window.display();
    }

    // Ÿ�̸� ������ ���� ���
    timerThread.join();

    return 0;
}

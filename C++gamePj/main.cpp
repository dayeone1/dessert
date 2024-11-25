#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include <vector>

using namespace std;

// 창넘김 if문에 들어갈 상수
enum screen_number {
    Start,
    Main,
    BadEnding,
    HappyEnding
};

int screen_num = screen_number::Start;

class Button {
public:
    // 기본 이미지, hover 이미지, 각각의 좌표값, 함수
    Button(const string& normalImagePath, const string& hoverImagePath,
           float x, float y, float x1, float y1, function<void()> onClick)
        : onClick(onClick) {
        if (!buttonImg.loadFromFile(normalImagePath) ||
            !buttonClickImg.loadFromFile(hoverImagePath)) {
            cerr << "Error loading images" << endl;
        }

        sprite.setTexture(buttonImg);
        sprite.setPosition(x, y);

        hoverSprite.setTexture(buttonClickImg);
        hoverSprite.setPosition(x1, y1); // 호버 이미지 위치 설정
    }

    void draw(sf::RenderWindow& window) {
        if (isHovered) {
            window.draw(hoverSprite);
        } else {
            window.draw(sprite);
        }
    }

    // 이미지 위에 마우스가 있는지 확인
    void checkHover(sf::Vector2f mousePos) {
        isHovered = sprite.getGlobalBounds().contains(mousePos);
    }

    // 클릭 확인
    void checkClick(sf::Vector2f mousePos) {
        if (sprite.getGlobalBounds().contains(mousePos)) {
            onClick();
        }
    }

private:
    sf::Texture buttonImg;        // 버튼 일반 이미지
    sf::Texture buttonClickImg;   // 버튼 클릭 이미지
    sf::Sprite sprite;            // 일반 이미지 스프라이트
    sf::Sprite hoverSprite;       // 클릭 이미지 스프라이트
    function<void()> onClick;
    bool isHovered = false;        // 현재 호버 상태
};


class PlayScreen {
public:
    PlayScreen() {

    }
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
};

class StartScreen {
public:
    StartScreen()
        :
        button_start("img/start/startButton_default.png", "img/start/startButton_start.png",
            855, 270, 740, 210, []() { // 호버 이미지 위치 변경
                cout << "Start button clicked!" << endl;
                screen_num = screen_number::Main; // 상태 변경
            }),
          button_rule("img/start/startButton_default1.png", "img/start/startButton_rule.png",
                      705, 270, 590, 210, []() { // 호버 이미지 위치 변경
                           cout << "Rule button clicked!" << endl;
                           // 룰 설명 이미지
                       })
    {
        if (!backgroundTexture.loadFromFile("img/background/start_back.png")) {
            cerr << "Error loading background image" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(backgroundSprite);
        button_rule.draw(window);
        button_start.draw(window);
    }

    void checkButtonClick(sf::Vector2f mousePos) {
        button_start.checkClick(mousePos);
        button_rule.checkClick(mousePos);
    }

    void checkButtonHover(sf::Vector2f mousePos) {
        button_start.checkHover(mousePos);
        button_rule.checkHover(mousePos);
    }

private:
    // 배경 텍스처 & 스크립트
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    Button button_start; 
    Button button_rule;
};

// 메인
int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 650), "test");

    StartScreen start_screen; // 버튼이 포함된 StartScreen 객체 생성

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (screen_num == screen_number::Start) {
                    start_screen.checkButtonClick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                }
            }
        }

        sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
        if (screen_num == screen_number::Start) {
            start_screen.checkButtonHover(mousePos); // 버튼 호버 체크
        }

        window.clear();
        if (screen_num == screen_number::Start) {
            start_screen.draw(window);
        }
        window.display();
    }
    return 0;
}

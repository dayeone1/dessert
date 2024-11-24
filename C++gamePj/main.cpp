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
    // 이미지 2개, x, y, x1, y1, 클릭 시 이벤트
    Button(const string& normalImagePath, const string& hoverImagePath,
        float x, float y, float x1, float y1, function<void()> onClick)
        : onClick(onClick) {
        // 이미지 로드
        if (!buttonImg.loadFromFile(normalImagePath) ||
            !buttonClickImg.loadFromFile(hoverImagePath)) {
            cerr << "Error loading images" << endl;
        }

        // 일반 이미지 스프라이트 설정
        setPosition(x, y);
        sprite.setTexture(buttonImg);

        // 클릭 이미지 스프라이트 설정
        setHoverPosition(x1, y1);
        hoverSprite.setTexture(buttonClickImg);
    }

    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

    void setHoverPosition(float x1, float y1) {
        hoverSprite.setPosition(x1, y1);
    }

    void draw(sf::RenderWindow& window) {
        // 현재 상태에 따라 적절한 스프라이트를 그리기
        if (isHovered) {
            window.draw(hoverSprite);
        }
        else {
            window.draw(sprite);
        }
    }

    void checkHover(sf::Vector2f mousePos) {
        isHovered = sprite.getGlobalBounds().contains(mousePos);
    }

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

class StartScreen {
public:
    StartScreen() {
        // 텍스처 및 스프라이트 생성
        if (!backgroundTexture.loadFromFile("img/background/start_back.png")) {
            cerr << "Error loading background image" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);

        // 버튼 생성
        buttons.emplace_back("img/start/startButton_default.png", "img/start/startButton_rule.png",
            705, 270, 590, 210, []() {
                cout << "Rule button clicked!" << endl;
                // 여기서 화면 전환 로직 추가 가능
            });
        buttons.emplace_back("img/start/startButton_default.png", "img/start/startButton_start.png",
            855, 270, 745, 210, []() {
                cout << "Start button clicked!" << endl;
                // 규칙 버튼 클릭 시 로직 추가 가능
            });
    }

    void draw(sf::RenderWindow& window) {
        window.draw(backgroundSprite);
        for (auto& button : buttons) {
            button.draw(window);
        }
    }

    void checkButtonClick(sf::Vector2f mousePos) {
        for (auto& button : buttons) {
            button.checkClick(mousePos);
        }
    }

    void checkButtonHover(sf::Vector2f mousePos) {
        for (auto& button : buttons) {
            button.checkHover(mousePos);
        }
    }

private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    vector<Button> buttons; // 버튼 벡터
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

        // 마우스 위치 체크
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
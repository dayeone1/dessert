#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include <string>

using namespace std;

// 화면 상태를 나타내는 열거형
enum screen_number {
    Start,
    Main,
    Happy_Ending,
    Bad_Ending,
    Rule,
};

int screen_num = screen_number::Start;
int score = -1;

class Button {
public:
    Button() {};
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
        hoverSprite.setPosition(x1, y1);
    }

    Button(const string& normalImagePath, const string& hoverImagePath,
           float x, float y, function<void()> onClick)
        : onClick(onClick) {
        if (!buttonImg.loadFromFile(normalImagePath) ||
            !buttonClickImg.loadFromFile(hoverImagePath)) {
            cerr << "Error loading images" << endl;
        }

        sprite.setTexture(buttonImg);
        sprite.setPosition(x, y);
        hoverSprite.setTexture(buttonClickImg);
        hoverSprite.setPosition(x, y);
    }

    void draw(sf::RenderWindow& window) {
        if (isHovered) {
            window.draw(hoverSprite);
        } else {
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

class Screen {
public:
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void checkButtonClick(sf::Vector2f mousePos) = 0;
    virtual void checkButtonHover(sf::Vector2f mousePos) = 0;
    virtual ~Screen() {} // 가상 소멸자
};

class EndingScreen : public Screen {
public:
    EndingScreen(int num)
        :button_start("img/start/startButton_default.png", "img/start/startButton_start.png", 1086, 450, 960, 380, []() {
        cout << "Start button clicked!" << endl;
        screen_num = screen_number::Main; // 상태 변경
        })
    {
        if (num == screen_number::Happy_Ending)
            img = "img/background/happy_end.png";
        else if (num == screen_number::Bad_Ending)
            img = "img/background/bad_end.png";
        else
            img = "img/background/rule.png";

        if (!backgroundTexture.loadFromFile(img)) {
            cerr << "Error loading main background image" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(backgroundSprite);
        if (score > -1) {
            sf::Font font;
            if (!font.loadFromFile("font/gulim.ttc")) { // 적절한 폰트 경로로 수정
                cerr << "Error loading font" << endl;
            }
            sf::Text scoreText;
            scoreText.setFont(font);
            scoreText.setString(to_string(score)+" 원");
            scoreText.setCharacterSize(30); // 글자 크기 설정
            scoreText.setFillColor(sf::Color::Black); // 글자 색상 설정
            scoreText.setPosition(933, 227); // 점수 위치 설정

            window.draw(scoreText); // 점수 텍스트 그리기
        }
        button_start.draw(window);
    }

    void checkButtonClick(sf::Vector2f mousePos) override {
        // 엔딩 화면의 버튼 클릭 처리
        button_start.checkClick(mousePos);
    }

    void checkButtonHover(sf::Vector2f mousePos) override {
        // 엔딩 화면의 버튼 호버 처리
        button_start.checkHover(mousePos);
    }

private:
    Button button_start;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    string img;
};

class MainScreen : public Screen {
public:
    MainScreen() {
        // 버튼 초기화
        // 0, 1, 2 -> 아이스크림 3, 4, 5 -> 생크림 6, 7 -> 생크림 8 -> 컵 
        // 9-> 빵 10, 11, 12-> 토핑 13-> 쓰레기통 14-> 작업대

        //왜 배열안에 넣기만 하면 오류가 나는거ㅛㅈ???////////?//////'/
        button_area[0] = Button("img/main/obj_box/ice_box.png", "", 84, 169, [this]() {
            cout << "Ice Box clicked!" << endl;
            event = 0;
            isFollowingMouse = true; // 마우스를 따라다니는 것을 활성화
            });

        button_area[1] = Button("img/main/obj_box/ice_box.png", "", 85, 260, [this]() {
            cout << "Another Box clicked!" << endl;
            event = 1;
            isFollowingMouse = true;
            });

        button_area[2] = Button("img/main/obj_box/ice_box.png", "", 85, 358, [this]() {
            cout << "Another Box clicked!" << endl;
            event = 2;
            isFollowingMouse = true;
            });

        button_area[3] = Button("img/main/obj_box/cream_box.png", "", 202, 160, [this]() {
            cout << "Another Box clicked!" << endl;
            event = 3;
            isFollowingMouse = true;
            });

        button_area[4] = Button("img/main/obj_box/cream_box.png", "", 202, 255, [this]() {
            cout << "Another Box clicked!" << endl;
            event = 4;
            isFollowingMouse = true;
            });

        button_area[5] = Button("img/main/obj_box/cream_box.png", "", 202, 358, [this]() {
            cout << "Another Box clicked!" << endl;
            event = 5;
            isFollowingMouse = true;
            });

        button_area[6] = Button("img/main/obj_box/fruit_box.png", "", 65, 497, [this]() {
            cout << "Another Box clicked!" << endl;
            event = 6;
            isFollowingMouse = true;
            });

        button_area[7] = Button("img/main/obj_box/fruit_box.png", "", 191, 497, [this]() {
            cout << "Another Box clicked!" << endl;
            event = 7;
            isFollowingMouse = true;
            });

        button_area[8] = Button("img/main/obj_box/cup_box.png", "", 387, 90, [this]() {
            cout << "Another Box clicked!" << endl;
            event = 8;
            isFollowingMouse = true;
            });

        button_area[9] = Button("img/main/obj_box/bread_box.png", "", 387, 269, [this]() {
            cout << "Another Box clicked!" << endl;
            event = 9;
            isFollowingMouse = true;
            });

        button_area[10] = Button("img/main/obj_box/topping_box.png", "", 392, 365, [this]() {
            cout << "Another Box clicked!" << endl;
            event = 10;
            isFollowingMouse = true;
            });

        button_area[11] = Button("img/main/obj_box/topping_box.png", "", 484, 365, [this]() {
            cout << "Another Box clicked!" << endl;
            event = 11;
            isFollowingMouse = true;
            });

        button_area[12] = Button("img/main/obj_box/topping_box.png", "", 576, 365, [this]() {
            cout << "Another Box clicked!" << endl;
            event = 12;
            isFollowingMouse = true;
            });

        button_area[13] = Button("img/main/obj_box/trash_box.png", "", 1014, 502, [this]() {
            cout << "Another Box clicked!" << endl;
            isFollowingMouse = false;
            event = -1;
            });

        button_area[14] = Button("img/main/obj_box/bench_box.png", "", 703, 269, [this]() {
            cout << "Another Box clicked!" << endl;
            isFollowingMouse = false;


            event = -1;
            });

        // 배경 및 기타 요소 초기화
        backgroundTexture.loadFromFile("img/background/main_back.png");
        backgroundSprite.setTexture(backgroundTexture);
        tableTexture.loadFromFile("img/main/table.png");
        tableSprite.setTexture(tableTexture);
        tableSprite.setPosition(690, 278);
        messageTexture.loadFromFile("img/main/message.png");
        messageSprite.setTexture(messageTexture);
        messageSprite.setPosition(690, 10);
        objTexture.loadFromFile("img/main/obj.png");
        objSprite.setTexture(objTexture);

        for (int i = 0; i < 3; i++) {
            charTexture[i].loadFromFile("img/main/char_" + to_string(i) + ".png");
            charSprite[i].setTexture(charTexture[i]);
            charSprite[i].setPosition(855, 100);
        }

        for (int i = 0; i < 13; i++) {
            followTexture[i].loadFromFile("img/main/obj_mouse/follow_" + to_string(i) + ".png");
            followSprite[i].setTexture(followTexture[i]);
            followSprite[i].setPosition(-100, -100);
        }

    }

    void draw(sf::RenderWindow& window) override {
        window.draw(backgroundSprite);
        window.draw(charSprite[emotion]);
        window.draw(messageSprite);
        window.draw(tableSprite);
        window.draw(objSprite);

        for (int i = 0; i < 15; i++) {
            button_area[i].draw(window);
        }

        /*button_area[0].draw(window);
        button_area[1].draw(window);
        button_area[2].draw(window);
        button_area[3].draw(window);
        button_area[4].draw(window);
        button_area[5].draw(window);
        button_area[6].draw(window);
        button_area[7].draw(window);
        button_area[8].draw(window);
        button_area[9].draw(window);
        button_area[10].draw(window);
        button_area[11].draw(window);
        button_area[12].draw(window);
        button_area[13].draw(window);
        button_area[14].draw(window);*/

        // 마우스를 따라다니는 이미지가 활성화 됐을 경우 그리기
        if (isFollowingMouse) {
            window.draw(followSprite[event]);
        }
    }

    void checkButtonClick(sf::Vector2f mousePos) override {
        for (int i = 0; i < 15; i++) {
            button_area[i].checkClick(mousePos);
        }
        /*button_area[0].checkClick(mousePos);
        button_area[1].checkClick(mousePos);
        button_area[2].checkClick(mousePos);
        button_area[3].checkClick(mousePos);
        button_area[4].checkClick(mousePos);
        button_area[5].checkClick(mousePos);
        button_area[6].checkClick(mousePos);
        button_area[7].checkClick(mousePos);
        button_area[8].checkClick(mousePos);
        button_area[9].checkClick(mousePos);
        button_area[10].checkClick(mousePos);
        button_area[11].checkClick(mousePos);
        button_area[12].checkClick(mousePos);
        button_area[13].checkClick(mousePos);
        button_area[14].checkClick(mousePos);*/
    }

    void checkButtonHover(sf::Vector2f mousePos) override {
        for (int i = 0; i < 15; i++) {
            button_area[i].checkHover(mousePos);
        }
        /*button_area[0].checkHover(mousePos);
        button_area[1].checkHover(mousePos);
        button_area[2].checkHover(mousePos);
        button_area[3].checkHover(mousePos);
        button_area[4].checkHover(mousePos);
        button_area[5].checkHover(mousePos);
        button_area[6].checkHover(mousePos);
        button_area[7].checkHover(mousePos);
        button_area[8].checkHover(mousePos);
        button_area[9].checkHover(mousePos);
        button_area[10].checkHover(mousePos);
        button_area[11].checkHover(mousePos);
        button_area[12].checkHover(mousePos);
        button_area[13].checkHover(mousePos);
        button_area[14].checkHover(mousePos);*/
    }

    void updateFollowSpritePosition(sf::Vector2f mousePos) {
        if (isFollowingMouse) {
            // 마우스로 이미지 클릭 시, 마우스의 위치-이미지의 크기/2의 위치에 이미지를 띄움
            followSprite[event].setPosition(mousePos.x - followSprite[event].getGlobalBounds().width / 2,
                mousePos.y - followSprite[event].getGlobalBounds().height / 2);
        }
    }

private:
    int emotion = 0;
    int event = -1;
    int score = 0;
    bool isFollowingMouse = false; // 마우스를 따라다닐지 여부

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture objTexture;
    sf::Sprite objSprite;
    sf::Texture tableTexture;
    sf::Sprite tableSprite;
    sf::Texture messageTexture;
    sf::Sprite messageSprite;

    sf::Texture charTexture[3];
    sf::Sprite charSprite[3];

    sf::Texture followTexture[13]; // 마우스를 따라다닐 이미지 텍스처
    sf::Sprite followSprite[13];    // 마우스를 따라다닐 이미지 스프라이트

    Button button_area[15];
};

class StartScreen : public Screen {
public:
    StartScreen()
        :button_start("img/start/startButton_default.png", "img/start/startButton_start.png", 855, 270, 740, 210, []() {
            cout << "Start button clicked!" << endl;
            score = 0;
            screen_num = screen_number::Main; // 상태 변경
        }),
        button_rule("img/start/startButton_default1.png", "img/start/startButton_rule.png", 705, 270, 590, 210, []() {
            cout << "Rule button clicked!" << endl;
            screen_num = screen_number::Rule;
        }) 
    {
        if (!backgroundTexture.loadFromFile("img/background/start_back.png")) {
            cerr << "Error loading background image" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(backgroundSprite);
        button_rule.draw(window);
        button_start.draw(window);
    }

    void checkButtonClick(sf::Vector2f mousePos) override {
        button_start.checkClick(mousePos);
        button_rule.checkClick(mousePos);
    }

    void checkButtonHover(sf::Vector2f mousePos) override {
        button_start.checkHover(mousePos);
        button_rule.checkHover(mousePos);
    }

private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    Button button_start;
    Button button_rule;
};

// 메인
int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 650), "test");

    StartScreen start_screen;
    MainScreen main_screen;
    EndingScreen happy_screen(screen_number::Happy_Ending);
    EndingScreen bad_screen(screen_number::Bad_Ending);
    EndingScreen rule_screen(screen_number::Rule);

    sf::Clock clock; // 타이머용 시계
    bool timerStarted = false; // 타이머 시작 여부

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                switch (screen_num) {
                case screen_number::Start:
                    start_screen.checkButtonClick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                    break;
                case screen_number::Main:
                    main_screen.checkButtonClick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                   
                    break;
                case screen_number::Rule:
                    rule_screen.checkButtonClick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                    break;
                case screen_number::Happy_Ending:
                    happy_screen.checkButtonClick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                    break;
                case screen_number::Bad_Ending:
                    bad_screen.checkButtonClick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                    break;
                }
            }
        }

        sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

        // 버튼 호버 체크를 항상 수행
        switch (screen_num) {
        case screen_number::Start:
            start_screen.checkButtonHover(mousePos); // 버튼 호버 체크
            break;
        case screen_number::Main:
            main_screen.checkButtonHover(mousePos);
            main_screen.updateFollowSpritePosition(mousePos);
            break;
        case screen_number::Happy_Ending:
            happy_screen.checkButtonHover(mousePos);
            break;
        case screen_number::Bad_Ending:
            bad_screen.checkButtonHover(mousePos);
            break;
        case screen_number::Rule:
            rule_screen.checkButtonHover(mousePos);
            break;
        }

        window.clear();

        switch (screen_num) {
        case screen_number::Start:
            start_screen.draw(window);
            break;
        case screen_number::Main:
            main_screen.draw(window);

            if (!timerStarted) {
                timerStarted = true;
                clock.restart(); // 타이머 초기화
            }
              
            // 경과 시간 출력
            //cout << "Elapsed Time: " << clock.getElapsedTime().asSeconds() << " seconds" << endl;

            // 10초가 지나면 EndingScreen으로 이동
            if (timerStarted && clock.getElapsedTime().asSeconds() >= 10.0f) {
                timerStarted = false;
                if (score >= 10000)
                    screen_num = screen_number::Happy_Ending;
                else
                    screen_num = screen_number::Bad_Ending;
            }
            break;
        case screen_number::Happy_Ending:
            happy_screen.draw(window); break;
        case screen_number::Bad_Ending:
            bad_screen.draw(window); break;
        case screen_number::Rule:
            rule_screen.draw(window); break;
        }
        window.display();
    }

}

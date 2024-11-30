#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include <string>
#include <random>

using namespace std;

// 화면 상태를 나타내는 열거형
enum screen_number {
    Loding,
    Start,
    Main,
    Happy_Ending,
    Bad_Ending,
    Rule,
};

int screen_num = screen_number::Loding;
int score = -1;

class Button {
public:
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
        // score을 -1로 만들어 main에서 reset 진행되게 함
        score = -1;
        screen_num = screen_number::Main; // 화면 변경
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
            scoreText.setString(to_string(score));
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
    MainScreen()
        // 각 오브제 클릭 영역 설정
        : button_area{
            Button(objString[0], "", 84, 169, [this]() {
                event = 0;
                isFollowingMouse = true; // 마우스를 따라다니는 것을 활성화
            }), 
            Button(objString[0], "", 85, 260, [this]() {
                event = 1;
                isFollowingMouse = true;
            }),
            Button(objString[0], "", 85, 358, [this]() {
                event = 2;
                isFollowingMouse = true;
            }),
            Button(objString[1], "", 202, 160, [this]() {
                event = 3;
                isFollowingMouse = true;
            }),
            Button(objString[1], "", 202, 255, [this]() {
                event = 4;
                isFollowingMouse = true;
            }),
            Button(objString[1], "", 202, 358, [this]() {
                event = 5;
                isFollowingMouse = true;
            }),
            Button(objString[2], "", 65, 497, [this]() {
                event = 6;
                isFollowingMouse = true;
            }),
            Button(objString[2], "", 191, 497, [this]() {
                event = 7;
                isFollowingMouse = true;
            }),
            Button(objString[3], "", 387, 90, [this]() {
                event = 8;
                isFollowingMouse = true;
            }),
            Button(objString[4], "", 387, 269, [this]() {
                event = 9;
                isFollowingMouse = true;
            }),
            Button(objString[5], "", 392, 365, [this]() {
                event = 10;
                isFollowingMouse = true;
            }),
            Button(objString[5], "", 484, 365, [this]() {
                event = 11;
                isFollowingMouse = true;
            }),
            Button(objString[5], "", 576, 365, [this]() {
                event = 12;
                isFollowingMouse = true;
            }),
            Button(objString[6], "", 1014, 502, [this]() {
                if (event == -1)
                    resetCook();
                else {
                     isFollowingMouse = false;
                    event = -1;
                }
            }),

            // 작업대
            Button(objString[7], "", 703, 269, [this]() {
                isFollowingMouse = false;

                // 1. 조합순서 확인
                if (event < cook_arr[cook_num][0] || event > cook_arr[cook_num][2]) {
                    // 1-2. 틀렸을경우 event = -1 하고 배열에는 추가하지 않음 화난표정
                    emotion = 2;
                }
                else {
                    // 2. event 확인 후 cook에 저장하기
                    emotion = 0;
                    cook[cook_num] = event;
                    cook_num++;
                }
               
                // 3. 파르페가 완성되었을때 예시와 같은지 확인
                if (cook_num == 9){
                    int chk = 1;
                    for (int i = 0; i < 9; i++)
                        if (cook[i] != recipe[i]) {
                            chk = 0; break;
                        }
                    
                    // 4. 맞는지 틀린지에 따라 표정 변화 후 파르페 배열 초기화, 맞을경우 score += 1000 추가
                    if (chk) {
                        emotion = 1;
                        score += 1000;
                        resetCook();
                        randomRecipe();
                    }
                    else {
                        emotion = 2;
                        resetCook();
                    }
                    cook_num = 0;
                }

                event = -1;
            })
        }
    {

        // 오브제 이미지

        
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

            cookTexture[i].loadFromFile("img/main/obj_cook/cook_" + to_string(i) + ".png");
            cookSprite[i].setTexture(cookTexture[i]);
        }
    }

            void draw(sf::RenderWindow& window) override {
                window.draw(backgroundSprite);
                window.draw(charSprite[emotion]);
                window.draw(messageSprite);
                window.draw(tableSprite);
                window.draw(objSprite);

                // 각 버튼을 그리기
                for (int i = 0; i < 15; i++) {
                    button_area[i].draw(window);
                }

                for (int i = 0; i < 9; i++) {
                    cookSprite[recipe[cook_order[i]]].setPosition(recipeXY[0][cook_order[i]], recipeXY[1][cook_order[i]]);
                    window.draw(cookSprite[recipe[cook_order[i]]]);

                    if (cook[cook_order[i]] > -1) {
                        cookSprite[cook[cook_order[i]]].setPosition(cookXY[0][cook_order[i]], cookXY[1][cook_order[i]]);
                        window.draw(cookSprite[cook[cook_order[i]]]);
                    }
                    
                }

                // 마우스를 따라다니는 이미지가 활성화 됐을 경우 그리기
                if (isFollowingMouse) {
                    window.draw(followSprite[event]);
                }
            }

            void checkButtonClick(sf::Vector2f mousePos) override {
                for (int i = 0; i < 15; i++) {
                    button_area[i].checkClick(mousePos);
                }
            }

            void checkButtonHover(sf::Vector2f mousePos) override {
                for (int i = 0; i < 15; i++) {
                    button_area[i].checkHover(mousePos);
                }
            }

            void updateFollowSpritePosition(sf::Vector2f mousePos) {
                if (isFollowingMouse) {
                    followSprite[event].setPosition(mousePos.x - followSprite[event].getGlobalBounds().width / 2,
                        mousePos.y - followSprite[event].getGlobalBounds().height / 2);
                }
            }

            void reset() {
                score = 0;
                emotion = 0;
                event = -1;
                cook_num = 0;
                isFollowingMouse = false;
                randomRecipe();
                resetCook();
            }

            void randomRecipe() {
                recipe[0] = 8;

                // 난수 생성기 및 시드 설정
                std::random_device rd; // 진정한 난수 생성기
                std::default_random_engine generator(rd()); // 시드를 rd()로 설정
                std::uniform_int_distribution<int> distribution(0, 13); // 0 ~ 13

                for (int i = 1; i < 9; i++) {
                    int ran = distribution(generator);

                    // 레시피 순서에 맞지 않는 재료일 경우 다시 생성, 맞으면 저장
                    if (ran >= cook_arr[i][0] && ran <= cook_arr[i][2]) {
                        recipe[i] = ran;
                        cout << ran << endl;
                    }
                    else {
                        i--; continue;
                    }
                }
            }

            void resetCook() {
                for (int i = 0; i < 9; i++)
                    cook[i] = -1;
            }

private:
    int emotion = 0; // 캐릭터의 표정
    int event = -1; // 오브제
    int cook_num = 0; // 조리 진행도
    bool isFollowingMouse = false; // 마우스를 따라다닐지 여부


    // 각 오브제 클릭 영역
    string objString[8] = {
        "img/main/obj_box/ice_box.png",
        "img/main/obj_box/cream_box.png",
        "img/main/obj_box/fruit_box.png",
        "img/main/obj_box/cup_box.png",
        "img/main/obj_box/bread_box.png",
        "img/main/obj_box/topping_box.png",
        "img/main/obj_box/trash_box.png",
        "img/main/obj_box/bench_box.png",
    };

    // 배경
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    // 각 음식 등 오브젝트
    sf::Texture objTexture;
    sf::Sprite objSprite;

    // 테이블(작업대 & 스코어판)
    sf::Texture tableTexture;
    sf::Sprite tableSprite;

    // 말풍선
    sf::Texture messageTexture;
    sf::Sprite messageSprite;

    // 캐릭터
    sf::Texture charTexture[3];
    sf::Sprite charSprite[3];

    // 음식 작업대 이미지
    sf::Texture cookTexture[13];
    sf::Sprite cookSprite[13];

    // 음식 작업대 이미지 위치 배열
    int cookXY[2][11] = { {790, 804, 804, 804, 804, 804, 804, 804, 798, 788},{328, 399, 378, 378, 355, 333, 333, 298, 288} };

    // 음식 레시피 이미지 위치 배열
    int recipeXY[2][11] = { {761, 777, 777, 777, 777, 777, 777, 771, 761} ,{86, 157, 136, 136, 113, 91, 91, 56, 46} };

    // 레시피 배열
    int recipe[9];

    // 작업대 배열
    int cook[9]; 

    // 작업대 들어갈 수 있는 음식의 종류(순서) 배열
    int cook_arr[9][3] = {{8,8,8}, {9,9,9}, {6,7,7}, {3, 4, 5}, {9,9,9}, {6,7,7}, {3, 4, 5}, {0, 1, 2}, {10, 11, 12} };

    // 이미지 생성 순서
    int cook_order[9] = { 1, 4, 3, 6, 2, 5, 0, 7, 8 };

    // 마우스를 따라다닐 이미지
    sf::Texture followTexture[13]; 
    sf::Sprite followSprite[13];
    
    // 버튼 배열 선언
    Button button_area[15];
};


class LoadingScreen{
public:
    LoadingScreen() {
        // 로딩 중 텍스트 설정
        if (!font.loadFromFile("font/gulim.ttc")) {
            cerr << "Error loading font" << endl;
        }
        loadingText.setFont(font);
        loadingText.setString("Loading...");
        loadingText.setCharacterSize(30);
        loadingText.setFillColor(sf::Color::Black);
        loadingText.setPosition(500, 300); // 텍스트 위치 설정

    }

    void draw(sf::RenderWindow& window){
        window.clear(sf::Color::White); // 배경색 설정
        window.draw(loadingText); // 로딩 텍스트 그리기
    }

private:
    sf::Font font;
    sf::Text loadingText;
};

class StartScreen : public Screen {
public:
    StartScreen()
        :button_start("img/start/startButton_default.png", "img/start/startButton_start.png", 855, 270, 740, 210, []() {
            cout << "Start button clicked!" << endl;
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
    LoadingScreen loading_screen;
    MainScreen main_screen;
    EndingScreen happy_screen(screen_number::Happy_Ending);
    EndingScreen bad_screen(screen_number::Bad_Ending);
    EndingScreen rule_screen(screen_number::Rule);

    bool loadingComplete = false;

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
        case screen_number::Loding:
            loading_screen.draw(window);
            if (!timerStarted) {
                timerStarted = true;
                clock.restart(); // 타이머 초기화
            }
            if (timerStarted && clock.getElapsedTime().asSeconds() >= 3.0f) {
                timerStarted = false;
                screen_num = screen_number::Start;
            }   
            break;
        case screen_number::Start:
            start_screen.draw(window);
            break;
        case screen_number::Main:
            // 메인스크린 초기화
            if (score == -1) {
                main_screen.reset();
                cout << "main screen reset" << endl;
            }
               
            main_screen.draw(window);

            if (!timerStarted) {
                timerStarted = true;
                clock.restart(); // 타이머 초기화
            }

            // 경과 시간 출력
            //cout << "Elapsed Time: " << clock.getElapsedTime().asSeconds() << " seconds" << endl;

            // 60초가 지나면 EndingScreen으로 이동
            if (timerStarted && clock.getElapsedTime().asSeconds() >= 20.0f) {
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

//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <string>
//#include <sstream>
//
//const int wys = 800;
//const int szer = 800;
//const int margines = 50;
//class GameInterface {
//private:
//    sf::Font font;
//    sf::RectangleShape okno;
//    sf::RectangleShape bottomBar;
//
//    sf::Text scoreText;
//    sf::Text timeText;
//    sf::Text livesText;
//    sf::Text playerNameText;
//
//    int score;
//    int lives;
//    std::string playerName;
//
//    sf::Clock gameClock;
//public:
//    GameInterface(const std::string& playerName)
//        : score(0), lives(3), playerName(playerName) {
//        if (!font.loadFromFile("Arial.ttf")) {
//            return;
//        }
//
//        okno.setSize(sf::Vector2f(float(szer), float(margines)));
//        okno.setFillColor(sf::Color(50, 50, 50));
//        okno.setPosition(0.f, 0.f);
//
//        bottomBar.setSize(sf::Vector2f(float(szer), float(margines)));
//        bottomBar.setFillColor(sf::Color(50, 50, 50));
//        bottomBar.setPosition(0.f, float(wys - margines));
//
//        configureText(scoreText, "Score: 0", 10.f, 5.f);
//        configureText(timeText, "Time: 0s", float(szer - 150), 5.f);
//        configureText(livesText, "Lives: 3", 10.f, float(wys - 45));
//        configureText(playerNameText, "Player: " + playerName, float(szer - 200), float(wys - 45));
//    }
//
//    void configureText(sf::Text& text, const std::string& content, float x, float y) {
//        text.setFont(font);
//        text.setString(content);
//        text.setCharacterSize(20);
//        text.setFillColor(sf::Color::White);
//        text.setPosition(x, y);
//    }
//
//    void update() {
//        int elapsedSeconds = static_cast<int>(gameClock.getElapsedTime().asSeconds());
//        timeText.setString("Time: " + std::to_string(elapsedSeconds) + "s");
//        scoreText.setString("Score: " + std::to_string(score));
//        livesText.setString("Lives: " + std::to_string(lives));
//    }
//    void addScore(int points) {
//        score += points;
//    }
//    void loseLife() {
//        if (lives > 0)
//            lives--;
//    }
//
//    void draw(sf::RenderWindow& window) {
//        window.draw(okno);
//        window.draw(bottomBar);
//        window.draw(scoreText);
//        window.draw(timeText);
//        window.draw(livesText);
//        window.draw(playerNameText);
//    }
//};
//
//int main() {
//    sf::RenderWindow window(sf::VideoMode(szer, wys), "Game Interface");
//
//    GameInterface gameInterface("Player1");
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//        gameInterface.update();
//
//        window.clear(sf::Color::Black);
//        gameInterface.draw(window);
//        window.display();
//    }
//    return 0;
//}

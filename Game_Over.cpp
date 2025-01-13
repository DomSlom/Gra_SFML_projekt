//#include <SFML/Graphics.hpp>
//#include <iostream>
//
//const int szer = 800;
//const int wys = 800;
//
//class GameOverScreen {
//private:
//    sf::Font font;
//    sf::Text gameOverText;
//
//public:
//    GameOverScreen() {
//        if (!font.loadFromFile("Arial.ttf")){
//            return;
//        }
//        gameOverText.setFont(font);
//        gameOverText.setString("Game Over");
//        gameOverText.setCharacterSize(70);
//        gameOverText.setFillColor(sf::Color::Red);
//        gameOverText.setRotation(45);
//        gameOverText.setStyle(sf::Text::Bold);
//        gameOverText.setPosition(wys/2 - 100, szer/2 - 100);
//    }
//
//    void draw(sf::RenderWindow& window) {
//        window.draw(gameOverText);
//    }
//};
//
//int main() {
//    sf::RenderWindow window(sf::VideoMode(wys, szer), "Game Over");
//
//    GameOverScreen gameOverScreen;
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        window.clear(sf::Color::Black);
//        gameOverScreen.draw(window);
//        window.display();
//    }
//
//    return 0;
//}

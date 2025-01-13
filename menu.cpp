//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <string>
//#include <stdexcept>
//#include <iostream>
//
//class Menu {
//public:
//    Menu(float width, float height) {
//        if (!font.loadFromFile("arial.ttf")) { 
//            throw std::runtime_error("Nie mo¿na wczytaæ czcionki!");
//        }
//
//        // Nazwy pozycji menu
//        std::vector<std::string> menuItems = { "Start", "Wczytaj gre", "Poprzednie wyniki", "Wyjscie" };
//
//        for (size_t i = 0; i < menuItems.size(); ++i) {
//            sf::Text text;
//            text.setFont(font);
//            text.setString(menuItems[i]);
//            text.setCharacterSize(40);
//            text.setFillColor(sf::Color(173, 216, 230)); 
//            if (i == selectedItemIndex) {
//                text.setStyle(sf::Text::Bold); 
//            }
//            else {
//                text.setStyle(sf::Text::Regular);
//            }
//
//            
//            sf::FloatRect textBounds = text.getGlobalBounds();
//            text.setPosition(sf::Vector2f(
//                width / 2 - textBounds.width / 2,
//                height / (menuItems.size() + 1) * (i + 1)
//            ));
//
//            menuText.push_back(text);
//        }
//    }
//
//    void draw(sf::RenderWindow& window) {
//        for (const auto& text : menuText) {
//            window.draw(text);
//        }
//    }
//
//    void moveUp() {
//        if (selectedItemIndex > 0) {
//            menuText[selectedItemIndex].setStyle(sf::Text::Regular); 
//            selectedItemIndex--;
//            menuText[selectedItemIndex].setStyle(sf::Text::Bold);   
//        }
//    }
//
//    void moveDown() {
//        if (selectedItemIndex < menuText.size() - 1) {
//            menuText[selectedItemIndex].setStyle(sf::Text::Regular);
//            selectedItemIndex++;
//            menuText[selectedItemIndex].setStyle(sf::Text::Bold);
//        }
//    }
//
//    int getSelectedItemIndex() const {
//        return selectedItemIndex;
//    }
//
//private:
//    std::vector<sf::Text> menuText;
//    sf::Font font;
//    int selectedItemIndex = 0; // Domyœlnie wybrana pierwsza pozycja
//};
//
//// Okno gry
//void runGame() {
//    sf::RenderWindow gameWindow(sf::VideoMode(800, 600), "Gra");
//
//    while (gameWindow.isOpen()) {
//        sf::Event event;
//        while (gameWindow.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                gameWindow.close();
//            }
//        }
//
//        //rysowanie elementów gry
//        gameWindow.clear(sf::Color::Black);
//        // elementy gry
//        gameWindow.display();
//    }
//}
//
//int main() {
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Menu");
//
//    sf::Texture tekstura_tlo;
//    if (!tekstura_tlo.loadFromFile("t³o_menu.jpg")) {
//        throw std::runtime_error("Nie mo¿na wczytaæ pliku t³a!");
//    }
//
//    sf::Sprite backgroundSprite(tekstura_tlo);
//    sf::Vector2u textureSize = tekstura_tlo.getSize();
//    sf::Vector2u windowSize = window.getSize();
//    backgroundSprite.setScale(
//        static_cast<float>(windowSize.x) / textureSize.x,
//        static_cast<float>(windowSize.y) / textureSize.y
//    );
//
//    Menu menu(window.getSize().x, window.getSize().y);
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//            if (event.type == sf::Event::KeyPressed) {
//                if (event.key.code == sf::Keyboard::Up) {
//                    menu.moveUp();
//                }
//                else if (event.key.code == sf::Keyboard::Down) {
//                    menu.moveDown();
//                }
//                else if (event.key.code == sf::Keyboard::Enter) {
//                    switch (menu.getSelectedItemIndex()) {
//                    case 0: // Start
//                        std::cout << "Rozpocznij grê" << std::endl;
//                        break;
//                    case 1: // Wczytaj grê
//                        window.close(); 
//                        runGame();  
//                        break;
//                    case 2: // Poprzednie wyniki
//                        std::cout << "Poprzednie wyniki" << std::endl;
//                        break;
//                    case 3: // Wyjœcie
//                        window.close();
//                        break;
//                    }
//                }
//            }
//        }
//
//        window.clear();
//        window.draw(backgroundSprite); 
//        menu.draw(window);
//        window.display();
//    }
//
//    return 0;
//}

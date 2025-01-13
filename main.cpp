///*temat gry:
//Opis gry:
//
//*/
//#include <SFML/Graphics.hpp>
//#include <iostream>
//
//const int wys = 900;
//const int szer = 900;
//const int margines = 100;
//const float speed = 300.f;
//const float skala = 0.25f;
//class statek {
//private:
//    sf::Vector2f position;
//    sf::Texture tekstura;
//    sf::Sprite pSprite;
//    sf::Vector2f size;
//public:
//    statek(float x_in, float y_in) {
//        position.x = x_in;
//        position.y = y_in;
//        tekstura.loadFromFile("RakietaSS.png");
//        pSprite.setTexture(tekstura);
//        pSprite.setPosition(position);
//        pSprite.setScale(sf::Vector2f(skala, skala));
//        size.x = pSprite.getGlobalBounds().width;
//        size.y = pSprite.getGlobalBounds().height;
//        pSprite.setOrigin(pSprite.getLocalBounds().width / 2, pSprite.getLocalBounds().height / 2);
//    }
//    void animuj1(float x_in, float y_in) {
//        sf::Vector2f pos;
//        pos.x = x_in;
//        pos.y = y_in;
//        if (position.x >= margines) {
//            pSprite.move(pos);
//        }
//        position = pSprite.getPosition();
//    }
//    void animuj2(float x_in, float y_in) {
//        sf::Vector2f pos;
//        pos.x = x_in;
//        pos.y = y_in;
//        if (position.x < szer - margines) {
//            pSprite.move(pos);
//        }
//        position = pSprite.getPosition();
//    }
//    void animuj3(float x_in, float y_in) {
//        sf::Vector2f pos;
//        pos.x = x_in;
//        pos.y = y_in;
//        if (position.y > margines) {
//            pSprite.move(pos);
//        }
//        position = pSprite.getPosition();
//    }
//    void animuj4(float x_in, float y_in) {
//        sf::Vector2f pos;
//        pos.x = x_in;
//        pos.y = y_in;
//        if (position.y < wys - margines) {
//            pSprite.move(pos);
//        }
//        position = pSprite.getPosition();
//        //std::cout << position.y << std::endl;
//    }
//    sf::Sprite getStatek() { return pSprite; }
//};
//
//int main() {
//
//    sf::RenderWindow window(sf::VideoMode(szer, wys), "Gra Video");
//    sf::Texture backgroundTexture;
//    if (!backgroundTexture.loadFromFile("t³o.jpg")) {
//        return -1;
//    }
//    sf::Sprite background(backgroundTexture);
//
//    statek st(szer / 2, wys / 2);
//
//    sf::Clock clock;
//
//    while (window.isOpen()) {
//        sf::Time deltaTime = clock.restart();
//
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//        }
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
//            st.animuj1(float(-speed * deltaTime.asSeconds()), 0);
//        }
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
//            st.animuj2(float(speed * deltaTime.asSeconds()), 0);
//        }
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
//            st.animuj3(0, float(-speed * deltaTime.asSeconds()));
//        }
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
//            st.animuj4(0, float(speed * deltaTime.asSeconds()));
//        }
//
//        window.clear(sf::Color::Black);
//        window.draw(background);
//        window.draw(st.getStatek());
//        window.display();
//    }
//
//    return 0;
//}

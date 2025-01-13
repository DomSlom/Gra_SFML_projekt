//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <cstdlib>
//#include <ctime>
//#include <cmath>
//#include <sstream>
//
//class Meteor {
//public:
//    Meteor(const sf::Texture& texture, const sf::Vector2u& windowSize, float speed)
//        : velocity(generateRandomVelocity(speed)), lastCollisionTime(0.f) {
//        sprite.setTexture(texture);
//
//        // Skalowanie meteoru
//        float scale = 0.08f; // Zmniejszenie o po³owê
//        sprite.setScale(scale, scale);
//
//        // Losowa pozycja meteoru na ekranie
//        float x = static_cast<float>(rand() % (windowSize.x - static_cast<int>(texture.getSize().x * scale)));
//        float y = static_cast<float>(rand() % (windowSize.y - static_cast<int>(texture.getSize().y * scale)));
//        sprite.setPosition(x, y);
//    }
//    bool canCollide(float currentTime) {
//        return currentTime - lastCollisionTime >= 1.0f; 
//    }
//    void registerCollision(float currentTime) {
//        lastCollisionTime = currentTime;
//    }
//
//
//    // Aktualizacja pozycji meteoru
//    void update(float deltaTime, const sf::Vector2u& windowSize) {
//        sf::Vector2f position = sprite.getPosition();
//        sf::FloatRect bounds = sprite.getGlobalBounds();
//        sf::Vector2f newPosition = position + velocity * deltaTime;
//
//        // Kolizje z krawêdziami okna
//        if (newPosition.x < 0 || newPosition.x + bounds.width > windowSize.x) {
//            velocity.x = -velocity.x;
//        }
//        if (newPosition.y < 0 || newPosition.y + bounds.height > windowSize.y) {
//            velocity.y = -velocity.y;
//        }
//
//        // Przesuniêcie sprite'a
//        sprite.move(velocity * deltaTime);
//    }
//
//    // Rysowanie meteoru
//    void draw(sf::RenderWindow& window) const {
//        window.draw(sprite);
//    }
//
//    // Pobranie granic sprite'a
//    sf::FloatRect getBounds() const {
//        return sprite.getGlobalBounds();
//    }
//
//private:
//    sf::Sprite sprite;
//    sf::Vector2f velocity;
//    float lastCollisionTime;
//
//    // Funkcja generuj¹ca losowy wektor prêdkoœci
//    sf::Vector2f generateRandomVelocity(float speed) {
//        float angle = static_cast<float>(rand()) / RAND_MAX * 360.f; // Losowy k¹t w stopniach
//        float radians = angle * 3.14159f / 180.f; // Zamiana na radiany
//        return sf::Vector2f(speed * cos(radians), speed * sin(radians));
//    }
//};
//
//int main() {
//    sf::Clock globalclock;
//    srand(static_cast<unsigned>(time(nullptr))); // Inicjalizacja generatora liczb pseudolosowych
//
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Meteor Game");
//    window.setFramerateLimit(60);
//
//    // Wczytanie tekstury meteoru
//    sf::Texture meteorTexture;
//    if (!meteorTexture.loadFromFile("MeteorySS.png")) {
//        throw std::runtime_error("Nie mo¿na wczytaæ pliku meteor.png! Upewnij siê, ¿e znajduje siê w katalogu programu.");
//    }
//
//    // Wczytanie tekstury gracza
//    sf::Texture playerTexture;
//    if (!playerTexture.loadFromFile("RakietaSS.png")) {
//        throw std::runtime_error("Nie mo¿na wczytaæ pliku player.png! Upewnij siê, ¿e znajduje siê w katalogu programu.");
//    }
//
//    // Tworzenie meteorów
//    std::vector<Meteor> meteors;
//    const int meteorCount = 5;
//    const float meteorSpeed = 100.f;
//
//    for (int i = 0; i < meteorCount; ++i) {
//        meteors.emplace_back(meteorTexture, window.getSize(), meteorSpeed);
//    }
//
//    // Tworzenie gracza
//    sf::Sprite player(playerTexture);
//    player.setScale(0.3f, 0.3f);
//    player.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
//
//    const float playerSpeed = 200.f;
//
//    // Licznik punktów
//    int points = 0;
//    sf::Font font;
//    if (!font.loadFromFile("arial.ttf")) {
//        throw std::runtime_error("Nie mo¿na wczytaæ pliku czcionki!");
//    }
//    sf::Text scoreText;
//    scoreText.setFont(font);
//    scoreText.setCharacterSize(24);
//    scoreText.setFillColor(sf::Color::White);
//    scoreText.setPosition(10.f, 10.f);
//
//    sf::Clock clock; // Do obliczania deltaTime
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//        }
//
//        // Obliczanie czasu deltaTime
//        float deltaTime = clock.restart().asSeconds();
//
//        // Sterowanie graczem
//        sf::Vector2f movement(0.f, 0.f);
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= playerSpeed;
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += playerSpeed;
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= playerSpeed;
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += playerSpeed;
//
//        player.move(movement * deltaTime);
//
//        // Kolizja z krawêdziami okna dla gracza
//        sf::FloatRect playerBounds = player.getGlobalBounds();
//        if (playerBounds.left < 0) player.setPosition(0, player.getPosition().y);
//        if (playerBounds.left + playerBounds.width > window.getSize().x)
//            player.setPosition(window.getSize().x - playerBounds.width, player.getPosition().y);
//        if (playerBounds.top < 0) player.setPosition(player.getPosition().x, 0);
//        if (playerBounds.top + playerBounds.height > window.getSize().y)
//            player.setPosition(player.getPosition().x, window.getSize().y - playerBounds.height);
//
//        // Aktualizacja meteorów i sprawdzanie kolizji
//        for (auto& meteor : meteors) {
//            meteor.update(deltaTime, window.getSize());
//            if (playerBounds.intersects(meteor.getBounds())) {
//                float currentTime = globalclock.getElapsedTime().asSeconds(); 
//                if (meteor.canCollide(currentTime)) {
//                    points++; 
//                    meteor.registerCollision(currentTime); 
//                }
//            }
//        }
//
//        // Aktualizacja tekstu punktów
//        std::ostringstream scoreStream;
//        scoreStream << "Punkty: " << points;
//        scoreText.setString(scoreStream.str());
//
//        // Rysowanie okna
//        window.clear(sf::Color::Black);
//        for (const auto& meteor : meteors) {
//            meteor.draw(window);
//        }
//        window.draw(player);
//        window.draw(scoreText);
//        window.display();
//    }
//
//    return 0;
//}

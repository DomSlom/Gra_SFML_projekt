#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Utwórz okno
    sf::RenderWindow window(sf::VideoMode(800, 600), "Dodawanie punktów co sekundê");

    // Zegar do œledzenia czasu
    sf::Clock clock;

    // Punkty gracza
    int punkty = 0;

    // Czcionka i tekst, aby wyœwietliæ punkty
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Nie uda³o siê za³adowaæ czcionki!" << std::endl;
        return -1;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(10.f, 10.f);

    while (window.isOpen()) {
        // Obs³uga zdarzeñ
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // SprawdŸ, czy minê³a 1 sekunda
        if (clock.getElapsedTime().asSeconds() >= 1.f) {
            punkty++;                     // Dodaj punkt
            clock.restart();              // Zresetuj zegar
        }

        // Aktualizacja tekstu
        text.setString("Punkty: " + std::to_string(punkty));

        // Renderowanie
        window.clear();
        window.draw(text);
        window.display();
    }

    return 0;
}
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <ctime>

//deklaracja sta³ych i zmiennych globalnych
const int wys = 800;
const int szer = 900;
const int margines = 80;
const float speed = 300.f;
const float skala = 0.25f;
const float skala_meteor = 0.1f;
const int liczba_meteorytow = 8;
const float meteor_speed = 100.f;
const float interface_szer = 50;
const float margines_odbicie = 50;
const float czas_kolizji = 1.f;
const float czas_oczekiwania = 3.f;
const int max_liczba_zyc = 10;
const int dodane_punkty = 1; //liczba dodawanych punktów
std::string playerName;
std::string gameLevel;

//flagi
bool isPaused = false; //zmienna odpowiadaj¹ca za zatrzymanie gry w trakcie odpalenia nowego okna
bool newGame = true;  //flaga nowej gry

class Menu {
public:
    Menu(float width, float height) {
        if (!font.loadFromFile("arial.ttf")) { 
        
        }
        // Nazwy pozycji menu
        std::vector<std::string> menuItems = { "Nowa gra", "Wczytaj gre", "Poprzednie wyniki", "Wyjscie" };

        for (size_t i = 0; i < menuItems.size(); ++i) {
            sf::Text text;
            text.setFont(font);
            text.setString(menuItems[i]);
            text.setCharacterSize(40);
            text.setFillColor(sf::Color(135, 206, 250)); // Jasny b³êkit
            text.setPosition(sf::Vector2f(width / 2 - text.getGlobalBounds().width / 2, height / (menuItems.size() + 1) * (i + 1)));
            menuText.push_back(text);
        }
        updateSelection();
    }
    void draw(sf::RenderWindow& window) {
        for (const auto& text : menuText) {
            window.draw(text);
        }
    }
    void moveUp() {
        selectedItemIndex = (selectedItemIndex == 0) ? menuText.size() - 1 : selectedItemIndex - 1;
        updateSelection();
    }
    void moveDown() {
        selectedItemIndex = (selectedItemIndex == menuText.size() - 1) ? 0 : selectedItemIndex + 1;
        updateSelection();
    }
    int getSelectedItemIndex() const {
        return selectedItemIndex;
    }
private:
    std::vector<sf::Text> menuText;
    sf::Font font;
    int selectedItemIndex = 0; // Domyœlnie wybrana pierwsza pozycja

    // Aktualizacja wyró¿nienia wybranej opcji
    void updateSelection() {
        for (size_t i = 0; i < menuText.size(); ++i) {
            if (i == selectedItemIndex) {
                menuText[i].setStyle(sf::Text::Bold);       // Pogrubienie wybranej opcji
                menuText[i].setFillColor(sf::Color::White); // Wybrana opcja w bia³ym kolorze
            }
            else {
                menuText[i].setStyle(sf::Text::Regular);       // Zwyk³y styl tekstu dla reszty
                menuText[i].setFillColor(sf::Color(135, 206, 250)); // Jasny b³êkit dla niewybranych opcji
            }
        }
    }
};

//okno help
void window_Help(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        return;
    }
    sf::Text helpText;
    helpText.setFont(font);
    helpText.setString("Sterowanie:\nW - ruch w gore\nS - ruch w dol\nA - ruch w lewo\nD - ruch w prawo\nESC - wyjscie\nF1 - pomoc\nPowodzenia!");
    helpText.setCharacterSize(24);
    helpText.setFillColor(sf::Color::White);
    helpText.setPosition(50.f, 50.f);

    sf::RenderWindow helpWindow(sf::VideoMode(600, 400), "Pomoc");
    while (helpWindow.isOpen()) {
        sf::Event event;
        while (helpWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                helpWindow.close();
                isPaused = false;
                std::cout << "start" << std::endl;
            }
        }

        helpWindow.clear(sf::Color::Black);
        helpWindow.draw(helpText);
        helpWindow.display();
    }
}

// okno z potwierdzeniem wyjœcia
bool okno_Exit(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Nie uda³o siê wczytaæ czcionki!" << std::endl;
        return false;
    }
    sf::Text message("Czy na pewno chcesz opuscic gre? (T/N)", font, 24);
    message.setFillColor(sf::Color::White);
    message.setPosition(window.getSize().x / 2 - message.getGlobalBounds().width / 2, window.getSize().y / 2 - message.getGlobalBounds().height / 2);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::T) {
                    isPaused = false;
                    return true; // U¿ytkownik potwierdza wyjœcie
                }
                if (event.key.code == sf::Keyboard::N) {
                    return false; // U¿ytkownik anuluje wyjœcie
                }
            }
        }
        window.clear();
        window.draw(message);
        window.display();
    }
    return false;
}

//okno nowej gry z mo¿liwoœci¹ wprowadzenia nazwy gracza i levelu
void nowa_gra(sf::RenderWindow& window) {
    isPaused = true;
    sf::RenderWindow newGameWindow(sf::VideoMode(600, 400), "Nowa Gra");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Nie uda³o siê za³adowaæ czcionki!" << std::endl;
        return;
    }
    sf::Text Name("Podaj nazwe gracza:", font, 24);
    Name.setFillColor(sf::Color::White);
    Name.setPosition(50, 50);

    sf::Text Level("Podaj poziom trudnosci (1-3):", font, 24);
    Level.setFillColor(sf::Color::White);
    Level.setPosition(50, 150);

    sf::Text inputName("", font, 24);
    inputName.setFillColor(sf::Color::Yellow);
    inputName.setPosition(300, 50);

    sf::Text inputLevel("", font, 24);
    inputLevel.setFillColor(sf::Color::Yellow);
    inputLevel.setPosition(400, 150);

    std::string name = "";
    std::string levelStr = "";
    bool enteringName = true;
    while (newGameWindow.isOpen()) {
        sf::Event event;
        while (newGameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                newGameWindow.close();
            }
            if (event.type == sf::Event::TextEntered) {
                if (enteringName) {
                    if (event.text.unicode == '\b' && !name.empty()) { // Backspace
                        name.pop_back();
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != '\b') {
                        name += static_cast<char>(event.text.unicode);
                    }
                }
                else {
                    if (event.text.unicode == '\b' && !levelStr.empty()) { // Backspace
                        levelStr.pop_back();
                    }
                    else if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                        levelStr += static_cast<char>(event.text.unicode);
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    if (enteringName) {
                        enteringName = false;
                    }
                    else {
                        int level = std::stoi(levelStr);
                        if (level >= 1 && level <= 3) {
                            playerName = name;
                            gameLevel = level;
                            newGameWindow.close();
                        }
                    }
                }
            }
        }

        inputName.setString(name);
        inputLevel.setString(levelStr);

        newGameWindow.clear();
        newGameWindow.draw(Name);
        newGameWindow.draw(Level);
        newGameWindow.draw(inputName);
        newGameWindow.draw(inputLevel);
        newGameWindow.display();
    }
    playerName = name;
    gameLevel = levelStr;
    std::cout << "Nazwa gracza " << playerName << std::endl;
    std::cout << "level gry " << levelStr << std::endl;
    isPaused = false;
}

// Okno gry
bool runGame() {
    //interface gry
    class GameInterface {
    private:
        sf::Font font;
        sf::RectangleShape top;
        sf::RectangleShape bottom;
        sf::Text levelText;
        sf::Text scoreText;
        sf::Text timeText;
        sf::Text livesText;
        sf::Text playerNameText;
        sf::Text authorText;
        int score;
        int lives;
        std::string level;
        std::string playerName;
        sf::Clock gameClock;
        float savedTime;
    public:
        GameInterface(float savedTime=0.f)
            : score(0), lives(max_liczba_zyc), level("1"), playerName("Gracz1"), savedTime(0.f) {
            if (!font.loadFromFile("Arial.ttf")) {
                return;
            }

            top.setSize(sf::Vector2f(float(szer), float(interface_szer)));
            top.setFillColor(sf::Color(50, 50, 50));
            top.setPosition(0.f, 0.f);

            bottom.setSize(sf::Vector2f(float(szer), float(interface_szer)));
            bottom.setFillColor(sf::Color(50, 50, 50));
            bottom.setPosition(0.f, float(wys - interface_szer));

            configureText(scoreText, 40, "Score: 0", 100.f, 5.f);
            configureText(levelText, 40, "Level: "+ level, 10.f, 5.f);
            configureText(timeText, 40,"Time: 0s", float(szer - 130), 5.f);
            configureText(livesText, 40,"Lives: 5", 250.f, 5.f);
            configureText(playerNameText, 40, "Player: " + playerName, 10.f, float(wys - 45));
            configureText(authorText, 40,"Game by Dominik Slominski", float(szer - 300), float(wys - 45));
            gameClock.restart();
        }

        void configureText(sf::Text& text, int size, const std::string& content, float x, float y) {
            text.setFont(font);
            text.setCharacterSize(size);
            text.setString(content);
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::White);
            text.setPosition(x, y);
        }

        void update() {
            levelText.setString("Level: " + level);
            playerNameText.setString("Player: " + playerName);
            int elapsedSeconds = static_cast<int>(savedTime+gameClock.getElapsedTime().asSeconds());
            timeText.setString("Time: " + std::to_string(elapsedSeconds) + "s");
            scoreText.setString("Score: " + std::to_string(score));
            livesText.setString("Lives: " + std::to_string(lives));
        }
        void addScore(int points, sf::Clock clock) {
            if (clock.getElapsedTime().asSeconds() >= 1.f){
                score += points;
                clock.restart();
            }
        }
        void setSavedTime(float time) {
            savedTime = time;
        }
        void set_lives(int liv) {
            lives = liv;
        }
        void loseLife() {
            if (lives > 0)
                lives--;
        }
        int get_score(){
            return score;
        }
        int get_lives() {
            return lives;
        }
        std::string get_level() {
            return level;
        }
        void set_name(std::string name) {
            playerName = name;
        }
        void set_level(std::string lev) {
            level = lev;
        }
        void draw(sf::RenderWindow& window) {
            window.draw(top);
            window.draw(bottom);
            window.draw(scoreText);
            window.draw(timeText);
            window.draw(livesText);
            window.draw(levelText);
            window.draw(playerNameText);
            window.draw(authorText);
        }
        // Funkcja zapisuj¹ca stan gry do pliku
        void save_to_file() const {
            std::ofstream file("interface.dat", std::ios::binary);
            if (file.is_open()) {
                int elapsedSeconds = static_cast<int>(savedTime + gameClock.getElapsedTime().asSeconds());
                file.write(reinterpret_cast<const char*>(&score), sizeof(score));
                file.write(reinterpret_cast<const char*>(&level), sizeof(level));
                file.write(reinterpret_cast<const char*>(&lives), sizeof(lives));
                file.write(reinterpret_cast<const char*>(&elapsedSeconds), sizeof(elapsedSeconds));
                size_t nameLength = playerName.size();
                file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
                file.write(playerName.c_str(), nameLength);
                file.close();
                std::cout << "zapisano dane interface'u do gry" << std::endl;
            }
            else {
                std::cerr << "Nie mo¿na zapisaæ stanu gry do pliku!" << std::endl;
            }
        }
        void save_game_scores() {

            //zapis nazwy gracza, poziomu i liczby punktów
            std::ofstream file("Wyniki.dat", std::ios::binary);
            if (!file) {
                std::cerr << "Nie mo¿na otworzyæ pliku do zapisu!" << std::endl;
                return;
            }
            // Zapisujemy dane
            
            // Zapis d³ugoœci nazwy gracza i samej nazwy
            size_t nameLength = playerName.size();
            file.write(reinterpret_cast<const char*>(&nameLength), sizeof(size_t));
            file.write(playerName.c_str(), nameLength);

            // Zapis poziomu trudnoœci
            file.write(reinterpret_cast<const char*>(&gameLevel), sizeof(std::string));

            // Zapis liczby punktów
            file.write(reinterpret_cast<const char*>(&score), sizeof(int));
            file.close();

            std::cout << "Nazwa gracza " << playerName << "poziom "<<gameLevel<<" liczba punktow " << score << std::endl;
            std::cout << "zapisano wynik gry do pliku" << std::endl;
        }
        void load_from_file(){        // Funkcja wczytuj¹ca stan gry z pliku
            std::ifstream file("interface.dat", std::ios::binary);
            if (file.is_open()) {
                int score_in, lives_in, elapsedSeconds_in;
                std::string level_in;
                size_t nameLength;
                file.read(reinterpret_cast<char*>(&score_in), sizeof(score_in));
                file.read(reinterpret_cast<char*>(&level_in), sizeof(level_in));
                file.read(reinterpret_cast<char*>(&lives_in), sizeof(lives_in));
                file.read(reinterpret_cast<char*>(&elapsedSeconds_in), sizeof(elapsedSeconds_in));
                file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
                std::string playerName_in(nameLength, ' ');
                file.read(&playerName_in[0], nameLength);
                file.close();
                score = score_in;
                gameLevel = level_in;
                lives = lives_in;
                savedTime = static_cast<float>(elapsedSeconds_in);
                playerName = playerName_in;
            }
            else {
                std::cerr << "Nie mo¿na wczytaæ stanu gry z pliku!" << std::endl;
                score = 0;
                lives = 4;
                savedTime = 0.f;
                playerName = "Gracz_domyœlny";
            }
        }
    };

    class statek {
    private:
        sf::Vector2f position;
        sf::Texture tekstura;
        sf::Sprite pSprite;
        sf::Vector2f size;
    public:
        statek() {
            tekstura.loadFromFile("RakietaSS.png");
            pSprite.setTexture(tekstura);
            pSprite.setPosition(position);
            pSprite.setScale(sf::Vector2f(skala, skala));
            size.x = pSprite.getGlobalBounds().width;
            size.y = pSprite.getGlobalBounds().height;
            pSprite.setOrigin(pSprite.getLocalBounds().width / 2, pSprite.getLocalBounds().height / 2);
        }
        sf::FloatRect getGlobalBounds() const {
            return pSprite.getGlobalBounds();
        }
        void animuj1(float x_in, float y_in) {
            sf::Vector2f pos;
            pos.x = x_in;
            pos.y = y_in;
            if (position.x >= margines) {
                pSprite.move(pos);
            }
            position = pSprite.getPosition();
        }
        void animuj2(float x_in, float y_in) {
            sf::Vector2f pos;
            pos.x = x_in;
            pos.y = y_in;
            if (position.x < szer - margines) {
                pSprite.move(pos);
            }
            position = pSprite.getPosition();
        }
        void animuj3(float x_in, float y_in) {
            sf::Vector2f pos;
            pos.x = x_in;
            pos.y = y_in;
            if (position.y > margines + interface_szer) {
                pSprite.move(pos);
            }
            position = pSprite.getPosition();
        }
        void animuj4(float x_in, float y_in) {
            sf::Vector2f pos;
            pos.x = x_in;
            pos.y = y_in;
            if (position.y < wys - margines - interface_szer) {
                pSprite.move(pos);
            }
            position = pSprite.getPosition();
        }
        sf::Sprite getStatek() { return pSprite; }
        
        //ustawianie pozycji
        void setPosition(sf::Vector2f pos) {
            position = pos;
            pSprite.setPosition(pos);
        }
        void save_to_file() {
            std::ofstream file("statek.dat", std::ios::binary);
            if (!file) {
                std::cerr << "Nie mo¿na otworzyæ pliku do odczytu!" << std::endl;
                return;
            }
            file.write(reinterpret_cast<const char*>(&position.x), sizeof(float));
            file.write(reinterpret_cast<const char*>(&position.y), sizeof(float));
            file.close();
            std::cout << "zapisano pozycjê statku" << std::endl;
            std::cout << "zapisano pozycjê " << position.x << " " << position.y << std::endl;
        }
        void read_from_file(){
            sf::Vector2f readPos;
            std::ifstream file("statek.dat", std::ios::binary);
            if (!file) {
                std::cerr << "Nie mo¿na otworzyæ pliku do odczytu!" << std::endl;
                return;
            }
            file.read(reinterpret_cast<char*>(&readPos.x), sizeof(float));
            file.read(reinterpret_cast<char*>(&readPos.y), sizeof(float));
            file.close();
            std::cout << "odczytano pozycjê statku " << readPos.x << " " << readPos.y << std::endl;
            pSprite.setPosition(readPos);
            position = readPos;
        }
    };

    class Meteor {
    private:
        sf::Sprite sprite;
        sf::Vector2f velocity;
        float lastCollisionTime; //czas od ostatniej kolizji
    public:
        Meteor(const sf::Texture& texture,  const sf::Vector2f& position, const sf::Vector2f& velocity): lastCollisionTime(0.f) {
            sprite.setTexture(texture);
            sprite.setScale(skala_meteor, skala_meteor);
            sprite.setPosition(position);
            this->velocity = velocity; //ustawienie prêdkoœci wektora private
        }
        sf::Vector2f generateRandomVelocity() {
            float angle = static_cast<float>(rand()) / RAND_MAX * 360.f;
            float radians = angle * 3.14159f / 180.f;
            return sf::Vector2f(meteor_speed * cos(radians), meteor_speed * sin(radians));
        }
        bool canCollide(float currentTime) {
            return currentTime - lastCollisionTime >= 1.0f;
        }
        void registerCollision(float currentTime) {
            lastCollisionTime = currentTime;
        }
        void update(float deltaTime){
            sf::Vector2f position = sprite.getPosition();
            sf::FloatRect bounds = sprite.getGlobalBounds();
            sf::Vector2f newPosition = position + velocity * deltaTime;

            if (newPosition.x < 0 || newPosition.x + bounds.width > szer) {
                velocity.x = -velocity.x;
            }
            if (newPosition.y < margines_odbicie || newPosition.y + bounds.height > wys-margines_odbicie) {
                velocity.y = -velocity.y;
            }
            sprite.move(velocity * deltaTime);
        }
        void draw(sf::RenderWindow& window) const {
            window.draw(sprite);
        }
        sf::FloatRect getBounds() const {
            return sprite.getGlobalBounds();
        }
        //ustawianie pozycji
        sf::Vector2f getPosition() const{
            return sprite.getPosition();
        }
        //ustawianie prêdkoœci meteorytów
        sf::Vector2f getVelocity() const{
            return velocity;
        }
    };

    //napis game over
    class GameOverScreen {
    private:
        sf::Font font;
        sf::Text gameOverText;
    public:
        GameOverScreen() {
            if (!font.loadFromFile("Arial.ttf")) {
                return;
            }
            gameOverText.setFont(font);
            gameOverText.setString("Game Over");
            gameOverText.setCharacterSize(70);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setRotation(45);
            gameOverText.setStyle(sf::Text::Bold);
            gameOverText.setPosition(wys / 2 - 100, szer / 2 - 100);
        }
        void draw(sf::RenderWindow& window) {
            window.draw(gameOverText);
        }
    };
    //pocz¹tek programu dla okna gry
    class save {
    public:
        //funkcja zapisu gry
        void save_to_file(statek st, std::vector<Meteor> meteors, GameInterface face){
            st.save_to_file();
            face.save_to_file();
            std::cout << "zapisano dane interface" << std::endl;
        }
        void save_meteors_to_file(const std::vector<Meteor>& meteors) {
            std::ofstream file("meteor.dat", std::ios::binary);
            if (!file) {
                std::cerr << "B³¹d zapisu do pliku" << std::endl;
                return;
            }
            for (const auto& meteor : meteors) {
                sf::Vector2f position = meteor.getPosition();
                sf::Vector2f velocity = meteor.getVelocity();
                file.write(reinterpret_cast<const char*>(&position), sizeof(position));
                file.write(reinterpret_cast<const char*>(&velocity), sizeof(velocity));
            }
            file.close();
            std::cout << "zapisano dane meteorów" << std::endl;
        }

        //funkcja odczytu gry
        void read_from_file(statek st, std::vector<Meteor> meteors, GameInterface face) {
            st.read_from_file(); 
            std::cout << "odczytano dane" << std::endl;
        }

        std::vector<std::pair<sf::Vector2f, sf::Vector2f>> load_meteors_from_file() {
            std::ifstream file("meteor.dat", std::ios::binary);
            std::vector<std::pair<sf::Vector2f, sf::Vector2f>> dane_z_pliku;

            if (!file) {
                std::cout << "Plik nie istnieje" << std::endl;
                return dane_z_pliku;
            }

            while (file) {
                sf::Vector2f position, velocity;
                file.read(reinterpret_cast<char*>(&position), sizeof(position));
                file.read(reinterpret_cast<char*>(&velocity), sizeof(velocity));
                if (file) {
                    dane_z_pliku.emplace_back(position, velocity);
                }
            }
            file.close();
            std::cout<<"odczytano dane meteorow"<<std::endl;
            return dane_z_pliku;
        }

    };
    sf::Clock globalclock; //zegar dla kolizji meteorytów
    sf::Clock gameOverClock; //zegar odpiczaj¹cy czas po koñcu gry
    sf::Clock ClockScores; //zegar odliczaj¹cy czas przydzielania punktów

    srand(static_cast<unsigned>(time(nullptr)));
    sf::RenderWindow Gamewindow(sf::VideoMode(szer, wys), "Gra");
    save zapis; //deklaracja klasy zapis

    GameOverScreen gameOverScreen;
    Gamewindow.setFramerateLimit(60); //czêstotliwoœæ odœwie¿ania okna

    bool g_over = false; //flaga konca gry

    //pobieranie tekstur t³a oraz meteorytów
    sf::Texture tekstura_meteor;
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("t³o.jpg")) {
    
    }
    sf::Sprite background(backgroundTexture);
    if (!tekstura_meteor.loadFromFile("MeteorySS.png")) { 
  
    }
    statek st; //dodawanie statku
    GameInterface gameInterface;
    std::vector<Meteor> meteors;
    if (newGame){
        //³adowanie meteorytów i generowanie losowych pozycji i prêdkoœci
        for (int i = 0; i < liczba_meteorytow; ++i) {
            sf::Vector2f position(
                static_cast<float>(rand() % (szer - static_cast<int>(tekstura_meteor.getSize().x * skala_meteor))),
                static_cast<float>(rand() % (wys - 2 * int(margines_odbicie) - static_cast<int>(tekstura_meteor.getSize().y * skala_meteor)) + margines_odbicie)
            );

            float angle = static_cast<float>(rand()) / RAND_MAX * 360.f;
            float radians = angle * 3.14159f / 180.f;
            sf::Vector2f velocity(meteor_speed * cos(radians), meteor_speed * sin(radians));

            meteors.emplace_back(tekstura_meteor, position, velocity);
        }
        sf::Vector2f statek_pozycja(szer / 2, wys / 2);
        st.setPosition(statek_pozycja); //ustawienie pozycji pocz¹tkowej statku
        gameInterface.set_level(gameLevel);
        gameInterface.set_name(playerName);
        
    }
    else {
        std::cout << "Wczytano grê z pliku" << std::endl;
        //³adowanie meteorytów z pliku
        auto savedMeteors = zapis.load_meteors_from_file();
        for (const auto& data : savedMeteors) {
            meteors.emplace_back(tekstura_meteor, data.first /*pozycja odczytana*/, data.second/*odczytana prêdkoœæ*/);
        }
        sf::Vector2f statek_pozycja(szer / 2, wys / 2);
        st.setPosition(statek_pozycja); //ustawienie pozycji pocz¹tkowej statku
        st.read_from_file();
        gameInterface.load_from_file();
    }

    sf::Clock clock;

    while (Gamewindow.isOpen()) {
        sf::Time deltaTime = clock.restart();

        sf::Event event;
        while (Gamewindow.pollEvent(event)) {
            //wywo³anie okna potwierdzaj¹cego zamkniêcie przy zamkniêciu x
            if (event.type == sf::Event::Closed) {
                isPaused = true;
                std::cout << "pauza" << std::endl;
                if (okno_Exit(Gamewindow)) {
                    if (!g_over) {
                        zapis.save_to_file(st, meteors, gameInterface);
                        zapis.save_meteors_to_file(meteors);
                    }
                    else {
                        gameInterface.save_game_scores();
                    }
                    Gamewindow.close();
                }
                else{
                    isPaused = false;
                    std::cout << "start" << std::endl;
                }
            }
            //wywo³anie okna pomocy
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1) {
                isPaused = true;
                std::cout << "pauza" << std::endl;
                window_Help(Gamewindow);
            }
            //wywo³anie okna potwierdzaj¹cego zamkniêcie poprzez przycisk escape
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    isPaused = true;
                    std::cout << "pauza" << std::endl;
                    if (okno_Exit(Gamewindow)) {
                        isPaused = false;
                        if (!g_over) {
                            zapis.save_to_file(st, meteors, gameInterface);
                            zapis.save_meteors_to_file(meteors);
                        }
                        else {
                            gameInterface.save_game_scores();
                        }
                        Gamewindow.close();
                    }
                    else {
                        isPaused = false;
                        std::cout << "start" << std::endl;
                    }
                }
            }
        }
        if (!isPaused) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                st.animuj1(float(-speed * deltaTime.asSeconds()), 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                st.animuj2(float(speed * deltaTime.asSeconds()), 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                st.animuj3(0, float(-speed * deltaTime.asSeconds()));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                st.animuj4(0, float(speed * deltaTime.asSeconds()));
            }

            //dodawanie punktów gracza
                gameInterface.addScore(dodane_punkty, ClockScores);

            //aktualizacja pozycji meteorytów i sprawdzanie kolizji
            sf::FloatRect playerBounds = st.getGlobalBounds();
            for (auto& meteor : meteors) {
                meteor.update(deltaTime.asSeconds());
                if (playerBounds.intersects(meteor.getBounds())) {
                    float currentTime = globalclock.getElapsedTime().asSeconds();
                    if (meteor.canCollide(currentTime)) {
                        gameInterface.loseLife();
                        meteor.registerCollision(currentTime);
                    }
                }
            }
            //koniec gry
            if (gameInterface.get_lives() <= 0) {
                g_over = true;
                isPaused = true;
                gameInterface.save_game_scores();
                std::cout << "koniec gry" << std::endl;
            }
            gameInterface.update(); //aktulizacja punktów i czasu gry

            Gamewindow.clear(sf::Color::Black);
            Gamewindow.draw(background);
            Gamewindow.draw(st.getStatek());
            gameInterface.draw(Gamewindow);

            //obs³uga koñca gry
            if (g_over) {
                gameOverScreen.draw(Gamewindow);
                gameOverClock.restart();
            }
            //rysowanie meteorytów
            for (const auto& meteor : meteors) {
                meteor.draw(Gamewindow);
            }
            Gamewindow.display();
        }//koniec warunku pauzy

        //odliczanie czasu po zakoñczeniu gry do przejœcia do menu
        if (gameOverClock.getElapsedTime().asSeconds() > czas_oczekiwania && g_over) {
            isPaused = false;
            Gamewindow.close();
        }
    }
    return true;
}

void odczytajDaneGracza(std::string& playerName, std::string& gameLevel, int& score) {
    std::ifstream file("Wyniki.dat", std::ios::binary);
    if (!file) {
        std::cerr << "Nie mo¿na otworzyæ pliku do odczytu!" << std::endl;
        return;
    }

    // Odczyt d³ugoœci nazwy gracza i samej nazwy
    size_t nameLength;
    file.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
    char* nameBuffer = new char[nameLength + 1]; // Alokacja bufora
    file.read(nameBuffer, nameLength);
    nameBuffer[nameLength] = '\0'; // Dodanie terminatora
    playerName = std::string(nameBuffer);
    delete[] nameBuffer;

    // Odczyt poziomu trudnoœci
    file.read(reinterpret_cast<char*>(&gameLevel), sizeof(std::string));

    // Odczyt liczby punktów
    file.read(reinterpret_cast<char*>(&score), sizeof(int));

    file.close();
    std::cout << "Dane zosta³y odczytane!" << std::endl;
    std::cout << playerName << gameLevel << score << std::endl;
}

void Wyniki(const std::string& playerName, std::string gameLevel, int score) {
    sf::RenderWindow window(sf::VideoMode(400, 300), "Poprzednie wyniki gry");
    sf::Font font;

    if (!font.loadFromFile("arial.ttf")) { // Upewnij siê, ¿e plik czcionki jest dostêpny
        std::cerr << "Nie mo¿na za³adowaæ czcionki!" << std::endl;
        return;
    }

    // Tworzenie tekstów do wyœwietlenia
    sf::Text playerText("Gracz: " + playerName, font, 24);
    playerText.setFillColor(sf::Color::White);
    playerText.setPosition(50, 50);

    sf::Text levelText("Poziom trudnosci: " + gameLevel, font, 24);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(50, 100);

    sf::Text scoreText("Punkty: " + std::to_string(score), font, 24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(50, 150);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        window.draw(playerText);
        window.draw(levelText);
        window.draw(scoreText);
        window.display();
    }
}

bool poprzednie_wyniki(){
    std::string Nazwa_gracza;
    std::string level;
    int punkty_gry;
    odczytajDaneGracza(Nazwa_gracza, level, punkty_gry);
    Wyniki(Nazwa_gracza, level, punkty_gry);
    return true;
};

int main() {
    bool runningMenu = true;
    while (runningMenu) {
        sf::RenderWindow window(sf::VideoMode(szer, wys), "Menu");

        sf::Texture tekstura_tlo;
        if (!tekstura_tlo.loadFromFile("t³o_menu.jpg")) {
            throw std::runtime_error("Nie mo¿na wczytaæ pliku t³a!");
        }

        sf::Sprite backgroundSprite(tekstura_tlo);
        sf::Vector2u textureSize = tekstura_tlo.getSize();
        sf::Vector2u windowSize = window.getSize();
        backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / textureSize.x,
            static_cast<float>(windowSize.y) / textureSize.y);

        Menu menu(window.getSize().x, window.getSize().y);

        bool exitMenu = false;

        while (window.isOpen() && !exitMenu) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    runningMenu = false;
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up) {
                        menu.moveUp();
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        menu.moveDown();
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        switch (menu.getSelectedItemIndex()) {
                        case 0: // Nowa gra
                            window.close();
                            newGame = true;
                            //okno nowej gry do wprowadzenia nazwy gracza i levelu
                            nowa_gra(window);
                            if (runGame()) {
                                exitMenu = true;
                            }
                            break;
                        case 1: // Wczytaj grê
                            newGame = false;
                            std::cout << "Wczytaj grê" << std::endl;
                            if (runGame()) {
                                exitMenu = true;

                            }
                            break;
                        case 2: // Poprzednie wyniki
                            if (poprzednie_wyniki()) {
                                exitMenu = true;
                            }
                            break;
                        case 3: // Wyjœcie
                            window.close();
                            runningMenu = false;
                            break;
                        }
                    }
                }
            }

            window.clear();
            window.draw(backgroundSprite);
            menu.draw(window);
            window.display();
        }
    }
    return 0;
}

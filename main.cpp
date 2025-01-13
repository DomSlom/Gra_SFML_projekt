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
//wymiary okna
const int wys = 800, szer = 900;
int level_in;
std::string player_name;

//flagi
bool isPaused = false; //zmienna odpowiadaj¹ca za zatrzymanie gry w trakcie odpalenia nowego okna
bool newGame = true;  //flaga nowej gry

//menu gry
class Menu {
private:
    std::vector<sf::Text> menuText;
    sf::Texture tekstura;
    sf::Sprite tlo;
    sf::Font font;
    sf::Vector2u tekstura_size;
    int selectedItemIndex = 0; // Domyœlnie wybrana pierwsza pozycja
    void updateSelection() { // Aktualizacja wyró¿nienia wybranej opcji
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
public:
    Menu(float width, float height) {
        if (!font.loadFromFile("arial.ttf")) { 
        
        }
        tekstura.loadFromFile("t³o_menu.jpg");
        tekstura_size = tekstura.getSize();
        tlo.setScale(width / tekstura_size.x, height / tekstura_size.y);
        tlo.setTexture(tekstura);
        // Nazwy pozycji menu przy u¿yciu wektora typu string
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
    void draw(sf::RenderWindow& window) { //wyœwietlanie elementów menu
        window.draw(tlo);
        for (const auto& text : menuText) {
            window.draw(text);
        }
    }
    void moveUp() { //przesuniêcie wybranej opcji menu w górê
        selectedItemIndex = (selectedItemIndex == 0) ? menuText.size() - 1 : selectedItemIndex - 1;
        updateSelection();
    }
    void moveDown() { //przesuniêcie wybranej opcji menu w dó³
        selectedItemIndex = (selectedItemIndex == menuText.size() - 1) ? 0 : selectedItemIndex + 1;
        updateSelection();
    }
    int getSelectedItemIndex() const { //funkcja zwracaj¹ca wybran¹ opcje menu
        return selectedItemIndex;
    }
};
//okno pomocy
void window_Help(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) { //³adowanie czcionki
        return;
    }
    sf::Text helpText; 
    helpText.setFont(font); //ustawienie w³asnoœci tekstu
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
                isPaused = false; //uruchomienie gry po pauzie
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
        isPaused = true; //pauza gry w trakcie otwarcia okna
        sf::RenderWindow newGameWindow(sf::VideoMode(600, 400), "Nowa Gra");
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Nie uda³o siê za³adowaæ czcionki!" << std::endl;
            return;
        }
        //wprowadzanie nazwy gracza
        sf::Text Name("Podaj nazwe gracza:", font, 24);
        Name.setFillColor(sf::Color::White);
        Name.setPosition(50, 50);
        sf::Text inputName("", font, 24);
        inputName.setFillColor(sf::Color::Yellow);
        inputName.setPosition(300, 50);
        //wprowadzanie poziomu
        sf::Text Level("Podaj poziom trudnosci (1-3):", font, 24);
        Level.setFillColor(sf::Color::White);
        Level.setPosition(50, 150);
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
                    if (enteringName) { //pobieranie nazwy
                        if (event.text.unicode == '\b' && !name.empty()) { // Backspace
                            name.pop_back(); //usuwa ostatni znak
                        }
                        else if (event.text.unicode < 128 && event.text.unicode != '\b') {
                            name += static_cast<char>(event.text.unicode); //dodawanie do nazwy kolejnycch znaków
                        }
                    }
                    else { //pobieranie levelu
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
                            int level = std::stoi(levelStr); //konwersja string to int
                            if (level >= 1 && level <= 3) { //warunke poprawnoœci poziomu
                                player_name = name; //przekazanie danych do zmiennej globalnej
                                level_in = level;
                                newGameWindow.close();
                            }
                        }
                    }
                }
            }
            inputName.setString(name);
            inputLevel.setString(levelStr);
            //renderowanie okna nowej gry
            newGameWindow.clear();
            newGameWindow.draw(Name);
            newGameWindow.draw(Level);
            newGameWindow.draw(inputName);
            newGameWindow.draw(inputLevel);
            newGameWindow.display();
        }
        player_name = name;
        level_in = std::stoi(levelStr);
        isPaused = false; //start gry
    }
// Okno gry
bool runGame() {
    //definicja poszczególnych klas dla okna gry
    //interface gry
    class GameInterface {
    private:
        sf::Font font;
        sf::Texture tekstura;
        sf::Sprite background;
        sf::RectangleShape top; //górny pasek
        sf::RectangleShape bottom; //dolny pasek
        sf::Text levelText;
        sf::Text scoreText;
        sf::Text timeText;
        sf::Text livesText;
        sf::Text playerNameText;
        sf::Text authorText;
        int score;
        int lives;
        int level;
        std::string playerName;
        sf::Clock gameClock;
        float savedTime;
    public:
        GameInterface(float savedTime = 0.f) //konstruktor interface'u
            : score(0), lives(5), level(1), playerName("Gracz1"), savedTime(0.f) {
            tekstura.loadFromFile("t³o.jpg");
            background.setTexture(tekstura);
            if (!font.loadFromFile("Arial.ttf")) {
                return;
            }
            //ustawienia górnego paska
            top.setSize(sf::Vector2f(float(szer), float(50.f)));
            top.setFillColor(sf::Color(50, 50, 50));
            top.setPosition(0.f, 0.f);
            //ustawienia dolnego paska
            bottom.setSize(sf::Vector2f(float(szer), float(50.f)));
            bottom.setFillColor(sf::Color(50, 50, 50));
            bottom.setPosition(0.f, float(wys - 50.f));
            //konfiguracje napisów
            configureText(scoreText, 40, "Score: 0", 100.f, 5.f);
            configureText(levelText, 40, "Level: ", 10.f, 5.f);
            configureText(timeText, 40, "Time: 0s", float(szer - 130), 5.f);
            configureText(livesText, 40, "Lives: 5", 250.f, 5.f);
            configureText(playerNameText, 40, "Player: " + playerName, 10.f, float(wys - 45));
            configureText(authorText, 40, "Game by Dominik Slominski", float(szer - 300), float(wys - 45));
            gameClock.restart(); //uruchomienie zegara gry
        }
        //funkcja konfiguruj¹ca
        void configureText(sf::Text& text, int size, const std::string& content, float x, float y) {
            text.setFont(font);
            text.setCharacterSize(size);
            text.setString(content);
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::White);
            text.setPosition(x, y);
        }
        void update() { //odœwierzanie danych interface'u
            levelText.setString("Level: " + std::to_string(level));
            playerNameText.setString("Player: " + playerName);
            int elapsedSeconds = static_cast<int>(savedTime + gameClock.getElapsedTime().asSeconds());
            timeText.setString("Time: " + std::to_string(elapsedSeconds) + "s");
            scoreText.setString("Score: " + std::to_string(score));
            livesText.setString("Lives: " + std::to_string(lives));
        }
        void addScore(int points, sf::Clock clock) { //dodawanie punktów
            if (clock.getElapsedTime().asSeconds() >= 1.f) {
                score += points;
                clock.restart();
            }
        }
        void loseLife() { //utrata punktów
            if (lives > 0)
                lives--;
        }
        int get_lives() { //funkcja zwraca liczbê ¿yæ
            return lives;
        }
        void set_name(std::string name) { //ustawianie nazwy gracza
            playerName = name;
            std::cout << "ustawiono" << std::endl;
        }
        void set_level(int lev) { //ustawianie levelu
            level = lev;
            std::cout << "ustawiono" << std::endl;
        }
        void draw(sf::RenderWindow& window) { //funkcja rysuj¹ca interface
            window.draw(background);
            window.draw(top);
            window.draw(bottom);
            window.draw(scoreText);
            window.draw(timeText);
            window.draw(livesText);
            window.draw(levelText);
            window.draw(playerNameText);
            window.draw(authorText);
        }
        void save_to_file() const {  // Funkcja zapisuj¹ca stan gry do pliku
            std::ofstream file("interface.dat", std::ios::binary); //otwarcie pliku.dat
            if (file.is_open()) {
                int elapsedSeconds = static_cast<int>(savedTime + gameClock.getElapsedTime().asSeconds());
                file.write(reinterpret_cast<const char*>(&score), sizeof(score)); //zapis do pliku
                file.write(reinterpret_cast<const char*>(&level), sizeof(level));
                file.write(reinterpret_cast<const char*>(&lives), sizeof(lives));
                file.write(reinterpret_cast<const char*>(&elapsedSeconds), sizeof(elapsedSeconds));
                size_t nameLength = playerName.size(); //d³ugoœæ nazwy gracza
                file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength)); //zapis d³ugoœci
                file.write(playerName.c_str(), nameLength);
                file.close(); //zamkniêcie pliku
                //std::cout << "zapisano dane interface'u do gry" << std::endl;
            }
            else {
                std::cerr << "Nie mo¿na zapisaæ stanu gry do pliku!" << std::endl; //obs³uga b³êdu
            }
        }
        void save_game_scores() { //zapis nazwy gracza, poziomu i liczby punktów
            std::ofstream file("Wyniki.dat", std::ios::binary);
            if (!file) { //je¿eli plik istnieje
                std::cerr << "Nie mo¿na otworzyæ pliku do zapisu!" << std::endl;
                return;
            }
            size_t nameLength = playerName.size();
            file.write(reinterpret_cast<const char*>(&nameLength), sizeof(size_t)); //zapis danych
            file.write(playerName.c_str(), nameLength);
            file.write(reinterpret_cast<const char*>(&level), sizeof(int));
            file.write(reinterpret_cast<const char*>(&score), sizeof(int));
            file.close();
            /* std::cout << "Nazwa gracza " << playerName << "poziom " << level << " liczba punktow " << score << std::endl;
             std::cout << "zapisano wynik gry do pliku" << std::endl;*/
        }
        void load_from_file() { // Funkcja wczytuj¹ca stan gry z pliku
            std::ifstream file("interface.dat", std::ios::binary); //otwarcie pliku
            if (file.is_open()) {
                int score_in, lives_in, elapsedSeconds_in, level_in; //zmienne pomocnicze
                size_t nameLength;
                file.read(reinterpret_cast<char*>(&score_in), sizeof(score_in)); //odczyt danych
                file.read(reinterpret_cast<char*>(&level_in), sizeof(level_in));
                file.read(reinterpret_cast<char*>(&lives_in), sizeof(lives_in));
                file.read(reinterpret_cast<char*>(&elapsedSeconds_in), sizeof(elapsedSeconds_in));
                file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
                std::string playerName_in(nameLength, ' ');
                file.read(&playerName_in[0], nameLength);
                file.close();
                score = score_in; //przekazanie danych pobranych z pliku do interface'u
                level = level_in;
                lives = lives_in;
                savedTime = static_cast<float>(elapsedSeconds_in);
                playerName = playerName_in;
            }
            else {
                std::cerr << "Nie mo¿na wczytaæ stanu gry z pliku!" << std::endl;
            }
        }
    };
    //klasa statek
    class statek {
    private:
        sf::Vector2f position;
        sf::Texture tekstura;
        sf::Sprite pSprite;
        float speed;
    public:
        statek(): speed(300.f) { //konstruktor rakiety
            tekstura.loadFromFile("RakietaSS.png"); //³adowanie tekstury
            pSprite.setTexture(tekstura);
            pSprite.setPosition(position);
            pSprite.setScale(sf::Vector2f(0.25f, 0.25f));
            //ustawianie puktu odniesienia na œrodek geometryczny tekstury
            pSprite.setOrigin(pSprite.getLocalBounds().width / 2, pSprite.getLocalBounds().height / 2); 
        }
        sf::FloatRect getGlobalBounds() const {//funkcja zwraca kontur sprite, u¿ywany podczas kontroli kolizji
            return pSprite.getGlobalBounds();
        }
        void animuj(float x_in, float y_in) { // Animacja statku
            sf::Vector2f moveVector(x_in, y_in); // Wektor ruchu wejœciowego
            sf::Vector2f position = pSprite.getPosition(); // Bie¿¹ca pozycja statku
            // Obliczenie nowej pozycji po ruchu
            float newX = position.x + moveVector.x;
            float newY = position.y + moveVector.y;
            // Blokowanie ruchu, jeœli nowa pozycja wykracza poza granice
            if (newX < 80.f && moveVector.x < 0) moveVector.x = 0;
            if (newX > szer - 80.f && moveVector.x > 0) moveVector.x = 0;
            if (newY < 130.f && moveVector.y < 0) moveVector.y = 0;
            if (newY > wys - 130.f && moveVector.y > 0) moveVector.y = 0;
            pSprite.move(moveVector);
        }

        sf::Sprite getStatek() { return pSprite; } //funkcja zwraca statek
        float get_speed() {
            return speed;
        }
        void setPosition(sf::Vector2f pos) {//ustawianie pozycji
            position = pos;
            pSprite.setPosition(pos);
        }
        void save_to_file() { //zapis pozycji statku do pliku
            std::ofstream file("statek.dat", std::ios::binary);
            if (!file) {
                std::cerr << "Nie mo¿na otworzyæ pliku do odczytu!" << std::endl;
                return;
            }
            file.write(reinterpret_cast<const char*>(&position.x), sizeof(float));
            file.write(reinterpret_cast<const char*>(&position.y), sizeof(float));
            file.close();
            //std::cout << "zapisano pozycjê statku" << std::endl;
        }
        void read_from_file(){ //odczyt pozycji statku
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
    //klasa meteorytów
    class Meteor {
    private:
        sf::Sprite sprite;
        sf::Vector2f velocity;
        float lastCollisionTime; //czas od ostatniej kolizji
        float speed;
        int liczba_meteorytow;
    public:
        //konstruktor meteorów
        Meteor(sf::Texture& tekstura, const sf::Vector2f& position, const sf::Vector2f& velocity): lastCollisionTime(0.f), speed(100.f), liczba_meteorytow(8){
            sprite.setTexture(tekstura);
            sprite.setScale(0.1f, 0.1f);
            sprite.setPosition(position);
            this->velocity = velocity; //ustawienie prêdkoœci wektora private
        }
        bool canCollide(float currentTime) { //sprawdzanie czasu kolizji
            return currentTime - lastCollisionTime >= 1.0f;
        }
        void registerCollision(float currentTime) { //rejestrowanie kolizji
            lastCollisionTime = currentTime;
        }
        void update(float deltaTime){ //aktualizacja prêdkoœci i pozycji meteorytów
            sf::Vector2f position = sprite.getPosition();
            sf::FloatRect bounds = sprite.getGlobalBounds(); //brzegi
            sf::Vector2f newPosition = position + velocity * deltaTime;
            if (newPosition.x < 0 || newPosition.x + bounds.width > szer) { //odbicie x
                velocity.x = -velocity.x; 
            }
            if (newPosition.y < 50.f || newPosition.y + bounds.height > wys-50.f) { //odbicie y 
                velocity.y = -velocity.y;
            }
            sprite.move(velocity * deltaTime);
        }
        void draw(sf::RenderWindow& window) const { //rysowanie
            window.draw(sprite); 
        }
        sf::FloatRect getBounds() const { //zwraca brzegi meteorytu
            return sprite.getGlobalBounds();
        }
        sf::Vector2f getPosition() const{ //zwraca pozycjê meteorytu
            return sprite.getPosition();
        }
        sf::Vector2f getVelocity() const{ //zwraca prêdkoœæ meteorytu
            return velocity;
        }
    };
    //napis game over
    class GameOverScreen {
    private:
        sf::Font font;
        sf::Text gameOverText;
    public:
        GameOverScreen() { //konstruktor
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
    //klasa funkcji obs³ugi plików
    class save {
    public:
        void save_to_file(statek st, const std::vector<Meteor> meteors, GameInterface face){//funkcja zapisu gry 
            st.save_to_file(); //zapis statku
            face.save_to_file(); //zapis interface'u
            //zapis meteorytów
            std::ofstream file("meteor.dat", std::ios::binary);
            if (!file) {
                std::cerr << "B³¹d zapisu do pliku" << std::endl;
                return;
            }
            for (const auto& meteor : meteors) { //iteracja po wszystkich elementach wektora meteorytów
                sf::Vector2f position = meteor.getPosition();
                sf::Vector2f velocity = meteor.getVelocity();
                file.write(reinterpret_cast<const char*>(&position), sizeof(position));
                file.write(reinterpret_cast<const char*>(&velocity), sizeof(velocity));
            }
            file.close();
        }
        std::vector<std::pair<sf::Vector2f, sf::Vector2f>> load_meteors_from_file() {//odczyt danych o pozycji i prêdkoœci meteorytów
            //dane s¹ zwracane w formie pary wektorów, pozycji i prêdkoœci, ka¿dy po dwie sk³adowe
            std::ifstream file("meteor.dat", std::ios::binary);
            std::vector<std::pair<sf::Vector2f, sf::Vector2f>> dane_z_pliku; //para wektorów
            if (!file) {
                std::cout << "Plik nie istnieje" << std::endl;
                return dane_z_pliku;
            }
            while (file) {
                sf::Vector2f position, velocity;
                file.read(reinterpret_cast<char*>(&position), sizeof(position));
                file.read(reinterpret_cast<char*>(&velocity), sizeof(velocity));
                if (file) {
                    dane_z_pliku.emplace_back(position, velocity); //dodanie do pary odczytanych wartoœci
                }
            }
            file.close();
            //std::cout<<"odczytano dane meteorow"<<std::endl;
            return dane_z_pliku;
        }
    };

    sf::Clock globalclock; //zegar dla kolizji meteorytów
    sf::Clock gameOverClock; //zegar odliczaj¹cy czas po koñcu gry
    sf::Clock ClockScores; //zegar odliczaj¹cy czas przydzielania punktów
    sf::Clock clock; //zegar licz¹cy czas pomiêdzy odswierzeniem okna
    srand(static_cast<unsigned>(time(nullptr)));

    sf::RenderWindow Gamewindow(sf::VideoMode(szer, wys), "Gra"); //definicja okna gry
    Gamewindow.setFramerateLimit(60); //czêstotliwoœæ odœwie¿ania okna

    GameOverScreen gameOverScreen;
    GameInterface gameInterface; //dodawanie interface'u
    statek st; //dodawanie statku
    save zapis; //deklaracja klasy zapis
    std::vector<Meteor> meteors; //dodanie meteorów
    sf::Texture meteor_tx; 
    if (!meteor_tx.loadFromFile("MeteorySS.png")) {
        std::cout << "Blad ladowania tekstury" << std::endl;
    }
    //warunek nowej gry
    if (newGame){//generowanie losowych pozycji i prêdkoœci meteorytów 
        for (int i = 0; i < 8; ++i) {
            sf::Vector2f position(
                static_cast<float>(rand() % (szer - 80)),
                static_cast<float>(rand() % (wys - 180) + 50.f)
            );
            float angle = static_cast<float>(rand()) / RAND_MAX * 360.f;
            float radians = angle * 3.14159f / 180.f;
            sf::Vector2f velocity(100.f * cos(radians), 100.f * sin(radians));
            meteors.emplace_back(meteor_tx,position, velocity);
        }
        sf::Vector2f statek_pozycja(szer / 2, wys / 2);
        st.setPosition(statek_pozycja); //ustawienie pozycji pocz¹tkowej statku
        gameInterface.set_name(player_name); //aktualizacja interface'u
        gameInterface.set_level(level_in);
    }
    else {//³adowanie meteorytów z pliku 
        auto savedMeteors = zapis.load_meteors_from_file();
        for (const auto& data : savedMeteors) {
            meteors.emplace_back(meteor_tx,data.first /*pozycja odczytana*/, data.second/*odczytana prêdkoœæ*/);
        }
        st.read_from_file();
        gameInterface.load_from_file();
    }
    //g³ówna pêtla gry
    while (Gamewindow.isOpen()) {
        bool g_over = false; //flaga konca gry
        sf::Time deltaTime = clock.restart();

        sf::Event event;
        while (Gamewindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {//wywo³anie okna potwierdzaj¹cego zamkniêcie przy zamkniêciu x
                isPaused = true;
                if (okno_Exit(Gamewindow)) {
                    if (!g_over) {
                        zapis.save_to_file(st, meteors, gameInterface); //zapis do pliku gry
                    }
                    else {
                        gameInterface.save_game_scores(); //zapis wyniku gry do pliku
                    }
                    Gamewindow.close();
                }
                else{
                    isPaused = false; //wznowienie gry
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1) {//wywo³anie okna pomocy
                isPaused = true;
                window_Help(Gamewindow); //okno pomocy
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) { //wywo³anie okna potwierdzaj¹cego zamkniêcie poprzez przycisk escape
                    isPaused = true;
                    if (okno_Exit(Gamewindow)) {
                        isPaused = false;
                        if (!g_over) {
                            zapis.save_to_file(st, meteors, gameInterface);
                        }
                        else {
                            gameInterface.save_game_scores();
                        }
                        Gamewindow.close();
                    }
                    else {
                        isPaused = false;
                    }
                }
            }
        }
        if (!isPaused) { //obs³uga dynamiki gry
            //animacja statku
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                st.animuj(float(-st.get_speed() * deltaTime.asSeconds()), 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                st.animuj(float(st.get_speed() * deltaTime.asSeconds()), 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                st.animuj(0, float(-st.get_speed() * deltaTime.asSeconds()));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                st.animuj(0, float(st.get_speed() * deltaTime.asSeconds()));
            }
            gameInterface.addScore(1.f, ClockScores); //dodawanie punktów gracza
            sf::FloatRect playerBounds = st.getGlobalBounds(); //krawêdzie statku
            for (auto& meteor : meteors) {
                meteor.update(deltaTime.asSeconds());//aktualizacja meteorytów
                if (playerBounds.intersects(meteor.getBounds())) { //sprawdzanie kolizji
                    float currentTime = globalclock.getElapsedTime().asSeconds();
                    if (meteor.canCollide(currentTime)) {
                        gameInterface.loseLife();
                        meteor.registerCollision(currentTime);
                    }
                }
            }
            if (gameInterface.get_lives() <= 0) { //koniec gry
                g_over = true;
                isPaused = true;
                gameInterface.save_game_scores();
            }
            gameInterface.update(); //aktulizacja interface'u
            Gamewindow.clear(sf::Color::Black);
            gameInterface.draw(Gamewindow);
            Gamewindow.draw(st.getStatek());

            if (g_over) {//obs³uga koñca gry
                gameOverScreen.draw(Gamewindow);
                gameOverClock.restart();
            }
            for (const auto& meteor : meteors) {//rysowanie meteorytów
                meteor.draw(Gamewindow);
            }
            Gamewindow.display();
        }
        if (gameOverClock.getElapsedTime().asSeconds() > 3.f && g_over) {//odliczanie czasu do przejœcia do menu
            isPaused = false;
            Gamewindow.close();
        }
    }
    return true;
}
//funkcja odczytuj¹ca dane poprzedniej gry
void odczytajDaneGracza(std::string& playerName, std::string& gameLevel, int& score) {
    std::ifstream file("Wyniki.dat", std::ios::binary);
    if (!file) {
        std::cerr << "Nie mo¿na otworzyæ pliku do odczytu!" << std::endl;
        return;
    }
    size_t nameLength; // Odczyt d³ugoœci nazwy gracza i samej nazwy
    file.read(reinterpret_cast<char*>(&nameLength), sizeof(size_t));
    char* nameBuffer = new char[nameLength + 1]; // Alokacja bufora
    file.read(nameBuffer, nameLength);
    nameBuffer[nameLength] = '\0'; // Dodanie terminatora
    playerName = std::string(nameBuffer);
    delete[] nameBuffer;
    file.read(reinterpret_cast<char*>(&gameLevel), sizeof(std::string)); // Odczyt poziomu trudnoœci
    file.read(reinterpret_cast<char*>(&score), sizeof(int));// Odczyt liczby punktów
    file.close();
}
//okno z wynikami gry
void Wyniki(const std::string& playerName, std::string gameLevel, int score) {
    sf::RenderWindow window(sf::VideoMode(400, 300), "Poprzednie wyniki gry");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Nie mo¿na za³adowaæ czcionki!" << std::endl;
        return;
    }
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
//funkcja odpowiadaj¹ca za za³adowanie i wyœwietenie danych
bool poprzednie_wyniki(){
    std::string Nazwa_gracza;
    std::string level;
    int punkty_gry;
    odczytajDaneGracza(Nazwa_gracza, level, punkty_gry);
    Wyniki(Nazwa_gracza, level, punkty_gry);
    return true;
};
//g³ówna funkcja ca³ej gry
int main() {
    bool runningMenu = true; //flaga otwarcia menu
    while (runningMenu) {
        sf::RenderWindow window(sf::VideoMode(szer, wys), "Menu");
        Menu menu(window.getSize().x, window.getSize().y);
        bool exitMenu = false; //flaga wyjœcia z menu
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
                        switch (menu.getSelectedItemIndex()) { //obs³uga opcji menu
                        case 0: // Nowa gra
                            window.close();
                            newGame = true;
                            nowa_gra(window); //okno nowej gry do wprowadzenia nazwy gracza i levelu
                            if (runGame()) { //funkcja odpowiadaj¹ca za kszta³t gry
                                exitMenu = true;
                            }
                            break;
                        case 1: // Wczytaj grê
                            newGame = false; 
                            //std::cout << "Wczytaj grê" << std::endl;
                            if (runGame()) { 
                                exitMenu = true;
                            }
                            break;
                        case 2: // Okno poprzednich wyników
                            if (poprzednie_wyniki()) {
                                exitMenu = true; 
                            }
                            break;
                        case 3: // Wyjœcie z gry
                            window.close();
                            runningMenu = false; //zamyka okno menu
                            break;
                        }
                    }
                }
            }
            window.clear();
            menu.draw(window);
            window.display();
        }
    }
    return 0;
}

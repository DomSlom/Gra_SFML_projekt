//#ifdef _MSC_VER
//#define _CRT_SECURE_NO_WARNINGS
//#endif
//
//#include <SFML/Graphics.hpp>
//#include <string>
//#include <ctime>
//#include <cstdlib>
//
//const int wys = 600;
//const int szer = 1000;
//const int margines = 75;
//
//typedef struct {
//    char nazwa[20];
//    int liczb_pkt;
//    char data[20]; // Pole na datê
//} gracz_cechy;
//
//void players_to_file()
//{
//    srand(time(NULL));
//    gracz_cechy playerschar[10];
//
//    FILE* fp;
//    fp = fopen("gracze.dat", "w+b");
//    if (fp == NULL) {
//        perror("B³¹d podczas otwierania pliku");
//        exit(EXIT_FAILURE);
//    }
//
//    strcpy(playerschar[0].nazwa, "Johny");
//    strcpy(playerschar[1].nazwa, "Bathman");
//    strcpy(playerschar[2].nazwa, "Dedman");
//    strcpy(playerschar[3].nazwa, "Bethymanamm");
//    strcpy(playerschar[4].nazwa, "Alloy_jr");
//    strcpy(playerschar[5].nazwa, "Poul");
//    strcpy(playerschar[6].nazwa, "Snake");
//    strcpy(playerschar[7].nazwa, "BK");
//    strcpy(playerschar[8].nazwa, "Pascal");
//    strcpy(playerschar[9].nazwa, "Java");
//
//    for (int i = 0; i < 10; i++) {
//        playerschar[i].liczb_pkt = rand() % 2000;
//
//        time_t rawtime;
//        struct tm* timeinfo;
//        time(&rawtime);
//        timeinfo = localtime(&rawtime);
//        strftime(playerschar[i].data, sizeof(playerschar[i].data), "%Y-%m-%d %H:%M:%S", timeinfo);
//    }
//
//    fwrite(playerschar, sizeof(gracz_cechy), 10, fp);
//    fclose(fp);
//}
//
//int porownaj(const void* left, const void* right) {
//    const gracz_cechy* a = (const gracz_cechy*)left;
//    const gracz_cechy* b = (const gracz_cechy*)right;
//    if (a->liczb_pkt > b->liczb_pkt) {
//        return -1;
//    }
//    else if (a->liczb_pkt < b->liczb_pkt) {
//        return 1;
//    }
//    else {
//        return 0;
//    }
//}
//
//class players_list {
//private:
//    sf::Text* players;
//    gracz_cechy* player_char;
//    sf::Font czcionka;
//    int Np;
//public:
//    players_list(int N);
//    void laduj();
//    void sortuj();
//    void draw(sf::RenderWindow& window);
//};
//
//players_list::players_list(int N)
//{
//    if (!czcionka.loadFromFile("arial.ttf"))
//        return;
//
//    FILE* fp = fopen("gracze.dat", "r+b");
//    if (fp == NULL) {
//        perror("B³¹d podczas otwierania pliku");
//        exit(EXIT_FAILURE);
//    }
//
//    unsigned int rozmiar_plik = 0, ile_graczy = 0;
//    fseek(fp, 0, SEEK_END);
//    rozmiar_plik = ftell(fp);
//    ile_graczy = rozmiar_plik / sizeof(gracz_cechy);
//    if (N > ile_graczy) {
//        Np = ile_graczy;
//    }
//    else {
//        Np = N;
//    }
//
//    players = new sf::Text[Np];
//    player_char = new gracz_cechy[Np];
//
//    rewind(fp);
//    fread(player_char, sizeof(gracz_cechy), Np, fp);
//    fclose(fp);
//
//    for (int i = 0; i < Np; i++) {
//        players[i].setFont(czcionka);
//        players[i].setCharacterSize(30);
//        players[i].setFillColor(sf::Color::Red);
//        players[i].setPosition(10 + 800 / 4, 20 + i * 50);
//    }
//}
//
//void players_list::laduj()
//{
//    std::string tmp_tekst;
//    for (int i = 0; i < Np; i++) {
//        players[i].setFont(czcionka);
//        players[i].setCharacterSize(30);
//        players[i].setFillColor(sf::Color::Blue);
//        players[i].setPosition(margines, 20 + i * 50);
//
//        tmp_tekst.assign(player_char[i].nazwa);
//        tmp_tekst += " Punkty: " + std::to_string(player_char[i].liczb_pkt);
//        tmp_tekst += " Data: " + std::string(player_char[i].data);
//        players[i].setString(tmp_tekst);
//    }
//}
//
//void players_list::sortuj()
//{
//    qsort(player_char, Np, sizeof(gracz_cechy), porownaj);
//}
//
//void players_list::draw(sf::RenderWindow& window)
//{
//    for (int i = 0; i < Np; i++) {
//        window.draw(players[i]);
//    }
//}
//
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(szer, wys), "SFML demo");
//
//    players_to_file(); // Tworzy plik z graczami
//
//    players_list* pl = new players_list(8);
//    pl->sortuj();
//    pl->laduj();
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        window.clear();
//        pl->draw(window);
//        window.display();
//    }
//
//    return 0;
//}

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <Windows.h>
#include <locale.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream> 
#include <iostream>
#undef NOMINMAX
#include <locale>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/System/String.hpp>
#include <vector>
#include <fstream>
using namespace std;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int BUTTON_WIDTH = 200;
const int BUTTON_HEIGHT = 50;
const int TITLE_WIDTH = 300;
const int TITLE_HEIGHT = 100;
const int MENU_PADDING = 20;
const int CELL_SIZE = 50;
const int FIELD_SIZE = 5;
const int PLAYER_INFO_WIDTH = 150;
const int PLAYER_INFO_HEIGHT = 50;
int level = 2;
void pvp(sf::RenderWindow& window, int* back);
int main();
void pvp_old(sf::RenderWindow& window, int* back);
void pvp_pass(sf::RenderWindow& window, int* back);
void pvp_end(sf::RenderWindow& window, int* back);
void pve_end(sf::RenderWindow& window, int* back);
void pve_old(sf::RenderWindow& window, int* back);
void pve(sf::RenderWindow& window, int* back);
void pve_pass(sf::RenderWindow& window, int* back);

enum MenuState {
    PLAY_PVP,
    PLAY_PVE,
    DIFFICULTY,
    HELP
};//фигня

enum MenuState_1 {
    isi,
    medium,
    hard
};//фигня

struct Button {
    sf::RectangleShape shape;
    sf::Text text;
};//фигня

void check_dictionary(char* slovo, int* word_len, int* check) {//проверка: есть ли слово в словаре
    FILE* f;
    f = fopen("russian_nouns.txt", "r");
    char word[50];
    fseek(f, 0, SEEK_SET);
    while (fgets(word, 50, f) != NULL) {
        int per = memcmp(slovo, word, *word_len);
        if (per == 0) {
            if (*word_len == strlen(word) - 1) {
                *check = 1;
                fseek(f, 0, SEEK_END);
            }
        }
    }
}

int poisk(char* slovo, int len) {
    FILE* f;
    f = fopen("russian_nouns.txt", "r");
    char word[50];
    int final_cnt = 0;
    int trig = 0;
    while (!feof(f)) {
        fgets(word, 50, f);
        if (word[0] == '0') {
            //printf("%c\n", word[0]);
            break;
        }
        /*if (word[0] != slovo[0]) {
            continue;
        }*/
        //printf("%s", word);
        int cnt = 0;
        while (word[cnt] != '\n') {
            cnt++;
        }
        int file_len = cnt;
        cnt = 0;
        for (int i = 0; i < file_len; i++) {
            if (word[i] == slovo[i]) {
                cnt++;
            }
            else {
                break;
            }
        }
        if (cnt == len && file_len == len) {
            fclose(f);
            if (file_len < level + 1) {
                return 1;
            }
            return 2;
        }
        else if (cnt == len) {
            //fclose(f);
            trig = 1;

        }
        else if (word[0] != slovo[0] && trig == 1) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    if (final_cnt > 0) {
        //printf("%d\n", final_cnt);
        return 1;
    }
    else {
        return 0;
    }
}

char* find_word(char field[5][5], int x, int y, char* now_word, int len_word, int oldX[25], int oldY[25], int trig, int inputX, int inputY, char old1[25][25], char old2[25][25]) {
    char* new_word = (char*)(calloc(25, sizeof(char)));
    if (x < 0 || x > 4 || y < 0 || y > 4 || field[x][y] == ' ') {
        new_word[0] = '0';
        return new_word;
    }
    int rev_trig = 1;
    int new_trig = 1;
    //char* new_word = (char*)(calloc(25, sizeof(char)));
    char* rev_word = (char*)(calloc(25, sizeof(char)));
    char* word1 = (char*)(calloc(25, sizeof(char)));
    char* word2 = (char*)(calloc(25, sizeof(char)));
    char* word3 = (char*)(calloc(25, sizeof(char)));
    char* word4 = (char*)(calloc(25, sizeof(char)));
    for (int i = 0; i < len_word; i++) {
        new_word[i] = now_word[i];
    }
    new_word[len_word] = field[x][y];
    int wordX[25];
    int wordY[25];
    for (int i = 0; i < len_word; i++) {
        wordX[i] = oldX[i];
        wordY[i] = oldY[i];
        //rev_word[i] = new_word[]
    }
    wordX[len_word] = x;
    wordY[len_word] = y;
    if (trig == 0 && inputX == x && inputY == y) {
        trig = 1;
    }
    if (len_word > level + 3) {
        new_word[0] = '0';
        return new_word;
    }
    len_word++;
    for (int i = 0; i < len_word; i++) {
        rev_word[i] = new_word[len_word - 1 - i];
    }
    //printf("%s\n", new_word);
    int cnt = 0;
    //trigInside = 1;

    for (int i = 0; i < 25; i++) {
        if (strcmp(old1[i], new_word) == 0) {
            new_trig = 0;
        }
    }
    for (int i = 0; i < 25; i++) {
        if (strcmp(old2[i], new_word) == 0) {
            new_trig = 0;
        }
    }
    for (int i = 0; i < 25; i++) {
        if (strcmp(old1[i], rev_word) == 0) {
            rev_trig = 0;
        }
    }
    for (int i = 0; i < 25; i++) {
        if (strcmp(old2[i], rev_word) == 0) {
            rev_trig = 0;
        }
    }
    if (poisk(new_word, len_word) == 2 && trig == 1 && new_trig == 1) {
        return new_word;
    }
    else if (poisk(rev_word, len_word) == 2 && trig == 1 && rev_trig == 1) {
        return rev_word;
    }
    else if (poisk(new_word, len_word) == 1 || poisk(rev_word, len_word) == 1 || trig == 0) {
        for (int i = 0; i < len_word; i++) {
            if (x - 1 == wordX[i] && y == wordY[i]) {
                word1[0] = '0';
            }
        }
        if (word1[0] != '0') {
            strcpy(word1, find_word(field, x - 1, y, new_word, len_word, wordX, wordY, trig, inputX, inputY, old1, old2));
        }
        for (int i = 0; i < len_word; i++) {
            if (x + 1 == wordX[i] && y == wordY[i]) {
                word2[0] = '0';
            }
        }
        if (word2[0] != '0') {
            strcpy(word2, find_word(field, x + 1, y, new_word, len_word, wordX, wordY, trig, inputX, inputY, old1, old2));
        }
        for (int i = 0; i < len_word; i++) {
            if (x == wordX[i] && y - 1 == wordY[i]) {
                word3[0] = '0';
            }
        }
        if (word3[0] != '0') {
            strcpy(word3, find_word(field, x, y - 1, new_word, len_word, wordX, wordY, trig, inputX, inputY, old1, old2));
        }
        for (int i = 0; i < len_word; i++) {
            if (x == wordX[i] && y + 1 == wordY[i]) {
                word4[0] = '0';
            }
        }
        if (word4[0] != '0') {
            strcpy(word4, find_word(field, x, y + 1, new_word, len_word, wordX, wordY, trig, inputX, inputY, old1, old2));
        }
        if (word1[0] != '0') {
            return word1;
        }
        else
            if (word2[0] != '0') {
                return word2;
            }
        if (word3[0] != '0') {
            return word3;
        }
        if (word4[0] != '0') {
            return word4;
        }
        free(word1);
        free(word2);
        free(word3);
        free(word4);
        new_word[0] = '0';
        return new_word;
    }
    else {
        new_word[0] = '0';
        return new_word;
    }
}

char* bot_turn(char field[5][5], char old1[25][25], char old2[25][25]) {
    char abc[] = "оеаинтсрвлкмдпуяыьгзбчйхжшюцщэфъё";
    char word[25];
    char* res_word = (char*)(calloc(25, sizeof(char)));
    int k = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int around_cnt = 0;
            //k++;
            int oldI = i;
            int oldj = j;
            printf("%d %d\n", i + 1, j + 1);
            if (field[abs(i + 1)][j] != ' ') {
                around_cnt++;
            }
            if (field[abs(i - 1)][j] != ' ') {
                around_cnt++;
            }
            if (field[i][abs(j + 1)] != ' ') {
                around_cnt++;
            }
            if (field[i][abs(j + 1)] != ' ') {
                around_cnt++;
            }
            if (field[abs(i - 1)][abs(j - 1)] != ' ') {
                around_cnt++;
            }
            if (field[abs(i + 1)][abs(j + 1)] != ' ') {
                around_cnt++;
            }
            if (field[abs(i - 1)][abs(j + 1)] != ' ') {
                around_cnt++;
            }
            if (field[abs(i + 1)][abs(j - 1)] != ' ') {
                around_cnt++;
            }
            if (field[i][j] != ' ') {
                //continue;
            }
            else {
                //int  = 0;
                for (int k = 0; k < 32; k++) {
                    //()
                    field[i][j] = abc[k];
                    //show_field(field);
                    int oldX[25];
                    int oldY[25];
                    for (int i = 0; i < 25; i++) {
                        oldX[i] = -1;
                        oldY[i] = -1;
                    }
                    res_word = find_word(field, i, j, word, 0, oldX, oldY, 1, -1, -1, old1, old2);
                    if (res_word[0] != '0') {
                        printf("%s\n", res_word);
                        return res_word;
                    }
                    else if (around_cnt >= 10) {
                        //if(field[i+1][j] == )
                        for (int q = 0; q < 5; q++) {
                            for (int r = 0; r < 5; r++) {
                                if (field[q][r] != ' ') {
                                    printf("%d %d\n", q + 1, r + 1);
                                    for (int t = 0; t < 25; t++) {
                                        oldX[t] = -1;
                                        oldY[t] = -1;
                                    }
                                    //oldX[0] = i;
                                    //oldY[0] = j;
                                    res_word = find_word(field, q, r, word, 0, oldX, oldY, 0, i, j, old1, old2);
                                    if (res_word[0] != '0') {
                                        printf("%s\n", res_word);
                                        return res_word;
                                    }
                                }
                            }
                        }
                        field[i][j] = ' ';
                    }
                    else {
                        field[i][j] = ' ';
                    }
                }
            }
            i = 4 - i;
            j = 4 - j;
            //i %= 5;
            //j %= 5;
            printf("%d %d\n", i + 1, j + 1);
            if (field[i][j] != ' ') {
                //continue;
            }
            else {
                for (int k = 0; k < 32; k++) {
                    //()
                    field[i][j] = abc[k];
                    //show_field(field);
                    int oldX[25];
                    int oldY[25];
                    for (int i = 0; i < 25; i++) {
                        oldX[i] = -1;
                        oldY[i] = -1;
                    }
                    res_word = find_word(field, i, j, word, 0, oldX, oldY, 1, -1, -1, old1, old2);
                    if (res_word[0] != '0') {
                        printf("%s\n", res_word);
                        return res_word;
                    }
                    else {
                        field[i][j] = ' ';
                    }
                }
            }
            i = oldI;
            j = oldj;
        }
    }
    printf("%s\n", "Пропуск хода");
    res_word[0] = '0';
    return res_word;
}

char* random_word(char rand_word[]) {//поиск рандоиного слова из словаря для начала игры
    FILE* fp = NULL;
    char words[20]; int len; char word[5];
    int i = 0, ran = 0; int a = 0;
    srand(time(NULL));
    fp = fopen("russian_nouns.txt", "r+");
    while (1) {
        for (; fgets(words, sizeof(words), fp); i++)
            ;
        ran = rand() % i;
        i = 0;
        rewind(fp);
        for (i = 0; i < ran; i++)
            fgets(words, sizeof(words), fp);
        for (int i = 0; i < 20; i++) {
            if (words[i] == 'ё') {
                a = 1;
            }
        }
        if ((len = strlen(words)) == 6) {
            for (int i = 0; i < 20; i++) {
                if (words[i] == 'ё') {
                    a = 1;
                }
            }
            if (a == 0) {
                for (int i = 0; i < 5; i++) {
                    rand_word[i] = words[i];
                }
                break;
            }
            a = 0;
        }
        else if (a == 1) {
            a = 0;
            rewind(fp);
        }
    }
    //printf("%s\n", words);
    return rand_word;
}

void createButton(Button& button, const sf::String& buttonText, const sf::Font& font, int x, int y) {
    y += TITLE_HEIGHT + 50;
    button.shape.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    button.shape.setPosition(x, y);
    button.shape.setFillColor(sf::Color::White);
    button.text.setFont(font);
    button.text.setString(buttonText);
    button.text.setCharacterSize(24);
    button.text.setFillColor(sf::Color::Black);
    sf::FloatRect textBounds = button.text.getLocalBounds();
    button.text.setPosition(x + (BUTTON_WIDTH - textBounds.width) / 2, y + (BUTTON_HEIGHT - textBounds.height) / 2);
}

void createButton_others(Button& button, const sf::String& buttonText, const sf::Font& font, int x, int y) {
    y += TITLE_HEIGHT + 50;
    button.shape.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    button.shape.setPosition(x, y);
    button.shape.setFillColor(sf::Color::Black);
    button.text.setFont(font);
    button.text.setString(buttonText);
    button.text.setCharacterSize(24);
    button.text.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = button.text.getLocalBounds();
    button.text.setPosition(x + (BUTTON_WIDTH - textBounds.width) / 2, y + (BUTTON_HEIGHT - textBounds.height) / 2);
}

void createButton_difficult(Button& button, const sf::String& buttonText, const sf::Font& font, int x, int y) {
    y += TITLE_HEIGHT - 100;
    button.shape.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    button.shape.setPosition(x, y);
    button.shape.setFillColor(sf::Color::White);
    button.text.setFont(font);
    button.text.setString(buttonText);
    button.text.setCharacterSize(24);
    button.text.setFillColor(sf::Color::Black);
    sf::FloatRect textBounds = button.text.getLocalBounds();
    button.text.setPosition(x + (BUTTON_WIDTH - textBounds.width) / 2 - 30, y + (BUTTON_HEIGHT - textBounds.height) / 2 - 15);
}
void createButton_status(Button& button, const sf::String& buttonText, const sf::Font& font, int x, int y) {
    y += TITLE_HEIGHT - 100;
    button.shape.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    button.shape.setPosition(x, y);
    button.shape.setFillColor(sf::Color::White);
    button.text.setFont(font);
    button.text.setString(buttonText);
    button.text.setCharacterSize(24);
    button.text.setFillColor(sf::Color::Black);
    sf::FloatRect textBounds = button.text.getLocalBounds();
    button.text.setPosition(x + (BUTTON_WIDTH - textBounds.width) / 2 - 30 - 20, y + (BUTTON_HEIGHT - textBounds.height) / 2 - 15);
}

void destroyButton(Button& button) {
    // Уничтожение ресурсов кнопки
}

int checkLetter(char let) {
    //int res = 0;
    char abc[] = "оеаинтсрвлкмдпуяыьгзбчйхжшюцщэфъё";
    for (int i = 0; i < 33; i++) {
        if (let == abc[i] - 32) {
            return 1;
        }
    }
    return 0;
}

int inArray(int array[FIELD_SIZE * FIELD_SIZE][2], int len, int y, int x) {
    for (int i = 0; i < len; i++) {
        if (array[i][0] == y && array[i][1] == x) {
            return 1;
        }
    }
    return 0;
}

struct Cell {
    sf::RectangleShape shape;
    sf::Text text;
    sf::String content;
    char value;
};

void createCell(Cell& cell, const sf::Font& font, int x, int y) { //клетка
    cell.shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    cell.shape.setPosition(x, y);
    cell.shape.setFillColor(sf::Color::Black);
    cell.shape.setOutlineThickness(2);
    cell.shape.setOutlineColor(sf::Color::White);

    cell.text.setFont(font);
    cell.text.setCharacterSize(24);
    cell.text.setFillColor(sf::Color::White);
    cell.text.setPosition(x + 10, y + 10);
}

int handleInput(const sf::Event& event, int currentState) {
    //int nowButton = -1;
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
            if (currentState > PLAY_PVP) {
                currentState--;
            }
            else {
                currentState = HELP;
            }
            return(currentState);
        }
        else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
            if (currentState < HELP) {
                currentState++;
            }
            else {
                currentState = PLAY_PVP;
            }
            return(currentState);
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            if (currentState == 0) {
                return -1;
            }
            else if (currentState == 1) {//PVE
                return 4;
            }
            else if (currentState == 2) {//difficult
                return 5;
            }
            else if (currentState == 3) {//information
                return 6;
            }
            // Обработка нажатия клавиши Enter
        }
        else {
            if (currentState < 0) {
                currentState = 0;
            }
            return currentState;
        }
    }
}

int handleInput_difficult(const sf::Event& event, int currentState) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
            if (currentState > isi) {
                currentState--;
            }
            else {
                currentState = hard;
            }
            return(currentState);
        }
        else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
            if (currentState < hard) {
                currentState++;
            }
            else {
                currentState = isi;
            }
            return(currentState);
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            if (currentState == 0) {//isi
                return -1;
            }
            else if (currentState == 1) {//medium
                return 4;
            }
            else if (currentState == 2) {//hard
                return 5;
            }
        }
        else {
            if (currentState < 0) {
                currentState = 0;
            }
            return currentState;
        }
    }
}

enum Menu_Status_game {
    new_game,
    old_game
};

int handleInput_status_game(const sf::Event& event, int currentState) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
            if (currentState > new_game) {
                currentState--;
            }
            else {
                currentState = old_game;
            }
            return(currentState);
        }
        else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
            if (currentState < old_game) {
                currentState++;
            }
            else {
                currentState = new_game;
            }
            return(currentState);
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            if (currentState == 0) {//new_game
                return -1;
            }
            else if (currentState == 1) {//old_game
                return 4;
            }
        }
        else {
            if (currentState < 0) {
                currentState = 0;
            }
            return currentState;
        }
    }
}

enum menustate_3 {
    save,
    surrender,
    pass,
    back
};
int handleInput_pvp(const sf::Event& event, int currentState) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
            if (currentState > save) {
                currentState--;
            }
            else {
                currentState = back;
            }
            return(currentState);
        }
        else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
            if (currentState < back) {
                currentState++;
            }
            else {
                currentState = save;
            }
            return(currentState);
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            if (currentState == 0) {//save
                return -1;
            }
            else if (currentState == 1) {//medium
                return 4;
            }
            else if (currentState == 2) {//hard
                return 5;
            }
            else if (currentState == 3) {
                return 6;
            }
        }
        else if (event.key.code == sf::Keyboard::Tab) {
            return 7;
        }
        else {
            if (currentState < 0) {
                currentState = 0;
            }
            return currentState;
        }
    }
}

int* cellChoice(const sf::Event& event, int selX, int selY) {
    int res[2] = { selX, selY };
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
            if (selY > 0) {
                selY--;
            }
            /* else {
                 selY = FIELD_SIZE - 1;
             }*/
            res[1] = selY;
            return(res);
        }
        else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
            if (selY < FIELD_SIZE - 1) {
                selY++;
            }
            /*else {
                selY = 0;
            }*/
            res[1] = selY;
            return(res);
        }
        else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
            if (selX > 0) {
                selX--;
            }
            /* else {
                 selX = FIELD_SIZE - 1;
             }*/
            res[0] = selX;
            return(res);
        }
        else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
            if (selX < FIELD_SIZE - 1) {
                selX++;
            }
            /*else {
                selX = 0;
            }*/
            res[0] = selX;
            return(res);
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            res[0] = -1;
            res[1] = -1;
            return(res);
            // Обработка нажатия клавиши Enter
        }
        else if (event.key.code == sf::Keyboard::Escape) {
            res[0] = -2;
            res[1] = -2;
            return(res);
            // Обработка нажатия клавиши Enter
        }
        else if (event.key.code == sf::Keyboard::Tab) {
            res[0] = -3;
            res[1] = -3;
            return(res);
            // Обработка нажатия клавиши Enter
        }
        else {
            return(res);
        }
    }
    //return currentState;
}

void information(sf::RenderWindow& window, int* back) {//кнопка справка
    setlocale(LC_ALL, "");
    sf::Font font;
    sf::Text developers;
    developers.setFont(font);
    developers.setCharacterSize(24);
    developers.setFillColor(sf::Color::White);

    Button button[1];
    const sf::String buttonLabels[] = { L"Назад\nTab" };
    int x = (WINDOW_WIDTH - BUTTON_WIDTH) - 600;
    int y = (BUTTON_HEIGHT + MENU_PADDING) * 5 + MENU_PADDING;
    createButton_others(button[0], buttonLabels[0], font, x, y);

    if (!font.loadFromFile("arial.ttf")) {
        return;
    }
    while (window.isOpen() && (*back != 1)) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.key.code == sf::Keyboard::Tab)
            {
                *back = 1;
                window.close();
            }
            else if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        developers.setString(L"Разработчики: Волгарев Платон, Аргандейкина Софья\nПреподаватель: Малышев Егор Валерьевич\n\n1) Для того, чтобы во время игры нажать кнопки сдаться,\nсохранить игру и выйти,пропустить ход сначала\nнажмите кнопку Tab,затем выбирайте соответсвующие\nкнопки \nЕсли хотите вернуться в начальное меню из данного раздела,\nнажмите Tab\n\n2) Если Вам нужно вернуться в начальное меню из разделов\nСправка или Сложность, нажмите Tab\n\n3) Если Вам нужно вернуться в начальное меню из разделов\nPLAY_PVP и PLAY_PVE,нажмите два раза Tab");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
        developers.setPosition(50, 60);
        sf::FloatRect titleBounds = developers.getLocalBounds();
        window.draw(button[0].shape);
        window.draw(button[0].text);
        window.draw(developers);
        window.display();
    }
}//кнопка справка

void difficult(sf::RenderWindow& window, int* back) {//кнопка сложность
    setlocale(LC_ALL, "");
    sf::Font font;
    sf::Text developers;
    developers.setFont(font);
    developers.setCharacterSize(24);
    developers.setFillColor(sf::Color::White);

    int currentState = -1;
    int trig = 0;

    Button button[1];
    const sf::String buttonLabels[] = { L"Назад\n  (Tab)" };
    int x_1 = (WINDOW_WIDTH - BUTTON_WIDTH) - 600;
    int y_1 = (BUTTON_HEIGHT + MENU_PADDING) * 5 + MENU_PADDING + 100;
    createButton_difficult(button[0], buttonLabels[0], font, x_1, y_1);

    Button button_isi[1];
    const sf::String buttonLabels_isi[] = { L"Легкий" };
    int x_2 = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    int y_2 = (BUTTON_HEIGHT + MENU_PADDING) * 2 + MENU_PADDING;
    createButton_difficult(button_isi[0], buttonLabels_isi[0], font, x_2, y_2);

    Button button_medium[1];
    const sf::String buttonLabels_medium[] = { L"Средний" };
    int x_3 = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    int y_3 = (BUTTON_HEIGHT + MENU_PADDING) * 3 + MENU_PADDING;
    createButton_difficult(button_medium[0], buttonLabels_medium[0], font, x_3, y_3);

    Button button_hard[1];
    const sf::String buttonLabels_hard[] = { L"Сложный" };
    int x_4 = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    int y_4 = (BUTTON_HEIGHT + MENU_PADDING) * 4 + MENU_PADDING;
    createButton_difficult(button_hard[0], buttonLabels_hard[0], font, x_4, y_4);

    if (!font.loadFromFile("arial.ttf")) {
        return;
    }
    while (window.isOpen() && (*back != 1)) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.key.code == sf::Keyboard::Tab)
            {
                *back = 1;
                window.close();
            }
            else if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                int prevState = currentState;
                currentState = handleInput_difficult(event, currentState); //обработка ввода
                if (currentState == 0) {
                    //currentState = prevState;
                    trig = 1;
                }
                else if (currentState == 1) {
                    //currentState = prevState;
                    trig = 2;
                }
                else if (currentState == 2) {
                    //currentState = prevState;
                    trig = 3;
                }
                else if (currentState == -1) {
                    currentState = prevState;
                    level = 2;
                }
                else if (currentState == 4) {
                    currentState = prevState;
                    level = 3;
                }
                else if (currentState == 5) {
                    currentState = prevState;
                    level = 4;
                }
            }
        }

        window.clear(sf::Color::Black);
        if (trig == 1 && level == 2) {
            button_isi[0].shape.setFillColor(sf::Color::Red);
            button_isi[0].text.setFillColor(sf::Color::White);
        }
        else if (trig == 1) {
            button_isi[0].shape.setFillColor(sf::Color::Green);
            button_isi[0].text.setFillColor(sf::Color::White);
        }
        else if (level == 2) {
            button_isi[0].shape.setFillColor(sf::Color::Magenta);
            button_isi[0].text.setFillColor(sf::Color::White);
        }
        else {
            button_isi[0].shape.setFillColor(sf::Color::White);
            button_isi[0].text.setFillColor(sf::Color::Black);
        }
        if (trig == 2 && level == 3) {
            button_medium[0].shape.setFillColor(sf::Color::Red);
            button_medium[0].text.setFillColor(sf::Color::White);
        }
        else if (trig == 2) {
            button_medium[0].shape.setFillColor(sf::Color::Green);
            button_medium[0].text.setFillColor(sf::Color::White);
        }
        else if (level == 3) {
            button_medium[0].shape.setFillColor(sf::Color::Magenta);
            button_medium[0].text.setFillColor(sf::Color::White);
        }
        else {
            button_medium[0].shape.setFillColor(sf::Color::White);
            button_medium[0].text.setFillColor(sf::Color::Black);
        }
        if (trig == 3 && level == 4) {
            button_hard[0].shape.setFillColor(sf::Color::Red);
            button_hard[0].text.setFillColor(sf::Color::White);
        }
        else if (trig == 3) {
            button_hard[0].shape.setFillColor(sf::Color::Green);
            button_hard[0].text.setFillColor(sf::Color::White);
        }
        else if (level == 4) {
            button_hard[0].shape.setFillColor(sf::Color::Magenta);
            button_hard[0].text.setFillColor(sf::Color::White);
        }
        else {
            button_hard[0].shape.setFillColor(sf::Color::White);
            button_hard[0].text.setFillColor(sf::Color::Black);
        }
        button[0].shape.setFillColor(sf::Color::Black);
        button[0].text.setFillColor(sf::Color::White);
        window.draw(button[0].shape);
        window.draw(button[0].text);
        window.draw(button_isi[0].shape);
        window.draw(button_isi[0].text);
        window.draw(button_medium[0].shape);
        window.draw(button_medium[0].text);
        window.draw(button_hard[0].shape);
        window.draw(button_hard[0].text);
        window.display();
    }
}

void status_game(sf::RenderWindow& window, int* back) {
    setlocale(LC_ALL, "");
    sf::Font font;
    sf::Text developers;
    developers.setFont(font);
    developers.setCharacterSize(24);
    developers.setFillColor(sf::Color::White);

    int currentState = -1;
    int trig = 0;
    int back_1 = 0;
    int Man_points = 0;
    int Bot_points = 0;
    int nowPl = 0;
    Button button[1];
    const sf::String buttonLabels[] = { L"Назад\n  (Tab)" };
    int x_1 = (WINDOW_WIDTH - BUTTON_WIDTH) - 600;
    int y_1 = (BUTTON_HEIGHT + MENU_PADDING) * 5 + MENU_PADDING + 100;
    createButton_difficult(button[0], buttonLabels[0], font, x_1, y_1);

    Button button_isi[1];
    const sf::String buttonLabels_isi[] = { L"Новая игра" };
    int x_2 = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    int y_2 = (BUTTON_HEIGHT + MENU_PADDING) * 2 + MENU_PADDING;
    createButton_status(button_isi[0], buttonLabels_isi[0], font, x_2, y_2);

    Button button_medium[1];
    const sf::String buttonLabels_medium[] = { L"Старая игра" };
    int x_3 = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    int y_3 = (BUTTON_HEIGHT + MENU_PADDING) * 3 + MENU_PADDING;
    createButton_status(button_medium[0], buttonLabels_medium[0], font, x_3, y_3);

    if (!font.loadFromFile("arial.ttf")) {
        return;
    }
    while (window.isOpen() && (*back != 1)) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.key.code == sf::Keyboard::Tab)
            {
                *back = 1;
                window.close();
            }
            else if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                int prevState = currentState;
                currentState = handleInput_status_game(event, currentState); //обработка ввода
                if (currentState == 0) {
                    //currentState = prevState;
                    trig = 1;
                }
                else if (currentState == 1) {
                    //currentState = prevState;
                    trig = 2;
                }
                else if (currentState == -1) {
                    trig = 3;
                }
                else if (currentState == 4) {
                    trig = 4;
                }
            }
        }

        window.clear(sf::Color::Black);
        if (trig == 1) {
            button_isi[0].shape.setFillColor(sf::Color::Green);
            button_isi[0].text.setFillColor(sf::Color::White);
        }
        else {
            button_isi[0].shape.setFillColor(sf::Color::White);
            button_isi[0].text.setFillColor(sf::Color::Black);
        }
        if (trig == 2) {
            button_medium[0].shape.setFillColor(sf::Color::Green);
            button_medium[0].text.setFillColor(sf::Color::White);
        }
        else {
            button_medium[0].shape.setFillColor(sf::Color::White);
            button_medium[0].text.setFillColor(sf::Color::Black);
        }
        if (trig == 3) {
            pvp(window, back);
        }
        if (trig == 4) {
            pvp_old(window, back);
        }
        button[0].shape.setFillColor(sf::Color::Black);
        button[0].text.setFillColor(sf::Color::White);
        window.draw(button[0].shape);
        window.draw(button[0].text);
        window.draw(button_isi[0].shape);
        window.draw(button_isi[0].text);
        window.draw(button_medium[0].shape);
        window.draw(button_medium[0].text);
        window.display();
    }
}

void status_game_pve(sf::RenderWindow& window, int* back) {
    setlocale(LC_ALL, "");
    sf::Font font;
    sf::Text developers;
    developers.setFont(font);
    developers.setCharacterSize(24);
    developers.setFillColor(sf::Color::White);

    int currentState = -1;
    int trig = 0;
    int back_1 = 0;
    int Man_points = 0;
    int Bot_points = 0;
    int nowPl = 0;
    Button button[1];
    const sf::String buttonLabels[] = { L"Назад\n  (Tab)" };
    int x_1 = (WINDOW_WIDTH - BUTTON_WIDTH) - 600;
    int y_1 = (BUTTON_HEIGHT + MENU_PADDING) * 5 + MENU_PADDING + 100;
    createButton_difficult(button[0], buttonLabels[0], font, x_1, y_1);

    Button button_isi[1];
    const sf::String buttonLabels_isi[] = { L"Новая игра" };
    int x_2 = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    int y_2 = (BUTTON_HEIGHT + MENU_PADDING) * 2 + MENU_PADDING;
    createButton_status(button_isi[0], buttonLabels_isi[0], font, x_2, y_2);

    Button button_medium[1];
    const sf::String buttonLabels_medium[] = { L"Старая игра" };
    int x_3 = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    int y_3 = (BUTTON_HEIGHT + MENU_PADDING) * 3 + MENU_PADDING;
    createButton_status(button_medium[0], buttonLabels_medium[0], font, x_3, y_3);

    if (!font.loadFromFile("arial.ttf")) {
        return;
    }
    while (window.isOpen() && (*back != 1)) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.key.code == sf::Keyboard::Tab)
            {
                *back = 1;
                window.close();
            }
            else if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                int prevState = currentState;
                currentState = handleInput_status_game(event, currentState); //обработка ввода
                if (currentState == 0) {
                    //currentState = prevState;
                    trig = 1;
                }
                else if (currentState == 1) {
                    //currentState = prevState;
                    trig = 2;
                }
                else if (currentState == -1) {
                    trig = 3;
                }
                else if (currentState == 4) {
                    trig = 4;
                }
            }
        }

        window.clear(sf::Color::Black);
        if (trig == 1) {
            button_isi[0].shape.setFillColor(sf::Color::Green);
            button_isi[0].text.setFillColor(sf::Color::White);
        }
        else {
            button_isi[0].shape.setFillColor(sf::Color::White);
            button_isi[0].text.setFillColor(sf::Color::Black);
        }
        if (trig == 2) {
            button_medium[0].shape.setFillColor(sf::Color::Green);
            button_medium[0].text.setFillColor(sf::Color::White);
        }
        else {
            button_medium[0].shape.setFillColor(sf::Color::White);
            button_medium[0].text.setFillColor(sf::Color::Black);
        }
        if (trig == 3) {
            pve(window, back);
        }
        if (trig == 4) {
            pve_old(window, back);
        }
        button[0].shape.setFillColor(sf::Color::Black);
        button[0].text.setFillColor(sf::Color::White);
        window.draw(button[0].shape);
        window.draw(button[0].text);
        window.draw(button_isi[0].shape);
        window.draw(button_isi[0].text);
        window.draw(button_medium[0].shape);
        window.draw(button_medium[0].text);
        window.display();
    }
}

void menu(sf::RenderWindow& window, int* back) {
    setlocale(LC_ALL, "");
    sf::Font font;
    sf::Text developers;
    developers.setFont(font);
    developers.setCharacterSize(24);
    developers.setFillColor(sf::Color::White);

    int currentState = -1;
    int trig = 0;

    Button button_isi[1];
    const sf::String buttonLabels_isi[] = { L"Сохранить" };
    int x_2 = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    int y_2 = (BUTTON_HEIGHT + MENU_PADDING) * 2 + MENU_PADDING;
    createButton_difficult(button_isi[0], buttonLabels_isi[0], font, x_2, y_2);

    Button button_medium[1];
    const sf::String buttonLabels_medium[] = { L"Сдаться" };
    int x_3 = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    int y_3 = (BUTTON_HEIGHT + MENU_PADDING) * 3 + MENU_PADDING;
    createButton_difficult(button_medium[0], buttonLabels_medium[0], font, x_3, y_3);

    Button button_hard[1];
    const sf::String buttonLabels_hard[] = { L"Пропуск" };
    int x_4 = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    int y_4 = (BUTTON_HEIGHT + MENU_PADDING) * 4 + MENU_PADDING;
    createButton_difficult(button_hard[0], buttonLabels_hard[0], font, x_4, y_4);

    Button button[1];
    const sf::String buttonLabels[] = { L"Назад" };
    int x_1 = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    int y_1 = (BUTTON_HEIGHT + MENU_PADDING) * 5 + MENU_PADDING;
    createButton_difficult(button[0], buttonLabels[0], font, x_1, y_1);

    if (!font.loadFromFile("arial.ttf")) {
        return;
    }
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                int prevState = currentState;
                currentState = handleInput_pvp(event, currentState); //обработка ввода
                if (currentState == 0) {
                    //currentState = prevState;
                    trig = 1;
                }
                else if (currentState == 1) {
                    //currentState = prevState;
                    trig = 2;
                }
                else if (currentState == 2) {
                    //currentState = prevState;
                    trig = 3;
                }
                else if (currentState == 3) {
                    //currentState = prevState;
                    trig = 4;
                }
                else if (currentState == -1) {
                    //currentState = prevState;
                    trig = 5;
                }
                else if (currentState == 5) {
                    //currentState = prevState;
                    trig = 6;
                }
                else if (currentState == 4) {
                    //currentState = prevState;
                    trig = 7;
                }
                else if (currentState == 6) {
                    trig = 8;
                }
                else if (currentState == 7) {
                    trig = 9;
                }
            }
        }

        window.clear(sf::Color::Black);
        if (trig == 1) {
            button_isi[0].shape.setFillColor(sf::Color::Green);
            button_isi[0].text.setFillColor(sf::Color::White);
        }
        else {
            button_isi[0].shape.setFillColor(sf::Color::White);
            button_isi[0].text.setFillColor(sf::Color::Black);
        }
        if (trig == 2) {
            button_medium[0].shape.setFillColor(sf::Color::Green);
            button_medium[0].text.setFillColor(sf::Color::White);
        }
        else {
            button_medium[0].shape.setFillColor(sf::Color::White);
            button_medium[0].text.setFillColor(sf::Color::Black);
        }
        if (trig == 3) {
            button_hard[0].shape.setFillColor(sf::Color::Green);
            button_hard[0].text.setFillColor(sf::Color::White);
        }
        else {
            button_hard[0].shape.setFillColor(sf::Color::White);
            button_hard[0].text.setFillColor(sf::Color::Black);
        }
        if (trig == 4) {
            button[0].shape.setFillColor(sf::Color::Green);
            button[0].text.setFillColor(sf::Color::White);
        }
        else {
            button[0].shape.setFillColor(sf::Color::White);
            button[0].text.setFillColor(sf::Color::Black);
        }
        if (trig == 5) {

            window.close();
            main();
        }
        if (trig == 6) {
            pvp_pass(window, back);
        }
        if (trig == 7) {
            pvp_end(window, back);
        }
        if (trig == 8) {
            pvp_old(window, back);
        }
        if (trig == 9) {
            window.close();
            main();
        }
        window.draw(button[0].shape);
        window.draw(button[0].text);
        window.draw(button_isi[0].shape);
        window.draw(button_isi[0].text);
        window.draw(button_medium[0].shape);
        window.draw(button_medium[0].text);
        window.draw(button_hard[0].shape);
        window.draw(button_hard[0].text);
        window.display();
    }
}

void menu_pve(sf::RenderWindow& window, int* back) {
    setlocale(LC_ALL, "");
    sf::Font font;
    sf::Text developers;
    developers.setFont(font);
    developers.setCharacterSize(24);
    developers.setFillColor(sf::Color::White);

    int currentState = -1;
    int trig = 0;

    Button button_isi[1];
    const sf::String buttonLabels_isi[] = { L"Сохранить" };
    int x_2 = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    int y_2 = (BUTTON_HEIGHT + MENU_PADDING) * 2 + MENU_PADDING;
    createButton_difficult(button_isi[0], buttonLabels_isi[0], font, x_2, y_2);

    Button button_medium[1];
    const sf::String buttonLabels_medium[] = { L"Сдаться" };
    int x_3 = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    int y_3 = (BUTTON_HEIGHT + MENU_PADDING) * 3 + MENU_PADDING;
    createButton_difficult(button_medium[0], buttonLabels_medium[0], font, x_3, y_3);

    Button button_hard[1];
    const sf::String buttonLabels_hard[] = { L"Пропуск" };
    int x_4 = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    int y_4 = (BUTTON_HEIGHT + MENU_PADDING) * 4 + MENU_PADDING;
    createButton_difficult(button_hard[0], buttonLabels_hard[0], font, x_4, y_4);

    Button button[1];
    const sf::String buttonLabels[] = { L"Назад" };
    int x_1 = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    int y_1 = (BUTTON_HEIGHT + MENU_PADDING) * 5 + MENU_PADDING;
    createButton_difficult(button[0], buttonLabels[0], font, x_1, y_1);

    if (!font.loadFromFile("arial.ttf")) {
        return;
    }
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                int prevState = currentState;
                currentState = handleInput_pvp(event, currentState); //обработка ввода
                if (currentState == 0) {
                    //currentState = prevState;
                    trig = 1;
                }
                else if (currentState == 1) {
                    //currentState = prevState;
                    trig = 2;
                }
                else if (currentState == 2) {
                    //currentState = prevState;
                    trig = 3;
                }
                else if (currentState == 3) {
                    //currentState = prevState;
                    trig = 4;
                }
                else if (currentState == -1) {
                    //currentState = prevState;
                    trig = 5;
                }
                else if (currentState == 5) {
                    //currentState = prevState;
                    trig = 6;
                }
                else if (currentState == 4) {
                    //currentState = prevState;
                    trig = 7;
                }
                else if (currentState == 6) {
                    trig = 8;
                }
                else if (currentState == 7) {
                    trig = 9;
                }
            }
        }

        window.clear(sf::Color::Black);
        if (trig == 1) {
            button_isi[0].shape.setFillColor(sf::Color::Green);
            button_isi[0].text.setFillColor(sf::Color::White);
        }
        else {
            button_isi[0].shape.setFillColor(sf::Color::White);
            button_isi[0].text.setFillColor(sf::Color::Black);
        }
        if (trig == 2) {
            button_medium[0].shape.setFillColor(sf::Color::Green);
            button_medium[0].text.setFillColor(sf::Color::White);
        }
        else {
            button_medium[0].shape.setFillColor(sf::Color::White);
            button_medium[0].text.setFillColor(sf::Color::Black);
        }
        if (trig == 3) {
            button_hard[0].shape.setFillColor(sf::Color::Green);
            button_hard[0].text.setFillColor(sf::Color::White);
        }
        else {
            button_hard[0].shape.setFillColor(sf::Color::White);
            button_hard[0].text.setFillColor(sf::Color::Black);
        }
        if (trig == 4) {
            button[0].shape.setFillColor(sf::Color::Green);
            button[0].text.setFillColor(sf::Color::White);
        }
        else {
            button[0].shape.setFillColor(sf::Color::White);
            button[0].text.setFillColor(sf::Color::Black);
        }
        if (trig == 5) {

            window.close();
            main();
        }
        if (trig == 6) {
            pve_pass(window, back);
        }
        if (trig == 7) {
            pve_end(window, back);
        }
        if (trig == 8) {
            pve_old(window, back);
        }
        if (trig == 9) {
            window.close();
            main();
        }
        window.draw(button[0].shape);
        window.draw(button[0].text);
        window.draw(button_isi[0].shape);
        window.draw(button_isi[0].text);
        window.draw(button_medium[0].shape);
        window.draw(button_medium[0].text);
        window.draw(button_hard[0].shape);
        window.draw(button_hard[0].text);
        window.display();
    }
}

void pvp(sf::RenderWindow& window, int* back) {
    char wordsMan[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE]; //слова и буквы(сначала буквы)
    int man_cnt = 0;
    char wordsBot[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE];
    int bot_cnt = 0;
    int xyChosen[FIELD_SIZE * FIELD_SIZE][2];
    char fieldChar[FIELD_SIZE][FIELD_SIZE];
    char wordsMan_current[FIELD_SIZE * FIELD_SIZE];
    char wordsBot_current[FIELD_SIZE * FIELD_SIZE];
    int check = 0;
    char rand_word[5];
    random_word(rand_word);
    sf::String inputText;
    sf::String inputText_1;
    sf::String FieldValue[FIELD_SIZE][FIELD_SIZE];
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            fieldChar[i][j] = ' ';
        }
    }

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            FieldValue[i][j] = " ";
        }
    }
    char alphabet[] = { "абвгдежзийклмнопрстуфхцчшщъыьэюя" };
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 33; j++) {
            if (rand_word[i] == alphabet[j]) {
                inputText_1[0] = 1072 + j - 32;
                FieldValue[2][i] = inputText_1[0];
                fieldChar[2][i] = (FieldValue[2][i]).getData()[0] - 80;
            }
        }
    }

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return;
    }

    std::vector<std::vector<Cell>> field(FIELD_SIZE, std::vector<Cell>(FIELD_SIZE)); //создаем поле
    for (int i = 0; i < FIELD_SIZE; ++i) {
        for (int j = 0; j < FIELD_SIZE; ++j) {
            int x = (WINDOW_WIDTH - (FIELD_SIZE * CELL_SIZE)) / 2 + j * CELL_SIZE;
            int y = (WINDOW_HEIGHT - (FIELD_SIZE * CELL_SIZE)) / 2 + i * CELL_SIZE;
            createCell(field[i][j], font, x, y); //создает поле 5*5
        }
    }

    Button button[1];
    const sf::String buttonLabels[] = { L"Назад\n  (Tab)" };
    int x = (WINDOW_WIDTH - BUTTON_WIDTH) - 600;
    int y = (BUTTON_HEIGHT + MENU_PADDING) * 5 + MENU_PADDING;
    createButton(button[0], buttonLabels[0], font, x, y);

    sf::Text player1Text;
    player1Text.setFont(font);
    //player1Text.setString("Player: 0");
    player1Text.setCharacterSize(24);
    player1Text.setFillColor(sf::Color::White);
    //sf::FloatRect player1Bounds = player1Text.getLocalBounds();
    //player1Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2+100);

    sf::Text player2Text;
    player2Text.setFont(font);
    player2Text.setCharacterSize(24);
    player2Text.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Man;
    Victory_Text_Man.setFont(font);
    Victory_Text_Man.setCharacterSize(24);
    Victory_Text_Man.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot;
    Victory_Text_Bot.setFont(font);
    Victory_Text_Bot.setCharacterSize(24);
    Victory_Text_Bot.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot_Man;
    Victory_Text_Bot_Man.setFont(font);
    Victory_Text_Bot_Man.setCharacterSize(24);
    Victory_Text_Bot_Man.setFillColor(sf::Color::White);


    sf::Event event;
    sf::Event Tvent;
    std::string s;
    int selX = 0;
    int selY = 0;
    int check2 = 0;
    int selX_check = 0;
    int selY_check = 0;
    int victory = 4;//0-если выиграл человек,1-если выиграл бот
    int* corSel = (int*)(calloc(2, sizeof(int)));
    corSel[0] = selX;//координаты выбранной ячейки
    corSel[1] = selY;
    int trigCel = 0;//триггер срабатывает если энтер нажать
    int nowPl = 0;//какой игрок играет:0-человек 1-бот, начинает человек
    int Man_points = 0;//очки человека
    int Bot_points = 0;//очки бота
    int status = 0;//переключение между режимом вставляется буква и выбираем слово
    int word_len = 0;//длина слова

    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//массив в котором хранятся данные о том что мы выбрали букву для слова 
        xyChosen[word_len][0] = -1;
        xyChosen[word_len][1] = -1;
    }
    while (window.isOpen() && (*back != 1)) {
        while (window.pollEvent(event) && (*back != 1)) {//проверяет происходит ли событие
            if (event.key.code == sf::Keyboard::Tab)
            {
                *back = 1;
                ofstream myfile;
                myfile.open("save.txt");
                for (int x = 0; x < 5; x++)
                {
                    for (int y = 0; y < 5; y++)
                    {
                        fieldChar[x][y] = toupper(fieldChar[x][y]);
                        myfile << fieldChar[x][y];
                    }
                    myfile << endl;
                }
                FILE* fp;
                fp = fopen("очки.txt", "w+");
                fprintf(fp, "%d\n%d\n%d\n", Man_points, Bot_points, nowPl);
                fclose(fp);
                menu(window, back);
            }
            else {
                if (nowPl == 0 || status == 3) {
                    if (status == 0) {
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 0) {
                            corSel = cellChoice(event, selX, selY);//отслеживает какая конкретно клавиша была нажата (клавиши управления)
                            if (corSel[0] + corSel[1] >= 0) {//если выбираем клетку Ю0
                                selX = corSel[0];
                                selY = corSel[1];
                            }
                            else if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;
                                corSel[1] = selY;
                                if (FieldValue[selY][selX] != " ") {//если клетка пустая
                                    trigCel = 0;//клетку нельзя выбрать
                                }
                                else {
                                    trigCel = 1;//то клетка зеленая
                                }
                            }

                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если клетка зеленая , ввели букву и нажали энтер
                            corSel = cellChoice(event, selX, selY);//координаты выбранной клетки в данный момент , чтобы отследить энтер где находится, проверяем что нажали именно энтер
                            if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;//берем предыдущие координаты
                                corSel[1] = selY;
                                selX_check = selX;
                                selY_check = selY;
                                //fieldChar[selY][selX] = (wchar_t)((int)inputText[0]-1104);
                                //printf("%d\n", inputText[0]);
                                FieldValue[selY][selX] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                                fieldChar[selY][selX] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                                if (checkLetter(fieldChar[selY][selX]) == 0) {//если буква нерусская
                                    FieldValue[selY][selX] = " ";//стираем букву, ставим пробел
                                    fieldChar[selY][selX] = ' ';
                                }
                                else {
                                    status = 1;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                                }
                                //printf("%d", fieldChar[selY][selX]);
                                //fieldChar[selY][selX][0] = (wchar_t)inputText[0];
                                /*if (isalpha(inputText[0])) {
                                    fieldChar[selY][selX][0] = (char)inputText[0];
                                }*/
                                trigCel = 0;//два раза нажали энтер(считай не нажали)

                            }
                            else if (corSel[0] + corSel[1] == -4) {//если эскейп
                                corSel[0] = selX;//предыдущие координаты возвращаем
                                corSel[1] = selY;
                                FieldValue[selY][selX] = " ";
                                fieldChar[selY][selX] = ' ';
                                trigCel = 0;
                            }
                            //printf("%d %d\n", selX, selY);
                        }
                        else if (trigCel == 1) {//если клетка зеленая
                            if (event.type == sf::Event::TextEntered) {
                                if (FieldValue[selY][selX] != " ") {
                                    trigCel = 0;
                                    continue;
                                }
                                inputText = ((wchar_t)(event.text.unicode));//считываем текст
                                if (inputText[0] >= 1072 && inputText[0] <= 1103) {//проверяет русскую букву
                                    inputText[0] -= 32;
                                }
                            }
                        }
                    }
                    else if (status == 1) {//выбираем слово
                        //int word_len = 0;
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 0) {//энтер еще не нажали
                            for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                xyChosen[i][0] = -1;
                                xyChosen[i][1] = -1;
                            }
                            corSel = cellChoice(event, selX, selY);//считываем координаты, выибраем клетку с которой слово будет начинаться
                            if (corSel[0] + corSel[1] >= 0) {//если не энтер, есть координаты
                                selX = corSel[0];//
                                selY = corSel[1];//
                            }
                            else if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;
                                corSel[1] = selY;
                                if (FieldValue[selY][selX] != " ") {//если в выбранной ячейке  есть какая-то буква
                                    trigCel = 1;//мы наэали энтер
                                    xyChosen[word_len][0] = selY;//координаты выделенной буквы
                                    xyChosen[word_len][1] = selX;
                                    //wordsMan[man_cnt][word_len] = fieldChar[selY][selX];
                                    word_len++;//длина слова плюс один
                                }
                                else {
                                    trigCel = 0;//если нет буквы то энтер нажмем второй раз
                                }
                            }
                            //printf("%d %d\n", selX, selY);
                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если мы один раз нажали энтер

                            corSel = cellChoice(event, selX, selY);
                            if (corSel[0] + corSel[1] >= 0) {//если перемещаю стрелкой
                                selX = corSel[0];
                                selY = corSel[1];
                                int a = inArray(xyChosen, word_len, selY, selX);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить
                                if (a == 0) {//выбранной ячейки нет в массиве
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    //wordsMan[man_cnt][0] = fieldChar[selY][selX];
                                    word_len++;
                                }
                                else {//если второй раз выделили одну и ту же ячейку
                                    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                        xyChosen[i][0] = -1;
                                        xyChosen[i][1] = -1;
                                    }
                                    word_len = 0;//слова нет
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    word_len++;//теперь есть первая буква
                                }
                            }
                            if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;//возвращаем в массив координаты ячейки последней
                                corSel[1] = selY;//
                                for (int i = 0; i < word_len; i++) {
                                    wordsMan_current[i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                                }
                                for (int i = 0; i < word_len; i++) {
                                    int k = 0;
                                    for (int j = 0; j < 32; j++) {
                                        if (wordsMan_current[i] == alphabet[j]) {
                                            k++;
                                            break;
                                        }
                                    }
                                    if (k == 0) {
                                        wordsMan_current[i] = wordsMan_current[i] + 32;
                                    }
                                    k = 0;
                                }
                                check_dictionary(wordsMan_current, &word_len, &check);
                                int a = inArray(xyChosen, word_len, selY_check, selX_check);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить

                                if (a == 0) {
                                    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                        xyChosen[i][0] = -1;
                                        xyChosen[i][1] = -1;
                                    }
                                    word_len = 0;//слова нет
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    word_len++;//теперь есть первая буква
                                    check = 0;
                                }
                                printf("\nпроверка ");
                                printf("%d\n", check);
                                //FieldValue[selY_check][selX_check] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                                //fieldChar[selY_check][selX_check] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                                if (check == 0) {//если слова нет в словаре
                                    FieldValue[selY_check][selX_check] = " ";//стираем букву, ставим пробел
                                    fieldChar[selY_check][selX_check] = ' ';
                                    trigCel = 0;
                                    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                        xyChosen[i][0] = -1;
                                        xyChosen[i][1] = -1;
                                    }
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    word_len = 0;
                                    status = 0;
                                    //   check2 = 1;
                                }
                                else {
                                    for (int i = 0; i < word_len; i++) {//довляем в массив слов сами буквы
                                        wordsMan[man_cnt][i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                                    }
                                    status = 0;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                                    check = 0;
                                    for (int i = 0; i < word_len; i++) {//в консоль выводятся слова
                                        if (i == 0 && nowPl == 0) {
                                            printf("Слово человека: ");
                                        }
                                        else if (i == 0 && nowPl == 1) {
                                            printf("Слово Бота: ");
                                        }
                                        printf("%c", wordsMan[man_cnt][i]);
                                    }
                                    printf("\n");
                                    if (nowPl == 0) {//если сейчас сходил человек, начисляем очки ему
                                        Man_points = Man_points + word_len;
                                        s = std::to_string(Man_points);
                                        printf("Сходил человек\nОчки человека: ");
                                        printf("%d\n\n", Man_points);
                                    }
                                    else {//если сейчас сходил бот, начисляем очки ему
                                        Bot_points = Bot_points + word_len;
                                        printf("Сходил Бот\nОчки Бота: ");
                                        printf("%d\n\n", Bot_points);
                                    }
                                    word_len = 0;
                                    man_cnt++;//слова увеличиваются
                                    if (nowPl == 0) {//если ходил человек, то теперь ход игрока
                                        nowPl = 1;
                                    }
                                    else {
                                        nowPl = 0;//если ходил игрок, то теперь ходит человек
                                    }
                                    int cnt = 0;
                                    for (int i = 0; i < FIELD_SIZE; i++) {
                                        for (int j = 0; j < FIELD_SIZE; j++) {
                                            if (FieldValue[i][j] != " ") {
                                                cnt++;
                                                if (cnt == 25) {
                                                    status = 3; //больше нет места на поле
                                                }
                                            }
                                        }
                                    }
                                    cnt = 0;
                                }
                            }
                            else if (corSel[0] + corSel[1] == -4) {//экейп
                                corSel[0] = selX;
                                corSel[1] = selY;
                                trigCel = 0;
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                word_len = 0;

                            }

                        }
                    }
                    else if (status == 3) {
                        if (Man_points > Bot_points) {
                            victory = 0;
                            printf("Поздравляем, Вы одержали победу!");
                        }
                        else if (Man_points < Bot_points) {
                            victory = 1;
                            printf("Вот одержал победу");
                        }
                        else {
                            victory = 2;
                            printf("Игра окончена с ничейным счетом");
                        }

                    }
                }
                else {
                    for (int i = 0; i < FIELD_SIZE; i++) {
                        for (int j = 0; j < FIELD_SIZE; j++) {
                            fieldChar[i][j] = tolower(fieldChar[i][j]);
                        }
                    }
                    int len = 0;

                    strcpy(wordsBot_current, bot_turn(fieldChar, wordsBot, wordsMan));
                    if (wordsBot_current[0] != '0') {
                        len = strlen(wordsBot_current);
                        strcpy(wordsBot[bot_cnt], wordsBot_current);
                        bot_cnt++;
                    }
                    for (int i = 0; i < FIELD_SIZE; i++) {
                        for (int j = 0; j < FIELD_SIZE; j++) {
                            //char prom[2];
                            //prom[0] = fieldChar[i][j];
                            if (FieldValue[i][j] == " " && fieldChar[i][j] != ' ') {
                                fieldChar[i][j] = toupper(fieldChar[i][j]);
                                printf("%d %c\n", (wchar_t)fieldChar[i][j], fieldChar[i][j]);
                                FieldValue[i][j] = (wchar_t)((wchar_t)fieldChar[i][j] - 64432);
                            }
                        }
                    }
                    Bot_points = Bot_points + len;
                    nowPl = 0;
                    trigCel = 0;
                    int cnt = 0;
                    for (int i = 0; i < FIELD_SIZE; i++) {
                        for (int j = 0; j < FIELD_SIZE; j++) {
                            if (FieldValue[i][j] != " ") {
                                cnt++;
                                if (cnt == 25) {
                                    status = 3; //больше нет места на поле
                                }
                            }
                        }
                    }
                    cnt = 0;
                }
            }
        }

        window.clear(sf::Color::Black);
        if (status == 0) {//энтер не нажат букву не ввели
            for (int i = 0; i < FIELD_SIZE; ++i) {//отрисовываем ячейки
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    //int a = inArray(xyChosen, word_len, i, j);
                    if (i == selY && j == selX && trigCel == 0) {//выбираю ячейку белая
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажала энтер зеленая
                        field[i][j].shape.setFillColor(sf::Color::Green);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(inputText);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//ячейка черная если не соответсвует ничему
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);//отрисовка фотрма
                    //if()
                    //field[i][j].text.setString(field[i][j].content + inputText);
                    window.draw(field[i][j].text);//отрисовка текст
                }
            }
        }
        else if (status == 1) {//если ввела букву и выбираю слово
            for (int i = 0; i < FIELD_SIZE; ++i) {
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    int a = inArray(xyChosen, word_len, i, j);//проверяю ячейку я выбрала или нет
                    if (i == selY && j == selX && trigCel == 0) {//если ячейка выбрана как первая буква слова
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажали энтер находимся в этой ячейке сейчас
                        field[i][j].shape.setFillColor(sf::Color::Magenta);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (a == 1 && trigCel == 1) {//ячейка которую когда-то выбрали
                        field[i][j].shape.setFillColor(sf::Color::Red);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(FieldValue[i][j]);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//просто черные ячейки
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);
                    window.draw(field[i][j].text);
                }
            }
        }
        button[0].shape.setFillColor(sf::Color::Black);
        button[0].text.setFillColor(sf::Color::White);

        if (victory == 4) {
            std::ostringstream player1ScoreString;    // объявили переменную
            player1ScoreString << Man_points;		//занесли в нее число очков, то есть формируем строку
            player1Text.setString("Player:" + player1ScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player1Text.setPosition(20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player1Text);//рисую этот текст

            std::ostringstream BotScoreString;    // объявили переменную
            BotScoreString << Bot_points;		//занесли в нее число очков, то есть формируем строку
            player2Text.setString("Bot:" + BotScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player2Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player2Text);//рисую этот текст
        }

        if (victory == 0) {
            Victory_Text_Man.setString("congratulations, you have won!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Man.getLocalBounds();
            window.draw(Victory_Text_Man);//рисую этот текст
        }
        else if (victory == 1) {
            Victory_Text_Bot.setString("the bot won");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot.getLocalBounds();
            window.draw(Victory_Text_Bot);//рисую этот текст
        }
        else if (victory == 2) {
            Victory_Text_Bot_Man.setString("draw!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot_Man.getLocalBounds();
            window.draw(Victory_Text_Bot_Man);//рисую этот текст
        }

        window.draw(button[0].shape);
        window.draw(button[0].text);
        window.display();
    }
}

void pve(sf::RenderWindow& window, int* back) {
    char wordsMan[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE]; //слова и буквы(сначала буквы)
    int man_cnt = 0;
    char wordsBot[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE];
    int bot_cnt = 0;
    int xyChosen[FIELD_SIZE * FIELD_SIZE][2];
    char fieldChar[FIELD_SIZE][FIELD_SIZE];
    char wordsMan_current[FIELD_SIZE * FIELD_SIZE];
    char wordsBot_current[FIELD_SIZE * FIELD_SIZE];
    int check = 0;
    char rand_word[5];
    random_word(rand_word);
    sf::String inputText;
    sf::String inputText_1;
    sf::String FieldValue[FIELD_SIZE][FIELD_SIZE];
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            fieldChar[i][j] = ' ';
        }
    }

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            FieldValue[i][j] = " ";
        }
    }
    char alphabet[] = { "абвгдежзийклмнопрстуфхцчшщъыьэюя" };
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 33; j++) {
            if (rand_word[i] == alphabet[j]) {
                inputText_1[0] = 1072 + j - 32;
                FieldValue[2][i] = inputText_1[0];
                fieldChar[2][i] = (FieldValue[2][i]).getData()[0] - 80;
            }
        }
    }

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return;
    }

    std::vector<std::vector<Cell>> field(FIELD_SIZE, std::vector<Cell>(FIELD_SIZE)); //создаем поле
    for (int i = 0; i < FIELD_SIZE; ++i) {
        for (int j = 0; j < FIELD_SIZE; ++j) {
            int x = (WINDOW_WIDTH - (FIELD_SIZE * CELL_SIZE)) / 2 + j * CELL_SIZE;
            int y = (WINDOW_HEIGHT - (FIELD_SIZE * CELL_SIZE)) / 2 + i * CELL_SIZE;
            createCell(field[i][j], font, x, y); //создает поле 5*5
        }
    }

    Button button[1];
    const sf::String buttonLabels[] = { L"Назад\n  (Tab)" };
    int x = (WINDOW_WIDTH - BUTTON_WIDTH) - 600;
    int y = (BUTTON_HEIGHT + MENU_PADDING) * 5 + MENU_PADDING;
    createButton(button[0], buttonLabels[0], font, x, y);

    sf::Text player1Text;
    player1Text.setFont(font);
    //player1Text.setString("Player: 0");
    player1Text.setCharacterSize(24);
    player1Text.setFillColor(sf::Color::White);
    //sf::FloatRect player1Bounds = player1Text.getLocalBounds();
    //player1Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2+100);

    sf::Text player2Text;
    player2Text.setFont(font);
    player2Text.setCharacterSize(24);
    player2Text.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Man;
    Victory_Text_Man.setFont(font);
    Victory_Text_Man.setCharacterSize(24);
    Victory_Text_Man.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot;
    Victory_Text_Bot.setFont(font);
    Victory_Text_Bot.setCharacterSize(24);
    Victory_Text_Bot.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot_Man;
    Victory_Text_Bot_Man.setFont(font);
    Victory_Text_Bot_Man.setCharacterSize(24);
    Victory_Text_Bot_Man.setFillColor(sf::Color::White);


    sf::Event event;
    sf::Event Tvent;
    std::string s;
    int selX = 0;
    int selY = 0;
    int check2 = 0;
    int selX_check = 0;
    int selY_check = 0;
    int victory = 4;//0-если выиграл человек,1-если выиграл бот
    int* corSel = (int*)(calloc(2, sizeof(int)));
    corSel[0] = selX;//координаты выбранной ячейки
    corSel[1] = selY;
    int trigCel = 0;//триггер срабатывает если энтер нажать
    int nowPl = 0;//какой игрок играет:0-человек 1-бот, начинает человек
    int Man_points = 0;//очки человека
    int Bot_points = 0;//очки бота
    int status = 0;//переключение между режимом вставляется буква и выбираем слово
    int word_len = 0;//длина слова

    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//массив в котором хранятся данные о том что мы выбрали букву для слова 
        xyChosen[word_len][0] = -1;
        xyChosen[word_len][1] = -1;
    }
    while (window.isOpen() && (*back != 1)) {
        while (window.pollEvent(event) && (*back != 1)) {//проверяет происходит ли событие
            if (event.key.code == sf::Keyboard::Tab)
            {
                *back = 1;
                ofstream myfile;
                myfile.open("save.txt");
                for (int x = 0; x < 5; x++)
                {
                    for (int y = 0; y < 5; y++)
                    {
                        fieldChar[x][y] = toupper(fieldChar[x][y]);
                        myfile << fieldChar[x][y];
                    }
                    myfile << endl;
                }
                FILE* fp;
                fp = fopen("очки.txt", "w+");
                fprintf(fp, "%d\n%d\n%d\n", Man_points, Bot_points, nowPl);
                fclose(fp);
                menu_pve(window, back);
            }
            else {

                if (status == 0) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 0) {
                        corSel = cellChoice(event, selX, selY);//отслеживает какая конкретно клавиша была нажата (клавиши управления)
                        if (corSel[0] + corSel[1] >= 0) {//если выбираем клетку Ю0
                            selX = corSel[0];
                            selY = corSel[1];
                        }
                        else if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;
                            corSel[1] = selY;
                            if (FieldValue[selY][selX] != " ") {//если клетка пустая
                                trigCel = 0;//клетку нельзя выбрать
                            }
                            else {
                                trigCel = 1;//то клетка зеленая
                            }
                        }

                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если клетка зеленая , ввели букву и нажали энтер
                        corSel = cellChoice(event, selX, selY);//координаты выбранной клетки в данный момент , чтобы отследить энтер где находится, проверяем что нажали именно энтер
                        if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;//берем предыдущие координаты
                            corSel[1] = selY;
                            selX_check = selX;
                            selY_check = selY;
                            //fieldChar[selY][selX] = (wchar_t)((int)inputText[0]-1104);
                            //printf("%d\n", inputText[0]);
                            FieldValue[selY][selX] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                            fieldChar[selY][selX] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                            if (checkLetter(fieldChar[selY][selX]) == 0) {//если буква нерусская
                                FieldValue[selY][selX] = " ";//стираем букву, ставим пробел
                                fieldChar[selY][selX] = ' ';
                            }
                            else {
                                status = 1;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                            }
                            //printf("%d", fieldChar[selY][selX]);
                            //fieldChar[selY][selX][0] = (wchar_t)inputText[0];
                            /*if (isalpha(inputText[0])) {
                                fieldChar[selY][selX][0] = (char)inputText[0];
                            }*/
                            trigCel = 0;//два раза нажали энтер(считай не нажали)

                        }
                        else if (corSel[0] + corSel[1] == -4) {//если эскейп
                            corSel[0] = selX;//предыдущие координаты возвращаем
                            corSel[1] = selY;
                            FieldValue[selY][selX] = " ";
                            fieldChar[selY][selX] = ' ';
                            trigCel = 0;
                        }
                        //printf("%d %d\n", selX, selY);
                    }
                    else if (trigCel == 1) {//если клетка зеленая
                        if (event.type == sf::Event::TextEntered) {
                            if (FieldValue[selY][selX] != " ") {
                                trigCel = 0;
                                continue;
                            }
                            inputText = ((wchar_t)(event.text.unicode));//считываем текст
                            if (inputText[0] >= 1072 && inputText[0] <= 1103) {//проверяет русскую букву
                                inputText[0] -= 32;
                            }
                        }
                    }
                }
                else if (status == 1) {//выбираем слово
                    //int word_len = 0;
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 0) {//энтер еще не нажали
                        corSel = cellChoice(event, selX, selY);//считываем координаты, выибраем клетку с которой слово будет начинаться
                        if (corSel[0] + corSel[1] >= 0) {//если не энтер, есть координаты
                            selX = corSel[0];//
                            selY = corSel[1];//
                        }
                        else if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;
                            corSel[1] = selY;
                            if (FieldValue[selY][selX] != " ") {//если в выбранной ячейке  есть какая-то буква
                                trigCel = 1;//мы наэали энтер
                                xyChosen[word_len][0] = selY;//координаты выделенной буквы
                                xyChosen[word_len][1] = selX;
                                //wordsMan[man_cnt][word_len] = fieldChar[selY][selX];
                                word_len++;//длина слова плюс один
                            }
                            else {
                                trigCel = 0;//если нет буквы то энтер нажмем второй раз
                            }
                        }
                        //printf("%d %d\n", selX, selY);
                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если мы один раз нажали энтер

                        corSel = cellChoice(event, selX, selY);
                        if (corSel[0] + corSel[1] >= 0) {//если перемещаю стрелкой
                            selX = corSel[0];
                            selY = corSel[1];
                            int a = inArray(xyChosen, word_len, selY, selX);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить
                            if (a == 0) {//выбранной ячейки нет в массиве
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                //wordsMan[man_cnt][0] = fieldChar[selY][selX];
                                word_len++;
                            }
                            else {//если второй раз выделили одну и ту же ячейку
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                word_len = 0;//слова нет
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                word_len++;//теперь есть первая буква
                            }
                        }
                        if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;//возвращаем в массив координаты ячейки последней
                            corSel[1] = selY;//
                            for (int i = 0; i < word_len; i++) {
                                wordsMan_current[i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                            }
                            for (int i = 0; i < word_len; i++) {
                                int k = 0;
                                for (int j = 0; j < 32; j++) {
                                    if (wordsMan_current[i] == alphabet[j]) {
                                        k++;
                                        break;
                                    }
                                }
                                if (k == 0) {
                                    wordsMan_current[i] = wordsMan_current[i] + 32;
                                }
                                k = 0;
                            }
                            check_dictionary(wordsMan_current, &word_len, &check);
                            int a = inArray(xyChosen, word_len, selY_check, selX_check);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить

                            if (a == 0) {
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                word_len = 0;//слова нет
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                word_len++;//теперь есть первая буква
                                check = 0;
                            }
                            printf("\nпроверка ");
                            printf("%d\n", check);
                            //FieldValue[selY_check][selX_check] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                            //fieldChar[selY_check][selX_check] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                            if (check == 0) {//если слова нет в словаре
                                FieldValue[selY_check][selX_check] = " ";//стираем букву, ставим пробел
                                fieldChar[selY_check][selX_check] = ' ';
                                trigCel = 0;
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                word_len = 0;
                                status = 0;
                                //   check2 = 1;
                            }
                            else {
                                for (int i = 0; i < word_len; i++) {//довляем в массив слов сами буквы
                                    wordsMan[man_cnt][i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                                }
                                status = 0;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                                check = 0;
                                for (int i = 0; i < word_len; i++) {//в консоль выводятся слова
                                    if (i == 0 && nowPl == 0) {
                                        printf("Слово человека: ");
                                    }
                                    else if (i == 0 && nowPl == 1) {
                                        printf("Слово Бота: ");
                                    }
                                    printf("%c", wordsMan[man_cnt][i]);
                                }
                                printf("\n");
                                if (nowPl == 0) {//если сейчас сходил человек, начисляем очки ему
                                    Man_points = Man_points + word_len;
                                    s = std::to_string(Man_points);
                                    printf("Сходил человек\nОчки человека: ");
                                    printf("%d\n\n", Man_points);
                                }
                                else {//если сейчас сходил бот, начисляем очки ему
                                    Bot_points = Bot_points + word_len;
                                    printf("Сходил Бот\nОчки Бота: ");
                                    printf("%d\n\n", Bot_points);
                                }
                                word_len = 0;
                                man_cnt++;//слова увеличиваются
                                if (nowPl == 0) {//если ходил человек, то теперь ход игрока
                                    nowPl = 1;
                                }
                                else {
                                    nowPl = 0;//если ходил игрок, то теперь ходит человек
                                }
                                int cnt = 0;
                                for (int i = 0; i < FIELD_SIZE; i++) {
                                    for (int j = 0; j < FIELD_SIZE; j++) {
                                        if (FieldValue[i][j] != " ") {
                                            cnt++;
                                            if (cnt == 25) {
                                                status = 3; //больше нет места на поле
                                            }
                                        }
                                    }
                                }
                                cnt = 0;
                            }
                        }
                        else if (corSel[0] + corSel[1] == -4) {//экейп
                            corSel[0] = selX;
                            corSel[1] = selY;
                            trigCel = 0;
                            for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                xyChosen[i][0] = -1;
                                xyChosen[i][1] = -1;
                            }
                            word_len = 0;

                        }

                    }
                }
                else if (status == 3) {
                    if (Man_points > Bot_points) {
                        victory = 0;
                        printf("Поздравляем, Вы одержали победу!");
                    }
                    else if (Man_points < Bot_points) {
                        victory = 1;
                        printf("Вот одержал победу");
                    }
                    else {
                        victory = 2;
                        printf("Игра окончена с ничейным счетом");
                    }

                }
            }
        }

        window.clear(sf::Color::Black);
        if (status == 0) {//энтер не нажат букву не ввели
            for (int i = 0; i < FIELD_SIZE; ++i) {//отрисовываем ячейки
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    //int a = inArray(xyChosen, word_len, i, j);
                    if (i == selY && j == selX && trigCel == 0) {//выбираю ячейку белая
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажала энтер зеленая
                        field[i][j].shape.setFillColor(sf::Color::Green);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(inputText);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//ячейка черная если не соответсвует ничему
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);//отрисовка фотрма
                    //if()
                    //field[i][j].text.setString(field[i][j].content + inputText);
                    window.draw(field[i][j].text);//отрисовка текст
                }
            }
        }
        else if (status == 1) {//если ввела букву и выбираю слово
            for (int i = 0; i < FIELD_SIZE; ++i) {
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    int a = inArray(xyChosen, word_len, i, j);//проверяю ячейку я выбрала или нет
                    if (i == selY && j == selX && trigCel == 0) {//если ячейка выбрана как первая буква слова
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажали энтер находимся в этой ячейке сейчас
                        field[i][j].shape.setFillColor(sf::Color::Magenta);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (a == 1 && trigCel == 1) {//ячейка которую когда-то выбрали
                        field[i][j].shape.setFillColor(sf::Color::Red);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(FieldValue[i][j]);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//просто черные ячейки
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);
                    window.draw(field[i][j].text);
                }
            }
        }
        button[0].shape.setFillColor(sf::Color::Black);
        button[0].text.setFillColor(sf::Color::White);

        if (victory == 4) {
            std::ostringstream player1ScoreString;    // объявили переменную
            player1ScoreString << Man_points;		//занесли в нее число очков, то есть формируем строку
            player1Text.setString("Player 1:" + player1ScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player1Text.setPosition(20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player1Text);//рисую этот текст

            std::ostringstream BotScoreString;    // объявили переменную
            BotScoreString << Bot_points;		//занесли в нее число очков, то есть формируем строку
            player2Text.setString("Player 2:" + BotScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player2Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player2Text);//рисую этот текст
        }

        if (victory == 0) {
            Victory_Text_Man.setString("congratulations, player 1 won!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Man.getLocalBounds();
            window.draw(Victory_Text_Man);//рисую этот текст
        }
        else if (victory == 1) {
            Victory_Text_Bot.setString("congratulations, player 2 won");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot.getLocalBounds();
            window.draw(Victory_Text_Bot);//рисую этот текст
        }
        else if (victory == 2) {
            Victory_Text_Bot_Man.setString("draw!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot_Man.getLocalBounds();
            window.draw(Victory_Text_Bot_Man);//рисую этот текст
        }

        window.draw(button[0].shape);
        window.draw(button[0].text);
        window.display();
    }
}

void pvp_old(sf::RenderWindow& window, int* back) {
    char wordsMan[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE]; //слова и буквы(сначала буквы)
    int man_cnt = 0;
    char wordsBot[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE];
    int bot_cnt = 0;
    int xyChosen[FIELD_SIZE * FIELD_SIZE][2];
    char fieldChar[FIELD_SIZE][FIELD_SIZE];
    char wordsMan_current[FIELD_SIZE * FIELD_SIZE];
    char wordsBot_current[FIELD_SIZE * FIELD_SIZE];
    int check = 0;
    char rand_word[5];
    //random_word(rand_word);
    sf::String inputText;
    sf::String inputText_1;
    sf::String FieldValue[FIELD_SIZE][FIELD_SIZE];

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            fieldChar[i][j] = ' ';
        }
    }

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            FieldValue[i][j] = " ";
        }
    }
    char fieldChar_1[5][5];
    char alphabet[] = { "абвгдежзийклмнопрстуфхцчшщъыьэюя" };
    /*for (int i = 0; i < 5; i++) {
         for (int j = 0; j < 33; j++) {
             if (rand_word[i] == alphabet[j]) {
                 inputText_1[0] = 1072 + j - 32;
                 FieldValue[2][i] = inputText_1[0];
                 fieldChar[2][i] = (FieldValue[2][i]).getData()[0] - 80;
             }
         }
     }*/
    FILE* f;
    f = fopen("save.txt", "r");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            fscanf(f, "%c", &fieldChar_1[i][j]);
            if (fieldChar_1[i][j] != '\n') {
                fieldChar[i][j] = fieldChar_1[i][j];
                char alphabet[] = { "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ" };
                for (int n = 0; n < 33; n++) {
                    if (fieldChar[i][j] == alphabet[n]) {
                        inputText_1[0] = 1072 + n - 32;
                        FieldValue[i][j] = inputText_1[0];
                        fieldChar[i][j] = (FieldValue[i][j]).getData()[0] - 80;
                        break;
                    }
                    if ((n == 32) && fieldChar[i][j] == ' ') {
                        inputText_1[0] = 160;
                        FieldValue[i][j] = " ";
                        break;
                    }
                    /*if (fieldChar[i][j] == '\n') {
                        inputText_1[0] = 10;
                        FieldValue[i][j] = inputText_1[0];
                    }*/
                }
            }
        }
    }
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return;
    }
    std::vector<std::vector<Cell>> field(FIELD_SIZE, std::vector<Cell>(FIELD_SIZE)); //создаем поле
    for (int i = 0; i < FIELD_SIZE; ++i) {
        for (int j = 0; j < FIELD_SIZE; ++j) {
            int x = (WINDOW_WIDTH - (FIELD_SIZE * CELL_SIZE)) / 2 + j * CELL_SIZE;
            int y = (WINDOW_HEIGHT - (FIELD_SIZE * CELL_SIZE)) / 2 + i * CELL_SIZE;
            createCell(field[i][j], font, x, y); //создает поле 5*5
        }
    }

    Button button[1];
    const sf::String buttonLabels[] = { L"Назад\n  (Tab)" };
    int x = (WINDOW_WIDTH - BUTTON_WIDTH) - 600;
    int y = (BUTTON_HEIGHT + MENU_PADDING) * 5 + MENU_PADDING;
    createButton(button[0], buttonLabels[0], font, x, y);

    sf::Text player1Text;
    player1Text.setFont(font);
    //player1Text.setString("Player: 0");
    player1Text.setCharacterSize(24);
    player1Text.setFillColor(sf::Color::White);
    //sf::FloatRect player1Bounds = player1Text.getLocalBounds();
    //player1Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2+100);

    sf::Text player2Text;
    player2Text.setFont(font);
    player2Text.setCharacterSize(24);
    player2Text.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Man;
    Victory_Text_Man.setFont(font);
    Victory_Text_Man.setCharacterSize(24);
    Victory_Text_Man.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot;
    Victory_Text_Bot.setFont(font);
    Victory_Text_Bot.setCharacterSize(24);
    Victory_Text_Bot.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot_Man;
    Victory_Text_Bot_Man.setFont(font);
    Victory_Text_Bot_Man.setCharacterSize(24);
    Victory_Text_Bot_Man.setFillColor(sf::Color::White);


    sf::Event event;
    sf::Event Tvent;
    std::string s;
    int selX = 0;
    int selY = 0;
    int check2 = 0;
    int selX_check = 0;
    int selY_check = 0;
    int victory = 4;//0-если выиграл человек,1-если выиграл бот
    int* corSel = (int*)(calloc(2, sizeof(int)));
    corSel[0] = selX;//координаты выбранной ячейки
    corSel[1] = selY;
    int trigCel = 0;//триггер срабатывает если энтер нажать
    int nowPl = 0;//какой игрок играет:0-человек 1-бот, начинает человек
    int Man_points = 0;//очки человека
    int Bot_points = 0;//очки бота
    int status = 0;//переключение между режимом вставляется буква и выбираем слово
    int word_len = 0;//длина слова
    int temp[3];
    FILE* fp = fopen("очки.txt", "r+");
    for (int i = 0; i < 3; i++) {
        fscanf(fp, "%d\n", &temp[i]);
    }
    Man_points = temp[0];
    Bot_points = temp[1];
    nowPl = temp[2];
    //fclose(fp);
    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//массив в котором хранятся данные о том что мы выбрали букву для слова 
        xyChosen[word_len][0] = -1;
        xyChosen[word_len][1] = -1;
    }
    *back = 0;
    while (window.isOpen() && (*back != 1)) {
        while (window.pollEvent(event) && (*back != 1)) {//проверяет происходит ли событие
            if (event.key.code == sf::Keyboard::Tab)
            {
                *back = 1;
                ofstream myfile;
                myfile.open("save.txt");
                for (int x = 0; x < 5; x++)
                {
                    for (int y = 0; y < 5; y++)
                    {
                        fieldChar[x][y] = toupper(fieldChar[x][y]);
                        myfile << fieldChar[x][y];
                    }
                    myfile << endl;
                }
                menu(window, back);
            }
            else {
                if (nowPl == 0 || status == 3) {
                    if (status == 0) {
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 0) {
                            corSel = cellChoice(event, selX, selY);//отслеживает какая конкретно клавиша была нажата (клавиши управления)
                            if (corSel[0] + corSel[1] >= 0) {//если выбираем клетку Ю0
                                selX = corSel[0];
                                selY = corSel[1];
                            }
                            else if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;
                                corSel[1] = selY;
                                if (FieldValue[selY][selX] != " ") {//если клетка пустая
                                    trigCel = 0;//клетку нельзя выбрать
                                }
                                else {
                                    trigCel = 1;//то клетка зеленая
                                }
                            }

                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если клетка зеленая , ввели букву и нажали энтер
                            corSel = cellChoice(event, selX, selY);//координаты выбранной клетки в данный момент , чтобы отследить энтер где находится, проверяем что нажали именно энтер
                            if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;//берем предыдущие координаты
                                corSel[1] = selY;
                                selX_check = selX;
                                selY_check = selY;
                                //fieldChar[selY][selX] = (wchar_t)((int)inputText[0]-1104);
                                //printf("%d\n", inputText[0]);
                                FieldValue[selY][selX] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                                fieldChar[selY][selX] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                                if (checkLetter(fieldChar[selY][selX]) == 0) {//если буква нерусская
                                    FieldValue[selY][selX] = " ";//стираем букву, ставим пробел
                                    fieldChar[selY][selX] = ' ';
                                }
                                else {
                                    status = 1;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                                }
                                //printf("%d", fieldChar[selY][selX]);
                                //fieldChar[selY][selX][0] = (wchar_t)inputText[0];
                                /*if (isalpha(inputText[0])) {
                                    fieldChar[selY][selX][0] = (char)inputText[0];
                                }*/
                                trigCel = 0;//два раза нажали энтер(считай не нажали)

                            }
                            else if (corSel[0] + corSel[1] == -4) {//если эскейп
                                corSel[0] = selX;//предыдущие координаты возвращаем
                                corSel[1] = selY;
                                FieldValue[selY][selX] = " ";
                                fieldChar[selY][selX] = ' ';
                                trigCel = 0;
                            }
                            //printf("%d %d\n", selX, selY);
                        }
                        else if (trigCel == 1) {//если клетка зеленая
                            if (event.type == sf::Event::TextEntered) {
                                if (FieldValue[selY][selX] != " ") {
                                    trigCel = 0;
                                    continue;
                                }
                                inputText = ((wchar_t)(event.text.unicode));//считываем текст
                                if (inputText[0] >= 1072 && inputText[0] <= 1103) {//проверяет русскую букву
                                    inputText[0] -= 32;
                                }
                            }
                        }
                    }
                    else if (status == 1) {//выбираем слово
                        //int word_len = 0;
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 0) {//энтер еще не нажали
                            corSel = cellChoice(event, selX, selY);//считываем координаты, выибраем клетку с которой слово будет начинаться
                            if (corSel[0] + corSel[1] >= 0) {//если не энтер, есть координаты
                                selX = corSel[0];//
                                selY = corSel[1];//
                            }
                            else if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;
                                corSel[1] = selY;
                                if (FieldValue[selY][selX] != " ") {//если в выбранной ячейке  есть какая-то буква
                                    trigCel = 1;//мы наэали энтер
                                    xyChosen[word_len][0] = selY;//координаты выделенной буквы
                                    xyChosen[word_len][1] = selX;
                                    //wordsMan[man_cnt][word_len] = fieldChar[selY][selX];
                                    word_len++;//длина слова плюс один
                                }
                                else {
                                    trigCel = 0;//если нет буквы то энтер нажмем второй раз
                                }
                            }
                            //printf("%d %d\n", selX, selY);
                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если мы один раз нажали энтер

                            corSel = cellChoice(event, selX, selY);
                            if (corSel[0] + corSel[1] >= 0) {//если перемещаю стрелкой
                                selX = corSel[0];
                                selY = corSel[1];
                                int a = inArray(xyChosen, word_len, selY, selX);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить
                                if (a == 0) {//выбранной ячейки нет в массиве
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    //wordsMan[man_cnt][0] = fieldChar[selY][selX];
                                    word_len++;
                                }
                                else {//если второй раз выделили одну и ту же ячейку
                                    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                        xyChosen[i][0] = -1;
                                        xyChosen[i][1] = -1;
                                    }
                                    word_len = 0;//слова нет
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    word_len++;//теперь есть первая буква
                                }
                            }
                            if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;//возвращаем в массив координаты ячейки последней
                                corSel[1] = selY;//
                                for (int i = 0; i < word_len; i++) {
                                    wordsMan_current[i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                                }
                                for (int i = 0; i < word_len; i++) {
                                    int k = 0;
                                    for (int j = 0; j < 32; j++) {
                                        if (wordsMan_current[i] == alphabet[j]) {
                                            k++;
                                            break;
                                        }
                                    }
                                    if (k == 0) {
                                        wordsMan_current[i] = wordsMan_current[i] + 32;
                                    }
                                    k = 0;
                                }
                                check_dictionary(wordsMan_current, &word_len, &check);
                                int a = inArray(xyChosen, word_len, selY_check, selX_check);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить

                                if (a == 0) {
                                    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                        xyChosen[i][0] = -1;
                                        xyChosen[i][1] = -1;
                                    }
                                    word_len = 0;//слова нет
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    word_len++;//теперь есть первая буква
                                    check = 0;
                                }
                                printf("\nпроверка ");
                                printf("%d\n", check);
                                //FieldValue[selY_check][selX_check] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                                //fieldChar[selY_check][selX_check] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                                if (check == 0) {//если слова нет в словаре
                                    FieldValue[selY_check][selX_check] = " ";//стираем букву, ставим пробел
                                    fieldChar[selY_check][selX_check] = ' ';
                                    trigCel = 0;
                                    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                        xyChosen[i][0] = -1;
                                        xyChosen[i][1] = -1;
                                    }
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    word_len = 0;
                                    status = 0;
                                    //   check2 = 1;
                                }
                                else {
                                    for (int i = 0; i < word_len; i++) {//довляем в массив слов сами буквы
                                        wordsMan[man_cnt][i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                                    }
                                    status = 0;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                                    check = 0;
                                    for (int i = 0; i < word_len; i++) {//в консоль выводятся слова
                                        if (i == 0 && nowPl == 0) {
                                            printf("Слово человека: ");
                                        }
                                        else if (i == 0 && nowPl == 1) {
                                            printf("Слово Бота: ");
                                        }
                                        printf("%c", wordsMan[man_cnt][i]);
                                    }
                                    printf("\n");
                                    if (nowPl == 0) {//если сейчас сходил человек, начисляем очки ему
                                        Man_points = Man_points + word_len;
                                        s = std::to_string(Man_points);
                                        printf("Сходил человек\nОчки человека: ");
                                        printf("%d\n\n", Man_points);
                                    }
                                    else {//если сейчас сходил бот, начисляем очки ему
                                        Bot_points = Bot_points + word_len;
                                        printf("Сходил Бот\nОчки Бота: ");
                                        printf("%d\n\n", Bot_points);
                                    }
                                    word_len = 0;
                                    man_cnt++;//слова увеличиваются
                                    if (nowPl == 0) {//если ходил человек, то теперь ход игрока
                                        nowPl = 1;
                                    }
                                    else {
                                        nowPl = 0;//если ходил игрок, то теперь ходит человек
                                    }
                                    int cnt = 0;
                                    for (int i = 0; i < FIELD_SIZE; i++) {
                                        for (int j = 0; j < FIELD_SIZE; j++) {
                                            if (FieldValue[i][j] != " ") {
                                                cnt++;
                                                if (cnt == 25) {
                                                    status = 3; //больше нет места на поле
                                                }
                                            }
                                        }
                                    }
                                    cnt = 0;
                                }
                            }
                            else if (corSel[0] + corSel[1] == -4) {//экейп
                                corSel[0] = selX;
                                corSel[1] = selY;
                                trigCel = 0;
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                word_len = 0;

                            }

                        }
                    }
                    else if (status == 3) {
                        if (Man_points > Bot_points) {
                            victory = 0;
                            printf("Поздравляем, Вы одержали победу!");
                        }
                        else if (Man_points < Bot_points) {
                            victory = 1;
                            printf("Вот одержал победу");
                        }
                        else {
                            victory = 2;
                            printf("Игра окончена с ничейным счетом");
                        }

                    }
                }
                else {
                    for (int i = 0; i < FIELD_SIZE; i++) {
                        for (int j = 0; j < FIELD_SIZE; j++) {
                            fieldChar[i][j] = tolower(fieldChar[i][j]);
                        }
                    }
                    int len = 0;

                    strcpy(wordsBot_current, bot_turn(fieldChar, wordsBot, wordsMan));
                    if (wordsBot_current[0] != '0') {
                        len = strlen(wordsBot_current);
                        strcpy(wordsBot[bot_cnt], wordsBot_current);
                        bot_cnt++;
                    }
                    for (int i = 0; i < FIELD_SIZE; i++) {
                        for (int j = 0; j < FIELD_SIZE; j++) {
                            //char prom[2];
                            //prom[0] = fieldChar[i][j];
                            if (FieldValue[i][j] == " " && fieldChar[i][j] != ' ') {
                                fieldChar[i][j] = toupper(fieldChar[i][j]);
                                printf("%d %c\n", (wchar_t)fieldChar[i][j], fieldChar[i][j]);
                                FieldValue[i][j] = (wchar_t)((wchar_t)fieldChar[i][j] - 64432);
                            }
                        }
                    }
                    Bot_points = Bot_points + len;
                    nowPl = 0;
                    trigCel = 0;
                    int cnt = 0;
                    for (int i = 0; i < FIELD_SIZE; i++) {
                        for (int j = 0; j < FIELD_SIZE; j++) {
                            if (FieldValue[i][j] != " ") {
                                cnt++;
                                if (cnt == 25) {
                                    status = 3; //больше нет места на поле
                                }
                            }
                        }
                    }
                    cnt = 0;
                }
            }
        }

        window.clear(sf::Color::Black);
        if (status == 0) {//энтер не нажат букву не ввели
            for (int i = 0; i < FIELD_SIZE; ++i) {//отрисовываем ячейки
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    //int a = inArray(xyChosen, word_len, i, j);
                    if (i == selY && j == selX && trigCel == 0) {//выбираю ячейку белая
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажала энтер зеленая
                        field[i][j].shape.setFillColor(sf::Color::Green);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(inputText);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//ячейка черная если не соответсвует ничему
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);//отрисовка фотрма
                    //if()
                    //field[i][j].text.setString(field[i][j].content + inputText);
                    window.draw(field[i][j].text);//отрисовка текст
                }
            }
        }
        else if (status == 1) {//если ввела букву и выбираю слово
            for (int i = 0; i < FIELD_SIZE; ++i) {
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    int a = inArray(xyChosen, word_len, i, j);//проверяю ячейку я выбрала или нет
                    if (i == selY && j == selX && trigCel == 0) {//если ячейка выбрана как первая буква слова
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажали энтер находимся в этой ячейке сейчас
                        field[i][j].shape.setFillColor(sf::Color::Magenta);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (a == 1 && trigCel == 1) {//ячейка которую когда-то выбрали
                        field[i][j].shape.setFillColor(sf::Color::Red);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(FieldValue[i][j]);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//просто черные ячейки
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);
                    window.draw(field[i][j].text);
                }
            }
        }
        button[0].shape.setFillColor(sf::Color::Black);
        button[0].text.setFillColor(sf::Color::White);

        if (victory == 4) {
            std::ostringstream player1ScoreString;    // объявили переменную
            player1ScoreString << Man_points;		//занесли в нее число очков, то есть формируем строку
            player1Text.setString("Player:" + player1ScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player1Text.setPosition(20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player1Text);//рисую этот текст

            std::ostringstream BotScoreString;    // объявили переменную
            BotScoreString << Bot_points;		//занесли в нее число очков, то есть формируем строку
            player2Text.setString("Bot:" + BotScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player2Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player2Text);//рисую этот текст
        }

        if (victory == 0) {
            Victory_Text_Man.setString("congratulations, you have won!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Man.getLocalBounds();
            window.draw(Victory_Text_Man);//рисую этот текст
        }
        else if (victory == 1) {
            Victory_Text_Bot.setString("the bot won");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot.getLocalBounds();
            window.draw(Victory_Text_Bot);//рисую этот текст
        }
        else if (victory == 2) {
            Victory_Text_Bot_Man.setString("draw!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot_Man.getLocalBounds();
            window.draw(Victory_Text_Bot_Man);//рисую этот текст
        }

        window.draw(button[0].shape);
        window.draw(button[0].text);
        window.display();
    }
}//сдаться

void pvp_pass(sf::RenderWindow& window, int* back) {
    char wordsMan[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE]; //слова и буквы(сначала буквы)
    int man_cnt = 0;
    char wordsBot[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE];
    int bot_cnt = 0;
    int xyChosen[FIELD_SIZE * FIELD_SIZE][2];
    char fieldChar[FIELD_SIZE][FIELD_SIZE];
    char wordsMan_current[FIELD_SIZE * FIELD_SIZE];
    char wordsBot_current[FIELD_SIZE * FIELD_SIZE];
    int check = 0;
    char rand_word[5];
    char alphabet[] = { "абвгдежзийклмнопрстуфхцчшщъыьэюя" };
    //random_word(rand_word);
    sf::String inputText;
    sf::String inputText_1;
    sf::String FieldValue[FIELD_SIZE][FIELD_SIZE];

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            fieldChar[i][j] = ' ';
        }
    }

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            FieldValue[i][j] = " ";
        }
    }
    char fieldChar_1[5][5];
    /* char alphabet[] = { "абвгдежзийклмнопрстуфхцчшщъыьэюя" };
     for (int i = 0; i < 5; i++) {
         for (int j = 0; j < 33; j++) {
             if (rand_word[i] == alphabet[j]) {
                 inputText_1[0] = 1072 + j - 32;
                 FieldValue[2][i] = inputText_1[0];
                 fieldChar[2][i] = (FieldValue[2][i]).getData()[0] - 80;
             }
         }
     }*/
    FILE* f;
    f = fopen("save.txt", "r");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            fscanf(f, "%c", &fieldChar_1[i][j]);
            if (fieldChar_1[i][j] != '\n') {
                fieldChar[i][j] = fieldChar_1[i][j];
                char alphabet[] = { "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ" };
                for (int n = 0; n < 33; n++) {
                    if (fieldChar[i][j] == alphabet[n]) {
                        inputText_1[0] = 1072 + n - 32;
                        FieldValue[i][j] = inputText_1[0];
                        fieldChar[i][j] = (FieldValue[i][j]).getData()[0] - 80;
                        break;
                    }
                    if ((n == 32) && fieldChar[i][j] == ' ') {
                        inputText_1[0] = 160;
                        FieldValue[i][j] = " ";
                        break;
                    }
                    /*if (fieldChar[i][j] == '\n') {
                        inputText_1[0] = 10;
                        FieldValue[i][j] = inputText_1[0];
                    }*/
                }
            }
        }
    }
    //   fclose(f);
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return;
    }
    std::vector<std::vector<Cell>> field(FIELD_SIZE, std::vector<Cell>(FIELD_SIZE)); //создаем поле
    for (int i = 0; i < FIELD_SIZE; ++i) {
        for (int j = 0; j < FIELD_SIZE; ++j) {
            int x = (WINDOW_WIDTH - (FIELD_SIZE * CELL_SIZE)) / 2 + j * CELL_SIZE;
            int y = (WINDOW_HEIGHT - (FIELD_SIZE * CELL_SIZE)) / 2 + i * CELL_SIZE;
            createCell(field[i][j], font, x, y); //создает поле 5*5
        }
    }

    Button button[1];
    const sf::String buttonLabels[] = { L"Назад\n  (Tab)" };
    int x = (WINDOW_WIDTH - BUTTON_WIDTH) - 600;
    int y = (BUTTON_HEIGHT + MENU_PADDING) * 5 + MENU_PADDING;
    createButton(button[0], buttonLabels[0], font, x, y);

    sf::Text player1Text;
    player1Text.setFont(font);
    //player1Text.setString("Player: 0");
    player1Text.setCharacterSize(24);
    player1Text.setFillColor(sf::Color::White);
    //sf::FloatRect player1Bounds = player1Text.getLocalBounds();
    //player1Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2+100);

    sf::Text player2Text;
    player2Text.setFont(font);
    player2Text.setCharacterSize(24);
    player2Text.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Man;
    Victory_Text_Man.setFont(font);
    Victory_Text_Man.setCharacterSize(24);
    Victory_Text_Man.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot;
    Victory_Text_Bot.setFont(font);
    Victory_Text_Bot.setCharacterSize(24);
    Victory_Text_Bot.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot_Man;
    Victory_Text_Bot_Man.setFont(font);
    Victory_Text_Bot_Man.setCharacterSize(24);
    Victory_Text_Bot_Man.setFillColor(sf::Color::White);


    sf::Event event;
    sf::Event Tvent;
    std::string s;
    int selX = 0;
    int selY = 0;
    int check2 = 0;
    int selX_check = 0;
    int selY_check = 0;
    int victory = 4;//0-если выиграл человек,1-если выиграл бот
    int* corSel = (int*)(calloc(2, sizeof(int)));
    corSel[0] = selX;//координаты выбранной ячейки
    corSel[1] = selY;
    int trigCel = 0;//триггер срабатывает если энтер нажать
    int nowPl = 0;//какой игрок играет:0-человек 1-бот, начинает человек
    int Man_points = 0;//очки человека
    int Bot_points = 0;//очки бота
    int status = 0;//переключение между режимом вставляется буква и выбираем слово
    int word_len = 0;//длина слова
    int temp[3];
    FILE* fp = fopen("очки.txt", "r+");
    for (int i = 0; i < 3; i++) {
        fscanf(fp, "%d\n", &temp[i]);
    }

    fclose(fp);
    Man_points = temp[0];
    Bot_points = temp[1];
    nowPl = temp[2];
    if (nowPl == 1) {
        nowPl = 0;
    }
    else if (nowPl == 0) {
        nowPl = 1;
    }
    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//массив в котором хранятся данные о том что мы выбрали букву для слова 
        xyChosen[word_len][0] = -1;
        xyChosen[word_len][1] = -1;
    }
    *back = 0;
    while (window.isOpen() && (*back != 1)) {
        while (window.pollEvent(event) && (*back != 1)) {//проверяет происходит ли событие
            if (event.key.code == sf::Keyboard::Tab)
            {
                *back = 1;
                ofstream myfile;
                myfile.open("save.txt");
                for (int x = 0; x < 5; x++)
                {
                    for (int y = 0; y < 5; y++)
                    {
                        fieldChar[x][y] = toupper(fieldChar[x][y]);
                        myfile << fieldChar[x][y];
                    }
                    myfile << endl;
                }
                menu(window, back);
            }
            else {
                if (nowPl == 0 || status == 3) {
                    if (status == 0) {
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 0) {
                            corSel = cellChoice(event, selX, selY);//отслеживает какая конкретно клавиша была нажата (клавиши управления)
                            if (corSel[0] + corSel[1] >= 0) {//если выбираем клетку Ю0
                                selX = corSel[0];
                                selY = corSel[1];
                            }
                            else if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;
                                corSel[1] = selY;
                                if (FieldValue[selY][selX] != " ") {//если клетка пустая
                                    trigCel = 0;//клетку нельзя выбрать
                                }
                                else {
                                    trigCel = 1;//то клетка зеленая
                                }
                            }

                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если клетка зеленая , ввели букву и нажали энтер
                            corSel = cellChoice(event, selX, selY);//координаты выбранной клетки в данный момент , чтобы отследить энтер где находится, проверяем что нажали именно энтер
                            if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;//берем предыдущие координаты
                                corSel[1] = selY;
                                selX_check = selX;
                                selY_check = selY;
                                //fieldChar[selY][selX] = (wchar_t)((int)inputText[0]-1104);
                                //printf("%d\n", inputText[0]);
                                FieldValue[selY][selX] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                                fieldChar[selY][selX] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                                if (checkLetter(fieldChar[selY][selX]) == 0) {//если буква нерусская
                                    FieldValue[selY][selX] = " ";//стираем букву, ставим пробел
                                    fieldChar[selY][selX] = ' ';
                                }
                                else {
                                    status = 1;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                                }
                                //printf("%d", fieldChar[selY][selX]);
                                //fieldChar[selY][selX][0] = (wchar_t)inputText[0];
                                /*if (isalpha(inputText[0])) {
                                    fieldChar[selY][selX][0] = (char)inputText[0];
                                }*/
                                trigCel = 0;//два раза нажали энтер(считай не нажали)

                            }
                            else if (corSel[0] + corSel[1] == -4) {//если эскейп
                                corSel[0] = selX;//предыдущие координаты возвращаем
                                corSel[1] = selY;
                                FieldValue[selY][selX] = " ";
                                fieldChar[selY][selX] = ' ';
                                trigCel = 0;
                            }
                            //printf("%d %d\n", selX, selY);
                        }
                        else if (trigCel == 1) {//если клетка зеленая
                            if (event.type == sf::Event::TextEntered) {
                                if (FieldValue[selY][selX] != " ") {
                                    trigCel = 0;
                                    continue;
                                }
                                inputText = ((wchar_t)(event.text.unicode));//считываем текст
                                if (inputText[0] >= 1072 && inputText[0] <= 1103) {//проверяет русскую букву
                                    inputText[0] -= 32;
                                }
                            }
                        }
                    }
                    else if (status == 1) {//выбираем слово
                        //int word_len = 0;
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 0) {//энтер еще не нажали
                            corSel = cellChoice(event, selX, selY);//считываем координаты, выибраем клетку с которой слово будет начинаться
                            if (corSel[0] + corSel[1] >= 0) {//если не энтер, есть координаты
                                selX = corSel[0];//
                                selY = corSel[1];//
                            }
                            else if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;
                                corSel[1] = selY;
                                if (FieldValue[selY][selX] != " ") {//если в выбранной ячейке  есть какая-то буква
                                    trigCel = 1;//мы наэали энтер
                                    xyChosen[word_len][0] = selY;//координаты выделенной буквы
                                    xyChosen[word_len][1] = selX;
                                    //wordsMan[man_cnt][word_len] = fieldChar[selY][selX];
                                    word_len++;//длина слова плюс один
                                }
                                else {
                                    trigCel = 0;//если нет буквы то энтер нажмем второй раз
                                }
                            }
                            //printf("%d %d\n", selX, selY);
                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если мы один раз нажали энтер

                            corSel = cellChoice(event, selX, selY);
                            if (corSel[0] + corSel[1] >= 0) {//если перемещаю стрелкой
                                selX = corSel[0];
                                selY = corSel[1];
                                int a = inArray(xyChosen, word_len, selY, selX);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить
                                if (a == 0) {//выбранной ячейки нет в массиве
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    //wordsMan[man_cnt][0] = fieldChar[selY][selX];
                                    word_len++;
                                }
                                else {//если второй раз выделили одну и ту же ячейку
                                    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                        xyChosen[i][0] = -1;
                                        xyChosen[i][1] = -1;
                                    }
                                    word_len = 0;//слова нет
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    word_len++;//теперь есть первая буква
                                }
                            }
                            if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;//возвращаем в массив координаты ячейки последней
                                corSel[1] = selY;//
                                for (int i = 0; i < word_len; i++) {
                                    wordsMan_current[i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                                }
                                for (int i = 0; i < word_len; i++) {
                                    int k = 0;
                                    for (int j = 0; j < 32; j++) {
                                        if (wordsMan_current[i] == alphabet[j]) {
                                            k++;
                                            break;
                                        }
                                    }
                                    if (k == 0) {
                                        wordsMan_current[i] = wordsMan_current[i] + 32;
                                    }
                                    k = 0;
                                }
                                check_dictionary(wordsMan_current, &word_len, &check);
                                int a = inArray(xyChosen, word_len, selY_check, selX_check);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить

                                if (a == 0) {
                                    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                        xyChosen[i][0] = -1;
                                        xyChosen[i][1] = -1;
                                    }
                                    word_len = 0;//слова нет
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    word_len++;//теперь есть первая буква
                                    check = 0;
                                }
                                printf("\nпроверка ");
                                printf("%d\n", check);
                                //FieldValue[selY_check][selX_check] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                                //fieldChar[selY_check][selX_check] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                                if (check == 0) {//если слова нет в словаре
                                    FieldValue[selY_check][selX_check] = " ";//стираем букву, ставим пробел
                                    fieldChar[selY_check][selX_check] = ' ';
                                    trigCel = 0;
                                    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                        xyChosen[i][0] = -1;
                                        xyChosen[i][1] = -1;
                                    }
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    word_len = 0;
                                    status = 0;
                                    //   check2 = 1;
                                }
                                else {
                                    for (int i = 0; i < word_len; i++) {//довляем в массив слов сами буквы
                                        wordsMan[man_cnt][i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                                    }
                                    status = 0;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                                    check = 0;
                                    for (int i = 0; i < word_len; i++) {//в консоль выводятся слова
                                        if (i == 0 && nowPl == 0) {
                                            printf("Слово человека: ");
                                        }
                                        else if (i == 0 && nowPl == 1) {
                                            printf("Слово Бота: ");
                                        }
                                        printf("%c", wordsMan[man_cnt][i]);
                                    }
                                    printf("\n");
                                    if (nowPl == 0) {//если сейчас сходил человек, начисляем очки ему
                                        Man_points = Man_points + word_len;
                                        s = std::to_string(Man_points);
                                        printf("Сходил человек\nОчки человека: ");
                                        printf("%d\n\n", Man_points);
                                    }
                                    else {//если сейчас сходил бот, начисляем очки ему
                                        Bot_points = Bot_points + word_len;
                                        printf("Сходил Бот\nОчки Бота: ");
                                        printf("%d\n\n", Bot_points);
                                    }
                                    word_len = 0;
                                    man_cnt++;//слова увеличиваются
                                    if (nowPl == 0) {//если ходил человек, то теперь ход игрока
                                        nowPl = 1;
                                    }
                                    else {
                                        nowPl = 0;//если ходил игрок, то теперь ходит человек
                                    }
                                    int cnt = 0;
                                    for (int i = 0; i < FIELD_SIZE; i++) {
                                        for (int j = 0; j < FIELD_SIZE; j++) {
                                            if (FieldValue[i][j] != " ") {
                                                cnt++;
                                                if (cnt == 25) {
                                                    status = 3; //больше нет места на поле
                                                }
                                            }
                                        }
                                    }
                                    cnt = 0;
                                }
                            }
                            else if (corSel[0] + corSel[1] == -4) {//экейп
                                corSel[0] = selX;
                                corSel[1] = selY;
                                trigCel = 0;
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                word_len = 0;

                            }

                        }
                    }
                    else if (status == 3) {
                        if (Man_points > Bot_points) {
                            victory = 0;
                            printf("Поздравляем, Вы одержали победу!");
                        }
                        else if (Man_points < Bot_points) {
                            victory = 1;
                            printf("Вот одержал победу");
                        }
                        else {
                            victory = 2;
                            printf("Игра окончена с ничейным счетом");
                        }

                    }
                }
                else {
                    for (int i = 0; i < FIELD_SIZE; i++) {
                        for (int j = 0; j < FIELD_SIZE; j++) {
                            fieldChar[i][j] = tolower(fieldChar[i][j]);
                        }
                    }
                    int len = 0;

                    strcpy(wordsBot_current, bot_turn(fieldChar, wordsBot, wordsMan));
                    if (wordsBot_current[0] != '0') {
                        len = strlen(wordsBot_current);
                        strcpy(wordsBot[bot_cnt], wordsBot_current);
                        bot_cnt++;
                    }
                    for (int i = 0; i < FIELD_SIZE; i++) {
                        for (int j = 0; j < FIELD_SIZE; j++) {
                            //char prom[2];
                            //prom[0] = fieldChar[i][j];
                            if (FieldValue[i][j] == " " && fieldChar[i][j] != ' ') {
                                fieldChar[i][j] = toupper(fieldChar[i][j]);
                                printf("%d %c\n", (wchar_t)fieldChar[i][j], fieldChar[i][j]);
                                FieldValue[i][j] = (wchar_t)((wchar_t)fieldChar[i][j] - 64432);
                            }
                        }
                    }
                    Bot_points = Bot_points + len;
                    nowPl = 0;
                    trigCel = 0;
                    int cnt = 0;
                    for (int i = 0; i < FIELD_SIZE; i++) {
                        for (int j = 0; j < FIELD_SIZE; j++) {
                            if (FieldValue[i][j] != " ") {
                                cnt++;
                                if (cnt == 25) {
                                    status = 3; //больше нет места на поле
                                }
                            }
                        }
                    }
                    cnt = 0;
                }
            }
        }

        window.clear(sf::Color::Black);
        if (status == 0) {//энтер не нажат букву не ввели
            for (int i = 0; i < FIELD_SIZE; ++i) {//отрисовываем ячейки
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    //int a = inArray(xyChosen, word_len, i, j);
                    if (i == selY && j == selX && trigCel == 0) {//выбираю ячейку белая
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажала энтер зеленая
                        field[i][j].shape.setFillColor(sf::Color::Green);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(inputText);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//ячейка черная если не соответсвует ничему
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);//отрисовка фотрма
                    //if()
                    //field[i][j].text.setString(field[i][j].content + inputText);
                    window.draw(field[i][j].text);//отрисовка текст
                }
            }
        }
        else if (status == 1) {//если ввела букву и выбираю слово
            for (int i = 0; i < FIELD_SIZE; ++i) {
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    int a = inArray(xyChosen, word_len, i, j);//проверяю ячейку я выбрала или нет
                    if (i == selY && j == selX && trigCel == 0) {//если ячейка выбрана как первая буква слова
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажали энтер находимся в этой ячейке сейчас
                        field[i][j].shape.setFillColor(sf::Color::Magenta);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (a == 1 && trigCel == 1) {//ячейка которую когда-то выбрали
                        field[i][j].shape.setFillColor(sf::Color::Red);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(FieldValue[i][j]);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//просто черные ячейки
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);
                    window.draw(field[i][j].text);
                }
            }
        }
        button[0].shape.setFillColor(sf::Color::Black);
        button[0].text.setFillColor(sf::Color::White);

        if (victory == 4) {
            std::ostringstream player1ScoreString;    // объявили переменную
            player1ScoreString << Man_points;		//занесли в нее число очков, то есть формируем строку
            player1Text.setString("Player:" + player1ScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player1Text.setPosition(20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player1Text);//рисую этот текст

            std::ostringstream BotScoreString;    // объявили переменную
            BotScoreString << Bot_points;		//занесли в нее число очков, то есть формируем строку
            player2Text.setString("Bot:" + BotScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player2Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player2Text);//рисую этот текст
        }

        if (victory == 0) {
            Victory_Text_Man.setString("congratulations, you have won!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Man.getLocalBounds();
            window.draw(Victory_Text_Man);//рисую этот текст
        }
        else if (victory == 1) {
            Victory_Text_Bot.setString("the bot won");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot.getLocalBounds();
            window.draw(Victory_Text_Bot);//рисую этот текст
        }
        else if (victory == 2) {
            Victory_Text_Bot_Man.setString("draw!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot_Man.getLocalBounds();
            window.draw(Victory_Text_Bot_Man);//рисую этот текст
        }

        window.draw(button[0].shape);
        window.draw(button[0].text);
        window.display();
    }
}

void pve_pass(sf::RenderWindow& window, int* back) {
    char wordsMan[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE]; //слова и буквы(сначала буквы)
    int man_cnt = 0;
    char wordsBot[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE];
    int bot_cnt = 0;
    int xyChosen[FIELD_SIZE * FIELD_SIZE][2];
    char fieldChar[FIELD_SIZE][FIELD_SIZE];
    char wordsMan_current[FIELD_SIZE * FIELD_SIZE];
    char wordsBot_current[FIELD_SIZE * FIELD_SIZE];
    int check = 0;
    char rand_word[5];
    //random_word(rand_word);
    sf::String inputText;
    sf::String inputText_1;
    sf::String FieldValue[FIELD_SIZE][FIELD_SIZE];

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            fieldChar[i][j] = ' ';
        }
    }

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            FieldValue[i][j] = " ";
        }
    }
    char fieldChar_1[5][5];
    /* char alphabet[] = { "абвгдежзийклмнопрстуфхцчшщъыьэюя" };
     for (int i = 0; i < 5; i++) {
         for (int j = 0; j < 33; j++) {
             if (rand_word[i] == alphabet[j]) {
                 inputText_1[0] = 1072 + j - 32;
                 FieldValue[2][i] = inputText_1[0];
                 fieldChar[2][i] = (FieldValue[2][i]).getData()[0] - 80;
             }
         }
     }*/
    FILE* f;
    f = fopen("save.txt", "r");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            fscanf(f, "%c", &fieldChar_1[i][j]);
            if (fieldChar_1[i][j] != '\n') {
                fieldChar[i][j] = fieldChar_1[i][j];
                char alphabet[] = { "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ" };
                for (int n = 0; n < 33; n++) {
                    if (fieldChar[i][j] == alphabet[n]) {
                        inputText_1[0] = 1072 + n - 32;
                        FieldValue[i][j] = inputText_1[0];
                        fieldChar[i][j] = (FieldValue[i][j]).getData()[0] - 80;
                        break;
                    }
                    if ((n == 32) && fieldChar[i][j] == ' ') {
                        inputText_1[0] = 160;
                        FieldValue[i][j] = " ";
                        break;
                    }
                    /*if (fieldChar[i][j] == '\n') {
                        inputText_1[0] = 10;
                        FieldValue[i][j] = inputText_1[0];
                    }*/
                }
            }
        }
    }
    //   fclose(f);
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return;
    }
    std::vector<std::vector<Cell>> field(FIELD_SIZE, std::vector<Cell>(FIELD_SIZE)); //создаем поле
    for (int i = 0; i < FIELD_SIZE; ++i) {
        for (int j = 0; j < FIELD_SIZE; ++j) {
            int x = (WINDOW_WIDTH - (FIELD_SIZE * CELL_SIZE)) / 2 + j * CELL_SIZE;
            int y = (WINDOW_HEIGHT - (FIELD_SIZE * CELL_SIZE)) / 2 + i * CELL_SIZE;
            createCell(field[i][j], font, x, y); //создает поле 5*5
        }
    }

    Button button[1];
    const sf::String buttonLabels[] = { L"Назад\n  (Tab)" };
    int x = (WINDOW_WIDTH - BUTTON_WIDTH) - 600;
    int y = (BUTTON_HEIGHT + MENU_PADDING) * 5 + MENU_PADDING;
    createButton(button[0], buttonLabels[0], font, x, y);

    sf::Text player1Text;
    player1Text.setFont(font);
    //player1Text.setString("Player: 0");
    player1Text.setCharacterSize(24);
    player1Text.setFillColor(sf::Color::White);
    //sf::FloatRect player1Bounds = player1Text.getLocalBounds();
    //player1Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2+100);

    sf::Text player2Text;
    player2Text.setFont(font);
    player2Text.setCharacterSize(24);
    player2Text.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Man;
    Victory_Text_Man.setFont(font);
    Victory_Text_Man.setCharacterSize(24);
    Victory_Text_Man.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot;
    Victory_Text_Bot.setFont(font);
    Victory_Text_Bot.setCharacterSize(24);
    Victory_Text_Bot.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot_Man;
    Victory_Text_Bot_Man.setFont(font);
    Victory_Text_Bot_Man.setCharacterSize(24);
    Victory_Text_Bot_Man.setFillColor(sf::Color::White);


    sf::Event event;
    sf::Event Tvent;
    std::string s;
    int selX = 0;
    int selY = 0;
    int check2 = 0;
    int selX_check = 0;
    int selY_check = 0;
    int victory = 4;//0-если выиграл человек,1-если выиграл бот
    int* corSel = (int*)(calloc(2, sizeof(int)));
    corSel[0] = selX;//координаты выбранной ячейки
    corSel[1] = selY;
    int trigCel = 0;//триггер срабатывает если энтер нажать
    int nowPl = 0;//какой игрок играет:0-человек 1-бот, начинает человек
    int Man_points = 0;//очки человека
    int Bot_points = 0;//очки бота
    int status = 0;//переключение между режимом вставляется буква и выбираем слово
    int word_len = 0;//длина слова
    int temp[3];
    FILE* fp = fopen("очки.txt", "r+");
    for (int i = 0; i < 3; i++) {
        fscanf(fp, "%d\n", &temp[i]);
    }

    fclose(fp);
    Man_points = temp[0];
    Bot_points = temp[1];
    nowPl = temp[2];
    if (nowPl == 1) {
        nowPl = 0;
    }
    else if (nowPl == 0) {
        nowPl = 1;
    }
    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//массив в котором хранятся данные о том что мы выбрали букву для слова 
        xyChosen[word_len][0] = -1;
        xyChosen[word_len][1] = -1;
    }
    *back = 0;
    while (window.isOpen() && (*back != 1)) {
        while (window.pollEvent(event) && (*back != 1)) {//проверяет происходит ли событие
            if (event.key.code == sf::Keyboard::Tab)
            {
                *back = 1;
                ofstream myfile;
                myfile.open("save.txt");
                for (int x = 0; x < 5; x++)
                {
                    for (int y = 0; y < 5; y++)
                    {
                        myfile << fieldChar[x][y];
                    }
                    myfile << endl;
                }
                menu(window, back);
            }
            else {

                if (status == 0) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 0) {
                        corSel = cellChoice(event, selX, selY);//отслеживает какая конкретно клавиша была нажата (клавиши управления)
                        if (corSel[0] + corSel[1] >= 0) {//если выбираем клетку Ю0
                            selX = corSel[0];
                            selY = corSel[1];
                        }
                        else if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;
                            corSel[1] = selY;
                            if (FieldValue[selY][selX] != " ") {//если клетка пустая
                                trigCel = 0;//клетку нельзя выбрать
                            }
                            else {
                                trigCel = 1;//то клетка зеленая
                            }
                        }

                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если клетка зеленая , ввели букву и нажали энтер
                        corSel = cellChoice(event, selX, selY);//координаты выбранной клетки в данный момент , чтобы отследить энтер где находится, проверяем что нажали именно энтер
                        if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;//берем предыдущие координаты
                            corSel[1] = selY;
                            selX_check = selX;
                            selY_check = selY;
                            //fieldChar[selY][selX] = (wchar_t)((int)inputText[0]-1104);
                            //printf("%d\n", inputText[0]);
                            FieldValue[selY][selX] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                            fieldChar[selY][selX] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                            if (checkLetter(fieldChar[selY][selX]) == 0) {//если буква нерусская
                                FieldValue[selY][selX] = " ";//стираем букву, ставим пробел
                                fieldChar[selY][selX] = ' ';
                            }
                            else {
                                status = 1;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                            }
                            //printf("%d", fieldChar[selY][selX]);
                            //fieldChar[selY][selX][0] = (wchar_t)inputText[0];
                            /*if (isalpha(inputText[0])) {
                                fieldChar[selY][selX][0] = (char)inputText[0];
                            }*/
                            trigCel = 0;//два раза нажали энтер(считай не нажали)

                        }
                        else if (corSel[0] + corSel[1] == -4) {//если эскейп
                            corSel[0] = selX;//предыдущие координаты возвращаем
                            corSel[1] = selY;
                            FieldValue[selY][selX] = " ";
                            fieldChar[selY][selX] = ' ';
                            trigCel = 0;
                        }
                        //printf("%d %d\n", selX, selY);
                    }
                    else if (trigCel == 1) {//если клетка зеленая
                        if (event.type == sf::Event::TextEntered) {
                            if (FieldValue[selY][selX] != " ") {
                                trigCel = 0;
                                continue;
                            }
                            inputText = ((wchar_t)(event.text.unicode));//считываем текст
                            if (inputText[0] >= 1072 && inputText[0] <= 1103) {//проверяет русскую букву
                                inputText[0] -= 32;
                            }
                        }
                    }
                }
                else if (status == 1) {//выбираем слово
                    //int word_len = 0;
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 0) {//энтер еще не нажали
                        corSel = cellChoice(event, selX, selY);//считываем координаты, выибраем клетку с которой слово будет начинаться
                        if (corSel[0] + corSel[1] >= 0) {//если не энтер, есть координаты
                            selX = corSel[0];//
                            selY = corSel[1];//
                        }
                        else if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;
                            corSel[1] = selY;
                            if (FieldValue[selY][selX] != " ") {//если в выбранной ячейке  есть какая-то буква
                                trigCel = 1;//мы наэали энтер
                                xyChosen[word_len][0] = selY;//координаты выделенной буквы
                                xyChosen[word_len][1] = selX;
                                //wordsMan[man_cnt][word_len] = fieldChar[selY][selX];
                                word_len++;//длина слова плюс один
                            }
                            else {
                                trigCel = 0;//если нет буквы то энтер нажмем второй раз
                            }
                        }
                        //printf("%d %d\n", selX, selY);
                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если мы один раз нажали энтер

                        corSel = cellChoice(event, selX, selY);
                        if (corSel[0] + corSel[1] >= 0) {//если перемещаю стрелкой
                            selX = corSel[0];
                            selY = corSel[1];
                            int a = inArray(xyChosen, word_len, selY, selX);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить
                            if (a == 0) {//выбранной ячейки нет в массиве
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                //wordsMan[man_cnt][0] = fieldChar[selY][selX];
                                word_len++;
                            }
                            else {//если второй раз выделили одну и ту же ячейку
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                word_len = 0;//слова нет
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                word_len++;//теперь есть первая буква
                            }
                        }
                        if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;//возвращаем в массив координаты ячейки последней
                            corSel[1] = selY;//
                            for (int i = 0; i < word_len; i++) {
                                wordsMan_current[i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                            }
                            for (int i = 0; i < word_len; i++) {
                                wordsMan_current[i] = wordsMan_current[i] + 32;
                            }
                            check_dictionary(wordsMan_current, &word_len, &check);
                            int a = inArray(xyChosen, word_len, selY_check, selX_check);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить

                            if (a == 0) {
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                word_len = 0;//слова нет
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                word_len++;//теперь есть первая буква
                                check = 0;
                            }
                            printf("\nпроверка ");
                            printf("%d\n", check);
                            //FieldValue[selY_check][selX_check] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                            //fieldChar[selY_check][selX_check] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                            if (check == 0) {//если слова нет в словаре
                                FieldValue[selY_check][selX_check] = " ";//стираем букву, ставим пробел
                                fieldChar[selY_check][selX_check] = ' ';
                                trigCel = 0;
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                word_len = 0;
                                status = 0;
                                //   check2 = 1;
                            }
                            else {
                                for (int i = 0; i < word_len; i++) {//довляем в массив слов сами буквы
                                    wordsMan[man_cnt][i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                                }
                                status = 0;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                                check = 0;
                                for (int i = 0; i < word_len; i++) {//в консоль выводятся слова
                                    if (i == 0 && nowPl == 0) {
                                        printf("Слово человека: ");
                                    }
                                    else if (i == 0 && nowPl == 1) {
                                        printf("Слово Бота: ");
                                    }
                                    printf("%c", wordsMan[man_cnt][i]);
                                }
                                printf("\n");
                                if (nowPl == 0) {//если сейчас сходил человек, начисляем очки ему
                                    Man_points = Man_points + word_len;
                                    s = std::to_string(Man_points);
                                    printf("Сходил человек\nОчки человека: ");
                                    printf("%d\n\n", Man_points);
                                }
                                else {//если сейчас сходил бот, начисляем очки ему
                                    Bot_points = Bot_points + word_len;
                                    printf("Сходил Бот\nОчки Бота: ");
                                    printf("%d\n\n", Bot_points);
                                }
                                word_len = 0;
                                man_cnt++;//слова увеличиваются
                                if (nowPl == 0) {//если ходил человек, то теперь ход игрока
                                    nowPl = 1;
                                }
                                else {
                                    nowPl = 0;//если ходил игрок, то теперь ходит человек
                                }
                                int cnt = 0;
                                for (int i = 0; i < FIELD_SIZE; i++) {
                                    for (int j = 0; j < FIELD_SIZE; j++) {
                                        if (FieldValue[i][j] != " ") {
                                            cnt++;
                                            if (cnt == 25) {
                                                status = 3; //больше нет места на поле
                                            }
                                        }
                                    }
                                }
                                cnt = 0;
                            }
                        }
                        else if (corSel[0] + corSel[1] == -4) {//экейп
                            corSel[0] = selX;
                            corSel[1] = selY;
                            trigCel = 0;
                            for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                xyChosen[i][0] = -1;
                                xyChosen[i][1] = -1;
                            }
                            word_len = 0;

                        }

                    }
                }
                else if (status == 3) {
                    if (Man_points > Bot_points) {
                        victory = 0;
                        printf("Поздравляем, Вы одержали победу!");
                    }
                    else if (Man_points < Bot_points) {
                        victory = 1;
                        printf("Вот одержал победу");
                    }
                    else {
                        victory = 2;
                        printf("Игра окончена с ничейным счетом");
                    }

                }
            }
        }

        window.clear(sf::Color::Black);
        if (status == 0) {//энтер не нажат букву не ввели
            for (int i = 0; i < FIELD_SIZE; ++i) {//отрисовываем ячейки
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    //int a = inArray(xyChosen, word_len, i, j);
                    if (i == selY && j == selX && trigCel == 0) {//выбираю ячейку белая
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажала энтер зеленая
                        field[i][j].shape.setFillColor(sf::Color::Green);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(inputText);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//ячейка черная если не соответсвует ничему
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);//отрисовка фотрма
                    //if()
                    //field[i][j].text.setString(field[i][j].content + inputText);
                    window.draw(field[i][j].text);//отрисовка текст
                }
            }
        }
        else if (status == 1) {//если ввела букву и выбираю слово
            for (int i = 0; i < FIELD_SIZE; ++i) {
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    int a = inArray(xyChosen, word_len, i, j);//проверяю ячейку я выбрала или нет
                    if (i == selY && j == selX && trigCel == 0) {//если ячейка выбрана как первая буква слова
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажали энтер находимся в этой ячейке сейчас
                        field[i][j].shape.setFillColor(sf::Color::Magenta);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (a == 1 && trigCel == 1) {//ячейка которую когда-то выбрали
                        field[i][j].shape.setFillColor(sf::Color::Red);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(FieldValue[i][j]);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//просто черные ячейки
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);
                    window.draw(field[i][j].text);
                }
            }
        }
        button[0].shape.setFillColor(sf::Color::Black);
        button[0].text.setFillColor(sf::Color::White);

        if (victory == 4) {
            std::ostringstream player1ScoreString;    // объявили переменную
            player1ScoreString << Man_points;		//занесли в нее число очков, то есть формируем строку
            player1Text.setString("Player 1:" + player1ScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player1Text.setPosition(20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player1Text);//рисую этот текст

            std::ostringstream BotScoreString;    // объявили переменную
            BotScoreString << Bot_points;		//занесли в нее число очков, то есть формируем строку
            player2Text.setString("Player 2:" + BotScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player2Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player2Text);//рисую этот текст
        }

        if (victory == 0) {
            Victory_Text_Man.setString("congratulations, player 1 won!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Man.getLocalBounds();
            window.draw(Victory_Text_Man);//рисую этот текст
        }
        else if (victory == 1) {
            Victory_Text_Bot.setString("congratulations, player 1 won!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot.getLocalBounds();
            window.draw(Victory_Text_Bot);//рисую этот текст
        }
        else if (victory == 2) {
            Victory_Text_Bot_Man.setString("draw!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot_Man.getLocalBounds();
            window.draw(Victory_Text_Bot_Man);//рисую этот текст
        }

        window.draw(button[0].shape);
        window.draw(button[0].text);
        window.display();
    }
}


void pvp_end(sf::RenderWindow& window, int* back) {
    char wordsMan[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE]; //слова и буквы(сначала буквы)
    int man_cnt = 0;
    char wordsBot[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE];
    int bot_cnt = 0;
    int xyChosen[FIELD_SIZE * FIELD_SIZE][2];
    char fieldChar[FIELD_SIZE][FIELD_SIZE];
    char wordsMan_current[FIELD_SIZE * FIELD_SIZE];
    char wordsBot_current[FIELD_SIZE * FIELD_SIZE];
    int check = 0;
    char rand_word[5];
    char alphabet[] = { "абвгдежзийклмнопрстуфхцчшщъыьэюя" };
    //random_word(rand_word);
    sf::String inputText;
    sf::String inputText_1;
    sf::String FieldValue[FIELD_SIZE][FIELD_SIZE];

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            fieldChar[i][j] = ' ';
        }
    }

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            FieldValue[i][j] = " ";
        }
    }
    char fieldChar_1[5][5];
    /* char alphabet[] = { "абвгдежзийклмнопрстуфхцчшщъыьэюя" };
     for (int i = 0; i < 5; i++) {
         for (int j = 0; j < 33; j++) {
             if (rand_word[i] == alphabet[j]) {
                 inputText_1[0] = 1072 + j - 32;
                 FieldValue[2][i] = inputText_1[0];
                 fieldChar[2][i] = (FieldValue[2][i]).getData()[0] - 80;
             }
         }
     }*/
    FILE* f;
    f = fopen("save.txt", "r");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            fscanf(f, "%c", &fieldChar_1[i][j]);
            if (fieldChar_1[i][j] != '\n') {
                fieldChar[i][j] = fieldChar_1[i][j];
                char alphabet[] = { "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ" };
                for (int n = 0; n < 33; n++) {
                    if (fieldChar[i][j] == alphabet[n]) {
                        inputText_1[0] = 1072 + n - 32;
                        FieldValue[i][j] = inputText_1[0];
                        fieldChar[i][j] = (FieldValue[i][j]).getData()[0] - 80;
                        break;
                    }
                    if ((n == 32) && fieldChar[i][j] == ' ') {
                        inputText_1[0] = 160;
                        FieldValue[i][j] = " ";
                        break;
                    }
                    /*if (fieldChar[i][j] == '\n') {
                        inputText_1[0] = 10;
                        FieldValue[i][j] = inputText_1[0];
                    }*/
                }
            }
        }
    }
    //   fclose(f);
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return;
    }
    std::vector<std::vector<Cell>> field(FIELD_SIZE, std::vector<Cell>(FIELD_SIZE)); //создаем поле
    for (int i = 0; i < FIELD_SIZE; ++i) {
        for (int j = 0; j < FIELD_SIZE; ++j) {
            int x = (WINDOW_WIDTH - (FIELD_SIZE * CELL_SIZE)) / 2 + j * CELL_SIZE;
            int y = (WINDOW_HEIGHT - (FIELD_SIZE * CELL_SIZE)) / 2 + i * CELL_SIZE;
            createCell(field[i][j], font, x, y); //создает поле 5*5
        }
    }

    Button button[1];
    const sf::String buttonLabels[] = { L"Назад" };
    int x = (WINDOW_WIDTH - BUTTON_WIDTH) - 600;
    int y = (BUTTON_HEIGHT + MENU_PADDING) * 5 + MENU_PADDING;
    createButton(button[0], buttonLabels[0], font, x, y);

    sf::Text player1Text;
    player1Text.setFont(font);
    //player1Text.setString("Player: 0");
    player1Text.setCharacterSize(24);
    player1Text.setFillColor(sf::Color::White);
    //sf::FloatRect player1Bounds = player1Text.getLocalBounds();
    //player1Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2+100);

    sf::Text player2Text;
    player2Text.setFont(font);
    player2Text.setCharacterSize(24);
    player2Text.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Man;
    Victory_Text_Man.setFont(font);
    Victory_Text_Man.setCharacterSize(24);
    Victory_Text_Man.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot;
    Victory_Text_Bot.setFont(font);
    Victory_Text_Bot.setCharacterSize(24);
    Victory_Text_Bot.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot_Man;
    Victory_Text_Bot_Man.setFont(font);
    Victory_Text_Bot_Man.setCharacterSize(24);
    Victory_Text_Bot_Man.setFillColor(sf::Color::White);


    sf::Event event;
    sf::Event Tvent;
    std::string s;
    int selX = 0;
    int selY = 0;
    int check2 = 0;
    int selX_check = 0;
    int selY_check = 0;
    int victory = 4;//0-если выиграл человек,1-если выиграл бот
    int* corSel = (int*)(calloc(2, sizeof(int)));
    corSel[0] = selX;//координаты выбранной ячейки
    corSel[1] = selY;
    int trigCel = 0;//триггер срабатывает если энтер нажать
    int nowPl = 0;//какой игрок играет:0-человек 1-бот, начинает человек
    int Man_points = 0;//очки человека
    int Bot_points = 0;//очки бота
    int status = 0;//переключение между режимом вставляется буква и выбираем слово
    int word_len = 0;//длина слова
    int temp[3];
    FILE* fp = fopen("очки.txt", "r+");
    for (int i = 0; i < 3; i++) {
        fscanf(fp, "%d\n", &temp[i]);
    }

    fclose(fp);
    Man_points = temp[0];
    Bot_points = temp[1];
    nowPl = temp[2];
    status = 3;
    if (nowPl == 1) {
        victory = 0;
    }
    else if (nowPl == 0) {
        victory = 1;
    }
    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//массив в котором хранятся данные о том что мы выбрали букву для слова 
        xyChosen[word_len][0] = -1;
        xyChosen[word_len][1] = -1;
    }
    *back = 0;
    while (window.isOpen() && (*back != 1)) {
        while (window.pollEvent(event) && (*back != 1)) {//проверяет происходит ли событие
            if (event.key.code == sf::Keyboard::Tab)
            {
                *back = 1;
                ofstream myfile;
                myfile.open("save.txt");
                for (int x = 0; x < 5; x++)
                {
                    for (int y = 0; y < 5; y++)
                    {
                        fieldChar[x][y] = toupper(fieldChar[x][y]);
                        myfile << fieldChar[x][y];
                    }
                    myfile << endl;
                }
                menu(window, back);
            }
            else {
                if (nowPl == 0 || status == 3) {
                    if (status == 0) {
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 0) {
                            corSel = cellChoice(event, selX, selY);//отслеживает какая конкретно клавиша была нажата (клавиши управления)
                            if (corSel[0] + corSel[1] >= 0) {//если выбираем клетку Ю0
                                selX = corSel[0];
                                selY = corSel[1];
                            }
                            else if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;
                                corSel[1] = selY;
                                if (FieldValue[selY][selX] != " ") {//если клетка пустая
                                    trigCel = 0;//клетку нельзя выбрать
                                }
                                else {
                                    trigCel = 1;//то клетка зеленая
                                }
                            }

                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если клетка зеленая , ввели букву и нажали энтер
                            corSel = cellChoice(event, selX, selY);//координаты выбранной клетки в данный момент , чтобы отследить энтер где находится, проверяем что нажали именно энтер
                            if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;//берем предыдущие координаты
                                corSel[1] = selY;
                                selX_check = selX;
                                selY_check = selY;
                                //fieldChar[selY][selX] = (wchar_t)((int)inputText[0]-1104);
                                //printf("%d\n", inputText[0]);
                                FieldValue[selY][selX] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                                fieldChar[selY][selX] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                                if (checkLetter(fieldChar[selY][selX]) == 0) {//если буква нерусская
                                    FieldValue[selY][selX] = " ";//стираем букву, ставим пробел
                                    fieldChar[selY][selX] = ' ';
                                }
                                else {
                                    status = 1;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                                }
                                //printf("%d", fieldChar[selY][selX]);
                                //fieldChar[selY][selX][0] = (wchar_t)inputText[0];
                                /*if (isalpha(inputText[0])) {
                                    fieldChar[selY][selX][0] = (char)inputText[0];
                                }*/
                                trigCel = 0;//два раза нажали энтер(считай не нажали)

                            }
                            else if (corSel[0] + corSel[1] == -4) {//если эскейп
                                corSel[0] = selX;//предыдущие координаты возвращаем
                                corSel[1] = selY;
                                FieldValue[selY][selX] = " ";
                                fieldChar[selY][selX] = ' ';
                                trigCel = 0;
                            }
                            //printf("%d %d\n", selX, selY);
                        }
                        else if (trigCel == 1) {//если клетка зеленая
                            if (event.type == sf::Event::TextEntered) {
                                if (FieldValue[selY][selX] != " ") {
                                    trigCel = 0;
                                    continue;
                                }
                                inputText = ((wchar_t)(event.text.unicode));//считываем текст
                                if (inputText[0] >= 1072 && inputText[0] <= 1103) {//проверяет русскую букву
                                    inputText[0] -= 32;
                                }
                            }
                        }
                    }
                    else if (status == 1) {//выбираем слово
                        //int word_len = 0;
                        if (event.type == sf::Event::Closed) {
                            window.close();
                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 0) {//энтер еще не нажали
                            corSel = cellChoice(event, selX, selY);//считываем координаты, выибраем клетку с которой слово будет начинаться
                            if (corSel[0] + corSel[1] >= 0) {//если не энтер, есть координаты
                                selX = corSel[0];//
                                selY = corSel[1];//
                            }
                            else if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;
                                corSel[1] = selY;
                                if (FieldValue[selY][selX] != " ") {//если в выбранной ячейке  есть какая-то буква
                                    trigCel = 1;//мы наэали энтер
                                    xyChosen[word_len][0] = selY;//координаты выделенной буквы
                                    xyChosen[word_len][1] = selX;
                                    //wordsMan[man_cnt][word_len] = fieldChar[selY][selX];
                                    word_len++;//длина слова плюс один
                                }
                                else {
                                    trigCel = 0;//если нет буквы то энтер нажмем второй раз
                                }
                            }
                            //printf("%d %d\n", selX, selY);
                        }
                        else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если мы один раз нажали энтер

                            corSel = cellChoice(event, selX, selY);
                            if (corSel[0] + corSel[1] >= 0) {//если перемещаю стрелкой
                                selX = corSel[0];
                                selY = corSel[1];
                                int a = inArray(xyChosen, word_len, selY, selX);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить
                                if (a == 0) {//выбранной ячейки нет в массиве
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    //wordsMan[man_cnt][0] = fieldChar[selY][selX];
                                    word_len++;
                                }
                                else {//если второй раз выделили одну и ту же ячейку
                                    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                        xyChosen[i][0] = -1;
                                        xyChosen[i][1] = -1;
                                    }
                                    word_len = 0;//слова нет
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    word_len++;//теперь есть первая буква
                                }
                            }
                            if (corSel[0] + corSel[1] == -2) {//если энтер
                                corSel[0] = selX;//возвращаем в массив координаты ячейки последней
                                corSel[1] = selY;//
                                for (int i = 0; i < word_len; i++) {
                                    wordsMan_current[i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                                }
                                for (int i = 0; i < word_len; i++) {
                                    int k = 0;
                                    for (int j = 0; j < 32; j++) {
                                        if (wordsMan_current[i] == alphabet[j]) {
                                            k++;
                                            break;
                                        }
                                    }
                                    if (k == 0) {
                                        wordsMan_current[i] = wordsMan_current[i] + 32;
                                    }
                                    k = 0;
                                }
                                check_dictionary(wordsMan_current, &word_len, &check);
                                int a = inArray(xyChosen, word_len, selY_check, selX_check);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить

                                if (a == 0) {
                                    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                        xyChosen[i][0] = -1;
                                        xyChosen[i][1] = -1;
                                    }
                                    word_len = 0;//слова нет
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    word_len++;//теперь есть первая буква
                                    check = 0;
                                }
                                printf("\nпроверка ");
                                printf("%d\n", check);
                                //FieldValue[selY_check][selX_check] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                                //fieldChar[selY_check][selX_check] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                                if (check == 0) {//если слова нет в словаре
                                    FieldValue[selY_check][selX_check] = " ";//стираем букву, ставим пробел
                                    fieldChar[selY_check][selX_check] = ' ';
                                    trigCel = 0;
                                    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                        xyChosen[i][0] = -1;
                                        xyChosen[i][1] = -1;
                                    }
                                    xyChosen[word_len][0] = selY;
                                    xyChosen[word_len][1] = selX;
                                    word_len = 0;
                                    status = 0;
                                    //   check2 = 1;
                                }
                                else {
                                    for (int i = 0; i < word_len; i++) {//довляем в массив слов сами буквы
                                        wordsMan[man_cnt][i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                                    }
                                    status = 0;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                                    check = 0;
                                    for (int i = 0; i < word_len; i++) {//в консоль выводятся слова
                                        if (i == 0 && nowPl == 0) {
                                            printf("Слово человека: ");
                                        }
                                        else if (i == 0 && nowPl == 1) {
                                            printf("Слово Бота: ");
                                        }
                                        printf("%c", wordsMan[man_cnt][i]);
                                    }
                                    printf("\n");
                                    if (nowPl == 0) {//если сейчас сходил человек, начисляем очки ему
                                        Man_points = Man_points + word_len;
                                        s = std::to_string(Man_points);
                                        printf("Сходил человек\nОчки человека: ");
                                        printf("%d\n\n", Man_points);
                                    }
                                    else {//если сейчас сходил бот, начисляем очки ему
                                        Bot_points = Bot_points + word_len;
                                        printf("Сходил Бот\nОчки Бота: ");
                                        printf("%d\n\n", Bot_points);
                                    }
                                    word_len = 0;
                                    man_cnt++;//слова увеличиваются
                                    if (nowPl == 0) {//если ходил человек, то теперь ход игрока
                                        nowPl = 1;
                                    }
                                    else {
                                        nowPl = 0;//если ходил игрок, то теперь ходит человек
                                    }
                                    int cnt = 0;
                                    for (int i = 0; i < FIELD_SIZE; i++) {
                                        for (int j = 0; j < FIELD_SIZE; j++) {
                                            if (FieldValue[i][j] != " ") {
                                                cnt++;
                                                if (cnt == 25) {
                                                    status = 3; //больше нет места на поле
                                                }
                                            }
                                        }
                                    }
                                    cnt = 0;
                                }
                            }
                            else if (corSel[0] + corSel[1] == -4) {//экейп
                                corSel[0] = selX;
                                corSel[1] = selY;
                                trigCel = 0;
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                word_len = 0;

                            }

                        }
                    }
                    else if (status == 3) {
                        if (victory == 1) {
                            //victory = 0;
                            printf("Поздравляем, Вы одержали победу!");
                        }
                        else if (victory == 0) {
                            // victory = 1;
                            printf("Вот одержал победу");
                        }
                        else {
                            //victory = 2;
                            printf("Игра окончена с ничейным счетом");
                        }

                    }
                }
                else {
                    for (int i = 0; i < FIELD_SIZE; i++) {
                        for (int j = 0; j < FIELD_SIZE; j++) {
                            fieldChar[i][j] = tolower(fieldChar[i][j]);
                        }
                    }
                    int len = 0;

                    strcpy(wordsBot_current, bot_turn(fieldChar, wordsBot, wordsMan));
                    if (wordsBot_current[0] != '0') {
                        len = strlen(wordsBot_current);
                        strcpy(wordsBot[bot_cnt], wordsBot_current);
                        bot_cnt++;
                    }
                    for (int i = 0; i < FIELD_SIZE; i++) {
                        for (int j = 0; j < FIELD_SIZE; j++) {
                            //char prom[2];
                            //prom[0] = fieldChar[i][j];
                            if (FieldValue[i][j] == " " && fieldChar[i][j] != ' ') {
                                fieldChar[i][j] = toupper(fieldChar[i][j]);
                                printf("%d %c\n", (wchar_t)fieldChar[i][j], fieldChar[i][j]);
                                FieldValue[i][j] = (wchar_t)((wchar_t)fieldChar[i][j] - 64432);
                            }
                        }
                    }
                    Bot_points = Bot_points + len;
                    nowPl = 0;
                    trigCel = 0;
                    int cnt = 0;
                    for (int i = 0; i < FIELD_SIZE; i++) {
                        for (int j = 0; j < FIELD_SIZE; j++) {
                            if (FieldValue[i][j] != " ") {
                                cnt++;
                                if (cnt == 25) {
                                    status = 3; //больше нет места на поле
                                }
                            }
                        }
                    }
                    cnt = 0;
                }
            }
        }

        window.clear(sf::Color::Black);
        if (status == 0) {//энтер не нажат букву не ввели
            for (int i = 0; i < FIELD_SIZE; ++i) {//отрисовываем ячейки
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    //int a = inArray(xyChosen, word_len, i, j);
                    if (i == selY && j == selX && trigCel == 0) {//выбираю ячейку белая
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажала энтер зеленая
                        field[i][j].shape.setFillColor(sf::Color::Green);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(inputText);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//ячейка черная если не соответсвует ничему
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);//отрисовка фотрма
                    //if()
                    //field[i][j].text.setString(field[i][j].content + inputText);
                    window.draw(field[i][j].text);//отрисовка текст
                }
            }
        }
        else if (status == 1) {//если ввела букву и выбираю слово
            for (int i = 0; i < FIELD_SIZE; ++i) {
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    int a = inArray(xyChosen, word_len, i, j);//проверяю ячейку я выбрала или нет
                    if (i == selY && j == selX && trigCel == 0) {//если ячейка выбрана как первая буква слова
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажали энтер находимся в этой ячейке сейчас
                        field[i][j].shape.setFillColor(sf::Color::Magenta);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (a == 1 && trigCel == 1) {//ячейка которую когда-то выбрали
                        field[i][j].shape.setFillColor(sf::Color::Red);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(FieldValue[i][j]);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//просто черные ячейки
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);
                    window.draw(field[i][j].text);
                }
            }
        }
        button[0].shape.setFillColor(sf::Color::Black);
        button[0].text.setFillColor(sf::Color::White);

        if (victory == 4) {
            std::ostringstream player1ScoreString;    // объявили переменную
            player1ScoreString << Man_points;		//занесли в нее число очков, то есть формируем строку
            player1Text.setString("Player:" + player1ScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player1Text.setPosition(20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player1Text);//рисую этот текст

            std::ostringstream BotScoreString;    // объявили переменную
            BotScoreString << Bot_points;		//занесли в нее число очков, то есть формируем строку
            player2Text.setString("Bot:" + BotScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player2Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player2Text);//рисую этот текст
        }

        if (victory == 0) {
            Victory_Text_Man.setString("congratulations, you have won!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Man.getLocalBounds();
            window.draw(Victory_Text_Man);//рисую этот текст
        }
        else if (victory == 1) {
            Victory_Text_Bot.setString("the bot won");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot.getLocalBounds();
            window.draw(Victory_Text_Bot);//рисую этот текст
        }
        else if (victory == 2) {
            Victory_Text_Bot_Man.setString("draw!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot_Man.getLocalBounds();
            window.draw(Victory_Text_Bot_Man);//рисую этот текст
        }

        window.draw(button[0].shape);
        window.draw(button[0].text);
        window.display();
    }
}

void pve_end(sf::RenderWindow& window, int* back) {
    char wordsMan[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE]; //слова и буквы(сначала буквы)
    int man_cnt = 0;
    char wordsBot[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE];
    int bot_cnt = 0;
    int xyChosen[FIELD_SIZE * FIELD_SIZE][2];
    char fieldChar[FIELD_SIZE][FIELD_SIZE];
    char wordsMan_current[FIELD_SIZE * FIELD_SIZE];
    char wordsBot_current[FIELD_SIZE * FIELD_SIZE];
    int check = 0;
    char rand_word[5];
    //random_word(rand_word);
    sf::String inputText;
    sf::String inputText_1;
    sf::String FieldValue[FIELD_SIZE][FIELD_SIZE];

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            fieldChar[i][j] = ' ';
        }
    }

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            FieldValue[i][j] = " ";
        }
    }
    char fieldChar_1[5][5];
    /* char alphabet[] = { "абвгдежзийклмнопрстуфхцчшщъыьэюя" };
     for (int i = 0; i < 5; i++) {
         for (int j = 0; j < 33; j++) {
             if (rand_word[i] == alphabet[j]) {
                 inputText_1[0] = 1072 + j - 32;
                 FieldValue[2][i] = inputText_1[0];
                 fieldChar[2][i] = (FieldValue[2][i]).getData()[0] - 80;
             }
         }
     }*/
    FILE* f;
    f = fopen("save.txt", "r");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            fscanf(f, "%c", &fieldChar_1[i][j]);
            if (fieldChar_1[i][j] != '\n') {
                fieldChar[i][j] = fieldChar_1[i][j];
                char alphabet[] = { "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ" };
                for (int n = 0; n < 33; n++) {
                    if (fieldChar[i][j] == alphabet[n]) {
                        inputText_1[0] = 1072 + n - 32;
                        FieldValue[i][j] = inputText_1[0];
                        fieldChar[i][j] = (FieldValue[i][j]).getData()[0] - 80;
                        break;
                    }
                    if ((n == 32) && fieldChar[i][j] == ' ') {
                        inputText_1[0] = 160;
                        FieldValue[i][j] = " ";
                        break;
                    }
                    /*if (fieldChar[i][j] == '\n') {
                        inputText_1[0] = 10;
                        FieldValue[i][j] = inputText_1[0];
                    }*/
                }
            }
        }
    }
    //   fclose(f);
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return;
    }
    std::vector<std::vector<Cell>> field(FIELD_SIZE, std::vector<Cell>(FIELD_SIZE)); //создаем поле
    for (int i = 0; i < FIELD_SIZE; ++i) {
        for (int j = 0; j < FIELD_SIZE; ++j) {
            int x = (WINDOW_WIDTH - (FIELD_SIZE * CELL_SIZE)) / 2 + j * CELL_SIZE;
            int y = (WINDOW_HEIGHT - (FIELD_SIZE * CELL_SIZE)) / 2 + i * CELL_SIZE;
            createCell(field[i][j], font, x, y); //создает поле 5*5
        }
    }

    Button button[1];
    const sf::String buttonLabels[] = { L"Назад" };
    int x = (WINDOW_WIDTH - BUTTON_WIDTH) - 600;
    int y = (BUTTON_HEIGHT + MENU_PADDING) * 5 + MENU_PADDING;
    createButton(button[0], buttonLabels[0], font, x, y);

    sf::Text player1Text;
    player1Text.setFont(font);
    //player1Text.setString("Player: 0");
    player1Text.setCharacterSize(24);
    player1Text.setFillColor(sf::Color::White);
    //sf::FloatRect player1Bounds = player1Text.getLocalBounds();
    //player1Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2+100);

    sf::Text player2Text;
    player2Text.setFont(font);
    player2Text.setCharacterSize(24);
    player2Text.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Man;
    Victory_Text_Man.setFont(font);
    Victory_Text_Man.setCharacterSize(24);
    Victory_Text_Man.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot;
    Victory_Text_Bot.setFont(font);
    Victory_Text_Bot.setCharacterSize(24);
    Victory_Text_Bot.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot_Man;
    Victory_Text_Bot_Man.setFont(font);
    Victory_Text_Bot_Man.setCharacterSize(24);
    Victory_Text_Bot_Man.setFillColor(sf::Color::White);


    sf::Event event;
    sf::Event Tvent;
    std::string s;
    int selX = 0;
    int selY = 0;
    int check2 = 0;
    int selX_check = 0;
    int selY_check = 0;
    int victory = 4;//0-если выиграл человек,1-если выиграл бот
    int* corSel = (int*)(calloc(2, sizeof(int)));
    corSel[0] = selX;//координаты выбранной ячейки
    corSel[1] = selY;
    int trigCel = 0;//триггер срабатывает если энтер нажать
    int nowPl = 0;//какой игрок играет:0-человек 1-бот, начинает человек
    int Man_points = 0;//очки человека
    int Bot_points = 0;//очки бота
    int status = 0;//переключение между режимом вставляется буква и выбираем слово
    int word_len = 0;//длина слова
    int temp[3];
    FILE* fp = fopen("очки.txt", "r+");
    for (int i = 0; i < 3; i++) {
        fscanf(fp, "%d\n", &temp[i]);
    }

    fclose(fp);
    Man_points = temp[0];
    Bot_points = temp[1];
    nowPl = temp[2];
    status = 3;
    if (nowPl == 1) {
        victory = 0;
    }
    else if (nowPl == 0) {
        victory = 1;
    }
    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//массив в котором хранятся данные о том что мы выбрали букву для слова 
        xyChosen[word_len][0] = -1;
        xyChosen[word_len][1] = -1;
    }
    *back = 0;
    while (window.isOpen() && (*back != 1)) {
        while (window.pollEvent(event) && (*back != 1)) {//проверяет происходит ли событие
            if (event.key.code == sf::Keyboard::Tab)
            {
                *back = 1;
                ofstream myfile;
                myfile.open("save.txt");
                for (int x = 0; x < 5; x++)
                {
                    for (int y = 0; y < 5; y++)
                    {
                        myfile << fieldChar[x][y];
                    }
                    myfile << endl;
                }
                menu(window, back);
            }
            else {

                if (status == 0) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 0) {
                        corSel = cellChoice(event, selX, selY);//отслеживает какая конкретно клавиша была нажата (клавиши управления)
                        if (corSel[0] + corSel[1] >= 0) {//если выбираем клетку Ю0
                            selX = corSel[0];
                            selY = corSel[1];
                        }
                        else if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;
                            corSel[1] = selY;
                            if (FieldValue[selY][selX] != " ") {//если клетка пустая
                                trigCel = 0;//клетку нельзя выбрать
                            }
                            else {
                                trigCel = 1;//то клетка зеленая
                            }
                        }

                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если клетка зеленая , ввели букву и нажали энтер
                        corSel = cellChoice(event, selX, selY);//координаты выбранной клетки в данный момент , чтобы отследить энтер где находится, проверяем что нажали именно энтер
                        if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;//берем предыдущие координаты
                            corSel[1] = selY;
                            selX_check = selX;
                            selY_check = selY;
                            //fieldChar[selY][selX] = (wchar_t)((int)inputText[0]-1104);
                            //printf("%d\n", inputText[0]);
                            FieldValue[selY][selX] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                            fieldChar[selY][selX] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                            if (checkLetter(fieldChar[selY][selX]) == 0) {//если буква нерусская
                                FieldValue[selY][selX] = " ";//стираем букву, ставим пробел
                                fieldChar[selY][selX] = ' ';
                            }
                            else {
                                status = 1;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                            }
                            //printf("%d", fieldChar[selY][selX]);
                            //fieldChar[selY][selX][0] = (wchar_t)inputText[0];
                            /*if (isalpha(inputText[0])) {
                                fieldChar[selY][selX][0] = (char)inputText[0];
                            }*/
                            trigCel = 0;//два раза нажали энтер(считай не нажали)

                        }
                        else if (corSel[0] + corSel[1] == -4) {//если эскейп
                            corSel[0] = selX;//предыдущие координаты возвращаем
                            corSel[1] = selY;
                            FieldValue[selY][selX] = " ";
                            fieldChar[selY][selX] = ' ';
                            trigCel = 0;
                        }
                        //printf("%d %d\n", selX, selY);
                    }
                    else if (trigCel == 1) {//если клетка зеленая
                        if (event.type == sf::Event::TextEntered) {
                            if (FieldValue[selY][selX] != " ") {
                                trigCel = 0;
                                continue;
                            }
                            inputText = ((wchar_t)(event.text.unicode));//считываем текст
                            if (inputText[0] >= 1072 && inputText[0] <= 1103) {//проверяет русскую букву
                                inputText[0] -= 32;
                            }
                        }
                    }
                }
                else if (status == 1) {//выбираем слово
                    //int word_len = 0;
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 0) {//энтер еще не нажали
                        corSel = cellChoice(event, selX, selY);//считываем координаты, выибраем клетку с которой слово будет начинаться
                        if (corSel[0] + corSel[1] >= 0) {//если не энтер, есть координаты
                            selX = corSel[0];//
                            selY = corSel[1];//
                        }
                        else if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;
                            corSel[1] = selY;
                            if (FieldValue[selY][selX] != " ") {//если в выбранной ячейке  есть какая-то буква
                                trigCel = 1;//мы наэали энтер
                                xyChosen[word_len][0] = selY;//координаты выделенной буквы
                                xyChosen[word_len][1] = selX;
                                //wordsMan[man_cnt][word_len] = fieldChar[selY][selX];
                                word_len++;//длина слова плюс один
                            }
                            else {
                                trigCel = 0;//если нет буквы то энтер нажмем второй раз
                            }
                        }
                        //printf("%d %d\n", selX, selY);
                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если мы один раз нажали энтер

                        corSel = cellChoice(event, selX, selY);
                        if (corSel[0] + corSel[1] >= 0) {//если перемещаю стрелкой
                            selX = corSel[0];
                            selY = corSel[1];
                            int a = inArray(xyChosen, word_len, selY, selX);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить
                            if (a == 0) {//выбранной ячейки нет в массиве
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                //wordsMan[man_cnt][0] = fieldChar[selY][selX];
                                word_len++;
                            }
                            else {//если второй раз выделили одну и ту же ячейку
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                word_len = 0;//слова нет
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                word_len++;//теперь есть первая буква
                            }
                        }
                        if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;//возвращаем в массив координаты ячейки последней
                            corSel[1] = selY;//
                            for (int i = 0; i < word_len; i++) {
                                wordsMan_current[i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                            }
                            for (int i = 0; i < word_len; i++) {
                                wordsMan_current[i] = wordsMan_current[i] + 32;
                            }
                            check_dictionary(wordsMan_current, &word_len, &check);
                            int a = inArray(xyChosen, word_len, selY_check, selX_check);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить

                            if (a == 0) {
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                word_len = 0;//слова нет
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                word_len++;//теперь есть первая буква
                                check = 0;
                            }
                            printf("\nпроверка ");
                            printf("%d\n", check);
                            //FieldValue[selY_check][selX_check] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                            //fieldChar[selY_check][selX_check] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                            if (check == 0) {//если слова нет в словаре
                                FieldValue[selY_check][selX_check] = " ";//стираем букву, ставим пробел
                                fieldChar[selY_check][selX_check] = ' ';
                                trigCel = 0;
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                word_len = 0;
                                status = 0;
                                //   check2 = 1;
                            }
                            else {
                                for (int i = 0; i < word_len; i++) {//довляем в массив слов сами буквы
                                    wordsMan[man_cnt][i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                                }
                                status = 0;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                                check = 0;
                                for (int i = 0; i < word_len; i++) {//в консоль выводятся слова
                                    if (i == 0 && nowPl == 0) {
                                        printf("Слово человека: ");
                                    }
                                    else if (i == 0 && nowPl == 1) {
                                        printf("Слово Бота: ");
                                    }
                                    printf("%c", wordsMan[man_cnt][i]);
                                }
                                printf("\n");
                                if (nowPl == 0) {//если сейчас сходил человек, начисляем очки ему
                                    Man_points = Man_points + word_len;
                                    s = std::to_string(Man_points);
                                    printf("Сходил человек\nОчки человека: ");
                                    printf("%d\n\n", Man_points);
                                }
                                else {//если сейчас сходил бот, начисляем очки ему
                                    Bot_points = Bot_points + word_len;
                                    printf("Сходил Бот\nОчки Бота: ");
                                    printf("%d\n\n", Bot_points);
                                }
                                word_len = 0;
                                man_cnt++;//слова увеличиваются
                                if (nowPl == 0) {//если ходил человек, то теперь ход игрока
                                    nowPl = 1;
                                }
                                else {
                                    nowPl = 0;//если ходил игрок, то теперь ходит человек
                                }
                                int cnt = 0;
                                for (int i = 0; i < FIELD_SIZE; i++) {
                                    for (int j = 0; j < FIELD_SIZE; j++) {
                                        if (FieldValue[i][j] != " ") {
                                            cnt++;
                                            if (cnt == 25) {
                                                status = 3; //больше нет места на поле
                                            }
                                        }
                                    }
                                }
                                cnt = 0;
                            }
                        }
                        else if (corSel[0] + corSel[1] == -4) {//экейп
                            corSel[0] = selX;
                            corSel[1] = selY;
                            trigCel = 0;
                            for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                xyChosen[i][0] = -1;
                                xyChosen[i][1] = -1;
                            }
                            word_len = 0;

                        }

                    }
                }
                else if (status == 3) {
                    if (victory == 1) {
                        //victory = 0;
                        printf("Поздравляем, Вы одержали победу!");
                    }
                    else if (victory == 0) {
                        // victory = 1;
                        printf("Вот одержал победу");
                    }
                    else {
                        //victory = 2;
                        printf("Игра окончена с ничейным счетом");
                    }

                }
            }
        }

        window.clear(sf::Color::Black);
        if (status == 0) {//энтер не нажат букву не ввели
            for (int i = 0; i < FIELD_SIZE; ++i) {//отрисовываем ячейки
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    //int a = inArray(xyChosen, word_len, i, j);
                    if (i == selY && j == selX && trigCel == 0) {//выбираю ячейку белая
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажала энтер зеленая
                        field[i][j].shape.setFillColor(sf::Color::Green);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(inputText);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//ячейка черная если не соответсвует ничему
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);//отрисовка фотрма
                    //if()
                    //field[i][j].text.setString(field[i][j].content + inputText);
                    window.draw(field[i][j].text);//отрисовка текст
                }
            }
        }
        else if (status == 1) {//если ввела букву и выбираю слово
            for (int i = 0; i < FIELD_SIZE; ++i) {
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    int a = inArray(xyChosen, word_len, i, j);//проверяю ячейку я выбрала или нет
                    if (i == selY && j == selX && trigCel == 0) {//если ячейка выбрана как первая буква слова
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажали энтер находимся в этой ячейке сейчас
                        field[i][j].shape.setFillColor(sf::Color::Magenta);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (a == 1 && trigCel == 1) {//ячейка которую когда-то выбрали
                        field[i][j].shape.setFillColor(sf::Color::Red);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(FieldValue[i][j]);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//просто черные ячейки
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);
                    window.draw(field[i][j].text);
                }
            }
        }
        button[0].shape.setFillColor(sf::Color::Black);
        button[0].text.setFillColor(sf::Color::White);

        if (victory == 4) {
            std::ostringstream player1ScoreString;    // объявили переменную
            player1ScoreString << Man_points;		//занесли в нее число очков, то есть формируем строку
            player1Text.setString("Player:" + player1ScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player1Text.setPosition(20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player1Text);//рисую этот текст

            std::ostringstream BotScoreString;    // объявили переменную
            BotScoreString << Bot_points;		//занесли в нее число очков, то есть формируем строку
            player2Text.setString("Bot:" + BotScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player2Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player2Text);//рисую этот текст
        }

        if (victory == 0) {
            Victory_Text_Man.setString("congratulations, player 1 won!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Man.getLocalBounds();
            window.draw(Victory_Text_Man);//рисую этот текст
        }
        else if (victory == 1) {
            Victory_Text_Bot.setString("congratulations, player 2 won!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot.getLocalBounds();
            window.draw(Victory_Text_Bot);//рисую этот текст
        }
        else if (victory == 2) {
            Victory_Text_Bot_Man.setString("draw!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot_Man.getLocalBounds();
            window.draw(Victory_Text_Bot_Man);//рисую этот текст
        }

        window.draw(button[0].shape);
        window.draw(button[0].text);
        window.display();
    }
}

void pve_old(sf::RenderWindow& window, int* back) {
    char wordsMan[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE]; //слова и буквы(сначала буквы)
    int man_cnt = 0;
    char wordsBot[FIELD_SIZE * FIELD_SIZE][FIELD_SIZE * FIELD_SIZE];
    int bot_cnt = 0;
    int xyChosen[FIELD_SIZE * FIELD_SIZE][2];
    char fieldChar[FIELD_SIZE][FIELD_SIZE];
    char wordsMan_current[FIELD_SIZE * FIELD_SIZE];
    char wordsBot_current[FIELD_SIZE * FIELD_SIZE];
    int check = 0;
    char rand_word[5];
    //random_word(rand_word);
    sf::String inputText;
    sf::String inputText_1;
    sf::String FieldValue[FIELD_SIZE][FIELD_SIZE];

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            fieldChar[i][j] = ' ';
        }
    }

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            FieldValue[i][j] = " ";
        }
    }
    char fieldChar_1[5][5];
    /* char alphabet[] = { "абвгдежзийклмнопрстуфхцчшщъыьэюя" };
     for (int i = 0; i < 5; i++) {
         for (int j = 0; j < 33; j++) {
             if (rand_word[i] == alphabet[j]) {
                 inputText_1[0] = 1072 + j - 32;
                 FieldValue[2][i] = inputText_1[0];
                 fieldChar[2][i] = (FieldValue[2][i]).getData()[0] - 80;
             }
         }
     }*/
    FILE* f;
    f = fopen("save.txt", "r");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            fscanf(f, "%c", &fieldChar_1[i][j]);
            if (fieldChar_1[i][j] != '\n') {
                fieldChar[i][j] = fieldChar_1[i][j];
                char alphabet[] = { "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ" };
                for (int n = 0; n < 33; n++) {
                    if (fieldChar[i][j] == alphabet[n]) {
                        inputText_1[0] = 1072 + n - 32;
                        FieldValue[i][j] = inputText_1[0];
                        fieldChar[i][j] = (FieldValue[i][j]).getData()[0] - 80;
                        break;
                    }
                    if ((n == 32) && fieldChar[i][j] == ' ') {
                        inputText_1[0] = 160;
                        FieldValue[i][j] = " ";
                        break;
                    }
                    /*if (fieldChar[i][j] == '\n') {
                        inputText_1[0] = 10;
                        FieldValue[i][j] = inputText_1[0];
                    }*/
                }
            }
        }
    }
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return;
    }
    std::vector<std::vector<Cell>> field(FIELD_SIZE, std::vector<Cell>(FIELD_SIZE)); //создаем поле
    for (int i = 0; i < FIELD_SIZE; ++i) {
        for (int j = 0; j < FIELD_SIZE; ++j) {
            int x = (WINDOW_WIDTH - (FIELD_SIZE * CELL_SIZE)) / 2 + j * CELL_SIZE;
            int y = (WINDOW_HEIGHT - (FIELD_SIZE * CELL_SIZE)) / 2 + i * CELL_SIZE;
            createCell(field[i][j], font, x, y); //создает поле 5*5
        }
    }

    Button button[1];
    const sf::String buttonLabels[] = { L"Назад\n  (Tab)" };
    int x = (WINDOW_WIDTH - BUTTON_WIDTH) - 600;
    int y = (BUTTON_HEIGHT + MENU_PADDING) * 5 + MENU_PADDING;
    createButton(button[0], buttonLabels[0], font, x, y);

    sf::Text player1Text;
    player1Text.setFont(font);
    //player1Text.setString("Player: 0");
    player1Text.setCharacterSize(24);
    player1Text.setFillColor(sf::Color::White);
    //sf::FloatRect player1Bounds = player1Text.getLocalBounds();
    //player1Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2+100);

    sf::Text player2Text;
    player2Text.setFont(font);
    player2Text.setCharacterSize(24);
    player2Text.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Man;
    Victory_Text_Man.setFont(font);
    Victory_Text_Man.setCharacterSize(24);
    Victory_Text_Man.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot;
    Victory_Text_Bot.setFont(font);
    Victory_Text_Bot.setCharacterSize(24);
    Victory_Text_Bot.setFillColor(sf::Color::White);

    sf::Text Victory_Text_Bot_Man;
    Victory_Text_Bot_Man.setFont(font);
    Victory_Text_Bot_Man.setCharacterSize(24);
    Victory_Text_Bot_Man.setFillColor(sf::Color::White);


    sf::Event event;
    sf::Event Tvent;
    std::string s;
    int selX = 0;
    int selY = 0;
    int check2 = 0;
    int selX_check = 0;
    int selY_check = 0;
    int victory = 4;//0-если выиграл человек,1-если выиграл бот
    int* corSel = (int*)(calloc(2, sizeof(int)));
    corSel[0] = selX;//координаты выбранной ячейки
    corSel[1] = selY;
    int trigCel = 0;//триггер срабатывает если энтер нажать
    int nowPl = 0;//какой игрок играет:0-человек 1-бот, начинает человек
    int Man_points = 0;//очки человека
    int Bot_points = 0;//очки бота
    int status = 0;//переключение между режимом вставляется буква и выбираем слово
    int word_len = 0;//длина слова
    int temp[3];
    FILE* fp = fopen("очки.txt", "r+");
    for (int i = 0; i < 3; i++) {
        fscanf(fp, "%d\n", &temp[i]);
    }
    Man_points = temp[0];
    Bot_points = temp[1];
    nowPl = temp[2];
    //fclose(fp);
    for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//массив в котором хранятся данные о том что мы выбрали букву для слова 
        xyChosen[word_len][0] = -1;
        xyChosen[word_len][1] = -1;
    }
    *back = 0;
    while (window.isOpen() && (*back != 1)) {
        while (window.pollEvent(event) && (*back != 1)) {//проверяет происходит ли событие
            if (event.key.code == sf::Keyboard::Tab)
            {
                *back = 1;
                ofstream myfile;
                myfile.open("save.txt");
                for (int x = 0; x < 5; x++)
                {
                    for (int y = 0; y < 5; y++)
                    {
                        myfile << fieldChar[x][y];
                    }
                    myfile << endl;
                }
                menu(window, back);
            }
            else {

                if (status == 0) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 0) {
                        corSel = cellChoice(event, selX, selY);//отслеживает какая конкретно клавиша была нажата (клавиши управления)
                        if (corSel[0] + corSel[1] >= 0) {//если выбираем клетку Ю0
                            selX = corSel[0];
                            selY = corSel[1];
                        }
                        else if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;
                            corSel[1] = selY;
                            if (FieldValue[selY][selX] != " ") {//если клетка пустая
                                trigCel = 0;//клетку нельзя выбрать
                            }
                            else {
                                trigCel = 1;//то клетка зеленая
                            }
                        }

                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если клетка зеленая , ввели букву и нажали энтер
                        corSel = cellChoice(event, selX, selY);//координаты выбранной клетки в данный момент , чтобы отследить энтер где находится, проверяем что нажали именно энтер
                        if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;//берем предыдущие координаты
                            corSel[1] = selY;
                            selX_check = selX;
                            selY_check = selY;
                            //fieldChar[selY][selX] = (wchar_t)((int)inputText[0]-1104);
                            //printf("%d\n", inputText[0]);
                            FieldValue[selY][selX] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                            fieldChar[selY][selX] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                            if (checkLetter(fieldChar[selY][selX]) == 0) {//если буква нерусская
                                FieldValue[selY][selX] = " ";//стираем букву, ставим пробел
                                fieldChar[selY][selX] = ' ';
                            }
                            else {
                                status = 1;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                            }
                            //printf("%d", fieldChar[selY][selX]);
                            //fieldChar[selY][selX][0] = (wchar_t)inputText[0];
                            /*if (isalpha(inputText[0])) {
                                fieldChar[selY][selX][0] = (char)inputText[0];
                            }*/
                            trigCel = 0;//два раза нажали энтер(считай не нажали)

                        }
                        else if (corSel[0] + corSel[1] == -4) {//если эскейп
                            corSel[0] = selX;//предыдущие координаты возвращаем
                            corSel[1] = selY;
                            FieldValue[selY][selX] = " ";
                            fieldChar[selY][selX] = ' ';
                            trigCel = 0;
                        }
                        //printf("%d %d\n", selX, selY);
                    }
                    else if (trigCel == 1) {//если клетка зеленая
                        if (event.type == sf::Event::TextEntered) {
                            if (FieldValue[selY][selX] != " ") {
                                trigCel = 0;
                                continue;
                            }
                            inputText = ((wchar_t)(event.text.unicode));//считываем текст
                            if (inputText[0] >= 1072 && inputText[0] <= 1103) {//проверяет русскую букву
                                inputText[0] -= 32;
                            }
                        }
                    }
                }
                else if (status == 1) {//выбираем слово
                    //int word_len = 0;
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 0) {//энтер еще не нажали
                        corSel = cellChoice(event, selX, selY);//считываем координаты, выибраем клетку с которой слово будет начинаться
                        if (corSel[0] + corSel[1] >= 0) {//если не энтер, есть координаты
                            selX = corSel[0];//
                            selY = corSel[1];//
                        }
                        else if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;
                            corSel[1] = selY;
                            if (FieldValue[selY][selX] != " ") {//если в выбранной ячейке  есть какая-то буква
                                trigCel = 1;//мы наэали энтер
                                xyChosen[word_len][0] = selY;//координаты выделенной буквы
                                xyChosen[word_len][1] = selX;
                                //wordsMan[man_cnt][word_len] = fieldChar[selY][selX];
                                word_len++;//длина слова плюс один
                            }
                            else {
                                trigCel = 0;//если нет буквы то энтер нажмем второй раз
                            }
                        }
                        //printf("%d %d\n", selX, selY);
                    }
                    else if (event.type == sf::Event::KeyPressed && trigCel == 1) {//если мы один раз нажали энтер

                        corSel = cellChoice(event, selX, selY);
                        if (corSel[0] + corSel[1] >= 0) {//если перемещаю стрелкой
                            selX = corSel[0];
                            selY = corSel[1];
                            int a = inArray(xyChosen, word_len, selY, selX);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить
                            if (a == 0) {//выбранной ячейки нет в массиве
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                //wordsMan[man_cnt][0] = fieldChar[selY][selX];
                                word_len++;
                            }
                            else {//если второй раз выделили одну и ту же ячейку
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                word_len = 0;//слова нет
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                word_len++;//теперь есть первая буква
                            }
                        }
                        if (corSel[0] + corSel[1] == -2) {//если энтер
                            corSel[0] = selX;//возвращаем в массив координаты ячейки последней
                            corSel[1] = selY;//
                            for (int i = 0; i < word_len; i++) {
                                wordsMan_current[i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                            }
                            for (int i = 0; i < word_len; i++) {
                                wordsMan_current[i] = wordsMan_current[i] + 32;
                            }
                            check_dictionary(wordsMan_current, &word_len, &check);
                            int a = inArray(xyChosen, word_len, selY_check, selX_check);//проверяет есть ли эта ячейка в массиве, чтобы повторно не выделить

                            if (a == 0) {
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {//то все обнуляется
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                word_len = 0;//слова нет
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                word_len++;//теперь есть первая буква
                                check = 0;
                            }
                            printf("\nпроверка ");
                            printf("%d\n", check);
                            //FieldValue[selY_check][selX_check] = inputText[0];//сэда записываем букву, которую ввели до этого в виде сфстринг
                            //fieldChar[selY_check][selX_check] = (FieldValue[selY][selX]).getData()[0] - 80;//здесь буква которую ввели массив чар
                            if (check == 0) {//если слова нет в словаре
                                FieldValue[selY_check][selX_check] = " ";//стираем букву, ставим пробел
                                fieldChar[selY_check][selX_check] = ' ';
                                trigCel = 0;
                                for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                    xyChosen[i][0] = -1;
                                    xyChosen[i][1] = -1;
                                }
                                xyChosen[word_len][0] = selY;
                                xyChosen[word_len][1] = selX;
                                word_len = 0;
                                status = 0;
                                //   check2 = 1;
                            }
                            else {
                                for (int i = 0; i < word_len; i++) {//довляем в массив слов сами буквы
                                    wordsMan[man_cnt][i] = fieldChar[xyChosen[i][0]][xyChosen[i][1]];
                                }
                                status = 0;//буква поставлена, переходим в режим выбора слова,розово-красный интерфейс
                                check = 0;
                                for (int i = 0; i < word_len; i++) {//в консоль выводятся слова
                                    if (i == 0 && nowPl == 0) {
                                        printf("Слово человека: ");
                                    }
                                    else if (i == 0 && nowPl == 1) {
                                        printf("Слово Бота: ");
                                    }
                                    printf("%c", wordsMan[man_cnt][i]);
                                }
                                printf("\n");
                                if (nowPl == 0) {//если сейчас сходил человек, начисляем очки ему
                                    Man_points = Man_points + word_len;
                                    s = std::to_string(Man_points);
                                    printf("Сходил человек\nОчки человека: ");
                                    printf("%d\n\n", Man_points);
                                }
                                else {//если сейчас сходил бот, начисляем очки ему
                                    Bot_points = Bot_points + word_len;
                                    printf("Сходил Бот\nОчки Бота: ");
                                    printf("%d\n\n", Bot_points);
                                }
                                word_len = 0;
                                man_cnt++;//слова увеличиваются
                                if (nowPl == 0) {//если ходил человек, то теперь ход игрока
                                    nowPl = 1;
                                }
                                else {
                                    nowPl = 0;//если ходил игрок, то теперь ходит человек
                                }
                                int cnt = 0;
                                for (int i = 0; i < FIELD_SIZE; i++) {
                                    for (int j = 0; j < FIELD_SIZE; j++) {
                                        if (FieldValue[i][j] != " ") {
                                            cnt++;
                                            if (cnt == 25) {
                                                status = 3; //больше нет места на поле
                                            }
                                        }
                                    }
                                }
                                cnt = 0;
                            }
                        }
                        else if (corSel[0] + corSel[1] == -4) {//экейп
                            corSel[0] = selX;
                            corSel[1] = selY;
                            trigCel = 0;
                            for (int i = 0; i < FIELD_SIZE * FIELD_SIZE; i++) {
                                xyChosen[i][0] = -1;
                                xyChosen[i][1] = -1;
                            }
                            word_len = 0;

                        }

                    }
                }
                else if (status == 3) {
                    if (Man_points > Bot_points) {
                        victory = 0;
                        printf("Поздравляем, Вы одержали победу!");
                    }
                    else if (Man_points < Bot_points) {
                        victory = 1;
                        printf("Вот одержал победу");
                    }
                    else {
                        victory = 2;
                        printf("Игра окончена с ничейным счетом");
                    }

                }
            }
        }

        window.clear(sf::Color::Black);
        if (status == 0) {//энтер не нажат букву не ввели
            for (int i = 0; i < FIELD_SIZE; ++i) {//отрисовываем ячейки
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    //int a = inArray(xyChosen, word_len, i, j);
                    if (i == selY && j == selX && trigCel == 0) {//выбираю ячейку белая
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажала энтер зеленая
                        field[i][j].shape.setFillColor(sf::Color::Green);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(inputText);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//ячейка черная если не соответсвует ничему
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);//отрисовка фотрма
                    //if()
                    //field[i][j].text.setString(field[i][j].content + inputText);
                    window.draw(field[i][j].text);//отрисовка текст
                }
            }
        }
        else if (status == 1) {//если ввела букву и выбираю слово
            for (int i = 0; i < FIELD_SIZE; ++i) {
                for (int j = 0; j < FIELD_SIZE; ++j) {
                    int a = inArray(xyChosen, word_len, i, j);//проверяю ячейку я выбрала или нет
                    if (i == selY && j == selX && trigCel == 0) {//если ячейка выбрана как первая буква слова
                        field[i][j].shape.setFillColor(sf::Color::White);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (i == selY && j == selX && trigCel == 1) {//нажали энтер находимся в этой ячейке сейчас
                        field[i][j].shape.setFillColor(sf::Color::Magenta);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].value = fieldChar[i][j][0];
                    }
                    else if (a == 1 && trigCel == 1) {//ячейка которую когда-то выбрали
                        field[i][j].shape.setFillColor(sf::Color::Red);
                        field[i][j].text.setFillColor(sf::Color::Black);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        field[i][j].text.setString(FieldValue[i][j]);
                    }
                    else {
                        field[i][j].shape.setFillColor(sf::Color::Black);//просто черные ячейки
                        field[i][j].text.setFillColor(sf::Color::White);
                        field[i][j].shape.setOutlineColor(sf::Color::White);
                        //field[i][j].content[0] = fieldChar[i][j][0];
                        field[i][j].text.setString(FieldValue[i][j]);
                        //field[i][j].text.setString(fieldChar[i][j][0]);
                    }
                    window.draw(field[i][j].shape);
                    window.draw(field[i][j].text);
                }
            }
        }
        button[0].shape.setFillColor(sf::Color::Black);
        button[0].text.setFillColor(sf::Color::White);

        if (victory == 4) {
            std::ostringstream player1ScoreString;    // объявили переменную
            player1ScoreString << Man_points;		//занесли в нее число очков, то есть формируем строку
            player1Text.setString("Player 1:" + player1ScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player1Text.setPosition(20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player1Text);//рисую этот текст

            std::ostringstream BotScoreString;    // объявили переменную
            BotScoreString << Bot_points;		//занесли в нее число очков, то есть формируем строку
            player2Text.setString("Player 2:" + BotScoreString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            player2Text.setPosition(WINDOW_WIDTH - PLAYER_INFO_WIDTH - 20, (WINDOW_HEIGHT - PLAYER_INFO_HEIGHT) / 2);
            window.draw(player2Text);//рисую этот текст
        }

        if (victory == 0) {
            Victory_Text_Man.setString("congratulations, player 1 won!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Man.getLocalBounds();
            window.draw(Victory_Text_Man);//рисую этот текст
        }
        else if (victory == 1) {
            Victory_Text_Bot.setString("congratulations, player 2 won!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot.getLocalBounds();
            window.draw(Victory_Text_Bot);//рисую этот текст
        }
        else if (victory == 2) {
            Victory_Text_Bot_Man.setString("draw!!!");//задаем строку тексту и вызываем сформированную выше строку методом .str() 
            Victory_Text_Bot_Man.setPosition(WINDOW_WIDTH / 3, (60));
            sf::FloatRect titleBounds = Victory_Text_Bot_Man.getLocalBounds();
            window.draw(Victory_Text_Bot_Man);//рисую этот текст
        }

        window.draw(button[0].shape);
        window.draw(button[0].text);
        window.display();
    }
}

int main() {
    setlocale(LC_ALL, "");
    SetConsoleCP(1251);
    int nowButton = 0;
    int back = 0;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Balda Game");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return EXIT_FAILURE;
    }

    Button buttons[4];
    const sf::String buttonLabels[] = { L"Играть PvP", L"Играть PvE", L"Сложность", L"Справка" };
    for (int i = 0; i < 4; ++i) {
        int x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
        int y = (BUTTON_HEIGHT + MENU_PADDING) * i + MENU_PADDING;
        createButton(buttons[i], buttonLabels[i], font, x, y);
    }


    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString(L"<БАЛДА>");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition((WINDOW_WIDTH - titleBounds.width) / 2, 50);

    int currentState = -1;
    sf::Vector2f mousePos;
    bool mouseClicked = false;
    int trig = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                int prevState = currentState;
                currentState = handleInput(event, currentState); //обработка ввода
                if (currentState == -1) {
                    currentState = prevState;
                    trig = 1;
                }
                else if (currentState == 6) {
                    //currentState = prevState;
                    trig = 2;
                }
                else if (currentState == 5) {
                    //currentState = prevState;
                    trig = 3;
                }
                else if (currentState == 4) {
                    trig = 4;
                }
            }
        }

        window.clear(sf::Color::Black);
        if (trig == 1) {
            if (currentState == 0) {
                status_game(window, &back);
                if (back == 1) {
                    back = 0;
                    main();
                }
            }
        }
        else if (trig == 2) {
            if (currentState == 6) {
                information(window, &back);
                if (back == 1) {
                    if (back == 1) {
                        back = 0;
                        main();
                    }
                }
            }
        }
        else if (trig == 3) {
            if (currentState == 5) {
                difficult(window, &back);
                if (back == 1) {
                    if (back == 1) {
                        back = 0;
                        main();
                    }
                }
            }
        }
        else if (trig == 4) {
            if (currentState == 4) {
                status_game_pve(window, &back);
                if (back == 1) {
                    back = 0;
                    main();
                }
            }
        }
        else {
            for (int i = 0; i < 4; ++i) {
                if (i == currentState) {
                    buttons[i].shape.setFillColor(sf::Color::Green);
                    buttons[i].text.setFillColor(sf::Color::White);
                }
                else {
                    buttons[i].shape.setFillColor(sf::Color::White);
                    buttons[i].text.setFillColor(sf::Color::Black);
                }
                window.draw(buttons[i].shape);
                window.draw(buttons[i].text);

            }
            window.draw(titleText);
        }

        window.display();
    }

    for (int i = 0; i < 4; ++i) {
        destroyButton(buttons[i]);
    }

    return EXIT_SUCCESS;
}


#include <cstring>
#include <string>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <thread>

class Console {
public:
    enum KEY_CODES {
        KEY_ESC = 0x1b,
        KEY_ENTER = 0xd
    };
    static int getKey();
    static void clearScreen();
    static void writeStrXY(int x, int y, const char s[]);
    static void cursorOff();
    static void cursorOn();
    static void writeCenteredStrXY(int y, const char* str);
};

class Coffee {
public:
    Coffee(const std::string& name, int power, const char* graphic)
            : name(name), power(power), graphic(graphic) {}

    virtual std::basic_string<char> prepare() = 0;

    std::string getName() const { return name; }
    int getPower() const { return power; }
    const char* getGraphic() const { return graphic; }

private:
    std::string name;
    int power;
    const char* graphic;
};

class Latte : public Coffee {
public:
    Latte()
            : Coffee("Latte", 40,
                     "    _______    \n"
                     "   |       |\\   \n"
                     "   |       | |  \n"
                     "   |       |/   \n"
                     "    \\_____/     \n") {}

    std::basic_string<char> prepare() override {
        std::cout << "Przygotowuje Latte...\n";
    }
};

class Espresso : public Coffee {
public:
    Espresso()
            : Coffee("Espresso", 80,
                     "               "
                     "    _____      \n"
                     "   |     |     \n"
                     "   |     |     \n"
                     "    \\___/      \n") {}

    std::basic_string<char> prepare() override {
        std::cout << "Przygotowuje Espresso...\n";
    }
};

class Cappuccino : public Coffee {
public:
    Cappuccino()
            : Coffee("Cappuccino", 50,
                     "    _______     \n"
                     "   |       |\\   \n"
                     "   |       | |  \n"
                     "   |       |/   \n"
                     "    \\_____/     \n") {}

    std::basic_string<char> prepare() override {
        std::cout << "Przygotowuje Cappuccino...\n";
    }
};

class BialaKawa : public Coffee {
public:
    BialaKawa()
            : Coffee("Biala kawa", 30,
                     "               "
                     "    |`````|     \n"
                     "    |     |     \n"
                     "    |     |     \n"
                     "     -----      \n") {}

    std::basic_string<char> prepare() override {
        std::cout << "Przygotowuje Biala kawe...\n";
    }
};

class Cup {
public:
    Cup(const char* graphic) : graphic(graphic) {}
    const char* getGraphic() const { return graphic; }

private:
    const char* graphic;
};

class CoffeeShop {
public:
    CoffeeShop() {
        cups.push_back(Cup("            \n    |`````|  \n    |     |    \n    |     |    \n     -----     \n"));
        cups.push_back(Cup("    ______     \n  |        |_.   \n  |        |  \\\n  |        |  / \n  |________|   \n"));
        cups.push_back(Cup("    _______     \n   |       |\\   \n   |       | |  \n   |       |/   \n    \\_____/     \n"));
        cups.push_back(Cup("             \n     _____      \n    |     |    \n    |     |    \n     \\___/     \n"));

        coffees.push_back(new Latte());
        coffees.push_back(new Espresso());
        coffees.push_back(new Cappuccino());
        coffees.push_back(new BialaKawa());
    }

    const Cup& getCup(int index) const {
        return cups.at(index);
    }

    Coffee* getCoffee(int index) const {
        return coffees.at(index);
    }

    ~CoffeeShop() {
        for (Coffee* coffee : coffees) {
            delete coffee;
        }
    }

private:
    std::vector<Cup> cups;
    std::vector<Coffee*> coffees;
};

class View {
public:
    enum COFFEE_MACHINE_PARAMS {
        ROWS = 19,
        COLS = 40
    };

    static constexpr int COFFEE_COUNT = 4;
    static void show(int selection);
    static void prepareCoffee(int selection);
    static void displayCoffeeMachine();
    static void displayEnergyBar();
    static void drawCup(int selection);

private:
    static constexpr const char* coffeeNames[COFFEE_COUNT] = {
            "Biala Kawa",
            "Czarna Kawa",
            "Cappuccino",
            "Espresso"
    };

    static std::vector<std::string> coffeeMachine;
    static int energy;
    static const int maxEnergy = 100;
};

int View::energy = 0;

std::vector<std::string> View::coffeeMachine = {
        "",
        "",
        " ____________________________________ ",
        " |                                  | ",
        " |  [<]  ````````````````````  [>]  | ",
        " |  [<]                        [>]  | ",
        " |       ````````````````````       | ",
        " |__________________________________| ",
        "   |`````````\\__________/`````````|   ",
        "   |             `   `            |   ",
        "   |                              |   ",
        "   |                              |   ",
        "   |                              |   ",
        "   |                              |   ",
        "   |.                            .|   ",
        "   |..                          ..|   ",
        "   |...                        ...|   ",
        "   ================================   ",
        "   ````````````````````````````````   "
};

void View::show(int selection) {
    Console::clearScreen();
    displayCoffeeMachine();
    Console::writeCenteredStrXY(6, coffeeNames[selection]);
    Console::writeCenteredStrXY(21, "Uzyj 'a' i 'd' do wyboru,");
    Console::writeCenteredStrXY(22, "Enter aby przygotowac");
    displayEnergyBar();
}

void View::displayCoffeeMachine() {
    for (int i = 0; i < View::ROWS; ++i) {
        Console::writeStrXY(1, i + 1, coffeeMachine[i].c_str());
    }
}

void View::drawCup(int selection) {
    CoffeeShop shop;
    const Cup& cup = shop.getCup(selection);

    int x = 13;
    int y = 13;

    const char* coffeeGraphic = cup.getGraphic();
    std::istringstream graphicStream(coffeeGraphic);
    std::string line;

    int currentY = y;
    while (std::getline(graphicStream, line)) {
        Console::writeStrXY(x, currentY++, line.c_str());
    }
}

void View::prepareCoffee(int selection) {
    Console::clearScreen();
    displayEnergyBar();
    displayCoffeeMachine();

    CoffeeShop shop;
    Coffee* coffee = shop.getCoffee(selection);

    std::string preparingMessage = "Przygotowywanie";
    Console::writeCenteredStrXY(6, preparingMessage.c_str());
    drawCup(selection);
    std::this_thread::sleep_for(std::chrono::seconds(4));
    Console::writeCenteredStrXY(6, "      Gotowe!     ");
    Console::writeCenteredStrXY(22, "      Smacznej kawusi! ;)    ");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    Console::clearScreen();    energy += coffee->getPower();
    if (energy > maxEnergy) energy = maxEnergy;

    displayEnergyBar();
    displayCoffeeMachine();


    drawCup(selection);
    std::this_thread::sleep_for(std::chrono::seconds(2));
}


void View::displayEnergyBar() {
    int energyPercentage = (energy * 100) / maxEnergy;

    Console::writeStrXY(1, 1, "Energia [");
    for (int i = 0; i < 50; ++i) {
        if (i < energyPercentage / 2) {
            Console::writeStrXY(i + 10, 1, "|");
        } else {
            Console::writeStrXY(i + 10, 1, " ");
        }
    }
    Console::writeStrXY(60, 1, "] ");
    Console::writeStrXY(62, 1, std::to_string(energyPercentage).c_str());
    Console::writeStrXY(65, 1, "%");
}

class TheGame {
public:
    TheGame();
    ~TheGame();
    void run();

private:
    void processKeyboard(int key);
    int selection;
};

TheGame::TheGame() : selection(0) {
    Console::cursorOff();
    Console::writeCenteredStrXY(12, "Naciśnij dowolny klawisz by rozpocząć grę");
    Console::getKey();
}

TheGame::~TheGame() {
    Console::clearScreen();
    Console::writeCenteredStrXY(12, "Naciśnij dowolny klawisz by zakończyć grę");
    Console::getKey();
    Console::cursorOn();
}

void TheGame::processKeyboard(int key) {
    switch (key) {
        case 'a':
            if (selection > 0) {
                selection--;
            }
            break;
        case 'd':
            if (selection < View::COFFEE_COUNT - 1) {
                selection++;
            }
            break;
        case Console::KEY_ENTER:
            View::prepareCoffee(selection);
            break;
        case Console::KEY_ESC:
            break;
    }
    View::show(selection);
}

void TheGame::run() {
    View::show(selection);

    int key = -1;
    do {
        if ((key = Console::getKey()) != -1) {
            processKeyboard(key);
        }
    } while (key != Console::KEY_ESC);
}

int main() {
    TheGame game;
    game.run();
    return EXIT_SUCCESS;
}

void Console::clearScreen() {
    COORD coord = { 0, 0 };
    DWORD written;
    DWORD cellCount;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    cellCount = csbi.srWindow.Right * csbi.srWindow.Bottom;

    FillConsoleOutputCharacter(hConsole, ' ', cellCount, coord, &written);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, coord, &written);

    SetConsoleCursorPosition(hConsole, coord);
}

void Console::cursorOff() {
    CONSOLE_CURSOR_INFO cursorInfo;
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void Console::cursorOn() {
    CONSOLE_CURSOR_INFO cursorInfo;
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

int Console::getKey() {
    if (_kbhit()) {
        int key = _getch();
        if (key == 0xE0 || key == 0x00) {
            key = 0x100 + _getch();
        }
        return key;
    }
    return -1;
}

void Console::writeStrXY(int x, int y, const char s[]) {
    COORD coord = { static_cast<SHORT>(x - 1), static_cast<SHORT>(y - 1) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    DWORD written;
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), s, strlen(s), &written, 0);
}

void Console::writeCenteredStrXY(int y, const char* str) {
    int nameLength = strlen(str);
    int centerX = (View::COLS - nameLength) / 2;
    writeStrXY(centerX, y, str);
}
#include <Windows.h>
#include "src/ShortestPathReturn.cpp"
#include "src/MenuHandler.cpp"


int main() {
    // Autor: Jacek Bogdański, 263895
    SetConsoleOutputCP(CP_UTF8);
    MenuHandler menu;
    menu.mainLoop();

    return 0;
}
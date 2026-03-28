// FarManager.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio.h>

using namespace std;



void openDir(const char* path) {
    WIN32_FIND_DATAA findData;
    HANDLE hFind;

    hFind = FindFirstFileA(path, &findData);

    printf("%s\n", path);
       
    do {
        if (lstrcmpA(findData.cFileName, ".") == 0 ||
            lstrcmpA(findData.cFileName, "..") == 0) {
            continue;
        }

        if (findData.dwFileAttributes) {
            printf("/%s\n", findData.cFileName);
        }
        else {
            printf("%s\n", findData.cFileName);
        }

    } while (FindNextFileA(hFind, &findData));

    FindClose(hFind);
}

int main() {
    int breakerMain = 1;
    int strMainNumber = 2;

    while (breakerMain) {
        openDir("C:\\Users\\levic\\OneDrive\\Desktop\\qwerty\\**");
        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
        }
        switch (key) {
            // Вниз
        case 80:
            strMainNumber++;
            break;
            // Вверх
        case 72:
            strMainNumber--;
            break;
            // Esc
        case 27:
            breakerMain = 0;
            break;
            // Enter
        case 13:
            break;
        }
        system("cls");
    }

	return 0;
}


// FarManager.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <direct.h>

using namespace std;


char newDir[260];

int nForSave = -1;

int openDir(int strMain) {
    int strDirNumber = 0;
    WIN32_FIND_DATAA findData;
    HANDLE hFind;

    // Получение актуального пути
    char cwd[260];
    _getcwd(cwd, 260);

    hFind = FindFirstFileA("*", &findData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        cout << "Ошибка открытия директории";
    }
    else {
        printf("%s\n", cwd);
        cout << "..";
        if (strDirNumber == strMain) printf("\t<--\n");
        else printf("\n");

        strDirNumber++;

        do {
            if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0) {
                continue;
            }

            // Проверка на каталог и сам вывод
            if ((findData.dwFileAttributes) & FILE_ATTRIBUTE_DIRECTORY) {
                if (strDirNumber == nForSave) {
                    _chdir(findData.cFileName);
                    nForSave = -1;
                    return strDirNumber;
                }
                else {
                    printf("/%s", findData.cFileName);
                    if (strDirNumber == strMain) {
                        printf("\t<--\n");
                    }
                    else printf("\n");
                }
            }
            else {
                printf("%s", findData.cFileName);
                if (strDirNumber == strMain) printf("\t<--\n");
                else printf("\n");
            }
            strDirNumber++;
        } while (FindNextFileA(hFind, &findData));

        FindClose(hFind);
        return strDirNumber;
    }

}

int main() {
    int breakerMain = 1;
    int strMainNumber = 0;

    int workingDir = _chdir("C:\\Users\\levic\\OneDrive\\Desktop\\qwerty\\");
    if (workingDir == -1) cout << "Ошибка в выборе рабочего каталога";
    else {
        while (breakerMain) {
            int maxFiles = openDir(strMainNumber);
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
                if (strMainNumber == 0) _chdir("..");
                else {
                    nForSave = strMainNumber;
                    strMainNumber = 0;
                }
                break;
            }
            if (strMainNumber < 0) strMainNumber = 0;
            else if (strMainNumber >= maxFiles) strMainNumber = maxFiles - 1;
            system("cls");
        }

    }

	return 0;
}


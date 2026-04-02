// FarManager.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <direct.h>

using namespace std;

// Переменные для хранения пути
char cwd[260];
char newDir[260];

int nForSave = -1;
int nForDelete = -1;
int nForShow = -1;

int statusError = 0;

char* workingDir1;
char* workingDir2;

char cwd1[260];
char cwd2[260];


void showFunction() {
    cout << endl << "F8 - удалить файл / директорию" << endl;
    cout << "F7 - создать директорию" << endl;
    cout << "SHIFT + F4 - создать файл" << endl;
    cout << "F3 - показ файла в консоли" << endl;
}


int showFile(char* path) {
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, path, "rb");

    if (err != 0 || !file) {
        printf("Ошбика показа файла");
        return -1;
    }
    else {
        char buffer[1024];
        size_t byteRead;
        system("cls");
        while ((byteRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            fwrite(buffer, 1, byteRead, stdout);
        }
        fclose(file);
        printf("\nНажмите ESC, чтобы выйти из файла..");
        int key = _getch();
        while (key != 27) {
            key = _getch();
        }
        return 0;
    }
}
int deleteFile(char* path) {
    return DeleteFileA(path);
}

int deleteDirectory(char* path) {
    WIN32_FIND_DATAA findData;
    HANDLE hFind;

    // Получение актуального пути
    char searchedPath[520];
    sprintf_s(searchedPath, "%s\\*", path);
    hFind = FindFirstFileA(searchedPath, &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        cout << "Ошибка открытия директории";
        return -1;
    }
    else {
        do {
            char fullPath[520];

            if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0) {
                continue;
            }

            sprintf_s(fullPath, "%s\\%s", path, findData.cFileName);

            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                deleteDirectory(fullPath);
            }

            // Если файл
            else {
                deleteFile(fullPath);
            }
        } while (FindNextFileA(hFind, &findData));

        FindClose(hFind);

        RemoveDirectoryA(path);
    }
    return 0;
}

void createFile(char* path) {
    char newFile[260];

    printf("Введите имя файла: ");
    scanf_s("%s", newFile, (unsigned)_countof(newFile));

    char fullPath[520];
    sprintf_s(fullPath, "%s\\%s", path, newFile);

    FILE* file = nullptr;
    errno_t err = fopen_s(&file, fullPath, "w");

    if (err != 0 || !file) {
        printf("Ошибка создания файла\n");
        return;
    }

    fclose(file);
}

int createFolder(char* path){
    // Ввод имени для папки
    char newFolder[260];
    printf("\nВведите название для папки: ");
    scanf_s("%s", newFolder, (unsigned)_countof(newFolder));

    char fullPath[520];
    sprintf_s(fullPath, "%s\\%s", path, newFolder);

    int result = _mkdir(fullPath);
    if (result == -1) {
        cout << "Ошбика при создании директории" << endl;
    }
    return result;
}

int openDirs(int strMain, int currentWindow) {
    char fullPath[520];
    char path1[520];
    char path2[520];
    char newPath[520];

    int strDirNumber = 0;

    WIN32_FIND_DATAA findData1;
    WIN32_FIND_DATAA findData2;

    HANDLE hFind1;
    HANDLE hFind2;

    sprintf_s(path1, "%s\\*", workingDir1);
    sprintf_s(path2, "%s\\*", workingDir2);

    // Получение хендлов для обоих каталогов
    hFind1 = FindFirstFileA(path1, &findData1);
    hFind2 = FindFirstFileA(path2, &findData2);

    if (hFind1 == INVALID_HANDLE_VALUE || hFind2 == INVALID_HANDLE_VALUE) {
        cout << "Ошибка открытия директории";
        return 0;
    }

    // Выводим заголовок с путями
    printf("%.50s | %.50s\n", workingDir1, workingDir2);

    cout << "..";
    if (strDirNumber == strMain) printf("\t<--\n");
    else printf("\n");
    strDirNumber++;

    // Получаем первый элемент для второго каталога
    int hasNext1 = 1;
    int hasNext2 = 2;

    while (hasNext1 || hasNext2) {
        // Пропускаем . и .. для первого каталога
        if (hasNext1 && (strcmp(findData1.cFileName, ".") == 0 || strcmp(findData1.cFileName, "..") == 0)) {
            hasNext1 = FindNextFileA(hFind1, &findData1);
            continue;
        }

        // Пропускаем . и .. для второго каталога
        if (hasNext2 && (strcmp(findData2.cFileName, ".") == 0 || strcmp(findData2.cFileName, "..") == 0)) {
            hasNext2 = FindNextFileA(hFind2, &findData2);
            continue;
        }

        // Выводим информацию о файлах/папках
        char name1[100] = "";
        char name2[100] = "";

        if (hasNext1) {
            strcpy_s(name1, findData1.cFileName);
        }
        else {
            strcpy_s(name1, "");
        }

        if (hasNext2) {
            strcpy_s(name2, findData2.cFileName);
        }
        else {
            strcpy_s(name2, "");
        }

        // Проверка на каталог и обработка действий
        if (hasNext1 && (findData1.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (strDirNumber == nForSave) {
                if (currentWindow == 1) {
                    sprintf_s(cwd1, "%s\\%s", workingDir1, findData1.cFileName);
                    workingDir1 = cwd1;
                }
                nForSave = -1;
            }
            else if (strDirNumber == nForDelete) {
                if (currentWindow == 1) {
                    sprintf_s(fullPath, "%s\\%s", workingDir1, findData1.cFileName);
                    deleteDirectory(fullPath);
                }
                nForDelete = -1;
            }
        }
        else if (hasNext1) { // Это файл
            if (strDirNumber == nForDelete) {
                if (currentWindow == 1) {
                    sprintf_s(fullPath, "%s\\%s", workingDir1, findData1.cFileName);
                    statusError = deleteFile(fullPath);
                }
                nForDelete = -1;
            }

            if (strDirNumber == nForShow) {
                if (currentWindow == 1) {
                    sprintf_s(fullPath, "%s\\%s", workingDir1, findData1.cFileName);
                    statusError = showFile(fullPath);
                }
                nForShow = -1;
            }
        }

        printf("%-50s | %-50s", name1, name2);
        if (strDirNumber == strMain) printf("\t<--\n");
        else printf("\n");

        strDirNumber++;

        // Получаем следующие элементы
        if (hasNext1) hasNext1 = FindNextFileA(hFind1, &findData1);
        if (hasNext2) hasNext2 = FindNextFileA(hFind2, &findData2);
    }

    FindClose(hFind1);
    FindClose(hFind2);
    showFunction();
    return strDirNumber;
}

int main() {
    int breakerMain = 1;
    int strMainNumber = 0;
    int currentWindow = 1;

    workingDir1 = _getcwd(cwd1, 260);
    workingDir2 = _getcwd(cwd2, 260);
    if (workingDir1 == NULL || workingDir2 == NULL) cout << "Ошибка в выборе рабочего каталога";
    else {
        while (breakerMain) {
            int maxFiles = openDirs(strMainNumber, currentWindow);
            int key = _getch();

            // Обработка расширенных клавиш
            if (key == 0 || key == 224) {
                key = _getch();

                switch (key) {
                    // Стрелка вниз
                case 80:
                    strMainNumber++;
                    break;
                    // Стрелка вверх
                case 72:
                    strMainNumber--;
                    break;
                    // F7
                case 65:
                    if (currentWindow == 1) statusError = createFolder(workingDir1);
                    else statusError = createFolder(workingDir2);
                    break;
                    // F8
                case 66:
                    if (strMainNumber != 0) {
                        nForDelete = strMainNumber;
                        strMainNumber = 0;
                    }
                    break;
                    // F3
                case 61:
                    if (strMainNumber != 0) nForShow = strMainNumber;
                    break;
                }
            }
            else {
                switch (key) {
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
            }
            if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && (GetAsyncKeyState(VK_F4) & 0x8000)) {
                if (currentWindow == 1) createFile(workingDir1);
                else createFile(workingDir2);
            }

            if (statusError == -1) break;
            if (strMainNumber < 0) strMainNumber = 0;
            else if (strMainNumber >= maxFiles && maxFiles > 0) strMainNumber = maxFiles - 1;
            system("cls");
        }
    }

    return 0;
}

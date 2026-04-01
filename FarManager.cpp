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

int openDir(char* actualDir, int strMain) {
    char fullPath[520];
    int strDirNumber = 0;
    WIN32_FIND_DATAA findData;
    HANDLE hFind;

    // Получение актуального пути

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
                    openDir(cwd, strMain);
                }
                else if (strDirNumber == nForDelete) {
                    sprintf_s(fullPath, "%s\\%s", cwd, findData.cFileName);
                    deleteDirectory(fullPath);
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
                if (strDirNumber != nForShow) {
                    printf("%s", findData.cFileName);
                    if (strDirNumber == strMain) printf("\t<--\n");
                    else printf("\n");
                }
                else {
                    sprintf_s(fullPath, "%s\\%s", cwd, findData.cFileName);
                    statusError = showFile(fullPath);
                    nForShow = -1;
                }
            }
            strDirNumber++;
        } while (FindNextFileA(hFind, &findData));
        
        FindClose(hFind);
        showFunction();
        return strDirNumber;
    }

}

int main() {
    int breakerMain = 1;
    int strMainNumber = 0;

    char* workingDir = _getcwd(cwd, 260);
    if (workingDir == NULL) cout << "Ошибка в выборе рабочего каталога";
    else {
        while (breakerMain) {
            workingDir = _getcwd(cwd, 260);
            int maxFiles = openDir(workingDir, strMainNumber);
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
                    statusError = createFolder(workingDir);
                    break;
                    // F8
                case 66:
                    if (strMainNumber != 0) {
                        nForDelete = strMainNumber;
                        strMainNumber = 0;
                    }
                    break;
                    // F4 
                case 62:
                    // Проверяем, зажат ли SHIFT
                    if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                        printf("Shift + F4 нажаты\n");
                        createFile(workingDir);
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

            if (statusError == -1) break;
            if (strMainNumber < 0) strMainNumber = 0;
            else if (strMainNumber >= maxFiles && maxFiles > 0) strMainNumber = maxFiles - 1;
            system("cls");
        }
    }

    return 0;
}

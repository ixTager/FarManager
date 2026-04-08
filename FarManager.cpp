// FarManager.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <direct.h>

using namespace std;

int maxFiles1 = 0;
int maxFiles2 = 0;

int nForSave1 = -1;
int nForSave2 = -1;
int nForDelete1 = -1;
int nForDelete2 = -1;
int nForShow1 = -1;
int nForShow2 = -1;
int nForCopy1 = -1;
int nForCopy2 = -1;
int currentWindow = 1;
int strMainNumber1 = 0;
int strMainNumber2 = 0;

int statusError = 0;

char workingDir1[260];
char workingDir2[260];
char oldWorkingDir1[260];
char oldWorkingDir2[260];

char buffer[1024];
char cwd1[260];
char cwd2[260];

char* copyedFileName = nullptr;

void showFunction() {
    cout << endl << "F8 - удалить файл / директорию" << endl;
    cout << "F7 - создать директорию" << endl;
    cout << "SHIFT + F4 - создать файл" << endl;
    cout << "F3 - показ файла в консоли" << endl;
    cout << "F5 - копирование файлов / директории" << endl;
}

void copyFile(const char* srcDir, const char* fileName, const char* destDir) {
    char srcPath[520];
    char destPath[520];

    sprintf_s(srcPath, "%s\\%s", srcDir, fileName);
    sprintf_s(destPath, "%s\\%s", destDir, fileName);

    FILE* src = nullptr;
    FILE* dest = nullptr;

    if (fopen_s(&src, srcPath, "rb") != 0 || !src) {
        printf("Ошибка открытия исходного файла\n");
        return;
    }

    if (fopen_s(&dest, destPath, "wb") != 0 || !dest) {
        printf("Ошибка создания файла назначения\n");
        fclose(src);
        return;
    }

    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytesRead, dest);
    }

    fclose(src);
    fclose(dest);
}

int showFile(char* path) {
    FILE* file = nullptr;
    errno_t err = fopen_s(&file, path, "rb");

    if (err != 0 || !file) {
        printf("Ошбика показа файла");
        return -1;
    }
    else {
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
    char fullPath[520];

    if (copyedFileName != nullptr) {
        sprintf_s(fullPath, "%s\\%s", path, copyedFileName);
    }
    else {
        printf("Введите имя файла: ");
        scanf_s("%s", newFile, (unsigned)_countof(newFile));
        sprintf_s(fullPath, "%s\\%s", path, newFile);
    }

    FILE* file = nullptr;
    errno_t err = fopen_s(&file, fullPath, "w");

    if (err != 0 || !file) {
        printf("Ошибка создания файла\n");
        return;
    }
    else if (copyedFileName != nullptr) {
        fwrite(&buffer, sizeof(char[1024]), sizeof(buffer) / sizeof(buffer[0]), file);
        copyedFileName = nullptr;
    }

    fclose(file);
}

int createFolder(char* path) {
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

void updateCurrentDirectory(int window) {
    if (window == 1) {
        _chdir(workingDir1);
        _getcwd(workingDir1, 260);
    }
    else {
        _chdir(workingDir2);
        _getcwd(workingDir2, 260);
    }
}

// Исправленная функция openDirs
int openDirs(int strMain, int currentWindow) {
    char fullPath[520];
    char path1[520];
    char path2[520];

    int strDirNumber = 0;

    WIN32_FIND_DATAA findData1;
    WIN32_FIND_DATAA findData2;

    HANDLE hFind1;
    HANDLE hFind2;

    // Обновляем пути перед отображением
    updateCurrentDirectory(1);
    updateCurrentDirectory(2);

    sprintf_s(path1, "%s\\*", workingDir1);
    sprintf_s(path2, "%s\\*", workingDir2);

    hFind1 = FindFirstFileA(path1, &findData1);
    hFind2 = FindFirstFileA(path2, &findData2);

    if (hFind1 == INVALID_HANDLE_VALUE || hFind2 == INVALID_HANDLE_VALUE) {
        cout << "Ошибка открытия директории";
        return 0;
    }

    // Выводим заголовок с путями
    printf("%.50s | %.50s\n", workingDir1, workingDir2);

    // Обработка ".." с индексом 0
    if (currentWindow == 1 && strDirNumber == strMain)
        printf("%-47s<-- | %-50s\n", "..", "..");
    else if (currentWindow == 2 && strDirNumber == strMain)
        printf("%-50s | %-47s<--\n", "..", "..");
    else
        printf("%-50s | %-50s\n", "..", "..");
    strDirNumber++;

    int hasNext1 = 1;
    int hasNext2 = 1;

    while (hasNext1 || hasNext2) {
        // Пропускаем . и .. для первого каталога
        if (hasNext1 && (strcmp(findData1.cFileName, ".") == 0 ||
            strcmp(findData1.cFileName, "..") == 0)) {
            hasNext1 = FindNextFileA(hFind1, &findData1);
            continue;
        }

        // Пропускаем . и .. для второго каталога
        if (hasNext2 && (strcmp(findData2.cFileName, ".") == 0 ||
            strcmp(findData2.cFileName, "..") == 0)) {
            hasNext2 = FindNextFileA(hFind2, &findData2);
            continue;
        }

        char name1[100] = "";
        char name2[100] = "";

        if (hasNext1) {
            strcpy_s(name1, findData1.cFileName);
        }

        if (hasNext2) {
            strcpy_s(name2, findData2.cFileName);
        }

        // Обработка операций с файлами/папками
        if (hasNext1 && (findData1.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (strDirNumber == nForSave1) {
                sprintf_s(workingDir1, "%s\\%s", workingDir1, findData1.cFileName);
                nForSave1 = -1;
            }
            else if (strDirNumber == nForDelete1) {
                sprintf_s(fullPath, "%s\\%s", workingDir1, findData1.cFileName);
                deleteDirectory(fullPath);
                nForDelete1 = -1;
            }
            else if (strDirNumber == nForCopy1) {
                    
            }
            
        }
        else if (hasNext1) {
            if (strDirNumber == nForDelete1) {
                sprintf_s(fullPath, "%s\\%s", workingDir1, findData1.cFileName);
                statusError = deleteFile(fullPath);
                nForDelete1 = -1;
            }
            else if (strDirNumber == nForShow1) {
                sprintf_s(fullPath, "%s\\%s", workingDir1, findData1.cFileName);
                statusError = showFile(fullPath);
                nForShow1 = -1;
            }
            else if (strDirNumber == nForCopy1) {
                copyFile(workingDir1, findData1.cFileName, workingDir2);
                nForCopy1 = -1;
                openDirs(strMainNumber2, 2);
            }

            if (copyedFileName == findData1.cFileName) {
                createFile(workingDir2);
                updateCurrentDirectory(2);
            }
        }

        if (hasNext2 && (findData2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            if (strDirNumber == nForSave2) {
                sprintf_s(workingDir2, "%s\\%s", workingDir2, findData2.cFileName);
                nForSave2 = -1;
            }
            else if (strDirNumber == nForDelete2) {
                sprintf_s(fullPath, "%s\\%s", workingDir2, findData2.cFileName);
                deleteDirectory(fullPath);
                nForDelete2 = -1;
            }
            else if (strDirNumber == nForCopy2) {

            }

        }
        else if (hasNext2) {
            if (strDirNumber == nForDelete2) {
                sprintf_s(fullPath, "%s\\%s", workingDir2, findData2.cFileName);
                statusError = deleteFile(fullPath);
                nForDelete2 = -1;
            }
            else if (strDirNumber == nForShow2) {
                sprintf_s(fullPath, "%s\\%s", workingDir2, findData2.cFileName);
                statusError = showFile(fullPath);
                nForShow2 = -1;
            }
            else if (strDirNumber == nForCopy2) {
                copyFile(workingDir2, findData2.cFileName, workingDir1);
                nForCopy2 = -1;
                openDirs(strMainNumber2, 1);
            }

            if (copyedFileName == findData2.cFileName) {
                createFile(workingDir1);
                updateCurrentDirectory(1);
            }

        }


        // Вывод с подсветкой выбранной строки
        if (strDirNumber == strMain) {
            if (currentWindow == 1)
                printf("%-47s\t<-- | %-50s\n", name1, name2);
            else
                printf("%-50s | %-47s\t<--\n", name1, name2);
        }
        else {
            printf("%-50s | %-50s\n", name1, name2);
        }

        if (currentWindow == 1) maxFiles1++;
        else maxFiles2++;

        strDirNumber++;

        if (hasNext1) hasNext1 = FindNextFileA(hFind1, &findData1);
        if (hasNext2) hasNext2 = FindNextFileA(hFind2, &findData2);
    }

    FindClose(hFind1);
    FindClose(hFind2);
    showFunction();
    if (currentWindow == 1) maxFiles1 = strDirNumber;
    else maxFiles2 = strDirNumber;
    return 1;
}
int main() {
    int breakerMain = 1;

    _getcwd(workingDir1, 260);
    strcpy_s(workingDir2, workingDir1);

    while (breakerMain) {
        if (currentWindow == 1) statusError = openDirs(strMainNumber1, currentWindow);
        else statusError = openDirs(strMainNumber2, currentWindow);

        int key = _getch();

        if (key == 0 || key == 224) {
            key = _getch();
            switch (key) {
            case 80: // Вниз
                if (currentWindow == 1)
                    strMainNumber1++;
                else
                    strMainNumber2++;
                break;
            case 72: // Вверх
                if (currentWindow == 1)
                    strMainNumber1--;
                else
                    strMainNumber2--;
                break;
            case 65: // F7
                if (currentWindow == 1)
                    statusError = createFolder(workingDir1);
                else
                    statusError = createFolder(workingDir2);
                break;
            case 66: // F8
                if (currentWindow == 1) {
                    if (strMainNumber1 > 0) {
                        nForDelete1 = strMainNumber1;
                        strMainNumber1 = 0;
                    }
                }
                else {
                    if (strMainNumber2 > 0) {
                        nForDelete2 = strMainNumber2;
                        strMainNumber2 = 0;
                    }
                }
                break;
            case 61: // F3
                if (currentWindow == 1) {
                    if (strMainNumber1 > 0)
                        nForShow1 = strMainNumber1;
                }
                else {
                    if (strMainNumber2 > 0)
                        nForShow2 = strMainNumber2;
                }
                break;
            case 63: // F5
                if (currentWindow == 1) {
                    if (strMainNumber1 > 0) nForCopy1 = strMainNumber1;
                }
                else {
                    if (strMainNumber2 > 0) nForCopy2 = strMainNumber2;
                }
                break;
            }
        }
        else {
            switch (key) {
            case 27: // ESC
                breakerMain = 0;
                break;
            case 13: // Enter
                if (currentWindow == 1) {
                    if (strMainNumber1 == 0) {
                        char* lastSlash = strrchr(workingDir1, '\\');
                        if (lastSlash != NULL) {
                            *lastSlash = '\0';
                        }
                        strMainNumber1 = 0;
                        maxFiles1 = 0;
                    }
                    else {
                        nForSave1 = strMainNumber1;
                        strMainNumber1 = 0;
                        maxFiles1 = 0;
                    }
                }
                else {
                    if (strMainNumber2 == 0) {
                        char* lastSlash = strrchr(workingDir2, '\\');
                        if (lastSlash != NULL) {
                            *lastSlash = '\0';
                        }
                        strMainNumber2 = 0;
                        maxFiles2 = 0;
                    }
                    else {
                        nForSave2 = strMainNumber2;
                        strMainNumber2 = 0;
                        maxFiles2 = 0;
                    }
                }
                break;
            case 9: // Tab
                currentWindow = currentWindow == 1 ? 2 : 1;
                break;
            }
        }

        // SHIFT + F4
        if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && (GetAsyncKeyState(VK_F4) & 0x8000)) {
            if (currentWindow == 1)
                createFile(workingDir1);
            else
                createFile(workingDir2);
        }

        // Корректировка индексов
        if (strMainNumber1 < 0) strMainNumber1 = 0;
        if (strMainNumber2 < 0) strMainNumber2 = 0;
        if (strMainNumber1 >= maxFiles1 && maxFiles1 > 0) strMainNumber1 = maxFiles1 - 1;
        if (strMainNumber2 >= maxFiles2 && maxFiles2 > 0) strMainNumber2 = maxFiles2 - 1;

        if (statusError == -1) break;
        system("cls");
    }

    return 0;
}

// FarManager.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <filesystem>
#include <conio.h>
#include <stdio.h>

using namespace std;
using namespace filesystem;


int main() {
	int strMainNumber = 2;
	int breakerMain = 1;

	path currentPath = current_path();

	while (breakerMain) {
		directory_iterator dirIterator(currentPath);
		int n = 3;

		cout << currentPath << endl;
		cout << "..";

		if (strMainNumber == 2) cout << "\t<--" << endl;
		else cout << endl;

		for (const auto& entry : dirIterator) {
			if (entry.is_directory()) {
				cout << "/";
			}

			if (strMainNumber == n) {
				cout << entry.path().filename();
				cout << "\t<--" << endl;
			}
			else {
				cout << entry.path().filename() << endl;
			}



			n++;
		}

		int key = _getch();
		if (key == 0 || key == 224) {
			key = _getch();
			switch (key) {
				// Вниз
			case 80:
				if (strMainNumber < n) strMainNumber++;
				else strMainNumber = 2;
				break;
				// Вверх
			case 72:
				if (strMainNumber > 1) strMainNumber--;
				else strMainNumber = n;
				break;
			}
		}

		else {
			switch (key) {
			// ESC
			case 27:
				breakerMain = 0;
				break;
			// Enter
			case 13:
				if (strMainNumber == 2) {
					path newPath = currentPath.parent_path();
					currentPath = newPath;
					break;

				}
			}
		}
		

		system("cls");
	}
	

	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.



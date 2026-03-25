// FarManager.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <filesystem>
#include <conio.h>
#include <stdio.h>

using namespace std;
using namespace filesystem;


int main() {
	int strMainNumber = 3;
	int breakerMain = 1;

	while (breakerMain) {
		int n = 3;
		path currentPath = current_path();
		directory_iterator dirIterator(currentPath);

		cout << currentPath << endl;
		cout << ".." << endl;

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
				strMainNumber++;
				break;
				// Вверх
			case 72:
				strMainNumber--;
				break;
			}
		}

		else if (key == 13){
			breakerMain = 0;
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



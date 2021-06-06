#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <string>

using namespace std;

#define AllocGood -2
#define AllocBad -1

// внутренний код ошибки
enum Outcomes {
	All_Good, //Все хорошо
	Error_Open,//Ошибка открытия файла
	File_Empty,//Пустой файл
	Error_Elements,//Некорректный элемент
	Not_Rectangular,//Непрямоугольная
	Error_Of_Memory//Ошибка выделения памяти
};

//объединение
union MyUnion {

	struct ErrorMatrix {

		int row;
		int column;
		int pos;

	} ErrElem;

	int ErrRectRow;

	int ErrAlloc;

	int ErrOpen;
};// NoArg;// = { -1 };

//проверка на корректность, прямоугольность, пустоту 
Outcomes CheckingMatrix(ifstream& fin, int& row_counter, int& column_counter, MyUnion *VariosError = NULL/*MyUnion &VariosError = NoArg*/) {
	int pos, quantity_of_elements_in_the_other_row = 0;
	//if (&VariosError == &NoArg) // if (VariosError.ErrOpen != -1)
	//	;
	column_counter = 0;
	row_counter = 0;
	while (true) {

		char c = fin.get();
		int var;
		switch (c) {

		case ' ': case '\t':
			continue;
		case EOF:
			if (quantity_of_elements_in_the_other_row == 0)
				break;
		case '\n':
			if (quantity_of_elements_in_the_other_row == 0)
				continue;
			if (column_counter == 0)
				column_counter = quantity_of_elements_in_the_other_row;
			else if (column_counter != quantity_of_elements_in_the_other_row) {
				if (VariosError != NULL)
					VariosError->ErrRectRow = row_counter;
				row_counter = 0;
				column_counter = 0;
				return Outcomes::Not_Rectangular;
			}
			row_counter++;
			quantity_of_elements_in_the_other_row = 0;
			continue;
		default:
			fin.unget();
			if (VariosError != NULL)
				pos = fin.tellg();
			fin >> var;
			if (fin.fail() || ((c = fin.peek()) != ' ' && c != '\t' && c != '\n' && c != EOF)) {
				if (VariosError != NULL) {
					VariosError->ErrElem.row = row_counter;
					VariosError->ErrElem.column = quantity_of_elements_in_the_other_row;
					VariosError->ErrElem.pos = pos;
				}
				row_counter = 0;
				column_counter = 0;
				return Outcomes::Error_Elements;
			}
			quantity_of_elements_in_the_other_row++;
			continue;
		}
		if (column_counter == 0) {
			row_counter = 0;
			return Outcomes::File_Empty;
		}
		return Outcomes::All_Good;
	}
}

//освобождение памяти
void DeletingMatrix(int**& matr, int row_counter) {
	for (int k = 0; k < row_counter; k++)
		delete[]matr[k];
	delete[]matr;
	matr = NULL;
}

//выделение памяти
int** AllocatingMatrix(int row_counter, int column_counter, int& i) {

	int** matr = NULL;
	i = AllocBad;
	try {
		matr = new int* [row_counter];
		for (i = 0; i < row_counter; i++)
			matr[i] = new int[column_counter];
	}
	catch (...) {
		if (matr != NULL)
			DeletingMatrix(matr, i);
		return NULL;
	}
	i = AllocGood;
	return matr;
}

//чтение матрицы
int** ReadingMatrix(ifstream& fin, int** matr, int row_counter, int column_counter) {
	for (int i = 0; i < row_counter; i++)
		for (int j = 0; j < column_counter; j++)
			fin >> *(*(matr + i) + j);
	return matr;
}

//вывод матрицы в консоль
string MatrixToStr(int** matr, int row_counter, int column_counter, int number = 5) {

	ostringstream ss;
	for (int i = 0; i < row_counter; i++) {
		for (int j = 0; j < column_counter; j++)
			ss << setw(number) << matr[i][j];
		ss << endl;
	}
	return ss.str();
}

//обработка матрицы
int** ProcessingMatrix(int** matr, int row_counter, int column_counter, int a, int b, int c, int d) {

	int sum = 0;
	if (a == c)
		for (; b < d + 1; b++) 
			sum += matr[a][b];
	else {
		for (; b < column_counter; b++)
			sum += matr[a][b];
		for (int e = a + 1; e < c; e++)
			for (int r = 0; r < column_counter; r++)
				sum += matr[e][r];
		for (int w = 0; w < d + 1; w++) 
			sum += matr[c][w];
	}
	for (int j = 0; j < column_counter; j++)
		matr[0][j] = sum;

	return matr;
}

//копирование элементов
int** Copy(int** matr, int** copy_matr, int row_counter, int column_counter) {
	for (int i = 0; i < row_counter; i++)
		for (int j = 0; j < column_counter; j++)
			copy_matr[i][j] = matr[i][j];
	return copy_matr;
}

//Создание копии матрицы
int** CopyingMatrix(int** matr, int row_counter, int column_counter, int& i) {
	
	int** copy_matr = AllocatingMatrix(row_counter, column_counter, i);

	if (copy_matr == NULL)
		return NULL;

	return Copy(matr, copy_matr, row_counter, column_counter);
}

//загрузка матрицы
Outcomes LoadingMatrix(const string& file, int**& matr, int& row_counter, int& column_counter, MyUnion* VariosError = NULL) {

	Outcomes result;

	ifstream fin;
	fin.open(file);
	if (!fin) {
		if (VariosError != NULL)
			VariosError->ErrOpen = errno;
		matr = NULL;
		row_counter = 0;
		column_counter = 0;
		return Outcomes::Error_Open;
	}

	result = CheckingMatrix(fin, row_counter, column_counter, VariosError);
	if (result != Outcomes::All_Good) {
		fin.close();
		matr = NULL;
		return result;
	}
	int i;
	matr =  AllocatingMatrix(row_counter, column_counter, i);
	if (matr == NULL) {
		fin.close();
		if (VariosError != NULL)
			VariosError->ErrAlloc = i;
		row_counter = 0;
		column_counter = 0;
		return Outcomes::Error_Of_Memory;
	}

	fin.clear();
	fin.seekg(0);
	ReadingMatrix(fin, matr, row_counter, column_counter);
	fin.close();

	return Outcomes::All_Good;
}

//главная 
int main() {

	for (;;) {

		cout << "Введите имя файла или \"*\" для завершения работы: ";
		if (cin.peek() == '*' && cin.rdbuf()->in_avail() == 2)
			return 0;

		MyUnion VariosError;

		int row_counter, column_counter;
		Outcomes result;

		string file;
		getline(cin, file);

		int** matr;

		if ((result = LoadingMatrix(file, matr, row_counter, column_counter, &VariosError)) != Outcomes::All_Good) {

			cout << "\nКод ошибки: " << result << endl;

			switch (result) {
			case Error_Open:
				cout << "Системный код ошибки \"errno\": " << VariosError.ErrOpen << endl;
				break;
			case File_Empty:
				cout << "\nВ файле отсутствует матрица.";
				break;
			case Error_Elements: {
				ifstream fin(file);
				fin.seekg(VariosError.ErrElem.pos);
				string split;
				fin >> split;
				fin.close();
				cout << "\nОшибка в строке " << 1 + VariosError.ErrElem.row << " в столбце " << 1 + VariosError.ErrElem.column << '.';
				cout << "\nНекорректный элемент имеет последовательность: " << split;
				break;
			}
			case Not_Rectangular:
				cout << "\nМатрица не прямоугольна, начиная со строки " << 1 + VariosError.ErrRectRow << '.';
				break;
			case Error_Of_Memory:
				if (VariosError.ErrAlloc == AllocBad) {
					cout << "\nОшибка выделения памяти под массив указателей для строк матрицы.\n";
					break;
				}
				else {
					cout << "\nОшибка выделения памяти под строки матрицы, начиная со строки " << VariosError.ErrAlloc + 1 << ".\n";
					break;
				}
			}

			cout << "\nНажмите любую клавишу...";
			system("cls");
			continue;
		}

		cout << "\n\nИсходная матрица: \n\n";
		cout << MatrixToStr(matr, row_counter, column_counter);

		int i;
		int** copy_matr = CopyingMatrix(matr, row_counter, column_counter, i);
		if (i == AllocBad) {
			cout << "\nОшибка выделения памяти под массив указателей для строк матрицы.\n";
			continue;
		}
		else if (i >= 0) {
			cout << "\nОшибка выделения памяти под строки матрицы, начиная со строки " << i + 1 << ".\n";
			continue;
		}

		cout << "\n\nВведите границы диапазона строк и столбцов(для Х и Y).\n";

		int a = -1, b = -1, c = -1, d = -1, mas[4], sum = 0;

		for (int i = 0; i < 4;) {

			cin.ignore(cin.rdbuf()->in_avail());
			if (cin.peek() == '*' && cin.rdbuf()->in_avail() == 2) {
				DeletingMatrix(matr, row_counter);
				return 0;
			}
			if (cin.peek() == '\n')
				continue;

			cin >> mas[i];

			if (cin.peek() != '\n') {
				cin.clear();
				cin.ignore(cin.rdbuf()->in_avail());
				cout << "Некорректный ввод значений для диапозона Х и Y. Повторите ввод.\n\n";
				continue;
			}

			if (a == -1) {
				a = mas[i] - 1;
				if (a < 0 || a > row_counter - 1) {
					a = -1;
					cout << "Недопустимый ввод строки для значения Х. Повторите ввод.\n\n";
					continue;
				}
			}
			else if (a != -1 && b == -1) {
				b = mas[i] - 1;
				if (b < 0 || b > column_counter - 1) {
					b = -1;
					cout << "Недопустимый ввод столбика для значения Х. Повторите ввод.\n\n";
					continue;
				}
			}
			else if (a != -1 && b != -1 && c == -1) {
				c = mas[i] - 1;
				if (c > row_counter - 1 || c < 0 || c < a) {
					c = -1;
					cout << "Недопустимый ввод строки для значения Y. Повторите ввод.\n\n";
					continue;
				}
			}
			else if (a != -1 && b != -1 && c != -1) {
				d = mas[i] - 1;
				if (d > column_counter - 1 || d < 0 || (a == c && b > d)) {
					cout << "Недопустимый ввод столбика для значения Y. Повторите ввод.\n\n";
					continue;
				}
			}
			i++;
		}

		ProcessingMatrix(matr, row_counter, column_counter, a, b, c, d);

		cout << "\n\nОбработанная матрица: \n\n";
		cout << MatrixToStr(matr, row_counter, column_counter);

		//проверка на вывод копии(начальная) и обработанная(начальная обработанная)
		/*		cout << "\n\n=============================\n\n";
		cout << "First: \n";
		cout << MatrixToStr(copy_matr, row_counter, column_counter);
		cout << "Second: \n";
		cout << MatrixToStr(matr, row_counter, column_counter);*/

		DeletingMatrix(matr, row_counter);
		DeletingMatrix(copy_matr, row_counter);
		return 0;
	}
}

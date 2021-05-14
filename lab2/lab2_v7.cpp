#define LINUX

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits>
#include <cerrno>

#ifdef LINUX
#include <stdlib.h>
#include <termios.h>
#else
#include <windows.h>
#include <conio.h>
#include <sstream>
#endif

typedef double d_arr;
using namespace std;

template <typename Xtype>
size_t ElemSize(Xtype a, ios::fmtflags f = ios_base::dec, int prec = 6)
{
    return ((ostringstream&)(ostringstream() << resetiosflags(ios_base::basefield) << setiosflags(f) << setprecision(prec) << a)).str().size();
}

void space(int needed, ostringstream& ostr)
{
    ostr << setw(needed) << setfill(' ') << "";
}
void assembly_stream(int num, const double& elem, ostringstream& number_stream, ostringstream& elem_stream)
{
    int differences = ElemSize(num) - ElemSize(elem);
    if (number_stream.str().size() + abs(differences) > 80)
    {
        cout << number_stream.str() << '\n' << elem_stream.str() << '\n' << '\n';
        number_stream.str("Номера  |");
        elem_stream.str("Элемены |");
    }
    if (differences > 0)
    {
        number_stream << ' ' << num << " |";
        space(1 + differences / 2 + differences % 2, elem_stream);
        elem_stream << elem;
        space(1 + differences / 2, elem_stream);
        elem_stream << '|';
    }
    else if (differences == 0)
    {
        number_stream << ' ' << num << " |";
        elem_stream << ' ' << elem << " |";
    }
    else
    {
        space(1 + abs(differences) / 2 + abs(differences) % 2, number_stream);
        number_stream << num;
        space(1 + abs(differences) / 2, number_stream);
        number_stream << '|';
        elem_stream << ' ' << elem << " |";
    }
}

#ifdef LINUX
static struct termios old, newattr;
void initTermios(int echo)
{
    tcgetattr(0, &old);
    newattr = old;
    newattr.c_lflag &= ~ICANON;
    newattr.c_lflag &= echo ? ECHO : ~ECHO;
    tcsetattr(0, TCSANOW, &newattr);
}
void resetTermios(void)
{
    tcsetattr(0, TCSANOW, &old);
}
char getch_(int echo)
{
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}
char getch(void)
{
    return getch_(0);
}
char getche(void)
{
    return getch_(1);
}
#endif
char hotkey_to_char(int echo)
{
    char ch;
#ifdef LINUX

    if (echo)
        ch = getche();
    else
        ch = getch();
#else
    ch = _getch();
#endif
    return ch;
}

void term_clear_screen() 
{
#ifdef LINUX
    system("clear");
#else
    system("cls");
#endif
}

int main()
{
    bool power = true;
    d_arr* buf_double = nullptr;
    int dsize;
    char mode = '0';
    setlocale(LC_ALL, "ru");
    while (power)
    {
        if ((mode == '1' || mode == '2') && buf_double)
            mode = '3';
        else
        {
            //term_clear_screen();
            cout<< "1) Создать исходные данные\n"
                << "2) Считать данные\n"
                << "3) Вывести исходные данные на экран\n"
                << "4) Обработать данные по алгоритму функции\n"
                << "5) Завершиен работы\n"
                << "Выберите режим работы: " << endl;
            mode = hotkey_to_char(false);
        }
        cout << '\n';

        d_arr dtemp = 0, S1 = 0, S2 = 0, Smax = 0;
        ostringstream numberstr("Номера  |", ostringstream::ate), elementstr("Элемены |", ostringstream::ate);
        string tempstr, FileAdress;
        fstream file;

        if (mode == '1' || mode == '2')
        {
            if (buf_double)
            {
                cout << "Очистка данных в памяти\n";
                delete[] buf_double;
                buf_double = nullptr;
            }
            dsize = 0;
        }

        switch (mode)
        {
        case '1':
            while (true)
            {
                cout << "Введите количество элементнов, которое вы хотите добавить(чтобы вернуться *): ";
                if (!(cin >> dsize) || (cin.peek() != '\n') || (dsize <= 0))
                {
                    cin.clear();
                    getline(cin, tempstr);
                    if (!dsize && tempstr == "*")
                        break;
                    else
                    {
                        cout << "Ошибка, введено недопустимое значение\n";
                        continue;
                    }
                }

                buf_double = new(nothrow) d_arr[dsize];
                if (buf_double == nullptr)
                {
                    cout << "\nОшибка выделения оперативной памяти\n";
                    break;
                }

                cout << "\nВведите значения для элементов(чтобы вернуться *):\n";
                for (int i = 0; i < dsize; i++)
                {
                    cout << "\tЭлемент " << (i + 1) << " = ";
                    if (!(cin >> buf_double[i]) || (cin.peek() != '\n'))
                    {
                        cin.clear();
                        getline(cin, tempstr);
                        if (!buf_double && tempstr == "*")
                        {
                            break;
                        }
                        else
                        {
                            cout << "\tВведено недопустимое значение!\n";
                            i--;
                            continue;
                        }
                    }
                }
                if (tempstr == "*")
                {
                    delete[] buf_double;
                    buf_double = nullptr;
                    break;
                }

                bool file_write = true;
                cin.ignore();
                while (true)
                {
                    cout << "\nВведите имя файла в который будут сохранены полученные данные(чтобы не сохранять*): ";
                    getline(cin, FileAdress);
                    if (FileAdress == "*")
                    {
                        file_write = false;
                        break;
                    }

                    file.open(FileAdress, ios_base::in);
                    if (file.is_open())
                    {
                        file.close();
                        char choice;
                        while (true)
                        {
                            cout << "\nФайл с таким названием уже существует, вы хотите:\n"
                                << "1)Перезаписать файл\n"
                                << "2)Указать новое имя файла\n"
                                << "3)Не сохранять" << endl;
                            choice = hotkey_to_char(false);
                            if (choice == '1' || choice == '2' || choice == '3')
                                break;
                            cout << "\nВведено неверное значение!\n";
                        }
                        if (choice == '2')
                            continue;
                        if (choice == '3')
                            file_write = false;
                        break;
                    }
                    break;
                }

                if (file_write)
                {
                    file.open(FileAdress, ios_base::out | ios_base::trunc | ios_base::binary);
                    for (int i = 0; i < dsize; i++)
                        file.write((char*)(buf_double + i), sizeof(d_arr));
                    file.close();
                }
                break;
            }
            break;
        case '2':
            while (true)
            {
                cout << "Введите имя файла(чтобы вернуться *): ";
                getline(cin, FileAdress);
                if (FileAdress == "*")
                    break;
                file.open(FileAdress, ios_base::in | ios_base::binary);
                if (!(file.is_open()))
                {
                    perror(("\nОшибка открытия файла с именем " + FileAdress).c_str());
                    continue;
                }

                file.seekg(0, ios::end);
                dsize = file.tellg();
                if (dsize % sizeof(d_arr))
                {
                    cout << "\nОшибка считывания, нарушена целостность данных\n";
                    file.close();
                    break; //continue
                }
                dsize /= sizeof(d_arr);

                if (!dsize)
                {
                    file.close();
                    cout << "\nВ файле не оказалось значений, которые можно считать\n";
                    break;
                }

                buf_double = new(nothrow) d_arr[dsize];
                if (buf_double == nullptr)
                {
                    file.close();
                    cout << "\nОшибка выделения оперативной памяти\n";
                    break; //continue
                }
                file.clear();
                file.seekg(0, ios::beg);
                for (int i = 0; i < dsize; i++)
                    file.read((char*)(buf_double + i), sizeof(d_arr));
                file.close();
                break;
            }
            break;
        case '3':
            if (!buf_double)
            {
                cout << "На данный момент в памяти еще нет никаких данных\n";
                break;
            }
            for (int i = 0; i < dsize; i++)
                assembly_stream(i + 1, buf_double[i], numberstr, elementstr);
            cout << numberstr.str() << '\n' << elementstr.str() << '\n' << '\n';
            break;
        case '4':
            if (!buf_double)
            {
                cout << "На данный момент в памяти еще нет никаких данных\n";
                break;
            }
            for (int i = 0; i < dsize; i++)
            {
                S1 += buf_double[i] / pow(i + 1, 2);
                if (i > 3)
                    S2 += pow(buf_double[i], 2);
            }
            Smax = S1 > S2 ? S1 : S2;
            cout << "\nРезультат работы Smax = " << Smax << '\n' << '\n';
            break;
        case '5':
            power = false;
            break;
        default:
            cout << "Введено неверное значение!\n";
        }
    }
    if (buf_double != nullptr)
        delete[] buf_double;
    return 0;
}
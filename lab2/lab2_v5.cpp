//#define SSTREAM
#define LINUX
//#define LINUXV2
//#define critical
#define Binary
//#define filesize
//#define withsave

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits>
#include <cerrno>
#ifndef LINUX
#include <sstream>
#endif

#ifdef LINUX
#include <stdlib.h>
#include <termios.h>
#else
#include <conio.h>
#endif

typedef double d_arr;
using namespace std;

template <typename Xtype>
size_t ElemSize(Xtype a, ios::fmtflags f = ios_base::dec, int prec = 6)
{
    return ((ostringstream&)(ostringstream()<<resetiosflags(ios_base::basefield)<<setiosflags(f)<<setprecision(prec)<<a)).str().size();
}

void space(int needed, ostringstream &ostr)
{
    ostr<<setw(needed)<<setfill(' ')<<"";
}
void assembly_stream(int num, const double &elem, ostringstream &number_stream, ostringstream &elem_stream)
{
    int differences=ElemSize(num)-ElemSize(elem);
    if(number_stream.str().size()+abs(differences)>80)
    {
        cout<<number_stream.str()<<'\n'<<elem_stream.str()<<'\n'<<'\n';
        number_stream.str("Номера  |");
        elem_stream.str("Элемены |");
    }
        if(differences>0)
    {
        number_stream<<' '<<num<<" |";
        space(1+differences/2+differences%2, elem_stream);
        elem_stream<<elem;
        space(1+differences/2, elem_stream);
        elem_stream<<'|';
    }
    else if(differences==0)
    {
        number_stream<<' '<<num<<" |";
        elem_stream<<' '<<elem<<" |";
    }
    else
    {
        space(1+abs(differences)/2+abs(differences)%2, number_stream);
        number_stream<<num;
        space(1+abs(differences)/2, number_stream);
        number_stream<<'|';
        elem_stream<<' '<<elem<<" |";
    }
}

#if defined LINUX && !defined LINUXV2
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
#ifdef LINUXV2
    system("stty raw");
    ch = getchar();
    system("stty cooked");
    cin.clear();
#else
    if (echo)
        ch = getche();
    else
        ch = getch();
#endif
#else
    ch = _getch();
#endif
    return ch;
}

int main()
{
    bool power = true;
    double* buf_double = nullptr;
    int dsize = 0;
#ifdef withsave
    int ndsize = 0;
#endif
    setlocale(LC_ALL, "ru");
    while (power)
    {
        char mode;
        cout << "\n1) Создать исходные данные\n"
            << "2) Считать данные\n"
            << "3) Вывести исходные данные на экран\n"
            << "4) Обработать данные по алгоритму функции\n"
            << "5) Завершиен работы\n"
#ifdef critical
            << "6) Критический режим считывания (при появлении ошибки программа игнорирует неверное значение)\n"
#endif
            << "Выберите режим работы: ";
        mode = hotkey_to_char(true);
        cout << '\n';
        int pos = 0;
        double dtemp = 0, S1 = 0, S2 = 0, Smax = 0;
        ostringstream numberstr("Номера  |", ostringstream::ate), elementstr("Элемены |", ostringstream::ate);
#ifdef critical 
            errlog(ostringstream::ate),
#endif
            

#ifdef SSTREAM
        string str;
        istringstream sstr;
#endif
        string tempstr, FileAdress;
        fstream file;
        char check;
        char errtype{ 0 };
        switch (mode)
        {
        case '1':
            dsize = 0;
            while (true)
            {
                cout << "\nВведите количество элементнов которое вы хотите добавить(чтобы вернуться *): ";
                if (!(cin >> dsize) || (cin.peek() != '\n') || (dsize <= 0))
                {
                    cin.clear();
                    getline(cin, tempstr);
                    if (!dsize && tempstr == "*")
                        break;
                    else
                    {
                        cout << "Введено недопустимое значение";
#ifdef LINUX
                        cin.sync_with_stdio(false);
#endif
                        cin.ignore(cin.rdbuf()->in_avail(), '\n');
#ifdef LINUX
                        cin.sync_with_stdio(true);
#endif
                        continue;
                    }
                }
                cout << "+++ dsize=" << dsize;
                buf_double = new(nothrow) d_arr[dsize];
                if (buf_double == nullptr)
                {
                    cout << "\nОшибка выделения оперативной памяти\n";
                    continue;
                }

                cout << "\nВведите значения для элементов(чтобы вернуться *):\n";
                for (int i = 0; i < dsize; i++)
                {
                    cout << "\tЭлемент " << (i + 1) << " = ";
                    if (!(cin >> buf_double[i]) || (cin.peek() != '\n'))
                    {
                        cin.clear();
                        cout << "---";
                        if (!dsize && cin.get() == '*' && cin.peek() == '\n')
                        {
#ifdef withsave
                            cout << "Сохранить введенные данные?[Y/n]";
#else
                            cout << "Полученные данные будут утеряны";
#endif
                        }
                        else
                        {
                            cout << "Введено недопустимое значение!\n";
#ifdef LINUX
                            cin.sync_with_stdio(false);
#endif
                            cin.ignore(cin.rdbuf()->in_avail(), '\n');
#ifdef LINUX
                            cin.sync_with_stdio(true);
#endif
                            i--;
                            continue;
                        }
                    }
                }
                cin.ignore();
                cin.clear();
                cout << "Введите имя файла в который будут сохранены полученные данные(чтобы не сохранять*): ";
                getline(cin, FileAdress);
                if (FileAdress != "*")
                {
                    bool file_write = true;
                    file.open(FileAdress, ios_base::in);
                    if (file.is_open())
                    {
                        file.close();
                        char choice;
                        while (true)
                        {
                            cout << "\nФайл с таким названием уже существует, вы хотите\n"
                                << "1)Перезаписать файл\n"
                                << "2)Вернуться назад\n";
                            choice = hotkey_to_char(false);
                            if (choice == '1' || choice == '2')
                            {
                                if (choice == '2')
                                    file_write = false;
                                break;
                            }
                            else
                                cout << "\nВведено неверное значение!\n";
                        }
                    }
                    if (file_write)
                    {
                        file.open(FileAdress, ios_base::out | ios_base::trunc | ios_base::binary);
                        file.write((char*)buf_double, sizeof(d_arr));
                        file.close();
                        break;
                    }
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
                //file.open(FileAdress, ios_base::in);
                file.open(FileAdress, ios_base::in | ios_base::binary);
                if (!(file.is_open()))
                {
                    perror(("\nОшибка открытия файла с именем " + FileAdress).c_str());
                    continue;
                }
                dsize = 0;
#ifndef filesize
                while (!(file >> ws).eof() && !errtype)
                {
                    pos = file.tellg();
#ifdef SSTREAM
                    getline(file, str)
                        sstr.str(str);
                    sstr.clear();
                    while (!sstr.eof())
                        sstr >> dtemp;
                    if (sstr.rdstate() != ios::eofbit)
#else
                    if (!(file >> dtemp) || ((check = file.peek()) != ' ' && check != '\n'
                        && check != '\t' && check != EOF))
#endif
                    {
                        errtype = 'f';
                        break;
                    }
                    else
                        dsize++;
                }
#else
                file.seekg(0, ios::end)
                    dsize = file.tellg();
                if (dsize % sizeof(d_arr))
                    file.close();
#endif          
                if (errtype == 'f')
                {
                    file.clear();
                    file.seekg(pos);
                    file >> tempstr;
                    file.close();
                    cout << "\nНайдена ошибка в элементе номер " << dsize
                        << "\nАбсолютная позиция в файле " << pos
                        << "\nНеверное значение: " << tempstr << '\n';
                    break;
                }
                if (!dsize)
                {
                    file.close();
                    cout << "В файле не оказалось значений, которые можно считать\n";
                    continue;
                }
                buf_double = new(nothrow) d_arr[dsize];
                if (buf_double == nullptr)
                {
                    file.close();
                    cout << "\nОшибка выделения оперативной памяти\n";
                    break;
                }
                file.seekg(0);
                for (int i = 0; i < dsize; i++)
                {
                    file.read((char*)(buf_double + i), sizeof(d_arr));
                }
                file.close();
                break;
            }
        case '3':
            if (!buf_double)
            {
                cout << "\nНа данный момент в памяти еще нет никаких данных\n";
                break;
            }
            for (int i = 0; i < dsize; i++)
                assembly_stream(i + 1, buf_double[i], numberstr, elementstr);
            cout << numberstr.str() << '\n' << elementstr.str() << '\n' << '\n';
            break;
        case '4':
            if (!buf_double)
            {
                cout << "\nНа данный момент в памяти еще нет никаких данных\n";
                break;
            }
            for (int i = 0; i < dsize; i++)
            {
                S1 += buf_double[i] / pow(i + 1, 2);
                if (i > 3)
                    S2 += pow(buf_double[i], 2);
            }
            Smax = S1 > S2 ? S1 : S2;
            cout << "\nРезультат работы Smax = " << Smax << '\n';
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
    buf_double = nullptr;
    return 0;
}
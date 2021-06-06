#define LINUX
#define filesize_v2
//#define filesize_v1
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#define hash_v2

#include <boost/crc.hpp>
#include <boost/cstdint.hpp>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits>
#include <cerrno>
#include <experimental/filesystem>

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
size_t TextVievSize(Xtype a, ios::fmtflags f = ios_base::dec, int prec = 6)
{
    return ((ostringstream&)(ostringstream() << resetiosflags(ios_base::basefield) << setiosflags(f) << setprecision(prec) << a)).str().size();
}

void space(int needed, ostringstream& ostr)
{
    ostr << setw(needed) << setfill(' ') << "";
}
void assembly_stream(int num, const double& elem, ostringstream& number_stream, ostringstream& elem_stream)
{
    int differences = TextVievSize(num) - TextVievSize(elem);
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

bool HashCheck(void const* buffer, size_t BUFFER_SIZE, size_t& HASH_SIZE, char* (&ChBUFFER))
{
    boost::crc_basic<16> crc(0x3D65u, 0u, 0u, true, true);
    crc.process_bytes(buffer, BUFFER_SIZE);
    unsigned short hash1 = crc.checksum();
    crc = boost::crc_basic<16>(0x8408u, crc.get_initial_remainder(),
        crc.get_final_xor_value(), crc.get_reflect_input(),
        crc.get_reflect_remainder());
    crc.process_bytes(buffer, BUFFER_SIZE);
    unsigned short hash2 = crc.checksum();
    HASH_SIZE = TextVievSize(hash1) + TextVievSize(hash2);
    ChBUFFER = new(nothrow) char[HASH_SIZE+1]; ////////+1 для char всегда нужно писать, запомнить
    if (!ChBUFFER)
    {
        cout << "\nНе удалось выделить память для проверки hash-функцией\n";
        return false;
    }
    _itoa(hash1, ChBUFFER, 10);
    _itoa(hash2, ChBUFFER + TextVievSize(hash1), 10);
    return true;
}
bool WriteHash(fstream& CRCHashFile, const string& FileAdress, const d_arr* buf_double, const int& dsize)
{
    size_t HASH_SIZE;
    CRCHashFile.open(FileAdress + ".crc", ios::out | ios::binary);
    if (!CRCHashFile.is_open())
    {
        cout << "\nНе удалось сохранить хэш файла, "; //////////я не стал сильно усложнять с ручным вводом имени файла или еще чем-либо
        return false;                                 //////////скорее всего у пользователя нет прав и могу еще об этом дописать или может 
    }                                                 //////////нет прав на чтение, пока ничего не буду дописывать
    char* hash = nullptr;                                               //char* hash = HashCheck(buf_double, dsize * sizeof(d_arr), HASH_SIZE);
    HashCheck(buf_double, dsize * sizeof(d_arr), HASH_SIZE, hash);      //CRCHashFile.write(HashCheck(buf_double, dsize * sizeof(d_arr), HASH_SIZE), HASH_SIZE);
    CRCHashFile.write(hash, HASH_SIZE);
    CRCHashFile.close();
    delete[] hash;
    return true;
}

#ifndef hash_v2
bool ReWriteHash(const char *hash)
{
    CRCHashFile.close();
    delete[] hash;
    cout << "\nОшибка, нарушена целостность данных в файле\n";
    return false;
}
#else
bool ReWriteHash(fstream & CRCHashFile, const d_arr * buf_double, const string& FileAdress, const int& dsize, char& mode, const char* hash, const int& HASH_SIZE, ostringstream & numberstr, ostringstream & elementstr)
{
    cout << "\nВнимание, могли произойти ошибки при считывании\n\n";
    for (int i = 0; i < dsize; i++)
        assembly_stream(i + 1, buf_double[i], numberstr, elementstr);
    cout << numberstr.str() << '\n' << elementstr.str() << '\n' << '\n';

    char choice;
    while (true)
    {
        cout << "Выберите действие:\n"
            << "1)На экран выведены корректные данные\n"
            << "2)На экран выведены некорректные данные" << endl;
        choice = hotkey_to_char(false);
        if (choice == '1' || choice == '2')
            break;
        cout << "\nВведено неверное значение!\n";
    }
    if (choice == '2')
    {
        cout << "\nОшибка, нарушена целостность данных в файле\n\n"; //////сильно не выдумывал что дальше
        CRCHashFile.close();
        delete[] hash;
        return false;
    }
    else
    {
        CRCHashFile.open(FileAdress + ".crc", ios::out | ios::binary);
        CRCHashFile.write(hash, HASH_SIZE);
        CRCHashFile.close();
        delete[] hash;
        //mode = '3';          /////заранее был вывод из 3 пункта меню, без передачи управления, нет смысла снова выводит таблицу 
        term_clear_screen();
        return true;
    }
}
#endif

int main()
{
    bool power = true, clearterm = false;
    d_arr* buf_double = nullptr;
    int dsize;
    char mode = '0';
    string FileAdress;
    setlocale(LC_ALL, "ru");
    while (power)
    {

        if (clearterm || FileAdress == "*")
        {
            clearterm = false;
            FileAdress.clear();
            term_clear_screen();
        }
        if ((mode == '1' || mode == '2') && buf_double)
            mode = '3';
        else
        {
            cout << "1) Создать исходные данные\n"
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
        fstream file, CRCHashFile;

        switch (mode)
        {
        case '1':
            while (true)
            {
                cout << "Введите количество элементнов, которое вы хотите добавить (чтобы вернуться в главное меню, нажмите *): ";
                cin.clear();
#ifdef LINUX
                std::cin.sync_with_stdio(false);
#endif
                cin.ignore(cin.rdbuf()->in_avail(), '\n');
                if (cin.peek() == '*' && cin.rdbuf()->in_avail() == 2)
                {
                    clearterm = true;
                    break;
                }
#ifdef LINUX
                std::cin.sync_with_stdio(true);
#endif
                cin >> dsize;
                if ((cin.peek() != '\n') || (dsize <= 0))
                {
                    cout << "Ошибка, введено недопустимое значение\n";
                    continue;
                }
                if (buf_double)
                {
                    delete[] buf_double;
                }

                buf_double = new(nothrow) d_arr[dsize];
                if (buf_double == nullptr)
                {
                    cout << "\nОшибка выделения оперативной памяти\n";
                    continue;
                }

                cout << "\nВведите значения для элементов (чтобы вернуться в главное меню, нажмите *):\n";
                for (int i = 0; i < dsize;)
                {
                    cout << "\tЭлемент " << (i + 1) << " = ";
                    cin.clear();
#ifdef LINUX
                    std::cin.sync_with_stdio(false);
#endif
                    cin.ignore(cin.rdbuf()->in_avail(), '\n');
                    if (cin.peek() == '*' && cin.rdbuf()->in_avail() == 2)
                    {                                                       //cout << "\ncin peek = " << cin.peek() << " elem in cin " << cin.rdbuf()->in_avail() << '\n';
                        clearterm = true;
                        break;
                    }
#ifdef LINUX
                    std::cin.sync_with_stdio(true);
#endif              
                    cin >> buf_double[i];
                    if (cin.peek() != '\n')
                    {
                        cout << "Ошибка, введено недопустимое значение\n";
                        continue;
                    }
                    i++;
                }
                if (clearterm)
                {
                    delete[] buf_double;
                    buf_double = nullptr;
                    break;
                }

                cin.ignore();
                while (true)
                {
                    cout << "\nВведите имя файла, в который будут сохранены полученные данные (чтобы не сохранять, нажмите *): ";
                    getline(cin, FileAdress);
                    if (FileAdress == "*")
                        break;
                    file.open(FileAdress, ios_base::in);
                    if (file.is_open())
                    {
                        file.close();
                        char choice;
                        while (true)
                        {
                            cout << "\nФайл с именем \"" << FileAdress << "\" уже существует, вы хотите:\n"
                                << "1)Перезаписать файл\n"
                                << "2)Указать новое имя файла\n"
                                << "3)Продолжить выполение программы без сохранения" << endl;
                            choice = hotkey_to_char(false);
                            if (choice == '1' || choice == '2' || choice == '3')
                                break;
                            cout << "\nВведено неверное значение!\n";
                        }
                        if (choice == '2')
                            continue;
                        if (choice == '3')
                            break;
                    }

                    file.open(FileAdress, ios_base::out | ios_base::binary);
                    if (!file.is_open())
                    {
                        perror(("\nОшибка открытия файла с именем \"" + FileAdress + "\"").c_str());
                        continue;
                    }
                    file.write((char*)buf_double, sizeof(d_arr) * dsize);       //for (int i = 0; i < dsize; i++)
                    file.close();                                               //file.write((char*)(buf_double + i), sizeof(d_arr));

                    if (!(WriteHash(CRCHashFile, FileAdress, buf_double, dsize)))
                        break;                              ///////можно continue, если есть файл с именем хэш-файла, но нет прав на запись, 
                    break;                                  ///////и тогда нужно попросить новое имя или результат ошибки в булевскую переменную и дальше что-то еще далать
                }
                break;
            }
            break;
        case '2':
            while (true)
            {
#ifdef LINUX
                std::cin.sync_with_stdio(false);
#endif
                cin.ignore(cin.rdbuf()->in_avail(), '\n');
#ifdef LINUX
                std::cin.sync_with_stdio(true);
#endif
                cout << "Введите имя файла (чтобы вернуться в главное меню *): " << endl;
                getline(cin, FileAdress);
                if (FileAdress == "*")
                    break;

                file.open(FileAdress, ios_base::in | ios_base::binary);
                if (!(file.is_open()))
                {
                    perror(("\nОшибка открытия файла с именем \"" + FileAdress + "\"").c_str());
                    continue;
                }
                /*if (buf_double)            этот вариант, если необходимо как можно эффективнее работать с памятью,
                {                            тут можно подумать так, если пользователь ввел ВЕРНОЕ имя, значит он
                    delete[] buf_double;     намеревается начать работу с новым файлом, и старый массив данных ему 
                }*/                        //уже не нужен, но нормально ли открылся файл, уже не так важно, т.к. старый отработан и все
#ifndef filesize_v2
#ifndef filesize_v1
                file.seekg(0, ios::end);
                dsize = file.tellg();
#else
                file.seekg(0);
                file.ignore(INT32_MAX, EOF);
                dsize = file.gcount();
#endif
#else           
                dsize = experimental::filesystem::file_size(FileAdress);            
#endif
                if (dsize % sizeof(d_arr))
                {
                    file.close();
                    cout << "\nОшибка считывания, нарушена целостность данных\n";
                    continue;
                }
                dsize /= sizeof(d_arr);

                if (!dsize)
                {
                    file.close();
                    cout << "\nВ файле не оказалось значений, которые можно считать\n";
                    continue;
                }                           // этот вариант подходит, если пользователь думал, что в файле есть данные
                if (buf_double)             // и хорошо бы их получить, но раз файл пустой или нарушена целостность,
                {                           // можно оставить массив в памяти, например, в нем хранились бы номера файлов 
                    delete[] buf_double;    // или еще какая-либо, информация к которой после отработки
                }                           // загруженного файла, все равно бы пришлось обратиться
                buf_double = new(nothrow) d_arr[dsize];
                if (buf_double == nullptr)
                {
                    file.close();
                    cout << "\nОшибка выделения оперативной памяти\n";
                    continue;
                }
                file.seekg(0);                                                  //file.clear();
                file.read((char*)buf_double, sizeof(d_arr) * dsize);            //for (int i = 0; i < dsize; i++)
                file.close();                                                   //file.read((char*)(buf_double + i), sizeof(d_arr));
                
                CRCHashFile.open(FileAdress + ".crc", ios::in | ios::binary);
                if (!CRCHashFile.is_open())
                {
                    CRCHashFile.close();
                    if (!WriteHash(CRCHashFile, FileAdress, buf_double, dsize))
                        break; ///////можно и continue, но для работы программы это не критично
                }
                else
                {
                    size_t HASH_SIZE_old, HASH_SIZE;
                    HASH_SIZE_old = experimental::filesystem::file_size(FileAdress + ".crc");
                    char* hash = nullptr;                                   //char* hash = HashCheck(buf_double, dsize * sizeof(d_arr), HASH_SIZE);
                    HashCheck(buf_double, dsize * sizeof(d_arr), HASH_SIZE, hash); ///////получаю еще и массив до сравнения, чтобы снова не вызывать функцию
                    if (HASH_SIZE != HASH_SIZE_old)
                    {
                        CRCHashFile.close();
                        if (ReWriteHash(CRCHashFile, buf_double, FileAdress, dsize, mode, hash, HASH_SIZE, numberstr, elementstr))
                            break;
                        else
                            continue;
                    }
                    bool hash_err = false;
                    char* hash_old = new char[HASH_SIZE_old+1];
                    for(int i=0; i<HASH_SIZE; i++)
                        CRCHashFile.read((hash_old + i), sizeof(char));          ////CRCHashFile.read(hash_old, HASH_SIZE_old); /////не подходит из-за иероглифов которые объединяют 2 байта в 1 символ
                    CRCHashFile.close();
                    for (int i = 0; i < HASH_SIZE && !hash_err; i++)
                    {
                        if (hash[i] != hash_old[i])
                        {
                            if (ReWriteHash(CRCHashFile, buf_double, FileAdress, dsize, mode, hash, HASH_SIZE, numberstr, elementstr))
                                break;
                            else
                            {
                                hash_err = true;
                            }
                        }
                        //if (HASH_SIZE - i == 1) ////////////тут можно раскоментить и проверить, все работает
                            //cout << "\nхэши совпадают\n";
                    }
                    delete[] hash_old;
                    if (hash_err)
                        continue;
                }
                break;
            }
            break;
        case '3':
            if (!buf_double)
            {
                cout << "На данный момент в памяти еще нет никаких данных, для их получения выберите действие 1 или 2\n\n";
                break;
            }
            for (int i = 0; i < dsize; i++)
                assembly_stream(i + 1, buf_double[i], numberstr, elementstr);
            cout << numberstr.str() << '\n' << elementstr.str() << '\n' << '\n';
            break;
        case '4':
            if (!buf_double)
            {
                cout << "На данный момент в памяти еще нет никаких данных, для их получения выберите действие 1 или 2\n\n";
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
            cout << "Введено неверное значение!\n\n";
        }
    }
    if (buf_double != nullptr)
        delete[] buf_double;
    return 0;
}
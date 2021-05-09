//#define SSTREAM
#define LINUX
//#define LINUXV2
#define critical

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
#include <conio.h>
#endif

using namespace std;

template <typename Xtype>
Xtype ElemSize(Xtype a)
{
    ostringstream tmp;
    tmp<<a;
    return tmp.str().size();
}
template <typename Xtype>
Xtype ElemSize(Xtype a, ios::fmtflags f)
{
    ostringstream tmp;
    tmp<<setiosflags(f)<<a;
    return tmp.str().size();
}
template <typename Xtype>
Xtype ElemSize(Xtype a, ios::fmtflags f, int prec)
{
    ostringstream tmp;
    tmp<<setiosflags(f)<<setprecision(prec)<<a;
    return tmp.str().size();
}

string space(int needed)
{
    string sp;
    while (needed-sp.size())
        sp += ' ';
    return sp;
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
char hotkey_to_char()
{
    char ch;
#ifdef LINUX
    #ifdef LINUXV2
    system("stty raw");
    ch= getchar();
    system("stty cooked");
    cin.clear();
    #else
    ch=getche();
    #endif
#else
    ch=_getch();
#endif
    return ch;
}
bool confirm_action(char not_default_solution)
{
    char ch;
    cin.sync_with_stdio(false);
    while(true)
    {
        cin >> ch;
        if((ch=='y'|| ch=='Y'|| ch=='n'|| ch=='N') && (cin.peek()=='\n'))
            break;
        cout<<"\nНеверное значение, попробуйте снова: ";
        cin.ignore(cin.rdbuf()->in_avail(), '\n');
    }
    cin.sync_with_stdio(true);
    if(ch==not_default_solution || ch==tolower(not_default_solution))
        return true;
    else
        return false;
}
void failbit_func(char &err)
{
    err='f';
}
void goodbit_func()
{
    cout<<'+';
}

int main()
{
    bool power = true;
    double *buf=nullptr;
    setlocale(LC_ALL, "ru");
    while(power)
    {
        char mode;
        cout<<"\n1) Создать исходные данные\n"
            <<"2) Нормальный режим считывания (при появлении ошибки программа остановит свою работу)\n"
            <<"3) Вывести исходные данные на экран\n"
            <<"4) Обработать данные по алгоритму функции\n"
            <<"5) Завершиен работы\n"
#ifdef critical
            <<"6) Критический режим считывания (при появлении ошибки программа игнорирует неверное значение)\n"
#endif
            <<"Выберите режим работы: ";
        mode=hotkey_to_char();
        cout<<'\n';
        int pos=0, dsize=0;
        double dtemp=0, S1=0, S2=0, Smax=0;
        ostringstream numberstr("Номера  |",ostringstream::ate), 
                      elementstr("Элемены |",ostringstream::ate),
                      resultstr(ostringstream::ate),
                      errlog(ostringstream::ate);
#ifdef SSTREAM
        string str;
        istringstream sstr;
#endif
        string tempstr, FileAdress;
        fstream file;
        char check;
        char errtype{0};
        switch (mode)
        {
        case '2':
            while(true)
            {
                cout<<"Введите имя файла: ";
                getline(cin, FileAdress);
                cout<<FileAdress<<'\n';
                file.open(FileAdress);
                if(file.is_open())
                    break;
                perror("Ошибка открытия файла");
            }
            cout<<"111\n";
            while(!(file>>ws).eof()&&!errtype)
            {
                pos=file.tellg();
#ifdef SSTREAM
                getline(file,str
                sstr.str(str);
                sstr.clear();
                sstr>>dtemp;
                if(sstr.rdstate()!=ios::eofbit)
#else
                if (!(file>>dtemp) || ((check=file.peek())!=' ' && check!='\n'
                && check!='\t' && check!=EOF))
#endif
                    failbit_func(errtype);
                else
                    goodbit_func();
            }
            if(errtype)
            {   
                if(errtype=='f')
                {
                    file.clear();
                    file.seekg(pos);
                    file >> tempstr;
                    cout<<"\nНайдена ошибка в элементе номер "<<dsize
                        <<"\nАбсолютная позиция в файле "<<pos
                        <<"\nНеверное значение: "<<tempstr;
                }
                if(errtype=='b')
                {
                    cout<<"Ошибка выделения оперативной памяти";
                }
                file.close();
            }
            break;
        case '5':
            power=false;
            break;
        default:
            cout<<"Введено неверное значение!\n";
        }
    }
    if(buf!=nullptr)
        delete[] buf;
    buf=nullptr;
    
    return 0; 
}

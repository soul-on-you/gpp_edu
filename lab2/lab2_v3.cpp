//#define SSTREAM
#define LINUX
//#define LINUXV2
//#define critical
//#define Binary
//#define filesize

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

typedef double d_arr;
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
        number_stream<<space(1)<<num<<space(1)<<"|";
        elem_stream<<space(1+differences/2+differences%2)<<elem<<space(1+differences/2)<<"|";
    }
    else if(differences==0)
    {
        number_stream<<space(1)<<num<<space(1)<<"|";
        elem_stream<<space(1)<<elem<<space(1)<<"|";
    }
    else
    {
        number_stream<<space(1+abs(differences)/2+abs(differences)%2)<<num<<space(1+abs(differences)/2)<<"|";
        elem_stream<<space(1)<<elem<<space(1)<<"|";
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
    ch= getchar();
    system("stty cooked");
    cin.clear();
    #else
    if(echo)
        ch=getche();
    else
        ch=getch();
    #endif
#else
    ch=_getch();
#endif
    return ch;
}
/*bool confirm_action(char not_default_solution)
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
}*/

void failbit_func(char &err)
{
    err='f';
}
void goodbit_func(int &size)
{
    size++;
}

int main()
{
    bool power = true;
    double *buf_double=nullptr;
    int dsize=0;
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
        mode=hotkey_to_char(true);
        cout<<'\n';
        int pos=0;
        double dtemp=0, S1=0, S2=0, Smax=0;
        ostringstream numberstr("Номера  |",ostringstream::ate),
#ifdef critical 
                      errlog(ostringstream::ate),
#endif
                      elementstr("Элемены |",ostringstream::ate);
                      
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
        case '1':
            while(true)
            {   
                bool file_write = true;
                cout<<"\nВведите имя файла(чтобы вернуться *): ";
                getline(cin, FileAdress);
                if(FileAdress=="*")
                    break;
                file.open(FileAdress, ios_base::in);
                if(file.is_open())
                {
                    file.close();
                    char choice;
                    while(true)
                    {
                        cout<<"\nФайл с таким названием уже существует, вы хотите\n"
                            <<"1)Перезаписать файл\n"
                            <<"2)Вернуться назад\n";
                        choice=hotkey_to_char(false);
                        if(choice=='1' || choice=='2')
                        {
                            if(choice=='2')
                                file_write=false;
                            break;
                        }
                        else
                            cout<<"\nВведено неверное значение!\n";
                    }
                }
                if(file_write)
                {
                    file.open(FileAdress, ios_base::out | ios_base::trunc);
                    //file.open(FileAdress, ios_base::out | ios_base::trunc | ios_base::binary)
                    cout<<"\nДля завершения записи нажмите *:\n";
                    stringstream received_for_writting;
                    char temp_elem;
                    while(true)
                    {
                        temp_elem=hotkey_to_char(true);
                        if(temp_elem=='*')
                            break;
                        received_for_writting<<temp_elem;
                    }
                    cout<<'\n';
#ifdef Binary
                    file.write(received_for_writting.str(), received_for_writting.str().size());
#else               
                    file<<received_for_writting.str()<<'\n';
#endif
                    file.close();
                    break;
                }
            }
            break;
        case '2':
            while(true)
            {   
                cout<<"Введите имя файла(чтобы вернуться *): ";
                getline(cin, FileAdress);
                if(FileAdress=="*")
                    break;
                file.open(FileAdress, ios_base::in);
                //file.open(FileAdress, ios_base::in | ios_base::binary);
                if(!(file.is_open()))
                {   
                    perror("Ошибка открытия файла");
                    continue;
                }
                dsize=0;
#ifndef filesize
                while(!(file>>ws).eof()&&!errtype)
                {
                    pos=file.tellg();
#ifdef SSTREAM
                    getline(file,str)
                    sstr.str(str);
                    sstr.clear();
                    while(!sstr.eof())
                    sstr>>dtemp;
                    if(sstr.rdstate()!=ios::eofbit)
#else
                    if (!(file>>dtemp) || ((check=file.peek())!=' ' && check!='\n'
                    && check!='\t' && check!=EOF))
#endif
                        failbit_func(errtype);
                    else
                        goodbit_func(dsize);
                }
#else
                file.seekg(ios::end)
                dsize=file.tellg();
                if(dsize%sizeof(d_arr))
                    file.close();                
#endif          
                buf_double=new(nothrow) d_arr[dsize];
                if(buf_double==nullptr)
                    errtype='b';
                if(errtype)
                {   
                    if(errtype=='f')
                    {
                        file.clear();
                        file.seekg(pos);
                        file >> tempstr;
                        cout<<"\nНайдена ошибка в элементе номер "<<dsize
                            <<"\nАбсолютная позиция в файле "<<pos
                            <<"\nНеверное значение: "<<tempstr<<'\n';
                    }
                    if(errtype=='b')
                    {
                        cout<<"\nОшибка выделения оперативной памяти\n";
                    }
                    file.close();
                    if(buf_double)
                    {
                        delete[] buf_double;
                        buf_double=nullptr;
                    }
                    dsize=0;
                    break;
                }

                file.seekg(ios::beg);
                cout<<'\n';
                for(int i=0; i<dsize; i++)
                {
                    file>>buf_double[i];
                    //cout<<' '<<buf_double[i];
                }
                file.close();
                break;
            }
        case '3':
            if(!buf_double)
                {
                    cout<<"На данный момент в памяти еще нет никаких данных\n";
                    break;
                }
            for(int i=0; i<dsize; i++)
                assembly_stream(i+1, buf_double[i], numberstr, elementstr);
            cout<<numberstr.str()<<'\n'<<elementstr.str()<<'\n'<<'\n';
            break;
        case '4':
            if(!buf_double)
            {
                cout<<"\nНа данный момент в памяти еще нет никаких данных\n";
                break;
            }
            for(int i=0; i<dsize; i++)
            {
                S1+=buf_double[i]/pow(i+1,2);
                if(i>3)
                    S2+=pow(buf_double[i],2);
            }
            Smax = S1>S2 ? S1:S2;
            cout<<"\nРезультат работы Smax = "<<Smax<<'\n';
            break;
        case '5':
            power=false;
            break;
        default:
            cout<<"Введено неверное значение!\n";
        }
    }
    if(buf_double!=nullptr)
        delete[] buf_double;
    buf_double=nullptr;
    return 0; 
}

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <limits>
#include <cerrno>

#define SSTREAM

using namespace std;
char hotkey_to_char();
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
void assembly_stream(int num, double elem, ostringstream &number_stream,
ostringstream &elem_stream, ostringstream &result_table_stream)
{
    int differences=ElemSize(num)-ElemSize(elem);
    if(number_stream.str().size()+abs(differences)>90)
    {
        result_table_stream<<number_stream.str()<<'\n'<<elem_stream.str()<<'\n'<<'\n';
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
double *re_reserve(const double &dtemp, int &size,double *buf, int &buf_error)
{
    double *temp;
    size++;
    //temp=new(nothrow) double[size];
    try
    {   
        temp= new double[size];
        if(buf!=nullptr)
        {
            for(int i=0; i<size-1; i++)
                temp[i]=buf[i];
        }
        temp[size-1]=dtemp;
    }
    catch(bad_alloc)
    {
        buf_error=2;
        temp=nullptr;
    }
    delete[] buf;       
    return temp;
}   
void failbit_func(int &err)
{
    err=1;
}
void goodbit_func(const double &dtemp, int &dsize, double *buf, int &err, ostringstream &numberstr,
ostringstream &elementstr, ostringstream &resultstr, double &S1, double &S2)
{
    buf=re_reserve(dtemp, dsize, buf, err);
    assembly_stream(dsize, dtemp, numberstr, elementstr,resultstr);
    S1+=dtemp/pow(dsize,2);
    if(dsize>4)
        S2+=pow(dtemp,2);
}

char hotkey_to_char()
{
    system("stty raw");
    char ch= getchar();
    system("stty cooked");
    cin.clear();
    return ch;
}
int main()
{
    setlocale(LC_ALL, "ru");
    while(true)
    {
        ifstream file;
        string FileAdress;
        while(true)
        {
            cout<<"Введите имя файла: ";
            getline(cin, FileAdress);
            file.open(FileAdress);
            if(file.is_open())
                break;
            cout<<"Ошибка открытия файла!\n";
        }
        char mode;
        cout<<"1) Нормальный режим считывания (при появлении ошибки программа остановит свою работу)\n"
            <<"2) Критический режим считывания (при появлении ошибки программа игнорирует неверное значение)\n"
            <<"3) Проверка целостности считаных данных\n"
            <<"4) Вывести исходные данные на экран\n"
            <<"5) Обработать данные по алгоритму функции\n"
            <<"6) еще что-то\n";
        cout<<"Выберите режим работы: ";
        mode=hotkey_to_char();
        cout<<"\n";
        int pos=0, dsize=0, errtype=0;
        double S1=0, S2=0, Smax=0;
        double dtemp=0;
        ostringstream numberstr("Номера  |",ostringstream::ate), 
                      elementstr("Элемены |",ostringstream::ate),
                      resultstr(ostringstream::ate),
                      errlog(ostringstream::ate);
#ifdef SSTREAM
        string str;
        istringstream sstr;
#endif
        string tempstr;
        char check;
        double *buf=nullptr;
        switch (mode)
        {
            case '2':
                while(!(file>>ws).eof()&&!errtype)
                {
                    pos=file.tellg();
#ifdef SSTREAM
                    file>>str;
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
                        goodbit_func(dtemp, dsize, buf, errtype, numberstr, elementstr, resultstr, S1, S2);
                }
            if(errtype==1)
            {
                file.clear();
                file.seekg(pos);
                file >> tempstr;
                cout<<"Найдена ошибка в элементе номер "<<dsize
                    <<"\nАбсолютная позиция в файле "<<pos
                    <<"\nНеверное значение: "<<tempstr;
                    break;
            }
            file.close();
            if(errtype==2)
            {
                cout<<"Ошибка выделения оперативной памяти";
                break;
            }
            cout<<'\n'<<resultstr.str()<<numberstr.str()<<'\n'<<elementstr.str()<<'\n';
            if(S1>=S2)
                Smax=S1;
            else
                Smax=S2;
            cout<<"\nМаксимальное значение Smax = "<<Smax<<'\n';
            break;
        case '6':
            for(int all_numbers=1;!(file>>ws).eof();all_numbers++)
            {
                pos=file.tellg();
                if (!(file >> dtemp) || ((check=file.peek())!=' ' && check!='\n'
                && check!='\t' && check!=EOF))
                {
                    if (file.eof())
                        break;
                    else
                    {
                    file.clear();
                    file.seekg(pos);
                    file >> tempstr;
                    errlog<<"Найдена ошибка в элементе номер "<<all_numbers<<
                            "\nАбсолютная позиция в файле "<<pos<<
                            "\nНеверное значение: "<<tempstr<<'\n';
                    }
                }
                else
                    goodbit_func(dtemp, dsize, buf, errtype, numberstr, elementstr, resultstr, S1, S2);
            }
            file.close();
            if(errtype==2)
            {
                errlog<<"Ошибка выделения оперативной памяти\n";
            }
            cout<<'\n'<<resultstr.str()<<numberstr.str()<<'\n'<<elementstr.str()<<'\n';
            if(S1>=S2)
                Smax=S1;
            else
                Smax=S2;
            cout<<"\nМаксимальное значение Smax = "<<Smax<<"\n"<<
            "Хотите получить лог об ошибках[y/N]: ";
            if(confirm_action('Y'))
            {
                if((errlog.str()).empty())
                    cout<<"\nПри обработке файла, ошибки не выявленны\n";
                else
                    cout<<'\n'<<errlog.str();
            }
            break;
        default:
            cout<<"Введено неверное значение!\n";
            break;
        }
        cout<<"\nХотети ввести новый файл[Y/n]: ";
        if(confirm_action('N'))
            break;
        delete[] buf;
        buf=nullptr;
        cin.ignore();
    }
    return 0; 
}
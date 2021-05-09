#define SSTREAM

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <limits>
#include <cerrno>

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
    double *buf_double=nullptr;
    setlocale(LC_ALL, "ru");
    while(true)
    {
        ifstream file;
        string FileAdress, tempstr;
        cout<<"\nВведите имя файла(чтобы выйти *): ";
        getline(cin, FileAdress);
        if(FileAdress=="*")
            break;
        file.open(FileAdress, ios_base::in);
        if(!(file.is_open()))
        {   
            perror("\nОшибка открытия файла");
            continue;
        }
        char mode, check, errtype{0};
        cout<<"\n";
        int pos=0, dsize=0;
        double S1=0, S2=0, Smax=0, dtemp=0;
        ostringstream numberstr("Номера  |",ostringstream::ate), 
                      elementstr("Элемены |",ostringstream::ate);
#ifdef SSTREAM
        string str;
        istringstream sstr;
#endif
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
                goodbit_func(dsize);
        }
        if(!dsize)
        {
            cout<<"Пустой файл\n";
            continue;
        }
        if(!errtype)
        {
            buf_double=new(nothrow) d_arr[dsize];
            if(buf_double==nullptr)
                errtype='b';
        }
        if(errtype)
        {   
            if(errtype=='f')
            {
                file.clear();
                file.seekg(pos);
                file >> tempstr;
                cout<<"Найдена ошибка в элементе номер "<<dsize
                    <<"\nАбсолютная позиция в файле "<<pos
                    <<"\nНеверное значение: \""<<tempstr<<'\"'<<'\n';
            }
            file.close();
            if(buf_double)
            {
                delete[] buf_double;
                buf_double=nullptr;
            }
            if(errtype=='b')
            {
                cout<<"Ошибка выделения оперативной памяти\n";
            }
            continue;
        }
        file.seekg(ios::beg);
        for(int i=0; i<dsize; i++)
            file>>buf_double[i];
        file.close();
        for(int i=0; i<dsize; i++)
        {
            S1+=buf_double[i]/pow(i+1,2);
            if(i>3)
                S2+=pow(buf_double[i],2);
            assembly_stream(i+1, buf_double[i], numberstr, elementstr);
        }
        Smax= S1>S2 ? S1:S2;
        cout<<numberstr.str()<<'\n'<<elementstr.str()
            <<"\n\nSmax = "<<Smax<<'\n';
        if(buf_double!=nullptr)
            delete[] buf_double;
        buf_double=nullptr;
    }
    return 0; 
}
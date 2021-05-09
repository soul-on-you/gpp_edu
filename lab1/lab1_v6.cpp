//#define SSTREAM
//#define withmanip
#define error_processing_v2

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
size_t ElemSize(Xtype a, ios::fmtflags f = ios_base::dec, int prec = 6)
{
    return ((ostringstream&)(ostringstream()<<resetiosflags(ios_base::basefield)<<setiosflags(f)<<setprecision(prec)<<a)).str().size();
}

#ifdef withmanip
ostream &space(ostream &mystream, int needed)
{
    return mystream<<setw(needed)<<setfill(' ')<<"";
}
omanip<int> space(int needed)
{
    return omanip<int>(space, needed);
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
#else
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
#endif

int main()
{
    double *buf_double;
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
            perror(("\nОшибка открытия файла с именем " + FileAdress).c_str());
            continue;
        }
        bool err=false;
        cout<<'\n';
        int pos=0, dsize=0;
        double dtemp=0;
        ostringstream numberstr("Номера  |",ostringstream::ate), 
                      elementstr("Элемены |",ostringstream::ate);
#ifdef SSTREAM
        istringstream sstr;
#endif
        while(!(file>>ws).eof())
        {
            pos=file.tellg();
#ifdef SSTREAM
            file>>tempstr;
            sstr.str(tempstr);
            sstr.clear();
            sstr>>dtemp;
            if(sstr.rdstate()!=ios::eofbit)
#else
            char check;
            if (!(file >> dtemp) || (file.peek()!= ' ' && file.peek()!= '\n'
                && file.peek() != '\t' && file.peek() != EOF))
#endif
                {
                    err=true;
                    break;
                }
            else
                dsize++;
        }
        if(err)
        {
#ifndef SSTREAM
            file.clear();
            file.seekg(pos);
            file >> tempstr;
#endif
            file.close();
            cout<<"Найдена ошибка в элементе номер "<<(dsize+1)
                <<"\nАбсолютная позиция в файле "<<pos
                <<"\nНеверное значение: \""<<tempstr<<'\"'<<'\n';
            continue;
        }
        if(!dsize)
        {
            file.close();
            cout<<"В файле не оказалось значений, которые можно считать\n";
            continue;
        }
#ifndef error_processing_v2
        buf_double=new(nothrow) d_arr[dsize];
#else
        try
        {   
            buf_double=new d_arr[dsize];
        }
        catch(bad_alloc)
        {
            buf_double=nullptr;
        }
#endif
        if(buf_double==nullptr)
        {
            file.close();
            cout<<"Ошибка выделения памяти/n";
            continue;
        }
        
        file.clear();
        file.seekg(0);
        for(int i=0; i<dsize; i++)
            file>>buf_double[i];
        file.close();
        
        double S1=0, S2=0, Smax=0;
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
        delete[] buf_double;
    }
    return 0; 
}
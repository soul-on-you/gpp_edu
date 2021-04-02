#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdio>
#include <cmath>
 
using namespace std;

template <typename Xtype>
Xtype ElemSize(Xtype a)
{
    ostringstream ostr;
    ostr << a;
    return ostr.str().size();
}
string space(int a)
{
    string sp;
    while (a)
    {
        sp += " ";
        a--;
    }
    return sp;
}
void assembly_stream(int num, double elem, ostringstream &number_stream, ostringstream &elem_stream)
{
    int differences=ElemSize(num)-ElemSize(elem);
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
    while(true)
    {
        cin >> ch;
        if((ch=='y'|| ch=='Y'|| ch=='n'|| ch=='N') && cin.peek()=='\n')
            break;
        cout<<"Неверное значение, попробуйте снова: ";
        cin.ignore(INT32_MAX, '\n');
    }
    if(ch==not_default_solution || ch==tolower(not_default_solution))
        return 1;
    else
        return 0;
}

int main()
{
    setlocale(LC_ALL, "ru");
    while(true)
    {
        ifstream file;
        string FileAdress;
        while(!(file.is_open()))
        {
            cout<<"Введите имя файла: ";
            getline(cin, FileAdress);
            file.open(FileAdress);
            if(!(file.is_open()))
                cout<<"Ошибка открытия файла!\n";
        }
        int mode=0;
        cout<<"1) Нормальный (при появлении ошибки программа остановит свою работу)\n";
        cout<<"2) Критический (при появлении ошибки программа игнорирует неверное значение)\n";
        cout<<"Выберите режим работы: ";
        cin>>mode;
        cout<<"\n";
        int pos=0, number=1, dsize=0;
        double S1=0, S2=0, Smax=0;
        double dtemp=0; 
        ostringstream numberstr("Номера  |",ostringstream::ate), 
                      elementstr("Элемены |",ostringstream::ate),
                      errlog(ostringstream::ate);
        string tempstr;
        char check;
        switch (mode)
        {
        case 1:
            while(!file.eof())
            {
                pos=file.tellg();
                if (!(file >> dtemp) || ((check=file.peek())!=' ' && check!='\n'
                && check!='\t' && check!=EOF))
                {
                    if (file.eof())
                    {   
                        cout<<"\n"<<numberstr.str()<<"\n"<<elementstr.str();
                        if(S1>=S2)
                            Smax=S1;
                        else
                            Smax=S2;
                        cout<<"\nМаксимальное значение Smax = "<<Smax<<"\n";
                        break;
                    }
                    else
                    {
                    file.clear();
                    file.seekg(pos);
                    file >> tempstr;
                    cout<<"Найдена ошибка в элементе номер "<<(number)<<
                    "\nАбсолютная позиция в файле "<<pos<<
                    "\nНеверное значение: "<<tempstr<<"\n";
                    break;
                    }
                }

                else 
                {
                    assembly_stream(number, dtemp, numberstr, elementstr);
                    S1+=dtemp/pow(number,2);
                    if(number>4)
                        S2+=pow(dtemp,2);
                    number++;
                }
            }
            break;
        case 2:
            for(int all_numbers=1;!file.eof();all_numbers++)
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
                            "\nНеверное значение: "<<tempstr<<"\n";
                    }
                }
                else 
                {
                    assembly_stream(number, dtemp, numberstr, elementstr);
                    S1+=dtemp/pow(number,2);
                    if(number>4)
                        S2+=pow(dtemp,2);
                    number++;
                }
            }
            cout<<"\n"<<numberstr.str()<<"\n"<<elementstr.str()<<"\n";
            if(S1>=S2)
                Smax=S1;
            else
                Smax=S2;
            cout<<"\nМаксимальное значение Smax = "<<Smax<<
            "\n\nХотите получить лог об ошибках[y/N]: ";
            if(confirm_action('Y'))
            {
                if((errlog.str()).empty())
                    cout<<"\nПри обработке файла, ошибки не выявленны\n";
                else
                    cout<<"\n"<<errlog.str();
            }
            break;
        default:
            cout<<"Введено неверное значение!\n";
            break;
        }
        file.close();
        cout<<"\nХотети ввести новый файл[Y/n]: ";
        if(confirm_action('N'))
            break;
        cin.ignore();
    }
    return 0; 
}
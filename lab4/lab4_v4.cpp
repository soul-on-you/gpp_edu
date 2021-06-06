#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits>
#include <cerrno>
#include <functional>

using namespace std;
typedef double Xtype;

/*struct MatrInfo
{
    Xtype** matr=NULL;
    int dsize_s=0, dsize_c=0;
    struct Err
    {
        char errtype=0;       //'f'-failbit  //'e'-empty file  //'o'-file isn't open
        union ErrInfo       //'q'-non-quadr_matr //'s'-string difference //'a'-fail alloc memory
        {
            int dsize_s=0;
            int dyn_elem;
            struct fbit
            {
                int dsize_s=0;
                int dsize_c=0;
                int pos=0;
            }fbit;
        }errinfo;
    }err;
};*/
struct MatrInfo
{
    Xtype** matr;
    int dsize_s, dsize_c;
    struct Err
    {
        char errtype;       //'f'-failbit  //'e'-empty file  //'o'-file isn't open
        union ErrInfo       //'q'-non-quadr_matr //'s'-string difference //'a'-fail alloc memory
        {
            int dsize_s;
            int dyn_elem;
            struct fbit
            {
                int dsize_s;
                int dsize_c;
                int pos;
            }fbit;
        }errinfo;
    }err;
};


void FReadMatr(ifstream& file, const MatrInfo &m_data) /////ifstream& file, Xtype** buf_a_arr, const int &bufstr, const int &bufcolomn
{
    int pos = file.tellg(); ///для чтения из случайного места: 1) сохраняем позицию
    file.seekg(0);
    for (int i = 0; i < m_data.dsize_s; i++)
        for (int j = 0; j < m_data.dsize_c; j++)
            file >> *(*(m_data.matr + i) + j);
    file.clear();
    file.seekg(pos); ///для чтения из случайного места: 2) возвращаем позицию
}

bool GetFileName(string &tmp)
{
    std::cin.sync_with_stdio(false);
    cout << "\nВведите имя файла(чтобы выйти *): ";
    std::cin.sync_with_stdio(true);
    if (cin.peek() == '*' && cin.rdbuf()->in_avail() == 2)
        return false;
    getline(cin, tmp);
    return true;
}

bool FileElementCorCheck(ifstream &file, MatrInfo &m_data) ////ErrInfo* err
{
    char tmp(0);
    Xtype Xtemp;
    size_t pos = file.tellg();
    if (!(file >> Xtemp) || (file.peek() != ' ' && file.peek() != '\n'
        && file.peek() != '\t' && file.peek() != EOF))
    {
        m_data.err.errinfo.fbit.pos=pos;
        m_data.err.errtype='f';
        m_data.err.errinfo.fbit.dsize_s=m_data.dsize_s;  ///////// В ОШИБКАХ МОЖНО ИСПОЛЬЗОВАТЬ САМ МАТР
        m_data.err.errinfo.fbit.dsize_c=m_data.dsize_c;  ///////// m_data.dsize_s и m_data.dsize_c
        return false;
    }
    return true;
}
bool CheckMatr(ifstream &file, MatrInfo &m_data)        ////int &dsize_s, int &dsize_c,  ErrInfo* err=nullptr
{
    int dsize_all=0;
    while (!(file >> ws).eof())   //////&& !err
    {
        if(!FileElementCorCheck(file, m_data))          //m_data.err.fbit.dsize_c=dsize_c; //m_data.err.fbit.dsize_s=dsize_s;
            return false;
        m_data.dsize_c++;
        char tmp(0);
        while (tmp != EOF)
        {
            tmp = file.get();
            switch (tmp)
            {
            case '\t': case ' ':
                break;
            case EOF: case '\n':
                if (m_data.dsize_c)
                {
                    m_data.dsize_s++;
                    dsize_all += m_data.dsize_c;
                    if (dsize_all % m_data.dsize_c)
                    {
                        m_data.err.errinfo.dsize_s = m_data.dsize_s;
                        m_data.err.errtype='s';
                        return false;
                    }
                    if (tmp != EOF)
                        m_data.dsize_c = 0;
                }
                break;
            default:
                file.unget();
                tmp = EOF;
            }
        }
    }
    if(m_data.dsize_s!=m_data.dsize_c)
    {
        m_data.err.errtype='q';
        return false;
    }
    return true;
}

bool CreateMatr(MatrInfo &m_data)    //////Xtype** matr, const int &str_in_matr, const int &column_in_matr, ErrInfo &err
{
    m_data.matr=new(nothrow) Xtype* [m_data.dsize_s];
    if(m_data.matr==nullptr)
    {
        m_data.err.errtype='a';
        m_data.err.errinfo.dyn_elem=0; /////под указатели но протестить закоментив
        return false;
    }
    for(int i=0; i<m_data.dsize_s; i++)
    {
        *(m_data.matr+i)=new(nothrow) Xtype[m_data.dsize_c];
        if(*(m_data.matr+i)==nullptr)
        {
            m_data.err.errtype='a';
            m_data.err.errinfo.dsize_s=(i+1);
            return false;
        }
    }
    return true;
}
void DeleteMatr(MatrInfo &m_data) ////Xtype** (&matr), const int &str_in_matr
{
    for(int i=0; i<m_data.dsize_s; i++)  ///////str_in_matr
        delete[] *(m_data.matr+i);
    delete[] m_data.matr;
    m_data.matr = nullptr;
}


//template <typename StreamType>
bool LoadMatr(function <bool(string&)> f_get_adress, MatrInfo &m_data)  ///function <void(StreamType&, Xtype**, const int&, const int&)>f_read
{                                             ///function <string()> f_get_adress, Xtype** matr, ifstream &file, ErrInfo* &err
    string FileAdress;
    if(!f_get_adress(FileAdress))
    {
        return false;
    }
    ifstream file(FileAdress, ios_base::in);
    if(file.is_open())
    {
        if(CheckMatr(file, m_data))  //////v2//m_data.dsize_s,m_data.dsize_c, &m_data.err
            if(CreateMatr(m_data)) /////m_data.matr, m_data.dsize_s, m_data.dsize_c, m_data.err
            {
                FReadMatr(file, m_data); //////
                file.close();
                return true;
            }
        file.close();
    }
    else
        m_data.err.errtype='o';
    return false;
}




template <typename N>
size_t TextVievSize(N a, ios::fmtflags f = ios_base::dec, int prec = 6)
{
    return ((ostringstream&)(ostringstream() << resetiosflags(ios_base::basefield) << setiosflags(f) << setprecision(prec) << a)).str().size();
}
void CoutMatr(const MatrInfo &m_data)         ////////Xtype** matr, const int& bufstr, const int& bufcolomn
{
    ostringstream numberstr("|   |", ostringstream::ate);
    ostringstream skipstr("+---+", ostringstream::ate);
    for (int j = 0; j <  m_data.dsize_c; j++)
    {
        skipstr << setw(12) << setfill('-') << '+';
        numberstr << setw(6 - TextVievSize(j) / 2 - TextVievSize(j) % 2) << "" << (j + 1) << setw(6 - TextVievSize(j) / 2) << '|';
    }
    cout << '\n' << skipstr.str() << '\n' << numberstr.str() << '\n' << skipstr.str() << '\n';
    for (int i = 0; i < m_data.dsize_s; i++)
    {
        cout << "| " << (i + 1) << " |";
        for (int j = 0; j <  m_data.dsize_c; j++)
        {
            cout << setw(10) << right << scientific << setprecision(2) << *(*(m_data.matr + i) + j) << " |";
        }
        cout << endl << skipstr.str() << '\n';
    }
}

void ErrCheck(const MatrInfo &m_data)
{
    switch (m_data.err.errtype)
    {
        case 'f':
            //string tmp;
            //file.clear();
            //file.seekg(pos);
            //file >> tmp;
            //file.close();
            cout << "Найдена ошибка в элементе номер " << (m_data.err.errinfo.fbit.dsize_c + 1) << " на строке " << (m_data.err.errinfo.fbit.dsize_s + 1)
                << "\nАбсолютная позиция в файле " << m_data.err.errinfo.fbit.pos
                //<< "\nНеверное значение: \"" << tmp << '\"' << '\n';
                << "\nФайл содержит некорректные значения\n";
            break;
        case 'e':
            cout << "\nВ файле не оказалось значений, которые можно считать\n";
            break;
        case 's':
            cout << "\nНе правильный формат, количество элементов строки "<<m_data.err.errinfo.dsize_s<<" не совпадает со стокой " <<(m_data.err.errinfo.dsize_s-1)<<'\n';
            break;
        case 'q':
            cout << "\nНе правильный формат, матрица не кваратная\n";
            break;
        case 'o':
            //perror(("\nОшибка открытия файла с именем \"" + FileAdress + "\"").c_str());
            break;
        case 'a':
            if(!m_data.err.errinfo.dsize_s)
                cout<<"\nОшибка, не удалось выделить память под массив указателей\n";
            else
                cout<<"\nОшибка, не удалось выделить память под "<<m_data.err.errinfo.dsize_s<<" строку\n";
            break;
        default:
            ///////Сделать выход из цикла
            break;
    }
}


void ResultFunc(MatrInfo &m_data)
{
    for (int i = 0, j = 0; i < m_data.dsize_s; i++, j++)
            if (*(*(m_data.matr + i) + j) < 0)
            {
                for (int k = 0; k < m_data.dsize_c; k++)
                {
                    if(k==j)
                        continue;
                    if (!(*(*m_data.matr + k)) && (*(*(m_data.matr + i) + k) > 0))
                    {
                        cout << "\nЗамена столбца " << (i + 1) << " на строку " << (k+1) << '\n';
                        Xtype Xtemp;
                        for (int l = 0; l < m_data.dsize_c; l++)
                        {
                            Xtemp = *(*(m_data.matr + i) + l);
                            *(*(m_data.matr + i) + l) = *(*(m_data.matr + l) + k);
                            *(*(m_data.matr + l) + k) = Xtemp;
                        }
                        break;
                    }
                    if (m_data.dsize_c - 1 == k)
                    {
                        cout << "\nНеудалось найти замену для " << (i+1) << " строки\n";
                    }
                }
               
            }
}
int main()
{
    MatrInfo m_data;
    setlocale(LC_ALL, "ru");
    while(true)
    {
        if(!LoadMatr(GetFileName, m_data))
        {
            ErrCheck(m_data);
        }
        CoutMatr(m_data);
        ResultFunc(m_data);
        CoutMatr(m_data);
    }
    if(m_data.matr!=nullptr)
    {
        DeleteMatr(m_data); ////p_buf_Xtype, pArrSIZE
    }
}
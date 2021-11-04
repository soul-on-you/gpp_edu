#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdio>
#include <cmath>

using namespace std;

int intsize(int a)
{
    int r;
    if (a > 0)
    {
        for (r = 0; a > 0; r++)
        {
            a = a / 10;
        }
    }
    else if (a < 0)
    {
        for (r = 0; a < 0; r++)
        {
            if (r == 0)
            {
                a = a + pow(10, r + 1);
            }
            else
            {
                a = a + pow(10, r + 1) - pow(10, r);
            }
        }
        r++;
    }
    else
    {
        r = 1;
    }
    return r;
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

int main()
{
    cout << setw(120) << setfill('=') << '\n';
    cout << "Enter .txt adress: ";
    fstream file;
    string adr;
    int confirm = 0;
    while (confirm == 0)
    {
        char Conf{0};
        const char spce(32);
        int correrctCH = 0;
        getline(cin, adr);
        cout << "Your adress is: " << adr << "\n";
        cout << "Confirm[Y/n]: ";
        cin >> Conf;
        if (Conf == 'N' || Conf == 'n')
        {
            while (correrctCH == 0)
            {
                cout << "Do you want to rewrite adress[Y/n]: ";
                cin >> Conf;
                if (Conf == 'N' || Conf == 'n')
                {
                    return 1;
                }
                else if (Conf == 'y' || Conf == 'Y')
                {
                    cout << "Enter new .txt adress: ";
                    correrctCH = 1;
                }
                else
                {
                    cout << "Use correct value!!!\t";
                }
            }
        }
        else if (Conf == 'Y' || Conf == 'y')
        {
            file.open(adr, fstream::app);
            if (!file)
            {
                cout << "Invalid file, maybe you lost adress, try again?[Y/n]: ";
                cin >> Conf;
                if (Conf == 'N' || Conf == 'n')
                {
                    return 1;
                }
                else
                {
                    cout << "Enter new .txt adress: ";
                }
            }
            else
            {
                cout << "File is open\n";
                file << spce;
                file.close();
                file.open(adr, fstream::in);
                confirm = 1;
            }
        }
        else
        {
            cout << "Use correct value!!!\t";
        }
    }
    cout << "\033[2J\033[H";

    int S1 = 0, S2 = 0, Smax = 0;
    int i = 0, n = 0, id = 0, chck = 0;
    int t = 0, err = 0, errOP = 0, backid = 0;
    int pstn = 0, r = 0, delta = 0;
    char check1{0}, check2{0};
    string str, fstr{"Elements: "};
    cout << "Numbers:  ";
    //ifstream copyF;
    //copyF.open(adr);
    ofstream outf;
    outf.open("dwjqjijdqwdklMDjipewoqjmsdkljopo21.txt", ios::trunc | ios::out);
    string estr = " ]Error!", eOP = "[ ";
    //file >> ws;
    for (i;; i++)
    {
        if (!(file >> t))
        {
            if (file.eof() || file.bad())
                break;
            else if (file.fail())
            {
                file.clear();
                char sim = file.get(); //закомментить
                //t = file.get();
                //char sim(t);
                //cout << "i=" << i << "\t" << "t=" << t << "\t" << "id=" << file.tellg() << "\t" << "char:" << sim << "\t";
                pstn = file.tellg();
                outf.seekp(pstn + (err * estr.size()) + (eOP.size() * errOP) - 1);
                if (chck == 0)
                {
                    outf << eOP << sim;
                    errOP++;
                    chck++;
                }
                else
                    outf << sim;
                backid = outf.tellp();
                //cout << "simID=" << pstn << "\n";
            }
        }
        else
        {
            id = file.tellg();
            r = intsize(t) + 1;
            outf.seekp(id - r + 1 + (err * estr.size()) + (errOP * eOP.size()));
            check1 = file.peek();
            file.seekg(id - r);
            check2 = file.peek();
            //cout << "i=" << i << "\t" << "t=" << t << "\t" << "id=" << id << "\t" << "r=" << r << "\t" << "outID=" << (id - r + 1) << "\t";
            //cout << "check:" << check << "\t";
            file.seekg(id);
            if ((check1 == '\t' || check1 == '\n' || check1 == ' ') && (check2 == '\t' || check2 == '\n' || check2 == ' '))
            {
                //cout << " t[" << n << "]=" << t << "\n";
                if ((r - 1) > intsize(n))
                {
                    delta = r - intsize(n);
                    if ((delta % 2) == 0)
                    {

                        //cout << " |" << setw((delta / 2)+1) << n << setw(delta / 2) << "|";
                        cout << " |" << space(((delta / 2) - 1)) << n << space((delta / 2)) << "|";
                    }
                    else
                    {
                        //cout << " |" << setw((delta / 2)+1) << n << setw((delta / 2) + 1) << "|";
                        cout << " |" << space((delta / 2)) << n << space((delta / 2)) << "|";
                    }
                    fstr += " |" + to_string(t) + "|";
                }
                else if ((r - 1) == intsize(n))
                {
                    cout << " |" << n << "|";
                    fstr += " |" + to_string(t) + "|";
                }
                else
                {
                    cout << " |" << n << "|";
                    delta = intsize(n) - r;
                    if ((delta % 2) == 0)
                    {
                        fstr += " |" + space(delta / 2) + to_string(t) + space((delta / 2) + 1) + "|";
                    }
                    else
                        fstr += " |" + space(delta / 2) + to_string(t) + space((delta / 2) + 1) + "|";
                }
                if (chck > 0)
                {
                    id = outf.tellp();
                    outf.seekp(backid);
                    outf << estr;
                    outf.seekp(id + estr.size());
                    err++;
                    chck = 0;
                }
                outf << t;
                n++;
                S1 += t / n;
                if (n > 3)
                {
                    S2 += pow(t, 2);
                }
            }
            else
            {
                outf << t;
                backid = outf.tellp();
                //cout << "error" << "\n";
            }
            //cout << "\n";
        }
    }
    cout << "\n"
         << fstr;
    if (S1 > S2)
    {
        Smax = S1;
    }
    else
    {
        Smax = S2;
    }
    cout << "\nSmax = " << Smax << "\n";
    file.close();
    outf.close();
    remove(adr.c_str());
    rename("dwjqjijdqwdklMDjipewoqjmsdkljopo21.txt", adr.c_str());
    cout << '\n';
    return 0;
}
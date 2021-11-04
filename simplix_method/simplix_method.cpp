#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class SimplixTable
{
    double **_matrix;
    double *_freeVar;
    int _freeVarSize;
    double *_bazisVar;
    int _bazisVarSize;

public:
    SimplixTable()
    {
        while (true)
        {
            string tmp;
            cin.clear();
            std::cin.sync_with_stdio(false);
            cin.ignore(cin.rdbuf()->in_avail());
            std::cin.sync_with_stdio(true);
            getline(cin, tmp);
            if (tmp == "*")
                break;
            istringstream ss(tmp);
            while (true)
            {
                if ((ss >> ws).eof())
                    break;
                char cTmp;
                cTmp = ss.peek();
                switch (cTmp)
                {
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '0':
                    ///
                    break;
                case '<':
                case '>':
                    break;
                case '=':
                    break;
                case '+':
                case '-':
                    break;
                default:
                    break;
                }
            }
        }
    };
};

int main()
{
    return 0;
}
#include <fstream>
#include <iostream>
using namespace std;

int main()
{
    // ofstream fout("file", ios::binary);
    // int N = 5;
    // for (int i = 0; i < N; ++i)
    // {
    //     fout.write((const char *)&i, sizeof(i));
    // }
    // fout.close();

    // fstream f("file", ios::in | ios::out | ios::binary);
    // if (!f)
    // {
    //     cout << "Error opening file!" << endl;
    //     system("pause");
    //     return 0;
    // }

    // // вывод из файла
    // for (int i = 0; i < N; ++i)
    // {
    //     int k;
    //     f.read((char *)&k, sizeof(k));
    //     cout << k << endl;
    // }
    // cout << endl;

    // вставка нового числа после k-го в файле
    // сдвиг чисел в файле, начиная с последнего и до k-го включительно
    // int k = 2;
    // int p = sizeof(int);
    // f.seekg(0, f.end);
    // for (int i = 0; i < N - k; ++i)
    // {
    //     f.seekp(-p, f.cur);
    //     int temp;
    //     f.read((char *)&temp, sizeof(temp));
    //     f.seekp(f.tellg());
    //     f.write((const char *)&temp, sizeof(temp));
    //     f.flush();
    //     p = 3 * sizeof(temp);
    // }

    // // вставка нового числа после k-го в файле
    // int m = 10; // новое числа
    // f.seekp(sizeof(int) * k, f.beg);
    // f.write((const char *)&m, sizeof(m));
    // f.flush();

    // // вывод из файла
    int N = 6;
    fstream f("file", ios::in | ios::out | ios::binary);
    f.seekp(0); // перезапись работает, крутой патерн
    int t = 12, t2 = 13;
    f.write(reinterpret_cast<char *>(&t), sizeof(int));
    f.write(reinterpret_cast<char *>(&t2), sizeof(int));
    f.flush();

    f.seekg(0, f.beg);
    for (int i = 0; i < N + 1; ++i)
    {
        int k;
        f.read((char *)&k, sizeof(k));
        cout << k << endl;
    }
    f.close();
}
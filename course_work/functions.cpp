#include <iostream>
#include <map>
#include <string>

template <typename X>
void coutmap(X x, std::string name)
{
    std::cout << name << ' ' << ':' << ' ';
    for (auto i : x)
        std::cout << i.first << ':' << i.second << ' ';
    std::cout << '\n';
}

int main()
{
    std::map<int, std::string> m = {{1, "one"}, {3, "three"}, {2, "two"}}; // инициализация от параметров при создании
    coutmap(m, "map");                                                     // вывод в функции i.first && i.second для 1 и 2 значений семейства подмножеств
    auto it5 = m.find(1);                                                  // поиск по ключу в паре в данном случае по int т.к. он в инициализации на месте ключа
    if (it5 != m.end())                                                    // если не находит то указатель доходит до конца => не нашел
        std::cout << "&it = " << &it5 << "  *it = " << it5->first << ':' << it5->second << '\n';
    else
        std::cout << "Not found \"" << 8 << "\"\n";
    it5->second = "chance one"; // можем по значению указателя ключа изменить его значениe
    //it5->first=13;  но не можем менять ключ
    m[3] = "new tree";                   // используя оператор [] можем изменять контейнеры
    m[5] = "skip four";                  // и создавать новые, которые не существовали
    auto ins1 = m.insert({6, "insert"}); // можем создавать новые через insert
    if (ins1.second)
        std::cout << "inseted\n";
    else
        std::cout << "already exist " << ins1.first->first << ':' << ins1.first->second << '\n';
    auto ins2 = m.insert({2, "fail"}); // но если с таким ключом уже есть то ничего новое не создаст
    if (ins2.second)
        std::cout << "inseted {6,\"insert\"} \n";
    else
        std::cout << "already exist " << ins2.first->first << ':' << ins2.first->second << '\n';
    coutmap(m, "map");
    std::cout << '\n'
              << '\n';
    return 0;
}

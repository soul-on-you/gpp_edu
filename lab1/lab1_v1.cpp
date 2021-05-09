#define NEW_THROW
//#define WITH_STREAM
#define APPEND_SIZE 5
 
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <cerrno>
 
/**
 * @brief Расширяет переданный массив или выделяет память.
 * @param arr Массив, который необходимо увеличить. В случае первычного выделения
 * памяти, указатель на массив должен быть инициализирован nullptr
 * @param size Текущее кол-во элементов в массиве. При удачном выделении памяти, будет
 * записано новое значение размера.
 * @param count На сколько элементов необходимо расширить массив.
 * @param delArrIfErr Удалять исходный массив если выделить память под новый не удалось.
 * @return Указатель на новый массив или nullptr если память выделить не удалось.
 *
 * Если в качестве исходного массива @a arr будет передан пустой указатель, то
 * будет создан новый массив размером size + count.
 */
int* resize(int* arr, int& size, int count, bool delArrIfErr = false);
 
/*
 @brief Выводит переданный массив на экран в виде таблицы
 @param arr Указатель на выводимый массив
 @param size Размер передаваемого массива
 @param str_len Длина строки таблицы в символах
 */
void printarr(int* arr, int size, int str_len);
 
/*
 @brief Запрашивает ввод значения у пользователя с обработкой ошибок ввода
 @param mesg Сообщение с приглашением к вводу.
 */
int askValue(std::string mesg);
 
int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru");
 
    while (true)
    {
        std::ifstream file_in;
        std::string filename;
 
        // Запрашиваем у пользователя имя файла и пытаемся его открыть
        while (true)
        {
            std::cout << "Для выхода введите символ *\n"
                << "Введите имя файла: ";
 
            getline(std::cin, filename);
 
            // Завершение работы программы при пустом вводе
            if (filename == "*")
                return 0;
 
            file_in.open(filename);
 
            if (file_in.is_open())
                break;
 
            std::cerr << "Ошибка открытия файла: \"" << filename << "\": "
                //<< std::strerror(errno)
                << std::endl;
        }
 
        // Чтение файла, проверка на ошибки
#ifdef WITH_STREAM
        std::istringstream stream;
#endif
        int pos = 0, count = 0;
        int size = 0;
        int* arr = nullptr;
        bool err = false, allocerr = false;
        while (!(file_in >> std::ws).eof())
        {
            int temp_el;
            pos = file_in.tellg();
#ifdef WITH_STREAM
            std::string temp_str;
            file_in >> temp_str;
            stream = std::istringstream(temp_str);
            stream >> temp_el;
 
            // Т.к. мы читаем значение элемента из отдельного потока, то если
            // мы не достигли конца потока или при чтении произошла ошибка, то
            // запись элемента некорректна
            if (!stream.eof() || stream.fail())
            {
                err = true;
                break;
            }
#else
            char c;
            if (!(file_in >> temp_el) || (c = file_in.peek()) != ' ' && c != '\n' && c != '\t' && c != EOF)
            {
                err = true;
                break;
            }
#endif
            if (count >= size)
                arr = resize(arr, size, APPEND_SIZE, true);
 
            if (arr == nullptr)
            {
                allocerr = true;
                break;
            }
 
            arr[count] = temp_el;
 
            count++;
        }
 
        if (err)
        {
 
            std::string str;
#ifdef WITH_STREAM
            str = stream.str();
 
            if (!stream.eof())
#else
            file_in.clear();
            char c = file_in.peek();
            file_in.seekg(pos);
            file_in >> str;
            if (c != ' ' && c != '\n' && c != '\t' && c != EOF)
#endif
            {
                std::cerr << "Некорректная запись " << (count + 1) << "го элемента.\n";
            }
            else
            {
                std::cerr << "Слишком большое значение " << (count + 1) << "го элемента.\n";
            }
 
            std::cerr << "Элемент: \"" << str << "\"\n"
                << "Абсолютная позиция первого символа: " << pos << std::endl;
 
            //file_in.close();
            //continue;
        }
 
        if (!count)
        {
            std::cout << "В файле нет данных для обработки.\n";
            file_in.close();
            continue;
        }
 
        if (allocerr)
        {
            std::cerr << "Не удалось выделить память под исходный массив!\n";
            file_in.close();
            continue;
        }
 
        std::cout << "Исходный массив из " << count << " элементов:\n";
        printarr(arr, count, 80);
        std::cout << std::endl;
 
        int d = askValue("Введите коэффициент (целое число) d: ");
        int a = askValue("Введите коэффициент (целое число) a: ");
        int b = askValue("Введите коэффициент (целое число) b: ");
        int* arr2 = new int[count];
        /*
        try
        {
            int* arr2 = new int[count];
        }
        catch (...)
        {
            std::cerr << "Не удалось выделить память под результирующий массив!\n\n";
            delete[] arr;
            file_in.close();
            continue;
        }*/
 
        bool f = false;
        int tmp = 0;
        for (int i = 0; i < count; i++)
        {
            if (arr[i] > d)
            {
                if (!f)
                {
                    f = true;
                    for (int j = 0; j < count && j < a; j++)
                        tmp += arr[j];
                }
                arr2[i] = tmp;
            }
            else
            {
                arr2[i] = arr[b - 1];
            }
        }
        delete[] arr;
 
        std::cout << "Итоговый массив из " << count << " элементов:\n";
        printarr(arr2, count, 80);
        std::cout << std::endl;
 
        delete[] arr2;
 
        std::cout << "\n\n";
 
        file_in.close();
    }
 
    return 0;
}
 
int* resize(int* arr, int& size, int count, bool delArrIfErr)
{
    // По умолчанию new кидает исключение, чтобы он этого не делал
    // а возвращал нулевой указатель в случае неудавшейся попытки выделения
    // памяти необходимо написать new(std::nothrow)
 
    int* tmp;
#ifdef NEW_THROW
    try
    {
        tmp = new int[size + count];
    }
    catch (...)
    {
        tmp = nullptr;
    }
#else
    t = new (std::nothrow) int[size + count];
#endif
 
    if (tmp && arr)
    {
        for (int i = 0; i < std::min(size, size + count); i++)
            tmp[i] = arr[i];
        size += count;
        delete[] arr;
    }
 
    else if (arr && delArrIfErr)
        delete[] arr;
 
    return tmp;
}
 
void printarr(int* arr, int size, int str_len)
{
    std::ostringstream headers;
    std::ostringstream separator;
    std::ostringstream values;
 
    std::cout << std::setw(str_len) << std::setfill('=') << '\n';
 
    for (int i = 0; i < size;)
    {
        headers = std::ostringstream("Номера   |", std::ios_base::ate);
        separator = std::ostringstream("---------|", std::ios_base::ate);
        values = std::ostringstream("Элементы |", std::ios_base::ate);
 
        for (; i < size; i++)
        {
            int title_len = std::to_string(i).length() + 1;
            int elem_len = std::to_string(arr[i]).length() + 1;
            int max = elem_len > title_len ? elem_len : title_len;
 
            headers << std::setw(max) << i << " |";
            separator << std::setfill('-') << std::setw(max + 2) << '|';
            values << std::setw(max) << arr[i] << " |";
 
            if (headers.str().length() >= str_len)
                break;
        }
 
        std::cout << headers.str() << '\n'
            << separator.str() << '\n'
            << values.str() << "\n";
 
        std::cout << std::setw(str_len) << std::setfill('=') << '\n';
    }
}
 
int askValue(std::string mesg)
{
    int tmp;
 
    while (true)
    {
        std::cout << mesg;
        std::cin >> tmp;
 
        if (std::cin.peek() == '\n')
            break;
 
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Ошибка ввода.\n";
    }
 
    return tmp;
}
#include <iostream>
#include <string.h>
#define __LINUX__
int getLength(char *startPointer)
{
    char *end = startPointer;
    while (*end++)
        ;
    return end - startPointer - 1;
}

struct findResult
{
    int startOffset;
    int sequenceOffset;
    int sequenceLength;
    findResult(int startOffset, int sequenceOffset, int sequenceLength)
    {
        this->startOffset = startOffset;
        this->sequenceLength = sequenceLength;
    }
    findResult()
    {
        this->startOffset = -1;
        this->sequenceOffset = -1;
        this->sequenceLength = -1;
    }
};

findResult findSequence(char *array)
{
    int arrayLength = getLength(array);
    if (arrayLength > 1)
    {
        for (int sequenceLength = arrayLength / 2; sequenceLength != 1; sequenceLength--)
        {
            for (int startOffset = 0; startOffset <= arrayLength - sequenceLength * 2; startOffset++)
                for (int sequenceOffset = 0, allVariants = arrayLength - sequenceLength * 2 - startOffset; sequenceOffset <= allVariants; sequenceOffset++)
                {
                    for (int validBlockSize = 0; validBlockSize < sequenceLength;)
                    {
                        if (array[validBlockSize + startOffset] != array[sequenceLength + validBlockSize + sequenceOffset + startOffset])
                        {
                            sequenceOffset += validBlockSize;
                            break;
                        }
                        if (++validBlockSize == sequenceLength)
                            return findResult(startOffset, sequenceLength + startOffset + sequenceOffset, sequenceLength);
                    }
                }
        }
    }
    return findResult();
}

int main()
{
    setlocale(LC_ALL, "ru");
    std::cout << "Введите последовательность:\n";
    int size = 800;
    char arr[size];
    std::cin.getline(arr, sizeof(arr));
    findResult result = findSequence(arr);
    if (result.startOffset != -1)
    {
#ifdef __LINUX__
        char second[result.sequenceLength + 1];
        strncpy(second, arr + result.startOffset, result.sequenceLength);
        second[result.sequenceLength + 1] = '\0';
#else
        char *second = new (std::nothrow) char[result.sequenceLength + 1];
        if (!second)
        {
            std::cout << "Не удалось выделить память под последовательность\n";
            return -1;
        }
        strncpy_s(second, result.sequenceLength + 1, arr + result.startOffset, result.sequenceLength);
#endif
        std::cout << "Найден повтор последовательности: " << second << '\n'
                  << "Начало повтора с позиции " << result.startOffset << '\n'
                  << "Длина последовательности " << result.sequenceLength << '\n';

#ifndef __LINUX__
        delete[] second;
#endif
    }
    else
    {
        std::cout << "Последовательность не найдена!" << '\n';
    }

    return 0;
}
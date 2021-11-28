#include <iostream>

int main()
{
    int MSize = 9;
    double **TriangleMatrixLow = (double **)(void *)(new (std::nothrow) char[sizeof(double *) * MSize + sizeof(double) * (MSize * (MSize + 1)) / 2]);
    for (int i = 0; i < MSize; i++)
    {
        TriangleMatrixLow[i] = (double *)(TriangleMatrixLow + MSize) + ((i + 1) * i) / 2;
        for (int j = 0; j <= i; j++)
        {
            TriangleMatrixLow[i][j] = (i + 1) + ((j + 1) / 10.0);
            std::cout << TriangleMatrixLow[i][j] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << '\n';

    for (int i = 0; i < MSize; i++)
    {
        for (int j = 0; j <= i; j++)
            std::cout << TriangleMatrixLow[i][j] << ' ';
        std::cout << '\n';
    }
    delete[](char *) TriangleMatrixLow;

    std::cout << '\n';

    double **TriangleMatrixHigh = (double **)(void *)(new (std::nothrow) char[sizeof(double *) * MSize + sizeof(double) * (MSize * (MSize + 1)) / 2]);
    for (int i = 0; i < MSize; i++) // (int i = MSize; i; i--)
    {
        TriangleMatrixHigh[i] = (double *)(TriangleMatrixHigh + MSize) + (i * (1 + 2 * MSize - i)) / 2; // + ((MSize + i) * 2 - i) // - i + 2 * MSize
                                                                                                        // for (int k = 0; k < MSize; k++)                                                                         //(MSize * (MSize - i)) - (MSize - i) * (MSize - i) / 2
                                                                                                        // {
        for (int j = 0; j < MSize - i;)
        {
            TriangleMatrixHigh[i][j] = (i + 1) + (++j / 10.0);
            std::cout << TriangleMatrixHigh[i][j] << ' ';
        }
        // }

        std::cout << '\n';
    }

    std::cout << '\n';

    for (int i = 0; i < MSize; i++)
    {
        for (int j = 1; j <= MSize - i; j++)
            std::cout << TriangleMatrixHigh[i][j] << ' ';
        std::cout << '\n';
    }
    delete[](char *) TriangleMatrixHigh;
    return 0;
}
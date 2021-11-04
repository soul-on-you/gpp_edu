#include <iostream>

using namespace std;

int main()
{
	int **matr, **p;

	int k[4] = {52, 94, -213, 74};
	matr = new int *[2];

	for (int i = 0; i < 2; i++)
		*(matr + i) = new int[2];

	p = matr;
	for (int n = 0, i = 0; n < 2; n++)
		for (int m = 0; m < 2; m++)
		{
			*(*(p + n) + m) = k[i];
			i++;
		}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
			cout << *(*(p + i) + j) << " ";

		cout << "\n";
	}

	return 0;
}
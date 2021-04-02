#include <iostream>

double *re_reserve(double dtemp, int &size,double *buf)
{
    double *temp;
    try
    {   
        size++;
        temp= new double[size];
        for(int i=0; (i+1)<size; i++)
            temp[i]=buf[i];
        temp[size]=dtemp;
    }
    catch()
    {

    }
    return *buf;
}

int main()
{
    return 0;
}
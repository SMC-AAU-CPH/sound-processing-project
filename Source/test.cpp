#include "cplxNum.h"
#include <iostream>
#include <math.h>

using namespace std;

int main (int argc, char * argv[])
{
    double r1;
    double r2;
    double i1;
    double i2;
    if(argc > 4)
    {
        r1 = atof(argv[1]);
        i1 = atof(argv[2]);
        r2 = atof(argv[3]);
        i2 = atof(argv[4]);
    }
    cplxNum<double> a (r1, i1);
    cplxNum<double> b (r2, i2);
    cplxNum<double> c;
    cout << "First number entered a = ";
    cout << a;
    cout << "Second number entered b = ";
    cout << b;
    cout << "a^b = " << endl;
    c = a ^ b;
    c.printExp();
    cout << c;
}


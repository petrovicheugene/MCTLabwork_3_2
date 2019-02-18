//=====================================================
#include <omp.h>
#include <iostream>
#include <math.h>
#include <random>
//=====================================================
using namespace std;

double f(double x, double y);
double calcIntegral(int num_Intervals);

static double YMin = 0.0;
static double YMax = M_PI;
static double XMin = 0.0;
static double XMax = 2.0;
//=====================================================
int main()
{
    cout << "Calculation of multiple integral using rectangle method." << endl;
    cout << "Function: sin(x + y) + x * pow(y, 2.0)" << endl;

    int num_intervals = 1;
    double eps = 0.001;

    double previousIntegral = calcIntegral(num_intervals);
    double start = omp_get_wtime();
    double integral;

    for (;;)
    {
        num_intervals *= 2;
        integral = calcIntegral(num_intervals);
        {
            if(abs(previousIntegral - integral) < eps)
            {
                break;
            }

            previousIntegral = integral;
        }
    }

    // output result
    double end = omp_get_wtime();

    cout << endl << "Integral value: " << integral << endl;
    printf("Calculation took %13.12f sec.\n", end - start);
    cout << "Number of intervals: " << num_intervals <<  endl;
    cout << "Interval lenghts: hy: " << M_PI / static_cast<double>(num_intervals)
         << " hx: " << 2.0 / static_cast<double>(num_intervals) << endl;

    system("pause");
    return 0;
}
//=====================================================
double f(double x, double y)
{
    return sin(x + y) + x * pow(y, 2.0);
}
//=====================================================
double calcIntegral(int num_intervals)
{
    double I = 0.0;
    double hy = (YMax - YMin) / static_cast<double>(num_intervals);
    double hx = (XMax - XMin) / static_cast<double>(num_intervals);

#pragma omp parallel reduction (+: I)
    {
#pragma omp for
        for (int ix = 0; ix < num_intervals; ix++)
        {
            double x = XMin + hx / 2 + ix * hx;
            for (int iy = 0; iy < num_intervals; iy++)
            {
                double y = YMin + hy / 2 + iy * hy;

                I += hx * hy * f(x, y);
            }
        }
    }

    return I;
}
//=====================================================

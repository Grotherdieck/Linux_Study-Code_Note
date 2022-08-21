#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main()
{
    ifstream ifs("text.txt");
    vector<double> arr;
    double tmp;
    while (ifs >> tmp) arr.push_back(tmp);
    double sum1 = 0.0;
    for (auto p : arr) sum1 += p;
    double bar = sum1 / 21;
    double square = 0.0;
    for (auto p : arr)
    {
        square += (p - bar) * (p - bar);
    }
    square /= 20;
    cout << square << endl;
    cout << bar << endl;
}
#include <iostream>
using namespace std;

int add(int a, int b);

int main(int argc, char* argv[])
{
    cout << "Hello World!" << endl;
    cerr << "Error" << endl;
    cout << add(1, 2);
}

int add(int a, int b) 
{
    return a + b;
}

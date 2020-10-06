#include <iostream>
using namespace std;

void triple(int& a) {
	a *= 3;
}

int main(int argc, char* argv[])
{
	int a = 2;

	triple(a);
	cout << a;
}


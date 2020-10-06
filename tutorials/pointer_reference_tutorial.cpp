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
	
	
	int a = 1;
	int b = 2;
	int c = 3;
	int* p;
	int* q;
	
	p = &a;
	q = &b;

	c = *p;
	p = q;

	*p = 13;
}


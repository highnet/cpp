
#include <iostream>
using namespace std;

class Box {
public:
	double length;
	double breadth;
	double height;
};

double calculateVolume(Box box) {
	return box.breadth * box.height * box.length;
}


int main(int argc, char* argv[])
{
	Box Box1;
	Box Box2;

	double volume1 = 0.0;
	double volume2 = 0.0;

	Box1.height = 5.0;
	Box1.length = 6.0;
	Box1.breadth = 7.0;

	Box2.height = 10.0;
	Box2.length = 12.0;
	Box2.breadth = 13.0;

	volume1 = calculateVolume(Box1);
	volume2 = calculateVolume(Box2);
	cout << volume1 << endl << volume2;
	
}

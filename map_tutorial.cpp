#include <iostream> 
#include <map>
using namespace std;

int main()
{

	map<int, string> registry;

	registry[1] = "a";
	registry[99] = "xx";

	cout << registry[1] << endl << registry[99] << endl << registry[42];

}

#include <iostream> 
#include <vector>
using namespace std;

int main()
{
	vector<int> v;
	for (int i = 0; i < 10; i++) {
		v.push_back(i);
	}

	for (int i = 0; i < v.size(); i++) {
		cout << v[i] << endl;
	}

	for (auto number : v) {
		cout << number << endl;
	}
}

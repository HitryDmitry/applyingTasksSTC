#include <iostream>

using namespace std;

char ReadAndPrint(){
    char symbol;
    cin >> symbol;
    if (symbol != '0'){
        cout << ReadAndPrint() << " ";
    }
    else return symbol;
}

int main()
{
    
	return 0;
}
#include <iostream>
using namespace std;
 
void print(int i) {
        cout << "整数为: " << i << endl;
}
 
void print(double  f) {
        cout << "浮点数为: " << f << endl;
}
 
// void print(char c[]) {
//         cout << "字符串为: " << c << endl;
// }

void print(const char *c) {
        cout << "字符串为: " << c << endl;
}


int main(void)
{
	print(3);
	print(1.1);
	print("hello\n");

	return 0;
}

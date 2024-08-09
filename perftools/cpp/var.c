// reference

#include <iostream>
 
using namespace std;

void func(int& var)
{
	cout << "var in func " << var << endl;

	var = 2;

	cout << "var modify to " << var << endl;
}

int main ()
{
   // 声明简单的变量
   int    i = 1;
   double d = 1.1;
 
   // 声明引用变量
   int&    r = i;
   double& s = d;
   
   i = 5;
   cout << "Value of i : " << i << endl;
   cout << "Value of i reference : " << r  << endl;
 
   d = 11.7;
   cout << "Value of d : " << d << endl;
   cout << "Value of d reference : " << s  << endl;

   func(i);

   
   cout << "in main i = " << i << endl;
   
   return 0;
}

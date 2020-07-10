#include"BigInteger.h"

int main() {
	cout << "BigIntegerTest1(int op1, int op2)" << endl;
	unsigned int op1 = 0, op2 = 0;
	cout << "op1 (0 ~ 4,294,967,295) : ";
	cin >> op1;
	cout << "op2 (0 ~ 4,294,967,295) : ";
	cin >> op2;
	cout << endl;
	BigIntegerTest1(op1, op2);
}
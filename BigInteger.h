#pragma once
#include<iostream>
#include<vector>
#include <iomanip>
using namespace std;

class BigInt {
public:
	//필드
	int arrLen;
	vector<unsigned int> valueArr;

	//메소드
	BigInt();					//생성자
	BigInt(const BigInt& obj);			//생성자
	BigInt(unsigned int n);				//생성자 : 초기 입력값은 4바이트 양의 정수 : 0 ~ 4,294,967,295
	BigInt& operator=(const BigInt& obj);		//대입연산
	BigInt operator+(BigInt& obj);			//덧셈연산
	BigInt operator*(BigInt& obj);			//곱셈연산
	bool operator==(BigInt& obj); 			// 크기 비교연산
	bool operator>(BigInt& obj);
	BigInt operator/(int two); 			//반으로 나누는 연산 
	BigInt operator/(BigInt& obj);			//나눗셈 연산 (this : 피제수, obj : 제수 => this / obj)
	BigInt operator-(BigInt& obj);			//뺄셈 연산 *this - obj (가정 : *this 는 항상 obj 보다 크다)
	BigInt operator%(BigInt& obj); 			//나머지 연산 this % obj 

	//입력, 출력
	friend ostream& operator<<(ostream& os, const BigInt& obj);
	friend istream& operator>>(istream& is, BigInt& obj);
};

void BigIntegerTest1(int op1, int op2);

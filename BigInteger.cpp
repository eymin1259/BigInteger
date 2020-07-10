#include"BigInteger.h"

BigInt::BigInt() {
	valueArr.push_back(0); //0
	arrLen = 1;
};

BigInt::BigInt(const BigInt& obj) {
	this->valueArr = obj.valueArr;
	this->arrLen = valueArr.size();
};

BigInt::BigInt(unsigned int n) {		// 초기 입력값은 4바이트 양의 정수 : 0 ~ 4,294,967,295
		//배열의 원소 하나는 int의 범위 내에서 모든 연산을 구현해야 하므로 
		//구현의 편의를 위해 0~9999까지만 저장
		// 0 <= n <= 9,999
	if (n >= 0 && n < 10000) {
		valueArr.push_back(n);
		arrLen = valueArr.size();
	}
	// 10,000 <= n <= 4,294,967,295
	else if (n >= 10000 && n < 100000000) {
		unsigned int small = n % 10000;
		unsigned int big = n / 10000;
		valueArr.push_back(big);
		valueArr.push_back(small);
	}
	else {
		unsigned int small = n % 10000;
		unsigned int big = (n / 10000) % 10000;
		unsigned int bigBig = n / 100000000;
		valueArr.push_back(bigBig);
		valueArr.push_back(big);
		valueArr.push_back(small);
	}
	arrLen = valueArr.size();
};

//대입연산
BigInt& BigInt:: operator=(const BigInt& obj) {
	this->valueArr = obj.valueArr;
	this->arrLen = valueArr.size();
	return *this;
};

//덧셈연산
BigInt BigInt::operator+(BigInt& obj) {
	BigInt result;
	BigInt smallObg;
	BigInt bigObj;

	if (*this > obj) {
		smallObg = obj;
		bigObj = *this;
	}
	else {
		smallObg = *this;
		bigObj = obj;
	}

	int shortLen = smallObg.arrLen;		//작은 수의 자릿수
	int longLen = bigObj.arrLen;		//큰 수의 자릿수
	int carryDigit = 0;					//올림수				
	result.valueArr.resize(longLen + 1);
	int j = shortLen - 1;

	for (int i = longLen - 1; i >= 0; i--) {
		int val = 0;
		if (j >= 0) {
			val = bigObj.valueArr[i] + smallObg.valueArr[j];
		}
		else {
			val = bigObj.valueArr[i];
		}

		if (val >= 10000) {
			val %= 10000;
			carryDigit = 1;
		}
		result.valueArr[i + 1] += val;
		result.valueArr[i] += carryDigit;
		j--;
		carryDigit = 0;
	}
	// 최대 자릿수 까지 채우지 못한경우 비여있는 자리 제거
	while (result.valueArr[0] == 0) {
		if (result.valueArr.size() == 1)
			break;
		result.valueArr.erase(result.valueArr.begin());
	}
	result.arrLen = result.valueArr.size();
	return result;
}
//곱셈 연산
BigInt BigInt::operator*(BigInt& obj) {
	BigInt result;	//0이 들어있음
	BigInt smallObg;
	BigInt bigObj;
	if (*this > obj) {
		smallObg = obj;
		bigObj = *this;
	}
	else {
		smallObg = *this;
		bigObj = obj;
	}

	if (smallObg.arrLen == 1 && smallObg.valueArr[0] == 0) 	//0을 곱하는 경우
		return result;
	if (bigObj.arrLen == 1 && bigObj.valueArr[0] == 0)
		return result;

	int shortLen = smallObg.arrLen;		//작은 수의 자릿수
	int longLen = bigObj.arrLen;		//큰 수의 자릿수


	//네자리를 한 단위로 하여 두 수의 각 단위마다의 곱 값이 저장될 2차배열
	vector<vector<int>> matrix;

	//2차배열을 두 수의 자릿수에 맞게 resize
	matrix.resize(shortLen);
	for (int i = 0; i < shortLen; i++)
		matrix[i].resize(longLen);

	result.valueArr.resize(shortLen + longLen); // n자릿수 * m자릿수는 최대 n+m 자릿수

	//(ABC * DEF) = (100a + 10b + c) * (100d + 10e + f)
	// = 100a(100d + 10e + f) 
	// + 10b (100d + 10e + f) 
	// + c   (100d + 10e + f) 
	//위와 같은 형태를 2차원 배열에 저장
	for (int i = shortLen - 1; i >= 0; i--) {
		for (int j = longLen - 1; j >= 0; j--) {
			matrix[i][j] = bigObj.valueArr[j] * smallObg.valueArr[i];
		}
	}

	//올림연산 전 k번째 자릿수의 값은 k = i+j +1 인 2차원 배열 i행 j열의 원소들의 합
	// ex) 0행 0열의 원소는 1 번째 자리수의 값
	for (int i = shortLen - 1; i >= 0; i--) {
		for (int j = longLen - 1; j >= 0; j--) {
			result.valueArr[i + j + 1] += matrix[i][j];
		}
	}

	// 올림 연산
	for (int i = result.valueArr.size() - 1; i >= 0; i--) {
		int val = result.valueArr[i] % 10000;		// 저장될 값
		int carry = result.valueArr[i] / 10000;		//	올림 수
		result.valueArr[i] = val;
		if (i != 0)
			result.valueArr[i - 1] += carry;
		else
			result.valueArr[i] += carry;
	}

	// 최대 자릿수 까지 채우지 못한경우 비여있는 자리 제거
	while (result.valueArr[0] == 0) {
		if (result.valueArr.size() == 1)
			break;
		result.valueArr.erase(result.valueArr.begin());
	}
	result.arrLen = result.valueArr.size();
	return result;
}
// 크기 비교연산
bool BigInt::operator==(BigInt& obj) {
	if (this->arrLen != obj.arrLen)
		return false;
	else {
		int check = 0;
		for (int i = 0; i < this->arrLen; i++) {
			if (this->valueArr[i] != obj.valueArr[i]) {
				check++;
				break;
			}
		}
		if (check == 0)
			return true;
		else false;
	}
}
bool BigInt::operator>(BigInt& obj) {
	if (this->arrLen > obj.arrLen) {
		return true;
	}

	else if (this->arrLen < obj.arrLen) {
		return false;
	}

	else { //this->arrLen == obj.arrLen
		bool check = false;
		for (int i = 0; i < this->arrLen; i++) {
			if (this->valueArr[i] > obj.valueArr[i]) {
				check = true;
				break;
			}
			else if (this->valueArr[i] < obj.valueArr[i]) {
				check = false;
				break;
			}
		}
		if (check == true) // this > obj
			return true;
		else // obj >= this
			return false;
	}
}

//반으로 나누는 연산 
BigInt BigInt::operator/(int two) {
	//cout << "반으로 나누는 연산 : " << *this << " / " << 2 << endl;
	BigInt ret;
	ret.valueArr.resize(this->arrLen);
	int rest = 0;
	for (int i = 0; i < this->arrLen; i++) {
		int val = ((rest + this->valueArr[i]) / 2);
		ret.valueArr[i] = val;
		rest = ((rest + this->valueArr[i]) % 2) * 10000;
		//cout << rest << endl;
	}
	ret.arrLen = ret.valueArr.size();
	return ret;
}

//나눗셈 연산
BigInt BigInt::operator/(BigInt& obj) { //this : 피제수, obj : 제수 => this/obj
	BigInt ret;
	int objLen = obj.arrLen;
	int thisLen = this->arrLen;

	// 제수가 큰경우
	if (objLen > thisLen) {
		//	cout << "case #1 " << endl;
		return ret; // 0
	}
	// 제수와 피제수 자릿수 같은경우
	else if (thisLen == objLen) {
		//	cout << "case #2 " << endl;
			// 두수가 같은 경우
		int same = 0;
		for (int i = 0; i < thisLen; i++) {
			if (this->valueArr[i] == obj.valueArr[i])
				same++;
		}
		if (same == thisLen) {
			ret = 1;
			ret.arrLen = ret.valueArr.size();
			return ret;
		}
		// 두 수가 자릿수는 같지만 값이다른경우
		//제수가 큰경우, ex) this 13 / obj 15
		else if (obj > * this)
			return ret; // 0
		// 피제수가 큰 경우, ex) this 9 / obj 1
		else {
			BigInt low = 1;
			BigInt high = *this;
			BigInt quotient = (low + high);
			quotient = quotient / 2;
			BigInt quoPlusOne = quotient;
			quoPlusOne.valueArr[quoPlusOne.arrLen - 1] += 1;
			BigInt temp = obj * quotient;
			BigInt temp2 = obj * quoPlusOne;

			while (!((*this) > temp&& temp2 > (*this))) { // 몫*제수 < 피제수(this) < (몫+1)*제수 을 만족시키는 몫을 찾는다
				if (temp > * this) {
					high = quotient;
					quotient = (low + high) / 2;
					quoPlusOne = quotient;
					quoPlusOne.valueArr[quoPlusOne.arrLen - 1] += 1;
					temp = obj * quotient;
					temp2 = obj * quoPlusOne;
				}
				else if (*this > temp2) {
					low = quotient;
					quotient = (low + high) / 2;
					quoPlusOne = quotient;
					quoPlusOne.valueArr[quoPlusOne.arrLen - 1] += 1;
					temp = obj * quotient;
					temp2 = obj * quoPlusOne;
				}
				else { //나머지 == 0
					break;
				}
			}

			ret = quotient;
			// 최대 자릿수 까지 채우지 못한경우 비여있는 자리 제거
			while (ret.valueArr[0] == 0) {
				if (ret.valueArr.size() == 1)
					break;
				ret.valueArr.erase(ret.valueArr.begin());
			}
			ret.arrLen = ret.valueArr.size();
			return ret;
		}
	}
	// 피제수(this)의 자릿수가 큰경우, ex) this 100 / obj 3
	else {
		BigInt low = 1;
		BigInt high = *this;
		BigInt quotient = (low + high);
		quotient = quotient / 2;
		BigInt quoPlusOne = quotient;
		quoPlusOne.valueArr[quoPlusOne.arrLen - 1] += 1;
		BigInt temp = obj * quotient;
		BigInt temp2 = obj * quoPlusOne;

		while (!((*this) > temp&& temp2 > (*this))) { // 몫*제수 < 피제수(this) < (몫+1)*제수 을 만족시키는 몫을 찾는다
			if (temp > * this) {
				high = quotient;
				quotient = (low + high) / 2;
				quoPlusOne = quotient;
				quoPlusOne.valueArr[quoPlusOne.arrLen - 1] += 1;
				temp = obj * quotient;
				temp2 = obj * quoPlusOne;
			}
			else if (*this > temp2) {
				low = quotient;
				quotient = (low + high) / 2;
				quoPlusOne = quotient;
				quoPlusOne.valueArr[quoPlusOne.arrLen - 1] += 1;
				temp = obj * quotient;
				temp2 = obj * quoPlusOne;
			}
			else { //나머지 == 0
				break;
			}
		}

		ret = quotient;
		// 최대 자릿수 까지 채우지 못한경우 비여있는 자리 제거
		while (ret.valueArr[0] == 0) {
			if (ret.valueArr.size() == 1)
				break;
			ret.valueArr.erase(ret.valueArr.begin());
		}
		ret.arrLen = ret.valueArr.size();
		return ret;
	}
}
//뺄셈 연산 *this - obj (가정 : *this 는 항상 obj 보다 크다)
BigInt BigInt::operator-(BigInt& obj) {
	if (obj.arrLen == 1 && obj.valueArr[0] == 0)
		return *this;

	BigInt result;

	int thisLen = this->arrLen;		//작은 수의 자릿수
	int objLen = obj.arrLen;		//큰 수의 자릿수
	int digit = 0;					//올림수				
	result.valueArr.resize(thisLen + 1);
	int j = objLen - 1;

	for (int i = thisLen - 1; i >= 0; i--) {
		int val = 0;
		if (j >= 0) {
			val = this->valueArr[i] + 10000 - obj.valueArr[j];
		}
		else {
			val = this->valueArr[i];
		}

		if (val >= 10000) {
			val %= 10000;
		}
		else {
			digit = 1;
		}
		result.valueArr[i + 1] += val;
		result.valueArr[i] -= digit;
		j--;
		digit = 0;
	}
	// 최대 자릿수 까지 채우지 못한경우 비여있는 자리 제거
	while (result.valueArr[0] == 0) {
		if (result.valueArr.size() == 1)
			break;
		result.valueArr.erase(result.valueArr.begin());
	}
	result.arrLen = result.valueArr.size();
	return result;
}

// 나머지 연산 this % obj 
BigInt BigInt::operator%(BigInt& obj) {
	//제수(obj)가 큰 경우 피제수(this)를 반환, ex) 3/10
	if (obj.arrLen > this->arrLen)
		return *this;
	// 제수(obj)와 피제수(this)가 같은 자릿수		
	else {
		BigInt ret;
		BigInt quotient = (*this / obj);
		BigInt temp = obj * quotient;

		ret = *this - temp;
		return ret;
	}
}

ostream& operator<<(ostream& os, const BigInt& obj) {	//cout
	//os << obj.valueArr[0] << "|";
	os << obj.valueArr[0];

	if (obj.arrLen > 1) {
		for (int i = 1; i < obj.arrLen; i++) {
			cout.width(4);
			cout.fill('0');
			//os << obj.valueArr[i] << "|";
			os << obj.valueArr[i];
		}
	}
	//os << obj.valueArr[0];
	return os;
}

istream& operator>>(istream& is, BigInt& obj) {	//cin
	is >> obj.valueArr[0];
	return is;
}

void BigIntegerTest1(int op1, int op2) {
	cout << "----------------------------------" << endl;
	cout << "      BigInteger Test result      " << endl;
	cout << "----------------------------------" << endl;
	BigInt obj1 = op1;
	BigInt obj2 = op2;
	BigInt obj3 = obj1 * obj1;
	BigInt obj4 = obj3 * obj3;
	cout << obj1 << " + " << obj2 << " = " << (obj1 + obj2) << endl;
	cout << obj1 << " * " << obj2 << " = " << (obj1 * obj2) << endl;
	if (obj2.arrLen == 1 && obj2.valueArr[0] == 0) {
		cout << obj1 << " / " << obj2 << " = " << "산술오류 : divider = 0" << endl;
		cout << obj1 << " % " << obj2 << " = " << "산술오류 : divider = 0" << endl;
	}
	else {
		cout << obj1 << " / " << obj2 << " = " << (obj1 / obj2) << endl;
		cout << obj1 << " % " << obj2 << " = " << (obj1 % obj2) << endl;
	}
	cout << "obj1^2 = " << obj3 << endl;
	cout << "obj1^4 = " << obj4 << endl;
	cout << "obj1^8 = " << (obj4 * obj4) << endl;
}

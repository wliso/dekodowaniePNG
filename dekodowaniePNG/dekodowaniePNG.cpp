// dekodowaniePNG.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <vector>
#include <cmath>
#include "jpg.h"
using namespace std;

bool czyPNG(vector<string> znak) {
	if (znak[0].compare("89") == 0 &&
		znak[1].compare("50") == 0 &&
		znak[2].compare("4e") == 0 &&
		znak[3].compare("47") == 0 &&
		znak[4].compare("d") == 0 &&
		znak[5].compare("a") == 0 &&
		znak[6].compare("1a") == 0 &&
		znak[7].compare("a") == 0)
		return true;
	else return false;
}

string char_to_str(char ch)
{
	bitset<8> temp(ch);
	return temp.to_string();
}

void doPlikuAscii(vector<string> slowa) {
	ofstream zapis("plik.png", ios::binary | ios::app);
	stringstream str2;
	string ascii;
	for (int i = 0; i < slowa.size(); i++) {
		if (slowa[i].size() == 1) { ascii = hexToAscii("0" + slowa[i]); }
		else
		ascii = hexToAscii(slowa[i]);
		zapis << ascii;
	}
	zapis.close();
}

void dekodowanie(vector<string> znak) {
	stringstream str2;
	for (int i = 1; i <= 4; i++) str2 << znak[11 + i];
	string val2;
	val2 = str2.str();
	cout << "ile bajtow znajdzie sie w polu "<< hexToAscii(val2) <<"?" << endl;
	stringstream str;
	for (int i = 1; i <= 4; i++) str << znak[7 + i];
	int val;
	str >> hex >> val;
	cout << val << endl;
	stringstream str3;
	for (int i = 1; i <= 4; i++) str3 << znak[15 + i];
	int val3;
	str3 >> hex >> val3;
	cout << "szerokosc obrazu: " << val3 << endl;
	stringstream str4;
	for (int i = 1; i <= 4; i++) str4 << znak[19 + i];
	int val4;
	str4 >> hex >> val4;
	cout << "wysokosc obrazu: " << val4 << endl;
	stringstream str5;
	str5 << znak[23 + 1];
	int val5;
	str5 >> hex >> val5;
	cout << "glebia koloru: " << val5 << endl;
	stringstream str6;
	str6 << znak[24 + 1];
	int val6;
	str6 >> hex >> val6;
	cout << "typ koloru: " << val6 << endl;
	stringstream str7;
	str7 << znak[25 + 1];
	int val7;
	str7 >> hex >> val7;
	cout << "metoda kompresji: " << val7 << endl;
	stringstream str8;
	str8 << znak[26 + 1];
	int val8;
	str8 >> hex >> val8;
	cout << "metoda filtracji: " << val8 << endl;
	stringstream str9;
	str9 << znak[27 + 1];
	int val9;
	str9 >> hex >> val9;
	cout << "metoda przeplatania: " << val9 << endl;
	stringstream str10;
	for (int i = 1; i <= 4; i++) str10 << znak[36 + i];
	string val10;
	val10 = str10.str();
	cout << "ile bajtow znajdzie sie w polu " << hexToAscii(val10) << "?" << endl;
	stringstream str11;
	for (int i = 1; i <= 4; i++) str11 << znak[32 + i];
	int val11;
	str11 >> hex >> val11;
	cout << val11 << endl;
	stringstream str12;
	str12 << znak[33 + 1];
	int val12;
	str12 >> hex >> val12;
	cout << "ilosc bitow znaczacych dla czerwonego: " << val12 << endl;
	stringstream str13;
	str13 << znak[34 + 1];
	int val13;
	str13 >> hex >> val13;
	cout << "ilosc bitow znaczacych dla zielonego: " << val13 << endl;
	stringstream str14;
	str14 << znak[35 + 1];
	int val14;
	str14 >> hex >> val14;
	cout << "ilosc bitow znaczacych dla niebieskiego: " << val14 << endl;
	stringstream str15;
	str15 << znak[36 + 1];
	int val15;
	str15 >> hex >> val15;
	cout << "ilosc bitow znaczacych dla kanalow alfa: " << val15 << endl;
}

int nwd(int a, int b)
{
	int t;

	while (b != 0)
	{
		t = b;
		b = a % b;
		a = t;
	};
	return a;
}

// Funkcja obliczania odwrotnoœci modulo n
//----------------------------------------
int odwr_mod(int a, int n)
{
	int a0, n0, p0, p1, q, r, t;

	p0 = 0; p1 = 1; a0 = a; n0 = n;
	q = n0 / a0;
	r = n0 % a0;
	while (r > 0)
	{
		t = p0 - q * p1;
		if (t >= 0)
			t = t % n;
		else
			t = n - ((-t) % n);
		p0 = p1; p1 = t;
		n0 = a0; a0 = r;
		q = n0 / a0;
		r = n0 % a0;
	}
	return p1;
}

// Procedura generowania kluczy RSA
//---------------------------------
vector<int> klucze_RSA()
{
	const int tp[10] = { 11,13,17,19,23,29,31,37,41,43 };
	int p, q, phi, n, e, d;

	do
	{
		p = 11;
		q = 23;
	} while (p == q);

	phi = (p - 1) * (q - 1);
	n = p * q;

	for (e = 3; nwd(e, phi) != 1; e += 2);
	d = odwr_mod(e, phi);

	//int tab[] = { e,n,d };
	vector<int> tab;
	tab.push_back(e);
	tab.push_back(n);
	tab.push_back(d);
	return tab;
}

int kodowanie_RSA(int e, int n,int t)
{
	int wynik=1,i;
	for (i = e; i > 0; i /= 2)
	{
		if (i % 2) wynik = (wynik * t) % n;
		t = (t * t) % n; 
	}
	return wynik;
}

void zapisZnakuDoPliku(string znak) {
	ofstream zapis("plikRSA.txt", ios::app);
	zapis << znak << " ";
	zapis.close();
	ofstream zapis2("plik.png", ios::binary | ios::app);
	stringstream str2;
	string ascii;
		if (znak.size() == 1) { ascii = hexToAscii("0" + znak); }
		else
			ascii = hexToAscii(znak);
		zapis2 << ascii;
	zapis2.close();
}

unsigned long hex2dec(string hex)
{
	unsigned long result = 0;
	for (int i = 0; i < hex.length(); i++) {
		if (hex[i] >= 48 && hex[i] <= 57)
		{
			result += (hex[i] - 48)*pow(16, hex.length() - i - 1);
		}
		else if (hex[i] >= 65 && hex[i] <= 70) {
			result += (hex[i] - 55)*pow(16, hex.length() - i - 1);
		}
		else if (hex[i] >= 97 && hex[i] <= 102) {
			result += (hex[i] - 87)*pow(16, hex.length() - i - 1);
		}
	}
	return result;
}

string intToHexString(int intValue) {

	string hexStr;
	std::stringstream sstream;
	sstream << std::hex << (int)intValue;
	hexStr = sstream.str();
	sstream.clear();

	return hexStr;
}

void szukajIDAT(vector<string> znak) {
	//49 44 41 54
	int kod;
	vector<int> tab = klucze_RSA();
	for (int i = 0; i < 8; i++) {
		zapisZnakuDoPliku(znak[i]);
	}
	for (int i = 8; i < znak.size(); ++i) {
		if (znak[i] == "49") {
			zapisZnakuDoPliku(znak[i]);
			if (znak[i+1] == "44") {
				++i;
				zapisZnakuDoPliku(znak[i]);
				if (znak[i+1] == "41") {
					++i;
					zapisZnakuDoPliku(znak[i]);
					if (znak[i+1] == "54") {
						++i;
						zapisZnakuDoPliku(znak[i]);
						stringstream strpom;
						for (int j = 4; j >= 1; j--) strpom << znak[i -3 - j];
						int val12;
						strpom >> hex >> val12;
						int l;
						for (l = 0; l < val12; l++) {
							int val15 = hex2dec(znak[i+l]);
							//cout << " val15: " << val15;
							kod = kodowanie_RSA(tab[0], tab[1], val15);
							//cout << " kod: " << kod<<endl;
							
							zapisZnakuDoPliku(intToHexString(kod));
						}
						i = i+l;
					}
				}
			}
		}
		else zapisZnakuDoPliku(znak[i]);
	}
	//cout << "tab : " << tab[0] << " " << tab[1] << endl;
}
int main()
{
	remove("plik.txt");
	remove("plikRSA.txt");
	remove("plik.png");
	vector<string> slowa;
	odczytZdjecia("prostokat.png", "plik.txt");
	slowa = odczytZPliku("plik.txt");
	//doPlikuAscii(slowa);
	bool ifPNG = czyPNG(slowa);
	if (ifPNG) {
		cout << "poprawny plik PNG" << endl;
		dekodowanie(slowa);
	}
	else
		cout<<"to nie jest plik PNG!" << endl;
	szukajIDAT(slowa);
	cout<<"wynik: "<<kodowanie_RSA(7, 143, 123)<<endl;
	cout << "wynik: " << kodowanie_RSA(3, 253, 86) << endl;
}



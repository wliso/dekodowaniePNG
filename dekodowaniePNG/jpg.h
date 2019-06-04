#pragma once
#include "pch.h"
#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <sstream>
#include "jpg.h"
using namespace std;

int toInt(string bitString)
{
	int tempInt;
	int num = 0;
	for (char c : bitString)
		num = (num << 1) |  // Shift the current set of bits to the left one bit
		(c - '0');

	return num;
}

unsigned char hexval(unsigned char c)
{
	if ('0' <= c && c <= '9')
		return c - '0';
	else if ('a' <= c && c <= 'f')
		return c - 'a' + 10;
	else if ('A' <= c && c <= 'F')
		return c - 'A' + 10;
	else abort();
}

string hexToAscii(string str) {
	string newString;
	newString.reserve(str.length() / 2);
	for (string::const_iterator p = str.begin(); p != str.end(); p++)
	{
		unsigned char c = hexval(*p);
		p++;
		if (p == str.end()) break; // incomplete last digit - should report error
		c = (c << 4) + hexval(*p); // + takes precedence over <<
		newString.push_back(c);
	}
	return newString;
}

void odczytZdjecia(string plikOdczytywany, string plikZapis) {
	ifstream f(plikOdczytywany, ios::binary | ios::in);
	ofstream zapis(plikZapis, ios::app);
	char c;
	int i = 1;
	while (f.get(c))
	{
		cout << c<<" ";
		bitset<8>  bity(c);
		int tmp = toInt(bity.to_string());
		zapis << hex << tmp << " ";
		if (i % 16 == 0) zapis << endl;
		i++;
	}
	zapis.close();
}
vector<string> odczytZPliku(const char* nazwa) {
	ifstream plik(nazwa);
	vector<string> znak;
	int i = 0;
	string tmp;

	do
	{
		plik >> tmp;
		znak.push_back(tmp);
		i++;
	} while (!plik.eof());

	plik.close();
	return znak;
}

bool czyJPG(vector<string> znak) {
	if (znak[0].compare("ff") == 0 &&
		znak[1].compare("d8") == 0 &&
		znak[2].compare("ff") == 0)
		return true;
	else return false;
}

void dekodowanieJPG(vector<string> znak) {
	stringstream str2;
	for (int i = 0; i < 2; i++) str2 << znak[4 + i];
	int val2;
	str2 >> hex >> val2;
	cout << "Length of segment excluding APP0 marker"<< val2 << endl;

	stringstream str1;
	for (int i = 0; i < 5; i++) str1 << znak[6 + i];
	string val1;
	val1 = str1.str();
	cout << "Identifier: " << hexToAscii(val1) << endl;

	stringstream str;
	str << znak[11];
	int vala;
	str >> hex >> vala;
	stringstream strb;
	strb << znak[12];
	int valb;
	strb >> hex >> valb;
	cout << "JFIF version: " << vala << "." << valb << endl;

	stringstream str3;
	str3 << znak[13];
	int val3;
	str3 >> hex >> val3;
	cout << "Density units: ";
	switch (val3) {
	case 0:
		cout << "No units; Ydensity : Xdensity" << endl;
		break;
	case 1:
		cout << "Pixels per inch (2.54 cm)" << endl;
		break;
	case 2:
		cout << "Pixels per centimeter" << endl;
		break;
	}

	stringstream str4;
	for (int i = 0; i < 2; i++) str4 << znak[14 + i];
	int val4;
	str4 >> hex >> val4;
	cout << "Horizontal pixel density: " << val4 << endl;

	stringstream str5;
	for (int i = 0; i < 2; i++) str5 << znak[16 + i];
	int val5;
	str5 >> hex >> val5;
	cout << "Vertical pixel density: " << val5 << endl;

	stringstream str6;
	str6 << znak[18];
	int val6;
	str6 >> hex >> val6;
	cout << "Xthumbnail: " << val6 << endl;

	stringstream str7;
	str7 << znak[19];
	int val7;
	str7 >> hex >> val7;
	cout << "Ythumbnail: " << val7 << endl;
	//0xdb

	cout << "Quantization Table" << endl;

	stringstream str8;
	for (int i = 0; i < 2; i++) str8 << znak[22 + i];
	int val8;
	str8 >> hex >> val8;
	cout << " Length of QT in bytes: " << val8 << endl;
	cout << "QT information:" << endl;
	bitset<8>  bity(znak[24]);
	bitset<4> bita, bitb;
	for (int i = 0; i < 4; i++) {
		bita[i] = bity[i];
		bitb[i] = bity[i + 4];
	}
	int b = toInt(bitb.to_string());
	cout << "number of QT: " << toInt(bita.to_string()) << endl;
	if (b == 0) 
		cout << "precision of QT: 8bit" << endl;
	else
		cout<<"precision of QT: 16bit"<< endl;

	/*stringstream str8;
	for (int i = 0; i < val7*val6; i++) str8 << znak[30+i];
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
	cout << "ilosc bitow znaczacych dla kanalow alfa: " << val15 << endl;*/
}
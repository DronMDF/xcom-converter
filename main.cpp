
#include <fstream>
#include <iostream>
#include "rle_streambuf.h"

using namespace std;

int main(int, char **)
{
	filebuf inbuf;
	inbuf.open("/home/dron/openxcom/share/openxcom/data/UNITS/XCOM_1.PCK", ios::in | ios::binary);
	rle_streambuf rle(&inbuf);
	istream in(&rle);

	int size = 0;

	while(in) {
		int c = in.get();
		if (size == 0) {
			for (int t = 0; t < c; t++) {
				cout << endl;
			}
			size = c * 32;
			continue;
		}

		if (c == 255) {
			cout << " pic size " << size << endl;
			size = 0;
			continue;
		}

		if (c == 0) {
			cout << ' ';
		} else {
			cout << char('.' + c % 92);
		}

		size++;
		if (size % 32 == 0) {
			cout << endl;
		}
	}
}

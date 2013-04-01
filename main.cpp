
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
			size = c * 32;
			continue;
		}

		if (c == 255) {
			cout << "pic size " << size << endl;
			size = 0;
			continue;
		}

		if (c == 254) {
			size += in.get();
		} else {
			size++;
		}
	}
}

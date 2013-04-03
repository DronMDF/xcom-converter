
#include <fstream>
#include <iostream>
#include <vector>
#include <boost/format.hpp>
#include "rle_streambuf.h"
#include "PngWriter.h"

using namespace std;
using namespace boost;

int main(int, char **)
{
	filebuf inbuf;
	inbuf.open("/home/dron/openxcom/share/openxcom/data/UNITS/XCOM_1.PCK", ios::in | ios::binary);
	rle_streambuf rle(&inbuf);
	istream in(&rle);

	int n = 1;
	vector<uint8_t> image;
	while(in) {
		int c = in.get();
		if (c == char_traits<char>::eof()) {
			break;
		}

		if (image.empty()) {
			image.assign(c * 32, 0);
			continue;
		}

		if (c != 255) {
			image.push_back(c);
			continue;
		}

		PngWriter writer(32, 40, image);
		writer.save(str(format("XCOM_1_%1%.png") % n));
		n++;
		image.clear();
	}
}

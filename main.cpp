
#include <array>
#include <fstream>
#include <iostream>
#include <vector>
#include <boost/format.hpp>
#include "rle_streambuf.h"
#include "PngWriter.h"

using namespace std;
using namespace boost;

vector<array<uint8_t, 3>> loadPalette(const string &filename, int offset, int ncolors)
{
	ifstream pf(filename.c_str(), ios::in | ios::binary);
	if (!pf) {
		throw runtime_error(filename + " not found");
	}

	// Move pointer to proper pallete
	pf.seekg(offset, ios::beg);

	vector<array<uint8_t, 3>> palette;
	for (int i = 0; i < ncolors; ++i) {
		array<uint8_t, 3> entry;
		pf.read(reinterpret_cast<char *>(&entry[0]), 3);
		entry[0] *= 4;
		entry[1] *= 4;
		entry[2] *= 4;
		palette.push_back(entry);
	}

	return palette;
}

int main(int, char **)
{
	const auto palette1 = loadPalette("/home/dron/openxcom/share/openxcom/data/GEODATA/PALETTES.DAT", 0, 256);
	const auto palette2 = loadPalette("/home/dron/openxcom/share/openxcom/data/GEODATA/PALETTES.DAT", 774, 256);
	const auto palette3 = loadPalette("/home/dron/openxcom/share/openxcom/data/GEODATA/PALETTES.DAT", 1548, 256);
	const auto palette4 = loadPalette("/home/dron/openxcom/share/openxcom/data/GEODATA/PALETTES.DAT", 2322, 256);
	const auto palette5 = loadPalette("/home/dron/openxcom/share/openxcom/data/GEODATA/BACKPALS.DAT", 0, 128);

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
		writer.save(str(format("XCOM_1_%1%.png") % n), palette2);
		n++;
		image.clear();
	}
}

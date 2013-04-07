
#include <array>
#include <fstream>
#include <iostream>
#include <vector>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include "rle_streambuf.h"
#include "PngWriter.h"

using namespace std;
using namespace boost;
using namespace boost::filesystem3;

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

void convertPCK(const string &filename, int width, int height,
		const vector<array<uint8_t, 3>> &palette, const string &outdir)
{
	const string base = path(filename).stem().string();
	const auto outpath = outdir + "/" + base;

	if (!exists(outpath)) {
		create_directory(outpath);
	}

	filebuf inbuf;
	inbuf.open(filename.c_str(), ios::in | ios::binary);
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
			image.assign(c * width, 0);
			continue;
		}

		if (c != 255) {
			image.push_back(c);
			continue;
		}

		PngWriter writer(width, height, image);
		writer.save(str(format("%1%/%2%_%3$04d.png") % outpath % base % n), palette);
		n++;
		image.clear();
	}
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		cout << argv[0] << " <xcom_dir> [<out_dir>]" << endl;
		return -1;
	}

	const string xdir = argv[1];
	const string outdir = (argc > 2) ? argv[2] : ".";

	const auto palette1 = loadPalette(xdir + "/GEODATA/PALETTES.DAT", 0, 256);
	const auto palette2 = loadPalette(xdir + "/GEODATA/PALETTES.DAT", 774, 256);
	const auto palette3 = loadPalette(xdir + "/GEODATA/PALETTES.DAT", 1548, 256);
	const auto palette4 = loadPalette(xdir + "/GEODATA/PALETTES.DAT", 2322, 256);
	const auto palette5 = loadPalette(xdir + "/GEODATA/BACKPALS.DAT", 0, 128);

	convertPCK(xdir + "/UNITS/XCOM_1.PCK", 32, 40, palette2, outdir);

	return 0;
}

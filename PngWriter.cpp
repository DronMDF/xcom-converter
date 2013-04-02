
#include "PngWriter.h"
#include <png++/png.hpp>

using namespace std;
using namespace png;

PngWriter::PngWriter(unsigned width, unsigned height, const vector<uint8_t> &data)
	: width(width), height(height), data(data)
{
	// Чтобы размер всегда был полный...
	while (this->data.size() < width * height) {
		this->data.push_back(0);
	}
}

void PngWriter::save(const std::string &filename) const
{
	image<png::rgba_pixel> image(width, height);
	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			const auto code = data[y * width + x];
			image.set_pixel(x, y, rgba_pixel(code, code, code));
		}
	}

	image.write(filename);
}

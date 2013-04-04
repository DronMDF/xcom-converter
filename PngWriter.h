
#pragma once
#include <array>
#include <vector>
#include <string>

class PngWriter {
public:
	PngWriter(unsigned width, unsigned height, const std::vector<uint8_t> &data);

	void save(const std::string &filename, const std::vector<std::array<uint8_t, 3>> &palette) const;
private:
	unsigned width;
	unsigned height;
	std::vector<uint8_t> data;
};

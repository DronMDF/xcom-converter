
#pragma once
#include <vector>
#include <string>

class PngWriter {
public:
	PngWriter(unsigned width, unsigned height, const std::vector<uint8_t> &data);

	void save(const std::string &filename) const;
private:
	unsigned width;
	unsigned height;
	std::vector<uint8_t> data;
};

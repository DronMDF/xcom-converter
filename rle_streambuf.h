
#pragma once
#include <streambuf>

class rle_streambuf : public std::streambuf {
public:
	rle_streambuf(std::streambuf *sb);
	rle_streambuf(const rle_streambuf &) = delete;
	rle_streambuf &operator=(const rle_streambuf &) = delete;
private:
	std::streambuf *sb;
	int zero_counter;

	virtual int_type underflow() override;
	virtual int_type uflow() override;
};

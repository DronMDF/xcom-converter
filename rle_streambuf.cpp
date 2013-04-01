
#include "rle_streambuf.h"

rle_streambuf::rle_streambuf(std::streambuf *sb)
	: sb(sb), zero_counter(0)
{
}

rle_streambuf::int_type rle_streambuf::underflow()
{
	if (zero_counter > 0) {
		return 0;
	}

	const auto v = sb->sgetc();
	if (v == 0xfe) {
		return 0;
	}

	return v;
}

rle_streambuf::int_type rle_streambuf::uflow()
{
	if (zero_counter > 0) {
		zero_counter--;
		return 0;
	}

	const auto v = sb->sbumpc();
	if (v == 0xfe) {
		zero_counter = sb->sbumpc() - 1;
		return 0;
	}

	return v;
}



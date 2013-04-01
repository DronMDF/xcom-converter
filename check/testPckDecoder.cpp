
#include <vector>
#include <istream>
#include <gtest/gtest.h>
#include <rle_streambuf.h>

using namespace std;

TEST(Pck, shouldDecodeRepeater)
{
	const vector<uint8_t> encoded = { 0xfe, 0x0c };
	stringbuf source(string(reinterpret_cast<const char *>(&encoded[0]), encoded.size()));
	rle_streambuf rle(&source);
	istream in(&rle);

	vector<uint8_t> decoded;
	copy(istream_iterator<uint8_t>(in), istream_iterator<uint8_t>(), back_inserter(decoded));
	ASSERT_EQ(decoded.size(), 0x0c);
	for (const auto c: decoded) {
		ASSERT_EQ(c, 0);
	}
}

TEST(Pck, shouldDecodeSequence)
{
	// Размер фрейма  - 32х40
	const vector<uint8_t> encoded = {
		0x0a,	// 32 * 10
		0xfe, 0x0c, // 12
		0xec,	// 1
		0xfe, 0x1e, // 30
		0xea, 0xeb, 0xec,	// 3
		0xfe, 0x1d, // 29
		0xec, 0xeb, // 2
		0xfe, 0x1e, // 30
		0xeb, 0xea, // 3
		0xfe, 0x1e, // 30
		0xea, 0xeb, // 2
		0xfe, 0x1d, // 29
		0xec, 0xeb, 0xec, // 3
		0xfe, 0x1d, // 29
		0xea, 0xeb, // 2
		0xfe, 0x1e, // 30
		0xeb, 0xea, // 2
		0xfe, 0x1e, // 30
		0x64, 0x65, 0x66, // 3
		0xfe, 0x1d, // 29
		0x66, 0x66, 0x64, 0x67,  // 4
		0xfe, 0x1d, // 29
		0x66, 0x67, // 2
		0xfe, 0xff, // 255
		0xfe, 0xff, // 255
		0xff
	};
	// TODO: Эта штука и правда декодится в 1164 байта... а должна по идее в 1280...

	stringbuf source(string(reinterpret_cast<const char *>(&encoded[0]), encoded.size()));
	rle_streambuf rle(&source);
	istream in(&rle);

	vector<uint8_t> decoded;
	copy(istream_iterator<uint8_t>(in), istream_iterator<uint8_t>(), back_inserter(decoded));

	ASSERT_EQ(decoded.size(), 32 * 30);
}

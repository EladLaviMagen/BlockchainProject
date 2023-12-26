#include "SHA256.h"
#include <cstring>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <vector>

SHA256::SHA256() : m_blocklen(0), m_bitlen(0) {
	m_state[0] = 0x6a09e667;
	m_state[1] = 0xbb67ae85;
	m_state[2] = 0x3c6ef372;
	m_state[3] = 0xa54ff53a;
	m_state[4] = 0x510e527f;
	m_state[5] = 0x9b05688c;
	m_state[6] = 0x1f83d9ab;
	m_state[7] = 0x5be0cd19;
}

void SHA256::conv(std::string str)
{
	std::vector<uint8_t> data = stringToBits(str);
	int i = 0;
	while (i < data.size())
	{
		m_data[m_blocklen++] = data[i];
		i++;
	}
}

std::vector<uint8_t> stringToBits(const std::string& input) {
	std::vector<uint8_t> result;

	for (char c : input) {
		// Convert each character to its ASCII value and add to the result vector
		result.push_back(static_cast<uint8_t>(c));
	}

	return result;
}
void SHA256::pad() {
	uint64_t i = m_blocklen;

	m_data[i++] = 0x80;
	while (i < 55) {
		m_data[i++] = 0x00; // Pad with zeros
	}
	m_bitlen += m_blocklen * 8;
	m_data[63] = m_bitlen;
	m_data[62] = m_bitlen >> 8;
	m_data[61] = m_bitlen >> 16;
	m_data[60] = m_bitlen >> 24;
	m_data[59] = m_bitlen >> 32;
	m_data[58] = m_bitlen >> 40;
	m_data[57] = m_bitlen >> 48;
	m_data[56] = m_bitlen >> 56;
}

uint64_t SHA256::mod_fin_val(uint32_t state[8])
{
	m_state[0] += state[0];
	m_state[1] += state[1];
	m_state[2] += state[2];
	m_state[3] += state[3];
	m_state[4] += state[4];
	m_state[5] += state[5];
	m_state[6] += state[6];
	m_state[7] += state[7];

	uint64_t result = static_cast<uint64_t>(m_state[0]) << 32 | m_state[1];
	result = (result << 32) | m_state[2];
	result = (result << 32) | m_state[3];
	result = (result << 32) | m_state[4];
	result = (result << 32) | m_state[5];
	result = (result << 32) | m_state[6];
	result = (result << 32) | m_state[7];

	return result;
	
}

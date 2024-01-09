#include "SHA256.h"



SHA256::SHA256() : m_blocklen(0), m_bitlen(0) 
{
	for (int i = 0; i < 64; i++)
	{
		m_data[i] = 0;
	}
	m_state[0] = 0x6a09e667;
	m_state[1] = 0xbb67ae85;
	m_state[2] = 0x3c6ef372;
	m_state[3] = 0xa54ff53a;
	m_state[4] = 0x510e527f;
	m_state[5] = 0x9b05688c;
	m_state[6] = 0x1f83d9ab;
	m_state[7] = 0x5be0cd19;
}

std::string SHA256::conv(std::string str)
{
	std::vector<uint8_t> data = stringToBits(str);
	int i = 0;
	while (i < data.size())
	{
		m_data[m_blocklen++] = data[i];
		i++;
	}
	pad();
	chunkLoop();
	return concatenate();

}

std::vector<uint8_t> SHA256::stringToBits(const std::string& input) {
	std::vector<uint8_t> result;

	for (char c : input) {
		// Convert each character to its ASCII value and add to the result vector
		result.push_back(static_cast<uint8_t>(c));
	}

	return result;
}
void SHA256::pad() {
	uint64_t i = m_blocklen;

	m_data[i++] = 0x80; //10000000
	while (i < 55) {
		m_data[i++] = 0x00; // Pad with zeros
	}
	m_bitlen += m_blocklen * 8;//big indian
	m_data[63] = m_bitlen;
	m_data[62] = m_bitlen >> 8;
	m_data[61] = m_bitlen >> 16;
	m_data[60] = m_bitlen >> 24;
	m_data[59] = m_bitlen >> 32;
	m_data[58] = m_bitlen >> 40;
	m_data[57] = m_bitlen >> 48;
	m_data[56] = m_bitlen >> 56;
}

std::string SHA256::concatenate()
{
	std::string result = "";
	std::stringstream s;
	s << std::setfill('0') << std::hex;
	for (uint8_t i = 0; i < 8; i++) {
		s << std::setw(2) << m_state[i];
	}
	return s.str();
}
void SHA256::chunkLoop()
{
	uint32_t maj = 0;
	uint32_t s_1 = 0;
	uint32_t ch = 0;
	uint32_t s_0 = 0;
	uint32_t temp1 = 0;
	uint32_t temp2 = 0;
	uint32_t w[64];
	for (int i = 0; i < 64; i++)
	{
		w[i] = 0;
	}
	uint32_t state[8];
	for (int i = 0; i < 8; i++)
	{
		state[i] = 0;
	}
	///Step 5 message schedule
	int j = 0;
	for (int i = 0; i < 16; i++)
	{
		w[i] = (m_data[j] << 24) + (m_data[j + 1] << 16) + (m_data[j + 2] << 8) + (m_data[j + 3]);
		j += 4;
	}
	for (int i = 16; i < 64; i++)
	{
		w[i] = 0;
	}

	for (int i = 16; i < 64; i++)
	{
		w[i] = s1(w[i - 2]) + w[i - 7] + s0(w[i - 15]) + w[i - 16];
	}
	///step 6 Commpression

	for (int i = 0; i < 8; i++) 
	{
		state[i] = m_state[i];
	}
	for (int i = 0; i < 64; i++)
	{
		s_1 = SHA256::rotr(state[4], 6) ^ SHA256::rotr(state[4], 11) ^ SHA256::rotr(state[4], 25);
		ch = ch_fuc(state[4], state[5], state[6]);
		temp1 = (state[7] + s_1 + ch + K[i] + w[i]) % 0x100000000;
		s_0 = SHA256::rotr(state[0], 2) ^ SHA256::rotr(state[0], 13) ^ SHA256::rotr(state[0], 22);
		maj = SHA256::maj_func(state[0], state[1], state[2]);
		temp2 = (s_0 + maj) % 0x100000000;
		state[7] = state[6];
		state[6] = state[5];
		state[5] = state[4];
		state[4] = (state[3] + temp1) % 0x100000000;
		state[3] = state[2];
		state[2] = state[1];
		state[1] = state[0];
		state[0] = (temp1 + temp2) % 0x100000000;
	}
	//Step 7
	for (int i = 0; i < 8; i++) {
		m_state[i] = (m_state[i] + state[i]) % 0x100000000;
	}


}

uint32_t SHA256::rotr(uint32_t x, uint32_t n) {
	return (x >> n) | (x << (32 - n));
}

uint32_t SHA256::ch_fuc(uint32_t e, uint32_t f, uint32_t g) {
	return (e & f) ^ (~e & g);
}

uint32_t SHA256::maj_func(uint32_t a, uint32_t b, uint32_t c) {
	return ((a & b) ^ (b & c) ^ (a & c));
}

uint32_t SHA256::s0(uint32_t x) {
	return SHA256::rotr(x, 7) ^ SHA256::rotr(x, 18) ^ (x >> 3);
}

uint32_t SHA256::s1(uint32_t x) {
	return SHA256::rotr(x, 17) ^ SHA256::rotr(x, 19) ^ (x >> 10);
}

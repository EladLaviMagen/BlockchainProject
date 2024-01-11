#include "SHA256.h"

std::vector<uint32_t> SHA256::m_state;

void SHA256::start()
{
	for (int i = 0; i < 8; i++)
	{
		m_state.push_back(0);
	}
}
std::string SHA256::conv(std::string str)
{
	SHA256::m_state.clear();
	start();
	SHA256::m_state[0] = 0x6a09e667;
	SHA256::m_state[1] = 0xbb67ae85;
	SHA256::m_state[2] = 0x3c6ef372;
	SHA256::m_state[3] = 0xa54ff53a;
	SHA256::m_state[4] = 0x510e527f;
	SHA256::m_state[5] = 0x9b05688c;
	SHA256::m_state[6] = 0x1f83d9ab;
	SHA256::m_state[7] = 0x5be0cd19;
	std::vector<uint8_t> m_data = stringToBits(str);
	uint32_t m_blocklen = m_data.size();
	m_data = pad(m_blocklen, m_data);
	for (int i = 0; i < m_data.size() / 64; i++)
	{
		std::vector<uint8_t> chunk;
		for (int j = 64 * i; j < 64 * i + 64; j++)
		{
			chunk.push_back(m_data[j]);
		}
		chunkLoop(chunk);
	}
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
std::vector<uint8_t> SHA256::pad(int m_blocklen, std::vector<uint8_t> m_data)
{
	uint64_t i = m_blocklen + 1;
	uint32_t m_bitlen = m_blocklen * 8;//big endian
	m_data.push_back(0x80);//10000000
	while (i % 64 != 0) 
	{
		m_data.push_back(0);
		i++;
	}
	if (i - m_blocklen < 8)
	{
		for (int j = 0; j < 64; j++)
		{
			m_data.push_back(0);
			i++;
		}
	}
	for (int j = 1; j <= 8; j++)
	{
		m_data[i-j] = m_bitlen % 0x100;
		m_bitlen = m_bitlen >> 8;
	}
	return m_data;
}

std::string SHA256::concatenate()
{
	std::string result = "";
	std::stringstream s;
	s << std::setfill('0') << std::hex;
	for (uint8_t i = 0; i < 8; i++) 
	{
		s << std::setw(8) << SHA256::m_state[i];
	}
	return s.str();
}



void SHA256::chunkLoop(std::vector<uint8_t> m_data)
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
		w[i] = s1(w[i - 2]) + w[i - 7] + s0(w[i - 15]) + w[i - 16] % 0x100000000;
	}
	///step 6 Commpression
	for (int i = 0; i < 8; i++) 
	{
		state[i] = SHA256::m_state[i];
	}
	for (int i = 0; i < 64; i++)
	{
		s_1 = SHA256::rotr(state[4], 6) ^ SHA256::rotr(state[4], 11) ^ SHA256::rotr(state[4], 25);
		ch = ch_fuc(state[4], state[5], state[6]);
		temp1 = (state[7] + s_1 + ch + k[i] + w[i]) % 0x100000000;
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
		SHA256::m_state[i] = (SHA256::m_state[i] + state[i]) % 0x100000000;
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

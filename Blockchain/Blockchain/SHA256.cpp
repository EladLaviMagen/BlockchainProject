#include "SHA256.h"
//Setting static variable
std::vector<uint32_t> SHA256::m_state;

//defines 
#define BITS_512 64
#define BYTE 0x100
#define WORD 0x100000000

std::string SHA256::conv(std::string str)
{
	//Resetting values, preparing for the process
	SHA256::m_state.clear();
	for (int i = 0; i < 8; i++)
	{
		m_state.push_back(0);
	}
	SHA256::m_state[0] = 0x6a09e667;
	SHA256::m_state[1] = 0xbb67ae85;
	SHA256::m_state[2] = 0x3c6ef372;
	SHA256::m_state[3] = 0xa54ff53a;
	SHA256::m_state[4] = 0x510e527f;
	SHA256::m_state[5] = 0x9b05688c;
	SHA256::m_state[6] = 0x1f83d9ab;
	SHA256::m_state[7] = 0x5be0cd19;
	//Splitting data into pieces, each the size of a byte
	std::vector<uint8_t> m_data = stringToBits(str);
	uint32_t m_blocklen = m_data.size();
	//Padding data
	m_data = pad(m_blocklen, m_data);
	//Chunk loop, happens for each 512 bits in the data
	for (int i = 0; i < m_data.size() / BITS_512; i++)
	{
		//Getting the chunk of 512 bits
		std::vector<uint8_t> chunk;
		for (int j = BITS_512 * i; j < BITS_512 * i + BITS_512; j++)
		{
			chunk.push_back(m_data[j]);
		}
		//Performing the actions of the chunk loop
		chunkLoop(chunk);
	}
	return concatenate();

}
bool SHA256::comp(std::string hash, std::string target)
{
	int i = 0;
	while (hash[i] == target[i] && target[i] == '0')
	{
		i++;
	}
	if (target[i] != '0')
	{
		return true;
	}
	return false;
}
/*
* Function will turn a string into "bits" 
* Input : the input string
* Output : a vector where each element is an int the size of a byte, turning every letter in the string to a value 
*/
std::vector<uint8_t> SHA256::stringToBits(const std::string& input) 
{
	std::vector<uint8_t> result;
	//Casting each char in the string to an int the size of a byte
	for (char c : input) {
		// Convert each character to its ASCII value and add to the result vector
		result.push_back(static_cast<uint8_t>(c));
	}
	return result;
}

/*
* Function will pad the input data so it's size is a multiple of 512 bits
* Input : the block length, and the data
* Output : padded vector of the data
*/
std::vector<uint8_t> SHA256::pad(int m_blocklen, std::vector<uint8_t> m_data)
{
	//Getting size in bytes
	uint64_t bytes = m_blocklen + 1;
	uint32_t m_bitlen = m_blocklen * 8;//big endian
	m_data.push_back(0x80);//10000000, appending 1, needs to be added to the data 
	//padding until the data length is a multiple of 512 bits (64 bytes)
	while (bytes % BITS_512 != 0)
	{
		m_data.push_back(0);
		bytes++;
	}
	//there has to be 8 empty bytes at the end of the padded input (for big endian)
	if (bytes - m_blocklen < 8)
	{
		//If there aren't 8 empty bytes at the end of the padded input, we pad to another multiple of 512 bits
		for (int j = 0; j < BITS_512; j++)
		{
			m_data.push_back(0);
			bytes++;
		}
	}
	//Setting big endian at the end of the padded input
	for (int j = 1; j <= 8; j++)
	{
		//We take each byte and insert it in the proper position
		m_data[bytes-j] = m_bitlen % BYTE;
		//Moving to next byte by rightshifting 8 bits
		m_bitlen = m_bitlen >> 8;
	}
	return m_data;
}

/*
* Function will concatenate (append) the hash values and create the final hash value
* Input : None
* Output : Final hash value, in the form of a string 
*/
std::string SHA256::concatenate()
{
	std::string result = "";
	std::stringstream s;
	//Appending the hash values, filling with zeros in case values are too small (have leading zeros)
	s << std::setfill('0') << std::hex;
	for (uint8_t i = 0; i < 8; i++) 
	{
		s << std::setw(8) << SHA256::m_state[i];
	}
	//Returning as string
	return s.str();
}


/*
* Function will perform all actions that are done within the chunk loop
* Input : the data which the hash will be calculated with
* Output : None
*/
void SHA256::chunkLoop(std::vector<uint8_t> m_data)
{
	//Setting up
	uint32_t maj = 0;
	uint32_t s_1 = 0;
	uint32_t ch = 0;
	uint32_t s_0 = 0;
	uint32_t temp1 = 0;
	uint32_t temp2 = 0;
	uint32_t words_data[BITS_512];
	for (int i = 0; i < BITS_512; i++)
	{
		words_data[i] = 0;
	}
	uint32_t state[8];
	int j = 0;
	///Message schedule
	//turning data to chunks of 32 bits each using left shifts
	for (int i = 0; i < 16; i++)
	{
		words_data[i] = (m_data[j] << 24) + (m_data[j + 1] << 16) + (m_data[j + 2] << 8) + (m_data[j + 3]);
		j += 4;
	}
	//Setting the rest of the values
	for (int i = 16; i < BITS_512; i++)
	{
		words_data[i] = s1(words_data[i - 2]) + words_data[i - 7] + s0(words_data[i - 15]) + words_data[i - 16] % 0x100000000;
	}
	///Commpression
	for (int i = 0; i < 8; i++) 
	{
		state[i] = SHA256::m_state[i];
	}
	for (int i = 0; i < BITS_512; i++)
	{
		s_1 = SHA256::rotr(state[4], 6) ^ SHA256::rotr(state[4], 11) ^ SHA256::rotr(state[4], 25);
		ch = ch_fuc(state[4], state[5], state[6]);
		temp1 = (state[7] + s_1 + ch + k[i] + words_data[i]) % WORD;
		s_0 = SHA256::rotr(state[0], 2) ^ SHA256::rotr(state[0], 13) ^ SHA256::rotr(state[0], 22);
		maj = SHA256::maj_func(state[0], state[1], state[2]);
		temp2 = (s_0 + maj) % WORD;
		state[7] = state[6];
		state[6] = state[5];
		state[5] = state[4];
		state[4] = (state[3] + temp1) % WORD;
		state[3] = state[2];
		state[2] = state[1];
		state[1] = state[0];
		state[0] = (temp1 + temp2) % WORD;
	}
	//Step modifying final values with the current hash values
	for (int i = 0; i < 8; i++) {
		SHA256::m_state[i] = (SHA256::m_state[i] + state[i]) % WORD;
	}


}

///////Functions for calculations//////////////////

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

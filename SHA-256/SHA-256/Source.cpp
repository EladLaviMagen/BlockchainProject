#include "SHA256.h"
#include <iostream>

#define CHECK_VAL "0000b273ff34fce19d6b804eff5a3f5747ada4eaa22f1d49c01e52ddb7875b4b"
int main()
{
	std::string str = "";
	std::cin >> str;
	std::cout << SHA256::conv(str);
	/*int i = 0;
	while(true)
	{
		std::string input = "sdsjkdsahdjvbwqjkdbsdlkjafhesjk fhwj fhewuirhyjhuiryewgeduiwoqgrf" + std::to_string(i);
		std::string hash = SHA256::conv(input);
		bool good = true; 
		for (int j = 0; j < 4; j++)
		{
			if (hash[j] != '0')
			{
				good = false;
			}
		}
		if (good)
		{
			std::cout << i << " - " << hash;
			break;
		}
		i++;
	}*/
	return 0;
}
#pragma once

#include<Windows.h>
#include<vector>

uintptr_t GetAdrOf(uintptr_t BaseAdrs, SIZE_T ReadAmmount,std::vector<BYTE> Signature, char* Mask, std::vector<BYTE>& ADRS_RUN_TIME)
{
	bool Isfound = false;
	std::vector<BYTE> Buffer(ReadAmmount);
	uintptr_t Adrs = BaseAdrs;

	while (!Isfound)
	{
		//Reading and storing BYTEs into Buffer
		memcpy(Buffer.data(), reinterpret_cast<BYTE*>(BaseAdrs), ReadAmmount);

		for (int i = 0; i <= ReadAmmount - Signature.size(); i++)
		{
			//If first element matches
			if (Buffer[i] == Signature[0])
			{
				Isfound = true;
				for (int x = 1; x < Signature.size(); x++)
				{
					if (Mask[x] == '?') continue; //Skipping for Wildcard
					else if (Mask[x] == 'x' && Signature[x] != Buffer[i + x])
					{
						Isfound = false;
						break;
					}
				}
				//If Signature Found
				if (Isfound)
				{
					//Clearing array before storing
					ADRS_RUN_TIME.clear();

					//Storing run time adrs
					for (int x = 0; x < Signature.size(); x++)
					{
						ADRS_RUN_TIME.push_back(Buffer[i+x]);
					}

					return Adrs + i;
				}
			}

			//Incrementing for each Byte read
			BaseAdrs++;
		}

		Adrs = BaseAdrs;
	}

	return 0;
}
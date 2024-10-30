#pragma once

#include<Windows.h>

bool Modify(uintptr_t BASE_ADRS, SIZE_T BYTES_TO_CHANGE, BYTE* NEW_BYTES)
{
	DWORD Temp;

	//Changing security
	if (!VirtualProtect((LPVOID)BASE_ADRS, BYTES_TO_CHANGE, PAGE_EXECUTE_READWRITE, &Temp))
	{
		return false;
	}

	//Changing Bytes
	memcpy((void*)BASE_ADRS, NEW_BYTES, sizeof(NEW_BYTES));

	//reseting security
	if (!VirtualProtect((LPVOID)BASE_ADRS, BYTES_TO_CHANGE, Temp, &Temp))
	{
		return false;
	}

	return true;
}
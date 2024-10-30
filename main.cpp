#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<iostream>

#include<cstdint>
#include<thread>
#include<vector>

#include"Helper/Change.h"
#include"Helper/Scanner.h"

//Signature and Mask
std::vector<BYTE> Signature = { 0x89, 0x08, 0x8B, 0x46 ,0x14 ,0xFF ,0x08 ,0x8D ,0x44 ,0x24 ,0x1C ,0x50 ,0x51 };
char Mask[] = "xxxxxxxxxxxxx";

//Creating console for Debug purposes
void CreateConsole()
{
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
}

void HAXMAIN(HINSTANCE Instance) noexcept
{
    CreateConsole();

    HMODULE Client = GetModuleHandle("ac_client.exe");
    if (!Client)
    {
        std::cout << "Failed to get Module Handle\n";
    }
    else
    {
        std::cout << "SUCCESS! : Got the base adrs\n";
    }
    
    uintptr_t BaseAdrs = reinterpret_cast<uintptr_t>(Client);
    SIZE_T BytesToRead = 512;

    //Printing Sig
    std::cout << "\nSignature     : ";
    for (BYTE x : Signature)
    {
        printf("%02x ", x);
    }

    //Signature scan result
    std::vector<BYTE> RUN_TIME_ADRS;

    uintptr_t BASE_OF_AMMO_ADRS = GetAdrOf(BaseAdrs, BytesToRead, Signature, Mask, RUN_TIME_ADRS);
    uintptr_t AMMO_ADRS = BASE_OF_AMMO_ADRS + 0x05; //Adrs of DEC functions

    //Printing Signature scan result
    std::cout << "\nRun Time Adrs : ";
    for (BYTE x : RUN_TIME_ADRS)
    {
        printf("%02x ", x);
    }

    //New Bytes
    BYTE NEW_BYTES[] = { 0xFF, 0x00 };

    //Displaying the BYTEs to change
    std::vector<BYTE> TO_CHANGE(2);
    if(BaseAdrs>0) memcpy(TO_CHANGE.data(), reinterpret_cast<BYTE*>(AMMO_ADRS), 2);
    printf("\n%02x %02x -> %02x %02x\n", TO_CHANGE[0], TO_CHANGE[1], NEW_BYTES[0], NEW_BYTES[1]);

    //Modifying Memory
    if (!Modify(AMMO_ADRS, 2, NEW_BYTES))
    {
        std::cout << "\nFailed to Change Bytes";
    }

    FreeConsole();
    
    //uninject
    FreeLibraryAndExitThread(Instance, 0);

    return;
}

BOOL WINAPI DllMain(
    HINSTANCE Instance,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved)  // reserved
{
    //on attach
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(Instance);

        //creating hack thread
        auto HackThread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(HAXMAIN), Instance, 0, nullptr);

        if (HackThread) CloseHandle(HackThread);
    }

    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
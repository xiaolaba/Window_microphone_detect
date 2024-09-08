// g++ -Wall -O2 mic_name.cpp -o mic_name.exe -lwinmm
// xiaolaba, 2010
// testing, bug, UTF8 , waveInCaps.szPname has lemgth 31, UTF8/WCHAR no proper handing. crash
// Storage : C:\MinGW\msys\1.0\home\user0\mic_detect

// output like this
/*
$ mic_name.exe
 strlen waveInCaps.szPname 31
Device 0:
  Manufacturer ID: 0x0001
  Product ID: 0xffff
  Driver Version: 0x0000
  Formats: 0x000fffff
  Channels: 0x0002
  Device Name:  strlen waveInCaps.szPname 31
 strlen waveInCaps.szPname 31
 strlen waveInCaps.szPname 31
 strlen waveInCaps.szPname 31
 strlen waveInCaps.szPname 31
 strlen waveInCaps.szPname 31
 strlen waveInCaps.szPname 31
*/

#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <conio.h>  // For _kbhit()
#include <iomanip>  // For std::hex and std::setw
#include <string>
#include <cstring>  // For strlen

// Function to interpret and display format information
std::wstring GetFormatString(DWORD formats) {
    std::wstring formatString;
    
    if (formats & WAVE_FORMAT_1M08) formatString += L"1-bit mono, ";
    if (formats & WAVE_FORMAT_1S08) formatString += L"8-bit mono, ";
    if (formats & WAVE_FORMAT_2M08) formatString += L"2-bit mono, ";
    if (formats & WAVE_FORMAT_2S08) formatString += L"16-bit mono, ";
    if (formats & WAVE_FORMAT_4M08) formatString += L"4-bit mono, ";
    if (formats & WAVE_FORMAT_4S08) formatString += L"32-bit mono, ";
    if (formats & WAVE_FORMAT_1M16) formatString += L"1-bit stereo, ";
    if (formats & WAVE_FORMAT_1S16) formatString += L"8-bit stereo, ";
    if (formats & WAVE_FORMAT_2M16) formatString += L"2-bit stereo, ";
    if (formats & WAVE_FORMAT_2S16) formatString += L"16-bit stereo, ";
    if (formats & WAVE_FORMAT_4M16) formatString += L"4-bit stereo, ";
    if (formats & WAVE_FORMAT_4S16) formatString += L"32-bit stereo, ";
    
    if (!formatString.empty()) {
        formatString.pop_back(); // Remove trailing comma
        formatString.pop_back(); // Remove trailing space
    } else {
        formatString = L"Unknown";
    }

    return formatString;
}




int main() {
    UINT numDevices = waveInGetNumDevs();  // Get the number of available input devices

    if (numDevices == 0) {
        std::cout << "No wave input devices available." << std::endl;
        return 1;
    }

    WAVEINCAPS waveInCaps;  // Structure to hold device information

    UINT i = 0;

    while (1) {
        for (i = 0; i < numDevices; ++i) {
            MMRESULT result = waveInGetDevCaps(i, &waveInCaps, sizeof(WAVEINCAPS));
            
            if (result != MMSYSERR_NOERROR) {
                std::cout << "Failed to get device capabilities for device " << i << std::endl;
                break;
            }

            // Use strlen for narrow character strings
            std::cout << " strlen waveInCaps.szPname " << strlen(waveInCaps.szPname) << std::endl;

            // Print ON/OFF status based on the device name
            std::wcout << L"Device " << i << L":" << std::endl;
            std::wcout << L"  Manufacturer ID: 0x" << std::hex << std::setw(4) << std::setfill(L'0') << waveInCaps.wMid << std::endl;
            std::wcout << L"  Product ID: 0x" << std::hex << std::setw(4) << std::setfill(L'0') << waveInCaps.wPid << std::endl;
            std::wcout << L"  Driver Version: 0x" << std::hex << std::setw(4) << std::setfill(L'0') << waveInCaps.vDriverVersion << std::endl;
            std::wcout << L"  Formats: 0x" << std::hex << std::setw(8) << std::setfill(L'0') << waveInCaps.dwFormats << std::endl;
            std::wcout << L"  Channels: 0x" << std::hex << std::setw(4) << std::setfill(L'0') << waveInCaps.wChannels << std::endl;
            
            std::wcout << L"  Device Name: " << waveInCaps.szPname << std::endl << std::endl;
            std::wcout << std::endl;
            
            if (waveInCaps.szPname[0] == '\0') {
                std::wcout << L"OFF Device " << i << L": " << waveInCaps.szPname << std::endl;
            } else {
                std::wcout << L"ON Device " << i << L": " << waveInCaps.szPname << std::endl;
            }
        }

        // Check if any key is pressed to exit
        if (_kbhit()) {
            std::cout << "\r\nExiting..." << std::endl;
            break;
        }

        Sleep(3000);  // Wait 3 seconds before checking again
    }

    return 0;
}

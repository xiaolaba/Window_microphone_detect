// g++ -Wall -O2 microphone_detect.cpp -o microphone_detect.exe -lwinmm
// xiaolaba, 2010
// testing, imcopmlete, only show DEvice 0 / messagebox " plugged in"
// no uses, bug
// Storage : C:\MinGW\msys\1.0\home\user0\mic_detect


#include <windows.h>
#include <mmsystem.h>
#include <iostream>

// Callback function to handle device change events
void CALLBACK DeviceChangeCallback(
    HWAVEIN hwi,
    UINT uMsg,
    DWORD_PTR dwInstance,
    DWORD_PTR dwParam1,
    DWORD_PTR dwParam2
) {
    if (uMsg == MM_WIM_OPEN) {
        // Microphone was plugged in
        MessageBox(NULL, "Microphone plugged in", "Notification", MB_OK);
    } else if (uMsg == MM_WIM_CLOSE) {
        // Microphone was unplugged
        MessageBox(NULL, "Microphone unplugged", "Notification", MB_OK);
    }
}

int main() {
    WAVEINCAPS waveInCaps;	// Structure to hold device information
    MMRESULT result;

    // Check the number of wave input devices
    UINT numDevices = waveInGetNumDevs();
    if (numDevices == 0) {
        MessageBox(NULL, "No wave input devices available.", "Error", MB_OK);
        return 1;
    }

   for (UINT i = 0; i < numDevices; ++i) {
        MMRESULT result = waveInGetDevCaps(i, &waveInCaps, sizeof(WAVEINCAPS));
        if (result == MMSYSERR_NOERROR) {
            std::wcout << L"Device " << i << L": " << waveInCaps.szPname << std::endl;  // Print the device name
        } else {
            std::cout << "Failed to get device capabilities for device " << i << std::endl;
        }
    }



    // Get information about the first device
    result = waveInGetDevCaps(0, &waveInCaps, sizeof(WAVEINCAPS));
    if (result != MMSYSERR_NOERROR) {
        MessageBox(NULL, "Failed to get wave input device capabilities.", "Error", MB_OK);
        return 1;
    }

    // Define wave format
    WAVEFORMATEX waveFormat;
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = 1; // Mono
    waveFormat.nSamplesPerSec = 44100; // 44.1kHz
    waveFormat.wBitsPerSample = 16; // 16-bit samples
    waveFormat.nBlockAlign = (waveFormat.nChannels * waveFormat.wBitsPerSample) / 8;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;

    // Open the wave input device
    HWAVEIN hWaveIn;
    result = waveInOpen(&hWaveIn, 1, &waveFormat, (DWORD_PTR)DeviceChangeCallback, 0, CALLBACK_FUNCTION);
    if (result != MMSYSERR_NOERROR) {
        MessageBox(NULL, "Failed to open wave input device.", "Error", MB_OK);
        return 1;
    }

    // Run a message loop to process events
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Close the wave input device
    waveInClose(hWaveIn);
    return 0;
}

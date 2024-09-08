// gcc -o waveincaps waveincaps.c -lwinmm
// xiaolaba, 2010
// testing ok, UTF8 , waveInCaps.szPname has lemgth 31

// output like this, number of audio input devices will has change and trigger once micro phone is plugge in/out
/*
$ waveincaps.exe
Initial number of audio input devices: 2
Device plugged in. New number of devices: 3
Device #0:
Name: left 麥克風 (2- High Definition
Manufacturer ID: 1
Product ID: 65535
Driver Version: 0.0
Formats Supported: 1048575
Channels: 2

Device #1:
Name: 麥克風 (2- High Definition Audi
Manufacturer ID: 1
Product ID: 65535
Driver Version: 0.0
Formats Supported: 1048575
Channels: 2

Device #2:
Name: 麥克風 (DroidCam Virtual Audio)
Manufacturer ID: 1
Product ID: 101
Driver Version: 10.0
Formats Supported: 1048575
Channels: 2

Device unplugged. New number of devices: 2
Device #0:
Name: 麥克風 (2- High Definition Audi
Manufacturer ID: 1
Product ID: 65535
Driver Version: 0.0
Formats Supported: 1048575
Channels: 2

Device #1:
Name: 麥克風 (DroidCam Virtual Audio)
Manufacturer ID: 1
Product ID: 101
Driver Version: 10.0
Formats Supported: 1048575
Channels: 2
*/

#include <windows.h>
#include <stdio.h>

void printWaveInCaps() {
    WAVEINCAPS waveInCaps;
    UINT numDevices = waveInGetNumDevs();  // Get the number of audio input devices

    for (UINT i = 0; i < numDevices; i++) {
        // Get the capabilities of the device
        if (waveInGetDevCaps(i, &waveInCaps, sizeof(WAVEINCAPS)) == MMSYSERR_NOERROR) {
            printf("Device #%d:\n", i);
            printf("Name: %s\n", waveInCaps.szPname);
            printf("Manufacturer ID: %u\n", waveInCaps.wMid);
            printf("Product ID: %u\n", waveInCaps.wPid);
            printf("Driver Version: %u.%u\n", 
                   waveInCaps.vDriverVersion >> 8, waveInCaps.vDriverVersion & 0xFF);
            printf("Formats Supported: %u\n", waveInCaps.dwFormats);
            printf("Channels: %u\n", waveInCaps.wChannels);
            printf("\n");
        } else {
            printf("Error getting device capabilities for device #%d\n", i);
        }
    }
}

int main() {
    UINT prevNumDevices = waveInGetNumDevs();
    UINT currNumDevices;

    printf("Initial number of audio input devices: %d\n", prevNumDevices);

    while (1) {
        currNumDevices = waveInGetNumDevs();  // Get the number of audio input devices

        if (currNumDevices != prevNumDevices) {
            if (currNumDevices > prevNumDevices) {
                printf("Device plugged in. New number of devices: %d\n", currNumDevices);
            } else {
                printf("Device unplugged. New number of devices: %d\n", currNumDevices);
            }
            prevNumDevices = currNumDevices;
            printWaveInCaps();  // Print the updated list of devices
        }

        Sleep(3000);  // Check every 1 second
    }

    return 0;
}

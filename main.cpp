#include "beacon-flood.hpp"
#include <vector>

typedef unsigned char BYTE;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("syntax : beacon-flood <interface> <ssid-list-file>\n");
        printf("sample : beacon-flood wlan0mon ssid-list.txt\n");
        return -1;
    }

    std::vector<std::string> SSIDList;

    char *dev = argv[1];
    char *ssid_list_file_name = argv[2];

    FILE *fp = fopen(ssid_list_file_name, "r");

    if (fp == NULL)
    {
        printf("File open failed\n");
        return -1;
    }

    char ssid[256] = { 0, };

    while (fgets(ssid, 256, fp))
    {
        ssid[strlen(ssid) - 1] = 0x00;
        SSIDList.push_back(std::string(ssid));
    }

    char command[256];
    sprintf(command, "iwconfig %s channel 2", dev);
    system(command);

    BeaconPacket beaconpacket(dev);
    while (true)
    {
        for (int i = 0; i < SSIDList.size(); i++)
        {
            beaconpacket.setSSID(SSIDList[i]);
            beaconpacket.PacketSend();
        }
        usleep(102400);
    }
}
#ifndef _BEACON_FLOOD_HPP_
#define _BEACON_FLOOD_HPP_

#include <string>
#include <map>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <pcap.h>

#define FALSE 0
#define TRUE 1

typedef bool                BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned int        DWORD;
typedef unsigned long long  QWORD;

class BeaconPacket
{
private:
    pcap_t *pcap = nullptr;
    std::string ssid;

public:
    BeaconPacket();
    BeaconPacket(char *dev);
    bool setSSID(const char *c_ssid);
    bool setSSID(const std::string ssid);
    bool PacketSend();
    bool PacketSend(pcap_t *pcap);
};

/* #pragma pack(push, 1)
typedef struct _Radiotap
{
    BYTE HeaderRevision = 0;
    BYTE HeaderPad = 0;
    WORD HeaderLength;
    DWORD PresentFlags = 0;
} Radiotap;
#pragma pack(pop)

typedef struct _BeaconFrame
{
    union
    {
        struct
        {
            WORD Version : 2;
            WORD Type    : 2;
            WORD Subtype : 4;
        };
        WORD FrameControlField = 0x0080;
    };
    WORD Duration = 0;
    union
    {
        BYTE ReceiverMac[6];
        BYTE DestinationMac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    };
    union
    {
        BYTE TransmitterMac[6];
        BYTE SourceMac[6];
    };
    BYTE BSSID[6];
    WORD FragmentNumber : 4;
    WORD SequenceNumber : 12;
} BeaconFrame;

#pragma pack(push, 1)
typedef struct _WirelessManagement
{
    struct
    {
        QWORD Timestamp;
        WORD BeaconInterval = (WORD)0x0064;
        WORD CapabilitiesInformation = (WORD)0x0431;
    } Fixed;
    struct
    {
        BYTE TagNumber = 3;
        BYTE Length = 1;
        BYTE Channel = 2;
    } DS;
    struct
    {
        BYTE TagNumber = 0;
        BYTE Length;
    };
    
} WirelessManagement;
#pragma pack(pop) */

#endif
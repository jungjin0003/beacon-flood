#include "beacon-flood.hpp"

std::map<std::string, BYTE *> ssid_mac_map;

BYTE Beacon[] = { 0x00, 0x00, 0x0b, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0x1d, 0x41, 0x7b, 0xaf, 0xfd, 0x87, 0x1d, 0x41, 0x7b, 0xaf, 0xfd, 0x00, 0x00, 0x1d, 0x94, 0x85, 0xf0, 0x7e, 0x01, 0x00, 0x00, 0x64, 0x00, 0x31, 0x04, 0x03, 0x01, 0x02, 0x00, 0x04 };
BYTE OUIList[][3] = {{ 0x00, 0x26, 0x37 }, { 0x00, 0x21, 0x19 }, {0x84, 0x0B, 0x2D }};

unsigned long long current_timestamp() 
{
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    unsigned long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}

BeaconPacket::BeaconPacket()
{
    srand(time(NULL));
}

BeaconPacket::BeaconPacket(char *dev) : BeaconPacket::BeaconPacket()
{
    char errbuf[PCAP_BUF_SIZE];
    this->pcap = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);

    if (this->pcap == NULL)
    {
        fprintf(stderr, "pcap_open_live(%s) return null - %s\n", dev, errbuf);
        exit(-1);
    }
}

bool BeaconPacket::PacketSend()
{
    return this->PacketSend(this->pcap);
}

bool BeaconPacket::setSSID(const char *c_ssid)
{
    return this->setSSID(std::string(c_ssid));
}

bool BeaconPacket::setSSID(const std::string ssid)
{
    this->ssid = ssid;
    return true;
}

bool BeaconPacket::PacketSend(pcap_t *pcap)
{
    unsigned int ssid_length = strlen(this->ssid.c_str());

    BYTE *Packet = new BYTE[sizeof(Beacon) + ssid_length];
    memcpy(Packet, Beacon, sizeof(Beacon));
    Packet[sizeof(Beacon) - 1] = ssid_length;
    memcpy(Packet + sizeof(Beacon), this->ssid.c_str(), ssid_length);

    if (ssid_mac_map.find(this->ssid.c_str()) == ssid_mac_map.end())
    {
        BYTE *mac = new BYTE[6];
        
        int oui = rand() % 3;
        for (int i = 0; i < 3; i++)
        {
            mac[i] = OUIList[oui][i];
        }

        for (int i = 3; i < 6; i++)
        {
            mac[i] = rand() % 0xff;
        }
        ssid_mac_map.insert({this->ssid.c_str(), mac});
    }

    memcpy(Packet + 21, ssid_mac_map[this->ssid.c_str()], 6);
    memcpy(Packet + 27, ssid_mac_map[this->ssid.c_str()], 6);

    *(unsigned long long *)(Packet + 35) = current_timestamp();

    int ret = pcap_sendpacket(pcap, Packet, sizeof(Beacon) + ssid_length);

    delete[] Packet;

    return ret == 0 ? true : false;
}
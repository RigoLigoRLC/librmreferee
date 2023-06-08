
#include "librmreferee/enums.h"
#include "librmreferee/packets.h"
#include "librmreferee/rmreferee.h"
#include <iostream>
#include <cstring>
using std::cout;
using std::hex;
using std::dec;

void crcTest() {
    const char * crcString = "RoboMaster";
    int strLength = std::strlen(crcString);
    int crc8 = RMReferee::Crc8(crcString, crcString + strLength, 0xff);
    if (crc8 != 0xd2) {
        cout << "FAIL: CRC8 Test. String \"" << crcString << "\", expect 0xd2, got 0x" << hex << crc8 << dec << ".\n";
    }

    int crc16 = RMReferee::Crc16(crcString, crcString + strLength, 0xffff);
    if (crc16 != 0xef3d) {
        cout << "FAIL: CRC16 Test. String \"" << crcString << "\", expect 0xef3d, got 0x" << hex << crc16 << dec << ".\n";
    }
}

void packetRecvTest() {
    const char * testPacket =
    "\xf0\xa5\x0b\xad\xf0\x0d" // Garbage data
    "\xa5\x20\x00\x05\x68\x03\x00\x64\x00\x64\x00\x64\x00\x64\x00\x64\x00\x64\x00\xb0\x04\xd0\x07"
    "\x64\x00\x64\x00\x64\x00\x64\x00\x64\x00\x64\x00\xb0\x04\xd0\x07\xf3\xbe" // RobotHPPacket
    "\xef\xa5" // Garbage data
    ;

    RMReferee::RefereeRecvControl recvCtl;
    recvCtl.FeedSerialPortBytes((uint8_t*)(testPacket), 47);

    if (recvCtl.AvailablePacketsCount() != 1) {
        cout << "FAIL: Packet receive test. Expected packet count 1, got " << recvCtl.AvailablePacketsCount() << '\n';
        return;
    }

    auto packet = recvCtl.GetNextPacket();
    if (packet->PacketId() != uint16_t(RMReferee::PacketType::RobotHP)) {
        cout << "FAIL: Packet receive test. Expected type " << uint16_t(RMReferee::PacketType::RobotHP) << ", got " << packet->PacketId() << '\n';
        return;
    }
}

int main() {
    crcTest();
    packetRecvTest();
}

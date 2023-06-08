
#include "librmreferee/packets.h"
#include "librmreferee/crc.h"
#include "librmreferee/rmreferee.h"
#include <cstring>
namespace RMReferee {
    bool BuildPacket(uint8_t *outBuffer, size_t bufferSize, uint16_t dataLength, uint8_t seq, uint16_t packetId, const void *data) {
        if (bufferSize < dataLength + RefereePacket::PacketExtrasLength)
            return false;

        uint8_t* writePtr = outBuffer;

        // Write packet header
        PacketHeader header { RefereePacket::StartOfFrame, dataLength, seq };
        header.CRC8 = PacketHeaderChecksum(&header);
        std::memcpy(writePtr, &header, sizeof(header));
        writePtr += sizeof(header);

        // Write packet ID
        std::memcpy(writePtr, &packetId, sizeof(packetId));
        writePtr += sizeof(packetId);

        // Write packet data body
        std::memcpy(writePtr, data, dataLength);
        writePtr += dataLength;

        // Write CRC16 checksum
        auto packetChecksum = PacketChecksum(outBuffer, dataLength);
        std::memcpy(writePtr, &packetChecksum, sizeof(packetChecksum));
        return true;
    }

    bool GameStatusPacket::Serialize(char *buffer, size_t bufferSize) {
        return BuildPacket((uint8_t*)buffer, bufferSize, sizeof(S), m_sequenceNumber, PacketId(), &m_data);
    }

    bool GameResultPacket::Serialize(char *buffer, size_t bufferSize) {
        return BuildPacket((uint8_t*)buffer, bufferSize, sizeof(S), m_sequenceNumber, PacketId(), &m_data);
    }

    bool RobotHPPacket::Serialize(char *buffer, size_t bufferSize) {
        return BuildPacket((uint8_t*)buffer, bufferSize, sizeof(S), m_sequenceNumber, PacketId(), &m_data);
    }

    bool RobotStatePacket::Serialize(char *buffer, size_t bufferSize) {
        return BuildPacket((uint8_t*)buffer, bufferSize, sizeof(S), m_sequenceNumber, PacketId(), &m_data);
    }

    bool HeatPowerStatePacket::Serialize(char *buffer, size_t bufferSize) {
        return BuildPacket((uint8_t*)buffer, bufferSize, sizeof(S), m_sequenceNumber, PacketId(), &m_data);
    }
};

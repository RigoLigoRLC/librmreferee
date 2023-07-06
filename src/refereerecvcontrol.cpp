
#include <array>
#include <ranges> // C++20
#include <vector>
#include "librmreferee/crc.h"
#include "librmreferee/log.h"
#include "librmreferee/packets.h"
#include "librmreferee/rmreferee.h"

namespace RMReferee {
    void RefereeRecvControl::FeedSerialPortBytes(const uint8_t *buf, size_t count) {
        // Append new data
        m_dataFifo.insert(m_dataFifo.end(), buf, buf + count);

        // Look for valid packets
        SearchPacketsInFifo();
    }

    size_t RefereeRecvControl::AvailablePacketsCount() {
        return m_packetQueue.size();
    }

    std::unique_ptr<RefereePacket> RefereeRecvControl::GetNextPacket() {
        auto ret = std::move(m_packetQueue.front());
        m_packetQueue.pop();
        return ret;
    }

    size_t RefereeRecvControl::SearchPacketsInFifo() {
        size_t ret = 0;

        // As long as the remaining bytes' length allows a minimal packet (PacketHeader + PacketType16 + CRC16),
        // try searching a packet.
        while (m_dataFifo.size() >= RefereePacket::PacketExtrasLength) {
            if (GrabOnePacket()) {
                ++ret;
            }
        }

        return ret;
    }

    size_t RefereeRecvControl::SearchPacket() {
        // Discard bytes at front until met with an SOF byte
        while (m_dataFifo.front() != RefereePacket::StartOfFrame) {
            m_dataFifo.pop_front();
            if (m_dataFifo.empty())
                return 0;
        }

        // Check packet header CRC8.
        uint8_t packetHeaderCrc = Crc8(m_dataFifo.cbegin(),
                                       m_dataFifo.cbegin() + sizeof(PacketHeader) - 1,
                                       0xff);
        if (packetHeaderCrc != m_dataFifo[sizeof(PacketHeader) - 1]) {
            // Should forcibly pop a byte on failure to ensure we go to next possible packet
            m_dataFifo.pop_front();
            return 0;
        }

        // Fetch the packet header
        PacketHeader header;
        std::copy(m_dataFifo.cbegin(), m_dataFifo.cbegin() + sizeof(PacketHeader), (uint8_t*)(&header));

        // Check packet size
        if (m_dataFifo.size() < RefereePacket::PacketExtrasLength + header.DataLength) {
            // Do not pop front here as we may still have data to receive
            return 0;
        }
        
        // Check packet body CRC16.
        size_t receivedCrcOffset = RefereePacket::PacketExtrasLength + header.DataLength - 2; // Minus the CRC16
        uint16_t packetCrc = Crc16(m_dataFifo.cbegin(),
                                   m_dataFifo.cbegin() + receivedCrcOffset,
                                   0xffff);
        uint16_t receivedCrc = (m_dataFifo[receivedCrcOffset]) | (m_dataFifo[receivedCrcOffset + 1] << 8);
        if (packetCrc != receivedCrc) {
            m_dataFifo.pop_front();
            return 0;
        }
        
        // CRC checks passed, consider this block of data an actual packet.
        return receivedCrcOffset + 2;
    }

    bool RefereeRecvControl::GrabOnePacket() {
        // Search for a packet first
        auto packetSize = SearchPacket();
        if (!packetSize)
            return false;

        // Try to create this packet in factory, and also remove the corresponding data from FIFO
        std::vector<uint8_t> packetBuf(m_dataFifo.cbegin(), m_dataFifo.cbegin() + packetSize);
        m_dataFifo.erase(m_dataFifo.cbegin(), m_dataFifo.cbegin() + packetSize);
        RefereePacket* packet = m_factory.TryMakePacket(packetBuf.data(), packetBuf.size());

        if (!packet) {
            const uint16_t *packetId = reinterpret_cast<const uint16_t*>(packetBuf.data() + sizeof(PacketHeader));
            Log::Log() << "[librmreferee] No factory method for packet 0x" << Log::hex << *packetId << Log::dec << ".\n";
        }

        // Put this packet into packet queue
        m_packetQueue.push(std::unique_ptr<RefereePacket>(packet));
        return true;
    }
};


#pragma once

#include <deque>
#include <memory>
#include <queue>
#include <map>
#include <functional>
#include "packets.h"
#include "crc.h"

namespace RMReferee {
    class PacketFactory {
    public:
        PacketFactory();
        ~PacketFactory() {}

        template<typename T> bool AddToFactory() {
            if (m_factoryMethods.contains(T::ClassPacketId()))
                return false;
            m_factoryMethods[T::ClassPacketId()] = T::Make;
            return true;
        }

        RefereePacket* TryMakePacket(const uint8_t *buf, size_t count) const;

    protected:
        std::map<uint16_t, std::function<RefereePacket*(const char *)>> m_factoryMethods;
    };

    class RefereeRecvControl {
    public:
        RefereeRecvControl() {}
        ~RefereeRecvControl() {}

        /**
            @brief Serial port receive handler. Feed received data into this method.
            @param buf Serial port buffer
            @param count Bytes feeding into this class
        */
        void FeedSerialPortBytes(const uint8_t *buf, size_t count);

        size_t AvailablePacketsCount();
        std::unique_ptr<RefereePacket> GetNextPacket();

    protected:
        std::deque<uint8_t> m_dataFifo;
        std::queue<std::unique_ptr<RefereePacket>> m_packetQueue;
        PacketFactory m_factory;

        /**
            @brief Searches for all possible valid packets in FIFO. Automatically called when feeding data.
                   Found packets would be appended into the queue.
        */
        size_t SearchPacketsInFifo();

        /**
            @brief Search for a valid packet. Bytes preceding a valid SOF would be all discarded,
                   but if a valid packet is found, the packet is kept intact. If an SOF is found but
                   the packet verification is not passed, SOF will be discarded and return zero.
            @return If zero, no valid packet is found. If not zero, indicates size of the found packet.
        */
        size_t SearchPacket();

        /**
            @brief Try to grab one packet and append to packet queue. Will call SearchPacket internally.
            @return false if no valid packet is found, true if a packet is found and appended to queue.
        */
        bool GrabOnePacket();
    };

    // Build packet utility function
    bool BuildPacket(uint8_t* outBuffer, size_t bufferSize, uint16_t dataLength, uint8_t seq, uint16_t packetId, const void* data);
};

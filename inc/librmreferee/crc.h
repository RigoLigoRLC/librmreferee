
#pragma once

/*
    RoboMaster Referee System uses CRC-8/MAXIM convention, but the algorithm given in the manual
    of referee system serial port protocol appendix does not seem to match other CRC generators.
    Be sure to test with official implementation only.
    CRC-16 uses CRC-16/MCRF4XX convention and matches other implementations.
*/

#include <type_traits>
#include <iterator>
#include <cstdint>
#include "packets.h"

namespace RMReferee {
    extern const uint8_t Crc8Lut[];
    extern const uint16_t Crc16Lut[];

    template <class I>
    uint8_t Crc8(I begin, I end, uint8_t crcBegin) {
        uint8_t ret = crcBegin;
        for (I it = begin; it != end; it++) {
            ret = Crc8Lut[ret ^ (*it)];
        }
        return ret;
    }

    template <class I>
    uint16_t Crc16(I begin, I end, uint16_t crcBegin) {
        uint16_t ret = crcBegin;
        for (I it = begin; it != end; it++) {
            ret = (ret >> 8) ^ Crc16Lut[(ret ^ uint16_t(*it)) & 0xff];
        }
        return ret;
    }

    inline uint8_t PacketHeaderChecksum(const PacketHeader *header) {
        return Crc8((uint8_t*)header, ((uint8_t*)(header + 1)) - 1, 0xff);
    }

    inline uint16_t PacketChecksum(const void *packet, size_t dataLength) {
        return Crc16((uint8_t*)packet, ((uint8_t*)packet) + RefereePacket::PacketExtrasLength + dataLength, 0xffff);
    }
};

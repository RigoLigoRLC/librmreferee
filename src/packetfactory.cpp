
#include "librmreferee/packets.h"
#include "librmreferee/rmreferee.h"

namespace RMReferee {
    PacketFactory::PacketFactory() {
        AddToFactory<GameStatusPacket>();
        AddToFactory<GameResultPacket>();
        AddToFactory<RobotHPPacket>();
        AddToFactory<RobotStatePacket>();
        AddToFactory<HeatPowerStatePacket>();
    }

    RefereePacket* PacketFactory::TryMakePacket(const uint8_t *buf, size_t count) const {
        // Find a factory method
        const uint16_t *packetId = reinterpret_cast<const uint16_t*>(buf + sizeof(PacketHeader));
        if (!m_factoryMethods.contains(*packetId))
            return nullptr;
        
        // Construct packet
        for(auto it = m_factoryMethods.lower_bound(*packetId); it != m_factoryMethods.upper_bound(*packetId); ++it)
            if (auto packet = it->second(reinterpret_cast<const char *>(buf)))
                return packet;
        
        return nullptr;
    }
};


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
        auto factoryMethod = m_factoryMethods.at(*packetId);
        return factoryMethod(reinterpret_cast<const char *>(buf));
    }
};

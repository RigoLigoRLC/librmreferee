
#pragma once
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <cstring>

#include "enums.h"
#include "librmreferee/structs.h"
#include "platform.h"

/*
    Referee system packets definition
*/

/*
TEMPLATE

    class _____ : public RefereePacket {
    public:
        struct S {
            ......
        } REFEREE_PACKED;
        _____() : RefereePacket() {}
        _____(const char *buf) : RefereePacket() { REFEREE_COPYDATA; }
        _____(S data, uint8_t seq = 0) : RefereePacket() { m_data = data; m_sequenceNumber = seq; }

        static uint16_t ClassPacketId() { return uint16_t(PacketType::______); }
        virtual uint16_t PacketId() override { return ClassPacketId(); }
        virtual uint16_t DataLength() override { return sizeof(S); }

        static RefereePacket* Make(const char* buf) { return new _____(buf); }
        
        virtual bool Serialize(char* buffer, size_t bufferSize) override;

    protected:
        S m_data;
    };

*/

namespace RMReferee {
    #define REFEREE_COPYDATA ::memcpy(&m_data, buf, sizeof(S))

    // Packet header structure
    struct PacketHeader {
        uint8_t SOF;
        uint16_t DataLength;
        uint8_t SequenceNumber;
        uint8_t CRC8;
    } REFEREE_PACKED;
    static_assert(sizeof(PacketHeader) == 5, "PacketHeader must be 5 bytes long with packing");

    // Base packet
    class RefereePacket {
    public:
        RefereePacket() {
            m_sequenceNumber = 0;
        };
        virtual ~RefereePacket() {}

        static uint16_t ClassPacketId() { return 0; }
        virtual uint16_t PacketId() { return ClassPacketId(); }
        virtual uint16_t DataLength() = 0;

        virtual uint8_t* Serialize() {
            uint16_t length = DataLength() + PacketExtrasLength;
            uint8_t* ret = new uint8_t[length];
            Serialize(reinterpret_cast<char*>(ret), length);
            return ret;
        };
        virtual bool Serialize(char* buffer, size_t bufferSize) = 0;

        uint8_t SequenceNumber() { return m_sequenceNumber; }
        void SetSequenceNumber(uint8_t seq) { m_sequenceNumber = seq; }

        static constexpr uint8_t StartOfFrame = 0xa5;

        // Length of extra packet data (parts excl. data body)
        // PacketHeader + PacketId + Crc16
        static constexpr size_t PacketExtrasLength = sizeof(PacketHeader) + 2 + 2;

    protected:
        uint8_t m_sequenceNumber;
    };

    // 0x0001 Game status
    class GameStatusPacket : public RefereePacket {
    public:
        struct S {
            GameType gameType : 4;
            GameStage gameStage : 4;
            uint16_t currentStageTimeRemaining;
            uint64_t unixTimestamp;
        } REFEREE_PACKED;
        GameStatusPacket() : RefereePacket() {}
        GameStatusPacket(const char *buf) : RefereePacket() { REFEREE_COPYDATA; }
        GameStatusPacket(S data, uint8_t seq = 0) : RefereePacket() { m_data = data; m_sequenceNumber = seq; }

        static uint16_t ClassPacketId() { return uint16_t(PacketType::GameStatus); }
        virtual uint16_t PacketId() override { return ClassPacketId(); }
        virtual uint16_t DataLength() override { return sizeof(S); }

        static RefereePacket* Make(const char* buf) { return new GameStatusPacket(buf); }

        virtual bool Serialize(char* buffer, size_t bufferSize) override;

    protected:
        S m_data;
    };

    // 0x0002 Game result
    class GameResultPacket : public RefereePacket {
    public:
        struct S {
            GameResult gameResult;
        } REFEREE_PACKED;
        GameResultPacket() : RefereePacket() {}
        GameResultPacket(const char *buf) : RefereePacket() { REFEREE_COPYDATA; }
        GameResultPacket(S data, uint8_t seq = 0) : RefereePacket() { m_data = data; m_sequenceNumber = seq; }

        static uint16_t ClassPacketId() { return uint16_t(PacketType::GameResultEvent); }
        virtual uint16_t PacketId() override { return ClassPacketId(); }
        virtual uint16_t DataLength() override { return sizeof(S); }

        static RefereePacket* Make(const char* buf) { return new GameResultPacket(buf); }

        virtual bool Serialize(char* buffer, size_t bufferSize) override;

    protected:
        S m_data;
    };

    // 0x0003 Robot HP
    class RobotHPPacket : public RefereePacket {
    public:
        struct S {
            uint16_t Red1, Red2, Red3, Red4, Red5, Red7, RedOutpost, RedBase;
            uint16_t Blue1, Blue2, Blue3, Blue4, Blue5, Blue7, BlueOutpost, BlueBase;
        } REFEREE_PACKED;
        RobotHPPacket() : RefereePacket() {}
        RobotHPPacket(const char *buf) : RefereePacket() { REFEREE_COPYDATA; }
        RobotHPPacket(S data, uint8_t seq = 0) : RefereePacket() { m_data = data; m_sequenceNumber = seq; }

        static uint16_t ClassPacketId() { return uint16_t(PacketType::RobotHP); }
        virtual uint16_t PacketId() override { return ClassPacketId(); }
        virtual uint16_t DataLength() override { return sizeof(S); }

        static RefereePacket* Make(const char* buf) { return new RobotHPPacket(buf); }

        virtual bool Serialize(char* buffer, size_t bufferSize) override;

    protected:
        S m_data;
    };

    // 0x0004 Dart State
    // FIXME: MISSING FROM OFFICIAL MANUAL

    // 0x0005 AI Challenge Buff
    // TODO: UNUSED

    // 0x0101 Playground State
    // TODO: UNUSED

    // 0x0102 Supply Station Action Event
    // TODO: UNUSED

    // 0x0103 Request Bullet Supply
    // FIXME: MISSING FROM OFFICIAL MANUAL

    // 0x0104 Referee Warning Event
    // TODO: UNUSED

    // 0x0105 Dart Gate Countdown
    // TODO: UNUSED

    // 0x0201 Robot State
    class RobotStatePacket : public RefereePacket {
    public:
        struct S {
            RobotID id;
            uint8_t level;
            uint16_t hp, maxHp;
            ShooterLimits shooter17mm_1;
            ShooterLimits shooter17mm_2;
            ShooterLimits shooter42mm;
            uint16_t chassisPowerLimit;
            PowerOutputState powerOutput;
        } REFEREE_PACKED;
        RobotStatePacket() : RefereePacket() {}
        RobotStatePacket(const char *buf) : RefereePacket() { REFEREE_COPYDATA; }
        RobotStatePacket(S data, uint8_t seq = 0) : RefereePacket() { m_data = data; m_sequenceNumber = seq; }

        static uint16_t ClassPacketId() { return uint16_t(PacketType::RobotState); }
        virtual uint16_t PacketId() override { return ClassPacketId(); }
        virtual uint16_t DataLength() override { return sizeof(S); }

        static RefereePacket* Make(const char* buf) { return new RobotStatePacket(buf); }
        
        virtual bool Serialize(char* buffer, size_t bufferSize) override;

    protected:
        S m_data;
    };

    // 0x0202 Real-Time Heat Power State
    class HeatPowerStatePacket : public RefereePacket {
    public:
        struct S {
            // Power supply state
            uint16_t chassisVoltage; // in millivolts (mV)
            uint16_t chassisCurrent; // in milliamps (mA)
            uint32_t chassisPower; // in milliwatts (mW)
            uint16_t chassisPowerBuffer; // in jouls (J)

            // Shooter heat state
            uint16_t heat17mm_1;
            uint16_t heat17mm_2;
            uint16_t heat42mm;
        } REFEREE_PACKED;
        HeatPowerStatePacket() : RefereePacket() {}
        HeatPowerStatePacket(const char *buf) : RefereePacket() { REFEREE_COPYDATA; }
        HeatPowerStatePacket(S data, uint8_t seq = 0) : RefereePacket() { m_data = data; m_sequenceNumber = seq; }


        static uint16_t ClassPacketId() { return uint16_t(PacketType::HeatPowerState); }
        virtual uint16_t PacketId() override { return ClassPacketId(); }
        virtual uint16_t DataLength() override { return sizeof(S); }

        static RefereePacket* Make(const char* buf) { return new HeatPowerStatePacket(buf); }
        
        virtual bool Serialize(char* buffer, size_t bufferSize) override;

    protected:
        S m_data;
    };
}

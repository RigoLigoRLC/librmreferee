
#pragma once
#include <cstdint>

namespace RMReferee {
    enum class PacketType : uint16_t {
        GameStatus = 0x0001,
        GameResultEvent = 0x0002,
        RobotHP = 0x0003,
        DartState = 0x0004,
        AIChallengeBuff = 0x0005,

        PlaygroundState = 0x0101,
        SupplyStationActionEvent = 0x0102,
        RequestBulletSupply = 0x0103,
        RefereeWarningEvent = 0x0104,
        DartGateCountdown = 0x0105,

        RobotState = 0x0201,
        HeatPowerState = 0x0202,
        RobotPosition = 0x0203,
        RobotBuff = 0x0204,
        AerialRobotEnergyState = 0x0205,
        DamageEvent = 0x0206,
        ShootEvent = 0x0207,
        RemainingBulletState = 0x0208,
        RobotRfidState = 0x0209,
        DartControlCommand = 0x020A,

        InterRobotCommsMessage = 0x0301,
        CustomControllerCommsMessage = 0x0302,
        MinimapInteractionCommsMessage = 0x0303, // TODO: What's this?
        KeyboardMouseMessage = 0x0304,
        ClientMinimapRecv = 0x0305, // TODO: What's this?
    };

    enum class GameType : uint8_t {
        Standard = 1,
        RMUTechnicalChallenge,
        ICRA_RMUA,
        RMUL_3V3,
        RMUL_1V1
    };

    enum class GameStage : uint8_t {
        NotStarted = 0,
        Preparing,
        SelfTest,
        Countdown,
        Started,
        EndScreen,
    };

    enum class GameResult : uint8_t {
        Tie = 0,
        RedWin,
        BlueWin,
    };

    enum class RobotID : uint8_t {
        Red1 = 1,
        Red2 = 2,
        Red3 = 3,
        Red4 = 4,
        Red5 = 5,
        RedAerial = 6,
        RedSentry = 7,
        RedDart = 8,
        RedRadar = 9,

        Blue1 = 101,
        Blue2 = 102,
        Blue3 = 103,
        Blue4 = 104,
        Blue5 = 105,
        BlueAerial = 106,
        BlueSentry = 107,
        BlueDart = 108,
        BlueRadar = 109,
    };

    enum class GraphicsType : uint8_t {
        Line = 0,
        Rectangle,
        Circle,
        Ellipse,
        FpNumber,
        IntNumber,
        String,
    };

    enum class GraphicsColor : uint8_t {
        TeamColor = 0,
        Yellow,
        Green,
        Orange,
        Violet,
        Pink,
        Cyan,
        Black,
        White,
    };
};

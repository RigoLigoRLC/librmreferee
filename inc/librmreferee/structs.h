
#pragma once

#include "platform.h"
#include "enums.h"

namespace RMReferee {
    struct ShooterLimits {
        uint16_t cooldownUnitPerSecond;
        uint16_t heatLimit;
        uint16_t speedLimit;
    } REFEREE_PACKED;

    union PowerOutputState {
        uint8_t byte;
        struct {
            bool gimbal : 1;
            bool chassis : 1;
            bool shooter : 1;
        };
    };

    struct GraphicalCommand {
        
    } REFEREE_PACKED;
};

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define PR_MAX_VALUE UINT8_MAX

/// Number of valves, pressure regulators, and pumps defined below
#define N_VALVES 32
#define N_PRS 3
#define N_PUMPS 2

/// The minimum and maximum pressure values (in PSI) supported by the pressure controllers.
#define PR1_MIN_PRESSURE 0
#define PR2_MIN_PRESSURE 0
#define PR3_MIN_PRESSURE -14

#define PR1_MAX_PRESSURE 29.5 // For control layer valves
#define PR2_MAX_PRESSURE 4.8 // Flow layer valves
#define PR3_MAX_PRESSURE 14

enum Command : uint8_t {
    VALVE,
    PRESSURE,
    PUMP,
    STATUS,
    UPTIME,
    ERROR
};

const uint8_t START_BYTE = 250;
const uint8_t STOP_BYTE = 251;
const uint8_t ESCAPE_BYTE = 252;

#endif

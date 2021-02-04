#ifndef VRA_ENVIRONMENT_CONTROL_H
#define VRA_ENVIRONMENT_CONTROL_H

#include "BLEService.h"

// #include "VRASettings.h"
// #include "VRAStorage.h"
#include "BLENotifyCharacteristic.h"

#include "SHT31.h"
#include "LPS35HW.h"

class VRAEnvironmentControl : public BLEService
{
public:
    enum class Characteristics: uint16_t
    {
        Environment = 0xFF00
    };

    VRAEnvironmentControl(UUID *p_uuid, EventQueue *p_eq, StateChain *p_stateChain, I2C *p_i2c);

    void init();
    void initCharacteristics();
    void pastBleInit();

private:
    void onStateOff();
    void onStateStandby();
    void onStateOn();

    void getEnv();

    EventQueue *eq;
    // VRASettings *settings;
    // VRAStorage *storage;

    I2C *i2c;

    LPS35HW *pressSens;
    Sht31 *tempHumidSens;
};

#endif //VRA_ENVIRONMENT_CONTROL_H
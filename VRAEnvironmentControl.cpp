#include "VRAEnvironmentControl.h"    
    
VRAEnvironmentControl::VRAEnvironmentControl(
    UUID *p_uuid, EventQueue *p_eq, StateChain *p_stateChain, I2C *p_i2c):
    BLEService("envCtrl", p_uuid, p_eq, p_stateChain),
    eq(p_eq),
    i2c(p_i2c){

    this->pressSens = new LPS35HW(this->i2c);
    this->tempHumidSens = new Sht31(this->i2c);
}

void VRAEnvironmentControl::init(){
    printf("[envCtrl] init\r\n");
    this->pressSens->init();
    this->tempHumidSens->triggerMeassure();
}

void VRAEnvironmentControl::initCharacteristics(){
    printf("[envCtrl] init Characteristics\r\n");
    this->addCharacteristic(
        new BLENotifyCharacteristic(
            (uint16_t) VRAEnvironmentControl::Characteristics::Environment,
            16,//size
            this->eq,
            1000,//interval
            500,//min
            600000,//max
            callback(this, &VRAEnvironmentControl::getEnv)
        )
    );
}

void VRAEnvironmentControl::pastBleInit(){
    printf("[envCtrl] pastBleInit\r\n");
}

void VRAEnvironmentControl::onStateOff(){
    printf("[envCtrl] off\r\n");
}

void VRAEnvironmentControl::onStateStandby(){
    printf("[envCtrl] standby\r\n");
}

void VRAEnvironmentControl::onStateOn(){
    printf("[envCtrl] on\r\n");
}

void VRAEnvironmentControl::getEnv(){
    printf("[envCtrl] getEnv\r\n");
    this->tempHumidSens->readTempHum();
    float environment[4]{
        this->tempHumidSens->readTemperature(),
        this->tempHumidSens->readHumidity(),
        this->pressSens->readTemperature(),
        this->pressSens->readPressure()
    };
    this->tempHumidSens->triggerMeassure();
    printf("t1: %d t2: %d h:%d p:%d\r\n", (int) environment[0], (int) environment[1], (int) environment[2], (int) environment[3]);
    
    this->setGatt(
        (uint16_t) VRAEnvironmentControl::Characteristics::Environment,
        environment, 4
    );
}
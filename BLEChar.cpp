#include "BLEChar.h"

//Serial pc2(p29, p30, "debug", 115200);

BLEChar::BLEChar(
    const UUID &uuid, 
    uint8_t properties, 
    uint8_t size){

    this->value = (uint8_t*) calloc(size, sizeof(uint8_t));

    // uint8_t properties = GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE
    //  | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ;

    // GattCharacteristic(
    //     const UUID &uuid,
    //     uint8_t *valuePtr = NULL,
    //     uint16_t len = 0,
    //     uint16_t maxLen = 0,
    //     uint8_t props = BLE_GATT_CHAR_PROPERTIES_NONE,
    //     GattAttribute *descriptors[] = NULL,
    //     unsigned numDescriptors = 0,
    //     bool hasVariableLen = true
    // )
    
    this->charac = new GattCharacteristic(
        uuid,
        reinterpret_cast<uint8_t *>(this->value),
        sizeof(uint8_t) * size,
        sizeof(uint8_t) * size,
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE | properties,
        NULL,
        0,
        false
    );
    if(properties & GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ){
        this->readCb = new Callback<void(void)>;
        this->charac->setReadAuthorizationCallback(this, &BLEChar::readAuth);
    }
    if(properties & GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE){
        this->writeCb = new Callback<void(void)>;
        this->charac->setWriteAuthorizationCallback(this, &BLEChar::writeAuth);
    }
}

void BLEChar::readAuth(GattReadAuthCallbackParams *params) {
    //pc2.printf("readAuth %x\n\r", this->charac->getValueAttribute().getUUID().getShortUUID());
    this->readCb->call();
    BLE::Instance(BLE::DEFAULT_INSTANCE).gattServer().write(
        this->charac->getValueHandle(), 
        this->value, 
        sizeof(this->value));
}

void BLEChar::writeAuth(GattWriteAuthCallbackParams *params) {
    memcpy(this->value, params->data, params->len);
    this->writeCb->call();
}

void BLEChar::setIntVal(int32_t val){
    this->value[3] = (uint8_t)(val & 0x000000FF);
    this->value[2] = (uint8_t)((val & 0x0000FF00) >> 8);
    this->value[1] = (uint8_t)((val & 0x00FF0000) >> 16);
    this->value[0] = (uint8_t)((val & 0xFF000000) >> 24);
}
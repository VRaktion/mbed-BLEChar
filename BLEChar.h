#ifndef BLE_CHAR_H
#define BLE_CHAR_H

#include "mbed.h"
#include "ble/BLE.h"
#include "ble/Gap.h"


class BLEChar
{
    public:

        // enum BLECharProps {
        //     NONE = GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE,
        //     READ = GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ,
        //     WRITE = GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE,
        // };

        BLEChar(
            const UUID &uuid, 
            uint8_t properties, 
            uint8_t size
        );

        void readAuth(GattReadAuthCallbackParams *params);
        void writeAuth(GattWriteAuthCallbackParams *params);

        Callback<void(void)> *readCb;
        Callback<void(void)> *writeCb;
    //private:
        uint8_t *value;
        GattCharacteristic *charac;

        void setIntVal(int32_t val);
};

#endif
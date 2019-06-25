#include "BLEChar.h"

// Serial pc2(p29, p30, "debug", 115200);

BLEChar::BLEChar(const UUID &uuid, uint8_t properties, uint16_t size): size(size) {

  this->value = (uint8_t *)calloc(size, sizeof(uint8_t));

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
      uuid, this->value, size, size,
      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE | properties, NULL, 0,
      false);
  if (properties & GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ) {
    this->readCb = new Callback<void(void)>;
    this->readCb->attach(emptyFct);
    this->charac->setReadAuthorizationCallback(this, &BLEChar::readAuth);
  }
  if (properties & GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE) {
    this->writeCb = new Callback<void(void)>;
    this->writeCb->attach(emptyFct);
    this->charac->setWriteAuthorizationCallback(this, &BLEChar::writeAuth);
  }
}

void BLEChar::readAuth(GattReadAuthCallbackParams *params) {
  // pc2.printf("readAuth %x\n\r",
  // this->charac->getValueAttribute().getUUID().getShortUUID());

  this->readCb->call();
  BLE::Instance(BLE::DEFAULT_INSTANCE)
      .gattServer()
      .write(this->charac->getValueHandle(), this->value, this->size);
}

void BLEChar::writeAuth(GattWriteAuthCallbackParams *params) {
  memcpy(this->value, params->data, params->len);
  this->writeCb->call();
}

void BLEChar::setIntVal(int32_t val) {
  memcpy(this->value, (uint8_t *)&val, 4);
  // this->value[3] = (uint8_t)(val & 0x000000FF);
  // this->value[2] = (uint8_t)((val & 0x0000FF00) >> 8);
  // this->value[1] = (uint8_t)((val & 0x00FF0000) >> 16);
  // this->value[0] = (uint8_t)((val & 0xFF000000) >> 24);
}

void BLEChar::setFloatVal(float val) {
  memcpy(this->value, (uint8_t *)&val, 4);
}

void BLEChar::setTrippleShortVal(int16_t val0, int16_t val1, int16_t val2) {
  memcpy(this->value, (uint8_t *)&val0, 2);
  memcpy(this->value+2, (uint8_t *)&val1, 2);
  memcpy(this->value+4, (uint8_t *)&val2, 2);
  // this->value[5] = (uint8_t)(val2 & 0x00FF);
  // this->value[4] = (uint8_t)((val2 & 0xFF00) >> 8);
  // this->value[3] = (uint8_t)(val1 & 0x00FF);
  // this->value[2] = (uint8_t)((val1 & 0xFF00) >> 8);
  // this->value[1] = (uint8_t)(val0 & 0x00FF);
  // this->value[0] = (uint8_t)((val0 & 0xFF00) >> 8);
}

void BLEChar::setQuatShortVal(int16_t val0, int16_t val1, int16_t val2,
                              int16_t val3) {
  memcpy(this->value, (uint8_t *)&val0, 2);
  memcpy(this->value+2, (uint8_t *)&val1, 2);
  memcpy(this->value+4, (uint8_t *)&val2, 2);
  memcpy(this->value+6, (uint8_t *)&val3, 2);
  // this->value[7] = (uint8_t)(val3 & 0x00FF);
  // this->value[6] = (uint8_t)((val3 & 0xFF00) >> 8);
  // this->value[5] = (uint8_t)(val2 & 0x00FF);
  // this->value[4] = (uint8_t)((val2 & 0xFF00) >> 8);
  // this->value[3] = (uint8_t)(val1 & 0x00FF);
  // this->value[2] = (uint8_t)((val1 & 0xFF00) >> 8);
  // this->value[1] = (uint8_t)(val0 & 0x00FF);
  // this->value[0] = (uint8_t)((val0 & 0xFF00) >> 8);
}

void BLEChar::setShortVal(int16_t val) {
  memcpy(this->value, (uint8_t *)&val, 2);
  // this->value[1] = (uint8_t)(val & 0x00FF);
  // this->value[0] = (uint8_t)((val & 0xFF00) >> 8);
}

void BLEChar::emptyFct(void) {}
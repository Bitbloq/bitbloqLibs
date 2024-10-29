#ifndef BITBLOQ_NOISE_SENSOR_H
#define BITBLOQ_NOISE_SENSOR_H
#include "Arduino.h"

class NoiseSensor
{
public:
  NoiseSensor(uint8_t pin);
  int readNoise();
private:
  uint8_t _pin;
};

#endif //BITBLOQ_NOISE_SENSOR_H

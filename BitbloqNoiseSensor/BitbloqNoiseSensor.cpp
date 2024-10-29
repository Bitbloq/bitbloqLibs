#include "Arduino.h"
#include "BitbloqNoiseSensor.h"

NoiseSensor::NoiseSensor(uint8_t pin)
{
  _pin = pin;
  pinMode(_pin, INPUT);
}

float map(
  float input,
  float input_start,
  float input_end,
  float output_start,
  float output_end
) {
  return output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start);
}

int read(uint8_t noisepin) {
  int level = 0;
  int voltage = 0;
  float noise = 0;
  int h = 0;
  int l = 0;
  int sumh = 0;
  int suml = 0;
  for (int i = 0; i < 1000; i++) {
    level = level + analogRead(noisepin);
  }
  level = level / 1000;
  for (int i = 0; i < 1000; i++) {
    voltage = analogRead(noisepin);
    if (voltage >= level) {
      h += 1;
      sumh = sumh + voltage;
    } else {
      l += 1;
      suml = suml + voltage;
    }
  }
  if (h == 0) {
    sumh = level;
  } else {
    sumh = sumh / h;
  }
  if (l == 0) {
    suml = level;
  } else {
    suml = suml / l;
  }
  noise = sumh - suml;
  if (noise < 0) {
    noise = 0;
  }
  if (noise <= 4) {
    noise = map(
      (float)noise,
      0.0,
      4.0,
      30.0,
      50.0
    );
  } else if (noise <= 8) {
    noise = map(
      (float)noise,
      4.0,
      8.0,
      50.0,
      55.0
    );
  } else if (noise <= 14) {
    noise = map(
      (float)noise,
      9.0,
      14.0,
      55.0,
      60.0
    );
  } else if (noise <= 32) {
    noise = map(
      (float)noise,
      15.0,
      32.0,
      60.0,
      70.0
    );
  } else if (noise <= 60) {
    noise = map(
      (float)noise,
      33.0,
      60.0,
      70.0,
      75.0
    );
  } else if (noise <= 100) {
    noise = map(
      (float)noise,
      61.0,
      100.0,
      75.0,
      80.0
    );
  } else if (noise <= 150) {
    noise = map(
      (float)noise,
      101.0,
      150.0,
      80.0,
      85.0
    );
  } else if (noise <= 231) {
    noise = map(
      (float)noise,
      151.0,
      231.0,
      85.0,
      90.0
    );
  } else {
    noise = map(
      (float)noise,
      231.0,
      1023.0,
      90.0,
      120.0
    );
  }
  return (int)noise;
}

int NoiseSensor::readNoise()
{
  return read(_pin);
}

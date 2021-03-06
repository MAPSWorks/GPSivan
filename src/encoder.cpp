#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "encoder.h"
#include "glgui.h"

encoder enc;

encoder::encoder()
{
}

encoder::~encoder()
{
}

void encoder::setup_encoder(int pin_a, int pin_b)
{
  _pin_a = pin_a;
  _pin_b = pin_b;
  _value = 0;
  _lastEncoded = 0;

  wiringPiSetup();

  pinMode(pin_a, INPUT);
  pinMode(pin_b, INPUT);
  pullUpDnControl(pin_a, PUD_UP);
  pullUpDnControl(pin_b, PUD_UP);

  wiringPiISR(pin_a, INT_EDGE_BOTH, update_encoder);
  wiringPiISR(pin_b, INT_EDGE_BOTH, update_encoder);
}

void update_encoder()
{
  int MSB = digitalRead(enc._pin_a);
  int LSB = digitalRead(enc._pin_b);
  
  int encoded = (MSB << 1) | LSB;
  int sum = (enc._lastEncoded << 2) | encoded;

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
    {
      gui.zoom_in();
      enc._value++;
    }
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
    {
      gui.zoom_out();
      enc._value--;
    }

  enc._lastEncoded = encoded;
}


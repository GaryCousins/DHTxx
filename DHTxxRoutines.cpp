// Source code file for DHTxx library

// Author:  Gary Cousins
// Date: 14/08/16
// Platform: Arduniuo UNO

// This code can be used, modified and distributed without limitiation
// but the author takes no repsonsibility of loss or damages incured as
// a result.  The author offers no support or warrenty of any kind.  It
// is the responsibility of the end user to ensure this code is fit
// for purpose.

// ---------------------------------------------------------------------
// Include files
#include <Arduino.h>        // Need all the standard definitions
#include "DHTxxRoutines.h"  // local header files

// ---------------------------------------------------------------------
// Global vaiables withing the DHT Library
//
bool DHTInitalised = false;
// Internal flag it indicate if the DHT library has been initialised

char IOPin = 0;
// Keep track of which pin is being used

// ---------------------------------------------------------------------
// Internal routines
//
void startReading();
int calcChecksum(char* byteStore);

// ---------------------------------------------------------------------

// Initialise
int initDHTxxSensor(int pin)
{
  // Check if the library is already setup
  if (DHTInitalised)
    return HDT_ALREADY_INIT;

  // Set register bank to operate with based on pin selected
  // Set interrupt on the correct bank for the correct pin
  if ((pin >= 2) && (pin <= 12))
  {
    IOPin = pin;
    pinMode(IOPin, OUTPUT);
  }
  else
    return DHT_INVALID_PIN;

  // Mark the library as initialised
  DHTInitalised = true;

  return DHT_OK;
}

// ---------------------------------------------------------------------

// Close
int closeDHTxxSensor()
{
  // Check if the library is already setup
  if (!DHTInitalised)
    return DHT_NOT_INIT;

  DHTInitalised = false;        // Mark the library as deinitialised
  IOPin = 0;                    // reset the IOPin
  return DHT_OK;
}

// ---------------------------------------------------------------------

// Read
int readDHTxxSensor(float* temperature, float* humidity)
{
  // Variables to measure timeouts
  unsigned long timer_now, timer_start;

  // Bit store and refernce pointer
  // Make them all char to use least memory space possible
  char byteStore[5] = {0, 0, 0, 0, 0};
  char bitCount = 0;
  char byteCount = 0;

  // Start a reading
  startReading();

  // Release the pin and wait for the line to go low (up to 40uS response time)
  timer_now = 0;
  timer_start = micros();
  while ((digitalRead(IOPin) == HIGH)  && (timer_now < DHT_TIMEOUT_US))
  {
    timer_now = micros() - timer_start;
  }
  // Check to see it we timed out
  if (timer_now >= DHT_TIMEOUT_US)
    return DHT_TIMEOUT;

  // It should now stay low for 80uS
  timer_now = 0;
  timer_start = micros();
  while ((digitalRead(IOPin) == LOW) && (timer_now < DHT_TIMEOUT_US))
  {
    timer_now = micros() - timer_start;
  }
  // Check to see it we timed out
  if (timer_now >= DHT_TIMEOUT_US)
    return DHT_TIMEOUT;

  // Then high for 80uS
  timer_now = 0;
  timer_start = micros();
  while ((digitalRead(IOPin) == HIGH) && (timer_now < DHT_TIMEOUT_US))
  {
    timer_now = micros() - timer_start;
  }
  // Check to see it we timed out
  if (timer_now >= DHT_TIMEOUT_US)
    return DHT_TIMEOUT;

  // Now we should start getting data bits
  // Each is 50uS low then either 26-28uS high or 70uS high for 0 or 1
  // 40 Bits in total as 5 bytes.

  // For 5 bytes of 8 bits (40 bits total)
  for (byteCount = 0; byteCount < 5; byteCount++)
  {
    for (bitCount = 0; bitCount < 8; bitCount++)
    {
      // Wait for the 50uS low to complete
      timer_now = 0;
      timer_start = micros();
      while ((digitalRead(IOPin) == LOW) && (timer_now < DHT_TIMEOUT_US))
      {
        timer_now = micros() - timer_start;
      }
      // Check to see it we timed out
      if (timer_now >= DHT_TIMEOUT_US)
        return DHT_TIMEOUT;

      // Count while the line is high
      timer_now = 0;
      timer_start = micros();
      while ((digitalRead(IOPin) == HIGH) && (timer_now < DHT_TIMEOUT_US))
      {
        timer_now = micros() - timer_start;
      }

      // Check to see what the result is
      if (timer_now < DHT_LEVEL_US)  // '0' Threshold
      { // Do nothing
        ;
      }
      else if (timer_now < DHT_TIMEOUT_US) // 'Timeout' Threshold
      { // Mask in a '1' at the right bit position
        byteStore[byteCount] = byteStore[byteCount] + (HIGH << (7 - bitCount));
      }
      else
      { // Timeout has occured
        return DHT_TIMEOUT;
      }
    }
  }

  if (calcChecksum(byteStore) == DHT_OK)
  {
    // Convert 2 byte humidity to floating point
    *humidity = (float)byteStore[0];
    *humidity = *humidity + ((float)byteStore[1]) / 100.0;

    // Convert 2 byte temperature to floating point
    *temperature = (float)byteStore[2];
    *temperature = *temperature + ((float)byteStore[3]) / 100.0;

    // Done
    return DHT_OK;
  }
  else
  {
    return DHT_BAD_CS;
  }
}

// ---------------------------------------------------------------------
// Internal routine to set initialise a sensor reading
void startReading()
{
  // Set the pin to output mode
  pinMode(IOPin, OUTPUT);

  // Pull low for DHT_INIT_MS
  digitalWrite(IOPin, LOW);
  delay(DHT_INIT_MS);
  digitalWrite(IOPin, HIGH);

  // Set the pin to input mode
  pinMode(IOPin, INPUT);

  return;
}

// ---------------------------------------------------------------------
// Confirm checksum
int calcChecksum(char* byteStore)
{
  // Local checksum valiable
  char checksum;

  // Calculte checksum
  checksum = byteStore[0] + byteStore[1] + byteStore[2] + byteStore[3];

  // Report good or bad
  if (byteStore[4] == checksum)
    return DHT_OK;
  else
    return DHT_BAD_CS;
}

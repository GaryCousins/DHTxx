// Test file for DHTxx Routines

// Author:  Gary Cousins
// Date: 09/08/16
// Platform: Arduniuo UNO

// This code can be used, modified and distributed without limitation
// but the author takes no responsibility of loss or damages incurred as
// a result.  The author offers no support or warranty of any kind.  It
// is the responsibility of the end user to ensure this code is fit
// for purpose.

// ---------------------------------------------------------------------
// Include files
#include "DHTxxRoutines.h"

// ---------------------------------------------------------------------

void setup()
{
  // Don't use the setup function
}

// ---------------------------------------------------------------------

void loop()
{
  // Local variables
  float temperature = 0, humidity = 0;
  unsigned long time_start = 0, time_end = 0;

  // Setup
  Serial.begin(57600);

  // Setup Sensor
  time_start = micros();
  Serial.print("initDHTxxSensor returned: ");
  Serial.println(initDHTxxSensor(10));
  time_end = micros();

  // Report on timing
  Serial.print("Time taken: ");
  Serial.print(time_end - time_start);
  Serial.println(" uS");
  Serial.println();

  // Main loop
  while (true)
  {
    time_start = micros();

    // Read temperature
    Serial.print("readDHTxxSensor returned: ");
    Serial.println(readDHTxxSensor(&temperature, &humidity));
    Serial.print("Temperature: ");
    Serial.println(temperature);

    // Read Humidity
    Serial.print("Humidity: ");
    Serial.println(humidity);
    time_end = micros();

    // Report on timing
    Serial.print("Time taken: ");
    Serial.print(time_end - time_start);
    Serial.println(" uS");
    Serial.println();
    delay(5000);
  }
}


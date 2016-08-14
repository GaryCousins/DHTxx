// Header file for DHTxx library

// Author:  Gary Cousins
// Date: 07/08/16
// Platform: Arduniuo UNO

// This code can be used, modified and distributed without limitiation
// but the author takes no repsonsibility of loss or damages incured as
// a result.  The author offers no support or warrenty of any kind.  It
// is the responsibility of the end user to ensure this code is fit
// for purpose.

// ---------------------------------------------------------------------
// Defines values for error codes
// ---------------------------------------------------------------------
#define DHT_OK           0
#define DHT_NOT_INIT     1
#define DHT_ALREADY_INIT 2
#define DHT_INVALID_PIN  3
#define DHT_TIMEOUT      4
#define DHT_BAD_CS       5
#define DHT_DEBUG        6

// ---------------------------------------------------------------------
// Defines values for sensor timing periods
// ---------------------------------------------------------------------
#define DHT_INIT_MS      18 // Initialisation in mS
#define DHT_LEVEL_US     40 // Logic HIGH/LOW timing threshold in uS
#define DHT_TIMEOUT_US   100// Timeout in uS

// ---------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------

// Initialise
// This routine intialised the temperature sensor
// Parameters:
// pin - int - Input - Arduino digial pin number the sensor data line is connected to (2 to 12)
// pullup - bool - Input - True to set pullup of selected pin
// Outputs:
// None
// Return value:
// DHT_OK, HDT_ALREADY_INIT, DHT_INVALID_PIN
int initDHTxxSensor(int pin);

// ---------------------------------------------------------------------

// Close
// This routine shutsdown the temperature sensor.
// but does not change the pin type or pullup setting
// Parameters:
// None
// Outputs:
// None
// Return value:
// DHT_OK, DHT_NOT_INIT
int closeDHTxxSensor();

// ---------------------------------------------------------------------

// Read
// This routine takes a temperature and humidity reading.
// Parameters:
// temperature - float* - Input - Pointer to a float to receive the temperature reading (DegC)
// temperature - float* - Input - Pointer to a float to receive the humidity reading (%)
// Outputs:
// None
// Return value:
// DHT_OK, DHT_NOT_INIT
int readDHTxxSensor(float* temperature, float* humidity);


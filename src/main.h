
/*
 * Set the next variables with your SSID/Password combination and the 
 * ThinkSpeak *WRITE* API KEY
 * 
 * PLATFORMIO_BUILD_FLAGS=-DWIFI_PASS=\"TopfSigrid\" \
 *                        -DWIFI_SSID=\"Internet\"   \
 *                        -DTHINGSPEAK_HEARTBEAT=\"72ND53937K5N52JP\"
 */

#ifdef WIFI_SSID
const char* wifi_ssid = WIFI_SSID;
#else
const char* wifi_ssid = "YOUR-WIFI-SSID";
#endif

#ifdef WIFI_PASS
const char* wifi_pass = WIFI_PASS;
#else
const char* wifi_pass = "YOUR-WIFI-PASSWORD";
#endif

#ifdef THINGSPEAK_HEARTBEAT
const char* thingspeakApiKey = THINGSPEAK_HEARTBEAT;
#else
const char* thingspeakApiKey = "YOUR-THINGSPEAK-APIKEY";
#endif

volatile int     heartbeat = false;
volatile int     alarm     = false;
volatile uint8_t seconds   = 0;

String status;
String date;

// ISR
void ICACHE_RAM_ATTR inline ISR_timer0();

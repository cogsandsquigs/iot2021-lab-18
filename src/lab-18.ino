#include "oled-wing-adafruit.h"
#include "MQTT.h"
#include "blynk.h"
#include "string"
using namespace std;

SYSTEM_THREAD(ENABLED);
OledWingAdafruit display;

double latitude;
double longitude;

MQTT client("lab.thewcl.com", 1883, callback);
WidgetMap myMap(V1);

void setup()
{
  //Serial.begin(9600);

  display.setup();
  display.clearDisplay();
  display.display();

  Blynk.begin("MecoKYlnBJ34JievhYFfRL01nY9gzn4O", IPAddress(167, 172, 234, 162), 9090); // in setup
}

void loop()
{
  display.loop();
  Blynk.run(); // first statement in loop

  if (client.isConnected())
  {
    client.loop();

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);

    display.println(String(latitude) + " " + String(longitude));
    myMap.location(1, latitude, longitude, "value");
    display.display();
  }
  else
  {
    client.connect(System.deviceID());

    if (client.isConnected())
    {
      client.subscribe("iss-pos-lat");
      client.subscribe("iss-pos-lon");
      client.publish("nodered", "get");
    }
  }
}

// recieve message
void callback(char *topic, byte *payload, unsigned int length)
{
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;

  double locale = stod(p);

  if (String(topic) == "iss-pos-lat")
  {
    latitude = locale;
  }
  else if (String(topic) == "iss-pos-lon")
  {
    longitude = locale;
  }
}
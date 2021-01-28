#include <painlessMesh.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

#define   MESH_PREFIX     "anankenode"
#define   MESH_PASSWORD   "esol_123"
#define   MESH_PORT       5555

#define   STATION_SSID     "BELL4G-6F00"
#define   STATION_PASSWORD "FEBY0T5BGY6"

#define HOSTNAME "MQTT_Bridge"

void receivedCallback( const uint32_t &from, const String &msg );
void mqttCallback(char* topic, byte* payload, unsigned int length);

IPAddress mqttBroker(138, 197, 92, 157);
IPAddress myIP(0, 0, 0, 0);
painlessMesh  mesh;
WiFiClient wifiClient;
PubSubClient mqttClient(mqttBroker, 1883, mqttCallback, wifiClient);

void setup() {
  Serial.begin(9600);

  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, 6 );
  mesh.onReceive(&receivedCallback);

  mesh.stationManual(STATION_SSID, STATION_PASSWORD);
  mesh.setHostname(HOSTNAME);

  mesh.setRoot(true);
  mesh.setContainsRoot(true);
}

void loop() {
  mesh.update();
  mqttClient.loop();
  Serial.println(mesh.getStationIP());
  mqttClient.connect("painlessMeshClient");
}



IPAddress getlocalIP() {
  return IPAddress(mesh.getStationIP());
}
void receivedCallback( const uint32_t &from, const String &msg ) {
  Serial.println(msg.c_str());
  mqttClient.publish("teafactory/test", msg.c_str());
}

void mqttCallback(char* topic, uint8_t* payload, unsigned int length) {

}

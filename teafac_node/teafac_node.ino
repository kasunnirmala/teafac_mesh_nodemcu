#include <Wire.h>
#include "painlessMesh.h"
#include "SHTSensor.h"


SHTSensor sht;


#define   MESH_PREFIX     "anankenode"
#define   MESH_PASSWORD   "esol_123"
#define   MESH_PORT       5555

Scheduler userScheduler;
painlessMesh  mesh;

void sendMessage() ;

Task taskSendMessage( TASK_SECOND * 2 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  sht.readSample();
  delay(500);
  float temperature = sht.getTemperature();
  float humidity = sht.getHumidity();
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  mac.trim();
  String sendStr = "{" + mac + ":[";
  sendStr += "Humidity: " + String(humidity) + " %\t" + String(humidity) + " %\t , Temperature : " + String(temperature) + " *C\t" + String(temperature) + " *C]}";
  Serial.println(sendStr);

  mesh.sendBroadcast( sendStr );
  taskSendMessage.setInterval(  TASK_SECOND * 2 );
}


void setup() {
  Wire.begin();
  Serial.begin(9600);

  if (sht.init()) {
    Serial.print("init(): success\n");
  } else {
    Serial.print("init(): failed\n");
  }
  sht.setAccuracy(SHTSensor::SHT_ACCURACY_MEDIUM); // only supported by SHT3x

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();



}

void loop() {
  mesh.update();
}

/*
 Name:		HomeSprinkler.ino
 Created:	3/25/2018 7:56:44 PM
 Author:	Spencer
*/

//https://github.com/tzapu/WiFiManager/blob/master/examples/AutoConnectWithFSParameters/AutoConnectWithFSParameters.ino
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

#include <PubSubClient.h>         //https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino

WiFiClient espClient;
PubSubClient client(espClient);

char mqtt_server[40];
int relayPin = 2;
char* sprinklerTopic = "home/sprinkler";
char* sprinklerTopicEvent = "home/sprinkler/event";

/*handle subscribed topics*/
void callback(char* topic, byte* payload, unsigned int length) {
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");

	String payloadValue = "";
	char* pv = "";
	strcpy(pv, (char*)payload);//rather use this over string object
	for (int i = 0; i < length; i++) {
		char receivedChar = (char)payload[i];
		payloadValue = payloadValue + receivedChar;
	}

	Serial.println(payloadValue);
	//TODO use json responses.
	//TODO static if thens

	if (strcmp(topic, sprinklerTopic) == 0) {
		if (payloadValue == "STATUS") {
			int resultInt = digitalRead(relayPin);
			delay(500);
			char* relayEvent = "RELAY:";
			relayEvent = relayEvent + resultInt;
			client.publish(sprinklerTopicEvent, relayEvent);
			Serial.println("Sent topic out");
		}
		else if (payloadValue == "ON") {
			digitalWrite(relayPin, 1);
			delay(500);
			client.publish(sprinklerTopicEvent, "ON");
			Serial.println("Turning on relay");
		}
		else if (payloadValue == "OFF") {
			digitalWrite(relayPin, 0);
			delay(500);
			client.publish(sprinklerTopicEvent, "OFF");
			Serial.println("Turning off relay");
		}
	}
}

void reconnect() {
	// Loop until we're reconnected
	while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");
		// Attempt to connect
		if (client.connect("IoT Client")) {
			Serial.println("connected");

			client.subscribe(sprinklerTopic); //waiting for subscibed topic
		}
		else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

void setup() {
	Serial.begin(115200);

	pinMode(relayPin, OUTPUT);
	digitalWrite(relayPin, 0);
	delay(2000);

	WiFiManager wifiManager;
	WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
	wifiManager.addParameter(&custom_mqtt_server);
	wifiManager.autoConnect("SprinklerAp");

	//Get the mqtt from the user input
	strcpy(mqtt_server, custom_mqtt_server.getValue());

	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);
	
	Serial.println("Connected");
}

void loop() {
	if (!client.connected()) {
		reconnect();
	}
	client.loop();
}
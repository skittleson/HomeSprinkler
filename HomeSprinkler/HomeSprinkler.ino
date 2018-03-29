/*
 Name:		HomeSprinkler.ino
 Created:	3/25/2018 7:56:44 PM
 Author:	Spencer Kittleson
*/
//TODO removed hard coded mqtt server. use #include <EEPROM.h> if possible
//TODO failover if relay is on too long
//TODO multiple sprinkler values

#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

#include <PubSubClient.h>         //https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino

#include <ArduinoJson.h>

WiFiClient espClient;
PubSubClient client(espClient);

char* mqttServer = "192.168.0.109";
const int mqttPort = 1883;
const int relayPin = 2;
const char* relayStatus = "status";
const char* relayOn = "true";
const char* relayOff = "false";
const char* sprinklerTopic = "home/sprinkler";
const char* sprinklerTopicEvent = "home/sprinkler/event";

/**
* Convert byte array into char array.
*/
char* byteArrayIntoCharArray(byte* bytes, unsigned int length) {
	char* data = (char*)bytes;
	//end string at length. (it's kinda hacky and surprising this even works. array length is correct as well. :) )
	data[length] = NULL;
	return data;
}

/**
* Callback handler for MQTT
*/
void callback(char* topic, byte* payload, unsigned int length) {
	//Only topic defined should be processed
	if (strcmp(topic, sprinklerTopic) != 0) { return; }

	char* payloadValue = byteArrayIntoCharArray(payload, length);

	//Allocate for json response
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["event"] = "relay";
	root["topic"] = topic;
	root["msg"] = "";
	root["uptime"] = millis();

	if (strcmp(payloadValue, relayStatus) == 0) {
		root[relayStatus] = !((bool)digitalRead(relayPin));
	}
	else if (strcmp(payloadValue, relayOn) == 0) {
		digitalWrite(relayPin, 0);
		root[relayStatus] = true;
	}
	else if (strcmp(payloadValue, relayOff) == 0) {
		digitalWrite(relayPin, 1);
		root[relayStatus] = false;
	}
	else {
		root["msg"] = "unknown payload";
	}

	String payloadJson;
	root.printTo(payloadJson);

	Serial.println(payloadJson);
	client.publish(sprinklerTopicEvent, payloadJson.c_str());
}

void reconnect() {
	// Loop until we're reconnected
	while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");
		// Attempt to connect
		if (client.connect("IoT Client")) {
			Serial.println("connected");
			//subscribe
			client.subscribe(sprinklerTopic);
		}
		else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5 * 1000);
		}
	}
}

void setup() {
	Serial.begin(115200);

	//Default the primary relay off
	pinMode(relayPin, OUTPUT);
	digitalWrite(relayPin, 1);
	delay(500);

	WiFiManager wifiManager;
	//wifiManager.resetSettings();
	//WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
	//wifiManager.addParameter(&custom_mqtt_server);
	wifiManager.autoConnect("SprinklerAp");

	//Get the mqtt from the user input
	//strcpy(mqtt_server, custom_mqtt_server.getValue());

	client.setServer(mqttServer, mqttPort);
	client.setCallback(callback);

	Serial.println("Connected");
}

void loop() {
	if (!client.connected()) {
		reconnect();
	}
	client.loop();
}
# HomeSprinkler
HomeSprinkler is a message listener for water sprinker based operations using ([mqtt](https://mqtt.org/)). This is not a replacement for water sprinkler scheduling.

## Quick Start

Upload the **HomeSprinkler.ino** to device.  Then navigate to http://10.0.1.1 .  Select a WiFi device, enter password, and add a MQTT server.

## Messages
### home/sprinkler
payload on `home/sprinkler` : `status`
response on `home/sprinkler/event` : `{"event":"relay","topic":"home/sprinkler", "uptime":1000, "status":false}`

payload on `home/sprinkler` : `true`
response on `home/sprinkler/event` : `{"event":"relay","topic":"home/sprinkler", "uptime":1001, "status":true}`

payload on `home/sprinkler` : `false`
response on `home/sprinkler/event` : `{"event":"relay","topic":"home/sprinkler", "uptime":1002, "status":false}`

## Hardware
Use a ESP8266 or ESP8285 based device then add a single relay on GPIO2.

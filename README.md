# HomeSprinkler
HomeSprinkler is a message listener for water sprinker based operations using ([mqtt](https://mqtt.org/)). This is not a replacement for water sprinkler scheduling.

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

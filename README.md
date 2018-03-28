# HomeSprinkler
HomeSprinkler tries to be a lean code to manage & trigger water sprinker based operations using a message queue design ([mqtt](https://mqtt.org/)). This is not a replacement for water sprinkler scheduling.

## Messages
### home/sprinkler
payload: `status`
response on `home/sprinkler/event` : `{"event":"relay","topic":"home/sprinkler","status":false}`

payload: `on`
response on `home/sprinkler/event` : `{"event":"relay","topic":"home/sprinkler","status":true}`

payload: `off`
response on `home/sprinkler/event` : `{"event":"relay","topic":"home/sprinkler","status":false}`

## Hardware
Use a ESP8266 or ESP8285 based device then add a single relay on GPIO2.

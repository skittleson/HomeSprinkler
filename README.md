# HomeSprinkler
HomeSprinkler tries to be a lean code to manage & trigger water sprinker based operations using a message queue design ([mqtt](https://mqtt.org/)). This is not a replacement for water sprinkler scheduling.

## Json Responses
`{"event":"relay-status","topic":"home/sprinkler","value":false}
{"event":"relay","topic":"home/sprinkler","value":true}
{"event":"relay","topic":"home/sprinkler","value":false}
`

## Hardware
Use a ESP8266 or ESP8285 based device then add a single relay on GPIO2.

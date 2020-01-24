# D1 ThingSpeak Heartbeat

## Abstract

A most minimal setup to detect missing heartbeats provoced
by an power- or network-outage.

## Prerequisites

- Wemos D1 Mini at hand
- ThingSpeak / MATHLAB account exists
- ITFFF account exists

## IFTTT Maker WebHooks

At ifttt.com/maker_webhooks click `Connect` and then `Settings`. Note down the string after `https://maker.ifttt.com/use/` of the field `URL`. E.g. `bhtMZFLFqdh46H9PzdQbX2mSuayRy_u7_ckcI0cWvxw`.

Go to ifttt.com/create and click on `This`. Search for `webhooks` and click the `WebHooks` applet, then click `Receive a web request` and enter `outage` and click `Create trigger`.

Click on `That`, search for `email` and click the `Email` applet, then `Send me an email` and click `Create action` and then `Finish`.

## ThingSpeak channels

At thingspeak.com/channels create a Channel and note the `Channel ID`,  `Write API Key` and `Read API Key`.

## MATHLAB Analyses

At thingspeak.com/apps/matlab_analyses create a `MATLAB Analysis` with name `IFTTT Trigger if no Heartbeat` and content:

```MATLAB
% ThingSpeak channel and read api key
thing_channel = 915121;
thing_readkey = "NJL2C6UMJVZHOX3U";

% IFTTT event and key
ifttt_event   = "outage";
ifttt_key     = "bhtMLFqdh4H9PzdQbX2mSuayRy_u7_ckcI0cWvxw";

try
    % Read the recent data.
    [idk,timestamp] = thingSpeakRead(thing_channel,'ReadKey',thing_readkey,'NumPoints',1,'Fields',1);
    
    now = datetime('now','Format','dd-MMM-yyyy HH:mm:ss');
    
    if minutes(duration(now-timestamp)) > 2
        fprintf("Trigger: %s\n", webread("https://maker.ifttt.com/trigger/"+ifttt_event+"/with/key/"+ifttt_key));
    end
catch someException
    fprintf("Failed to send alert: %s\n", someException.message);
end
```

## MATHLAB TimeControl

At thingspeak.com/apps/timecontrols create a `New TimeControl` with the name `Heartbeat Evaluation` and the following settings:

| Field | Value |
| ------ | ------- |
| Frequency | Recurring |
| Recurrence | Minute |
| Every | 5 |
| Action | MATHLAB Analyses |
| Code to execute | IFTTT Trigger if no Heartbeat |

## Wemos D1 Mini

Upload the code with your settings (see `main.h`) to the Wemos D1 Mini. 

| Setting | Description | Environment Var |
| - | - | - |
| wifi_ssid | The Wifi SSID the D1 Mini connects to. | WIFI_SSID |
| wifi_pass | The password used to connect to Wifi. | WIFI_PASS |
| thingspeakApiKey | Write API Key for the heartbeat channel. | THINGSPEAK_HEARTBEAT |

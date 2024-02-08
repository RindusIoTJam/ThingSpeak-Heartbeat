# D1 ThingSpeak Heartbeat

## Abstract

A most minimal setup to detect missing heartbeats provoked
by an power- or network-outage.

## Prerequisites

- Wemos D1 Mini at hand
- ThingSpeak / MATHLAB account exists
- Slack Incomming Webhook Integration exists

## ThingSpeak channels

At thingspeak.com/channels create a Channel and note the `Channel ID`,  `Write API Key` and `Read API Key`.

## MATHLAB Analyses

At thingspeak.com/apps/matlab_analyses create a `MATLAB Analysis` with name `Write 0 to Heartbeat if last Heartbeat too old` and content:

```MATLAB
try
    [bla,heartbeat] = thingSpeakRead(967121,'ReadKey',"ZHOXUL2C6VMJNJ3U",'NumPoints',1,'Fields',1);

    now = datetime('now','Format','dd-MMM-yyyy HH:mm:ss');

    %fprintf("heartbeat %s\n", heartbeat);
    %fprintf("now       %s\n", now);
    %fprintf("age   str %s\n", datestr(now-heartbeat,'HH:MM:SS'));
    %fprintf("age   min %d\n", minutes(duration(now-heartbeat)) );

    % write a 0 so React has something to react on
    if minutes(duration(now-heartbeat)) > 3
        thingSpeakWrite(967121,0,'WriteKey',"KO0T51GCEWV76VN3");
    end
    
catch someException
    fprintf("Failed to send alert: %s\n", someException.message);
end
```

## MATHLAB TimeControl

At thingspeak.com/apps/timecontrols create a `New TimeControl` with the name `Run Heartbeat Evaluation` and the following settings:

| Field | Value |
| ------ | ------- |
| Frequency | Recurring |
| Recurrence | Minute |
| Every | 5 |
| Action | MATHLAB Analyses |
| Code to execute | Write 0 to Heartbeat Casa if last Heartbeat too old |

## MATHLAB ThingHTTP

At https://thingspeak.com/apps/thinghttp create a `New ThingHTTP` with the name `Slack Event Outage` and the following settings:

| Field | Value |
| ------ | ------- |
| URL | https://hooks.slack.com/services/TVWG8R3TY/B8X88x5/HCmVtHqsehKy2VKKYQIxQlJnw7 |
| Method | POST |
| Content Type | application/json |
| Host | hooks.slack.com |
| Body | {'text': 'CASA OUTAGE', 'username': 'ThingSpeak'} |

At https://thingspeak.com/apps/thinghttp create a `New ThingHTTP` with the name `Slack Event Recover` and the following settings:

| Field | Value |
| ------ | ------- |
| URL | https://hooks.slack.com/services/TVWG8R3TY/B8X88x5/HCmVtHqsehKy2VKKYQIxQlJnw7 |
| Method | POST |
| Content Type | application/json |
| Host | hooks.slack.com |
| Body | {'text': 'RECOVER', 'username': 'ThingSpeak'} |

## MATHLAB React

At https://thingspeak.com/apps/reacts create a `New React` with the name `Outage Detect` and the following settings:

| Field | Value |
| ------ | ------- |
| Condition Type | Numeric |
| Test Frequency |On data insertion |
| Channel | {your channel} |
| Condition | Field 1 (heartbeat) is equal to 0 |
| Action | ThingHTTP :: Slack Event Outage |
| Run | Only the first time the condition is met |

At https://thingspeak.com/apps/reacts create a `New React` with the name `Recover Detect` and the following settings:

| Field | Value |
| ------ | ------- |
| Condition Type | Numeric |
| Test Frequency |On data insertion |
| Channel | {your channel} |
| Condition | Field 1 (heartbeat) is equal to 1 |
| Action | ThingHTTP :: Slack Event Recover |
| Run | Only the first time the condition is met |


## Wemos D1 Mini

Upload the code with your settings (see `main.h`) to the Wemos D1 Mini. 

| Setting | Description | Environment Var |
| - | - | - |
| wifi_ssid | The Wifi SSID the D1 Mini connects to. | WIFI_SSID |
| wifi_pass | The password used to connect to Wifi. | WIFI_PASS |
| thingspeakApiKey | Write API Key for the heartbeat channel. | THINGSPEAK_HEARTBEAT |

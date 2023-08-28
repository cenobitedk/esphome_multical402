# Kamstrup Multical 402 for esphome

## Introduction

This is a custom component for esphome that can read values from a Kamstrup Multical 402 meter. This meter type is used for district heating for the most.

The code is combined and adapted from other sources, see [Sources & Inspiration](#sources--inspiration) below.

The meter is using KMP (Kamstrup Meter Protocol) for communication which is also used in power meters, but the code in this repository is written to read specific register values.

## Optical Reader

The Multical 402 meter is equipped with an optical communication port. In order to communicate with the meter an Optical Reader is needed.

These are typically equipped with an USB port and are rather expensive, but for this project we only need a simple circuit that can be controlled by the ESP chip.

Options:

- Prebuilt: https://www.ebay.com/usr/arelektronik – this is what I got
- DIY: [https://wiki.hal9k.dk/projects/kamstrup](https://wiki.hal9k.dk/projects/kamstrup) - in Danish

However, I don't think it's feasible making your own. The one from ebay is really cheap (18 EUR), but if you have a 3D printer already you can get the pcb and magnet for only 11 EUR.

## Configuration

Update the `multical402.yaml` file as follows.

### Board

In my case I'm using a Weemos D1 mini clone, but make sure to specify the right board here. Get more info [here](https://esphome.io/components/esp8266.html).

```yaml
esphome:
  board: d1_mini    <-- Set the right board here
  ...
```

### Wifi

Specify wifi credentials.

```yaml
wifi:
  ssid: INSERT_WIFI_SSID
  password: INSERT_WIFI_PASSWORD
```

### UART

The optical reader has 2 connections (besides VCC and GND). Specify them here:

```yaml
uart:
  - id: uart_bus
    rx_pin: D1      <-- Set RX pin
    tx_pin: D2      <-- Set TX pin
    ...
```

### Update interval

The code will run every 10 seconds. This can be changed here:

```yaml
custom_component:
  - lambda: |-
      auto multical402 = new Multical402(
        10000,      <-- Specify update interval in milliseconds
        ...
```

### Sensors

Currently the code is hardcoded to read the following register values:

- Energy (Wh)
- Power (W)
- Temperature In (°C)
- Temperature Out (°C)
- Temperature Diff (°C)
- Flow (l/h)
- Volume (m3)

The code (`kmp.h`) can be changed to read other registers, but that is outside the scope of this readme.

### Unit of Measurement

Make sure to specify the unit of measurement on the sensors to match the value from the meter (eg. **Wh** vs **kWh** vs **MWh**).

```yaml
sensors:
- name: 'Energy'
    ...
    unit_of_measurement: MWh  <-- specify unit of measurement here
    ...
```

## Troubleshooting

In case no values are being read and the log shows something like this:

```sh
[21:04:37][D][Multical402:048]: Start update
[21:04:37][W][Multical402:163]: Timed out listening for data
[21:04:37][W][Multical402:163]: Timed out listening for data
[21:04:38][W][Multical402:163]: Timed out listening for data
[21:04:38][W][Multical402:163]: Timed out listening for data
[21:04:38][W][Multical402:163]: Timed out listening for data
[21:04:39][W][Multical402:163]: Timed out listening for data
[21:04:39][W][Multical402:163]: Timed out listening for data
[21:04:39][D][Multical402:078]: End update
```

Check the following:

- Check the IR LED on the optical reader; use a mobile phone and use the camera, the IR LED should blink every 10 seconds (or other if the update interval has been changed). If this isn't the case, check the UART pins and the wiring.
- Press the "play" button on the meter to enable the IR Port. The meter will turn off the optical port after 30min of inactivity on either buttons and optical port.
- Make sure the optical reader is positioned the right way; turn it 180° to flip the position of the IR sensor.

## Sources & Inspiration

- https://github.com/Hal9k-dk/kamstrup/tree/master/Software%20eksempler
- https://github.com/ProudElm/ESPHome_multical_sensor
- https://github.com/jjansen85/esphome_multical401
- https://github.com/sebbebebbe/Multical402MQTT
- https://github.com/nabovarme/MeterLogger/wiki/Kamstrup-Protocol

## Protocol Description

- https://github.com/nabovarme/MeterLoggerTesto/blob/master/5512447_M1_GB.pdf

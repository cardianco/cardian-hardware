### Features

+ Low cost, high performance , high reliability
+ FSK modulation, 2-way half –duplex communication, strong anti-interfere
+ 434/470/869/915MHz ISM band，globally license free.
+ Maximal output power100mW(20dBm)，output power adjustable between 1-20dBm
+ Sensitivity -117dBm
+ **Supply current for Tx 100mA@20dBm, 40mA@14dBm**
+ **Supply current for Rx 25mA**
+ Low current sleep mode 1uA
+ Standard TTL UART interface, extendable to RS232 or other interface
+ Operation frequency can be configured, acceptable for several modules working in different frequency    with no disturbance on each other.
+ Optional for frequency deviation and receiving ISM bandwidth, users can be modulated by software.
+ *Easily use, auto exchange on communication & transceiver*
+ Communication speed **1.2kbps** -115.2kbps，can be modulated through software
+ Afford ENABLE control signal，user self control work duty cycle
+ RSSI function
+ With LED indication
+ **Longer transmission distance，over 1Km in open air**
+ **Small size 16X20X2MM，9 PIN DIP & 9 PIN SMD package，easy for assembly.**
+ Tuning free
+ Accord FCC,ETSI standard

### Config

*HM-TRPW* | *MCU*
`VCC`    &rarr; `VCC`
`RX`     &rarr; `DXT`
`GND`    &rarr; `GND`
`DXT`    &rarr; `RXT`
`Enable` &rarr; 1k Ω &rarr; `GND`

To start the configuration, connect the `config` pin of the module to the `GND`.
The configuration values are in hex and start with `"\xAA\xFA"`.

+ `F0`: Reset to default parameter.
  `"\xAA\xFA\xF0"`

+ `E1`: Reading the current Config parameter.
  `"\xAA\xFA\xE1"`

+ `D2`:  Set up working frequency, <4 Byte> as Hz.
  `"\xAA\xFA\xD2"` + `"\u18AD\u2380"`
  *414000000* to *454000000*

+ `C3`: Set up wireless data rate.
  `"\xAA\xFA\xC3"` + `"\u0000\u2580"`
  12OO-1152OO bps
  default: 96OObps

+ `B4`: Set up receiving bandwidth.
  `"\xAA\xFA\xB4"` + `"\u0069"`

+ `A5`: Set up frequency deviation.
  `"\xAA\xFA\xA5"` + `"\x0a"`
  10 to 160 **kHz**

+ `96`: Set up transmission power.
  `"\xAA\xFA\x96"` + `"\x00"`
  0 to 7 indexes: *{1, 2, 5, 8, 11, 14, 17, 20}* **dBm**

+ `1E`: Set up UART transfer speed.
  > **Note**: DO NOT CHANGE THIS CONFIGURATION, OTHERWISE IT CLOUD BREAK THE MODULE.

  Set up UART transfer speed
  `"\xAA\xFA\x1E"` `"\u0000\u2580"`

+ `87`: Wireless signal strength when receiving useful data.
  `"\xAA\xFA\x87"` (no input)
  RSSI value is 8 bit, range: O-255

+ `78`: Disturb wireless signal strength.
  `"\xAA\xFA\x78"` (no input)
  RSSI value is 8 bit , range: O-255

### Usage

Only need to write data into `UART`, data will transmitted and will received on the other side.

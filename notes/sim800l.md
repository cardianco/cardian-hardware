# SIM800L

## Table Of Content
- [SIM800L](#sim800l)
  - [Table Of Content](#table-of-content)
  - [Documentation](#documentation)
    - [Connections](#connections)
      - [Notes](#notes)
      - [Pins](#pins)
  - [Notes](#notes-1)
      - [AT Command syntax](#at-command-syntax)
      - [Supported character sets](#supported-character-sets)
  - [Commands](#commands)
    - [General](#general)
    - [Network](#network)
      - [Network Registration](#network-registration)
      - [Signal Quality Report](#signal-quality-report)
      - [Set Phone Functionality](#set-phone-functionality)
      - [SIM PIN](#sim-pin)
    - [Communication](#communication)
      - [Call](#call)
      - [SMS](#sms)
      - [GPRS](#gprs)
  - [Setup GPRS](#setup-gprs)
    - [TCP/UDP](#tcpudp)
  - [References](#references)

## Documentation
### Connections
#### Notes
1. The module requires a power source of `4.4` (<u>***Not 5!***</u>) volts and `2A` of current.
2. It is recommended to use `3.3V` to send commands from the microcontroller when using the *Sim800* module's `RX` and `TX` to connect with the *MCU*
   (*By putting a* **diode** *in the route*).<br>
   Tx &rarr; RX and RX &rarr; TX.
3. To determine if the module is activated or not, try using the **Ring pin** (mentioned below).<br>
   It must remain always **On**.
4. Your module is restarting if:
   1. The led blinks quickly, pauses, and then resumes flashing quickly.
   2. If an **LED** is attached to the **Ring** pin, it will turn <u>off</u>.
   3. A **Null** Unicode character will be sent through **USART** to the **MCU**.
   4. You can see `SMS Ready` or `Call Ready` following each command.
5. There are **2A** spikes while registering SIM, calling, and so on that *cannot be detected with a multimeter* and cause modules to restart; to avoid this, use a capacitor between the module's `VCC` and `GND` (*1500uf must be enough*).

#### Pins
||||
|---:|---|:---|
|IPX ANT 1                   | - |RING *(Phone Ring)*|
|*(antena)*               Net| - |DTR|
|                       VCC  | - |MIC+ *(Microphone)*|
|*(reset)*                RST| - |MIC- *(Microphone)*|
|*(UART data reciver)*    RXD| - |SPK+ *(Speaker)*|
|*(UART data transmiter)* TXD| - |SPK- *(Speaker)*|
|*(Ground)*               GND| - ||

As it show above, connections to mcu are like:
+ GND<sub>(module)</sub> &rarr; GND<sub>(mcu)</sub> & GND<sub>(power supply)</sub>.
+ VCC<sub>(module)</sub> &rarr; VCC<sub>(power supply)</sub>.
+ TX<sub>(module)</sub> &rarr; RX<sub>(mcu)</sub>.
+ RX<sub>(module)</sub> &larr; Diod<sub>(optional)</sub> &larr; TX<sub>(mcu)</sub>.

## Notes
#### AT Command syntax
The `AT` or `at` or `aT` or `At` prefix must be set at the beginning of each Command line. To terminate a Command line enter `<LF>` `(\n)`

#### Supported character sets

The SIM7080 Series AT Command interface defaults to the IRA character set. The SIM7080 Series supports the following character sets:
+ `GSM format`, `UCS2`, `HEX`, `IRA`, `PCCP`, `PCDN`, `8859-1`

*The character set can be set and interrogated using the `AT+CSCS` Command (3GPP TS 27.007).<br>The character set is defined in GSM specification 3GPP TS 27.005.*

## Commands
### General
1. Power Off: `AT+CPOWD=<n>`<br>
   |`<n>`|description|
   |---|---|
   |0  |Power off urgently *(Will not send out NORMAL POWER DOWN)*|
   |1  |Normal power off *(Will send out NORMAL POWER DOWN)*|
2. Get Local Timestamp: `AT+CLTS=<mode>`<br>
   Sets Module time to local UTC from service provider.
   
   |`<mode>`|description|
   |--------|---|
   |0       |Disable|
   |1       |Enable |
3. a

### Network

#### Network Registration
To check if the SIM registered: `AT+CREG`

**Output**:<br>
command: `+CREG: <n>,<stat>[,<lac>,<ci>]`

`<n>`:

0. Disable network registration.
1. Enable network registration.
2. Enable network registration, with location info.<br> `+CREG: <stat>[,<lac>,<ci>]`.

`<stat>`:

0. Not registered, MT is not currently searching a new operator to register to.
1. Registered, home network
2. Not registered, but MT is currently searching a new operator to register to.
3. Registration denied.
4. Unknown.
5. Registered, roaming.

`<lac>`: String type `""` two byte location area code in hexadecimal format.

`<ci>`: String type `""` two byte cell ID in hexadecimal format
To check if SIM signal: `AT+CSQ`

*Unsolicited Result Code*:
+ if `<n>=1` and `<stat>` is changing.
+ if `<n>=2` and `<stat>` or `<lac>,<ci>` are changing.

#### Signal Quality Report

```
AT+CSQ
```
Output: `+CSQ: <rssi>,<ber>`

|`<rssi>`|description|
|--------|:---:|
|0       |-115 dBm or less|
|1       |-111 dBm|
|2...30  |-110... -54 dBm|
|31      |-52 dBm or greater|
|99      |not known or not detectable|

|`<ber>`|description|
|-------|:---:|
|0...7  |As RXQUAL values in the table in GSM 05.08 [20] subclause 7.2.4|
|99     |Not known or not detectable|

#### Set Phone Functionality

command: `AT+CFUN=<fun>[,<rst>]`

|`<fun>` |description|
|--------|---|
|0       |Minimum functionality|
|<u>1</u>|Full functionality (Default)|
|4       |Disable phone both transmit and receive RF circuits.|

`<rst>`: 1 <sub>*Reset the MT before setting it to `<fun>` power level*</sub>

#### SIM PIN

`AT+CPIN?`

|`<code>`   |description|
|-----------|---|
|READY      |MT is not pending for any password.|
|SIM PIN    |MT is waiting SIM PIN to be given.|
|SIM PUK    |MT is waiting for SIM PUK to be given.|
|PH_SIM PIN |ME is waiting for phone to SIM card (antitheft).|
|PH_SIM PUK |ME is waiting for SIM PUK (antitheft).|
|SIM PIN2   |PIN2, e.g. for editing the FDN book possible<br>only if preceding Command was acknowledged with +CME ERROR: 17|
|SIM PUK2   |Possible only if preceding Command was acknowledged with error +CME ERROR: 18.|

### Communication
#### Call
Command for call: `ATD` `<country code><phone number>` `;` `<cr><lf>`
*Example*: `ATD+989381234567;\r\n`

Command for **cancel** call: `ATH`

#### SMS

##### Send

1. First switch to TEXT mode.<br>
   `AT+CMGF=1`
2. Send Command for *SMS*:
   `AT+CMGS="<country code><phone number>"` `<lf>` `this is a test` `<ctrl + z>`.<br>
    Example: `AT+CMGS=\"+989389898990\"\nthis is a test\x1a`

##### Read

1. `AT+CMGR=<index>`: Read message.

2. `AT+CMGF=1`: Switch to text mode. `0`<sub>pdu</sub>  `1`<sub>text</sub>

3. `AT+CMGD=<index>,<mode>`: Delete message, index from `1`.

    `0`<sub>one</sub> `1`<sub>all read</sub>  `2`<sub>all read & sent</sub> `3`<sub>all read, sent & unsent</sub>  `4`<sub>all</sub>

4. `AT+CMGD=0,4`: Delete all messages.

#### GPRS

`AT+CGATT`: Attach or detach from GPRS service.
[More (Setup GPRS)](#setup-gprs)

### TCP/UDP

1. Close connection<br>
   `AT+CIPCLOSE`
   |`<n>`|description|
   |-----|---|
   |0    |Slow close|
   |1    |Quick close|

   `<id>`: A numeric parameter which indicates the connection number|

2. Set CSD or GPRS for Connection Mode
	```
	AT+CIPCSGP=1,"internet"
	```
3. Open connection. <br>
   Single connection: <br>
   `AT+CIPSTART=` `<mode>`,`<IP address>`,`<port>` Or<br>`AT+CIPSTART=<mode>`,`<domain name>`,`<port>` <br>
   Multi connection: <br>`AT+CIPSTART=` `<n>`,`<mode>`,`<IP address>`,`<port>` Or<br> `AT+CIPSTART=<n>`,`<mode>`,`<domain name>`,`<port>`

   |`<mode>`|description|
   |---|---|
   |"TCP"| Establish a TCP connection|
   |"UDP"| Establish a UDP connection|
   
   `<n>`: connection number.
4. Connection status.<br>
   `AT+CIPSTATUS`
   
   |||
   |---|---|
   |||

## References

*[0] SIM800 Series - AT Command Manual, Ver 1.12. SIMCom, China, 2020.6.15.*

[sim800l_manual]: https://www.elecrow.com/wiki/images/2/20/SIM800_Series_AT_Command_Manual_V1.09.pdf "Series_AT Command Manual_V1.09"
# Bluetooth JDY31

|Command        |Function            |Response    |
|---------------|--------------------|------------|
|`AT+VERSION`   |version number      |JDY-31-V1.2 |
|`AT+RESET`     |Soft reset          |+Ok         |
|`AT+DISC`      |Disconnected        |+Ok         |
|`AT+LADDR`     |module's MAC address|12-digit num|
|`AT+PIN`       |password            |default:1234|
|`AT+PIN<pin>`  |set password        |+Ok         |
|`AT+BAUD`      |Baud rate           |4-2         |
|`AT+NAME`      |Bluetooth name      |JDY-31-SPP  |
|`AT+NAME<name>`|Set bluetooth name  |+Ok         |
|`AT+DEFAULT`   |reset factory       |+Ok         |
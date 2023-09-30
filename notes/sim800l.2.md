

1. `ATA`: **Answer** an incoming call.

2. `ATZ`: **Reset** default configuration.

3. `ATI`: Modules **information**.

4. `ATD+980123456789;`: Make a **call**.

5. `ATH`: **Terminate** call.

6. `AT&F`: **Factory** defined configuration.

7. `AT&V`: Display current **config**.

8. `AT&V0`: Display current config, responses in numeric format.

9. `AT&W`: Store active profile.

10. `AT+HVOIC`: Disconnect voice call only.

11. `AT+CPOWD=1`: Normal **power off**.

12. `AT+CPOWD=0`: Urgently **power off**.

13. `AT+SJDR=<n>`: Set **jamming detection** function. `0` <sub>disable</sub>,  `1` <sub>enable</sub>

14. `AT+CANT=1,0,10`: Enable **antenna detecting**.

15. `AT+CANT=1,1,10`: Enable **antenna detecting** & reporting it every *10s*.

16. `AT+CROAMING`: **Roaming** state.`0`<sub>home</sub>, `1`<sub>international</sub>, `2`<sub>other</sub>

17. `AT+CBATCHK=1`: Set `VBAT` checking feature `ON/OFF`.

18. `AT+CPAS`: **Phone activity** status.`0`<sub>ready</sub>, `2`<sub>unknown</sub>, `3`<sub>ringing</sub>, `4` <sub>call in progress</sub>

19. `AT+CSQ`: **Signal quality** report,
    `0`<sub>&chi;&lt;-115</sub> , `1`<sub>-111</sub> , `2`&middot;&middot;&middot;`30`<sub>-110...-54</sub> , `31`<sub>-52&lt;&chi;</sub>  ,  `99`<sub>unknown</sub>.

    `0`&middot;&middot;&middot;`7`: `0` is best `7` is worst.

20. `AT+CFUN=1`: Set full phone functionality.

    `0`<sub>minimum</sub>, `1`<sub>full</sub>, `4`<sub>disable RF's RX&TX</sub>

21. `AT+CFUN=1,1`: Reset & set full **phone functionality**.

22. `AT+CCLK?`: **Clock**.

23. `AT+CBC`: **Battery** charge, `bcs`, `bcl`, `voltage`

    `bcs`: `0`<sub>not charging</sub>, `1`<sub>charging</sub>, `2`<sub>finished</sub>

    `bcl`: Charge precent

    `voltage`: mV

24. `AT+CSCLK?`: **Slow clock** mode.

25. `AT+CSCLK=0`: Disable **slow clock**. `0`<sub>disable</sub>, `1`<sub>enable</sub>, `2`<sub>automatic</sub>

26. `AT+CENG`: Switch on or off **engineering** mode.



#### GPRS

`AT+CGQREQ?`: `<cid>`,`<precedence>`,`<delay>`,`<reliability>`,`<peak>`,`<mean>`,`+CGQREQ`

1. `AT+CGATT=1`: Attach or detach from GPRS service. `0`<sub>detached</sub>, `1`<sub>attached</sub>

1. `AT+SAPBR=3,1,"Contype","GPRS"`: Set connection type to GPRS.

1. `AT+SAPBR=3,1,"APN","mtnirancell"`: Set APN operator.

1. `AT+SAPBR=1,1`: Enable GPRS.

1. `AT+SAPBR=0,1`: Disable GPRS.

1. `AT+SAPBR=2,1`: Get IP.

1. `AT+SAPBR=<cmd_type>,<cid>[,<ConParamTag>,<ConParamValue>]`

   `<cmd_type>`: `0` <sub>Close</sub>, `1` <sub>Open</sub>, `2` <sub>Query</sub>, `3` <sub>Set parameters</sub>, `4` <sub>Get parameters</sub>

   `<cid>`: <sub>Bearer profile identifier</sub>

   `<status>`:  `0` <sub>connecting</sub>, `1` <sub>connected</sub>, `2` <sub>closing</sub>, `3` <sub>closed</sub>
   `<ConParamTag>`: 
   
   + `"CONTYPE"`: <sub>Type of Internet connection.</sub>
   + `"APN"`: <sub>Access point name.</sub>
   + `"USER"`: <sub>User name.</sub>
   + `"PWD"`: <sub>Password.</sub>
   + `"PHONENUM"`: <sub>Phone number for CSD call.</sub>
   + `"RATE"`: <sub>CSD connection rate.</sub>

#### Http

1. `AT+HTTPINIT`: initialize

#### Restart

```bash
# Airplane mode
AT+CFUN=0
# Full function
AT+CFUN=1
```

#### Enable GPRS

```bash
# AT+SAPBR=<type>,<cid>[,<param>,<value>]
# <type>: 0.close 1.open 2.open 3.set parameter
# Set connection type
AT+SAPBR=3,1,"Contype","GPRS"
# Set APN(Access Point Name)
AT+SAPBR=3,1,"APN","mtnirancell"
# Enable GPRS, Open bearer
AT+SAPBR=1,1
# Query bearer
AT+SAPBR=2,1
```

#### Disable GPRS

```bash
# Disable GPRS
AT+SAPBR=0,1
```

#### Http Request

```bash
# Initialize
AT+HTTPINIT

# Add Http params
# AT+HTTPPARA=<param name>,<value>
# AT+HTTPPARA="CID",<profile-id>
AT+HTTPPARA="CID",1

# AT+HTTPPARA="URL",<target url>
AT+HTTPPARA="URL","https://cardian.ir/graphql"

# Set Http content-type
# AT+HTTPPARA="CONTENT","<mime types>"
AT+HTTPPARA="CONTENT","application/json"

# AT+HTTPDATA=<data size>,<wait in ms>
AT+HTTPDATA=192,5000

# Post DATA, if there is a post
postdata

# AT+HTTPACTION=<action>
# <action>:0 GET, 1 POST, 2 HEAD, 3 DELETE
AT+HTTPACTION=0

# Read all data
AT+HTTPREAD

# Terminate HTTP Service
AT+HTTPTERM  
```

#### TCP Connection

```bash
# set to single connection
AT+CIPMUX=0
# Set default APN
AT+CSTT="mtnirancell","",""
# Start wireless connection
AT+CIICR
# Get assigned IP address
AT+CIFSR

# Close old connections (optional)
# $ SHUT OK
AT+CIPSHUT
# manually read data (optional)
AT+CIPRXGET=1
# AT+CIPSTART="<TCP/UDP>","<server>","<port>"
AT+CIPSTART="TCP","cardian.ir",80
# Send TCP packet
# AT+CIPSEND=<data-length>
AT+CIPSEND=107

POST /index.html HTTP/1.0\nHOST: cardian.ir\nContent-type: application/json\nContent-length: 13\n{"test":true}\n

# Close TCP connection
AT+CIPCLOSE
```

> **NOTE**: If `+PDP: DEACT` URC is reported which means the GPRS is released by the
> network, then user still needs to execute `AT+CIPSHUT` command to make PDP context come back to original state.

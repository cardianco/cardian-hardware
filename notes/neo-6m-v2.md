### Configuration:

#### $GPGGA:

<kbd>$GPGGA</kbd>
 ,`[0]` [<kbd>utc</kbd>](#c0)
 ,`[1]` [<kbd>latitude</kbd>](#c1)
 ,`[2]` [<kbd>N</kbd>](#c2)
 ,`[3]` [<kbd>Longitude</kbd>](#c3)
 ,`[4]` [<kbd>W</kbd>](#c4)
 ,`[5]` [<kbd>quality-code</kbd>](#c5)
 ,`[6]` [<kbd>satellites-num</kbd>](#c6)
 ,`[7]` [<kbd>horizontal-dilution</kbd>](#c7)
 ,`[8]` [<kbd>altitude</kbd>](#c8)
 ,`[9]` [<kbd>M</kbd>](#c9)
 ,`[10]` [<kbd>mean-sea-level</kbd>](#c10)
 ,`[11]` [<kbd>M</kbd>](#c11)
 ,`[12]` [<kbd>last-DGPS-UTC | dgps-station-id</kbd>](#c12)
 ,`[13]` [<kbd>checksum</kbd>](#c13)

0. <b id='c0'>UTC</b> – represents the time at which the fix location was taken, 11:06:17 UTC
1. <b id='c1'>latitude</b> – latitude 41 deg XX.XXXXX’ N
2. <b id='c2'>N</b>
3. <b id='c3'>longitude</b> – Longitude 008 deg 31.54761′ W
4. <b id='c4'>W</b>
5. <b id='c5'>quality-code</b> – fix quality (0 = invalid; 1= GPS fix; 2 = DGPS fix; 3 = PPS fix; 4 = Real Time Kinematic; 5 = Float RTK; 6 = estimated (dead reckoning); 7 = Manual input mode; 8 = Simulation mode)
6. <b id='c6'>satellites-num</b> – number of satellites being tracked
7. <b id='c7'>horizontal-dilution</b> – Horizontal dilution of position
8. <b id='c8'>altitude</b> – Altitude, in meters above the sea level
9. <b id='c9'>M</b>
10. <b id='c10'>mean-sea-level</b> – Height of geoid (mean sea level) above WGS84 ellipsoid
11. <b id='c11'>M</b>
12. <b id='c12'>last-DGPS-UTC</b> – time in seconds since last DGPS update<br>
    <b id='c121'>DGPS-station-id</b> – DGPS station ID number
14. <b id='c13'>*42</b> – the checksum data, always begins with `*`

#### other values:

- `$GPGSA`: GPS DOP and active satellites
- `$GPGSV`: Detailed GPS satellite information
- `$GPGLL`: Geographic Latitude and Longitude
- `$GPRMC`: Essential GPS pvt (position, velocity, time) data
- `$GPVTG`: Velocity made good

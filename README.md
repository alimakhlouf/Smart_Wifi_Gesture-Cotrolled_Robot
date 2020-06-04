# Smart_Wifi_Robot
### As the name reveals this is a robot that is able to do the following:

**1-  it's controlled wirelessly by hand movements through WIFI technology**

**2- it can read Temperature, Humidity Percentage and Smoke Concentration and upload these information  on a website that can be accessed by anyone.**

**3- It can also upload these information to an LCD screen placed on your hand if you requested it.**

**4- you can adjust Robot WIFI options with your mobile using Bluetooth.**

**5- detects if the Robot is facing an obstacle and if so it stops and informs both the user and the webpage.**

**6- The Robot body consists of 2 ECUs that communicate with each other using CAN Protocol.** 



### the project consists of 3 ECUs

#### **1- Remote Control ECU placed on the user gloves** 

####  it's responsible for:

- reading the user's hand movement and then command the robot to move a certain move. 
- contains buttons that the user can us to ask the robot of the current sensors readings (Temperature, Humidity and Smoke)
- Has an LCD screen attached to it to provide some user interface and display the latest sensors readings.
- displays a "block" message if the robot has faced an obstacle or a warning message if a sensor reading has exceeded a certain limit.
-  the WiFi module can be configured by mobile using Bluetooth. 

#### 2- WiFi-Motors ECU placed on the Robot

#### it's responsible for:

- Getting the sensors readings from the Sensors ECU through CAN Protocol and send them to the user and the webpage
- Communicating with the Remote Control ECU .
- Controlling the motors , an thus stops the motors if the Sensors ECU detected an obstacle.
-  the WiFi module can be configured by mobile using Bluetooth. 

#### 3- Sensors ECU

#### it's responsible for :

- sending the sensors reading to the WiFi-Motors ECU whenever it gets a requests
- sending an immediate message to the WiFi-Motors ECU if an obstacle has been detected or a certain sensor reading has exceeded a certain limit.



### Hardware Specifications:

#### modules used :

1. ATMEGA32/16 MCUs.
2. 16x2 LCD (hd44780) with a serial access using SIPO shift register IC (74hc595). 
3. WIFI module(ESP8266).
4. CAN Controller Module(mcp2515).
5. Motor Drive IC (L292d).
6. Ultrasonic sensor (hc-sr04) for detecting obstacles.
7. Gas sensor (MQ-2). 
8. Temperature and Humidity sensor (DHT11).
9. 3-Axis Gyro+Accelerometer Sensor (MPU-6050) for detecting and movements




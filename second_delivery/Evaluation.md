# Evaluation
 
## Energy Consumption
The energy consumption is one of the main constraints of the system in terms of evaluation. This system is powered by 3 batteries with a total amount of 3750mAh. Alternatives of this solution could be using solar energy or rechargeable batteries. However, the solar panels would be costly so it is not respecting the constraints we have on the terms of production cost. Also the rechargeable batteries cannot be used since the system is thought to be used also in gardens where it will not be possible to charge it continuously.
 
We will activate the components:
 
•Display: The value will be updated after every measurement and it will remain active on some small intervals of time. More precisely it will be activated for 1 second every 1 minute.
 
•Relay: Tt will be activated only when irrigation is needed. The assumption we make is that the relay will be active only for 10 seconds per day since we do on average 2 irrigation per day with 5 seconds each.
 
•Sensors: Dht22 sensor will read the temperature values with a sampling period of 2 hours. On the other hand the Seesaw soil moisture sensor will have a variable sampling period (6/12 hours).
 
 
## Analysis on sampling frequency (Battery lifetime)
Here we will represents the power consumption the components need when they are on active mode:
dht22 = 1,5 mA
seesaw adafruit = 5 mA
relay = 135 mA
TFT ILI9341 LCD display = 30 mA

**Battery lifetime = Capacity / Current Intensity**
 
###### first approach
The first approach we considered was the one in which the sensors continuously sample, the relay pump is activated only twice a day(for irrigation purpose only) and the display is always on.

Component _name | mA | duty_cycle 
--- | --- | --- 
Dht22 | 1,5 | 100% 
Seesaw | 5 | 100% 
ILI9341 | 30 | 100% 
relay | 135 | ~ 0.001% 

Total: 1,5* 1 + 5* 1 + 30* 1 + 135* 0.000001 ≈ 36,5 mA.
With the battery of 3750 mA our system would last: 3750 mAh / 36,5mA = 103 hours ( something more than 4 days). This a very bad result since it doesn't satisfy the requirement to last at least one year. However it will not be necessary to continuously sample since the variation of the air temperature and soil moisture is not very high.
 
###### second approach
One second approach is that we sample with the dht22 sensor 1 time every 2 hours and with the humidity sensor once in 6 hours with 2 seconds active for every time we mwasyre. The relay is activated as we said before only 2 times in a day for 5 seconds eaach time and the display is always active.
 
Component _name | mA | duty_cycle 
--- | --- | --- 
Dht22 | 1,5 | ~ 0.03% 
Seesaw | 5 | ~ 0.009% 
ILI9341 | 30 | 100% 
relay | 135 | ~ 0.001% 

Total = 1,5* 0,0003 + 5* 0,00009 + 30* 1 + 135* 0,000001 ≈ 30,005 mA.
With the battery of 3750 mA our system would last: 3750 mAh / 30,005 mA = 125 hours (5 days)
Still we do not have good result. Notice that the major part of the consumption comes from the display which has always been active.By trying to reduce the active time od the display we for sure will have better results.

###### third approach
The approach that we are actually considering is to sample with the dht22 sensor 1 time every 2 hours and with the humidity sensor once in 6 hours with 2 seconds active for every time we mwasyre. The relay is activated as we said before only 2 times in a day for 5 seconds eaach time but now the display will be active for only 1 second every minute. We choose 1 second since it is reasonable to have a good monitoring but also considering the battery life improvement.


Component _name | mA | duty_cycle 
--- | --- | --- 
Dht22 | 1,5 | ~ 0.03% 
Seesaw | 5 | ~ 0.009% 
ILI9341 | 30 | ~ 1,6% 
relay | 135 | ~ 0.001% 

Total = 1,5* 0,0003 + 5* 0,00009 + 30* 0,016 + 135* 0,000001 ≈ 0,505 mA.
With the battery of 3750 mA our system would last: 3750 mAh / 0,505 mA = 7425 hours ( around 309 days)
This approach meets the requirement of making the battery last for almost 1 year.


## Messages structure
As we said before, there will be 3 types of messages we will send to the cloud.
All of them will have the same length of 6 bytes : [data{4}|{1}type{1}]
 
Temperature type message :
       data = value of the most recent temperature read
       type = 0000 0001
 
Soil moisture type message :
       data = values of the most recent soil moisture read
       type = 0000 0010
 
Irrigation type message :
       data = irrigation time
       type = 0000 0100
 
 
## Analysis on radio usage
To transmit the messages to the cloud we can choose to do it in 2 different ways:
1. Transmit after every time we read a value with the corresponding values and every time we irrigate with responding time of irrigation. In total, during a single day, we will send 12 messages with the temperature value, 4 messages with humidity value and 2 messages with irrigation time value.
The total time the radio will remain active is:
TOTAL = 18 * Twut(Wake Up Transceiver) + 18 * Ttr (Transmission Data)
 
2. Transmit only once a day. This way we will need memory to store the messages but we would reduce the frequency of waking up the transceiver. So the total time the radio will remain active is:
TOTAL = 1 * Twut + 18 * Ttr
 
The choice we consider is the one to use the first transmission method. Even though we might reduce the battery consumption and make it last more with the second method, it is not good since we would have the data updated once a day losing the possibility to monitor the system and act on it if needed.
 
## Network Usage
Using the LoRa protocol for communication we are able to connect to gateways found very far (till 15 km far) from where the system is located which goes along with the constraint of implementing the system within a couple of km close to the nearest gateway.
 
Even though this module has a small data  transmission bandwidth (50 kb/s), we do not have any problem with that since the length of the payload is only 6 bytes.
 
We will use IoT_Lab for 2 scopes. The first one is to test the system by creating a simulated environment. The other scope is the one to analyze a larger scale of the nodes.
 

#### Technology

### Smart Irrigation System


### IoT Architecture


<img width="777" alt="Screenshot 2022-03-20 at 14 55 15" src="https://user-images.githubusercontent.com/96237399/159165876-55e59552-4a00-490b-82ba-22a59c00d43c.png">


### Sensors and Actuators

There is presented a list of possible sensors and actuators for our system

## DHT22 temperature and humidity sensor

DHT22 temperature and humidity sensor is used to measure environment temperature and humidity in order to provide, according to the weather conditions, the right amount of water necessary for the plant’s growth.


<img width="276" alt="image" src="https://user-images.githubusercontent.com/96237399/159164804-97fc2d38-2c98-4d91-8ef7-c9c6f8f415c8.png">

    • Model DHT22 
    • Power supply 3.3-6V DC 
    • Output signal digital signal via single-bus 
    • Sensing element Polymer capacitor 
    • Operating range: humidity 0-100%RH / temperature -40~80 Celsius 
    • Accuracy humidity +-2%RH(Max +-5%RH); / temperature <+-0.5 Celsius
    • Resolution : humidity 0.1%RH / temperature 0.1 Celsius 
    • Repeatability humidity +-1%RH / temperature +-0.2 Celsius 
    • Humidity hysteresis +-0.3%RH 
    • Long-term Stability +-0.5%RH/year 
    • Sensing period Average: 2s 
    • Interchangeability fully interchangeable 
    • Dimensions small size 14*18*5.5mm; big size 22*28*5mm 
    
## Soil moisture sensor

Soil moisture sensor is used to measure the soil moisture in order to provide, according to the soil conditions, the right amount of water necessary for the plant’s growth.

<img width="381" alt="Screenshot 2022-03-20 at 15 11 06" src="https://user-images.githubusercontent.com/96237399/159166696-30cfc39f-ea52-4960-a255-588bd2d76027.png">



    • Operating voltage: 3.3 ~ 5.5 VDC
    • Output voltage: 0 ~ 3.0 VDC
    • Operating Current: 5mA 
    • Interface: PH2.54-3P
    • Dimensions: 98 x 23 mm (WxW)
    • Weight 15g
    • Pin: analog signal output, GND, VCC
    
## 2 Channel relay module

 Channel relay module is used to trigger the water pump.
 

<img width="365" alt="Screenshot 2022-03-20 at 15 07 52" src="https://user-images.githubusercontent.com/96237399/159166467-4ff64821-48b7-46e7-be1d-d2a96185cac2.png">

     
    • Contact current 10A and 250V AC or 30V DC
    • Each channel has indication LED
    • Coil voltage 12V per channel
    • Kit operating voltage 5-12 V 
    • Input signal 3-5 V for each channel
    • Three pins are normally open and closed for each channel
    
## OLED Display

Oled display allows you to show the soil and weather conditions provided by sensors. In order to have an efficient system il display won't be always turned on but only sometimes per day.


<img width="301" alt="Screenshot 2022-03-20 at 15 09 15" src="https://user-images.githubusercontent.com/96237399/159166538-fca87343-f9f5-4cb2-b1b6-0276cd6d9eb2.png">


    • Resolution: 128 x 64
    • Model: SH1106
    • PCB Size: 35.4 x 33.5 mm
    • Active Area: 29.42 x 14.7 mm
    • Interface Type: SPI / I2C 
    • Display Color:White
    • Operating Voltage: 3.3V - 5.0V
    • Logic Voltage: 3.3V
    • Pixel Pitch: 0.23 x 0.23 (mm)
    • Pixel Size: 0.21 x 0.21 (mm)
    • Operating Current: 20ma
    • Operating Temperature: -40~70℃
    • Dimensions: 35.4 X 33.5mm
    • Weight: 5.7g
    
## Water pump

Water pump is activated according to the values provided by sensors. Trigger by relay module, the water pump will take the right water amount to irrigate the plants.


<img width="272" alt="Screenshot 2022-03-20 at 15 12 17" src="https://user-images.githubusercontent.com/96237399/159166704-92702612-6f3b-4d34-bbe6-15ca21031653.png">


    • Operating Voltage: 3-5 V
    • Operating Current: 100 - 200 mA
    • Water Inlet Entrance Dimension: 5.0 mm
    • Water Outlet Exit Outer Diameter: 7.5 mm
    • Water Outlet Exit Inner Diameter: 4.5 mm
    • Head Distance: 0.3 - 0.8 meter
    • Flow Rate: 1.2 - 1.6 Liter per minute
    • Weight: 28g   
    
    
### COMMUNICATION

## Wifi Module (ESP-01S ESP8266)

Used as communication module to get access to the internet.


![51u-rqsuZgL _SL500_](https://user-images.githubusercontent.com/96237399/159165388-07003f71-63bd-4d24-bdf0-763848b891c6.jpg)

    • Processor: L106 32-bit RISC microprocessor core based on the Tensilica Xtensa Diamond Standard 106Micro running at 80 MHz.
    • Memory:
    • 32 KiB instruction RAM
    • 32 KiB instruction cache RAM
    • 80 KiB user-data RAM
    • 16 KiB ETS system-data •AM
    • External QSPI flash: up to 16 MiB is supported (512 KiB to 4 MiB typically included)
    • IEEE 802.11 b/g/n Wi-Fi.
        • Integrated TR switch, balun, LNA, power ampli er and matching network
     • WEP or WPA/WPA2 authentication, or open network
    • 17 GPIO pins.
    • SPI.
    • I²C (software implementation)

### MQTT Broker (AWS Core IOT)

Sensor datas are transmitted through a message broker to the IoT core through publish
method for data preprocessing.
   
   ### DEVELPOMENT BOARD

### STM32 Nucleo F401RE


![F8029425-01](https://user-images.githubusercontent.com/96237399/159165481-b6815bd4-fa2d-459c-95cc-ee73c7a17801.jpg)

    • Microcontroller: STM32F401RET6 (32-bit)
    • Architecture: ARM Cortex M4 CPU with FPU
    • Power consumption: 2.4uA at standby without RTC
    • CPU Frequency: 84 MHz
    • Crystal Oscillator Range: 4 to 26 MHz
    • MCU Operating Voltage (VDD): 1.7V to 3.6V
    • Board Operating Voltage (VIN): 7V to 15V
    • Flash Memory: 512KB
    • SRAM: 96 KB
    • GPIO Pins: 50
    • ADC: 12-bit 16Channel
    • RTC: In-built 32kHz with calibration
    • Timers :
        • 16-bit (6)
        • 32-bit (2)
    • Watchdog Timers: 2
    • USART/UART Communication: 4
    • I2C Communication: 3
    • SPI Communication: 3
    • USB2.0 Support: Yes
    • Internal Crystal Oscillator: Yes, 16MHz
    • On Board Debugger: Yes, Serial Wire and JTAG 
 
    
### UBUNTU Server

An Ubuntu server will be hosted on the AWS cloud to run the MQTT broker service
    
### Web Dashboard

Web dashboard is used as platform to display the dataset in visual form. With Web
dashboard the end user can also send command back to actuators whether to take
action or not
    
    
    
    
    

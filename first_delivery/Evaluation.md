# Evaluation

## Network Technology
The system will communicate with the cloud using MQTT protocol over a **secure** network connection.

We will estimate the **latency** of the system from 2 different points of view:
	1- The time the system needs to read sensors, elaborate them and change the state of the actuators
	2- The time the data will be displayed on the dashboard(will depend form the type of MQTT protocol used)
	
## Embedded System
We will also use the **IoT lab** to monitor the **power consumption** of our components. In this environment we will create a **power management system** to also measure the **correctness** of our measurements. The sensors will be **read periodically** creating in this way a lower power consumption. The rate of reading the sensors may vary from 5 values/day during the hottest days to 2 values/day during the coldest ones. The Embedded system will be isolated and will need a battery from which it takes the power. Some values for the power consumption are read also in the description of every product.


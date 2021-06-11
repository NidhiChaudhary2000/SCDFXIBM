# SCDFXIBM TEAM: Spokes For Folks

Using IoT sensors integrated inside a resuable Personal Protective Equipment (PPE) suit, we aim to give first responders the comfort and the care they deserve when responding to a pandemic.

## The Prototype

We used an ESP32 to actuate the sensors. Due to limitations in what materials we had, we could only work with a temparature and humidity sensor.

![ESP32](https://user-images.githubusercontent.com/85609211/121624043-5e916900-caa3-11eb-9d84-510e08fbd472.png)


The programme is able to read temperature and humidity and using the code in Arduino IDE, we are able to send the information to IBM Watson IoT 
Platform where a dashboard is updated.

In order to communicate with the IoT platform, we used the Cloud Foundry Service from IBM Cloud and connected it with our application.
![image](https://user-images.githubusercontent.com/85609211/121624425-1757a800-caa4-11eb-9d02-088a273648d2.png)

Finally, we connected the device to the IoT platform and were able to send real time information about temperature and humidity to the dashboard.
![IBM connect](https://user-images.githubusercontent.com/85609211/121624630-71f10400-caa4-11eb-928a-1e3b3f752206.png)
![Temp monitor](https://user-images.githubusercontent.com/85609211/121624638-73bac780-caa4-11eb-928c-b6a2b1517c01.png)


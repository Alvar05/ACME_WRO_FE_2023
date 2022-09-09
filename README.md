Engineering materials
====

This repository contains engineering materials of a self-driven vehicle's model participating in the WRO Future Engineers competition in the season 2022 of the team ACME.

## Content

* `t-photos` contains 2 photos of the team (an official one and one funny photo with all team members). Also, a panoramic version of the Funny photo and a link to its making-of video.
* `v-photos` contains 8 photos of the vehicle (6 from every side, top and bottom, 1 with last year's vehicle and 1 without the M5Stack and all the components visible).
* `video` contains the video.md file with the link to a video with a driving demonstration.
* `schemes` contains one schematic diagram of the electromechanical components, illustrating all the elements (electronic components and motors) used in the vehicle and how they connect to each other.
* `src` contains code of control software for all components which were programmed to participate in the competition.
* `models` contains all the 3D-printed parts of the vehicle. 
* `other` contains the PCBs of the vehicle and the datasheets of all the components.
* `YouTube Channel` All public videos are uploaded at https://www.youtube.com/channel/UCMpIuC9MiNL4U_FDTKrd1CQ
 
## Introduction
 
Since our participation last year, we have made a new vehicle (smaller and faster) using what we learnt from the old vehicle, which had two Arduino controlled boards communicated by I2C, so we could use a larger number of sensors.



## This year vehicle

This year we are improving the robot, our main goals are to make it smaller and faster, so we are building it all over again, over a different base. This time, instead of making our own PLA structure, we used an RC mini-Z toy car and started building the new vehicle over its base structure.

The code consists of 2 different programs for both M5Stack and ProMicro controller. The M5Stack is the main controller (Master) and connects by I2C with the ProMicro, which controls everything related to HC-04 sensors and sends it to the master. Every module of each program contains a simple description of what it does at the beggining of it, but, basically, they contain functions to use in the main program related to the electromechanical part specified or I2C communication.

To compile and upload the programs to the controllers, we use Arduino IDE. To upload the code to the ESP32 controller of the M5Stack, you can follow the steps in https://docs.m5stack.com/en/quick_start/m5core/Arduino.



### Problems with the processor

At first, we tried to implement the same board distribution on the new robot; two boards, with one Arduino nano each; but, soon we realised that, due to the change of structure and the new sensors used (ex; OPT 3101), the two board distribution became inappropriate.
The Arduino memory wasn’t large enough, and fitting two boards into such a small structure was almost impossible. Our solution was to implement the M5Stack processor.

### New tires

We tried to create our own rubber tires for the new vehicle using PLA-made molds. The first few tires turned out to be not good enough, so we bought and started using Pololu tires. We will be making more tires, hoping that we can substitute the Pololu ones the moment we get ours to be good enough.

### Receiving data from the robot to ease debugging
 
To receive data from the new robot, we designed an APP with MIT App Inventor (https://appinventor.mit.edu/) to have better management and distribution of the info. We also can interact with it and make changes in some variables without having to upload a new sketch (to save time). This APP works with bluetooth (previously disabled before the competition) and connects with the bluetooth module of the M5Stack. 
 
The elements we have in the APP are some variables to change like speedness or constants for the PID functions, a console to send data to the robot, a Play/Stop button and a graphic section which shows a 2D version of the field with a car that estimates and emulates the moves of the real robot in the screen and draw lines that show the path the robot made. 
 
The method used to emulate the robot moves is odometry based on the two encoders that are placed at both of the rear tires, which can rotate freely and don’t steer (this allows us to use a differential-based odometry method). Later on, we found out that our encoders weren't precise enough to have a good actualization of the position, causing lots of error between the estimation and the real position. 
 
By the code part, we use the BluetoothSerial.h library to control the bluetooth data of the M5Stack. To update the data from the robot, the APP sends a petition to the robot which, when received, returns a string between a first “<” character and a final “>” character to make it easier for the APP to detect the correct data and avoid other noise. The data received outside the “<string>”, is accumulated and when a \n is received, all the accumulated data is printed in the screen console (this allows us to send some flag words to the APP console using the println() method and have more control on what is happening in the running program).
 
### Choosing sensors
 
We thought of introducing a sensor that would allow us to read distances somehow more accurately than an ultrasonic sensor and over a wider range than a conventional infrared sensor.
 
We chose the OPT3101 sensor from Pololu, with components from Texas Instruments. This is a Time Of Flight (TOF) sensor, which measures distances of up to 1 metre by calculating the time it takes for a previously emitted infrared light beam to bounce back to the sensor.
 
It has six emitters and two receivers that allow the data to be divided into three channels: left, centre and right, with a range of fifty, sixty and fifty degrees respectively.
The operation of this type of sensor avoids many problems as it does not depend on the intensity of the light received, as would be the case with a conventional infrared sensor.
Sensor website:
https://www.pololu.com/product/3680
 
We tried to communicate this sensor via I2C with our main processor, serving as a slave of the Arduino, along with other sensors such as the MPU6050 gyroscope.
Because of this, we had to ensure that the libraries of each sensor were compatible with each other, as well as programming the reception of data with interrupts to prevent data from different sensors from interfering with each other.


### Measuring distances
Just like we did in the old vehicle, the distance it moves must be somehow recorded, and for this task we are using hall effect sensors as encoders. To implement those sensors, the box in between the rear wheels containing the differential has been replaced by a modified copy that makes space for three sensors.
Each wheel is connected to a cylindrical magnet which, when moving, is detected by the sensor, giving a signal every few degrees. This way we can get an idea of the distance travelled by the robot. The third sensor is supposed to read the rotation degrees of the motor, so we can estimate the actual speed of the motor.


### Problems whith the OPT3031
The Tof sensor that we tried to use ended up generating more problems that the ones it solved.
The use of this sensor at the same time of the MPU6050 gyroscope caused trouble, it stopped working at some point whithout any apearent reason and the data recieved was quite irregular.
We thought on implementing a different Tof sensor, being the VL53LX our main option but, by now, we will be using the HC-sr04 ultrasonic sensor, that seems to allways work precisely enough. 
Despite of that, the ultrasonic has generated some problems until now, but those seem to be easier to deal whith. That is why we won't change it yet.
The main ploblem seems to be caused by the vibration of the wheels when spining. This vibration spreds though the robot and to the sensor, making it send false data.

### Implementing VL53L1X

Finally, we tried the VL53L1X that seemed to work really good in short distances (not working really good with non-reflective black walls at high distances). The solution was to implement HC-sr04 and VL53L1X at the same time, using ultrasonics in long distances (with a slower refresh) and the TOF ones in shorter ranges.

### The HC-sr04 sensors' issues
As result of using again the ultrasonic sensors some old problems showed up again. Due to the fact that the HC-sr04 has a much narrower range than the OPT3031 we have seen ourselves pushed into using three HC to cover the two sides of the robot aswell as the front. The problem abot that is that we don't have enough pins in the M5Stack to connect that much sensors.
We are using again two boards, one controlled by the M5 and the other by an Arduino, this way we can get the pins we need.

### Camera module
To achieve the goal of detecting the signs and their colour we used the HuskyLens camera module connected to the M5Stack using I2C. The HuskyLens camera has its own processor which outputs the detected object information (x and y position, height and width, colour…) to the M5Stack. Then the M5Stack can use the height value to do a simple rule of three, with a pre-calibrated value, and calculate the distance to the object; or it can use the x value to estimate the direction of the object. It also reads the colour value and depending on the sign colour makes the robot turn right (if it’s red) or left (if it’s green). The camera is mounted with a 3D-printed support to a servo so it can rotate and the servo is fixed to the PCB.

### How it does during the matches

We wanted a fast robot, but now is too fast in some zones of the mat and we're working on reducing its minimum velocity to have a better control in some especific tasks. Our idea is to make a first lap with low velocity to read all the aleatory components of the round and, in the next laps, make acceleration and deceleration sections knowing our path to be faster.

Happy cloning!

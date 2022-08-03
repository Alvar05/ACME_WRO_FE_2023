////////////////////////////////////////////////01-08-22////////////////////////////////////////////////


Engineering materials
====

This repository contains engineering materials of a self-driven vehicle's model participating in the WRO Future Engineers competition in the season 2022 of the team ACME.

## Content

* `t-photos` contains 2 photos of the team (an official one and one funny photo with all team members)
* `v-photos` contains 6 photos of the vehicle (from every side, from top and bottom)
* `video` contains the video.md file with the link to a video where driving demonstration exists
* `schemes` contains one or several schematic diagrams in form of JPEG, PNG or PDF of the electromechanical components illustrating all the elements (electronic components and motors) used in the vehicle and how they connect to each other.
* `src` contains code of control software for all components which were programmed to participate in the competition
* `models` is for the files for models used by 3D printers, laser cutting machines and CNC machines to produce the vehicle elements. If there is nothing to add to this location, the directory can be removed.
* `other` is for other files which can be used to understand how to prepare the vehicle for the competition. It may include documentation how to connect to a SBC/SBM and upload files there, datasets, hardware specifications, communication protocols descriptions etc. If there is nothing to add to this location, the directory can be removed.

## Introduction

_This part must be filled by participants with the technical clarifications about the code: which modules the code consists of, how they are related to the electromechanical components of the vehicle, and what is the process to build/compile/upload the code to the vehicle’s controllers._

### Recieving data from the robot to ease debugging
 
To recieve data from the new robot, we designed an APP with MIT App Inventor (https://appinventor.mit.edu/) to have a better management and distribution of the info. Also we can interact with it and make changes in some variables without having to upload a new sketch (to save time). This APP works with bluetooth (previusly disabled before the competition) and connects with the bluetooth module of the M5Stack. 
 
The elements we have in the APP are some variables to change like speedness or constants for the PID functions, a console to send data to the robot, a Play/Stop button and a grafic section which shows a 2D version of the field with a car that estimates and emulates the moves of the real robot in the screen and draw lines that show the path the robot made. 
 
The method used to emulate the robot moves is odometry based on the two encoders that are placed at both of the rear tires, which can rotate freely and don’t steer (this allows us to use a diferential-based odometry method).
 
By the code part, we use the BluetoothSerial.h library to control the bluetooth data of the M5Stack. To update the data from the robot, the APP sends a petition to the robot which, when recieved, returns a string between a first “<” character and a final “>” character to make it easier for the APP to detect the correct data and avoid other noise. The data recieved outside of the “<String>”, is acumulated and when a \n is recieved, all the acumulated data is printed in the screen console (this allows us to send some flag words to the APP console using the println() method and have more control on whats happening in the running program).
 
### Choosing sensors
 
We thought of introducing a sensor that would allow us to read distances somewhat more accurately than an ultrasonic sensor and over a wider range than a conventional infrared sensor.
 
We chose the OPT3101 sensor from Pololu, with components from Texas Instruments. This is a Time Of Flight (TOF) sensor, which measures distances of up to 1 metre by calculating the time it takes for a previously emitted infrared light beam to bounce back to the sensor.
 
It has six emitters and two receivers that allow the data to be divided into three channels: left, centre and right, with a range of fifty, sixty and fifty degrees respectively.
The operation of this type of sensor avoids many problems as it does not depend on the intensity of the light received, as would be the case with a conventional infrared sensor.
Sensor website:
https://www.pololu.com/product/3680
 
We tried to communicate this sensor via I2C with our main processor, serving as a slave of the Arduino, along with other sensors such as the MPU6050 gyroscope.
Because of this, we had to ensure that the libraries of each sensor were compatible with each other, as well as programming the reception of data with interrupts to prevent data from different sensors from interfering with each other.

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
* `YouTube Channel` All public videos are uploaded at https://www.youtube.com/channel/UCMpIuC9MiNL4U_FDTKrd1CQ
 
## Introduction
 
Since our participation last year, we have been improving our vehicle to make it look smaller and more compact, but, to understand it, we should take a look at the basics and the functioning of the old robot.
We created two arduino controlled boards and communicated them by I2c, this way we could use a larger number of sensors at a time.
Next on, a deeper explanation of its functioning.
 
### Sensor board

This board has been equipped with connectors for three HC-Sr04 ultrasonic sensors, for a MPU 6050 gyroscope sensor and for two encoders; all that controlled by an arduino pro micro that receives the data and sends it via I2c to the main controller in the motor board. It has some extra connectors to link possible future sensors. Its dimensions are 54mm x 71mm  It’s fitted into a PLA-made case in which most of the sensors are kept. The box has from one to three holes in each side (we have different designs) designed to fit in the HC-Sr04 and a motor, as well as an extra hole to make the arduino cable able to reach the pro micro connector. It has been designed to be compatible with Lego, so it could be used as a functional piece. Its dimensions are 87.80mm x 32.80mm x 71.80mm.

### Motor board and main controller

This other board is also fitted into a PLA-made Lego-compatible case, of the same dimensions as the sensors one. The motor board has control over the wheels, the speed at which the vehicle moves and the direction it takes, also it has the purpose of dealing with the data that receives by I2c from the other board.

### Photoreceptor encoders

To measure the distance the robot is driving, we use two TCST 2103 photoreceptors with light diode. Those components are welded to a little board which only needs Vcc (5V) and GND and has a pin that receives the pulses from the photoreceptor. We made a cage for this board to be able to attach a gear which, when rotating, will be detected by the sensor and make the board react, by emitting pulses every few grades of rotation so then, with a mathematical algorithm, the robot can calculate the distance it's moving. This cage is also compatible with Lego Mindstorms to be able to attach it to the rest of the body of the robot.

### Ultrasonic sensors

We made a cage compatible with Lego Mindstorms for the HC-04 ultrasonic sensors to also attach them easily to the main body of the robot. We use the ultrasonic sensors to read how far the borders are (with the lateral sensors) and to know how far things ahead are (with the frontal sensor).

### Chassis design

To carry out this project we have created a chassis from Lego Mindstorm Robot Inventor pieces and 3D prints designed by ourselves. The first challenge was to design a steering system, since we needed a system as precise as possible, within the capabilities of Lego. To resolve that we made an Ackerman steering system, which allowed us to make tight turns without the wheels slipping. Linked to this we have some 3D prints, designed to be able to join with Lego pieces. These serve to support and unite all the sensors and plates of the robot, in addition to work as the base of the chassis. Finally, at the front, we have two fixed wheels with an encoder sensor on each, aided by 3D printing to read the wheel turns.

### Camera module

To achieve the goal of detecting the signs and their colour we used the Pixy2 camera module connected to an Arduino Nano. The Pixy camera has its own processor which outputs the detected object information (x and y position, height and width, colour…) to the arduino. Then the arduino can use the height value to do a simple rule of three, with a pre-calibrated value, and calculate the distance to the object; or it can use the x value to estimate the direction of the object. It also reads the colour value and depending on the sign colour makes the robot turn right (if it’s red) or left (if it’s green). The camera case was designed by ourselves and 3D printed in PLA plastic and holds the camera with some pressure between the main case and the top. The case has seven holes compatible with the Lego Technic connectors to make mounting the camera with the Lego parts possible.


## Updates (from the old model to the new one)
/ This part must be filled by participants with the technical clarifications about the /       / code: which modules the code consists of, how they are related to the 	          /       / electromechanical components of the vehicle, and what is the process to           /        / build/compile/upload the code to the vehicle’s controllers.			          /

This year we are improving the robot, our main goals are to make it smaller and faster, so we are building it all over again, over a different base. This time, instead of making our own PLA structure, we used a RC mini-Z toy and started building the new vehicle over its base structure.

### Problems with the processor

At first, we tried to implement the same board distribution on the new robot; two boards, with one arduino nano each; but, soon we realised that, due to the change of structure and the new sensors used (ex; OPT 3101), the two board distribution became inappropriate.
The arduino memory wasn’t large enough and fitting two boards into such a small structure was almost impossible. Our solution was to implement the M5Stack processor.

### New tires

We tried to create our own rubber tires for the new vehicle using PLA-made molds. The first few tires turned out to be not good enough so we bought and started using Pololu tires. We will be making more tires hoping that we can substitute the Pololu ones the moment we get ours to be good enough.

### Receiving data from the robot to ease debugging
 
To receive data from the new robot, we designed an APP with MIT App Inventor (https://appinventor.mit.edu/) to have better management and distribution of the info. We also can interact with it and make changes in some variables without having to upload a new sketch (to save time). This APP works with bluetooth (previously disabled before the competition) and connects with the bluetooth module of the M5Stack. 
 
The elements we have in the APP are some variables to change like speedness or constants for the PID functions, a console to send data to the robot, a Play/Stop button and a graphic section which shows a 2D version of the field with a car that estimates and emulates the moves of the real robot in the screen and draw lines that show the path the robot made. 
 
The method used to emulate the robot moves is odometry based on the two encoders that are placed at both of the rear tires, which can rotate freely and don’t steer (this allows us to use a differential-based odometry method). Later on, we found out that our encoders weren't precise enough to have a good actualization of the position, causing lots of error between the estimation and the real position. 
 
By the code part, we use the BluetoothSerial.h library to control the bluetooth data of the M5Stack. To update the data from the robot, the APP sends a petition to the robot which, when recieved, returns a string between a first “<” character and a final “>” character to make it easier for the APP to detect the correct data and avoid other noise. The data received outside of the “<string>”, is accumulated and when a \n is received, all the accumulated data is printed in the screen console (this allows us to send some flag words to the APP console using the println() method and have more control on what is happening in the running program).
 
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
Each wheel is connected to a cylindrical magnet which, when moving, is detected by the sensor, giving a signal every few degrees. This way we can get an idea of the distance travelled by the robot. 
The third sensor is supposed to read the rotation degrees of the motor, so we can estimate the actual traction of the wheels. By now we are not using this sensor.


### Problems whith the OPT3031
The Tof sensor that we tried to use ended up generating more problems that the ones it solved.
The use of this sensor at the same time of the MPU6050 gyroscope caused trouble, it stopped working at some point whithout any apearent reason and the data recieved was quite irregular.
We thought on implementing a different Tof sensor, being the VL53LX our main option but, by now, we will be using the HC-sr04 ultrasonic sensor, that seems to allways work precisely enough. 
Despite of that, the ultrasonic has generated some problems until now, but those seem to be easier to deal whith. That is why we won't change it yet.
The main ploblem seems to be caused by the vibration of the wheels when spining. This vibration spreds though the robot and to the sensor, making it send false data.


### The HC-sr04 sensors' issues
As result of using again the ultrasonic sensors some old problems showed up again. Due to the fact that the HC-sr04 has a much narrower range than the OPT3031 we have seen ourselves pushed into using three HC to cover the two sides of the robot aswell as the front. The problem abot that is that we don't have enough pins in the M5Stack to connect that much sensors.
We are using again two boards, one controlled by the M5 and the other by an Arduino, this way we can get the pins we need.


### Reflected light sensors
Our original plan involed detecting the colour lines on the game field, so we can get information to locate the robot during the run. To achieve that, we are choosing now what sensor to use. Our options are the cny70, the tcrt5000 and the tcs34725. By now we have tested the two frirst mentioned sensors and the only thing that seems clear is that reading an orange line over a white background is a chalenge itself.

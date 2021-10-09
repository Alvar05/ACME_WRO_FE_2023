### Sensor board

This board has been equipped with connectors for three HC04 ultrasonic sensors, for a MPU 6050 gyroscope sensor and for two encoders; all that controlled by an arduino pro micro that receives the data and sends it via I2C to the main controller in the motor board. It has some extra connectors to link possible future sensors. Its dimensions are 54mm x 71mm  It’s fitted into a PLA-made box in which most of the sensors are kept. The box has from one to three holes in each side designed to fit in the HC04 and a motor as well as an extra hole to make the arduino cable able to reach the pro micro connector. It has been designed to be compatible with Lego, so it could be used as a functional piece. Its dimensions are 87.80mm x 32.80mm x 71.80mm.

### Motor board and main controller

This board...

### Photoreceptor encoders

To measure the distance the robot is driving, we use two TCST 2103 photoreceptors with light diode. Those components are welded to a little board wich only needs Vcc (5V) and GND and has a pin that recieves the pulses from the photoreceptor. We made a cage for this board to be able to attach a gear wich, when rotating, will be detected by the sensor and make the board react emitting pulses every few grades of rotation so then, with a mathemathic algorithm, the robot can calculate the distance it's moving. This cage is also compatible with Lego Mindstorms to be able to attach it to the rest of the body of the robot.

### Ultrasonic sensors

We made a cage compatible with Lego Mindstorms for the HC-04 ultrasonic sensors to also attach them easily to the main body of the robot. We use the ultrasonic sensors to read how far the borders are (with the lateral sensors) and to know how far things ahead are (with the frontal sensor).

### Chasis design

To carry out this project we have created a chassis from Lego Mindstorm Robot Inventor pieces and 3D prints designed by ourselves. The first challenge was to design a steering system, since we needed a system as precise as possible, within the capabilities of Lego. To resolve that we made an Ackerman steering system, which allows us to make tight turns without the wheels slipping. Linked to this we have some 3D prints, designed to be able to join with Lego pieces. These serve to support and unite all the sensors and plates of the robot, in addition to work as the base of the chassis. Finally, at the front, we have two fixed wheels with an encoder sensor on each, aided by 3D printing to read the wheel turns.

### Camera module

To achieve the goal of detecting the signs and their color we used the Pixy2 camera module connected to an Arduino Nano. The Pixy camera has its own processor which outputs the detected object information (x and y position, height and width, color…) to the arduino. Then the arduino can use the height value to do a simple rule of three, with a pre-calibrated value, and calculate the distance to the object; or it can use the x value to estimate the direction of the object. It also reads the color value and depending on the sign color makes the robot turn right (if it’s red) or left (if it’s green). The camera case was designed by ourselves and 3D printed in PLA plastic and holds the camera with some pressure between the main case and the top. The case has seven holes compatible with the Lego Technic connectors to make possible mounting the camera with the Lego parts.

# Servo Finger
The sketch emulates time-based finger press with use of a servo.
It uses a potentiometer to adjust the time interval between presses
The Arduino Pro mini is used and to minimize connections the pot is connected to VCC, A3 and A2 (all the pins are close to each other), so A2 serves as GND after pin config
Servo is connected to SERVO_PIN which is recommended to be D0, beause the servo is pluged to programming (UART) port to have GND, VCC and PWM pin on it 

## How it looks like:
![ServoFinger](http://i.piccy.info/i9/675f4fa836f4f133fabcec57710e4b15/1586668419/126758/1372417/IMG_20200407_212421.jpg | width=200)

## What do you need:
- Arduino Pro Mini
- 10k-100k potentiometer
- Any servo you want

## How to use:
- Compile and flash the sketch
- Use pot to adjust the 'click' time
- Connect power
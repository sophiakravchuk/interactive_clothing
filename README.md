# Interactive Clothing Project (Device for people who suffers from Amblyopia)

## Description
A simple clothing that helps people who suffers from Amblyopia(a disorder of sight in which the brain fails to process inputs from one eye, and over time favors the other eye.) to walk freely and to have a better coordination in space. You can attach our device,supplemented with a frame of intelligent control LED tapes(WS2812B),  to your feet and leg and by tracking a colour of LED tape, navigate through the space. LED tape changes colour of lights and the pattern depending on certain metrics taken by the sensor(range-finder HC-SR04).
Our device has two different modes.

### Mode 1.
LED tape lights in different colours depending on distance of closest item.
If some item is closer than 20 cm to person - tape has red light, 20 - 50 cm: orange, 50 - 100 cm: yellow, and more: green.

#### Example:
![mode 1](https://github.com/juliapochynok/interactive_clothing/blob/master/ezgif.com-gif-maker%20(1).gif?raw=true)


link - https://drive.google.com/open?id=1AEwmK_NFDQ_S34hLclMP7Uz-a3Hduuyx

### Mode 2.
LED tape lights certain amount of LEDs depending on distance of closest item.
If closest item is 5 cm away from person's feet - 5 LEDs does NOT glow, 10 cm: 10 LEDs does not glow.

#### Example:
![mode 2](https://github.com/juliapochynok/interactive_clothing/blob/master/cm_state.gif?raw=true)


link - https://drive.google.com/open?id=1o1Cnzm2foz8ZPk9AcE9jDxdfRMFJAjPs
## How it works:
### Getting data to process

We are using the rangefinder as one of the sensors that produces the information.

### Displaying patterns
Depending on data we get our interactive clothing displays different patterns with different combinations of colours. We have described how it worked above(Modes 1/2).

## Materials that we use

### STM32F4Discovery microcontroller
![](https://www.waveshare.com/img/devkit/STM32F4DISCOVERY/STM32F4DISCOVERY-5.jpg)

### WS2812B Intelligent control LED tapes
![](https://nettigo.eu/system/images/2203/original.JPG?1495546386)

### Sensor - ( temporarily range-finder HC-SR04)
![](https://uawest.com/image/cache/product_images/original_images/1829_0-700x700.jpg)

## Our progress
### Done:
- got acquainted with the principles of diodes operation
- used Motion Detector to illuminate the diode depending on the presence of motion
- worked with one Intelligent control LED (WS2812B)
- got acquainted with the features of the color transfer of the diode
- we worked with a rangefinder to illuminate the diode(not WS2812B) depending on the distance at which the object is located.
- we learnt how to light up a certain number of LED on the tape 
- we learnt how to  to set a specific color to the Intelligent control LED tape
- we learnt how to change LED tape lighting colour depending on distance
- in accordance with the results of done work develop the mechanism of operation of the LED strip
- made two prototypes
- tested prototypes on people
- make all components of our project "more comfortable" for users

### Should be done:
Everything is done! :=)


## Documentation

## Team
Sophia Kravchuk, Julia Pochynok, Sophia Haletska

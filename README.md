# ArduinoTec-Pedals
This project uses a Arduino Leonardo, Pro Micro or Teensy to replace the controller board of the Fanatec CSPV1. 
This is based on the https://github.com/MHeironimus/ArduinoJoystickLibrary library created by Matthew Heironimus
See the Circuit diagram for the wiring layout.

In addition to this you would need:

1. Arduino Leonardo (https://store.arduino.cc/usa/leonardo) or Pro Micro 5v 16mhz (https://www.sparkfun.com/products/12640)
2. a small breadboard or Arduino Shield (available on ebay and similar to the one shown at https://www.adafruit.com/product/51)to place it on if you don't want to solder the cables onto the board directly
3. Connectors if you want to be able to connect the load cells and HE sensors via a connector block
4. 1 load cell amp( existing fanatec one can be reused)

Connect the items as per the diagram, write the code to the Arduino, plug into the USB, calibrate via driver calibration screen, and you are ready to race.


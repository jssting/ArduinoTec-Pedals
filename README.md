# ArduinoTec-Pedals
This project uses a Arduino Leonardo, Pro Micro or Teensy to replace the controller board of the Fanatec CSPV1. 
This is based on the https://github.com/MHeironimus/ArduinoJoystickLibrary library created by Matthew Heironimus
See the Circuit diagram for the wiring layout.

In addition to this you would need:

1. Arduino Leonardo (https://store.arduino.cc/usa/leonardo) or Pro Micro 5v 16mhz (https://www.sparkfun.com/products/12640)
2. a small breadboard or Arduino Shield (available on ebay and similar to the one shown at https://www.adafruit.com/product/51)to place it on if you don't want to solder the cables onto the board directly
3. Connectors if you want to be able to connect the load cells and HE sensors via a connector block
4. 1 load cell amp( existing fanatec one can be reused). I'used the Leo Bodnar LC Amplifier (http://www.leobodnar.com/shop/index.php?main_page=product_info&cPath=73&products_id=199). You can also re-use the existing one from the Fanatec Pedals(recommended), or use a hx711. The original LC amp is already correctly set, but the Leobodnar and HX711 will need to be configured in a half bridge mode, I added a 1k Ohm resistor across the input and GND(not 5V).
5. For the Vibration Motor I used a 2N3638 PNP Transistor rated to 25v. 
6. I re-used the Fanatec Hall Effect sensors as mine were still in good working order. If you do want to use another sensor, ensure it is a linear sensor with Analog output that can sense the distance of the magnet/and field variation. Do not use arduino ones as they one sense on/off and not field variation. 
Connect the items as per the diagram, write the code to the Arduino, plug into the USB, calibrate via driver calibration screen, and you are ready to race.


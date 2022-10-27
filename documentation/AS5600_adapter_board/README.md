AS5600
------
This implementation uses an [AS5600](https://ams.com/en/AS5600) adaptor [board](https://ams.com/as5600adapterboard) to replace the Hall Effect sensor on the Fanatec CSPV1.

The AS5600 chip has analog, PWM and I2C outputs.  ArduinoTec-Pedals supports the analog interface.

The AS5600 provides 12bit resolution over the entirety of its 360 degree range.  Alternately, it can be programmed to provide the full resolution over a smaller angle of rotation.  See the [datasheet](https://ams.com/documents/20143/36005/AS5600_DS000365_5-00.pdf) for programming and other details.

Bracket
-------
A 3D printed bracket is used to mount the adaptor board to the ClubSport V1 pedal:

- [pdf file](he_bracket.pdf)
- [FreeCad file](he_bracket.FCStd)
- [STL file](he_bracket.stl)

Here is a [picture](AS5600_mounted.jpg) of the board and adapter mounted

Notes
-----
- The screws to attach the adapter to the pedal make contact with the adapter board.  A dremel or file can be used to remove some of the PCB material at those two points.
- The adapter board comes with straight headers.  These may need to be replaced with right angle headers to fit in available space between the throttle pedal and the frame.
- The picture shows the I2C interface being used.  The analog interface had too much noise for me.  Using the I2C interface required modifications to ArduinoTec-Pedals.

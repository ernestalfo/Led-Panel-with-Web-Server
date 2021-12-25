# Led-Panel-with-Web-Server
NodeMCU + ArduinoNano. 8 rows and N columns led display using SPI with 74HC594/74HC595 IC. 

This is a two-year-old project that combines an Arduino Nano and a NodeMCU communicating via serial to achieve 8 rows and N columns (the number of columns will depend 
on how many shift register has) led display. You can access a web interface to configure and send the messages to the display, then You can choose between 4 different letters, 
some effects, show the clock with the current time and other functions.

The led panel is designed to be built with common leds, the 8 rows are driven with 8 bjt pnp type transitors and the columns are driven with the 
IC ULN28003 (8 npn transitors inside). There will be as many resistors as there are columns on the display and the resistance value is calculated to operate 
the LEDs above the nominal current (approximately 20% above) to obtain a brighter and more legible display. 

For the web server I relied on John Lassen's code which can be found at https://www.john-lassen.de/en/projects/esp-8266-arduino-ide-webconfig. This was my starting point but 
I modified it conveniently to my needs.

If you want you can see some photos of my prototype in this link: https://www.linkedin.com/posts/ernestoalfop_diy-maker-iot-activity-6594048878104891392-Walo

My father-in-law was using my prototype as commercial advertising in his business until a thunderstorm killed the ardunino nano. 
If you think it's worth building your own prototype and you think I can help with anything, you can contact me at ernestoalfcorreo@gmail.com.

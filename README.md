arduQuakes
==========

----
Sisifo software for adxl345.
https://sites.google.com/site/ogssisifo/
http://www.cloc3.net/dokuwiki/doku.php/progettoSisifo

---

HOWTO use this software.

1.	Look at connectionSchema.zip archive to set physical connections between arduino and adxl345 sensors;
2.	connect arduino to your pc via usb cable;
3.	upload sketches in arduSketches folder to your arduino1 board (with libraries in ./libraries folder);
4.	look at html files in web folder and move them in your local webserver;
5.	run the script ncWebService.sh in web folder;
6.	start your browers:
		konqueror http://localhost

enjoy.

if you need simply an output on command line you may replace the step 4 in this way:

4.	exec 5 <>/dev/arduino #use here the real device created in your system.
5.	head -n 10 <&5 & echo n=10 >&5

p.s.: current version of arduino sketches is improved to work with arduino ethshield:
	http://arduino.cc/en/Main/ArduinoEthernetShield

for testing:
1.	add your shield to the main arduino board and restore adxl345 connections.
2.	connect arduino shield to your pc ethport.
3.	configure arduino address in a working way for your system.
4.	try this command:
		GET http://arduino:8000/?n=6


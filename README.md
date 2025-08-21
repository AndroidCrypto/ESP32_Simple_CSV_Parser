# ESP32 Simple CSV Parser
The simple CSV Parser allows to extract data from a CSV file obtained from the Weather Service Provider **[Meteomatics.com](https://www.meteomatics.com/)**.

It is a supplementary project and article for other articles regarding [Meteomatics.com](https://www.meteomatics.com/) to keep them shorter. The final code of this parser is used in my projects "" and "" (links comin soon).

This is the accompanying repository for my article "**How to parse a Meteomatics.com Weather Forecast CSV file with an ESP32**" available here:

## Runtime Environment
````plaintext
Arduino 2.3.6 (Windows)
esp32 boards version 3.2.0
For compiling and uploading, I'm using the "ESP32 Dev Module" board, but all other microcontrollers of the ESP32 familiy should work as well.
````

This is the Serial Monitor printout of the sample data:
````plaintext
ESP32 Simple CSV Parser V03
-----------------------------------
httpResponseString (raw data):
validdate;t_2m:C;precip_1h:mm
2025-08-16T21:45:00Z;16.7;0.00
2025-08-16T22:45:00Z;15.5;3.50

-----------------------------------
Content of the data in ASCII and HEX encoding
payloadLength: 92
-----------------------------------
ASCII encoding
v a l i d d a t e ; t _ 2 m : C 
; p r e c i p _ 1 h : m m 
 2 0 
2 5 - 0 8 - 1 6 T 2 1 : 4 5 : 0 
0 Z ; 1 6 . 7 ; 0 . 0 0 
 2 0 2 
5 - 0 8 - 1 6 T 2 2 : 4 5 : 0 0 
Z ; 1 5 . 5 ; 3 . 5 0 
 
-----------------------------------
HEX encoding
76 61 6C 69 64 64 61 74 65 3B 74 5F 32 6D 3A 43 
3B 70 72 65 63 69 70 5F 31 68 3A 6D 6D A 32 30 
32 35 2D 30 38 2D 31 36 54 32 31 3A 34 35 3A 30 
30 5A 3B 31 36 2E 37 3B 30 2E 30 30 A 32 30 32 
35 2D 30 38 2D 31 36 54 32 32 3A 34 35 3A 30 30 
5A 3B 31 35 2E 35 3B 33 2E 35 30 A 
-----------------------------------
Find FIELD_DELIMITER and LINE_DELIMITER
FIELD_DELIMITER found at pos 9
FIELD_DELIMITER found at pos 16
LINE_DELIMITER found at pos 29
FIELD_DELIMITER found at pos 50
FIELD_DELIMITER found at pos 55
LINE_DELIMITER found at pos 60
FIELD_DELIMITER found at pos 81
FIELD_DELIMITER found at pos 86
LINE_DELIMITER found at pos 91
-----------------------------------
Get CSV Header data
headerField1: validdate
headerField2: t_2m:C
headerField3: precip_1h:mm
-----------------------------------
Parse the complete file
-----------------------------------
Parsing result
-= Header section data =-
headerField1: validdate
headerField2: t_2m:C
headerField3: precip_1h:mm
-----------------------------------
-= Data section data =-
Number of forecasts: 2
001: 2025-08-16T21:45:00Z 16.70 C 0.00 mm
002: 2025-08-16T22:45:00Z 15.50 C 3.50 mm
-----------------------------------

````

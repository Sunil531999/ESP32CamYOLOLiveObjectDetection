# ESP32CamYOLOLiveObjectDetection
This is a proof of concept for live object detection using YOLO and ESP32 Cam.

Parts Required:
---------------
1. ESP32 Cam

Files and Software Required:
----------------------------
1. YOLOv3 Weight [Click to Download](https://pjreddie.com/media/files/yolov3.weights)

Required edits in code:
---------------
1. Change WiFi credentials in the code it is on line number 9 & 10.
<img src="https://github.com/Sunil531999/ESP32CamYOLOLiveObjectDetection/blob/main/images/wifi%20credentials.png" width="500" height="65">
2. IP Address will be printed on the Serial Monitor
<img src="https://github.com/Sunil531999/ESP32CamYOLOLiveObjectDetection/blob/main/images/serial%20monitor.png" width="260" height="150">
3. Take that IP Address and replace it with the IP Address in the main.py python file on line number 22.
<img src="https://github.com/Sunil531999/ESP32CamYOLOLiveObjectDetection/blob/main/images/ip%20address%20python%20file.png" width="500" height="52">
4. If you want to change the resolution and picture type than you need to change the below configuration in ObjectDection.cpp
<img src="https://github.com/Sunil531999/ESP32CamYOLOLiveObjectDetection/blob/main/images/frame%20quality%20and%20size.png" width="500" height="52">

Procedure:
----------
The whole project is build in PlatformIO in VSCode So if you are not comfortable with PlatformIO or don't know about it no problem just change the extention of ObjectDection.cpp to ObjectDection.ino and remove the Arduino.h header file line from the code. Now you can open it in Arduino IDE.
First download YOLOv3 weight and put it in the src folder. Now just upload the code to ESP32Cam.
After the upload go to the serial monitor and you will see the IP Address that is assigned.
Now you are good to go, run the python file and it will start fetching the images from the ip address on which the ESP32Cam is serving the image. Now point the camera to some object and it will start its object detection.

Where can it be used ?:
----------------------
It can used it projects like self driving cars, robots which can itself identify obstacles and overcome that obstacle.

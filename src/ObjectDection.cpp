#include "Arduino.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#define CAMERA_MODEL_AI_THINKER
#include "esp_camera.h"
#include "camera_pins.h"

// Replace with your network credentials
const char* ssid = "Your_SSID"; //Your WiFi Name
const char* password = "Your_PASSWORD"; //Your WiFi Password

// Create an instance of the server
AsyncWebServer server(80);

void WiFi_init() {
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi Network");
  // Wait until connection is established
  while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(100);
  }
  // Print ESP32's IP & HostName
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}
void Camera_init(){
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_SVGA; // frame size 
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 40; //chanege the quality according to your needs
  config.fb_count = 1;

  // Initialize the camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}
void setup() {
  Serial.begin(115200);
  Camera_init();
  WiFi_init();
  Serial.println();
  Serial.println("Use this address in your python code");
  Serial.print("http://");
  Serial.print(WiFi.localIP());Serial.println("/");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) { //in place of / you can also add an adress like /picture or /capture
  
      camera_fb_t * frame = NULL;
      frame = esp_camera_fb_get();
  
      request->send_P(200, "image/jpeg", (const uint8_t *)frame->buf, frame->len);
  
      esp_camera_fb_return(frame);
    });

  server.begin();
}

void loop() {}

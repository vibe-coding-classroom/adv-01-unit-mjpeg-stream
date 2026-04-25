#include "esp_camera.h"
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

WiFiServer server(80);

void startCamera() {
    // Camera configuration (Pins for ESP32-CAM)
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = 5;
    config.pin_d1 = 18;
    config.pin_d2 = 19;
    config.pin_d3 = 21;
    config.pin_d4 = 36;
    config.pin_d5 = 39;
    config.pin_d6 = 34;
    config.pin_d7 = 35;
    config.pin_xclk = 0;
    config.pin_pclk = 22;
    config.pin_vsync = 25;
    config.pin_href = 23;
    config.pin_sscb_sda = 26;
    config.pin_sscb_scl = 27;
    config.pin_pwdn = 32;
    config.pin_reset = -1;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    
    if(psramFound()){
        config.frame_size = FRAMESIZE_VGA;
        config.jpeg_quality = 10;
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }
}

void handle_mjpeg_stream(WiFiClient client) {
    client.printf("HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", _STREAM_CONTENT_TYPE);

    while (client.connected()) {
        camera_fb_t * fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Camera capture failed");
            break;
        }

        // TASK 2: Manual implementation of MJPEG part headers
        // TODO: Implement the byte-level sending logic here as per README Task 2
        /*
        client.print(_STREAM_BOUNDARY);
        client.printf(_STREAM_PART, fb->len);
        client.write(fb->buf, fb->len);
        client.print("\r\n");
        */

        esp_camera_fb_return(fb);
        delay(10); // Adjust for frame rate
    }
}

void setup() {
    Serial.begin(115200);
    startCamera();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.println(WiFi.localIP());

    server.begin();
}

void loop() {
    WiFiClient client = server.available();
    if (client) {
        String line = client.readStringUntil('\r');
        if (line.indexOf("GET /stream") >= 0) {
            handle_mjpeg_stream(client);
        }
        client.stop();
    }
}

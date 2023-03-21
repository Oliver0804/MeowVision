/*  This example uses the camera to capture a JPEG image,
    and saves the image to SD Card.
*/
#include "VideoStream.h"
#include "AmebaFatFS.h"

#define CHANNEL 0
#define FILENAME "image.jpg"

VideoSetting config(VIDEO_FHD, CAM_FPS, VIDEO_JPEG, 1);

VideoSetting configMD(VIDEO_VGA, 10, VIDEO_RGB, 0);     // Low resolution RGB video for motion detection
StreamIO videoStreamerMD(1, 1);

uint32_t img_addr = 0;
uint32_t img_len = 0;

AmebaFatFS fs;
void camera_init(){
  Camera.configVideoChannel(CHANNEL, config);
  Camera.videoInit();
  Camera.channelBegin(CHANNEL);
}

void get_pic2sd(){
  fs.begin();
  File file = fs.open(String(fs.getRootPath()) + String(FILENAME));
  delay(100);
  Camera.getImage(CHANNEL, &img_addr, &img_len);
  file.write((uint8_t*)img_addr, img_len);
  file.close();
  fs.end();
}
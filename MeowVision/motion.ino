
#include "WiFi.h"
#include "VideoStream.h"
#include "StreamIO.h"
#include "RTSP.h"
#include "MotionDetection.h"
#include "VideoStreamOverlay.h"


#define CHANNEL 0       // High resolution video channel for streaming
#define CHANNELMD 3     // RGB format video for motion detection only avaliable on channel 3
#define MDRES 16        // Motion detection grid resolution

MotionDetection MD(MDRES, MDRES);

void mdPostProcess(char* md_result) {
    // Motion detection results is expressed as an MDRES x MDRES array
    // With 0 or 1 in each element indicating presence of motion
    // Iterate through all elements to check for motion
    // and calculate largest rectangle containing motion
    int motion = 0, j, k;
    int jmin = MDRES - 1, jmax = 0;
    int kmin = MDRES - 1, kmax = 0;
    for (j = 0; j < MDRES; j++) {
        for (k = 0; k < MDRES; k++) {
            printf("%d ", md_result[j * MDRES + k]);
            if (md_result[j * MDRES + k]) {
                motion = 1;
                if (j < jmin) {
                    jmin = j;
                }
                if (k < kmin) {
                    kmin = k;
                }
                if (j > jmax) {
                    jmax = j;
                }
                if (k > kmax) {
                    kmax = k;
                }
            }
        }
        printf("\r\n");
    }
    printf("\r\n");

    OSD.clearAll(CHANNEL);
    if (motion) {
        // Scale rectangle dimensions according to high resolution video stream and draw with OSD
        int xmin = (int)(kmin * config.width() / MDRES) + 1;
        int ymin = (int)(jmin * config.height() / MDRES) + 1;
        int xmax = (int)((kmax + 1) * config.width() / MDRES) - 1;
        int ymax = (int)((jmax + 1) * config.height() / MDRES) - 1;
        OSD.drawRect(CHANNEL, xmin, ymin, xmax, ymax, 3, OSD_COLOR_GREEN);
    }
    OSD.update(CHANNEL);
}
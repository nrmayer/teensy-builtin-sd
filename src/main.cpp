#include <Arduino.h>
#include "sd_manager.hpp"


#define LOG_FILE_FORMAT "l%d.log"
#define LOG_FILE_DIR "/logs/"
#define LOG_FILE_FULL LOG_FILE_DIR LOG_FILE_FORMAT

void setup() {
    Serial.begin(9600);
    while (!Serial) {}

    BuiltinSd.start();

    SdFile dir = BuiltinSd.open_file(LOG_FILE_DIR, O_RDONLY);
    if (!dir) {
        Serial.println("log dir open failed");
        return;
    }

    SdFile dir_file;
    int highest_log_num = 1;
    while(dir_file.openNext(&dir, O_RDONLY)) {
        char name_buf[32];
        int num_match;

        dir_file.getName(name_buf, sizeof(name_buf));
        
        Serial.println(name_buf);

        if (!sscanf(name_buf, LOG_FILE_FORMAT, &num_match)) continue;
        highest_log_num = highest_log_num > num_match ? highest_log_num : num_match;

        dir_file.close();
    }
    Serial.print("new dir file at index:");
    Serial.println(highest_log_num+1);

    char log_file_path[32];
    snprintf(log_file_path, sizeof(log_file_path), LOG_FILE_FULL, highest_log_num+1);

    Serial.println(log_file_path);

    SdFile new_log_file = BuiltinSd.open_file(log_file_path, O_WRITE | O_APPEND | O_CREAT);
    new_log_file.write("This is a VERY important log");
    new_log_file.sync();
}

void loop() {

}
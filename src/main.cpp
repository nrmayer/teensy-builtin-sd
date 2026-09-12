#include <Arduino.h>
#include <SdFat.h>

SdFat sd;
SdFile file;
SdFile dir;
SdFile dir_file;

#define LOG_FILE_FORMAT "l%d.log"
#define LOG_FILE_DIR "/logs/"
#define LOG_FILE_FULL LOG_FILE_DIR LOG_FILE_FORMAT

void setup() {
    Serial.begin(9600);
    while (!Serial) {}

    sd.begin(SdioConfig(FIFO_SDIO));

    if (!file.open("test_file.txt", O_RDONLY)) {
        Serial.println("open failed");
        return;
    }

    uint8_t buf[512];
    int read = file.read(buf, sizeof(buf));
    buf[read] = '\0'; // null term

    Serial.println((const char*)buf);

    file.close();

    if (!dir.open(LOG_FILE_DIR, O_RDONLY)) {
        Serial.println("log dir open failed");
        return;
    }
    
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

    SdFile new_log_file;
    char log_file_path[sizeof(LOG_FILE_FULL+2)];
    sprintf(log_file_path, LOG_FILE_FULL, LOG_FILE_FORMAT, highest_log_num+1);

    Serial.println(log_file_path);

    new_log_file.open(log_file_path, O_WRITE | O_APPEND | O_CREAT);
    new_log_file.write("This is a VERY important log");
    new_log_file.sync();
}

void loop() {

}
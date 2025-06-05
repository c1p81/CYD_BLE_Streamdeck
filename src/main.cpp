#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <esp32_smartdisplay.h>
#include <SD_MMC.h>
#include <NimBLEDevice.h>
#include <NimBLEServer.h>
#include <NimBLEUtils.h>
#include <NimBLEHIDDevice.h>
#include <HIDTypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define REPORT_ID 1
#define REPORT_SIZE 8

const char* btn1_label="Speed+";
uint8_t btn1_mod = 0x00;
uint8_t btn1_key = 0x23;

const char* btn2_label="Play/Stop";
uint8_t btn2_mod = 0x00;
uint8_t btn2_key = 0x2C;

char* btn3_label="Speed-";
uint8_t btn3_mod = 0x00;
uint8_t btn3_key = 0x22;

char* btn4_label="Normal";
uint8_t btn4_mod = 0x00;
uint8_t btn4_key = 0x23;

char* btn5_label="-10sec";
uint8_t btn5_mod = 0x00;
uint8_t btn5_key = 0x20;

char* btn6_label="+10sec";
uint8_t btn6_mod = 0x00;
uint8_t btn6_key = 0x21;

char* btn7_label="Vol-";
uint8_t btn7_mod = 0x01;
uint8_t btn7_key = 0x51;

char* btn8_label="Vol+";
uint8_t btn8_mod = 0x01;
uint8_t btn8_key = 0x52;


TFT_eSPI tft = TFT_eSPI();

// HID Report Map for a basic keyboard
static const uint8_t hidReportDescriptor[] = {
  0x05, 0x01, // Usage Page (Generic Desktop Ctrls)
  0x09, 0x06, // Usage (Keyboard)
  0xA1, 0x01, // Collection (Application)
  0x85, REPORT_ID, // Report ID
  0x05, 0x07, // Usage Page (Kbrd/Keypad)
  0x19, 0xE0, // Usage Minimum (224)
  0x29, 0xE7, // Usage Maximum (231)
  0x15, 0x00, // Logical Minimum (0)
  0x25, 0x01, // Logical Maximum (1)
  0x75, 0x01, // Report Size (1)
  0x95, 0x08, // Report Count (8)
  0x81, 0x02, // Input (Data,Var,Abs)
  0x95, 0x01, 0x75, 0x08, 0x81, 0x03, // Reserved byte
  0x95, 0x06, 0x75, 0x08,
  0x15, 0x00, 0x25, 0x65,
  0x05, 0x07, 0x19, 0x00, 0x29, 0x65,
  0x81, 0x00, // Input (keys)
  0xC0        // End Collection
};

NimBLEServer *pServer;
NimBLECharacteristic *inputChar;

bool deviceConnected = false;

// Structure to hold button configuration
struct ButtonConfig {
  char button_id[10];
  char label[10];
  uint8_t modifier;
  uint8_t key;
};

#define MAX_BUTTONS 6
ButtonConfig buttons[MAX_BUTTONS];  // Global array to store parsed button info
int buttonCount = 0;


class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* s) {
    deviceConnected = true;
    Serial.println("Client connected.");
  }
  void onDisconnect(NimBLEServer* s) {
    deviceConnected = false;
    Serial.println("Client disconnected.");
  }
};



void sendKey(uint8_t modifier, uint8_t keyCode) {
  if (!deviceConnected) return;

  uint8_t report[REPORT_SIZE] = { 0 };
  report[0] = modifier;
  report[2] = keyCode; // Set keycode (e.g., 0x04 = 'a')

  inputChar->setValue(report, sizeof(report));
  inputChar->notify();

  delay(10);

  // Release
  memset(report, 0, sizeof(report));
  inputChar->setValue(report, sizeof(report));
  inputChar->notify();
}


// Speed-
void btn1_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (deviceConnected) {
            sendKey(btn1_mod,btn1_key);
        }
    }
}

// Play stop
void btn2_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (deviceConnected) {
            sendKey(btn2_mod,btn2_key);
        }
    }
}

// Speed +
void btn3_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (deviceConnected) {
            sendKey(btn3_mod,btn3_key);
          }
    }
}

// Normal speed
void btn4_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (deviceConnected) {
            sendKey(btn4_mod,btn4_key);
        }
    }
}


// back 10 seconds
void btn5_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (deviceConnected) {
            sendKey(btn5_mod,btn5_key);
          }
    }
}


// forward 10 seconds
void btn6_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (deviceConnected) {
            Serial.println("luca");
            sendKey(btn6_mod,btn6_key);
        }
    }
}

// volume down
void btn7_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (deviceConnected) {
            sendKey(btn7_mod,btn7_key);
        }
    }
}

// volume up
void btn8_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if (deviceConnected) {
            sendKey(btn8_mod,btn8_key);
        }
    }
}

void create_grid_with_buttons(lv_obj_t *parent) {
    // Create a container to hold the grid
    lv_obj_t *grid = lv_obj_create(parent);
    lv_obj_set_size(grid, 240, 320); // Adjust size as needed
    lv_obj_center(grid);             // Center on screen
    lv_obj_set_flex_flow(grid, LV_FLEX_FLOW_ROW_WRAP); // Horizontal wrap layout
    lv_obj_set_style_pad_row(grid, 10, 0);              // Spacing between rows
    lv_obj_set_style_pad_column(grid, 10, 0);           // Spacing between columns
    lv_obj_set_style_pad_all(grid, 10, 0);              // Inner padding

    // Create 6 buttons (2 rows × 3 columns)
    for (int i = 0; i < 8; i++) {
        lv_obj_t *btn = lv_btn_create(grid);
        lv_obj_set_size(btn, 100, 60);  // Adjust button size as needed

        lv_obj_t *label = lv_label_create(btn);
        switch(i){
            case 0:
                lv_label_set_text_fmt(label, btn1_label);
                lv_obj_center(label);
                lv_obj_add_event_cb(btn, btn1_event_handler, LV_EVENT_CLICKED, NULL);
                break;
            case 1:
                lv_label_set_text_fmt(label, btn2_label);
                lv_obj_center(label);
                lv_obj_add_event_cb(btn, btn2_event_handler, LV_EVENT_CLICKED, NULL);
                break;
            case 2:
                lv_label_set_text_fmt(label, btn3_label);
                lv_obj_center(label);
                lv_obj_add_event_cb(btn, btn3_event_handler, LV_EVENT_CLICKED, NULL);
                break;
            case 3:
                lv_label_set_text_fmt(label, btn4_label);
                lv_obj_center(label);
                lv_obj_add_event_cb(btn, btn4_event_handler, LV_EVENT_CLICKED, NULL);
                break;
            case 4:
                lv_label_set_text_fmt(label, btn5_label);
                lv_obj_center(label);
                lv_obj_add_event_cb(btn, btn5_event_handler, LV_EVENT_CLICKED, NULL);
                break;
            case 5:
                lv_label_set_text_fmt(label, btn6_label);
                lv_obj_center(label);
                lv_obj_add_event_cb(btn, btn6_event_handler, LV_EVENT_CLICKED, NULL);
                break;
            case 6:
                lv_label_set_text_fmt(label, btn7_label);
                lv_obj_center(label);
                lv_obj_add_event_cb(btn, btn7_event_handler, LV_EVENT_CLICKED, NULL);
                break;
            case 7:
                lv_label_set_text_fmt(label, btn8_label);
                lv_obj_center(label);
                lv_obj_add_event_cb(btn, btn8_event_handler, LV_EVENT_CLICKED, NULL);
                break;


            } 
        
    }
}

void lv_tick_handler(void *arg) {
  lv_tick_inc(1);
}

void my_disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map) {
    //int32_t x1 = area->x1;
    //int32_t y1 = area->y1;
    //int32_t x2 = area->x2;
    //int32_t y2 = area->y2;

    //int32_t w = x2 - x1 + 1;
    //int32_t h = y2 - y1 + 1;

    // Replace this with your actual display driver's draw function
    //my_display_draw_bitmap(x1, y1, w, h, (lv_color_t *)px_map);
    //drawBitmap(area->x1, area->y1, width, height, (lv_color_t*)px_map);


    // Tell LVGL we're done flushing
    lv_display_flush_ready(disp);   
}


void setup() {
  Serial.begin(115200);
  delay(1000);

  NimBLEDevice::init("ESP32 NimBLE Keyboard");
  pServer = NimBLEDevice::createServer();

  static ServerCallbacks serverCallbacks;
  pServer->setCallbacks(&serverCallbacks);
 
  NimBLEService *hidService = pServer->createService("1812"); // HID service UUID

  NimBLECharacteristic *hidInfo = hidService->createCharacteristic("2A4A", NIMBLE_PROPERTY::READ);
  uint8_t hidInfoValue[4] = { 0x11, 0x01, 0x00, 0x02 }; // HID info: ver 1.11, not normally connectable, USB HID
  hidInfo->setValue(hidInfoValue, sizeof(hidInfoValue));

  NimBLECharacteristic *reportMap = hidService->createCharacteristic("2A4B", NIMBLE_PROPERTY::READ);
  reportMap->setValue(hidReportDescriptor, sizeof(hidReportDescriptor));

  NimBLECharacteristic *hidControlPoint = hidService->createCharacteristic("2A4C", NIMBLE_PROPERTY::WRITE_NR);
  hidControlPoint->setValue((uint8_t *)"\x00", 1);

  inputChar = hidService->createCharacteristic("2A4D", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  inputChar->createDescriptor("2902", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);

  NimBLEDescriptor *reportRef = inputChar->createDescriptor("2908", NIMBLE_PROPERTY::READ, 2);
  uint8_t refVal[2] = { REPORT_ID, 0x01 }; // Report ID, Input Report
  reportRef->setValue(refVal, sizeof(refVal));

  NimBLECharacteristic *protoMode = hidService->createCharacteristic("2A4E", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE_NR);
  protoMode->setValue((uint8_t *)"\x01", 1); // Report Protocol mode

  hidService->start();

  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID("1812");
  pAdvertising->setAppearance(961); // Keyboard
  
  pAdvertising->start();

  digitalWrite(22, HIGH); // Touch controller chip select (if used)
  digitalWrite(15, HIGH); // TFT screen chip select
  digitalWrite( 5, HIGH); // SD card chips select, must use GPIO 5 (ESP32 SS)

  tft.begin();
  delay(500);
  //tft.setRotation(0);  // Rotate 90°
  // Initialize LVGL and display here
  if (!SD.begin(5)) {
    Serial.println("SD card failed!");
  } else {
    Serial.println("SD card initialized.");
  }


  lv_init();
  smartdisplay_init();    


  const esp_timer_create_args_t lv_tick_timer_args = {
    .callback = &lv_tick_handler,
    .arg = NULL,
    .dispatch_method = ESP_TIMER_TASK,
    .name = "lv_tick"
  };
  esp_timer_handle_t lv_tick_timer;
  esp_timer_create(&lv_tick_timer_args, &lv_tick_timer);
  esp_timer_start_periodic(lv_tick_timer, 10000); // 10 ms interval

  lv_display_t *disp = lv_display_create(320, 240);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);


  lv_obj_t *scr = lv_scr_act();
  if (scr) {
    create_grid_with_buttons(scr);
  } else {
    printf("Error: lv_scr_act() returned NULL\n");
  }
}
ulong next_millis;
auto lv_last_tick = millis();

void loop() {
  if (pServer->getConnectedCount() > 0 && !deviceConnected) {
  //Serial.println("Detected device connected (GATT)");
    deviceConnected = true;
  }

  auto const now = millis();
  lv_tick_inc(now - lv_last_tick);
  lv_last_tick = now;

  lv_timer_handler();

  vTaskDelay(pdMS_TO_TICKS(5)); // Better than delay()
}
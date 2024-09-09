/* Author: Shaurya Pathak
   Last Editied: 22-05-2023

   This is a simple sketch for the LilyGO T-Watch which converts it to a Bluetooth Powerpoint Presenter for Windows. 

   It uses the BleKeyboard library to send Keystrokes to the PC after connecting.

   WARNING: The base version of the library will cause memory issues and compilation errors. The fix for this problem is to go in the BleKeyboard
            library file and un-comment the USE NIMBLE case. After doing this you must also install the NimBLE library, which should resolve the memory
            issues. 

    Please refer to the documentation attached with this sketch which will have more information about the documentation of the libraries and their
    intricacies.

*/

// Selecting our hardware for our program
#define LILYGO_WATCH_2020_V3           
// Enabling our GUI Library
#define LILYGO_WATCH_LVGL    
    

// Function Libraries
#include <LilyGoWatch.h>
#include <BleKeyboard.h>

// Initialising
BleKeyboard bleKeyboard("BT_WATCH_PRES", "LilyGO", 100); // This sets the device to be discoverable as "BT_WATCH_PRES"
TTGOClass *ttgo; 

// Here we are setting all of our functions which are used to send Key Strokes to the PC
static void prev(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
      bleKeyboard.write(KEY_LEFT_ARROW);
    }
}

static void next(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
      bleKeyboard.write(KEY_RIGHT_ARROW);
    }
}

static void stop(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
      bleKeyboard.write(KEY_ESC);
    }
}

static void start(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
      bleKeyboard.press(KEY_LEFT_SHIFT);
      bleKeyboard.press(KEY_F5);

      delay(500);

      bleKeyboard.releaseAll();
    }
}

static void mute(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
      bleKeyboard.write(KEY_MEDIA_MUTE);
    }
}

static void voldown(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
      bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    }
}

static void volinc(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
      bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    }
}

// Setup function to create the GUI buttons and Enable Bluetooth
void setup() 
{
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->lvgl_begin();
    bleKeyboard.begin();

    // Creating our UI
    lv_obj_t *label;

    lv_obj_t *tplbl = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(tplbl, "Bluetooth Presenter");
    lv_obj_align(tplbl, NULL, LV_ALIGN_CENTER, 0, -110);

    // Features of the button
    lv_obj_t *nxtbtn = lv_btn_create(lv_scr_act(), NULL); // Creating a next button
    lv_obj_set_event_cb(nxtbtn, next);
    lv_obj_align(nxtbtn, NULL, LV_ALIGN_CENTER, 120, 80);
    lv_obj_set_size(nxtbtn, 50, 50);
    // Button display
    label = lv_label_create(nxtbtn, NULL);
    lv_label_set_text(label, LV_SYMBOL_NEXT);

    // Features of the button
    lv_obj_t *prvbtn = lv_btn_create(lv_scr_act(), NULL); // Creating a back button
    lv_obj_set_event_cb(prvbtn, prev);
    lv_obj_align(prvbtn, NULL, LV_ALIGN_CENTER, -40, 80);
    lv_obj_set_size(prvbtn, 50, 50);
    // Button display
    label = lv_label_create(prvbtn, NULL);
    lv_label_set_text(label, LV_SYMBOL_PREV);

    // Features of the button
    lv_obj_t *closebtn = lv_btn_create(lv_scr_act(), NULL); // Creating a back button
    lv_obj_set_event_cb(closebtn, stop);
    lv_obj_align(closebtn, NULL, LV_ALIGN_CENTER, 42, 80);
    lv_obj_set_size(closebtn, 50, 50);
    // Button display
    label = lv_label_create(closebtn, NULL);
    lv_label_set_text(label, LV_SYMBOL_CLOSE);

    // Features of the button
    lv_obj_t *startbtn = lv_btn_create(lv_scr_act(), NULL); // Creating a back button
    lv_obj_set_event_cb(startbtn, start);
    lv_obj_align(startbtn, NULL, LV_ALIGN_CENTER, 42, 15);
    lv_obj_set_size(startbtn, 50, 50);
    // Button display
    label = lv_label_create(startbtn, NULL);
    lv_label_set_text(label, LV_SYMBOL_PLAY);

    // Features of the button
    lv_obj_t *volup = lv_btn_create(lv_scr_act(), NULL); // Creating a next button
    lv_obj_set_event_cb(volup, volinc);
    lv_obj_align(volup, NULL, LV_ALIGN_CENTER, 120, -50);
    lv_obj_set_size(volup, 50, 50);
    // Button display
    label = lv_label_create(volup, NULL);
    lv_label_set_text(label, LV_SYMBOL_VOLUME_MAX);

    // Features of the button
    lv_obj_t *voldwn = lv_btn_create(lv_scr_act(), NULL); // Creating a back button
    lv_obj_set_event_cb(voldwn, voldown);
    lv_obj_align(voldwn, NULL, LV_ALIGN_CENTER, 42, -50);
    lv_obj_set_size(voldwn, 50, 50);
    // Button display
    label = lv_label_create(voldwn, NULL);
    lv_label_set_text(label, LV_SYMBOL_VOLUME_MID);

    // Features of the button
    lv_obj_t *volmute = lv_btn_create(lv_scr_act(), NULL); // Creating a back button
    lv_obj_set_event_cb(volmute, mute);
    lv_obj_align(volmute, NULL, LV_ALIGN_CENTER, -40, -50);
    lv_obj_set_size(volmute, 50, 50);
    // Button display
    label = lv_label_create(volmute, NULL);
    lv_label_set_text(label, LV_SYMBOL_MUTE);
}

// Nothing in the loop because the LVGL library takes care of everything else
void loop() {
  lv_task_handler();
  delay(5);
}

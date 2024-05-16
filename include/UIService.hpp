#include "../config/LGFX_ESP32_YELGURK.hpp"
#include <lvgl.h>
#include <vector>
#include "../img/back_black.hpp"
#include "../img/back_dark.hpp"
#include "../img/back_blue.hpp"
#include "../img/back_gold.hpp"
#include "../img/screen_baby.hpp"
#include "../img/screen_logo_1.hpp"
#include "../img/screen_logo_2.hpp"
#include "../img/screen_money.hpp"
#include "../font/lv_font_bebas_24.hpp"
#include "../font/lv_font_bebas_34.hpp"
#include "../font/lv_font_bebas_44.hpp"
#include "../font/lv_font_bebas_56.hpp"

#ifndef UIService_hpp
#define UIService_hpp

extern const uint8_t LCD_SDA;
extern const uint8_t LCD_SCL;
extern const uint8_t LCD_DC;
extern const uint8_t LCD_CS[6];
extern const uint8_t BTN[6];
extern uint8_t LCD_OPA;

#define LGFX_USE_V1
#define SCREEN_WIDTH    320U
#define SCREEN_HEIGHT   170U
#define SCREEN_BUFFER   (SCREEN_WIDTH * SCREEN_HEIGHT * LV_COLOR_DEPTH) / 10 / 2

using namespace std;

static LGFX**           lcd;
static uint8_t**        lvBuff_1;
static uint8_t**        lvBuff_2;
static lv_obj_t**       screen;
static vector<lv_obj_t*> ** _content;

static void lcd_flush_cb(lv_display_t* display, const lv_area_t* area, unsigned char* data, uint8_t panel_index)
{
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);
    lv_draw_sw_rgb565_swap(data, w*h);
    lcd[panel_index]->pushImage(area->x1, area->y1, w, h, (uint16_t*)data);
    lv_display_flush_ready(display);
}

class UIService
{
private:
    lv_obj_t * _create_content(lv_obj_t * screen, const void* img_src, const char* text); 
    const lv_font_t * _get_font_size(const char* text);
    void init_content();
    void hide_screen_content(vector<lv_obj_t*> * scr_content);
    void hide_all_content();

public:
    void init_screen();
    void show_hello_screen();
    void show_main_screen();
    void show_content(uint8_t screen_no, uint8_t content_no);
    uint8_t get_size(uint8_t screen_no);
    void set_opa(uint8_t screen_no, uint8_t content_no);
};

#endif
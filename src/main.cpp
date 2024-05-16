#include "../include/main.hpp"

extern const uint8_t LCD_SDA    = 5;
extern const uint8_t LCD_SCL    = 6;
extern const uint8_t LCD_DC     = 4;
extern const uint8_t LCD_CS[6]  = { 7, 15, 16, 17, 18, 8 };
extern const uint8_t BTN[6]     = { 3, 46, 9, 10, 11, 12 };
extern uint8_t LCD_OPA = 255;

static volatile bool trigger = false;

void setup()
{
    Serial.begin(115200);

    for (auto pin : BTN)
    {
        pinMode(pin, INPUT_PULLDOWN);
        attachInterrupt(pin, [](){ trigger = true; }, RISING);
    }

    for (auto lcd_cs_pin : LCD_CS)
    {
        pinMode(lcd_cs_pin, OUTPUT);
        digitalWrite(lcd_cs_pin, 1);
    }

    Serial.println("LCD init...");
    UI.init_screen();
    UI.show_hello_screen();
    Serial.println("LCD inited!");

    /*
    pinMode(HIT_SENSOR_DIGITAL_S_ESP_PIN, INPUT);
    attachInterrupt(
        HIT_SENSOR_DIGITAL_S_ESP_PIN,
        [](){ hit_sensor_trigger = true; },
        RISING
    );
    */
}

void roll(uint8_t index);

void loop()
{
    lv_task_handler();

    if (trigger)
    {
        if (!game_started)
        {
            game_started = true;
            UI.show_main_screen();
        }
        else
            for (uint8_t pin = 0; pin < 6; pin++)
                if (digitalRead(BTN[pin]) > 0)
                {
                    roll(pin);
                    break;
                }

        trigger = false;
    }
}

void roll(uint8_t index)
{
    UI.show_main_screen();
    UI.show_content(index, 2);
    LCD_OPA = 255;

    uint8_t roll_res = millis() % UI.get_size(index);
    roll_res = roll_res < 2 ? 2 : roll_res;

    uint32_t ms_start       = millis(),
             ms_old         = millis(),
             ms_curr        = millis(),
             ms_del_hide    = 4000,
             ms_del_show    = 7000,
             ms_del         = 600;

    uint8_t min = 2,
            max = UI.get_size(index),
            curr_index = 0;

    double spl = 1.5;
    double temp;

    for (uint8_t i = min + 1; millis() - ms_start < ms_del_hide;)
    {
        if ((ms_curr = millis()) - ms_old >= ms_del)
        {
            ms_old = ms_curr;
            ms_del /= (spl /= 1.04) < 1.0 ? 1 : spl;

            UI.show_content(index, (curr_index = i));

            if ((i += 1) >= max)
                i = min;
        }

        LCD_OPA = millis() - ms_start < 1750 ? 255 : static_cast<uint8_t>(
            255.0 / 1750 * ((temp = 1750 - (millis() - 1750 - ms_start)) < 0 ? 0 : temp)
        ); 

        if (millis() - ms_start >= 3400)
            LCD_OPA = 0;

        UI.set_opa(index, curr_index);
        lv_task_handler();
    }

    LCD_OPA = 0;
    UI.show_content(index, roll_res);
    ms_start = millis();
    
    while (millis() - ms_start < 500)
    {
        LCD_OPA = static_cast<uint8_t>(255.0 / 500 * (millis() - ms_start)); 

        UI.set_opa(index, roll_res);
        lv_task_handler();
    }

    LCD_OPA = 255;

    while (millis() - ms_start < ms_del_show)
    {
        UI.set_opa(index, roll_res);
        lv_task_handler();
    }

    UI.show_main_screen();
}
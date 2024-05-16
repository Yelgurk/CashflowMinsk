#include "../include/UIService.hpp"

static uint32_t my_tick_get_cb(void) {
  return millis();
}

void UIService::init_screen()
{
    uint8_t lcd_count = sizeof(LCD_CS) / sizeof(uint8_t);

    lcd      = new LGFX*[lcd_count];
    lvBuff_1 = new uint8_t*[lcd_count];
    lvBuff_2 = new uint8_t*[lcd_count];
    screen   = new lv_obj_t*[lcd_count];
    _content = new vector<lv_obj_t*>*[lcd_count];

    for (uint8_t panel = 0; panel < lcd_count; panel++)
    {
        lcd[panel]      = new LGFX(LCD_SDA, LCD_SCL, LCD_DC, LCD_CS[panel]);
        lvBuff_1[panel] = new uint8_t[SCREEN_BUFFER];
        lvBuff_2[panel] = new uint8_t[SCREEN_BUFFER];
        _content[panel] = new vector<lv_obj_t*>();
    }

    for (uint8_t panel = 0; panel < lcd_count; panel++)
    {
        LGFX* lcd_init = lcd[panel];
        lcd_init->init();
        lcd_init->setRotation(3);

        lv_init();
        lv_tick_set_cb(my_tick_get_cb);
    
        auto * disp = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
        lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);

        if (panel == 0)
            lv_display_set_flush_cb(disp, [](lv_display_t* display, const lv_area_t* area, unsigned char* data) {
                lcd_flush_cb(display, area, data, 0);
            });
        else
        if (panel == 1)
            lv_display_set_flush_cb(disp, [](lv_display_t* display, const lv_area_t* area, unsigned char* data) {
                lcd_flush_cb(display, area, data, 1);
            });
        else
        if (panel == 2)
            lv_display_set_flush_cb(disp, [](lv_display_t* display, const lv_area_t* area, unsigned char* data) {
                lcd_flush_cb(display, area, data, 2);
            });
        else
        if (panel == 3)
            lv_display_set_flush_cb(disp, [](lv_display_t* display, const lv_area_t* area, unsigned char* data) {
                lcd_flush_cb(display, area, data, 3);
            });
        else
        if (panel == 4)
            lv_display_set_flush_cb(disp, [](lv_display_t* display, const lv_area_t* area, unsigned char* data) {
                lcd_flush_cb(display, area, data, 4);
            });
        else
        if (panel == 5)
            lv_display_set_flush_cb(disp, [](lv_display_t* display, const lv_area_t* area, unsigned char* data) {
                lcd_flush_cb(display, area, data, 5);
            });

        lv_display_set_buffers(disp, lvBuff_1[panel], lvBuff_2[panel], SCREEN_BUFFER, LV_DISPLAY_RENDER_MODE_PARTIAL);
        lv_disp_set_default(disp);

        screen[panel] = lv_obj_create(NULL);
        lv_obj_clear_flag(screen[panel], LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_bg_color(screen[panel], lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        
        lv_disp_load_scr(screen[panel]);
    }

    init_content();
}

void UIService::hide_screen_content(vector<lv_obj_t*> * scr_content)
{
    for (uint8_t i = 0; i < scr_content->size(); i++)
        lv_obj_set_style_opa(scr_content->at(i), 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void UIService::hide_all_content()
{
    for (uint8_t i = 0; i < 6; i++)
        hide_screen_content(_content[i]);
}

void UIService::show_hello_screen()
{
    hide_all_content();
    for (uint8_t i = 0; i < 6; i++)
        lv_obj_set_style_opa(_content[i]->at(0), LCD_OPA, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void UIService::show_main_screen()
{
    hide_all_content();
    for (uint8_t i = 0; i < 6; i++)
        lv_obj_set_style_opa(_content[i]->at(1), LCD_OPA, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void UIService::show_content(uint8_t screen_no, uint8_t content_no)
{
    hide_screen_content(_content[screen_no]);
    lv_obj_set_style_opa(_content[screen_no]->at(content_no), LCD_OPA, LV_PART_MAIN | LV_STATE_DEFAULT);
}

uint8_t UIService::get_size(uint8_t screen_no) {
    return _content[screen_no]->size();
}


void UIService::set_opa(uint8_t screen_no, uint8_t content_no)
{
    lv_obj_set_style_opa(_content[screen_no]->at(content_no), LCD_OPA, LV_PART_MAIN | LV_STATE_DEFAULT);
}

lv_obj_t * UIService::_create_content(lv_obj_t * screen, const void* img_src, const char* text)
{
    lv_obj_t * container = lv_img_create(screen);
    lv_img_set_src(container, img_src);
    lv_obj_set_width(container, SCREEN_WIDTH);
    lv_obj_set_height(container, SCREEN_HEIGHT);
    lv_obj_set_align(container, LV_ALIGN_CENTER);
    lv_obj_add_flag(container, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    //lv_img_set_zoom(container, 320);
    lv_obj_set_style_opa(container, LCD_OPA, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * label = lv_label_create(container);
    lv_obj_set_align(label, LV_ALIGN_CENTER);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, _get_font_size(text), LV_PART_MAIN | LV_STATE_DEFAULT);

    return container;
}

const lv_font_t * UIService::_get_font_size(const char* text)
{
    uint16_t lenght = strlen(text);

    if (lenght < 20)
        return &lv_font_bebas_56;
    
    if (lenght < 40)
        return &lv_font_bebas_44;
    
    if (lenght < 120)
        return &lv_font_bebas_34;
    
    return &lv_font_bebas_24;
}

void UIService::init_content()
{
    _content[0]->push_back(_create_content(screen[0], &screen_logo_2, ""));
    _content[0]->push_back(_create_content(screen[0], &back_black, "ЧЕРНАЯ ПОЛОСА"));
    _content[0]->push_back(_create_content(screen[0], &back_black, "ВОЗЬМИТЕ\nКАРТОЧКУ\nРАСХОДЫ"));
    _content[0]->push_back(_create_content(screen[0], &back_black, "РАЗВОД"));
    _content[0]->push_back(_create_content(screen[0], &back_black, "ВАША СТРАХОВКА\nОТ УВОЛЬНЕНИЯ\nАННУЛИРОВАНА"));
    _content[0]->push_back(_create_content(screen[0], &back_black, "СЫГРАЙТЕ В КАЗИНО\nставка 5 000 $"));
    _content[0]->push_back(_create_content(screen[0], &back_black, "У ВАС УКРАЛИ\nДЕНЬГИ\n2 000 $"));
    _content[0]->push_back(_create_content(screen[0], &back_black, "ВЫ СИЛЬНО ЗАБОЛЕЛИ!\nТЕРЯЕТЕ ВСЕ ДЕНЬГИ\nНА РУКАХ!"));
    _content[0]->push_back(_create_content(screen[0], &back_black, "ВАШ ДЕПОЗИТ\nВ БАНКЕ\nСГОРЕЛ"));
    _content[0]->push_back(_create_content(screen[0], &back_black, "ДОХОД ОТ БИЗНЕСА\nС НАИБОЛЬШИМ\nДЕНЕЖНЫМ ПОТОКОМ\nУПАЛ ДО НУЛЯ"));
    _content[0]->push_back(_create_content(screen[0], &back_black, "ЖИЛЬЦЫ ИСПОРТИЛИ МЕБЕЛЬ!\nДОХОД ОТ АРЕНДЫ ВАШЕЙ\nНЕДВИЖИМОСТИ С\nНАИБОЛЬШИМ ДЕНЕЖНЫМ ПОТОКОМ\nУПАЛ ДО НУЛЯ!"));
    _content[0]->push_back(_create_content(screen[0], &back_black, "ВАШ ДОМИК В ЗАПОВЕДНИКЕ\nСГОРЕЛ ДО ТЛА"));
    _content[0]->push_back(_create_content(screen[0], &back_black, "ВАШ БИЗНЕС НЕ ВЫДЕРЖАЛ\nКОНКУРЕНЦИИ! КАССОВЫЙ"));
    _content[0]->push_back(_create_content(screen[0], &back_black, "РАЗРЫВ! ДЕНЕЖНЫЙ ПОТОК\nМИНУС 500 $"));
    
    _content[1]->push_back(_create_content(screen[1], &screen_logo_1, ""));
    _content[1]->push_back(_create_content(screen[1], &screen_money, ""));
    _content[1]->push_back(_create_content(screen[1], &back_gold, "ЕСЛИ У ВАС ЕСТЬ\nЗЕМЛЯ ЗА ГОРОДОМ (6 СОТОК)\nМОЖЕТЕ ПОСТРОИТЬ ДОМ НА 2\nСПАЛЬНИ ЗА 50 000 $"));
    _content[1]->push_back(_create_content(screen[1], &back_gold, "ВЫГОДНЫЙ ДЕПОЗИТ\nВ БАНК ПОД 2%\nСУММА 100 000 $"));
    _content[1]->push_back(_create_content(screen[1], &back_gold, "ВЫИГРАЛИ 5000 $"));
    _content[1]->push_back(_create_content(screen[1], &back_gold, "ВЫ НАШЛИ АРЕНДАТОРА\nНА СВОЕ НОВОЕ ЖИЛЬЕ\n+300 $ ЗА СПАЛЬНЮ"));
    _content[1]->push_back(_create_content(screen[1], &back_gold, "ВЫ НАШЛИ ПОКУПАТЕЛЯ\nНА СВОЙ АВТОМОБИЛЬ\nПО РЫНОЧНОЙ ЦЕНЕ"));
    _content[1]->push_back(_create_content(screen[1], &back_gold, "ПРЕМИЯ НА РАБОТЕ\nХ2"));
    _content[1]->push_back(_create_content(screen[1], &back_gold, "ВЫ ЗАЧЕМ-ТО\nПРОПУСТИЛИ СВОЮ ЗАРПЛАТУ…"));
    _content[1]->push_back(_create_content(screen[1], &back_gold, "РЫНОК - АУКЦИОН"));
    _content[1]->push_back(_create_content(screen[1], &back_gold, "СТРАХОВКА ОТ УВОЛЬНЕНИЯ\nИ БОЛЕЗНИ\n3 000 $"));
    _content[1]->push_back(_create_content(screen[1], &back_gold, "ЕЖЕМЕСЯЧНЫЙ РАСХОД\n-300 $"));
    _content[1]->push_back(_create_content(screen[1], &back_gold, "ВАШ ДОМИК В ЗАПОВЕДНИКЕ\nПРИВЛЕК СТРАННЫХ ЛЮДЕЙ\nВАМ ПРЕДЛАГАЮТ ЗА НЕГО\n60 000$"));

    _content[2]->push_back(_create_content(screen[2], &screen_logo_2, ""));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "БЛАГОВТОРИТЕЛЬНОСТЬ\n\nпожертвуйте\n10% от общего дохода"));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "ВЫ ПОЗНАКОМИЛИСЬ\nС ЮРИСТОМ"));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "ВАМ УЛЫБНУЛАСЬ УДАЧА!\nКИДАЕТЕ 3 КУБИКА\n2 ХОДА"));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "ВЫ ЗАПЛАТИЛИ\nУЛИЧНОМУ МУЗЫКАНТУ"));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "ВАМ УЛЫБНУЛАСЬ УДАЧА!\nКИДАЕТЕ 4 КУБИКА\n1 ХОД"));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "ВАМ УЛЫБНУЛАСЬ УДАЧА!\nКИДАЕТЕ 2 КУБИКА\n3 ХОДА"));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "ВАС ОТБЛАГОДАРИЛИ!\nВЫ ПОЛУЧАЕТЕ\n20 ТОКЕНОВ SOL"));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "ФОНДЫ,\nКОТОРЫМИ ВЫ ВЛАДЕЕТЕ,\nУДВОИЛИСЬ!"));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "ВОЗМОЖНОСТЬ"));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "ВЫ ПОЖЕРТВОВАЛИ\nВ ПРИЮТ ДЛЯ ЖИВОТНЫХ"));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "ДЕЛАЕТЕ\nДОПОЛНИТЕЛЬНЫЙ ХОД"));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "ЧАШКА АРОМАТНОГО КОФЕ\nОТ ЗАВЕДЕНИЯ"));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "ВЫ НАШЛИ 1 ФРАГМЕНТ\nКАРТЫ"));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "ВАС ПОЗНАКОМИЛИ\nС АРХИТЕКТОРОМ"));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "РЫНОК"));
    _content[2]->push_back(_create_content(screen[2], &back_blue, "ИММУНИТЕТ\nОТ ЧЕРНОЙ ПОЛОСЫ\nНА 50% РАСХОДОВ"));

    _content[3]->push_back(_create_content(screen[3], &screen_logo_1, ""));
    _content[3]->push_back(_create_content(screen[3], &screen_money, ""));
    _content[3]->push_back(_create_content(screen[3], &back_gold, "ВЫЙГРАЛИ В ЛОТЕРЕЮ\n4000 $"));
    _content[3]->push_back(_create_content(screen[3], &back_gold, "ЕСЛИ ЕСТЬ ЗНАКАОМЫЙ\nАРХИТЕКТОР И ЗЕМЛЯ В ГОРОДЕ (6СОТОК)\nВЫ МОЖЕТЕ ПОСТРОИТЬ ДОМ\nНА 5 СПАЛЕН 120 000 $ ЕСЛИ НЕТ,\nТО НА 2 СПАЛЬНИ\nЗА 60 000 $"));
    _content[3]->push_back(_create_content(screen[3], &back_gold, "ДОХОД ОТ ВАШЕГО БИЗНЕСА\nC НАИМЕНЬШИМ ДЕНЕЖНЫМ ПОТОКОМ\nх2"));
    _content[3]->push_back(_create_content(screen[3], &back_gold, "ВАШ БИЗНЕС\nС НАИВЫСШИМДЕНЕЖНЫМ ПОТОКОМ\nХОТЯТ КУПИТЬ\nЗА 25 000 $"));
    _content[3]->push_back(_create_content(screen[3], &back_gold, "ВАШ ДОМИК В ЗАПОВЕДНИКЕ\nПРИВЛЕК ТУРИСТОВ!\nВЫ ПОЛУЧАЕТЕ + 500 $\nК ПАССИВНОМУ ДОХОДУ"));
    _content[3]->push_back(_create_content(screen[3], &back_gold, "ХОРОШАЯ КАРМА!\nУ ВАС ЗАЩИТАОТ ЧЕРНОЙ ПОЛОСЫ\nНА 1 РАЗ"));
    _content[3]->push_back(_create_content(screen[3], &back_gold, "ЕСЛИ У ВАС\nЕСТЬ ЗЕМЛЯ В ГОРОДЕ (6СОТОК)\nМОЖЕТЕ ПОСТРОИТЬ КОТТЕДЖ\nНА 3 СПАЛЬНИ ЗА 80 000 $"));
    _content[3]->push_back(_create_content(screen[3], &back_gold, "ВАШ ДОМИК В ЗАПОВЕДНИКЕ\nПРИВЛЕК ЛЮБИТЕЛЕЙ ТИШИНЫ\nВЫ ПОЛУЧАЕТЕ +300 $\nК ПАССИВНОМУ ДОХОДУ"));
    _content[3]->push_back(_create_content(screen[3], &back_gold, "ПРЕМИЯ НА РАБОТЕ х3"));
    _content[3]->push_back(_create_content(screen[3], &back_gold, "ВЫ ЗАЧЕМ-ТО\nПРОПУСТИЛИ СВОЮ ЗАРПЛАТУ…"));
    _content[3]->push_back(_create_content(screen[3], &back_gold, "СТРАХОВКА НЕДВИЖИМОСТИ 500 $\nМИНУС 50 $ ДЕНЕЖНОГО ПОТОКА"));
    _content[3]->push_back(_create_content(screen[3], &back_gold, "СТРАХОВКА\nОТ УВОЛЬНЕНИЯИ БОЛЕЗНИ\nСТОИМОСТЬ - 2 000 $\n\nМИНУС 200 $ ДЕНЕЖНОГО ПОТОКА"));
    _content[3]->push_back(_create_content(screen[3], &back_gold, "ВЫ СДАЛИ СВОЙ\nНОВЫЙ АВТОМОБИЛЬ В АРЕНДУ\nПАССИВНЫЙ ДОХОД +700 $"));
    _content[3]->push_back(_create_content(screen[3], &back_gold, "ВЫ НАШЛИ ПАРОЛЬ ОТ КОШЕЛЬКА,\nГДЕ ХРАНИЛИ СВОИ БИТКОЙНЫ\n\n+1 БИТКОИН"));

    _content[4]->push_back(_create_content(screen[4], &screen_logo_2, ""));
    _content[4]->push_back(_create_content(screen[4], &screen_baby, ""));
    _content[4]->push_back(_create_content(screen[4], &back_dark, "ВАШИ ДЕТИ МОТИВИРУЮТ ВАС!\nВЫ УСЕРДНО ТРУДИТЕСЬ!\nДОБАВЬТЕ +500 $\nК ПАССИВНОМУ ДОХОДУ"));
    _content[4]->push_back(_create_content(screen[4], &back_dark, "ЕСЛИ ПРОШЛО 2 ЧАСА ИГРЫ:\nДЕТИ ПРОДОЛЖИЛИ ВАШЕ ДЕЛО!\nЗАПИШИТЕ 10% К ПАССИВНОМУ ДОХОДУ\nЗА КАЖДОГО РЕБЕНКА"));
    _content[4]->push_back(_create_content(screen[4], &back_dark, "ВАШИ ДЕТИ\nУСТРОИЛИ ПОТОП В ШКОЛЕ!\nЗАПЛАТИТЕ 200 $ ШТРАФ\nЗА КАЖДОГО"));
    _content[4]->push_back(_create_content(screen[4], &back_dark, "ВАШ РЕБЕНОК\nЗАНЯЛ ПЕРВОЕ МЕСТО\nНА ГОРОДСКОЙ ОЛИМПИАДЕ!\nВАС ПЕРЕПОЛНЯЕТ ГОРДОСТЬ!"));
    _content[4]->push_back(_create_content(screen[4], &back_dark, "ДЕЙСТВУЕТ НА ПРОТЯЖЕНИИ 1ого ЧАСА!\nВАШИ ДЕТИ НЕ ДАЮТ СПАТЬ!\nВАША НЕРВНАЯ СИСТЕМА ИСТОЩЕНА\n-100 $ ПАССИВНОГО ДОХОДА\nЗА КАЖДОГО РЕБЕНКА"));
    _content[4]->push_back(_create_content(screen[4], &back_dark, "ЕСЛИ СЕЙЧАС 3Й ЧАС ИГРЫ:\nВАШ РЕБЕНОК\nСТАЛ УСПЕШНЫМ ПРЕДПРИНИМАТЕЛЕМ!\nПОДАРИЛ ВАМ КОТТЕДЖ НА 2 СПАЛЬНИ!\nМОЖЕТЕ СДАВАТЬ ЗА 800$\nИЛИ ПРОДАТЬ В РЫНКЕ"));

    _content[5]->push_back(_create_content(screen[5], &screen_logo_1, ""));
    _content[5]->push_back(_create_content(screen[5], &screen_money, ""));
    _content[5]->push_back(_create_content(screen[5], &back_gold, "ВЫИГРАЛИ 2000$"));
    _content[5]->push_back(_create_content(screen[5], &back_gold, "ЕСЛИ ЕСТЬ ЗНАКОМЫЙ\nАРХИТЕКТОР И ЗЕМЛЯ\nВ ГОРОДЕ (6 СОТОК)\n\n ВЫ МОЖЕТЕ ПОСТРОИТЬ ДОМ\nНА 5 СПАЛЕН 150 000$"));
    _content[5]->push_back(_create_content(screen[5], &back_gold, "ДОХОД\nОТ ВАШЕГО БИЗНЕСА\nх2"));
    _content[5]->push_back(_create_content(screen[5], &back_gold, "ВАШ БИЗНЕС С НАИВЫСШИМ\nДЕНЕЖНЫМ ПОТОКОМ ОТ 500$\nХОТЯТ КУПИТЬ\nЗА 25 000$"));
    _content[5]->push_back(_create_content(screen[5], &back_gold, "ВАШ ДОМИК В ЗАПОВЕДНИКЕ\nПРИВЛЕК ТУРИСТОВ!\nВЫ ПОЛУЧАЕТЕ +500$\nК ПАССИВНОМУ ДОХОДУ"));
    _content[5]->push_back(_create_content(screen[5], &back_gold, "ХОРОШАЯ КАРМА!\nУ ВАС ЗАЩИТА\nОТ ЧЕРНОЙ ПОЛОСЫ\n\n1 РАЗ ЗА ИГРУ"));
    _content[5]->push_back(_create_content(screen[5], &back_gold, "ЕСЛИ У ВАС ЕСТЬ\nЗЕМЛЯ В ГОРОДЕ (6 СОТОК)\nМОЖЕТЕ ПОСТРОИТЬ КОТТЕДЖ\nНА 3 СПАЛЬНИ ЗА 80 000$"));
    _content[5]->push_back(_create_content(screen[5], &back_gold, "ВАШ ДОМИК В ЗАПОВЕДНИКЕ\nПРИВЛЕК ЛЮБИТЕЛЕЙ ТИШИНЫ!\nВЫ ПОЛУЧАЕТЕ +300$\nК ПАССИВНОМУ ДОХОДУ"));
    _content[5]->push_back(_create_content(screen[5], &back_gold, "ПРЕМИЯ НА РАБОТЕ\nх2"));
    _content[5]->push_back(_create_content(screen[5], &back_gold, "ВЫ ЗАЧЕМ-ТО\nПРОПУСТИЛИ СВОЮ ЗАРПЛАТУ..."));
    _content[5]->push_back(_create_content(screen[5], &back_gold, "СТРАХОВКА\nОТ УВОЛЬНЕНИЯ И БОЛЕЗНИ\nСТОИМОСТЬ - 2000$\n- 200$ ДЕНЕЖНОГО ПОТОКА"));
    _content[5]->push_back(_create_content(screen[5], &back_gold, "ВЫ СДАЛИ СВОЙ\nНОВЫЙ АВТОМОБИЛЬ В АРЕНДУ\nПАССИВНЫЙ ДОХОД +700$"));
}
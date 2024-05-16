#pragma once

#define LGFX_USE_V1

#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
//lgfx::Panel_ILI9481     _panel_instance;
//lgfx::Panel_ILI9486     _panel_instance;
//lgfx::Panel_ILI9488     _panel_instance;
//lgfx::Panel_ST7796      _panel_instance;
lgfx::Panel_ST7789      _panel_instance;

//lgfx::Bus_Parallel8 _bus_instance;
lgfx::Bus_SPI _bus_instance;

//lgfx::Light_PWM     _light_instance;

public:
  LGFX(uint8_t sda, uint8_t scl, uint8_t dc, uint8_t cs)
  {
    {
      auto cfg = _bus_instance.config();

      //cfg.i2s_port = I2S_NUM_0;
      cfg.spi_mode = 0;
      cfg.spi_3wire  = true;
      cfg.freq_write = 24000000;
      cfg.pin_sclk = scl;
      cfg.pin_mosi = sda;
      cfg.pin_miso = -1;
      cfg.pin_dc   = dc;

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();

      cfg.pin_cs           =    cs;
      cfg.pin_rst          =    -1;
      cfg.pin_busy         =    -1;

      cfg.panel_width      =   170;
      cfg.panel_height     =   320;
      cfg.offset_x         =    35;
      cfg.offset_y         =     0;
      cfg.offset_rotation  =     0;
      cfg.dummy_read_pixel =     8;
      cfg.dummy_read_bits  =     1;
      cfg.readable         =  true;
      cfg.invert           =  true;
      cfg.rgb_order        = false;
      cfg.dlen_16bit       = false;
      cfg.bus_shared       =  true;

      /* only for test */
      cfg.memory_width     =   170;
      cfg.memory_height    =   320; 

      _panel_instance.config(cfg);
    }

    setPanel(&_panel_instance);
  }
};
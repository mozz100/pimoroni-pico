#include <stdio.h>
#include "pico/stdlib.h"
#include <string>

#include "breakout_paa5100.hpp"

using namespace pimoroni;

BreakoutPAA5100 flo(BG_SPI_FRONT);
BreakoutPAA5100::Degrees rotation = BreakoutPAA5100::DEGREES_0;
const uint8_t SIZE = BreakoutPAA5100::FRAME_SIZE;
uint8_t data[BreakoutPAA5100::RAW_DATA_LEN];

std::string value_to_char(uint8_t value) {
  const std::string charmap = " .:-=+*#%@";
  float val = (float)value / 255.0f;
  val *= charmap.length() - 1;
  value = (uint8_t)val;
  std::string chosen_char = charmap.substr(val, 1);
  return chosen_char.append(chosen_char);  // Double chars to - sort of - correct aspect ratio
}

int main() {
  stdio_init_all();

  flo.init();
  flo.set_rotation(rotation);

  uint8_t offset = 0;
  uint8_t value = 0;
  
  while(true) {
    printf("Capturing...\n");
    uint16_t data_size = 0;
    if(flo.frame_capture(data, data_size)) {
      for(uint8_t y = 0; y < SIZE; y++) {
        if(rotation == BreakoutPAA5100::DEGREES_180 || rotation == BreakoutPAA5100::DEGREES_270)
          y = SIZE - y - 1;

        for(uint8_t x = 0; x < SIZE; x++) {
          if(rotation == BreakoutPAA5100::DEGREES_180 || rotation == BreakoutPAA5100::DEGREES_90)
            x = SIZE - x - 1;

          if(rotation == BreakoutPAA5100::DEGREES_90 || rotation == BreakoutPAA5100::DEGREES_270)
            offset = (x * 35) + y;
          else
            offset = (y * 35) + x;

          value = data[offset];
          printf("%s", value_to_char(value).c_str());
        }
        printf("\n");
      }
    }
    else {
      printf("Capture failed. %d bytes received, of %d. Recapturing in ", data_size, BreakoutPAA5100::RAW_DATA_LEN);
    }
    printf("5...\n");
    sleep_ms(1000);
    printf("4...\n");
    sleep_ms(1000);
    printf("3...\n");
    sleep_ms(1000);
    printf("2...\n");
    sleep_ms(1000);
    printf("Get Ready!\n");
    sleep_ms(1000);
  };
  return 0;
}
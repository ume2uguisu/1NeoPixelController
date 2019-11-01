#include <Adafruit_NeoPixel.h>

// hardwear settings
#define LED_PIN 4
#define SW_PIN 0
#define NUM_PIXELS 1

int pixelFormat = NEO_GRB + NEO_KHZ800;
Adafruit_NeoPixel *pixels;

// colors
#define RED 0
#define GREEN 1
#define BLUE 2
#define YELLOW 3
#define PURPLE 4
#define YELLOW_GREEN 5
#define WHITE 6
#define MIX 7

// flash patterns
#define SOLID 0
#define SLOWLY 1
#define QUICKLY 2
#define VERY_QUICKLY 3

// actions
#define NONE 0
#define CHANGE_COLOR 1
#define CHANGE_FLASH 2
#define MIX_COUNT_MAX 6

typedef struct { int rgb[3]; } color;

//Gloval Variables
int color_mode = 0;
int flash_mode = 0;
int push_count = 0;
int brightness = 0;
int mix_current_col = RED;
int mix_count =0;

int next_brightness(){

  switch(flash_mode){
    case SOLID:
      brightness=255;
      break;
    case SLOWLY:
      if(brightness==255){
        brightness=0;
      }else{
        brightness=brightness+1;
      }
      break;
    case QUICKLY:
      if(brightness==255){
        brightness=0;
      }else{
        brightness=brightness+6;
      }
      break;
    case VERY_QUICKLY:
      if(brightness==255){
        brightness=0;
      }else{
        brightness=brightness+12;
      }
      break;
   }
}

int check_sw_action(){
    int action = 0;
    if(push_count>=10){
      action = CHANGE_COLOR;
    }
    if(push_count>=30){
      action = CHANGE_FLASH;
    }
    return action;
}

color get_rgb(int request){
  color rgb;
  switch(request){
      case RED:
         rgb = (color){brightness, 0, 0};
        break;
      case GREEN:
         rgb = (color){0, brightness,0 };
        break;
      case BLUE:
        rgb = (color){0, 0, brightness};
        break;        
      case YELLOW:
        rgb = (color){brightness, brightness, 0};
        break;        
      case PURPLE:
        rgb = (color){brightness, 0, brightness};
        break;        
      case YELLOW_GREEN:
        rgb = (color){0, brightness, brightness};
        break;       
      case WHITE:
        rgb = (color){brightness, brightness, brightness};
        break;
      case MIX:
        if (mix_count < MIX_COUNT_MAX){
          mix_count++;
        }else{
          if (mix_current_col == BLUE){
            mix_current_col = RED;
          }else{
            mix_current_col ++;
          }
          mix_count = 0;
        }
        rgb = get_rgb(mix_current_col);
        break;
  }
  return rgb;
}

void setup() {
  pixels = new Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, pixelFormat);
  pixels->begin();
  pixels->clear(); 
  pinMode(SW_PIN, INPUT_PULLUP);
}

void loop() {
  int action = NONE;
  
  if(digitalRead(SW_PIN) == LOW) {
    push_count ++;
  }else{
      action=check_sw_action();
      
      switch(action){
          case CHANGE_COLOR:
            if(color_mode == MIX){
              color_mode = RED;
            }else{
              color_mode++;
            }
          break;
          case CHANGE_FLASH:
            if(flash_mode == VERY_QUICKLY){
                flash_mode = SOLID;
            }else{
                flash_mode++;
              }
            break;
          case NONE:
            break;
      }
    push_count = 0;
  }
}

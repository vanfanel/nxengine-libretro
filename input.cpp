#include "nx.h"
#include "libsnes.hpp"

uint8_t mappings[SDLK_LAST];

bool inputs[INPUT_COUNT];
bool lastinputs[INPUT_COUNT];
int last_sdl_key;


bool input_init(void)
{
	memset(inputs, 0, sizeof(inputs));
	memset(lastinputs, 0, sizeof(lastinputs));
	memset(mappings, 0xff, sizeof(mappings));

   mappings[SNES_DEVICE_ID_JOYPAD_LEFT]   = LEFTKEY;  
   mappings[SNES_DEVICE_ID_JOYPAD_RIGHT]  = RIGHTKEY;  
   mappings[SNES_DEVICE_ID_JOYPAD_UP]     = UPKEY;  
   mappings[SNES_DEVICE_ID_JOYPAD_DOWN]   = DOWNKEY;  
   
   mappings[SNES_DEVICE_ID_JOYPAD_B] = JUMPKEY;
   mappings[SNES_DEVICE_ID_JOYPAD_Y] = FIREKEY;
   mappings[SNES_DEVICE_ID_JOYPAD_L] = PREVWPNKEY;
   mappings[SNES_DEVICE_ID_JOYPAD_R] = NEXTWPNKEY;
   mappings[SNES_DEVICE_ID_JOYPAD_A] = INVENTORYKEY;
   mappings[SNES_DEVICE_ID_JOYPAD_X] = MAPSYSTEMKEY;
	
	return 0;
}

// set the SDL key that triggers an input
void input_remap(int keyindex, int sdl_key)
{}

// get which SDL key triggers a given input
int input_get_mapping(int keyindex)
{
   int i;

   for(i=0;i<=SDLK_LAST;i++)
   {
      if (mappings[i] == keyindex)
         return i;
   }

   return -1;
}

const char *input_get_name(int index)
{
   static const char *input_names[] =
   {
      "left", "right", "up", "down",
      "jump", "fire", "pervious wpn", "next wpn",
      "inventory", "map",
      "escape",
      "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12",
      "freeze frame", "frame advance", "debug fly"
   };

   if (index < 0 || index >= INPUT_COUNT)
      return "invalid";

   return input_names[index];
}

void input_set_mappings(int *array)
{
}

/*
void c------------------------------() {}
*/

void input_poll(void)
{
   extern snes_input_state_t snes_input_state_cb;

   for (unsigned i = 0; i < 12; i++)
   {
      int ino = mappings[i];
      if (ino != 0xff) inputs[ino] = snes_input_state_cb(0,
            SNES_DEVICE_JOYPAD, 0, i);
   }
}

// keys that we don't want to send to the console
// even if the console is up.
static int IsNonConsoleKey(int key)
{
	static const int nosend[] = { SDLK_LEFT, SDLK_RIGHT, 0 };

	for(int i=0;nosend[i];i++)
		if (key == nosend[i])
			return true;

	return false;
}


void input_close(void)
{}

static const int buttons[] = { JUMPKEY, FIREKEY, 0 };

bool buttondown(void)
{
   for(int i=0;buttons[i];i++)
   {
      if (inputs[buttons[i]])
         return 1;
   }

   return 0;
}

bool buttonjustpushed(void)
{
   for(int i=0;buttons[i];i++)
   {
      if (inputs[buttons[i]] && !lastinputs[buttons[i]])
         return 1;
   }

   return 0;
}

bool justpushed(int k)
{
   return (inputs[k] && !lastinputs[k]);
}


//Menus.
//I'm on the "Warily doubtful" side of the fence as far as possibility goes
//But I'm going to TRY IT ANYWAY.

#define MENU_BGND   ST7735_BLACK //Background color.
#define MENU_TXT    ST7735_GREEN //Color of text. Going for old-school retro.
#define MENU_CUR    ST7735_GREEN //We'll be using background highlighting.
#define MENU_TITLE  ST7735_GREEN //If we want titles to be a different color
#define MENU_WARN   ST7735_RED   //Warning color in menus.
#define MENU_INFO   ST7735_GREEN //If we want non-selectable info to be a different color.
#define MENU_INVAL  ST7735_GREEN //If we want invalid items to be a different color

void drawMenu(byte menu, byte selection){
  //Draws the "menu" with the current "selection"
  //Each menu will be their own sub-function. This will just call the correct function and apply it.
}

//MENU LIST. TODO, obviously. I feel like text will eat into program memory quickly.
/* 14 rows at 9px height (8px text plus 1px extra padding. Looks nicer.
 * Slideshow speed - Title, Exit, 12 settings (1s intervals?)
 * Mode select - Title, Exit, 4 selections (Mark invalid options?)
 * Stats (Battery voltage, etc) - Title, Exit, no other selections (Just looking!)
 * Brightness - Title, Exit, 10% intervals (10, 20, etc)
 * Main Menu (Select above menus, exit menu, power off)
 * 
 */

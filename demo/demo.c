#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "demo.h"
#include "switches.h"

#define LED BIT6

// axis zero for col, axis 1 for row
short drawPos[2] = {10,10}, controlPos[2] = {10,10};
short velocity[2] = {3,8}, limits[2] = {screenWidth-36, screenHeight-8};

short redrawScreen = 1;
u_int controlFontColor = COLOR_GREEN;

void wdt_c_handler()
{
  static int secCount = 0;

  secCount ++;
  if (secCount >= 25) {/* 10/sec */
    secCount = 0;
    redrawScreen = 1;
  }
}

void main()
{
  P1DIR |= LED;/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();

  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);              /**< GIE (enable interrupts) */

  clearScreen(COLOR_BLACK);
  while (1) {/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      update_shape();
    }
    P1OUT &= ~LED;/* led off */
    or_sr(0x10);/**< CPU OFF */
    P1OUT |= LED;/* led on */
  }
}

void
update_shape()
{
  static unsigned char row = screenHeight / 2, col = screenWidth / 2;
  static char blue = 31, green = 0, red = 31;
  static char currStripeColor = 0; 
  static char presentHeight = (screenWidth * 3) / 4;
  static char openPresent = 0, openPresentStep = 0, stripeStep = 0;
  static char rowStep = 0;

  int leftWallPos[2] = {col/4, row - 25}; 
  int rightWallPos[2] = { limits[0] + 16, row - 25};

  if (sw2_state_down) {
    openPresent = 1;
  }
  if (sw3_state_down) return;
  if (sw4_state_down) {
    openPresent = openPresentStep = rowStep = stripeStep = 0;
    clearScreen(COLOR_BLACK);
    return;
  }
  if(openPresent) {
    fillRectangle(leftWallPos[0], leftWallPos[1], 4, presentHeight, COLOR_BLUE);
    fillRectangle(rightWallPos[0], rightWallPos[1], 4, presentHeight, COLOR_BLUE);

    if(openPresentStep <= presentHeight - 4)
      fillRectangle(leftWallPos[0] + 4, leftWallPos[1], presentHeight - 8, openPresentStep+=3, COLOR_BLACK);

    // Open box
    if(rowStep < 30) {
      open_top(leftWallPos, rightWallPos, presentHeight , rowStep);
      rowStep += 3;
    } else {
      write_helloThere(col, row);
    }
  } else {
    int startingCol = col / 4;
    char color =  (currStripeColor) ? COLOR_BROWN : COLOR_YELLOW;

    if(stripeStep + 16 <= presentHeight){
      fillRectangle(startingCol + stripeStep, row - 25, 16, presentHeight, color);
      stripeStep += 16;
      currStripeColor ^= 1;
    } else {
      fillRectangle(startingCol + (presentHeight - stripeStep), row - 25, 16, presentHeight, color);
    }
    // Draw top
    fillRectangle(leftWallPos[0] - 8, leftWallPos[1] - 16, presentHeight + 16, 16, COLOR_YELLOW);
  }// end of else
}

void open_top(int leftWallPos[], int rightWallPos[], char presentHeight, char rowStep)
{
  fillRectangle(leftWallPos[0] - 8, leftWallPos[1] - rowStep, presentHeight + 16, 3, COLOR_BLACK);
  fillRectangle(leftWallPos[0] - 8, leftWallPos[1] - 16 - rowStep, presentHeight + 16, 16, COLOR_BROWN);
}

void write_helloThere(unsigned char col, unsigned char row)
{
  drawString8x12(15, row - 50, "Hello There", COLOR_WHITE, COLOR_BLACK);
}

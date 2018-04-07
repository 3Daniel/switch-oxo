#include <string.h>
#include <stdio.h>
#include <switch.h>
#include "graphics.h"

uint8_t* g_framebuf;
u32 g_framebuf_width;
int player_number = 1;
int board[9] = {0,0,0,0,0,0,0,0,0};

int CheckBoardTouchPos(uint32_t x, uint32_t y);
void FillBoardBox(uint8_t box, color_t color);
void CheckForWinner();
int CompareBoardPositions(int one, int two, int three);
void ResetGame();

int main(int argc, char **argv)
{
	gfxInitDefault();
	u32 prev_touchcount=0;
	

	while(appletMainLoop())
	{
		g_framebuf = gfxGetFramebuffer(&g_framebuf_width, NULL);
        memset(g_framebuf, 237, gfxGetFramebufferSize());
		
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		if (kDown & KEY_PLUS) break; // break in order to return to hbmenu
		
		touchPosition touch;

        u32 touch_count = hidTouchCount();

        if (touch_count != prev_touchcount)
        {
            prev_touchcount = touch_count;
        }

		//Debug draw all touches
		/*
        for(int i=0; i<touch_count; i++)
        {
            //Read the touch screen coordinates
            hidTouchRead(&touch, i);

			//Debug draw touch points in blue
			DrawBox(touch.px + 30, touch.py - 30, 60, 60, Color(0,0,255,255));
        }
		*/
		if (touch_count > 0){
			//Read the first touch only
			hidTouchRead(&touch, 0);
			
			int boardTouchPos = CheckBoardTouchPos(touch.px, touch.py);
			
			if (boardTouchPos != 0 && board[boardTouchPos - 1] == 0) {
				board[boardTouchPos - 1] = player_number;
				
				CheckForWinner();
				
				if (player_number == 1) {
					player_number = 2;
				} else {
					player_number = 1;
				}
			}
			
			//If the reset button is pressed, reset the board
			if (touch.px >= 100 && touch.px <= 300 && touch.py >= 500 && touch.py <= 600){
				ResetGame();
			}
		}
		
		if (player_number == 1) {
			DrawBox(100, 100, 100, 100, Color(255,0,0,255));
		} else {
			DrawBox(100, 100, 100, 100, Color(0,255,0,255));
		}
		
		for(int i = 0; i < 9; i++){
			if (board[i] == 1) {
				FillBoardBox(i + 1 , Color(255,0,0,255));
			}
			
			if (board[i] == 2) {
				FillBoardBox(i + 1, Color(0,255,0,255));
			}
		}
		
		//Draw reset button
		DrawBox(100, 500, 200, 100, Color(0,0,255,255));
		
		//Draw game board lines
		DrawBox(340, 255, 600, 10, Color(0,0,0,255));
		DrawBox(340, 455, 600, 10, Color(0,0,0,255));
		
		DrawBox(540, 55, 10, 600, Color(0,0,0,255));
		DrawBox(740, 55, 10, 600, Color(0,0,0,255));
		
		gfxFlushBuffers();
		gfxSwapBuffers();
		gfxWaitForVsync();
	}

	gfxExit();
	return 0;
}

int CheckBoardTouchPos(uint32_t x, uint32_t y)
{
	//Check touch is within board area
	if (x < 340 || x >= 940 || y < 50 || y >= 655)
		return 0;
	
	//Column 1
	if (x >= 340 && x < 540) {
		//Row 1
		if (y >= 55 && y < 255)
			return 1;
		
		//Row 2
		if (y >= 255 && y < 455)
			return 4;
		
		//Row 3
		if (y >= 455 && y < 655)
			return 7;
	}
	
	//Column 2
	if (x >= 540 && x < 740) {
		//Row 1
		if (y >= 55 && y < 255)
			return 2;
		
		//Row 2
		if (y >= 255 && y < 455)
			return 5;
		
		//Row 3
		if (y >= 455 && y < 655)
			return 8;
	}
	
	//Column 3
	if (x >= 740 && x < 940) {
		//Row 1
		if (y >= 55 && y < 255)
			return 3;
		
		//Row 2
		if (y >= 255 && y < 455)
			return 6;
		
		//Row 3
		if (y >= 455 && y < 655)
			return 9;
	}
	
	return 1;
}

void FillBoardBox(uint8_t box, color_t color) 
{
	switch(box) {
		case 1 : DrawBox(340, 55, 200, 200, color);
				 break;
		case 2 : DrawBox(540, 55, 200, 200, color);
				 break;
		case 3 : DrawBox(740, 55, 200, 200, color);
				 break;
		case 4 : DrawBox(340, 255, 200, 200, color);
				 break;
		case 5 : DrawBox(540, 255, 200, 200, color);
				 break;
		case 6 : DrawBox(740, 255, 200, 200, color);
				 break;
		case 7 : DrawBox(340, 455, 200, 200, color);
				 break;
		case 8 : DrawBox(540, 455, 200, 200, color);
				 break;
		case 9 : DrawBox(740, 455, 200, 200, color);
				 break;
	}
}

void CheckForWinner()
{
	if (CompareBoardPositions(board[0], board[1], board[2]) ||
		CompareBoardPositions(board[3], board[4], board[5]) ||
		CompareBoardPositions(board[6], board[7], board[8]) ||
		CompareBoardPositions(board[0], board[3], board[6]) ||
		CompareBoardPositions(board[1], board[4], board[7]) ||
		CompareBoardPositions(board[2], board[5], board[8]) ||
		CompareBoardPositions(board[0], board[4], board[8]) ||
		CompareBoardPositions(board[2], board[4], board[6])) 
		{
			for(int i = 0; i < 9; i++){
				if (player_number == 1) {
					board[i] = 1;
				}
				
				if (player_number == 2) {
					board[i] = 2;
				}
			}
		}
}

int CompareBoardPositions(int one, int two, int three)
{
	if (one != 0 && one == two && two == three) {
		return 1;
	} else {
		return 0;
	}
}

void ResetGame()
{
	player_number = 1;
			
	for(int i = 0; i < 9; i++){
		board[i] = 0;
	}
}

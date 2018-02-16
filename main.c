#include <stdio.h>
#include "ui.h"
#include "sweep.h"
static void startbtn_click() {
	resetmap();
	ui_map_clear();
}
/*
static void update_ui(int width, int height) {
	unsigned int w,h,n;
	getconf(&w, &h, &n);
	int i,j,num;
	for(i = 0; i <= h; i++)
		for(j = 0; j <= h; j++) {
			if(getblock(x, y, &num) == 1) {
				ui_map_setblock(x, y, num);
			}

		}
	ui_map_refresh();
}
*/

static void map_click(int x,int y) {
	enum CLICK_RESULT cr;
	unsigned int w,h,n;
	unsigned char num;
	int i,j;
	getconf(&w, &h, &n);
	cr = click(x,y);
	for(i = 0; i < h; i++)
		for(j = 0; j < w; j++) {
			if(getblock(j, i, &num) == 1) {
				ui_map_setblock(j, i, num);
			}

		}
	ui_map_refresh();
	if(cr == BORM) {
		ui_show_message("Boom !");
	} else if(cr == WAN) {
		ui_show_message("You Win.");
	}
	
		
}

int main() {
	ui_init();
	setconf(8, 8, 10);
	ui_map_setsize(8, 8);
	ui_reg_startbtn_click(startbtn_click);
	ui_reg_map_click(map_click);
	ui_show();
	ui_loop();
}

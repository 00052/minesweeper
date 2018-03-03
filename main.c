#include <stdio.h>
#include "ui.h"
#include "sweep.h"
static void startbtn_click() {
	resetmap();
	ui_map_clear();
	ui_map_setenable(1);
	ui_timer_start();
}
static int mapconfig_change(enum UI_MAP_LEVEL level, int w, int h, int n) {
	switch(level) {
	case EASY:
		w = 12, h = 12, n = 10;
		break;
	case GENERAL:
		w = 20, h = 20, n = 80;
		break;
	case DEFAULT:
		w = 30, h = 16, n = 150;
		break;
	case CUSTOM:
		if(w < 5 || w > 40 || h < 5 || h > 40 || n > w*h/2)
			return 0;
	}
	setconf(w, h, n);
	ui_map_setsize(w, h);
	ui_set_nmines(n);
	resetmap();
	ui_timer_stop();
	ui_timer_start();
	ui_map_clear();
	ui_map_setenable(1);
	return 1;
}

static void map_click(enum UI_MOUSEBTN btn, int x,int y) {
	unsigned int w,h,n;
	getconf(&w, &h, &n);
	if(btn == LBUTTON) {
		enum CLICK_RESULT cr;
		unsigned char num;
		int i,j;
		cr = click(x,y);
		for(i = 0; i < h; i++)
			for(j = 0; j < w; j++) {
				if(getblock(j, i, &num) == 1) {
					ui_map_setblock(j, i, num);
				}

			}
		ui_map_refresh();
		if(cr == BORM) {
			ui_map_setenable(0);
			ui_timer_stop();
			ui_show_message("Boom !");
		} else if(cr == WAN) {
			ui_map_setenable(0);
			ui_timer_stop();
			ui_show_message("You Win.");
		}
	} else if(btn == RBUTTON) {
		printf("right button\n");
		if(sweep_rclick(x, y)) {
			switch(getblock(x, y, NULL)) {
			case 0:
				ui_map_setblock(x, y, -1);
				printf("ui_map_setblock(%d, %d, -1)\n", x, y);
				break;
			case 2:
				ui_map_setblock(x, y, -2);
				printf("ui_map_setblock(%d, %d, -2)\n", x, y);
				break;
			}
			ui_set_nmines(n - sweep_nflags());
			ui_map_refresh();
		}
	}
}

int main() {
	ui_init();
	//setconf(8, 8, 6);
	//ui_map_setsize(8, 8);
	mapconfig_change(EASY,0,0,0);
	ui_reg_startbtn_click(startbtn_click);
	ui_reg_map_click(map_click);
	ui_reg_mapconfig_change(mapconfig_change);
	ui_show();
	ui_loop();
}

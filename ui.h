#ifndef UI_H
#define UI_H

enum UI_MAP_LEVEL {
	EASY,
	GENERAL,
	DEFAULT,
	CUSTOM
};

enum UI_MOUSEBTN {
	LBUTTON,
	RBUTTON
};
typedef void(*STARTBTN_CLICK)();
typedef void(*MAP_CLICK)(enum UI_MOUSEBTN btn, int x,int y);
typedef int(*MAPCONFIG_CHANGE)(enum UI_MAP_LEVEL level, int w, int h, int n);

int ui_init(void);
int ui_show(void);
int ui_map_clear();
int ui_map_setsize(unsigned int width,unsigned int height);
int ui_map_setenable(int enable);
int ui_map_setblock(int x,int y,char n);
int ui_map_refresh();
int ui_set_nmines(int n);
//int ui_show_nleftmines(int n);
int ui_reg_startbtn_click(STARTBTN_CLICK callback);
int ui_reg_map_click(MAP_CLICK callback);
int ui_reg_mapconfig_change(MAPCONFIG_CHANGE callback);
int ui_show_message(char *msg);
int ui_loop();
void ui_quit();
int ui_timer_start();
int ui_timer_stop();
int ui_timer_value();
#endif

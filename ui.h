#ifndef UI_H
#define UI_H

enum UI_MAP_LEVEL {
	EASY,
	GENERAL,
	DEFAULT,
	CUSTOM
};
typedef void(*STARTBTN_CLICK)();
typedef void(*MAP_CLICK)(int x,int y);
typedef void(*MAPCONFIG_CHANGE)(enum UI_MAP_LEVEL level, int w, int h, int n);

int ui_init(void);
int ui_show(void);
int ui_map_clear();
int ui_map_setsize(unsigned int width,unsigned int height);
int ui_map_setenable(int enable);
int ui_map_setblock(int x,int y,char n);
int ui_map_refresh();
int ui_mines_count_show(int n);
int ui_mines_rest_show(int n);
int ui_reg_startbtn_click(STARTBTN_CLICK callback);
int ui_reg_map_click(MAP_CLICK callback);
int ui_reg_mapconfig_change(MAPCONFIG_CHANGE callback);
int ui_show_message(char *msg);
int ui_loop();
void ui_quit();
#endif

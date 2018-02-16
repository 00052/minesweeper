#ifndef SWEEP_H
#define SWEEP_H
#include <stddef.h>
enum CLICK_RESULT{
	BORM,
	NOBORM,
	INVALID,
	WAN
};
int setconf(unsigned int w, unsigned int h, unsigned int n);
void getconf(unsigned *w, unsigned int *h, unsigned int *n);
void resetmap();
enum CLICK_RESULT click(unsigned int x, unsigned int y);
int gethidenum();
int getblock(unsigned int x, unsigned int y, unsigned char *);
#endif

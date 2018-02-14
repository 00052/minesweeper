#ifndef SWEEP_H
#define SWEEP_H
#include <stddef.h>
enum CLICK_RESULT{
	BORM,
	NOBORM,
	INVALID,
	WAN
};
int setconf(size_t w, size_t h, unsigned int n);
void getconf(size_t *w, size_t *h, unsigned int *n);
void resetmap();
enum CLICK_RESULT click(unsigned int x, unsigned int y);
int gethidenum();
int getblock(unsigned int x, unsigned int y, unsigned char *);
#endif

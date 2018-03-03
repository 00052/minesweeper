#include "sweep.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#define MAX_WIDTH 40
#define MAX_HEIGHT 40
struct block{
	unsigned char shown : 2;
	char nmines : 6;
};

static size_t width, height, nmines, nflags;
static struct block map[MAX_WIDTH][MAX_HEIGHT];

//static int finished;
static int configured = 0; 
static int nrestblocks;

#define SWAP_S(m,n) { \
	unsigned char __buf[sizeof(m)]; \
	assert(sizeof m == sizeof n); \
	memcpy(__buf,&m,sizeof m); \
	memcpy(&m,&n,sizeof m); \
	memcpy(&n,__buf,sizeof m); \
}

#define SWAP_P(m,n,l) { \
	unsigned char __buf[l]; \
	assert(m != NULL && n!= NULL && l>0); \
	if(m != n) { \
		memcpy(__buf,m,l); \
		memcpy(m,n,l); \
		memcpy(n,__buf,l); \
	} \
}

static void show_map() {
	int i,j;
	printf("DEBUG: show_map {\n");
	for(i=0;i<height;i++) {
		fputs("* ",stdout);
		for(j=0;j<width;j++) {
			if(map[j][i].nmines == 9)
				fputs(" *",stdout);
			else
				printf("%2d",map[j][i].nmines);
			
		}
		putchar('\n');
	}
	puts("}\n");
}

int setconf(size_t w, size_t h, unsigned int n) {
	width = w;
	height = h;
	nmines = n;
	configured = 1;
	resetmap();
	return 0;
} 

void getconf(size_t *w, size_t *h, unsigned int *n) {
	assert(configured == 1);
	assert(w != NULL && h != NULL && n != NULL);
	*w = width;
	*h = height;
	*n = nmines;
}

void resetmap() {
	int i,j,randval,len;
	struct block *p,*q;
	assert(configured == 1);
	srand(time(NULL));

	nrestblocks = width * height; //

	nflags = 0;

	//finished = 0;//not in playing flag
	len = width * height;
	
	for(i=0;i<height;i++) //Clear blocks on map
		for(j=0;j<width;j++) 
			memset(&map[j][i],0,sizeof(map[0][0]));
	//show_map();

	//show_map();
	for(i=0;i<nmines;i++) //Add mines to map
		map[i%width][i/width].nmines = 9; 
	//show_map();
	for(i=0;i<nmines;i++) { //Dislocate mines which added before.
		randval = rand() % len;
		p = &map[i%width][i/width]; 
		q = &map[randval % width][randval/width];
		if(q->nmines == 0)  {
			p->nmines = 0;
			q->nmines = 9;
		} else 
			p->nmines = 9;

		//SWAP_P(p,q,sizeof(struct block));
	}
	//show_map();
	for(i=0; i<height; i++) //generate number
		for(j=0; j<width; j++) {

#define INC_IFNOT_MINE(b){ \
	if((b).nmines != 9) \
		b.nmines ++; \
}

			if(map[j][i].nmines != 9)
				continue;
			if(j>0 && i>0)
				INC_IFNOT_MINE(map[j-1][i-1]);
			if(j>0)
				INC_IFNOT_MINE(map[j-1][i]);

			if(j>0 && i<MAX_HEIGHT-1)
				INC_IFNOT_MINE(map[j-1][i+1]);
			if(i<MAX_HEIGHT-1)
				INC_IFNOT_MINE(map[j][i+1]);
			if(j<MAX_WIDTH-1 && i<MAX_HEIGHT)
				INC_IFNOT_MINE(map[j+1][i+1]);
			if(j<MAX_WIDTH-1)
				INC_IFNOT_MINE(map[j+1][i]);
			if(j<MAX_WIDTH-1 && i>0)
				INC_IFNOT_MINE(map[j+1][i-1]);
			if(i>0)
				INC_IFNOT_MINE(map[j][i-1]);
		}

	//show_map();
			

}
static void auto_sweep(unsigned int x, unsigned int y) {
	if(x < 0 || x > width-1 || y < 0 || y > height-1 || map[x][y].shown == 2)
		return;

	if(map[x][y].shown == 0) {
		if(map[x][y].nmines == 9)
			return;

		map[x][y].shown = 1;
		nrestblocks --;

		if(map[x][y].nmines == 0) {
			//printf("%d %d\n",x,y);
			auto_sweep(x-1,y);
			auto_sweep(x,y-1);
			auto_sweep(x+1,y);
			auto_sweep(x,y+1);

			auto_sweep(x-1,y-1);
			auto_sweep(x+1,y-1);
			auto_sweep(x+1,y+1);
			auto_sweep(x-1,y+1);

		}
	}
}
/**
 * show_all
 * fill all block -> shown with 1
 */
static void show_all_mine() {
	int i,j;
	for(i=0; i<height; i++) 
		for(j=0; j<width; j++) {
			if(map[j][i].nmines == 9)
				map[j][i].shown = 1;
		}

}

enum CLICK_RESULT click(unsigned int x, unsigned int y) {
	assert(configured == 1);
	if(x > width-1 || y > height-1)
		return INVALID;
	if(map[x][y].shown == 1)
		return INVALID;
	//show_map();

	if(map[x][y].nmines == 9) {//borm
		nrestblocks = 0;
		show_all_mine();
		return BORM;
	} else {
		auto_sweep(x,y);
		//show_map();
			
		if(nrestblocks == nmines) {
			//finished = 1;
			return WAN;
		} else
			return NOBORM;
	}
}
/*
int gethidenum() {
	return nrestblocks;
}
*/
int sweep_rclick(unsigned x, unsigned y) {
	if(map[x][y].shown != 1) {
		printf("map.shown == %d\n", map[x][y].shown);
		if(map[x][y].shown == 2) {
			map[x][y].shown = 0;
			nflags --;
			printf("nflags --\n");
		} else {
			map[x][y].shown = 2;
			nflags ++;
			printf("nflags ++\n");
		}
		return 1;//succeed
	}
	return 0;//invalid click
}

int sweep_nflags() {
	//if(nrestblocks != nmines)
	return nflags;
}

/*
 Ret Value: block.shown */
int getblock(unsigned int x, unsigned int y, unsigned char *pval) {
	//assert(pval != NULL);
	assert(x < width && y < height);
	assert(configured == 1);
	if(map[x][y].shown == 1 && pval != NULL) {
		*pval = map[x][y].nmines;
	}
	return map[x][y].shown;
}	



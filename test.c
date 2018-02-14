#include "sweep.h"
#include <stdio.h>

void print_map() {
	int i,j,w,h,n;
	unsigned char nmines;
	
	getconf(&w,&h,&n);
	printf("   ");
	for(i=0;i<h;i++) 
		printf("%2u",i);
	putchar('\n');

	printf("  +");
	for(i=0;i<h;i++) 
		printf("--");
	putchar('\n');
	
	for(i=0;i<h;i++) {
		printf("%2u|",i);
		for(j=0;j<w;j++) {
			if(getblock(j,i,&nmines) == 1)
				if(nmines == 9)
					printf(" *");
				else
					printf("%2u",nmines);
			else
				printf("  ");
		}
		putchar('\n');
	}
	printf("Rest %d blocks.\n\n",gethidenum());

	
}

int main() {
	int x,y;
	enum CLICK_RESULT res;
	setconf(8,8,4);
	resetmap();
	print_map();
	while(1) {
		int n;
		printf("Click x y: ");
		n = scanf("%d%d", &x, &y);
		getchar();
		if(n<2)
			continue;
		res = click(x,y);
		print_map();
		if(res == BORM) {
			printf("Borm !!! Game over\n");
			break;
		} else if(res == WAN) { 
			printf("You Win.\n");
			break;
		} else if(res == INVALID) {
			printf("Invalid location\n");
		}
	}
	
	
	

	return 0;
}

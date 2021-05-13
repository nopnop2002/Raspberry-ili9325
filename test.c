#include <stdio.h>
void main() {
	int width = 100;
	int height = 200;
#ifdef WIDTH
	width = WIDTH;
#endif
#ifdef HEIGHT
	height = HEIGHT;
#endif
	printf("width=%d height=%d\n",width, height);
}


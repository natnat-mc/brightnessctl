#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BRIGHTNESS_PATH (BASE_PATH "/brightness")
#define MAX_BRIGHTNESS_PATH (BASE_PATH "/max_brightness")

int getBrightness(void);
int setBrightness(int value);
int getMaxBrightness(void);
int parseNum(char* str, int* err);

int printUsage(int fail);
int printBrightness(void);
int updateBrightness(char* str);

int maxBrightness=-1;
char buf[128];
char* script;

int main(int argc, char** argv) {
	script=argv[0];
	if(argc>2) return printUsage(1);
	if(argc==2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))) return printUsage(0);
	
	if(argc==1) return printBrightness();
	if(argc==2) return updateBrightness(argv[1]);
	return 1;
}

int printUsage(int fail) {
	FILE* file=fail?stderr:stdout;
	fprintf(file, "Usage:\t%s : gets current brightness\n", script);
	fprintf(file, "Or:\t%s <percent> : set brightness to absolute value\n", script);
	fprintf(file, "Or:\t%s \"+\"|\"-\"<percent> : increase or decrease brightness\n", script);
	return fail;
}

int parseNum(char* str, int* err) {
	int val=0;
	int sgn=1;
	if(*str=='+') {
		str++;
	} else if(*str=='-') {
		sgn=-1;
		str++;
	}
	for(char* c=str; *c; c++) {
		if(*c>='0' && *c<='9') {
			val=val*10+(*c-'0');
		} else {
			*err=1;
			return 0;
		}
	}
	*err=0;
	return val*sgn;
}

int getBrightness(void) {
	int fd=open(BRIGHTNESS_PATH, O_RDONLY);
	if(fd<0) {
		perror("open()");
		return -1;
	}
	if(read(fd, buf, sizeof(buf))<0) {
		perror("read()");
		return -1;
	}
	int brightness=atoi(buf);
	if(getMaxBrightness()<0) return -1;
	close(fd);
	return brightness*100/maxBrightness;
}
int setBrightness(int value) {
	if(getMaxBrightness()<0) return -1;
	sprintf(buf, "%d", value*maxBrightness/100);
	int fd=open(BRIGHTNESS_PATH, O_WRONLY|O_TRUNC);
	if(fd<0) {
		perror("open()");
		return -1;
	}
	if(write(fd, buf, strlen(buf))<0) {
		perror("write()");
		return -1;
	}
	close(fd);
	return 0;
}
int getMaxBrightness(void) {
	int fd=open(MAX_BRIGHTNESS_PATH, O_RDONLY);
	if(fd<0) {
		perror("open()");
		return -1;
	}
	if(read(fd, buf, sizeof(buf))<0) {
		perror("read()");
		return -1;
	}
	maxBrightness=atoi(buf);
	close(fd);
	return maxBrightness;
}

int printBrightness(void) {
	int brightness=getBrightness();
	if(brightness<0) return 1;
	printf("%d\n", brightness);
	return 0;
}
int updateBrightness(char* str) {
	int brightness;
	if(*str=='+' || *str=='-') {
		int err=0;
		int difference=parseNum(str, &err);
		if(err) return printUsage(1);
		brightness=getBrightness();
		if(brightness<0) return 1;
		brightness+=difference;
		if(brightness<0) brightness=0;
		if(brightness>100) brightness=100;
	} else {
		int err=0;
		brightness=parseNum(str, &err);
		if(err || brightness>100) return printUsage(1);
	}
	return setBrightness(brightness)<0;
}

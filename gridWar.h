#ifndef _COLORS_
#define _COLORS_

/* FOREGROUND */
#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define KBOLD "\x1B[1m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define BOLD(x)	"\x1B[1m" x RST
#define UNDL(x)	"\x1B[4m" x RST

#endif	/* _COLORS_ */

#define FAVORITE 'm'
#define TIMESTEP 10000 //in milliseconds

#ifndef _NAMES_
#define _NAMES_
//State names
#define A "pples"
#define B "utter"
#define C "harlie"
#define D "uff"
#define E "dward"
#define F "reddy"
#define G "eorge"
#define H "arry"
#define I "nk"
#define J "ohnny"
#define K "ing"
#define L "ondon"
#define M "onkey"
#define N "uts"
#define O "range"
#define P "udding"
#define Q "ueenie"
#define R "obert"
#define S "ugar"
#define T "ommy"
#define U "ncle"
#define V "inegar"
#define W "illie"
#define X "erxes"
#define Y "ellow"
#define Z "ebra"
#endif

std::string nameList[] = {A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z};




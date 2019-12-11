#ifndef _KEYB_H_
#define _KEYB_H_

#ifdef LINUX

/*****************************************************************************/
/*  GETCH  */
/*****************************************************************************/
int getch(void);


/*****************************************************************************/
/*  KBHIT  */
/*****************************************************************************/
int kbhit();

#endif

int (*getch_)(); /* these pointers will be assigned the appropriate function by funcsetup()*/
int (*kbhit_)(); 

int stdinterm();
void funcsetup();

#endif

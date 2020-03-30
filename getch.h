/*
 * getch.h
 *
 *  Created on: Mar 30, 2020
 *      Author: igor
 */

#ifndef GETCH_H_
#define GETCH_H_

#ifdef __cplusplus
extern "C" {
#endif

void reset_terminal_mode();
void set_conio_terminal_mode();
int kbhit();
int getch();

#ifdef __cplusplus
}
#endif

#endif /* GETCH_H_ */

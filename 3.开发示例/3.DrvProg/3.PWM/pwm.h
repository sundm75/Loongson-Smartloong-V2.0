#ifndef _PWM_H
#define _PWM_H


int pwmset(unsigned int sel, unsigned int value, unsigned int freq) ;
int pwminit(void);
int pwmclose(void);
#endif

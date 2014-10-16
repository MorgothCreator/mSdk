#ifndef _exceptionhandler
#define _exceptionhandler

extern void Entry(void);
extern void UndefInstHandler(void);
extern void SVC_Handler(void);
extern void AbortHandler(void);
extern void IRQHandler(void);
extern void FIQHandler(void);

#endif

#ifndef __UARTSTDIO_H__
#define __UARTSTDIO_H__
//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
extern void UARTStdioInit(unsigned long ulPort);
extern int  UARTgets(char *pcBuf, unsigned long ulLen);
extern unsigned char UARTgetc(void);
extern void UARTprintf(const char *pcString, ...);
extern int UARTwrite(const char *pcBuf, unsigned long ulLen);

#endif // __UARTSTDIO_H__

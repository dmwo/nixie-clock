#include <xc.h>  
  
#define MODE_Button_Clear()  (IOCBFbits.IOCBF7 = 0)
#define DOWN_Button_Clear()  (IOCCFbits.IOCCF3 = 0)
#define UP_Button_Clear()    (IOCCFbits.IOCCF4 = 0)
#define LEFT_Button_Clear()  (IOCCFbits.IOCCF6 = 0)
#define RIGHT_Button_Clear() (IOCCFbits.IOCCF7 = 0)

/* Setting negative edge detection bits */
#define MODE_BUTTON_NEGEDGE  IOCBNbits.IOCBN7
#define DOWN_BUTTON_NEGEDGE  IOCCNbits.IOCCN3
#define UP_BUTTON_NEGEDGE    IOCCNbits.IOCCN4
#define LEFT_BUTTON_NEGEDGE  IOCCNbits.IOCCN6
#define RIGHT_BUTTON_NEGEDGE IOCCNbits.IOCCN7

/* Setting positive edge detection bits */
#define MODE_BUTTON_POSEDGE  IOCBPbits.IOCBP7
#define DOWN_BUTTON_POSEDGE  IOCCPbits.IOCCP3
#define UP_BUTTON_POSEDGE    IOCCPbits.IOCCP4
#define LEFT_BUTTON_POSEDGE  IOCCPbits.IOCCP6
#define RIGHT_BUTTON_POSEDGE IOCCPbits.IOCCP7
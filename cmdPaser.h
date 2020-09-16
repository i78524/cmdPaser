/******************************************************************************/
/* 通用parser                                                                 */
/* Author:YiWen Chen                                                          */
/* Last update: 2017/04/08                                                    */
/*                                                                            */
/******************************************************************************/
#ifndef cmdPaser_V20170407_H
#define cmdPaser_V20170407_H

#include <stdio.h>
#include <string.h>




/******************************** 使用備註 *************************************
(1). 各函式上面都有dfone來決定要不要開log 自行調整
*******************************************************************************/

// Real Stack biffer, very import! =========================================
#define parserBufLen  512
char realPaserbuf[parserBufLen]; // Gobal大家公用的
int  realPaserLen;               // Gobal大家公用的


//==============================================================================




// [與一般parser 有關之函式, 可藉由簡單修正換至其他專案]
void printCMDUnit(char** cmdUnitArrayPtr, int cmdUnitNum);
void analyzeUartCmd(char** cmdUnitArrayPtr, int cmdUnitNum, char* cmdStr);
int cmdPaserContents(char* cmd);
int cmdPaserHeadTail(char* inputData, int inputLen);




#endif  // #ifndef _H

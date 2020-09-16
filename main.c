/******************************************************************************/
/* main進入點                                                                 */
/* Author:YiWen Chen                                                          */
/* Last update: 2020/07/28                                                    */
/*                                                                            */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "cmdPaser.h"

 
int main(int argc, char *argv[])
{ 
  // 輸入範例為這三個指令黏在一起
  // $TSY:1997,12,12,23,50,30,*#
  // $ADD:1,2,3,4,5,*#
  // $SUB:9,8,7,*#
  // 前後還有aaa,ccc與bbb雜訊

  char inStr[128];
  memset(inStr, 0, sizeof(inStr));
  strcpy(inStr, "aaa$TSY:1997,12,12,23,50,30,*#ccc$ADD:1,2,3,4,5,*#$SUB:9,8,7,*#bbb");
  
  printf("[main] inStr = %s \n", inStr);
  cmdPaserHeadTail(inStr, strlen(inStr));
  puts("[main] end");  
}



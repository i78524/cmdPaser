/******************************************************************************/
/* 通用parser                                                                 */
/* Author:YiWen Chen                                                          */
/* Last update: 2017/04/08                                                    */
/*                                                                            */
/******************************************************************************/

#include "cmdPaser.h"






/*******************************************************************************
int printCMDUnit(char** cmdUnitArrayPtr, int cmdUnitNum)
[與parser 有關之函式]
單純印東西 可以用來debug
*******************************************************************************/
void printCMDUnit(char** cmdUnitArrayPtr, int cmdUnitNum)
{
  char** strToken = cmdUnitArrayPtr;

  printf("[printCMDUnit] cmdUnitNum = %d\n[printCMDUnit] ", cmdUnitNum);
  int i = 0;   
  for(i =0; i <= cmdUnitNum; i++){
     printf("st[%d]=%s ", i, strToken[i]);
     //printf("[printCMDUnit] strToken[%d] ptr= %p\n", i, &(strToken[i]));
  } 
  printf("\n");
}



/*******************************************************************************
void analyzeUartCmd(char** cmdUnitArrayPtr, int cmdUnitNum)
[與parser 有關之函式]
被cmdPaserContents()使用著

cmdUnitArrayPtr 是一個string陣列A的指標
cmdUnitNum 是string陣列A 的 內容數量
＊＊＊從這裡開始 跟parser無關
*******************************************************************************/
#define aUPrint  printf
//#define aUPrint  //
void analyzeUartCmd(char** cmdUnitArrayPtr, int cmdUnitNum, char* cmdStr)
{
  char** strToken = cmdUnitArrayPtr;


  // 在這邊的實驗 幾乎確認了%s 配合 strToken[i]可以正確取得cmdUnit
  // 可以用以下實驗來了解指標結構
  aUPrint("[analyzeUartCmd] cmdUnitNum = %d\n", cmdUnitNum);
  int i = 0;   
  for(i =0; i <= cmdUnitNum; i++){
     printf("[analyzeUartCmd] strToken[%d] S= %s\n", i, strToken[i]);
     printf("[analyzeUartCmd] strToken[%d] ptr= %p\n", i, &(strToken[i]));
  }


  if( strncmp( strToken[0], "TSY", 3) == 0) {
      aUPrint("[analyzeUartCmd] is TSY Case\n");

  } else if( strncmp( strToken[0], "ADD", 3) == 0) {
      aUPrint("[analyzeUartCmd] is ADD Case\n");
      
  } else if( strncmp( strToken[0], "SUB", 3) == 0) {
      aUPrint("[analyzeUartCmd] is SUB Case\n");
      
  } else {
      aUPrint("[analyzeUartCmd] Else..Case..\n");
  }  
}




/*******************************************************************************
int cmdPaserContents(char* cmd)
[與parser 有關之函式]
被cmdPaserHeadTail()呼叫使用著

return -1:The str[0] is not $! Do Nothing
return 0: 正常結束

這裡分析cmd 功能，根據不同功能，要丟到不同的函式繼續作分析以及後續處理
若要修改成其他的parser 最主要修改分析cmd的字串就好

變數char* cmdPoint[10] 與 int pointTotal會是傳給下一個指令的重要資訊
*******************************************************************************/
//#define cPCPrint  printf
#define cPCPrint  //
int cmdPaserContents(char* cmd)
{
  // example: $TSY:2015,12,12,23,50,30,*#
  // 裡面有          年  月 日 時 分 秒
  int cmdLengthMax = 150;
  char* cmdPoint[15]; // 這些會存放cmd中參數字串的指標（這個指標是char* cmd的延伸），根據','來切, 參數是否只有15個自行作加減  (很重要)
  int pointTotal = 0;  //pint的個數  (很重要)
  char* checkHeadstr = "$"; //檢查頭  (很重要)
  char* checkTailstr = "*#"; //檢查最後尾巴  (很重要)
  char  checkCutChar = ','; //檢查中間切斷點  (很重要)
  char cmdCpoyComplete[cmdLengthMax+ 1 ]; //複製一個完整的cmd指令，用來預備是否有其他運用 我們偷偷規定指令長度最長是150
        
  int cmdLen = 0; // input cmd的 長度
  int i = 0;      // for loop used  
  
  cPCPrint("[cmdPaserContents] cmd=%s\n", cmd); 
  if( strncmp( cmd, checkHeadstr, strlen(checkHeadstr) ) == 0) { // 至少再檢查一次checkHeadstr

      cmdLen = strlen(cmd);
      cPCPrint("[cmdPaserContents] cmd = %s, cmdLen = %d\n", cmd, cmdLen);
      
      if(cmdLen <= cmdLengthMax)
      {
          //完整複製一份指令
          memset(cmdCpoyComplete, 0, cmdLengthMax + 1);
          strcpy(cmdCpoyComplete, cmd);

          
          // 先處理命令 將他丟到cmdPoint[pointTotal]
          // 會這樣做，是因為我知道指令長成這樣:$TSY:2015,12,12,23,50,30,*#
          cmdPoint[pointTotal] = cmd + 1;
          *(cmd + 4) = 0; //把:號 改成0
          pointTotal++;

         // 處理完命令之後，處理這個指令的參數，用checkCutChar來作分割
          cmdPoint[pointTotal] = cmd + 5;
          for(i = 5; i < cmdLen; i++) {
            if( *(cmd + i) == checkCutChar ) {
              pointTotal++;//有找到才++
              cmdPoint[pointTotal] = (cmd + i) + 1; //逗號的下一個 就應該是參數的指標開頭
              *(cmd + i) = 0; //把逗號 改成0
            }
          }
          
          // 印出來分割的結果
          cPCPrint("[cmdPaserContents] the cmd have %d+1 point!\n", pointTotal);  
          for(i = 0; i <= pointTotal; i++) { //注意 這裡是使用<=
            cPCPrint("[cmdPaserContents] cmdPoint[%d]=%s\n", i, cmdPoint[i]);
            cPCPrint("[cmdPaserContents] cmdPoint[%d]=%p\n", i, cmdPoint + i);
          }
          
          
          // 在這裡 我們檢查最後一個pint 是否符合 cmd tail=="*#"
          if( strncmp( cmdPoint[pointTotal], checkTailstr, strlen(checkTailstr) ) == 0) {        
            // 在這裡 我們幾乎確定:
            //  cmdPoint[0]是命令
            //  cmdPoint[1] ~ cmdPoint[pointTotal-1] 是合法的參數形式
            //  cmdPoint[pointTotal] 是存放checkTailstr
            //  pointTotal 是參數個數（這個數量是從0開始算，包含命令以及checkTailstr）
            // 接下來將"命令" 以及他的"參數" "參數個數"丟到其他函式作確認
            cPCPrint("[cmdPaserContents] cmdPoint[pointTotal] is %s! Good\n", checkTailstr);        
           analyzeUartCmd(cmdPoint, pointTotal, cmdCpoyComplete);
            
            
            return 0;    
          } else {
            cPCPrint("[cmdPaserContents][error] cmdPoint[pointTotal] is not %s! Do Nothing\n", checkTailstr);
            // example: $*#
            // example: $,*# 
            return -1;    
          }
      } else {
          printf("[cmdPaserContents][error] cmdLen=%d > cmdLengthMax=%d, is error!!!\n", cmdLen, cmdLengthMax);
      }
  } else {
    printf("[cmdPaserContents][error] The str[0] is not $! Do Nothing\n");
    return -1;
  }  
}



/*******************************************************************************
int cmdPaserHeadTail(char* inputData, int inputLen)
[與parser 有關之函式]

return -1:竟然找不到頭, gobal realPaserbuf/realPaserLen 全部要刪掉
return 1: 找到頭 找不到尾 正常結束 到函式外面繼續接收 不做任何事情 算好事
return 0: 正常結束

若要修改成其他的parser 最主要修改分析頭與尾
*******************************************************************************/
//#define cPHTPrint  printf
#define cPHTPrint  // //這樣用就可以消除printf
int cmdPaserHeadTail(char* inputData, int inputLen)
{

  char tmpPaserbuf[parserBufLen];
  int  headPos;
  int  tailPos;
  
  // (0). 先input丟到最後尾巴
  memcpy(realPaserbuf+realPaserLen, inputData, inputLen);
  realPaserLen = realPaserLen + inputLen;
  cPHTPrint("[cmdPaserHeadTail] in (0), the realPaserLen = %d\n", realPaserLen);
  
  while(1)
  {
      // (1). 必要的初始化
      memset(tmpPaserbuf, 0, parserBufLen);
      headPos=0;
      tailPos=0;
      
      // (2). 找頭 跟 尾（通用時需要在此微調）
      int i = 0;
      for(i = 0; i < realPaserLen; i++) {
        if(realPaserbuf[i] == '$') {
          break;
        }
      }
      if(i < realPaserLen) {
        headPos = i;  
      } else {
        headPos = 16888; //用這個表示沒找到頭
      }

      for(i = 0; i < realPaserLen - 1; i++) { //因為*# 所以是-1
        if(realPaserbuf[i] == '*') {
          if(realPaserbuf[i+1] == '#') {
            break;
          }
        }
      }
      if(i < realPaserLen - 1) { //因為*# 所以是-1
        tailPos = i + 1; //因為*# 所以需要+1
      } else {
        tailPos = 16888; //用這個表示沒找到尾
      }
      
      
      cPHTPrint("[cmdPaserHeadTail] >>>>> headPos=%d, tailPos=%d\n", headPos, tailPos);
      
      // (3). 找到 頭 跟 尾 的位置，作相對應處理
      if(headPos == 16888) { // 竟然找不到頭, 全部要刪掉
        memset(realPaserbuf, 0, parserBufLen);
        memset(tmpPaserbuf, 0, parserBufLen);
        realPaserLen = 0;
        headPos = 0;
        tailPos = 0;
        return -1;   
      }
      
      if(headPos != 16888 && tailPos == 16888) { //找到頭 找不到尾 繼續接收 不做任何事情 算好事
        return 1;
      }
      
      if(headPos != 16888 && tailPos != 16888) { // 有找到 要開始作處理
          if(tailPos > headPos) { //尾 一定要比 頭 大
            // 確定字串
            memcpy(tmpPaserbuf, realPaserbuf + headPos, tailPos - headPos + 1); 
           
            
            // 處理字串 作相對應處理
            cmdPaserContents(tmpPaserbuf);// 用tmp可以確保後面是空的 不會有字元沾粘問題
            
            // 處理realPaserbuf往前面移動
            realPaserLen = realPaserLen - (tailPos /*- headPos*/ + 1);
            cPHTPrint("(0). (tailPos - headPos + 1)=%d, realPaserLen=%d\n", (tailPos - headPos + 1), realPaserLen);        
            
            memset(tmpPaserbuf, 0, parserBufLen); //很快的, tmpPaserbuf馬上就被清空
            cPHTPrint("(1). tmpPaserbuf=%s\n", tmpPaserbuf);
            memcpy(tmpPaserbuf, realPaserbuf + tailPos/* - headPos*/ + 1, parserBufLen - (tailPos /*- headPos*/ + 1)); // 從尾巴的下一個 往前面移/ 全部長度-想要消失不見的長度
            cPHTPrint("(2). tmpPaserbuf=%s\n", tmpPaserbuf);
            memset(realPaserbuf, 0, parserBufLen);
            memcpy(realPaserbuf, tmpPaserbuf, parserBufLen);
            cPHTPrint("(3). realPaserbuf=%s\n", realPaserbuf); 
            
            //備註：為什麼字串偏移時不必參考headPos?
            //因為headPos如果不為0，代表headPos之前的字元一定都是垃圾
            //因此可以放心的直接刪除掉
            //基本上，沒有垃圾的字串，其headPos最後都應該在shift的過程中會被調整為0
            //舉例：12345$abcde*#$QWE
            //這個字串串中的12345都是垃圾，當我們取出$abcde*#時
            //可以放心地將12345$abcde*#全部刪除掉，只留下$QWE 
                   
          }
      }
      
      // 如果字串還有遺留,應該檢查是不是有其他指令，反之離開迴圈
      if(realPaserLen == 0) {
        break;
      }
  } // end of while
  
  return 0;
}


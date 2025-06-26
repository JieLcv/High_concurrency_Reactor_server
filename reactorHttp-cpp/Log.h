#pragma once
#include <stdarg.h>
#include <stdio.h>

#define  DEBUG   1  

#if DEBUG
/*
*  如果不加 do ... while(0) 在进行条件判断的时候(只有一句话), 省略了{}, 就会出现语法错误
*  if 
*     xxxxx
*  else
*     xxxxx
*  宏被替换之后, 在 else 前面会出现一个 ;  --> 语法错误
*/
#define LOG(type, fmt, args...)  \
  do{\
    printf("%s: %s@%s, line: %d\n***LogInfo[", type, __FILE__, __FUNCTION__, __LINE__);\
    printf(fmt, ##args);\
    printf("]\n\n");\
  }while(0)
// ##args在进行宏参数传递的时候，不会把args展开，而是原封不动传递，即连接成一个字符串
// #args代表将args转换为字符串
#define Debug(fmt, args...) LOG("DEBUG", fmt, ##args)
#define Error(fmt, args...) do{LOG("ERROR", fmt, ##args);exit(0);}while(0)
#else
#define LOG(fmt, args...)  
#define Debug(fmt, args...)
#define Error(fmt, args...)
#endif



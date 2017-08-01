///////////////////////////////////
/// @file Encode.h
/// @brief 解码程序
/// @author: Ying Jia
/// @date 2017.6.10
///////////////////////////////////
#ifndef ENCODE_H
#define ENCODE_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>  
#include <time.h>  
#include <string.h>

int m_iSeed_Unit = 13;
int m_iUnit = 6;//大于2的偶数
int BINSEQ[15][4] =
{ 0, 0, 0, 1,
0, 0, 1, 0,
0, 0, 1, 1,
0, 1, 0, 0,
0, 1, 0, 1,
0, 1, 1, 0,
0, 1, 1, 1,
1, 0, 0, 0,
1, 0, 0, 1,
1, 0, 1, 0,
1, 0, 1, 1,
1, 1, 0, 0,
1, 1, 0, 1,
1, 1, 1, 0,
1, 1, 1, 1 };
int m_iWindow = 200;//中心窗口大小
int m_iMaxp = 2000;//图片最大尺寸
int m_iMinp = 40;//图片最小尺寸
int m_iDPPx = 2;//像素点大小
int m_iDPPy = 2;
int m_iMsg_length = 162;//信息位长度

char table[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l','m',
'n','o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
'A', 'B', 'C', 'D', 'E', 'F','G', 'H', 'I', 'J', 'K', 'L', 'M',
'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\n' };

/** 图像矩阵生成函数 **/
int fnImage_fill(char *adMsg_num, char *adMsg_info, int iImage_width, int iImage_height, int **pPicData);
/**基本编码图像的生成以及掩码的添加**/
void fnCoverSeed(int **adImage_seed, int iMarks, int iMarks1,int **adImage_fseed);
/**crc校验码生成函数**/
int fnCrc32(const int * pSrc, int nSize);
/**将十进制的数转换成二进制比特流，生成crc校验码函数的补充函数**/
int *fnDec2Bin(int iNum, int *pResult, int iIdx);
/**将字符串转化成二进制比特流**/
void fnStr2Bin(char *pMessage,int *iStream);

#endif

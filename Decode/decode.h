////////////////////////////////////////////////
/// COPYRIGHT NOTICE
/// Copyright (c) 2017,USTC
/// All rights reserved.
///
/// @file Decode.h
/// @brief Decode.c文件的头文件
///
/// @version 1.3
/// @author: Ying Jia
/// @date 2017.6.10
///
///////////////////////////////////////////////
#ifndef DECODE_H
#define DECODE_H
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include <malloc.h>
#include <time.h>
#include <conio.h>
#include<Windows.h>
#define SIZE1 1080               ///  解码程序输入的数据矩阵的第一维
#define SIZE2 1920                ///  解码程序输入的数据矩阵的第二维
#define WINDOW 700                ///  中心窗口大小
#define MAXP 2000               ///  最大黑点数量
#define MINP 40                  ///  最小黑点数量
#define SEED_unit 13                  ///  可提供338个点位，除去6个标识点、11个掩膜标志点外，共321个信息点
#define UNIT 6                        ///  必须是大于2的偶数
#define STACKBUF 20000                ///  STACKBUF=(WINDOW*WINDOW)/2
#define M_PI 3.14159265358979323846     ///圆周率
/** @brief  解码程序*/
void fnDotDecode(int **adCenterblock, int **adInimage, int iInimage_height, int iInimage_width, int *aiSecmsg, int *iErrcode, int iWindowx, int iWindowy);
/** @brief 求黑点数量、中心坐标、面积、排序*/
void fnPickSortDots(int **adCenterblock, const int iThreshold, double *adPointsy, double *adPointsx, int *aiPointsind, int *iErrcode, int *iPoints, int *dPointsize);
/** @brief 求某个黑点的横纵坐标和面积*/
void fnConnects(int **adCenterblock, int i, int j, const int iThreshold, double *y, double *x, double *dWeight);
/** @brief 每点求下方最临近点，计算偏移角度和距离*/
void fnNearAng(const double *adPointsy, const double *adPointsx, const int *aiPointsind, int iPoints, double *pdDis, double *pdAng);
/** @brief 以最集中的作为倾斜角、间隔（编码矩阵中每点的大小）*/
void fnAngScale(const double *pdDis, const double *pdAng, int iPoints, int *iErrcode, double *iXang, double *iXscale, double *iYscale);
/** @brief 求角度之间距离*/
double fnDistant(double a, double b);
/** @brief 寻找矩阵起始标志点及方向，旋转矩阵到正方向*/
void fnFindStart(int **aiMatr, int *iSty, int *iStx);
/** @brief 按顺序读取矩阵*/
void fnUnCoverSeed(int **aiMatr, int iSty, int iStx, int *aiSecmsg);
/** @brief 求模*/
int fnMod(int a, int b);
/** @brief 与整数非常接近时，四舍五入*/
double fnRound(double a);
/** @brief 01比特流转换成字符*/
char *fnBit2char(int *aMsg,char *pMsg,int *iErrcode);
/** @brief CRC校验码生成函数*/
int fnCrc32Decode(const int * pSrc, int nSize);
/** @brief 将十进制的数转换成二进制比特流，生成crc校验码函数的补充函数*/
int *fndec2bin(int iNum, int *pResult, int i);
#endif

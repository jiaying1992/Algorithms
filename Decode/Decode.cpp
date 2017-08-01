////////////////////////////////////////////////
/// @file Decode.c
/// @brief 解码程序
/// @author: Ying Jia
/// @date 2017.6.10
//////////////////////////////////////////////////

//#include "stdafx.h"
#if defined(_MSC_VER) && defined(_DEBUG)
#include <stdlib.h>
#include <crtdbg.h>
#include <cstring>
#endif

#include "Decode.h"

int m_Decode_iMsg_length= 162;          ///  解码出的01比特流的有效位数
char m_acTable[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '\n' };   /// 01比特流转换成字符的查询表
/**
*
* @param
* @return 输出解码成功或者失败，成功则输出字符，失败则输出失败原因
* @note 该版本中输入图像矩阵大小为600*600，若有不同，请修改头文件中SIZE1和SIZE2的大小
*/
int  main()
{
	// check memory leak in VC++
#if defined(_MSC_VER) && defined(_DEBUG)
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpFlag);
	//_CrtSetBreakAlloc(793);
#endif

	int i=0,j=0,m=0,n=0;                            /// 用于for循环的控制变量
    int iErrcode=0;                                 /// 错误标志
	int iInimage_height,iInimage_width; /// 输入图像的高和宽
	int iWindowx, iWindowy;
	 int **aiInimage;
    aiInimage = (int **)malloc(SIZE1 * sizeof(int *));
    for(i=0; i<SIZE1; i++) {
        aiInimage[i] = (int * )malloc(SIZE2 * sizeof(int));
		memset(aiInimage[i], 0,sizeof(int) * SIZE2);
	}
    
	//int aiSecmsg[321]={0};                          /// 解出的全部321位码流信息    
    int *aiSecmsg;
    aiSecmsg = (int*)malloc(sizeof(int) * 321);  
	memset(aiSecmsg, 0, sizeof(int) * 321);
	
    int aiSecmsg1[194]={0};                         /// 解出的有效194位码流信息
    
	//int aiInimage[SIZE1][SIZE2]={0};                /// 输入图像
    //int **aiInimage1;
    //aiInimage1 = (int **)malloc(SIZE1 * sizeof(int *));
    //for(i=0; i<SIZE1; i++) {
    //    aiInimage1[i] = (int * )malloc(SIZE2 * sizeof(int));
	//	memset(aiInimage1[i], 0,sizeof(int) * SIZE2);
	//}
    
	//int aiCenterblock[WINDOW][WINDOW]={0};          /// 输入图像中间WINDOW*WINDOW大小的区域
    int **aiCenterblock;
    aiCenterblock = (int **)malloc(WINDOW * sizeof(int *));
	for(i=0; i<WINDOW; i++){
		aiCenterblock[i] = (int * )malloc(WINDOW * sizeof(int));   
		memset(aiCenterblock[i], 0,sizeof(int) * WINDOW);
	}
	int **aiCenterblocktemp;
	aiCenterblocktemp = (int **)malloc(WINDOW * sizeof(int *));
	for(i=0; i<WINDOW; i++){
		aiCenterblocktemp[i] = (int * )malloc(WINDOW * sizeof(int));   
		memset(aiCenterblocktemp[i], 0,sizeof(int) * WINDOW);
	}
        
    
	char *pcMsg;                                    /// 存放解码出的字符
	FILE *fp;                                       /// 输入图像的文件
    char pcMsg_char[28] = { '\0' };
    /// 读取输入图像矩阵
	
	if((fp=fopen("IMG_7355.txt","r"))==NULL){                          /// 根据输入不同修改txt文件的名称
        printf("\nCannot open file strike any key exit!\n");        
		goto FREE_MEM;        
	}
	/*for(i=0; i<SIZE1; i++){
		for(j=0; j<SIZE2; j++){
			aiInimage[i][j] = fgetc(fp) - '0';
		}
	}*/
	for(i=0; i<SIZE1; i++){
        for(j=0; j<SIZE2; j++){
            fscanf(fp, "%d", &aiInimage[i][j]); 
        }
	}
	fclose(fp);
	iInimage_height=SIZE1;
	iInimage_width=SIZE2;
	if((iInimage_height<WINDOW) || (iInimage_width<WINDOW)){       /// 检查输入参数合法性
		iErrcode=101;   ///图像尺寸过小
		goto Label;
	}
	for(i=((iInimage_height/2)-(WINDOW/2));i<((iInimage_height/2)+(WINDOW/2));i++){
        for(j=((iInimage_width/2)-(WINDOW/2));j<(iInimage_width/2+(WINDOW/2));j++){
           aiCenterblock[m][n]=aiInimage[i][j];                   /// 取输入图像的中间部分组成WINDOW*WINDOW大小的aiCenterblock
		   aiCenterblocktemp[m][n]=aiInimage[i][j];
            n++;
        }
        m++;
        n=0;
	}
	/** @brief  解码程序*/
	long t1=GetTickCount();
	for( iWindowy=WINDOW/8*2; iWindowy<=WINDOW/8*6; iWindowy+=WINDOW/8*2)
	{
		for( iWindowx=WINDOW/8*2; iWindowx<=WINDOW/8*6; iWindowx+=WINDOW/8*2)
		{
			for(int i=0; i<WINDOW; i++)
			{
				for(int j=0; j<WINDOW; j++)
				{
					aiCenterblock[i][j]=aiCenterblocktemp[i][j];
				}
			}
	     fnDotDecode(aiCenterblock, aiInimage, iInimage_height, iInimage_width, aiSecmsg, &iErrcode, iWindowx, iWindowy);
	/// 取解码比特流的前194位有效位
	for (i=0; i<194; i++){
        aiSecmsg1[i]=aiSecmsg[i];
	}
Label:	
	switch(iErrcode) {
        case 0:
            printf("解码（01比特流）成功\n");
            for(i=0; i<321; i++){
                 printf("%d",aiSecmsg[i]);
	        }
	        printf("\n");
	        /** @brief 01比特流转换成字符*/
            pcMsg = fnBit2char(aiSecmsg1, pcMsg_char, &iErrcode);
	        if(iErrcode==0){
                   printf("解码（信息）成功\n");
	               printf("%s", pcMsg);
				   printf("\n");
                   free(aiSecmsg);
	               for (int i = 0; i < SIZE1; i++) free(aiInimage[i]);
	               free(aiInimage);
	              for (int i = 0; i < WINDOW; i++) free(aiCenterblock[i]);
	              free(aiCenterblock);
	              system("pause");
	              return 0;
	        }
	        else{
                printf("解码(信息）失败，校验出错\n");
				continue;
	        }
            break;
        case 101: 
            printf("解码（01比特流）失败，输入图像尺寸太小\n"); 
            //break;
			continue;
        case 102:
            printf("解码（01比特流）失败，黑点数量太少\n");
            //break;
			continue;
        case 103:
            printf("解码（01比特流）失败，黑点数量太多\n");
            //break;
			continue;
        case 104:
            printf("解码（01比特流）失败，黑点未在四个方向上集中\n");
            //break;
			continue;
       case 105:
            printf("解码（01比特流）失败，未找到x-y方向\n");
            //break;
			continue;
        case 106:
            printf("解码（01比特流）失败，读取数据矩阵时，超出图像边界\n");
            //break;
			continue;
        default:
            printf("解码（01比特流）失败，未能定义起始标志\n");
			continue;
    }
		}
	}
	long t2=GetTickCount();
	printf("%d", t2-t1);
FREE_MEM: 
      
    free(aiSecmsg);
	
	for (int i = 0; i < SIZE1; i++) free(aiInimage[i]);
	free(aiInimage);

	for (int i = 0; i < WINDOW; i++) free(aiCenterblock[i]);
	free(aiCenterblock);

	system("pause");
	return 0;
}

/**
* 解码程序
* @param[in] aiCenterblock[][] 取输入图像的中心方块
* @param[in] aiInimage[][]  输入图像
* @param[in] iInimage_height 输入图像的高
* @param[in] iInimage_width 输入图像的宽
* @param[in] aiSecsg[] 解码得到的码流
* @param[in] iErrcode 错误标志
* @return 无返回值
* @note
*/
void fnDotDecode(int **aiCenterblock, int **aiInimage, int iInimage_height, int iInimage_width, int *aiSecmsg, int *iErrcode,int iWindowx, int iWindowy)
{
	int i,j;                               /// 用于for循环的控制变量
	int iTemp1,iTemp2,iTemp3,iCount;       /// 中间变量
    int iThreshold;                        /// 黑白划分阈值
	int iPoints;                           /// 黑点的数量
	int iPointsize;                        /// 点的大小
	int iSty,iStx;                         /// 起始标志点的坐标
	int iWindow=WINDOW/2;                  /// 窗口
	int iSEED_size=SEED_unit*UNIT;         /// 与编码相关的参数
	int iHalfunit=(int)(UNIT/2);           /// 与编码相关的参数
	//int aiPointsind[MAXP];                 /// 黑点按照y坐标排序
    int *aiPointsind;
    aiPointsind = (int*)malloc(sizeof(int)*MAXP);
    
	//int iGrayhist[256]={0};                /// 灰度直方图
    int *iGrayhist;
    iGrayhist = (int*)malloc(sizeof(int) * 256);
	memset(iGrayhist, 0, sizeof(int) * 256);
    //int aiMatr[SEED_unit*2][SEED_unit*2];  /// 数据矩阵
    int **aiMatr;
    aiMatr = (int **)malloc(SEED_unit*2 * sizeof(int *));
    for(i=0; i<SEED_unit*2; i++)
        aiMatr[i] = (int*)malloc(SEED_unit*2 * sizeof(int));
        
	double dTemp1,dTemp2;                  /// 中间变量
	double dAngsin,dAngcos;                /// 倾斜角的正弦值、余弦值
	double dXscale,dYscale,dXang;          /// 横向间隔、纵向间隔、倾斜角
	double dCenterx=iInimage_width/2;      /// 读取数据矩阵的中心点的X坐标
	double dCentery=iInimage_height/2;     /// 读取数据矩阵的中心点的y坐标
	//double adPointsy[MAXP]={0.0};          /// 黑点的y坐标
	//double adPointsx[MAXP]={0.0};          /// 黑点的x坐标
	//double adDis[MAXP]={0.0};              /// 每个黑点与其下方最邻近黑点之间的距离
	//double adAng[MAXP]={0.0};              /// 每个黑点与其下方最邻近黑点之间的角度
    double *adPointsy, *adPointsx, *adDis, *adAng;
    adPointsy = (double*)malloc(sizeof(double)*MAXP);
	memset(adPointsy, 0, sizeof(double) * MAXP);
    adPointsx = (double*)malloc(sizeof(double)*MAXP);
	memset(adPointsx, 0, sizeof(double) * MAXP);
    adDis = (double*)malloc(sizeof(double)*MAXP);
	memset(adDis, 0, sizeof(double) * MAXP);
    adAng = (double*)malloc(sizeof(double)*MAXP);
	memset(adAng, 0, sizeof(double) * MAXP);

    
	/// 在图像中心WINDOW区域内，确定一个中心点、倾斜角度、间隔、点大小
	for(i=0; i<WINDOW; i++){
		for(j=0; j<WINDOW; j++){
			iGrayhist[(int)aiCenterblock[i][j]]++;
		}
	}
	for(i=0; i<MAXP; i++){
        aiPointsind[i]=-1;
	}
	iTemp1=0;
	for(i=0; i<256; i++){
		iTemp1+=iGrayhist[i];
		if(iTemp1>(WINDOW*WINDOW*2/UNIT/UNIT)){
			iThreshold=i;     /// 取中心区域，求黑白划分阈值threshold
			break;
		}
	}
    //iThreshold=1;
	/// 求各黑点数量points、中心pointsy\pointsx、面积pointsw、排序pointsind
	fnPickSortDots(aiCenterblock,iThreshold,adPointsy,adPointsx,aiPointsind,iErrcode,&iPoints,&iPointsize);
	if( *iErrcode!=0){
		return;
	}
	/// 每点求下方最临近点，计算偏移角度
	fnNearAng(adPointsy,adPointsx,aiPointsind,iPoints, adDis, adAng);
	for(i=0;i<MAXP;i++){
        for(j=0;j<MAXP;j++){
            adAng[i]=fnRound(adAng[i]);
        }
	}
	/// 以最集中的作为倾斜角、间隔（编码矩阵中每点的大小）
	fnAngScale(adDis, adAng, iPoints,iErrcode,&dXang,&dXscale,&dYscale);
	if(*iErrcode!=0){
		return;
	}
	dXscale/=UNIT;
	dYscale/=UNIT;
	/// 以最接近图像中心的黑点作为数据矩阵中心点
	dTemp1=WINDOW*WINDOW;
	for(i=0; i<iPoints; i++){
		dTemp2=(adPointsy[i]+1-iWindowy)*(adPointsy[i]+1-iWindowy)+(adPointsx[i]+1-iWindowx)*(adPointsx[i]+1-iWindowx);
		if(dTemp2<dTemp1){
			dTemp1=dTemp2;
			j=i;
		}
	}
	dCentery=dCentery-iWindow+adPointsy[j]+1;
	dCenterx=dCenterx-iWindow+adPointsx[j]+1;              /// 读取数据矩阵的中心点
	/// 读取矩阵数据
	/// 从中心点出发，根据倾斜角和间隔，计算矩阵点的位置，判断黑白，填充矩阵
	for(i=0; i<SEED_unit*2; i++){
		for(j=0; j<SEED_unit*2; j++){
			aiMatr[i][j]=1;
		}
	}
	dAngsin=sin((double)dXang/180*M_PI);
	dAngcos=cos((double)dXang/180*M_PI);
	for(i=1; i<=iSEED_size; i+=iHalfunit){
		for(j=1; j<=iSEED_size; j+=iHalfunit){
			iTemp3=i-iSEED_size/2-1;
			iTemp2=j-iSEED_size/2-1;
			dTemp1=(int)(dCentery+iTemp2*dXscale*dAngsin+iTemp3*dYscale*dAngcos+0.5);
			dTemp2=(int)(dCenterx+iTemp2*dXscale*dAngcos-iTemp3*dYscale*dAngsin+0.5);
			iCount=0;   /// 在四倍点大小的范围内，黑色像素的数量超过点大小的一半，即判为黑点
			for(iTemp3=(int)(dTemp1-iPointsize+0.5); iTemp3<=(int)(dTemp1+iPointsize+0.5)-1; iTemp3++){
				for(iTemp2=(int)(dTemp2-iPointsize+0.5); iTemp2<=(int)(dTemp2+iPointsize+0.5)-1; iTemp2++){
					if((iTemp2>=1) && (iTemp2<=iInimage_width) && (iTemp3>=1) &&(iTemp3<=iInimage_height)){
						if (aiInimage[iTemp3 - 1][iTemp2 - 1] <iThreshold){
							iCount+=1;
						}
					}
                    else{
                        *iErrcode=106;   /// 读取数据矩阵时发生超出图像边界事件
                        return;
                    }
				}
			}
			if(iCount>iPointsize*iPointsize/2){
				aiMatr[(i-1)/iHalfunit][(j-1)/iHalfunit]=0;
			}
			else{
				aiMatr[(i-1)/iHalfunit][(j-1)/iHalfunit]=1;
			}
			if((dTemp1>=1) && (dTemp1<=iInimage_width) && (dTemp2>=1) && (dTemp2<=iInimage_height)){
				aiInimage[(int)dTemp1 - 1][(int)dTemp2 - 1] = 255;
			}
		}

    }
	/// 寻找矩阵起始标志点及方向，旋转矩阵到正方向
    fnFindStart(aiMatr,&iSty,&iStx);
    if(iSty==0){
	     *iErrcode=107;   /// 未能定位起始标志
		 return;
    }
	/// 按顺序读取矩阵
	fnUnCoverSeed(aiMatr,iSty,iStx,aiSecmsg);

	free(aiPointsind);
	free(iGrayhist);

	for (i = 0; i < SEED_unit*2; i++) free(aiMatr[i]);
	free(aiMatr);
	
	free(adPointsy);
	free(adPointsx);
	free(adDis);
	free(adAng);
}


/**
* 求黑点数量points、中心pointsy\pointsx、面积pointsw、排序pointsind
* @param[in] aiCenterblock[][] 取输入图像的中心方块
* @param[in] iThreshold 黑白划分的阈值
* @param[in] adPointsy[] 存储每个黑点的y坐标值
* @param[in] adPointsx[] 存储每个黑点的X坐标值
* @param[in] aiPointsind[] 每个黑点按照y坐标值递增排序
* @param[in] iErrcode 错误标志
* @param[in] iPoints 黑点数量
* @param[in] iPointsize 黑点面积
* @return 无返回值
* @note
*/
void fnPickSortDots(int **aiCenterblock, const int iThreshold, double *adPointsy, double *adPointsx, int *aiPointsind, int *iErrcode, int *iPoints, int *iPointsize)
{
	int i,j;                           /// 用于for循环的控制变量
	int iTemp1,iTemp2;                 /// 中间变量
	//double adPointsw[MAXP]={0.0};      /// 每个黑点的面积
    double *adPointsw;
    adPointsw = (double*)malloc(sizeof(double)*MAXP);   
	memset(adPointsw, 0, sizeof(double) * MAXP);
    
	double dMeanweight=0.0;            /// 与黑点面积相关的变量
	double dWeight_up,dWeight_dn;      /// 用于将过大或者过小的黑点滤掉
	*iErrcode=0;
	*iPoints=0;
	for(i=0; i<WINDOW; i++){
		for(j=0; j<WINDOW; j++){
			if(aiCenterblock[i][j]>=iThreshold){
				continue;
			}
			(*iPoints)++;
			if((*iPoints)>MAXP){
				(*iErrcode)=103;
				return ;
			}
			/// 计算（i,j）这个点的横纵坐标和大小
			fnConnects(aiCenterblock,i,j,iThreshold,&(adPointsy[(*iPoints)-1]),&(adPointsx[(*iPoints)-1]),&(adPointsw[(*iPoints)-1]));
			dMeanweight+=adPointsw[(*iPoints)-1];
			if((*iPoints)==MAXP){
				break;
			}
		}
		if((*iPoints)==MAXP){
			break;
		}
	}
	if((*iPoints)<MINP){
        (*iErrcode)=102;
		return ;
	}
	dMeanweight/=(double)(*iPoints);
	dWeight_up=dMeanweight*1.5;
	dWeight_dn=dMeanweight*0.75;
	*iPointsize=(int)(sqrt(dWeight_dn)+0.5);
	///按y顺序排序pointsind，剔除过大或过小的点，个数points
	aiPointsind[0]=0;
	iTemp1=1;
	for(i=1; i<(*iPoints); i++){
		if((adPointsw[i]>dWeight_up) || (adPointsw[i]<dWeight_dn)){
			continue;
		}
		iTemp2=iTemp1;
		while((iTemp2>0) && ((adPointsy[i]<adPointsy[aiPointsind[iTemp2-1]]) ||
			((adPointsy[i]==adPointsy[aiPointsind[iTemp2-1]]) && (adPointsx[i]<adPointsx[aiPointsind[iTemp2-1]])))){
				iTemp2--;
		}
		for(j=iTemp1;j>=iTemp2+1;j--){
            aiPointsind[j]=aiPointsind[j-1];
		}
		aiPointsind[iTemp2]=i;
		iTemp1++;
	}
	*iPoints=iTemp1;

	free(adPointsw);
}

/**
* 求第(i,j)个点的横纵坐标(y,x)和面积
* @param[in] aiCenterblock[][] 取输入图像的中心方块
* @param[in] i 所求点的索引
* @param[in] j 所求点的索引
* @param[in] iThreshold 黑白划分的阈值
* @param[in] y 所求点的纵坐标
* @param[in] x 所求点的横坐标
* @param[in] dWeight 所求点的面积
* @return 无返回值
* @note
*/
void fnConnects(int **aiCenterblock, int i, int j, const int iThreshold, double *y, double *x, double *dWeight)
{
	int iStackhead=1;                 /// 寻找相邻黑色像素过程中的控制变量
	int iStacktail=0;                 /// 寻找相邻黑色像素过程中的控制变量
	//int aiStacky[STACKBUF]={0};       /// 存放相邻的黑色像素的y坐标
	//int aiStackx[STACKBUF]={0};       /// 存放相邻的黑色像素的x坐标
    int *aiStacky, *aiStackx;
    aiStacky = (int*)malloc(sizeof(int)*STACKBUF);
	memset(aiStacky, 0, sizeof(int) * STACKBUF);
    aiStackx = (int*)malloc(sizeof(int)*STACKBUF);   
	memset(aiStackx, 0, sizeof(int) * STACKBUF);
    
	int iSumy,iSumx;                  /// 与（i,j）像素相邻的所有黑色像素的坐标之和
	int iWeight1=0;                   /// 标志与（i,j）像素相邻的所有黑色像素的个数
	aiStacky[0]=i;
	aiStackx[0]=j;
	aiCenterblock[i][j]=255;
	iWeight1=aiCenterblock[i][j];
	*dWeight=1.0;
	iSumy=i*aiCenterblock[i][j];
	iSumx=j*aiCenterblock[i][j];
	/// 寻找与（i,j）像素相邻的黑色像素，组成一个大的黑点
  	while(iStackhead!=iStacktail){
		i=aiStacky[iStacktail];
		j=aiStackx[iStacktail];
		iStacktail++;
		if(iStacktail>=STACKBUF){
			iStacktail=0;
		}
		/// 判断相邻的点是否越界、是否是黑点，如果满足条件，将其组成一个黑点
		if((i+1<WINDOW) && (aiCenterblock[i+1][j]<iThreshold)){
			aiCenterblock[i+1][j]=255;
			aiStacky[iStackhead]=i+1;
			aiStackx[iStackhead]=j;
			(*dWeight)++;
			iWeight1+=aiCenterblock[i+1][j];
			iSumy+=(i+1)*aiCenterblock[i+1][j];
			iSumx+=j*aiCenterblock[i+1][j];
			iStackhead++;
			if(iStackhead>=STACKBUF){
				iStackhead=0;
			}
		}
		if((i-1>=0) && (aiCenterblock[i-1][j]<iThreshold)){
			aiCenterblock[i-1][j]=255;
			aiStacky[iStackhead]=i-1;
			aiStackx[iStackhead]=j;
			(*dWeight)++;
			iWeight1+=aiCenterblock[i-1][j];
			iSumy+=(i-1)*aiCenterblock[i-1][j];
			iSumx+=j*aiCenterblock[i-1][j];
			iStackhead++;
			if(iStackhead>=STACKBUF){
				iStackhead=0;
			}
		}
		if((j+1<WINDOW) && (aiCenterblock[i][j+1]<iThreshold)){
			aiCenterblock[i][j+1]=255;
			aiStacky[iStackhead]=i;
			aiStackx[iStackhead]=j+1;
			(*dWeight)++;
			iWeight1+=aiCenterblock[i][j+1];
			iSumy+=i*aiCenterblock[i][j+1];
			iSumx+=(j+1)*aiCenterblock[i][j+1];
			iStackhead++;
			if(iStackhead>=STACKBUF){
				iStackhead=0;
			}
		}
		if((j-1>=0) && (aiCenterblock[i][j-1]<iThreshold)){
			aiCenterblock[i][j-1]=255;
			aiStacky[iStackhead]=i;
			aiStackx[iStackhead]=j-1;
			(*dWeight)++;
			iWeight1+=aiCenterblock[i][j-1];
			iSumy+=i*aiCenterblock[i][j-1];
			iSumx+=(j-1)*aiCenterblock[i][j-1];
			iStackhead++;
			if(iStackhead>=STACKBUF){
				iStackhead=0;
			}
		}
	}
	/// 计算横纵坐标
	*y=(double)iSumy/iWeight1;
	*x=(double)iSumx/iWeight1;

	free(aiStacky);
	free(aiStackx);
}


/**
* 每点求下方最临近点，计算偏移角度和距离
* @param[in] adPointsy[] 黑点的纵坐标
* @param[in] adPointsx[] 黑点的横坐标
* @param[in] aiPointsind[] 黑点按照纵坐标递增排序
* @param[in] iPoints 黑点的数量
* @param[in] adDis[] 每个黑点与其下方最邻近黑点之间的距离
* @param[in] adAng[] 每个黑点与其下方最邻近黑点之间的偏移角度
* @return 无返回值
* @note
*/
void fnNearAng(const double *adPointsy, const double *adPointsx, const int *aiPointsind, int iPoints, double *adDis, double *adAng)
{
	int i,j;                        /// 用于for循环等的控制变量
	double dTemp1,dTemp2;           /// 中间变量
	double dValue1,dValue2;         /// 计算距离的中间变量
	for(i=0; i<iPoints; i++){
		dTemp1=WINDOW*2.0;
		j=i+1;
		while((j<iPoints) && (!(adPointsy[aiPointsind[j]]-adPointsy[aiPointsind[i]]>dTemp1))){
			dValue1=adPointsy[aiPointsind[j]]-adPointsy[aiPointsind[i]];
			dValue2=adPointsx[aiPointsind[j]]-adPointsx[aiPointsind[i]];
			dTemp2=sqrt(dValue1*dValue1+dValue2*dValue2);
			if(dTemp2<dTemp1){
				dTemp1=dTemp2;
				adDis[i]=dTemp1;    /// 黑点与其下方邻近黑点的最短距离
				dTemp2=adPointsx[aiPointsind[i]]-adPointsx[aiPointsind[j]];
				/// 计算黑点与其下方最邻近点的角度
				if(dTemp2>=0.0){
					adAng[i]=180.0-acos(dTemp2/adDis[i])/M_PI*180.0;
				}
				else{
					adAng[i]=acos(-dTemp2/adDis[i])/M_PI*180.0;
				}
			}
			j++;
		}
	}
}

/**
* 按顺序读取矩阵
* @param[in] aiMatr[][] 填充得到的数据矩阵
* @param[in] iSty 起始点的纵坐标
* @param[in] iStx 起始点的横坐标
* @param[in] aiSecmsg[] 解码得到的码流
* @return 无返回值
* @note
*/
void fnUnCoverSeed(int **aiMatr,int iSty,int iStx, int *aiSecmsg)
{
	int i,j;                            /// 用于for循环等的控制变量
	int iMask;                          /// 掩码
	int iPatternsize=SEED_unit*2;
	int iP0,iP1,iP2,iP3,iP4;
	int iCount=0;                       /// 计数变量
	int iTempy=iSty;
	int iTempx=iStx-2;                  /// 初始定位点
	double dS0,dS1,dS2,dS3,dS4;
	if (iStx+10<=iPatternsize){
		iP1=aiMatr[iSty-1][iStx+3]*8+aiMatr[iSty-1][iStx+5]*4+aiMatr[iSty-1][iStx+7]*2+aiMatr[iSty-1][iStx+9];
		iP2=fnMod((iP1-2),15)+1;
	}
	else{
		iP2=aiMatr[iSty-1][fnMod((iStx+17),iPatternsize)]*8+aiMatr[iSty-1][fnMod((iStx+19),iPatternsize)]*4+aiMatr[iSty-1][fnMod((iStx+21),iPatternsize)]*2+aiMatr[iSty-1][fnMod((iStx+23),iPatternsize)];
		iP1=fnMod(iP2,15)+1;
	}
	iP0=aiMatr[iSty-1][fnMod((iStx+11),iPatternsize)]*4+aiMatr[iSty-1][fnMod((iStx+13),iPatternsize)]*2+aiMatr[iSty-1][fnMod((iStx+15),iPatternsize)];
	if( iP0==0 ){
		iP0=11;
	}
	else{
		iP0=iP0+3;
	}
	iP3=fnMod((iP1-iP0-1),15)+1;
	iP4=fnMod((iP2-iP0-1),15)+1;
	dS1=(fnMod((iP1*97),17)+1)/37.0;
	dS2=(fnMod((iP2*97),17)+1)/37.0;
	dS3=(fnMod((iP3*97),17)+1)/37.0;
	dS4=(fnMod((iP4*97),17)+1)/37.0;
	for (i=1;i<=iPatternsize;i++){
		for (j=fnMod((i-1),2)+1;j<=iPatternsize;j+=2){
			dS1=3.96*dS1*(1-dS1);
			dS2=3.96*dS2*(1-dS2);
			dS3=3.96*dS3*(1-dS3);
			dS4=3.96*dS4*(1-dS4);
			iTempx=fnMod((iTempx+1),iPatternsize)+1;
			if ((i==1) ||
			   (i==2 && j==2) || (i==iPatternsize && j==2) ||
			   (i==3 && j==1) || (i==iPatternsize-1 && j==1)){
				continue;
			}
			if (iTempy>=iSty && iTempx>=iStx)
				dS0=dS1;
			else if (iTempy>=iSty && iTempx<iStx)
				dS0=dS2;
			else if (iTempy<iSty && iTempx>=iStx)
				dS0=dS3;
			else
				dS0=dS4;
			iMask=(dS0*128.0-floor(dS0*128.0))>0.5;
			aiSecmsg[iCount]=aiMatr[iTempy-1][iTempx-1]^iMask;
			iCount=iCount+1;
		}
		iTempx=fnMod(i,2)+iStx-2;
		iTempy=fnMod(iTempy,iPatternsize)+1;
	}
}

/**
* 以最集中的作为倾斜角、间隔（编码矩阵中每点的大小）
* @param[in] adDis[] 每个黑点与其下方最邻近黑点之间的距离
* @param[in] adAng[] 每个黑点与其下方最邻近黑点之间的偏移角度
* @param[in] iPoints 黑点数量
* @param[in] iErrcode 错误标志
* @param[in] dXang 以最集中的作为倾斜角
* @param[in] dXscale 编码矩阵中每点的横向间隔
* @param[in] dYscale 编码矩阵中每点的纵向间隔
* @return 无返回值
* @note
*/
void fnAngScale(const double *adDis,const double *adAng,int iPoints, int *iErrcode, double *dXang, double *dXscale, double *dYscale)
{
	int iWindow2 = 10;                               /// 角度窗口
	int iEps = 5;                                    /// 角度阈值
	int i=0,j=0;                                     /// 用于for循环等的控制变量
	int iTemp;                                       /// 中间变量
	int iMaxp,iMaxn;                                 /// 出现次数最多的角度、及其出现次数
	int aiTemp[180]={0};                             /// 每个角度的出现次数
	int aiPos[4]={0},aiNum[4]={0};                   /// 存放四个出现次数最多的角度（从小到大存放）、黑点的角度在四个集中方向附近的黑点个数
	double dDis1,dDis2;                              /// 角度之间的距离
	double dTemp;                                    /// 中间变量
	double adPosp[4]={0.0},adScalep[4]={0.0};        /// 角度在四个集中方向附近的所有黑点的角度和、角度在四个集中方向附近的所有黑点的距离中值
	//double adScale[4][MAXP];                         /// 角度在四个集中方向附近的所有黑点的距离
    double **adScale;
    adScale = (double **)malloc(4 * sizeof(double *));
    for(i=0; i<4; i++)
        adScale[i] = (double * )malloc(MAXP * sizeof(double));
    
	*iErrcode = 0;
	for(i = 0; i < iPoints; i++){
		if(adDis[i]==0) {
			continue;
		}
		dDis1 = (int)(adAng[i]);
		j = fnMod((dDis1-iWindow2),180)+1;
		while(j != (fnMod((dDis1-1),180)+1)){
			aiTemp[j-1] = aiTemp[j-1]+1;
			j = j+1;
			if(j>180){
				j = 1;
			}
		}
	}
	for(i = 0; i < 4; i++){
		iMaxp = 0;
		iMaxn = 0;
		for (j = 0; j < 180; j++){
			if(aiTemp[j] > iMaxn){
				iMaxn = aiTemp[j];
				iMaxp = j+1;
			}
		}
		aiPos[i] = iMaxp;
		j = fnMod((iMaxp-iWindow2),180)+1;
		while (j != fnMod((iMaxp+iWindow2-1),180)+1){
			aiTemp[j-1] = 0;
			j = j+1;
			if (j>180) {
				j = 1;
			}
		}
	}
	for ( i = 0; i < 3; i++){
		for (j = i+1; j < 4; j++){
			if (aiPos[i]>aiPos[j]){
				iTemp = aiPos[i];
				aiPos[i] = aiPos[j];
				aiPos[j] = iTemp;
			}
		}
	}
	/// 统计四个集中方向
	for ( i = 0; i < iPoints; i++){
		if (0<adAng[i]-aiPos[0] && adAng[i]-aiPos[0]<iWindow2){
			adPosp[0] += adAng[i];
			aiNum[0] = aiNum[0]+1;
			adScale[0][aiNum[0]-1] = adDis[i];
		}
		else if (0<adAng[i]-aiPos[1] && adAng[i]-aiPos[1]<iWindow2){
			adPosp[1] +=adAng[i];
			aiNum[1] = aiNum[1]+1;
			adScale[1][aiNum[1]-1] = adDis[i];
		}
		else if (0<adAng[i]-aiPos[2] && adAng[i]-aiPos[2]<iWindow2){
			adPosp[2] +=adAng[i];
			aiNum[2] = aiNum[2]+1;
			adScale[2][aiNum[2]-1] = adDis[i];
		}
		else if (0<adAng[i]-aiPos[3] && adAng[i]-aiPos[3]<iWindow2){
			adPosp[3] +=adAng[i];
			aiNum[3] = aiNum[3] +1;
			adScale[3][aiNum[3]-1] = adDis[i];
		}
		else if (0<adAng[i]+180-aiPos[3] && adAng[i]+180-aiPos[3]<iWindow2){
			adPosp[3] +=adAng[i]+180;
			aiNum[3] = aiNum[3]+1;
			adScale[3][aiNum[3]-1] =adDis[i];
		}
	}
	if (aiNum[0]==0 || aiNum[1]==0 || aiNum[2]==0 || aiNum[3]==0){
		*dXang=0;
		*dXscale=0;
		*dYscale=0;
		*iErrcode=104;    ///点未能在四个方向上集中
		return;
	}
	adPosp[0] = adPosp[0]/aiNum[0];
	adPosp[1] = adPosp[1]/aiNum[1];
	adPosp[2] = adPosp[2]/aiNum[2];
	adPosp[3] = adPosp[3]/aiNum[3];
	for (i = 0; i < (int)aiNum[0]/2; i++){
		for (j = i+1; j < aiNum[0]; j++){
                if(adScale[0][i]<adScale[0][j]){
			         dTemp = adScale[0][i];
                     adScale[0][i] = adScale[0][j];
                     adScale[0][j] = dTemp;
                }
		}
	}
    i=i-1;
	adScalep[0] = adScale[0][i];
	for (i = 0; i < (int)aiNum[1]/2; i++){
		for (j = i+1; j < aiNum[1]; j++){
			if (adScale[1][i]<adScale[1][j]){
				dTemp = adScale[1][i];
				adScale[1][i] = adScale[1][j];
				adScale[1][j] = dTemp;
			}
		}
	}
	i=i-1;
	adScalep[1] = adScale[1][i];
	for (i = 0; i < (int)aiNum[2]/2; i++){
		for (j = i+1; j < aiNum[2]; j++){
			if (adScale[2][i]<adScale[2][j]){
				dTemp = adScale[2][i];
				adScale[2][i] = adScale[2][j];
				adScale[2][j] = dTemp;
			}
		}
	}
	i=i-1;
	adScalep[2] = adScale[2][i];
	for (i = 0; i < (int)aiNum[3]/2; i++){
		for (j = i+1; j < aiNum[3]; j++){
			if (adScale[3][i]<adScale[3][j]){
				dTemp = adScale[3][i];
				adScale[3][i] = adScale[3][j];
				adScale[3][j] = dTemp;
			}
		}
	}
	i=i-1;
	adScalep[3] = adScale[3][i];
	dDis1 = abs(adPosp[2] - adPosp[0]);
	dDis2 = abs(adPosp[3] - adPosp[1]);
	if (abs(dDis1-90)<iEps && (abs(dDis2-90)<iEps)){
		if (adScalep[0]<adScalep[1]){
			*dXang = adPosp[1];
			*dXscale = adScalep[1];
			*dYscale = adScalep[3];
		}
		else{
			*dXang = adPosp[0];
			*dXscale = adScalep[0];
			*dYscale = adScalep[2];
		}
	}
	else if (abs(dDis1-90)<iEps){
		*dXang = adPosp[0];
		*dXscale = adScalep[0];
		*dYscale = adScalep[2];
	}
	else if (abs(dDis2-90)<iEps){
		*dXang = adPosp[1];
		*dXscale = adScalep[1];
		*dYscale = adScalep[3];
	}
	else{
		*dXang = 0;
		*dXscale = 0;
		*dYscale = 0;
		*iErrcode = 105;    ///未能找到x-y方向
		return ;
	}
	adPosp[0] =0;
	aiNum[0] = 0;
	for (i = 0; i < iPoints; i++){
		dDis1 = fnDistant(adAng[i], *dXang);
		if (abs(dDis1)<iWindow2/2){
			adPosp[0] = adPosp[0]+*dXang+dDis1;
			aiNum[0] = aiNum[0]+1;
		}
		dDis1 = fnDistant(adAng[i],(*dXang+90));
		if (abs(dDis1)<iWindow2/2){
			adPosp[0] = adPosp[0]+*dXang+dDis1;
			aiNum[0] = aiNum[0]+1;
		}
		dDis1 = fnDistant(adAng[i],(*dXang-90));
		if (abs(dDis1)<iWindow2/2){
			adPosp[0] = adPosp[0]+*dXang+dDis1;
			aiNum[0] = aiNum[0]+1;
		}
	}
	*dXang = adPosp[0]/aiNum[0];

	for (i = 0; i < 4; i++) free(adScale[i]);
	free(adScale);
}


/**
* 求角度之间距离
* @param[in] dInput1 输入角度
* @param[in] dInput2 输入角度
* @return dOutput 两个角度之间距离
* @note 无
*/
double fnDistant(double dInput1, double dInput2)
{
	double dOutput;
	dOutput = dInput1-dInput2;
	if (abs((int)(dInput1-dInput2+180))<abs((int)dOutput)){
		dOutput = dInput1-dInput2+180;
	}
	if (abs((int)(dInput1-dInput2-180))<abs((int)dOutput)){
		dOutput = dInput1-dInput2-180;
	}
	return dOutput;
}

/**
* 寻找矩阵起始标志点及方向，旋转矩阵到正方向
* @param[in] aiMatr[][] 填充得到的数据矩阵
* @param[in] iSty 矩阵起始标志点的纵坐标
* @param[in] iStx 矩阵起始标志点的横坐标
* @return 无返回值
* @note 返回iSty和iStx的值，并会改变aiMatr[][]中的值
*/
void fnFindStart(int **aiMatr, int *iSty, int *iStx)
{
	int iPatternsize=SEED_unit*2;
    int i=0,j=0,ii=0,jj=0;                      /// 用于for循环等的控制变量
	//int aiMatr1[SEED_unit*2][SEED_unit*2];      /// 数据矩阵
    int **aiMatr1;
    aiMatr1 = (int **)malloc(SEED_unit*2 * sizeof(int *));
    for(i=0; i<SEED_unit*2; i++)
        aiMatr1[i] = (int * )malloc(SEED_unit*2 * sizeof(int));
    
	*iStx=0;
	*iSty=0;
	/// 寻找符合编码的定位点
	for ( i = 1; i <=iPatternsize; i++){
		for ( j = 1; j <=iPatternsize; j++){
			if (aiMatr[i-1][j-1]==0){
				if (aiMatr[i-1][fnMod(j,iPatternsize)]==0){
					if (aiMatr[fnMod((i-3),iPatternsize)][j-1]==0){
						*iSty = iPatternsize-i+1;
						*iStx = j;
						for (ii = 1; ii <=iPatternsize; ii++){
							for (jj = 1; jj <= iPatternsize; jj++){
								aiMatr1[ii-1][jj-1] = aiMatr[iPatternsize-ii][jj-1];
							}
						}
						break;
					}
					else if (aiMatr[fnMod((i+1),iPatternsize)][j-1]==0){
						*iSty = i;
						*iStx = j;
						for (ii = 1; ii <=iPatternsize; ii++){
							for (jj = 1; jj <=iPatternsize; jj++){
								aiMatr1[ii-1][jj-1] = aiMatr[ii-1][jj-1];
							}
						}
						break;
					}
					else if (aiMatr[fnMod((i-3),iPatternsize)][fnMod(j,iPatternsize)]==0){
						*iSty = iPatternsize-i+1;
						*iStx = fnMod((iPatternsize-j-1),iPatternsize)+1;
						for (ii = 1; ii <=iPatternsize; ii++){
							for (jj = 1; jj <=iPatternsize; jj++){
								aiMatr1[ii-1][jj-1] = aiMatr[iPatternsize-ii][iPatternsize-jj];
							}
						}
						break;
					}
					else if (aiMatr[fnMod((i+1),iPatternsize)][fnMod(j,iPatternsize)]==0){
						*iSty = i;
						*iStx = fnMod((iPatternsize-j-1),iPatternsize)+1;
						for (ii = 1; ii <= iPatternsize; ii++){
							for (jj = 1; jj <= iPatternsize; jj++){
								aiMatr1[ii-1][jj-1] = aiMatr[ii-1][iPatternsize-jj];
							}
						}
						break;
					}
				}
				else if (aiMatr[fnMod(i,iPatternsize)][j-1]==0){
					if (aiMatr[i-1][fnMod((j-3),iPatternsize)]==0){
						*iSty = iPatternsize-j+1;
						*iStx = i;
						for ( ii = 1; ii <= iPatternsize; ii++){
							for (jj = 1; jj <= iPatternsize; jj++){
								aiMatr1[ii-1][jj-1] = aiMatr[jj-1][iPatternsize-ii];
							}
						}
						break;
					}
					else if (aiMatr[i-1][fnMod((j+1),iPatternsize)]==0){
						*iSty = j;
						*iStx = i;
						for (ii = 1; ii <= iPatternsize; ii++){
							for (jj = 1; jj <= iPatternsize; jj++){
								aiMatr1[ii-1][jj-1] = aiMatr[jj-1][ii-1];
							}
						}
						break;
					}
					else if (aiMatr[fnMod(i,iPatternsize)][fnMod((j-3),iPatternsize)]==0){
						*iSty = fnMod((iPatternsize-j),iPatternsize)+1;
						*iStx = fnMod((iPatternsize-1-i),iPatternsize)+1;
						for (ii = 1; ii <= iPatternsize; ii++){
							for (jj = 1; jj <= iPatternsize; jj++){
								aiMatr1[ii-1][jj-1] = aiMatr[iPatternsize-jj][iPatternsize-ii];
							}
						}
						break;
					}
					else if (aiMatr[fnMod(i,iPatternsize)][fnMod((j+1),iPatternsize)]==0){
						*iSty = j;
						*iStx = fnMod((iPatternsize-1-i),iPatternsize)+1;
						for (ii = 1; ii <= iPatternsize; ii++){
							for (jj = 1; jj <= iPatternsize; jj++){
								aiMatr1[ii-1][jj-1] = aiMatr[iPatternsize-jj][ii-1];
							}
						}
						break;
					}
				}
			}
		}
		if (*iSty!=0){
			break;
		}
	}
	if (*iSty==0){
		return;
	}
	for (i = 0; i < iPatternsize; i++){
		for (j = 0; j < iPatternsize; j++){
			aiMatr[i][j] = aiMatr1[i][j];
		}
	}

	for(i=0; i < SEED_unit * 2; i++) free(aiMatr1[i]);
	free(aiMatr1);
}

/**
* 求模，结果是正数
* @param[in] iInput1 被求模数
* @param[in] iInput2 模数
* @return iOutput 返回求模的结果
* @note 无
*/
int fnMod(int iInput1, int iInput2)
{
    int iOutput;
    if(iInput1>=0){
        iOutput=iInput1%iInput2;
    }
    else{
        iOutput=(iInput1+iInput2)%iInput2;
    }
    return iOutput;
}

/**
* 当一个数非常接近整数时，四舍五入
* @author: Ying Jia
* @param[in] dInput 输入的操作数
* @return dOutput 返回结果
* @note 无
*/
double fnRound(double dInput)
{
    int iTemp;
    double dOutput;
    iTemp=(int)(dInput+0.5);
    if(fabs(dInput-iTemp)<0.1){
        dOutput=iTemp;
    }
    else{
        dOutput=dInput;
    }
    return dOutput;
}

/**
* 01比特流转换成字符
* @param[in] aiSecmsg[] 01比特流
* @param[in] pcMsg_char[] 输出字符
* @param[in] iErrcode 错误标志
* @return pcMsg_char[] 返回输出的字符
* @note
*/
char *fnBit2char(int *aiSecmsg,char *pcMsg_char, int *iErrcode)
{
	int aiMsg_bit[162] = { 0 };       /// 解码出比特流的前162位有效位
	int aiCrc[32] = { 0 };            /// 与校验相关的数组
	int *piCrc32;                     /// 二进制形式的校验码
	int iCrctemp;                     /// 十进制形式的校验码
	int iFlag = 0;                    /// 标志
	int iIdx;                         /// 用于for循环等的控制变量
	int i;                            /// 用于for循环等的控制变量
	for (iIdx = 0; iIdx < m_Decode_iMsg_length; iIdx++)
		aiMsg_bit[iIdx] = aiSecmsg[iIdx];
	iCrctemp = fnCrc32Decode(aiMsg_bit, m_Decode_iMsg_length);
	piCrc32 = fndec2bin(iCrctemp, aiCrc, 0);
	for (iIdx = 0 && (!iFlag); iIdx < 32; iIdx++)
	{
		if (piCrc32[iIdx] != aiSecmsg[iIdx + m_Decode_iMsg_length])
			{
				*iErrcode = 108;
				iFlag = 1;
				char pchError[20]= "The decode is error";
				pcMsg_char = pchError;
				return pcMsg_char;
			}
	}
	for (i = 0; i < 27; i++)
	{
		iIdx = (((((aiMsg_bit[6 * i] * 2) + aiMsg_bit[1 + 6 * i]) * 2 + aiMsg_bit[2 + 6 * i]) * 2
			+ aiMsg_bit[3 + 6 * i]) * 2 + aiMsg_bit[4 + 6 * i]) * 2 + aiMsg_bit[5 + 6 * i];
		pcMsg_char[i] = m_acTable[iIdx];
	}
	return pcMsg_char;
}

/**
* CRC校验码生成函数
* @param[in] aiMsg_bit[] 待加密的比特信息，由str2bin函数生成
* @param[in] iSize 信息比特流的长度
* @return iCrc 32位的crc校验码，以十进制的形式返回
* @note
*/
int fnCrc32Decode(const int * aiMsg_bit, int iSize)
{
	int i, j, iIdx;                            /// 用于for循环等的控制变量
	int iByte, iCrc, iMask;
	static unsigned int s_aiCrctable[256];

	if (s_aiCrctable[1] == 0)
	{
		for (iByte = 0; iByte <= 255; iByte++)
		{
			iCrc = iByte;
			for (j = 7; j >= 0; j--)
			{
				iMask = -(iCrc & 1);
				iCrc = (iCrc >> 1) ^ (0xEDB88320 & iMask);
			}
			s_aiCrctable[iByte] = iCrc;
		}
	}
	i = 0;
	iCrc = 0xFFFFFFFF;
	for (iIdx=0; iIdx < iSize; iIdx++)
	{
		iCrc = (iCrc >> 8) ^ s_aiCrctable[(iCrc ^ aiMsg_bit[iIdx]) & 0xFF];
		i = i + 1;
	}
	return ~iCrc;

}

/**
* 将十进制的数转换成二进制比特流，生成crc校验码函数的补充函数
* @param[in] iNum 待转换的数，32位的crc校验码
* @param[in] aiCrc[] 存储转换结果
* @param[in] i 函数内部数组索引
* @return aiCrc 存32位的校验码
* @note
*/
int *fndec2bin(int iNum, int *aiCrc, int i)
{
    if(iNum<0)
        iNum=-iNum;
	if (iNum == 0)
		return aiCrc;
	else
	{
		aiCrc[31 - i] = iNum % 2;
		return fndec2bin(iNum / 2, aiCrc, ++i);
	}
}


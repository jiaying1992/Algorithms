///////////////////////////////////
/// @file Encode.c
/// @brief 解码程序
/// @author: Ying Jia
/// @date 2017.6.10
///////////////////////////////////
#include "Encode.h"
/**
*
* @param[in] adMsg_num&adMsg_info 长度分别为3和24的字符串，待编码信息
* @param[in] iImage_width&iImage_height 最终图像矩阵的大小，可根据需要调整
* @return adImage_f 嵌入信息的图像矩阵
* @note 测试最终输出以txt格式，可根据后期需要调整
*/
int main()
{
	char *adMsg_num = "Dec";
	char *adMsg_info = "ZhongGuoKeXueJiShu1958hf";/*存储要嵌入的信息，由用户输入*/
	int iImage_width = 600;
	int iImage_height = 600;/*最终输出图片像素点个数*/
	int iStatus;//编码状态参数

	int  i, j;

	int **pPicData = (int **)malloc(sizeof(int *)*iImage_height);//pPicData存储最终的隐码图像
	for (i = 0; i<iImage_height; i++)
	{
		*(pPicData + i) = (int *)malloc(sizeof(int)*iImage_width);
		if (*(pPicData + i) == NULL)
		{
			printf("Failed in applying memory\n");
			return 201;
		}
	}
	iStatus = fnImage_fill(adMsg_num, adMsg_info, iImage_width, iImage_height,pPicData);

	FILE *fp = fopen("code1.txt", "w");
	if (fp == NULL)
	{
		printf("Cannot open file");
		return 203;
	}

	for (i = 0; i<iImage_height; i++)
	{
		for (j = 0; j<iImage_width; j++)
		{
			fprintf(fp, "%d", (pPicData[i][j]));
		}
	}
	fclose(fp);
	free(pPicData);
	return 0;
}

/**
* 图像矩阵生成函数
* 输入参数：
* @param[in] adMsg_num&adMsg_info：需要嵌入的信息，其中adMsg_num为三位字符，adMsg_info为24位字符，
共计27位；
* @param[in] iImage_width&iImage_height：输出的图像矩阵大小，其中image_width和image_height
分别表示图像的宽度和高度；
* @param[out] pPicData：二维图像矩阵，二值矩阵；
* @return 编码状态信息
*			0：编码成功；
*			101：信息位长度大于最多能容纳的信息量；
*			102：信息点设定过大；
*			103：图片像素点过少；
*			202：内存申请失败
* 1）对字符信息进行编码，转化成二进制数据；
* 2）对二进制数据进行crc编码，数据加校验信息共有194位；
* 3）将比特流数据填充到基本图样中，并对图像进行加掩码之后填充到最终图像中
**/

int fnImage_fill(char *adMsg_num, char *adMsg_info, int iImage_width, int iImage_height,int **pPicData)
{

	if (m_iMsg_length > m_iSeed_Unit * (m_iSeed_Unit * 2 - 1) - 4)
	{
		return 101;
	}
	if (m_iDPPx >10 || m_iDPPy >10)
	{
		return 102;
	}
	if ((m_iSeed_Unit*m_iUnit*m_iDPPx>iImage_width) || (m_iSeed_Unit*m_iUnit*m_iDPPy>iImage_height))
	{
		return 103;
	}

	/*
	初始化变量
	@aMsg[]：存储输入的字符串信息，总共27位
	@adMsg_bit：存储将字符串转化成的二进制比特流
	@aCrc32[]：存储32位的crc校验码
	@aSecmsg[]：存储需要嵌入到图片矩阵中的信息，包括校验码
	*/
	char aMsg[28] = { '\0' };
	int *adMsg_bit = malloc(sizeof(int) *m_iMsg_length);
	if (adMsg_bit == NULL)
	{
		printf("Failed in applying memory\n");
		return 202;
	}
	int aCrc[32] = { 0 };
	int *adCrc32;
	int aSecmsg[194] = { 0 };
	int i, j, i1, j1;//循环的控制变量
	strcat(strcat(aMsg, adMsg_num), adMsg_info);
	fnStr2Bin(aMsg,adMsg_bit);/*将长度为27的字符串转化成162位的比特流*/
	int crcTemp = fnCrc32(adMsg_bit, m_iMsg_length);
	adCrc32 = fnDec2Bin(crcTemp, aCrc, 0);/*对信息比特流加校验码*/
	for (i = 0; i<m_iMsg_length; i++)
	{
		aSecmsg[i] = adMsg_bit[i];
	}

	for (i = 0; i<32; i++)
	{
		aSecmsg[i + m_iMsg_length] = adCrc32[i];
	}

	free(adMsg_bit);

	/*
	生成基本编码样图
	@m_iSeed_Unit  基本编码图大小，设定为13
	@m_iUnit = 6   在两个编码信息行/列之间，均加入空白行，
	@adImage_Seed 基本编码样图
	*/
	int iSeed_Size = m_iSeed_Unit*m_iUnit;
	int iHalfunit = m_iUnit / 2;
	int **adImage_Seed = (int **)malloc(sizeof(int *)*iSeed_Size);
	if (*adImage_Seed == NULL)
	{
		printf("Failed in applying memory\n");
		return 202;
	}
	for (i = 0; i<iSeed_Size; i++)
	{
		if (( *(adImage_Seed + i) = (int *)malloc(sizeof(int)*iSeed_Size) )== NULL )
		{
			printf("Failed in applying memory\n");
			return 202;
		}
	}
	for (i = 0; i<iSeed_Size; i++)
		for (j = 0; j<iSeed_Size; j++)
		{
			adImage_Seed[i][j] = 1;
		}

	adImage_Seed[0][0] = 0;
	adImage_Seed[0][iHalfunit] = 0;
	adImage_Seed[m_iUnit][0] = 0;/*设置定位点信息*/
	int iIdx = 0;

	/*将194位的信息比特流嵌入到基本编码图样中*/
	for (i = 0; i<iSeed_Size; i += iHalfunit)
		for (j = i%m_iUnit; j<iSeed_Size; j += m_iUnit)
		{
			if ((i == 0) || (i == iHalfunit && j == iHalfunit) ||
				(i == iSeed_Size - iHalfunit && j == iHalfunit) ||
				(i == m_iUnit && j == 0) || (i == iSeed_Size - m_iUnit && j == 0))
				continue;
			if (iIdx < m_iMsg_length + 32)
			{
				adImage_Seed[i][j] = aSecmsg[iIdx];
				iIdx++;
			}
		}

	/*	填充图像 */
	int iSeed_width = iSeed_Size*m_iDPPx;
	int iSeed_height = iSeed_Size*m_iDPPy;//计算图像块的大小
	int iTemp = ((iImage_width / 2 - iSeed_width / 2) / iSeed_width) + 1;
	int iStartx = iImage_width / 2 - iSeed_width / 2 - iSeed_width * iTemp - 1;
	iTemp = ((iImage_height / 2 - iSeed_height / 2) / iSeed_height) + 1;
	int iStarty = (iImage_height / 2 - iSeed_height / 2 - iSeed_height * iTemp) - 1;//计算初始填充位置

	for (i = 0; i<iImage_height; i++)
		for (j = 0; j<iImage_width; j++)
		{
			pPicData[i][j] = 1;
		}

	int iSeed_x = iStartx;
	int iSeed_y = iStarty;
	int iCoor_x, iCoor_y;//图像坐标控制变量
	srand((unsigned)time(NULL));
	int iMarkseq = rand() % 15 + 1;//生成随机数做标记
	int iMarks = iMarkseq;
	int iMarks1 = 4;

	int flag = 1;

	while (flag)
	{
		int **adImage_fSeed = (int **)malloc(sizeof(int *)*iSeed_Size);
		if (*adImage_fSeed == NULL)
			exit(1);
		for (i = 0; i<iSeed_Size; i++)
		{
			if ((*(adImage_fSeed + i) = (int *)malloc(sizeof(int)*iSeed_Size)) == NULL)
				exit(1);
		}
		fnCoverSeed(adImage_Seed, iMarks, iMarks1,adImage_fSeed);//对基本图像做掩码处理
		for (i = 0; i<iSeed_Size; i++)
			for (j = 0; j<iSeed_Size; j++)
				for (i1 = 0; i1<m_iDPPx; i1++)
					for (j1 = 0; j1<m_iDPPy; j1++)
					{
						iCoor_y = iSeed_y + i*m_iDPPy + i1;
						iCoor_x = iSeed_x + j*m_iDPPx + j1;
						if (iCoor_y >= 0 && iCoor_y<iImage_height && iCoor_x >= 0 && iCoor_x<iImage_width)
							pPicData[iCoor_y][iCoor_x] = adImage_fSeed[i][j];//填充图像
					}
		free(adImage_fSeed);
		iSeed_x = iSeed_x + iSeed_width;
		iMarks = iMarks % 15 + 1;
		if (iSeed_x >= iImage_width)
		{
			iSeed_x = iStartx;
			iSeed_y = iSeed_y + iSeed_height;
			srand((unsigned)time(NULL));
			iMarks1 = rand() % 8 + 4;
			  = (iMarkseq + iMarks1 - 1) % 15 + 1;
			iMarkseq = iMarks;
			if (iSeed_y >= iImage_height)
				flag = 0;
		}
	}
	free(adImage_Seed);
	return 0;
}

/**
* 基本编码图像的生成以及掩码的添加
* 输入:图像只包含信息位与定位点的图样，信息长度和定位信息
* @param[in]	adImageSeed:已经嵌入数据与定位信息的基本图样，其余位为0；
* @param[in]	iMarks，iMarks1定位信息
* @param[in] adImage_fSeed 经填充与掩码操作的图样,也作为最终填充的基本样图
**/
void fnCoverSeed(int **adImage_Seed, int iMarks, int iMarks1,int **adImage_fSeed)
{
	int iSeed_Size = m_iSeed_Unit*m_iUnit;//基本编码图像大小
	int iHalfunit = m_iUnit / 2;//信息位之间的间隔
	int i, j;//循环控制变量
	for (i = 0; i<iSeed_Size; i++)
		for (j = 0; j<iSeed_Size; j++)
		{
			adImage_fSeed[i][j] = adImage_Seed[i][j];
		}
	/*图样第一行嵌入图样相互之间的定位信息*/
	adImage_fSeed[0][2 * m_iUnit] = BINSEQ[iMarks - 1][0];
	adImage_fSeed[0][3 * m_iUnit] = BINSEQ[iMarks - 1][1];
	adImage_fSeed[0][4 * m_iUnit] = BINSEQ[iMarks - 1][2];
	adImage_fSeed[0][5 * m_iUnit] = BINSEQ[iMarks - 1][3];
	adImage_fSeed[0][6 * m_iUnit] = BINSEQ[iMarks1 - 4][1];
	adImage_fSeed[0][7 * m_iUnit] = BINSEQ[iMarks1 - 4][2];
	adImage_fSeed[0][8 * m_iUnit] = BINSEQ[iMarks1 - 4][3];
	adImage_fSeed[0][9 * m_iUnit] = BINSEQ[iMarks - 1][0];
	adImage_fSeed[0][10 * m_iUnit] = BINSEQ[iMarks - 1][1];
	adImage_fSeed[0][11 * m_iUnit] = BINSEQ[iMarks - 1][2];
	adImage_fSeed[0][12 * m_iUnit] = BINSEQ[iMarks - 1][3];

	double Temp = (iMarks * 97 % 17 + 1) / 37.0;//生成掩码
	int iIdx = 0;
	int iSeed;

	/*对待嵌比特流信息后面随机加入0或者1*/
	for (i = 0; i<iSeed_Size; i += iHalfunit)
	{
		for (j = (i % 2)* iHalfunit; j<iSeed_Size; j += m_iUnit)
		{
			Temp = 3.96*Temp*(1 - Temp);
			if ((i == 0) ||
				(i == iHalfunit && j == iHalfunit) || (i == iSeed_Size - iHalfunit && j == iHalfunit) ||
				(i == m_iUnit && j == 0) || (i == iSeed_Size - m_iUnit && j == 0))
				continue;//跳过定位点所在的位置
			;
			iSeed = (Temp * 128.0 - floor(Temp * 128.0))>0.5;
			adImage_fSeed[i][j] = adImage_Seed[i][j] ^ iSeed;
		}
	}
}
/**
* crc校验码生成函数
* 输入参数：
* @param[in] pSrc[]：待加密的比特信息，由str2bin函数生成；
* @param[in] nSize：信息比特流的长度。
* 输出参数：
* return cfgCrc32：32位的crc校验码，以十进制的形式返回。
**/
int fnCrc32(const int *pSrc, int iSize)
{
	int i, j, iIdx = 0;
	int iByte,iMask,iCrc;
	static unsigned int aCrcTable[256];

	if (aCrcTable[1] == 0)
	{
		for (iByte = 0; iByte <= 255; iByte++)
		{
			iCrc = iByte;
			for (j = 7; j >= 0; j--)
			{
				iMask = -(iCrc & 1);
				iCrc = (iCrc >> 1) ^ (0xEDB88320 & iMask);
			}
			aCrcTable[iByte] = iCrc;
		}
	}

	i = 0;
	iCrc = 0xFFFFFFFF;

	for (; iIdx < iSize; iIdx++)
	{
		iCrc = (iCrc >> 8) ^ aCrcTable[(iCrc ^ pSrc[iIdx]) & 0xFF];
		i = i + 1;
	}
	return ~iCrc;

}
/**
* 将十进制的数转换成二进制比特流，生成crc校验码函数的补充函数
* 输入参数
* @param[in] iNum：待转换的数，32位的crc校验码；
* @param[in] pResultp[]:存储转换结果
* @param[in] nIdx:函数内部数组索引
* 返回参数：
* @return pResult:存32位的校验码
**/
int *fnDec2Bin(int iNum, int *pResult, int iIdx)
{
	if (iNum < 0)
		iNum = -iNum;
	if (iNum == 0)
		return pResult;
	else
	{
		pResult[31 - iIdx] = iNum % 2;
		return fnDec2Bin(iNum / 2, pResult, ++iIdx);
	}
}

/**
* 将字符串转化成二进制比特流
* 输入参数：
* @param[in] message[]：待转换的字符串
* @param[in] stream[]:转换好的二进制比特流
**/
void fnStr2Bin(char *pMessage,int *iStream)
{
	int i, j;
	int iMsg_size = strlen(pMessage);
	for (i = 0; i<iMsg_size; i++)
	{
		j = 0;
		while (j<strlen(table) && pMessage[i] != table[j])
			j++;
		if (j >= strlen(table))
		{
			printf("Invalid Character!\n");
			system("pause");
			exit(0);
		}
		iStream[6 * i + 5] = j % 2;
		iStream[6 * i + 4] = (j >> 1) % 2;
		iStream[6 * i + 3] = (j >> 2) % 2;
		iStream[6 * i + 2] = (j >> 3) % 2;
		iStream[6 * i + 1] = (j >> 4) % 2;
		iStream[6 * i] = (j >> 5) % 2;
	}
}

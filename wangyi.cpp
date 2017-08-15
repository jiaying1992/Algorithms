// 网易笔试题目，0和1组成的序列，输出最长的0和1间隔序列的长度
// author: Ying Jia
// time: 20170812
// contact: 1439352516@qq.com
#include<stdio.h>
#include<iostream>
#include<vector>
using namespace std;
int main()
{
	char c[51]={'\0'};
	gets(c);
    vector<int> v;
	for(int i=0; i<50; i++)
	{
		if(c[i]!='\0')
		    v.push_back(c[i]-48);
	}
    int length=v.size();
    int *res=(int *)malloc(sizeof(int)*length);
    res[0]=1;
    int count=1;
    for(int i=1; i<length; i++)
    {
        if(v[i]==v[i-1])
        {
            count=1;
            res[i]=res[i-1];
        }
        else
        {
            count++;
            res[i]=max(res[i-1], count);
        }
    }
	cout<<res[length-1];
    return 0;
}

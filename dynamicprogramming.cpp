// author: Ying Jia
// time: 20170805
// contact: 1439352516@qq.com

#include<iostream>
#include<algorithm>

using namespace std;

/* 数塔问题
#define MAX 100
int D[MAX][MAX];
int n;
int maxSum[MAX][MAX];
int main()
{
	cout<<"请输入三角形矩阵的行数："<<endl;
	cin>>n;
	cout<<"请输入矩阵中每点的值："<<endl;
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<=i; j++)
		{
			cin>>D[i][j];
		}
		//cout<<i;
	}
	cout<<"输入完毕"<<endl;
	for(int i=0; i<n; i++)
	{
		maxSum[n-1][i]=D[n-1][i];
	}
	for(int i=n-2; i>=0; i--)
	{
		for(int j=0; j<=i; j++)
		{
			maxSum[i][j]=max(maxSum[i+1][j],maxSum[i+1][j+1])+D[i][j];
		}
	}
	cout<<"最大路径的值为："<<endl;
	cout<<maxSum[0][0]<<endl;
}
*/

/* 最大连续子序列之和
int main()
{
	int data[]={1,-2,3,-1,7};
	int sum=0, max=data[0];
	for(int i=0; i<sizeof(data)/sizeof(data[0]);i++)
	{
		sum+=data[i];
		if(sum>max)
		{
			max=sum;
		}
		if(sum<0)
		{
			sum=0;
		}
	}
	cout<<max<<endl;
	return 1;
}*/

/* 01背包问题
int main()
{
	int value[]={0,8,10,4,5,5};
	int weight[]={0,6,4,2,4,3};
	int dp[6][11]={0};
	for(int i=1; i<=5; i++)
	{
		for(int j=1; j<=10; j++)
		{
			if(j>weight[i])
				dp[i][j]=max(dp[i-1][j],dp[i-1][j-weight[i]]+value[i]);
			else
				dp[i][j]=dp[i-1][j];
		}
	}
	cout<<dp[5][10]<<endl;
}*/

/* 最长递增子序列
int main()
{
	int a[]={1,-1,2,-3,4,-5,6,-7};
	int length, max=0;
	int *b;
	b=(int *)malloc(sizeof(a));
	length=sizeof(a)/sizeof(a[0]);
	for( int i=0; i<length; i++)
	{
		b[i]=1;
		for(int j=0; j<i; j++)
		{
			if(a[i]>a[j] && b[i]<=b[j])
			{
				b[i]=b[j]+1;
			}
		}
	}
	for(int i=0; i<length; i++)
	{
		if(b[i]>max)
		{
			max=b[i];
		}
	}
	cout<<max<<endl;
}*/

// 最长公共子序列LCS
#define M 8
#define N 6

void printLSC(int i, int j, char *a, int status[][N])
{
	if(i==0 || j==0)
		return;
	if(status[i][j]==0)
	{
		printLSC(i-1,j-1,a,status);
		printf("%c",a[i]);
	}
	else
	{
		if(status[i][j]==1)
			printLSC(i-1,j,a,status);
		else
			printLSC(i,j-1,a,status);
	}
}

int main()
{
	char a[]={' ', 'A', 'B', 'C', 'B', 'D', 'A', 'B'};
	char b[]={' ', 'B', 'D', 'C', 'B', 'A'};
	int status[M][N]={0};
	int dp[M][N]={0};
	for(int i=1; i<M; i++)
	{
		for(int j=1; j<N; j++)
		{
			if(a[i]==b[j])
			{
				dp[i][j]=dp[i-1][j-1]+1;
				status[i][j]=0;
			}
			else if(dp[i][j-1]>=dp[i-1][j])
			{
				dp[i][j]=dp[i][j-1];
				status[i][j]=2;
			}
			else
			{
				dp[i][j]=dp[i-1][j];
				status[i][j]=1; 
			}
		}
	}
	printf("最大长度：%d",dp[M-1][N-1]);
	printf("\n");
	printLSC(M-1,N-1,a,status);
	printf("\n");
}



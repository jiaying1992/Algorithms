// author: Ying Jia
// time: 20170731
// contact: 1439352516@qq.com

#include<iostream>
#include<vector>

using namespace std;

/*--------------print vector----------------*/
void print(vector<int> a)
{
	for(int i=0; i<a.size(); i++)
	{
		cout<<a[i]<<' ';
	}
	cout<<endl;
}

/*----------------swap two values---------------*/
void swap(int &a, int &b)
{
	int temp=a;
	a=b;
	b=temp;
}

/*----------------------quick sort-----------------*/
void quicksort(vector<int> &a, int low, int high)
{
	int lowtemp=low, hightemp=high;
	if(low<high)
	{
		int privot=a[low];
	    while(low<high)
	   {
		  while(low < high && a[high]>=privot)
			--high;
		  swap(a[low], a[high]);
		  while(low < high && a[low]<=privot)
			++low;
		  swap(a[low],a[high]);
	    }
		privot=low;
		quicksort(a, lowtemp, privot-1);
		quicksort(a, privot+1, hightemp);
	}
}

/*---------------------bubble sort----------------------*/
void bubblesort(vector<int> &a)
{
	int flag=1;
	int len=a.size();
	while(len>1 && flag==1)
	{
		flag=0;
		for(int i=0; i<len-1; i++)
		{
			if(a[i]>a[i+1])
			{
				swap(a[i], a[i+1]);
				flag=1;
			}
		}
		len--;
	}
}

/*---------------------select sort----------------------------*/
void selectsort(vector<int> &a)
{
	for(int i=0; i<a.size()-1; i++)
	{
		int label=i;
		for(int j=i+1; j<a.size(); j++)
		{
			if(a[j]<a[label])
			{
				label=j;
			}
		}
		swap(a[i], a[label]);
	}
}

void heapadjust(vector<int> &a, int s, int len)
{
	int temp=a[s];
	int child=2*s+1;
	while(child<len)
	{
		if(child+1 < len && a[child]<a[child+1])
			++child;
		if(a[s]<a[child])
		{
			a[s]=a[child];
			s=child;
			child=2*s+1;
		}
		else
		{
			break;
		}
		a[s]=temp;
	}
}

void buildingheap(vector<int> &a)
{
	for(int i=(a.size()-1)/2; i>=0; i--)
	{
		heapadjust(a, i, a.size());
	}
}

/*------------------------heap sort-------------------------------*/
void heapsort(vector<int> &a)
{
	buildingheap(a);
	for(int i=a.size()-1; i>0; i--)
	{
		int temp=a[i];
		a[i]=a[0];
		a[0]=temp;
		heapadjust(a, 0, i);
	}
}

/*------------------------insert sort---------------------------------*/
void insertsort(vector<int> &a)
{
	for(int i=1; i<a.size(); i++)
	{
		if(a[i-1]>a[i])
		{
			int temp=a[i];
			a[i]=a[i-1];
			int j=i-1;
			while(j >=0 && temp<a[j])
			{
				a[j+1]=a[j];
				j--;
			}
			a[j+1]=temp;
		}
	}
}

void shellinsertsort(vector<int> &a, int n, int dk)
{
	for(int i=dk; i<n; i++)
	{
		if(a[i]<a[i-dk])
		{
			int j=i-dk;
			int temp=a[i];
			a[i]=a[i-dk];
			while( j >=0 && temp<a[j])
			{
				a[j+dk]=a[j];
				j-=dk;
			}
			a[j+dk]=temp;
		}
	}
}

/*----------------------shell sort---------------------------*/
void shellsort(vector<int> &a)
{
	int dk=a.size()/2;
	while(dk>=1)
	{
		shellinsertsort(a, a.size(), dk);
		dk=dk/2;
	}
}

int main()
{
	int array[]={3,1,5,7,2,4,9,6,10,8};
	size_t count=sizeof(array)/sizeof(int);
	vector<int> a(array, array+count);
	cout<<"初始值："<<endl;
	print(a);
	//cout<<"快速排序结果："<<endl;
	//quicksort(a, 0, a.size()-1);
	//print(a);
	//cout<<"冒泡排序结果："<<endl;
	//bubblesort(a);
    //print(a);
	//cout<<"简单选择排序结果："<<endl;
	//selectsort(a);
	//print(a);
	//cout<<"堆排序结果："<<endl;
	//heapsort(a);
	//print(a);
	//cout<<"直接插入排序结果："<<endl;
	//insertsort(a);
	//print(a);
	cout<<"希尔排序结果："<<endl;
	shellsort(a);
	print(a);
}

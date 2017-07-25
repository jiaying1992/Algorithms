// kmp string search
// author: Ying Jia
// time: 20170725
#include<iostream>

using namespace std;

int ViolenceSearch(string s, string p)
{
	int i=0, j=0;
	while(i<s.size() && j<p.size())
	{
		if(s[i]==p[j])
		{
			i++;
			j++;
		}
		else
		{
			i=i-j+1;
			j=0;
		}
	}
	if(j==p.size()) return i-j;
	else return -1;
}

void GetNext(string s, int next[])
{
	next[0]=-1;
	int k=-1;
	int j=0;
	while(j< (s.size()-1))
	{
		if(k==-1 || s[k]==s[j])
		{
			k++;
			j++;
			if(s[j]!=s[k])
			{
				next[j]=k;
			}
			else
			{
				next[j]=next[k];
			}
		}
		else
		{
			k=next[k];
		}
	}
}

int KmpSearch(string s, string p)
{
	int plength=p.size();
	int slength=s.size();
	int *nextkmp;
	nextkmp=(int *) malloc(sizeof(int)*plength);
	GetNext(p, nextkmp);
	int i=0, j=0;
	while(i<slength && j<plength)
	{
		
		if(j==-1 || s[i]==p[j])
		{
			i++;
			j++;
		}
		else
		{
			j = nextkmp[j];
		}
	}
	if(j == p.size())
	{
		return i-j;
	}
	else
	{
		return -1;
	}
	free(nextkmp);
}
int main()
{
	string s="abcehgnabcdemg";
	string p="abcde";
	int result1=KmpSearch(s, p);
	cout<<result1<<endl;
	int result2=ViolenceSearch(s, p);
	cout<<result2<<endl;
}

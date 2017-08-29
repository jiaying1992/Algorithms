// author: YingJia
// time: 20170829
// contact: 1439352516@qq.com
#include<iostream>

using namespace std;

struct ListNode {
	int val;
	struct ListNode* next;
	ListNode(int x):val(x), next(NULL){}
};

int main()
{
	ListNode *head=new ListNode(0);
	head->next=NULL;
	ListNode *temp;
	int val;
	// 头插法
	/*while(cin>>val)
	{
		ListNode* node=new ListNode(val);
		node->next = head->next;
		head->next=node;
	}
   temp=head;*/
	// 尾插法
	temp=head;
	while(cin>>val)
	{
		ListNode* node=new ListNode(val);
		head->next=node;
		head=node;
	}
	// 实现奇偶位置节点互换
	ListNode* tmp;
	ListNode* pre=temp;
	ListNode* next=temp->next->next;
	ListNode* curr=temp->next;
	while(next!=NULL)
	{
		tmp=next->next;
		pre->next=next;
		next->next=curr;
		curr->next=tmp;
		pre=curr;
		curr=tmp;
		next=tmp->next;
	}

	while(temp->next!=NULL)
	{
		temp=temp->next;
		cout<<temp->val<<' ';
	}
	return 0;
}

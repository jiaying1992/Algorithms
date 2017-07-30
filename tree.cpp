// author: Ying Jia
// time: 20170728
// contact: 1439352516@qq.com

#include <iostream>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;


/*-------------the node of the binary tree-------------------------------*/
class BinaryTreeNode{
private:    
	int val;
	BinaryTreeNode *pLeft;
	BinaryTreeNode *pRight;
public:
	BinaryTreeNode( const int &item, BinaryTreeNode *lPtr = NULL, BinaryTreeNode *rPtr = NULL): val(item), pLeft(lPtr), pRight(rPtr){}
	void set_data(int item){val=item;}
	int get_data()const{return val;}
	void set_left(BinaryTreeNode *l){pLeft=l;}
	BinaryTreeNode *get_left()const{return pLeft;}
	void set_right(BinaryTreeNode *r){pRight=r;}
	BinaryTreeNode *get_right()const{return pRight;}
};

/*----------------the binary tree-----------------------------------------*/
class BinaryTree{
private:
	BinaryTreeNode *head;
public:
	BinaryTree(BinaryTreeNode *t=NULL):head(t){}
	~BinaryTree(){delete [] head;}
	void set_head(BinaryTreeNode* t){head=t;}
	BinaryTreeNode *get_head()const{return head;}
	BinaryTreeNode *create_tree();
	void pre_order(BinaryTreeNode *h) const;
	void pre_order1(BinaryTreeNode *h) const;
	void in_order(BinaryTreeNode *h) const;
	void in_order1(BinaryTreeNode *h) const;
	void post_order(BinaryTreeNode *h) const;
	void post_order1(BinaryTreeNode *h) const;
	void level_order(BinaryTreeNode *h) const;
	int get_leaf_num(BinaryTreeNode *h) const;
	int get_tree_height(BinaryTreeNode *h) const;
	bool is_binary_balancetree(BinaryTreeNode *h) const;
	void swap_left_right(BinaryTreeNode *h);
	BinaryTreeNode *get_nearest_common_father(BinaryTreeNode *h, BinaryTreeNode *pNode1, BinaryTreeNode *pNode2) const;
	void print_root(BinaryTreeNode *h, int sum) const;
	bool is_in_tree(BinaryTreeNode *h, BinaryTreeNode *pNode) const;
};
 /*------------------construct a binary tree by pre-order---------------------*/
BinaryTreeNode* BinaryTree::create_tree()
{
	BinaryTreeNode *head, *left, *right;
	char val;
	//cout<<" constructing a binary tree."<<endl;
	//cout<<" please input the node val. "<<endl;
	cin>>val;
	if(val != '#')
	{
		head = new BinaryTreeNode(val-48);
		left = create_tree();
		head->set_left(left);
		right = create_tree();
		head->set_right(right);
		return head;
	}
	else
	{
		head = NULL;
		return head;
	}
}

/*-----------------preorder traversal(recursion)------------------*/
void BinaryTree::pre_order(BinaryTreeNode *h) const
{
	if(h!=NULL)
	{
		cout<<h->get_data()<<' ';
		pre_order(h->get_left());
		pre_order(h->get_right());
	}
}

/*-----------------preorder traversal------------------*/
void BinaryTree::pre_order1(BinaryTreeNode *h) const
{
	if(h == NULL)
		return;
	stack<BinaryTreeNode*> s;
	BinaryTreeNode *p=h;
	while(p!=NULL || !s.empty())
	{
		while(p!=NULL)
		{
			cout<<p->get_data()<<' ';
			s.push(p);
			p=p->get_left();
		}
		if(!s.empty())
		{
			p=s.top();
			s.pop();
			p=p->get_right();
		}
	}

}

/*-----------------inorder traversal(recursion)-------------------*/
void BinaryTree::in_order(BinaryTreeNode *h) const
{
	if(h!=NULL)
	{
		in_order(h->get_left());
		cout<<h->get_data()<<' ';
		in_order(h->get_right());
    }
}

/*-----------------inorder traversal-------------------*/
void BinaryTree::in_order1(BinaryTreeNode *h) const
{
	if(h == NULL)
		return;
	stack<BinaryTreeNode*> s;
	BinaryTreeNode *p=h;
	while(p!=NULL || !s.empty())
	{
		while(p!=NULL)
		{
			s.push(p);
			p=p->get_left();
		}
		if(!s.empty())
		{
			p=s.top();
			cout<<p->get_data()<<' ';
			s.pop();
			p=p->get_right();
		}
	}
}

/*------------------postorder traverscal(recursion)-----------------------*/
void BinaryTree::post_order(BinaryTreeNode *h) const
{
	if(h!=NULL)
	{
		post_order(h->get_left());
		post_order(h->get_right());
		cout<<h->get_data()<<' ';
	}
}

/*------------------postorder traverscal-----------------------*/
void BinaryTree::post_order1(BinaryTreeNode *h) const
{
	if(h==NULL)
		return;
	stack<BinaryTreeNode*> s;
	BinaryTreeNode *cur;
	BinaryTreeNode *pre=NULL;
	s.push(h);
	while(!s.empty())
	{
		cur=s.top();
		if((cur->get_left()==NULL && cur->get_right()==NULL) || (pre!=NULL && (pre==cur->get_left() || pre==cur->get_right())))
		{
			cout<<cur->get_data()<<' ';
			s.pop();
			pre=cur;
		}
		else
		{
			if(cur->get_right()!=NULL)
				s.push(cur->get_right());
			if(cur->get_left()!=NULL)
				s.push(cur->get_left());
		}
	}
}

/*-----------------------levelorder traversal(queue)-----------------*/
void BinaryTree::level_order(BinaryTreeNode *h) const
{
	if(h==NULL) return;
	queue<BinaryTreeNode*> q;
	q.push(h);
	while(!q.empty())
	{
		BinaryTreeNode *p = q.front();
		cout<<p->get_data()<<' ';
		q.pop();
		if(p->get_left() != NULL)
			q.push(p->get_left());
		if(p->get_right() != NULL)
			q.push(p->get_right());
	}
}

/*-------------------------compute the number of leaf node(recursion)--------*/
int BinaryTree::get_leaf_num(BinaryTreeNode *h) const
{
	if(h == NULL)
		return 0;
	if(h->get_left() == NULL && h->get_right() == NULL)
		return 1;
	return get_leaf_num(h->get_left())+get_leaf_num(h->get_right());
}

/*-----------------------compute the height of binary tree(recursion)-----------------*/
int BinaryTree::get_tree_height(BinaryTreeNode *h) const
{
	if(h==NULL)
		return 0;
	return 1+max(get_tree_height(h->get_left()), get_tree_height(h->get_right()));
}

/*-----------------------if binary balance tree--------------------------*/
bool BinaryTree::is_binary_balancetree(BinaryTreeNode *h) const
{
	if(h==NULL)
		return true;
	int depth=get_tree_height(h->get_left())-get_tree_height(h->get_right());
	if(depth>1 || depth<-1)
		return false;
	else
		return is_binary_balancetree(h->get_left()) && is_binary_balancetree(h->get_right());
}

/*--------------------swap left and right(recursion)-----------------*/
void BinaryTree::swap_left_right(BinaryTreeNode *h)
{
	if(h == NULL)
		return;
	BinaryTreeNode *temp=h->get_left();
	h->set_left(h->get_right());
	h->set_right(temp);
	swap_left_right(h->get_left());
	swap_left_right(h->get_right());
}

/*----------------------if a node is in a tree(recursion)------------------------*/
bool BinaryTree::is_in_tree(BinaryTreeNode *h, BinaryTreeNode *pNode) const
{
	if(h == NULL)
		return false;
	else if(h == pNode)
		return true;
	else
	{
		bool res=false;
		if(h->get_left()!=NULL)
			res=is_in_tree(h->get_left(), pNode);
		if( !res && h->get_right()!= NULL)
			res=is_in_tree(h->get_right(), pNode);
		return res;
	}
}

/*----------------get the nearest common father of two nodes(recursion)----------*/
BinaryTreeNode* BinaryTree::get_nearest_common_father(BinaryTreeNode *h, BinaryTreeNode *pNode1, BinaryTreeNode *pNode2) const
{
	//if(is_in_tree(pNode1, pNode2))
		//return pNode1;
	//if(is_in_tree(pNode2, pNode1))
	//	return pNode2;
	if(h==NULL)
		return NULL;
	if(h==pNode1 || h==pNode2)
		return h;
	BinaryTreeNode *left=get_nearest_common_father(h->get_left(), pNode1, pNode2);
	BinaryTreeNode *right=get_nearest_common_father(h->get_right(), pNode1, pNode2);
	if(left && right)
		return h;
	return left? left:right;
	/*bool left1, left2, right1, right2;
	left1=is_in_tree(h->get_left(), pNode1);
	left2=is_in_tree(h->get_left(), pNode2);
	right1=is_in_tree(h->get_right(), pNode1);
	right2=is_in_tree(h->get_right(), pNode2);
	if((left1 && right2) || (left2 && right1))
		return h;
	else if(left1 && left2)
		return get_nearest_common_father(h->get_left(), pNode1, pNode2);
	else if(right1 && right2)
		return get_nearest_common_father(h->get_right(), pNode1, pNode2);
	else
		return NULL;*/
}

stack<BinaryTreeNode*> dfs;
stack<BinaryTreeNode*> print;
/*--------------------print path which equal to sum----------*/
void BinaryTree::print_root(BinaryTreeNode *h, int sum) const
{
	//stack<BinaryTreeNode*> dfs;
	//stack<BinaryTreeNode*> print;
	if(h == NULL)
		return;
	sum-=h->get_data();
	dfs.push(h);
	if(sum<=0)
	{
		if(sum==0)
		{
			while(!dfs.empty())
			{
				print.push(dfs.top());
				dfs.pop();
			}
			while(!print.empty())
			{
				cout<<print.top()->get_data()<<' ';
				dfs.push(print.top());
				print.pop();
			}
			cout<<endl;
		}
		sum+=h->get_data();
		dfs.pop();
		return;
	}
	print_root(h->get_left(), sum);
	print_root(h->get_right(), sum);
	sum+=h->get_data();
	dfs.pop();
}

int main()
  {
      BinaryTree tree;
      /*--------------------------------------------------------------------------*/
      cout<<"请输入二叉树前序序列进行建树，'#'代表空节点："<<endl;
      tree.set_head(tree.create_tree());
      cout<<endl;
      /*--------------------------------------------------------------------------*/
      cout<<"前序遍历的结果：";
     tree.pre_order(tree.get_head());
     cout<<endl<<endl;
	  /*--------------------------------------------------------------------------*/
      cout<<"前序遍历的结果(非递归）：";
     tree.pre_order1(tree.get_head());
     cout<<endl<<endl;
     /*--------------------------------------------------------------------------*/
     cout<<"中序遍历的结果：";
     tree.in_order(tree.get_head());
     cout<<endl<<endl;
	 /*--------------------------------------------------------------------------*/
     cout<<"中序遍历的结果（非递归）：";
     tree.in_order1(tree.get_head());
     cout<<endl<<endl;
     /*--------------------------------------------------------------------------*/
     cout<<"后序遍历的结果：";
     tree.post_order(tree.get_head());
     cout<<endl<<endl;
	  /*--------------------------------------------------------------------------*/
     cout<<"后序遍历的结果（非递归）：";
     tree.post_order1(tree.get_head());
     cout<<endl<<endl;
     /*--------------------------------------------------------------------------*/
     cout<<"层次遍历的结果：";
     tree.level_order(tree.get_head());
     cout<<endl<<endl;
     /*--------------------------------------------------------------------------*/
     cout<<"该二叉树叶子节点的个数：";
     cout<<tree.get_leaf_num(tree.get_head())<<endl<<endl;
     /*--------------------------------------------------------------------------*/
     cout<<"该二叉树的高度是：";
     cout<<tree.get_tree_height(tree.get_head())<<endl<<endl;
     /*--------------------------------------------------------------------------*/
	 cout<<"该二叉树是否是平衡二叉树：";
	 cout<<tree.is_binary_balancetree(tree.get_head())<<endl<<endl;
	 /*----------------------------------------------------------------------------*/
     tree.swap_left_right(tree.get_head());
     cout<<"交换左右子树之后的先序遍历结果为：";
     tree.pre_order(tree.get_head());
     cout<<endl<<endl;
     /*--------------------------------------------------------------------------*/
     BinaryTreeNode *p1 = tree.get_head()->get_left()->get_right();
     BinaryTreeNode *p2 = tree.get_head()->get_left()->get_left();
     BinaryTreeNode *p3 = tree.get_head()->get_right()->get_right()->get_right();
     cout<<p1->get_data()<<" 和 "<<p2->get_data()<<"的最近公共祖先是：";
     BinaryTreeNode *p = tree.get_nearest_common_father(tree.get_head(),p1,p2);
     cout<<p->get_data()<<endl;
     cout<<p1->get_data()<<" 和 "<<p3->get_data()<<"的最近公共祖先是：";
     p = tree.get_nearest_common_father(tree.get_head(),p1,p3);
     cout<<p->get_data()<<endl<<endl;
     /*--------------------------------------------------------------------------*/
     cout<<"路径如下："<<endl;
     tree.print_root(tree.get_head(),12);
     return 0;
 }

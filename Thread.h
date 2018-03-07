#include<stdio.h>
#include<assert.h>
#include<stdlib.h>

typedef int DataType;

typedef enum PtrTag
{
	THREAD, // 线索化 
	LINK, // 链接左右孩纸 
}PtrTag;

typedef struct BinaryTreeNodeThd 
{
	DataType _data;
	struct BinaryTreeNodeThd* _left;
	struct BinaryTreeNodeThd* _right;
	struct BinaryTreeNodeThd* _parent;
	PtrTag _leftTag;
	PtrTag _rightTag;
}BTNodeThd;

BTNodeThd* BuyBTNodeThd(DataType x)
{
	BTNodeThd* node = (BTNodeThd*)malloc(sizeof(BTNodeThd));
	assert(node);
	node->_left = NULL;
	node->_right = NULL;
	node->_parent = NULL;
	node->_leftTag = LINK;
	node->_rightTag = LINK;
	node->_data = x;
	return node;
}

BTNodeThd* CreateBTreeThd(DataType* a, DataType invalid, size_t* pIndex) 
{
	BTNodeThd* root = NULL;
	assert(a&&pIndex);
	if (a[*pIndex] != invalid) 
	{
		root = BuyBTNodeThd(a[*pIndex]);
		++(*pIndex);
		root->_left = CreateBTreeThd(a, invalid, pIndex);
		++(*pIndex);
		root->_right = CreateBTreeThd(a, invalid, pIndex);
		if (root->_left)
			root->_left->_parent = root;
		if (root->_right)
			root->_right->_parent = root;
	}
	return root;
}

void BTreeThdInOrder(BTNodeThd* root) //中序
{
	if (root == NULL)
		return;
	BTreeThdInOrder(root->_left);
	printf("%d ", root->_data);
	BTreeThdInOrder(root->_right);
}

void BTreeThdInOrderThreading(BTNodeThd* cur, BTNodeThd** pprev) 
{
	if (cur == NULL)
		return;
	BTreeThdInOrderThreading(cur->_left, &(*pprev));
	if (cur->_left == NULL)
	{
		cur->_left = *pprev;
		cur->_leftTag = THREAD;//线索左孩子
	}
	if ((*pprev) != NULL && (*pprev)->_right == NULL) 
	{
		(*pprev)->_right = cur;
		(*pprev)->_rightTag = THREAD;
	}
	*pprev = cur;
	BTreeThdInOrderThreading(cur->_right, &(*pprev));
}

BTNodeThd* BTreeInOrderNext(BTNodeThd* cur)
{
	if (cur->_rightTag == THREAD)
		return cur->_right;
	cur = cur->_right;
	while ((cur->_leftTag == LINK) && cur->_left)     //如果节点的右侧不是线索，则要以中序的方法找下一个节点
		cur = cur->_left;
	return cur;

}
void BTreeThdInOrderThd(BTNodeThd* root)
{
	BTNodeThd* cur = root;
	if (root == NULL)
		return;
	while (cur->_leftTag == LINK)
		cur = cur->_left;
	while (cur != NULL)
	{
		printf("%d ", cur->_data);
		cur = BTreeInOrderNext(cur);
	}
	printf("\n");
}

void BTreeThdPrevOrderThreading(BTNodeThd* cur, BTNodeThd** pprev) 
{
	if (cur == NULL)
		return;
	if (cur->_left == NULL) 
	{
		cur->_left = *pprev;
		cur->_leftTag = THREAD;
	}
	if (*pprev != NULL && (*pprev)->_right == NULL) 
	{
		(*pprev)->_right = cur;
		(*pprev)->_rightTag = THREAD;
	}
	(*pprev) = cur;
	if (cur->_leftTag == LINK)
		BTreeThdPrevOrderThreading(cur->_left, &(*pprev));
	if (cur->_rightTag == LINK)
		BTreeThdPrevOrderThreading(cur->_right, &(*pprev));
}

BTNodeThd* BTreePrevOrderNext(BTNodeThd* cur)
{
	if (cur->_left != NULL && cur->_leftTag == LINK)
		return cur->_left;
	else
		return cur->_right;
}

void BTreeThdPrevOrderThd(BTNodeThd* root)
{
	BTNodeThd* cur = root;

	if (root == NULL)
		return;

	while (cur)
	{
		printf("%d ", cur->_data);
		cur = BTreePrevOrderNext(cur);
	}
	printf("\n");
}


void TestBTreeThd()
{
	BTNodeThd* prev = NULL;
	int a[] = { 1,2,3,'#','#',4,'#','#',5,6,'#','#','#' };
	BTNodeThd* tree1 = NULL, *tree2 = NULL;
	size_t index = 0;
	tree1 = CreateBTreeThd(a, '#', &index);
	printf("中序遍历>");
	BTreeThdInOrder(tree1);
	BTreeThdInOrderThreading(tree1, &prev);
	prev->_rightTag = THREAD;
	printf("\n");
	printf("线索化中序遍历>");
	BTreeThdInOrderThd(tree1);
	index = 0;
	tree2 = CreateBTreeThd(a, '#', &index);
	BTreeThdPrevOrderThreading(tree2, &prev);
	prev->_rightTag = THREAD;
	printf("前序遍历>");
	BTreeThdPrevOrderThd(tree2);
}
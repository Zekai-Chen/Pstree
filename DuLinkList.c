

#include "DuLinkList.h"


void DuLinkListToArr(DuLinkList* list_inStack, DuLinkList_Arr* parr_inStack) {
	//特殊情况处理
	if (list_inStack->cursize == 0) {
		parr_inStack->arr = NULL;
		parr_inStack->size = 0;
		return;
	}

	parr_inStack->arr = (DuLinkList_ArrNode*)malloc(sizeof(DuLinkList_ArrNode) * list_inStack->cursize);
	parr_inStack->size = list_inStack->cursize;
	DuLNode* curNode = list_inStack->head->next;
	for (int i = 0; i < list_inStack->cursize; i++) {
		parr_inStack->arr[i].data = curNode->data;
		parr_inStack->arr[i].pOri = curNode;
		curNode = curNode->next;
	}
}

void ArrToDuLinkList(DuLinkList_Arr* parr_inStack, DuLinkList* list_inStack) {

	//初始化一下list_inStack head两边指向自身 cursize是0
	//(断开head的前后)
	list_inStack->head->next = list_inStack->head;
	list_inStack->head->prev = list_inStack->head;
	list_inStack->cursize = 0;

	if (parr_inStack->size == 0) {
		//长度为0 直接return
		return;
	}

	for (int i = 0; i < parr_inStack->size; i++) {
		//Insert_Prev(head)即为尾插(循环链表head前面就是尾部)
		//Insert_Prev的时候,进去链表的node前后会重新设置,相当于断开重设
		Insert_Prev(list_inStack, list_inStack->head, parr_inStack->arr[i].pOri);
	}

	//DuLNode* curNode = parr_inStack->arr[0].pOri;
	//DuLNode* prevNode = NULL;
	//DuLNode* nextNode = NULL;
	//list_inStack->head = curNode;
	//for (int i = 0; i < parr_inStack->size; i++) {
	//	//prev		
	//	if (i == 0) {
	//		prevNode = NULL;
	//	}
	//	else {
	//		prevNode = parr_inStack->arr[i - 1].pOri;
	//	}
	//	//next	
	//	if (i == parr_inStack->size-1) {
	//		nextNode = NULL;
	//	}
	//	else {
	//		nextNode = parr_inStack->arr[i + 1].pOri;
	//	}
	//	//prev尾,self头,self尾,next头 4个部位(4个指针)
	//	if (prevNode) {
	//		prevNode->next = curNode;
	//	}
	//	curNode->prev = prevNode;
	//	curNode->next = nextNode;
	//	if (nextNode) {
	//		nextNode->prev = curNode;
	//	}
	//}
	//list_inStack->cursize = parr_inStack->size;
}

DuLNode* NewNode()	//创建
{
	//DuLNode* s = (DuLNode*)malloc(sizeof(DuLNode));
	//if(NULL == s) exit(1);
	//memset(s, 0, sizeof(DuLNode));
	//return s;
	DuLNode* s = (DuLNode*)calloc(1, sizeof(DuLNode));//calloc会置0
	if (NULL == s)
	{
		exit(1);
	}
	return s;
}

void FreeNode(DuLNode* p)//释放
{
	free(p);// p 
}

//InitList;
//DestroyList;
//Insert_Prev;


void InitList(DuLinkList* plist)
{
	//这是个双向循环链表,初始化0长度时自带一个head节点其data始终为0
	assert(plist != NULL);
	DuLNode* s = NewNode();//其data为0
	//其next和prev指向自身
	s->next = s;
	s->prev = s;
	plist->head = s;
	plist->cursize = 0;
}

void Insert_Prev(DuLinkList* plist, DuLNode* target_node, DuLNode* pnode)
{//Insert_Prev意思是插入(Insert)位置为target_node的前面(Prev)
	assert(plist != NULL && target_node != NULL && pnode != NULL);
	pnode->next = target_node;
	pnode->prev = target_node->prev;
	target_node->prev = pnode;
	pnode->prev->next = pnode;
	plist->cursize += 1;
}

DuLNode* InsertElem_Prev(DuLinkList* plist, DuLNode* target_node, ElemType val)
{
	assert(plist != NULL && target_node != NULL);
	DuLNode* s = NewNode();
	s->data = val;
	Insert_Prev(plist, target_node, s);//可以看出来这个函数为什么叫Insert_Prev,因为它的参数就是pre
	return s;
}
void Push_Front(DuLinkList* plist, ElemType val)
{
	InsertElem_Prev(plist, plist->head->next, val);
}
DuLNode* Push_Back(DuLinkList* plist, ElemType val)
{
	return InsertElem_Prev(plist, plist->head, val);
}
void EarseList(DuLinkList* plist, DuLNode* ptr)
{
	assert(plist != NULL && ptr != NULL);
	ptr->next->prev = ptr->prev;
	ptr->prev->next = ptr->next;
	FreeNode(ptr);
	plist->cursize -= 1;
}

void Pop_Back(DuLinkList* plist)
{
	assert(plist != NULL);
	if (plist->cursize == 0) return;
	EarseList(plist, plist->head->prev);
}
void Pop_Front(DuLinkList* plist)
{
	assert(plist != NULL);
	if (plist->cursize == 0) return;
	EarseList(plist, plist->head->next);
}



void CloseList(DuLinkList* plist)
{
	assert(plist);
	while (plist->head->next != plist->head)
	{
		DuLNode* q = plist->head->next;
		EarseList(plist, q);
	}
}
void DestroyList(DuLinkList* plist)
{
	assert(plist != NULL);
	CloseList(plist);
	FreeNode(plist->head);
	plist->head = NULL;
}


void PrintList(DuLinkList* plist)
{
	assert(plist != NULL);
	DuLNode* p = plist->head->next; // frist;
	printf("[start][\n");
	while (p != plist->head)
	{
		printf("%d ", p->data.pid);
		p = p->next;
	}
	printf("][end]\n");
}


//--------------------------------
// 文件名	:仿制pstree
// 创建时间	:2022/10/8 14:06:18
//--------------------------------
//https://github.com/SiyuanYue/NJUOSLab-M1-pstree/blob/main/pstree.c
//http://jyywiki.cn/OS/2022/labs/M1
//https://www.atatus.com/blog/pstree-command-in-linux/
// https://www.codenong.com/11528267/
// http://en.verysource.com/code/30665376_1/pstree.c.html
//-n 按数字升序排列所有进程
//-p 在名字后面显示pid
//-V 显示pstree的版本号
//-u 在名字后面显示进程所属用户
//-g 在名字后面显示进程组id
//链表快排[先排指针然后整理]//https://www.cnblogs.com/xylc/p/3795018.html
//#include "DuLinkList.h"
#include "DuLinkList.h"
#include "math.h"
HelperNode HARR[100000] = {0};//helperArr
int MAX_PID = 0;
DuLNode* pROOT = NULL;//树的根节点

int max(int a, int b) {
	if (a>=b)
	{
		return a;
	}
	return b;
}

int isSpace(char c) {
	if (c == 9		//TAB
		|| c == 10		//LF换行
		|| c == 13		//CR回车
		|| c == 32		//空格
		)
	{
		return 1;
	}
	return 0;
}
	//略过连续的空格
char* SkipWhiteSpace(char* p) {

	while (1) {
		if (!isSpace(*p))return p;
		p++;
	}

	return p;
}

//读取一个非空格字符串
char* ReadElem(char* p, char* elem) {

	char* start = p;

	while (1)
	{
		if (isSpace(*p))break;
		p++;
	}
	strncpy(elem, start, p - start);
	elem[p - start] = '\0';
	return p;

}

void getInfoIn_status(char* status_path,int* ppid,int* uid,int *gid,char* name) {
	FILE* fp = fopen(status_path, "r");
	if (!fp)
	{
		printf("cant open %s", status_path);
		//pause();
		exit(1);
	}
	while (!feof(fp)) {
		char line[256] = { 0 };
		for (int i = 1; i <= 10; i++) {
			//第一行 name
			//第七行 ppid
			//第九行 uid
			//第十行 gid
			fgets(line, sizeof(line), fp);
			//fgets是读取文件的一行,最后的\n也会读入到一参里面(一参正常结尾是\n\0)
			//fgets二参数一般填一参字符串buf长度即可
			//printf("%s", line);
			if (i == 1) {
				//printf("%s", line);
				char* p = line;
				char elem[128] = { 0 };

				//Name:
				p = SkipWhiteSpace(p);
				p = ReadElem(p, elem);
				//printf("%s\n", elem);

				//name
				p = SkipWhiteSpace(p);
				p = ReadElem(p, elem);
				//printf("name=%s\n", elem);
				strcpy(name, elem);
			}
			else if (i == 7) {
				//printf("%s", line);
				char* p = line;
				char elem[128] = { 0 };

				//PPID:
				p = SkipWhiteSpace(p);
				p = ReadElem(p, elem);
				//printf("%s\n", elem);

				//ppid
				p = SkipWhiteSpace(p);
				p = ReadElem(p, elem);
				//printf("ppid=%s\n", elem);
				int int_ppid = atoi(elem);
				assert(int_ppid >= 0);
				*ppid = int_ppid;
			}
			else if (i == 9) {
				//printf("%s", line);
				
				char* p = line;
				char elem[128] = { 0 };

				//UID:
				p = SkipWhiteSpace(p);
				p = ReadElem(p, elem);
				//printf("%s\n", elem);

				//uid
				p = SkipWhiteSpace(p);
				p = ReadElem(p, elem);
				//printf("uid=%s\n", elem);
				int int_uid = atoi(elem);
				assert(int_uid >= 0);
				*uid = int_uid;

			}else if (i == 10) {
				//printf("%s", line);
				
				char* p = line;
				char elem[128] = { 0 };

				//GID:
				p = SkipWhiteSpace(p);
				p = ReadElem(p, elem);
				//printf("%s\n", elem);

				//gid
				p = SkipWhiteSpace(p);
				p = ReadElem(p, elem);
				int int_gid = atoi(elem);
				assert(int_gid >= 0);
				*gid = int_gid;
				//printf("uid=%d\n", int_gid);

			}

			

		}
		
		break;
	}
	fclose(fp);

}

char* uidToName(uid_t uid)
{
	struct passwd* pw_ptr;
	static char numstr[10];

	if ((pw_ptr = getpwuid(uid)) == NULL)
	{
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name;
}

void ReadAndStoreAllProcessInfo() {
	DIR* dp = opendir("/proc");
	if (!dp)
	{
		printf("无法打开/proc/");
		//pause();
		exit(1);
	}
	else
	{
		

		struct dirent* entry;
		while ((entry = readdir(dp)) != NULL)
		{
			int pid = 0;
			int ppid = 0;
			int uid = 0;
			int gid = 0;
			char name[NAME_LEN] = { 0 };
			char status_path[256] = { 0 };
			if ((pid = atoi(entry->d_name)) == 0)
			{//遇到0和非数字就pass掉
				continue;
			}
			else {// store in pidinfo (name and pid and ppid)
				//printf("d_name : %s\n", entry->d_name);
				sprintf(status_path, "/proc/%d/status", pid);
				getInfoIn_status(status_path,&ppid,&uid,&gid,name);
				HARR[pid].flag_activated = 1;
				HARR[pid].ppid = ppid;
				HARR[pid].uid = uid;
				HARR[pid].gid = gid;
				strcpy(HARR[pid].name, name);
				HARR[pid].heapPointer = NULL;
				MAX_PID = max(MAX_PID, pid);
			}
		}
	}

}

void MakeTree() {

	for (int pid = 0; pid <= MAX_PID; pid++) {
		if (HARR[pid].flag_activated) {

			int ppid = HARR[pid].ppid;
			
			//
			DuLNode* parent_heapPointer;
			if (ppid == -1) {
				printf("MakeTree err: ppid == -1");
				//pause();
				exit(1);
			}
			else if (ppid == 0) {
				parent_heapPointer = pROOT;
			}
			else
			{
				parent_heapPointer = HARR[ppid].heapPointer;
			}

			if (parent_heapPointer->data.childsList.cursize==0)
			{
				InitList(&(parent_heapPointer->data.childsList));
			}
			PsTreeNode e = { 0 };
			e.pid = pid;
			e.ppid = ppid;
			e.uid = HARR[pid].uid;
			e.gid = HARR[pid].gid;
			strcpy(e.name, HARR[pid].name);

			DuLNode* self_heapPointer =Push_Back(&(parent_heapPointer->data.childsList), e);
			HARR[pid].heapPointer = self_heapPointer;
		}
	}

}

void _PrintTree(DuLNode* proot, int level, char option) {
//-n 按数字升序排列所有进程
//-p 在名字后面显示pid
//-V 显示pstree的版本号
//-u 在名字后面显示进程所属用户
//-g 在名字后面显示进程组id
//-Z 打开所有功能
	//打印版本号
	if (option == 'V') {
		printf("我的pstree 版本号:1.0\n");
		return;
	}


	//打印
	for (int i = 0; i < level; i++) {
		printf("  ");
	}
	//printf("%s(%d,%d)\n",proot->data.name, proot->data.pid, proot->data.ppid);
	if (option == '\0' || option == 'n') {
		printf("%s\n",proot->data.name);
	}
	else if (option == 'p') {
		printf("%s(pid=%d)\n", proot->data.name, proot->data.pid);
	}
	else if (option == 'u') {
		printf("%s(user=%s)\n", proot->data.name, uidToName(proot->data.uid));
	}
	else if (option == 'g') {
		printf("%s(gid=%d)\n", proot->data.name, proot->data.gid);
	}

	//为父节点的情况
	if (proot->data.childsList.cursize>0) {
		//排序链表
		if (option == 'n')
		{
			Qsort(&(proot->data.childsList), proot->data.childsList.cursize);
		}

		//遍历双向循环链表
		DuLNode* p = proot->data.childsList.head->next;
		while (p != proot->data.childsList.head)
		{
			level++;
			_PrintTree(p, level,option);
			level--;
			//printf("%d ", p->data);
			p = p->next;
		}
	}

}

void PrintTree(DuLNode* proot,char option) {
	_PrintTree(proot, 0, option);
	
}

int main(int argc, char *argv[])
{

	//printf("argc=%d\n",argc);
	char option;
	if (argc == 1) {
		option = '\0';
	}
	else if (argc == 2) {
		if (strlen(argv[1]) < 2) {
			printf("option输入错误,参数太短,请输入-n/p/V/u/g");
			//pause();
			exit(1);
		}
		else {
			option = argv[1][1];
		}
	}

	//
	//option = 'g';
	//
	//printf("option=%c(%d)\n", option, option);

	if (option != '\0'
		&& option != 'n'
		&& option != 'p'
		&& option != 'V'
		&& option != 'u'
		&& option != 'g'
		) {
		printf("option输入错误,参数字母不对,请输入-n/p/V/u/g");
		//pause();
		exit(1);
	}

	pROOT = NewNode();
	pROOT->data.pid = 0;
	pROOT->data.ppid = -1;
	pROOT->data.uid = 0;
	pROOT->data.gid = 0;
	strcpy(pROOT->data.name, "ROOT_pid0");


	HARR[0].flag_activated = 0;


	ReadAndStoreAllProcessInfo();

	MakeTree();

	PrintTree(pROOT->data.childsList.head->next, option);

	//pause();
    return 0;
}
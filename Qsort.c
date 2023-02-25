#include "Qsort.h"
#include "DuLinkList.h"

void swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int partition(DuLinkList_ArrNode* arr, int left, int right) {
	DuLinkList_ArrNode pivot = arr[left];
	//此时left虚位以待
	while (1) {
		//right 此时left虚位以待 所以 先 移动right 直到填一个 比pivot小的数 过去
		while (1) {
			///[排序逻辑关键位置01]if (arr[right].data.pid < pivot.data.pid) {//不用<=的原因 用也可以 就是感觉没必要移动相等的,少动一个省时间
			if (arr[right].data.pid < pivot.data.pid) {//不用<=的原因 用也可以 就是感觉没必要移动相等的,少动一个省时间
				arr[left] = arr[right];
				left++;
				if (left == right) { arr[left] = pivot; return left; }//返回pivot最终位置
				break;
			}
			else {// arr[right] > pivot
				right--;
				if (left == right) { arr[left] = pivot; return left; }//返回pivot最终位置
			}
		}
		//left 此时right虚位以待 移动left 直到填一个 比pivot小的数 过去
		while (1) {
			///[排序逻辑关键位置02]if (arr[left].data.pid > pivot.data.pid) {//不用>=的原因 用也可以 就是感觉没必要移动相等的,少动一个省时间
			if (arr[left].data.pid > pivot.data.pid) {//不用>=的原因 用也可以 就是感觉没必要移动相等的,少动一个省时间
				arr[right] = arr[left];
				right--;
				if (left == right) { arr[left] = pivot; return left; }//返回pivot最终位置
				break;
			}
			else {// arr[right] > pivot
				left++;
				if (left == right) { arr[left] = pivot; return left; }//返回pivot最终位置
			}
		}
	}
}

void _Qsort(DuLinkList_ArrNode* arr, int left, int right) {
	int pivotPos = partition(arr, left, right);
	//left~pivotPos-1 pivotPos+1~right
	if (left < pivotPos - 1)//那就是至少两个以上乱序元素了还是要partition一下
	{
		_Qsort(arr, left, pivotPos - 1);
	}
	else {
		//left==pivotPos-1 左区间 就一个数 不用继续partition
		//left==pivotPos  左区间 没有数 不用继续partition
		//skip
	}

	if (pivotPos + 1 < right)//那就是至少两个以上乱序元素了还是要partition一下
	{
		_Qsort(arr, pivotPos + 1, right);
	}
	else {
		//right==pivotPos-1 右区间 就一个数 不用继续partition
		//right==pivotPos  右区间 没有数 不用继续partition
		//skip
	}

}



void Qsort(DuLinkList* list, int size) {
	if (size <= 1)return;

	DuLinkList_Arr dularr;
	DuLinkListToArr(list, &dularr);

	_Qsort(dularr.arr, 0, size - 1);

	ArrToDuLinkList(&dularr, list);

	free(dularr.arr);
}



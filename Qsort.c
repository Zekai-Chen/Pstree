#include "Qsort.h"
#include "DuLinkList.h"

void swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int partition(DuLinkList_ArrNode* arr, int left, int right) {
	DuLinkList_ArrNode pivot = arr[left];
	//��ʱleft��λ�Դ�
	while (1) {
		//right ��ʱleft��λ�Դ� ���� �� �ƶ�right ֱ����һ�� ��pivotС���� ��ȥ
		while (1) {
			///[�����߼��ؼ�λ��01]if (arr[right].data.pid < pivot.data.pid) {//����<=��ԭ�� ��Ҳ���� ���Ǹо�û��Ҫ�ƶ���ȵ�,�ٶ�һ��ʡʱ��
			if (arr[right].data.pid < pivot.data.pid) {//����<=��ԭ�� ��Ҳ���� ���Ǹо�û��Ҫ�ƶ���ȵ�,�ٶ�һ��ʡʱ��
				arr[left] = arr[right];
				left++;
				if (left == right) { arr[left] = pivot; return left; }//����pivot����λ��
				break;
			}
			else {// arr[right] > pivot
				right--;
				if (left == right) { arr[left] = pivot; return left; }//����pivot����λ��
			}
		}
		//left ��ʱright��λ�Դ� �ƶ�left ֱ����һ�� ��pivotС���� ��ȥ
		while (1) {
			///[�����߼��ؼ�λ��02]if (arr[left].data.pid > pivot.data.pid) {//����>=��ԭ�� ��Ҳ���� ���Ǹо�û��Ҫ�ƶ���ȵ�,�ٶ�һ��ʡʱ��
			if (arr[left].data.pid > pivot.data.pid) {//����>=��ԭ�� ��Ҳ���� ���Ǹо�û��Ҫ�ƶ���ȵ�,�ٶ�һ��ʡʱ��
				arr[right] = arr[left];
				right--;
				if (left == right) { arr[left] = pivot; return left; }//����pivot����λ��
				break;
			}
			else {// arr[right] > pivot
				left++;
				if (left == right) { arr[left] = pivot; return left; }//����pivot����λ��
			}
		}
	}
}

void _Qsort(DuLinkList_ArrNode* arr, int left, int right) {
	int pivotPos = partition(arr, left, right);
	//left~pivotPos-1 pivotPos+1~right
	if (left < pivotPos - 1)//�Ǿ�������������������Ԫ���˻���Ҫpartitionһ��
	{
		_Qsort(arr, left, pivotPos - 1);
	}
	else {
		//left==pivotPos-1 ������ ��һ���� ���ü���partition
		//left==pivotPos  ������ û���� ���ü���partition
		//skip
	}

	if (pivotPos + 1 < right)//�Ǿ�������������������Ԫ���˻���Ҫpartitionһ��
	{
		_Qsort(arr, pivotPos + 1, right);
	}
	else {
		//right==pivotPos-1 ������ ��һ���� ���ü���partition
		//right==pivotPos  ������ û���� ���ü���partition
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



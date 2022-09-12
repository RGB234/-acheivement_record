#include <stdio.h>
#include <malloc.h>

#define SIZE_OF_NAME 14
#define SIZE_OF_SCORE 3

typedef struct score {
	char name[14];
	int score[3]; //����, ����, ����
	int ranking[3]; //����, ���, ����

} SCORE ;

typedef struct data {
	SCORE acheiv; //acheivement
	struct data* p_next;
} DATA ;

void AddData(DATA** pp_head, DATA** pp_tail, SCORE input) {

	int i, total=0, average=0, order=0;

	if (NULL == *pp_head) {
		*pp_head = (DATA*)malloc(sizeof(DATA));
		*pp_tail = *pp_head;
	
	}
	else {
		(*pp_tail)->p_next = (DATA*)malloc(sizeof(DATA));
		*pp_tail = (*pp_tail)->p_next;
	}

	/*�̸�*/
	for (i = 0; i < SIZE_OF_NAME; i++) {
		(*pp_tail)->acheiv.name[i] = input.name[i];
	}
	/*����*/
	for (i = 0; i < SIZE_OF_SCORE; i++) {
		
		(*pp_tail)->acheiv.score[i] = input.score[i];
	}
	/* ����, ���, ����*/

	(*pp_tail)->acheiv.ranking[0] = input.ranking[0];
	(*pp_tail)->acheiv.ranking[1] = input.ranking[1];
	(*pp_tail)->acheiv.ranking[2] = 0;  //�������� �ű�� �Լ��� �ϴ� ����

	(*pp_tail)->p_next = NULL;
}

void scoreSort(DATA* p_head){ //������ ���� �Լ�

	struct sort_Array {
		int score;
		struct sort_Array* q_next;
	};
	typedef struct sort_Array SORT_ARRAY;

	SORT_ARRAY* q_head = NULL;
	SORT_ARRAY* q;
	DATA* p = p_head; // p_head �� ������ ���� p_head ��� p ���(�߿�)
	int order; //����
	int countNumber = 0; //�л� ��

	// (1)---�� pp->acheiv.ranking[0], �� �������յ� �����ϱ� ���� ���Ḯ��Ʈ ���� �� �������� ����---
	
	q_head = (SORT_ARRAY*)malloc(sizeof(SORT_ARRAY));
	q = q_head;
	q->score = p->acheiv.ranking[0];
	q->q_next = NULL;

	p = p->p_next;
	
	while (p != NULL) {
		
		q->q_next = (SORT_ARRAY*)malloc(sizeof(SORT_ARRAY));
		q = q->q_next;

		q->score = p->acheiv.ranking[0];
		q->q_next = NULL;

		p = p->p_next;

	}
	////(test) q->score �� ����� �������յ� �� Ȯ��(���) -> Ȯ�ΰ�� ��������
	//q = q_head;
	//while (q != NULL) {
	//	printf("%d\n", q->score);
	//	q = q->q_next;
	//}
	


	// (2)--����� �������յ� ����--

	q = q_head; //���渮��Ʈ�� ��� �ʱ�ȭ

	while (q != NULL) {

		q = q->q_next;
		countNumber++; // ����� ��� ��, �� �л� ��
	}

	// SORTARRAY* q �������� ����
	for (int step = 0; step < countNumber - 1; step++) {

		int flag = 0;
		q = q_head;

		for (int i = 0; i < countNumber - 1 - step; i++) {

			if( (q->score) < (q->q_next->score) ){ //�������� ����
				int temp = q->score;
				q->score = q->q_next->score;
				q->q_next->score = temp;
				flag = 1;

			} 
			q = q->q_next;
		}
		if (flag == 0) break;
	} // SORTARRAY* q �������� ��

	////(test) q->score �� ����� ���� �� �������յ� �� Ȯ��(���) -> Ȯ�ΰ�� ��������
	//q = q_head;
	//while (q != NULL) {
	//	printf("%d\n", q->score);
	//	q = q->q_next;
	//}

	// (3)--���� ó��--
	
	p = p_head; //�� ���Ḯ��Ʈ�� ��� �ʱ�ȭ, q�� �ݺ��������� ��� ��� �ʱ�ȭ�� ����

	for (int n = 0; n < countNumber; n++) {

		order = 1;
		q = q_head;

		for (int i = 0; i < countNumber; i++) {
			
			if ( p->acheiv.ranking[0] == q->score ) {
				p->acheiv.ranking[2] = order;
				break;
				// ���� �� �л��� ���������� ���� �Ȱ����� �׳� ���������� ó��
			}
			else {
				q = q->q_next;
				order++;
			}
		}
		p = p->p_next;
	}

	while (NULL != q_head) {
		q = q_head;
		q_head = q->q_next;
		free(q);
	}
}


void main() {

	int choice, i = 1; // i�� �� ��° �Է����� �ǹ�
	DATA* p_head = NULL, * p_tail = NULL;
	DATA* p = NULL;

	while (1) {
		int n=0, total = 0;
		SCORE data = { 0, };

		printf("\n    [Menu]     \n");
		printf(" 1. ���� �Է� \n");
		printf(" 2. ���� Ȯ�� \n");
		printf(" 3. ���� \n\n");
		printf(" ------------ \n\n");
		printf(" ���� (1~3) : ");
		scanf("%d", &choice);


		if (choice == 1) {

			/* ����ü SCORE �Է� (���⼭�� SCORE data) */
			printf("%d ��° �л� �̸� �Է�: ", i);
			scanf("%s", &data.name);
			printf("���� ����: ");
			scanf("%d", &data.score[0]);
			printf("���� ����: ");
			scanf("%d", &data.score[1]);
			printf("���� ����: ");
			scanf("%d", &data.score[2]);

			for (n = 0; n < SIZE_OF_SCORE; n++) {
				total = total + data.score[n];
			}
			data.ranking[0] = total;
			data.ranking[1] = total / SIZE_OF_SCORE;
			data.ranking[2] = 0;

			AddData(&p_head, &p_tail, data); //���� ����Ʈ ����

			i++;
		}
		else if (choice == 2) {

			p = p_head; //p_head ���� ������ �ʱ� ���� p�� �̿�(�߿�)
			scoreSort(p);

			printf("---------------------------------------------------\n");
			printf("      �̸�      ����  ����  ����   ����  ���  ���\n");
			printf("---------------------------------------------------\n");
			while (NULL != p) {
				printf("%-14s  %-4d  %-4d  %-4d  %-4d  %-4d  %-4d\n",
					p->acheiv.name, p->acheiv.score[0], p->acheiv.score[1], p->acheiv.score[2], p->acheiv.ranking[0],
					p->acheiv.ranking[1], p->acheiv.ranking[2]);
				printf("\n");
				p = p->p_next;
			};
		}
			
		else if(choice ==3) { 
			break; 
			/*���Ḯ��Ʈ �����Ҵ� ����*/
			while (NULL != p_head) {
				p = p_head;
				p_head = p->p_next;
				free(p);
			}
			p_tail = p_head; //NULL
		}
		else { 
			printf("1~3�߿��� ����\n");
			rewind(stdin);
		}
	}
}
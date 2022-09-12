#include <stdio.h>
#include <malloc.h>

#define SIZE_OF_NAME 14
#define SIZE_OF_SCORE 3

typedef struct score {
	char name[14];
	int score[3]; //국어, 영어, 수학
	int ranking[3]; //총합, 평균, 순위

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

	/*이름*/
	for (i = 0; i < SIZE_OF_NAME; i++) {
		(*pp_tail)->acheiv.name[i] = input.name[i];
	}
	/*점수*/
	for (i = 0; i < SIZE_OF_SCORE; i++) {
		
		(*pp_tail)->acheiv.score[i] = input.score[i];
	}
	/* 총합, 평균, 순위*/

	(*pp_tail)->acheiv.ranking[0] = input.ranking[0];
	(*pp_tail)->acheiv.ranking[1] = input.ranking[1];
	(*pp_tail)->acheiv.ranking[2] = 0;  //순위까지 매기는 함수는 일단 보류

	(*pp_tail)->p_next = NULL;
}

void scoreSort(DATA* p_head){ //순위값 지정 함수

	struct sort_Array {
		int score;
		struct sort_Array* q_next;
	};
	typedef struct sort_Array SORT_ARRAY;

	SORT_ARRAY* q_head = NULL;
	SORT_ARRAY* q;
	DATA* p = p_head; // p_head 값 보존을 위해 p_head 대신 p 사용(중요)
	int order; //순위
	int countNumber = 0; //학생 수

	// (1)---각 pp->acheiv.ranking[0], 즉 점수총합들 저장하기 위한 연결리스트 구성 및 점수총합 저장---
	
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
	////(test) q->score 에 저장된 점수총합들 값 확인(출력) -> 확인결과 문제없음
	//q = q_head;
	//while (q != NULL) {
	//	printf("%d\n", q->score);
	//	q = q->q_next;
	//}
	


	// (2)--저장된 점수총합들 정렬--

	q = q_head; //연길리스트의 헤더 초기화

	while (q != NULL) {

		q = q->q_next;
		countNumber++; // 연결된 노드 수, 즉 학생 수
	}

	// SORTARRAY* q 점수정렬 시작
	for (int step = 0; step < countNumber - 1; step++) {

		int flag = 0;
		q = q_head;

		for (int i = 0; i < countNumber - 1 - step; i++) {

			if( (q->score) < (q->q_next->score) ){ //내림차순 정렬
				int temp = q->score;
				q->score = q->q_next->score;
				q->q_next->score = temp;
				flag = 1;

			} 
			q = q->q_next;
		}
		if (flag == 0) break;
	} // SORTARRAY* q 점수정렬 끝

	////(test) q->score 에 저장된 정렬 후 점수총합들 값 확인(출력) -> 확인결과 문제없음
	//q = q_head;
	//while (q != NULL) {
	//	printf("%d\n", q->score);
	//	q = q->q_next;
	//}

	// (3)--순위 처리--
	
	p = p_head; //각 연결리스트의 헤더 초기화, q는 반복과정에서 계속 헤더 초기화할 예정

	for (int n = 0; n < countNumber; n++) {

		order = 1;
		q = q_head;

		for (int i = 0; i < countNumber; i++) {
			
			if ( p->acheiv.ranking[0] == q->score ) {
				p->acheiv.ranking[2] = order;
				break;
				// 만약 두 학생의 점수총합이 서로 똑같으면 그냥 같은순위로 처리
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

	int choice, i = 1; // i는 몇 번째 입력인지 의미
	DATA* p_head = NULL, * p_tail = NULL;
	DATA* p = NULL;

	while (1) {
		int n=0, total = 0;
		SCORE data = { 0, };

		printf("\n    [Menu]     \n");
		printf(" 1. 성적 입력 \n");
		printf(" 2. 성적 확인 \n");
		printf(" 3. 종료 \n\n");
		printf(" ------------ \n\n");
		printf(" 선택 (1~3) : ");
		scanf("%d", &choice);


		if (choice == 1) {

			/* 구조체 SCORE 입력 (여기서는 SCORE data) */
			printf("%d 번째 학생 이름 입력: ", i);
			scanf("%s", &data.name);
			printf("국어 점수: ");
			scanf("%d", &data.score[0]);
			printf("영어 점수: ");
			scanf("%d", &data.score[1]);
			printf("수학 점수: ");
			scanf("%d", &data.score[2]);

			for (n = 0; n < SIZE_OF_SCORE; n++) {
				total = total + data.score[n];
			}
			data.ranking[0] = total;
			data.ranking[1] = total / SIZE_OF_SCORE;
			data.ranking[2] = 0;

			AddData(&p_head, &p_tail, data); //연결 리스트 구성

			i++;
		}
		else if (choice == 2) {

			p = p_head; //p_head 값이 변하지 않기 위해 p를 이용(중요)
			scoreSort(p);

			printf("---------------------------------------------------\n");
			printf("      이름      국어  영어  수학   총점  평균  등수\n");
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
			/*연결리스트 동적할당 해제*/
			while (NULL != p_head) {
				p = p_head;
				p_head = p->p_next;
				free(p);
			}
			p_tail = p_head; //NULL
		}
		else { 
			printf("1~3중에서 선택\n");
			rewind(stdin);
		}
	}
}
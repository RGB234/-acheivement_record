#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define SIZE_OF_NAME 14 //이름 최대길이는 13+1(\n)
#define SIZE_OF_SCORE 3 //국어,영어,수학 3가지

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
	//for (i = 0; i < SIZE_OF_NAME; i++) {
	//	(*pp_tail)->acheiv.name[i] = input.name[i]; //strcmp
	//}
	strncpy((*pp_tail)->acheiv.name, input.name, sizeof((*pp_tail)->acheiv.name)-1);

	/*점수*/
	for (i = 0; i < SIZE_OF_SCORE; i++) {
		
		(*pp_tail)->acheiv.score[i] = input.score[i];
	}
	/* 총합, 평균, 순위*/

	(*pp_tail)->acheiv.ranking[0] = input.ranking[0];
	(*pp_tail)->acheiv.ranking[1] = input.ranking[1];
	(*pp_tail)->acheiv.ranking[2] = 0;  //순위까지 매기는 함수는 메인화면에서 2. 선택시 적용

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

	int i = 1; // i는 몇 번째 입력인지 의미
	int load_sentry = 0;
	char name[14];
	DATA* p_head = NULL, * p_tail = NULL;
	DATA* p = NULL;

	while (1) {
		int choice, n=0, total = 0;
		SCORE data = { 0, };

		printf("\n    [Menu]     \n");
		printf(" 1. 성적 입력 \n");
		printf(" 2. 성적 확인 \n");
		printf(" 3. 성적 수정 및 삭제 \n");
		printf(" 4. 성적 불러오기 \n");
		printf(" 5. 성적 저장하기 \n");
		printf(" 6. 종료 \n\n");
		printf(" ------------ \n\n");
		printf(" 선택 (1~6) : ");

		rewind(stdin);
		scanf("%d", &choice);


		if (choice == 1) {

			/* 구조체 SCORE 입력 (여기서는 SCORE data) */
			rewind(stdin);
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

			if (NULL == p) {
				printf("빈 데이터입니다\n");
				break;
			}

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
		else if(choice == 3){

			//성적 수정 및 삭제
			while (1) {

				printf("\n1. 성적 수정\n");
				printf("2. 성적 삭제\n");
				printf("3. 메인 화면으로 돌아가기\n");
				printf("선택(1~3): ");
				scanf("%d", &choice);


				if (choice == 1) {

					p = p_head;

					rewind(stdin);
					printf("\n수정할 학생의 이름 : ");
					scanf("%s", &name);

					while (NULL != p) {
						if (0 == strcmp(p->acheiv.name, name)) { //두 개가 같으면
							break;
						}
						else {
							p = p->p_next;
						}
					}

					printf("국어 점수: ");
					scanf("%d", &(p->acheiv.score[0]));
					printf("영어 점수: ");
					scanf("%d", &(p->acheiv.score[1]));
					printf("수학 점수: ");
					scanf("%d", &(p->acheiv.score[2]));

					for (n = 0; n < SIZE_OF_SCORE; n++) {
						total = total + p->acheiv.score[n];
					}

					p->acheiv.ranking[0] = total;
					p->acheiv.ranking[1] = total / SIZE_OF_SCORE;
					p->acheiv.ranking[2] = 0; //순위지정은 성적확인 선택시 지정됨

				}
				else if (choice == 2) {

					p = p_head;

					printf("삭제할 학생의 이름: ");
					scanf("%s", name);

					while (NULL != p) {
						if (0 == strcmp(p->acheiv.name, name)) { //두 개가 같으면
							break;
						}
						else {
							p = p->p_next;
						}
					}
					
					//while (NULL != p->p_next) {
					//	//덮어쓰기
					//	
					//	strcpy(p->acheiv.name, p->p_next->acheiv.name);
					//	for (n = 0; n < SIZE_OF_SCORE; n++) {
					//		p->acheiv.score[n] = p->p_next->acheiv.score[n];
					//		p->acheiv.ranking[n] = p->p_next->acheiv.ranking[n];
					//	}
					//	p->p_next = p->p_next->p_next;

					//	p = p->p_next;
					//}
					//free(p);
				}
				else if (choice == 3) {
					break;
				}
				else printf("1~3중에서 선택");
			}
		}

		else if (choice == 4 && load_sentry == 0) {

			//불러오기

			int file_size =0, score[3], ranking[3];
			
			FILE* p_file = fopen("archive.txt", "rt");

			load_sentry = 1; //파일 불러오기는 한 번만 가능 (중복해서 같은 자료를 여러 번 불러오는것을 막기위함)

			//char temp[60]; //파일의 첫번째 줄 스킵위치 -> 36
			//int current_pointer = 0;
			//fgets(temp, 60, p_file);
			//current_pointer = ftell(p_file);
			//printf("%d\n", current_pointer);

			fseek(p_file, 0, SEEK_END);
			file_size = ftell(p_file);
			fseek(p_file, 36, SEEK_SET); // 파일의 첫번째 줄 스킵위치

			if (NULL != p_file) {
				while (ftell(p_file) != file_size) {

					SCORE data_archive = { 0, };

					fscanf(p_file, "%s", name);
					fscanf(p_file, "%d", &score[0]);
					fscanf(p_file, "%d", &score[1]);
					fscanf(p_file, "%d", &score[2]);
					fscanf(p_file, "%d", &ranking[0]);
					fscanf(p_file, "%d", &ranking[1]);
					fscanf(p_file, "%d", &ranking[2]);

					strcpy(data_archive.name, name);
					data_archive.score[0] = score[0];
					data_archive.score[1] = score[1];
					data_archive.score[2] = score[2];
					data_archive.ranking[0] = ranking[0];
					data_archive.ranking[1] = ranking[1];
					data_archive.ranking[2] = ranking[2];

					AddData(&p_head, &p_tail, data_archive);
				}

			}
			else printf("파일 읽기에 실패하였습니다.\n");
			fclose(p_file);
		}
		else if (choice == 4 && load_sentry == 1) printf("파일 불러오기는 한 번만 가능합니다");
		else if (choice == 5) {

		p = p_head;

			//저장하기

			scoreSort(p);

			FILE* p_file = fopen("archive.txt", "wt");
			if (NULL != p_file) {

				fprintf(p_file, "이름 국어 영어 수학 총점 평균 순위\n");

				while (NULL != p) {

					fprintf(p_file, "%s %d %d %d %d %d %d", 
						p->acheiv.name, p->acheiv.score[0], p->acheiv.score[1], p->acheiv.score[2], p->acheiv.ranking[0],
						p->acheiv.ranking[1], p->acheiv.ranking[2]);
					if (NULL != p->p_next )fprintf(p_file, "\n");
					p = p->p_next;
				}
			}
			else printf("파일 열기에 실패하였습니다\n");
			fclose(p_file);
		}
			
		else if(choice == 6) { 
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
			printf("1~6중에서 선택\n");
			rewind(stdin);
		}
	}
}

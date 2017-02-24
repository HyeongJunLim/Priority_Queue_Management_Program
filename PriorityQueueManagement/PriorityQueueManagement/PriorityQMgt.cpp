#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// 정의
#define INSERT	0
#define DELETE	1
#define END	-1

// 구조체 정의
typedef struct priority_queue *queue_pointer;
typedef struct queue_head *head_pointer;

struct priority_queue	{
	int				priority;
	int				computing_time;
	queue_pointer	left_link, right_link;
};
struct queue_head	{
	queue_pointer	left_link;
	queue_pointer	right_link;
};

// 함수 정의
void initialize_queue(void);							// 큐 초기화
int insert_queue(int priority, int computing_time);		// 큐 삽입
int delete_queue(int priority);							// 큐 삭제
void print_queue(void);									// 큐 출력
void PrintError(char *msg);								// 에러메세지 출력
int SearchLv(int priority);								// 큐 ID 구분

// 큐 포인터 선언
head_pointer	queue[2];

int main(void)	{
	FILE	*fp;		// 파일 포인터
	int ret;			// 리턴값
	int type = 0, priority = 0, computing_time = 0;
	

	
	fp = fopen("input_file.txt","rt");
	if(fp == NULL)	{
		PrintError("fopen 실패");
		exit(0);
	}

	// 큐 초기화 실행
	initialize_queue();

	
	while(1)	{
		// input_file.txt로부터 type, priority, computing_time을 받아옴
		fscanf(fp,"%d	%d	%d",&type, &priority, &computing_time);
		
		// type 값을 통해 INSERT , DELETE, END 진행
		switch(type)	{
			case INSERT:	// 큐 삽입	(0)
				insert_queue(priority, computing_time);
				break;
			case DELETE:	// 큐 삭제	(1)
				delete_queue(priority);
				break;
			case END:		// 입력완료	(-1)
				print_queue();
				break;
		}
				
		if(type == END)
			break;
	}
	
	ret = fclose(fp);
	if(ret != 0)
		PrintError("fclose 실패");

	return 0;
}


void initialize_queue(void)	{
	// 큐 초기화
	for(int i=0;i<3; i++)
	{
		queue[i] = (head_pointer)malloc(sizeof(struct queue_head));
		queue[i]->left_link = NULL;
		queue[i]->right_link = NULL;
	}
}
int insert_queue(int priority, int computing_time)	{
	// 큐 삽입
	queue_pointer new_node = (queue_pointer)malloc(sizeof(struct priority_queue));	// 새노드
	if(new_node == NULL)
		PrintError("메모리 할당 에러");
	new_node->left_link = new_node->right_link = NULL;
	new_node->priority = priority;
	new_node->computing_time = computing_time;

	int ID = SearchLv(priority);		// 큐 ID
		
	if( queue[ID]->right_link == NULL )	{	//  첫 노드
		queue[ID]->right_link = new_node;
		return 0;
	}
	
	queue_pointer temp;					// 위치찾기용 
	temp = queue[ID]->right_link;

	while( temp != NULL )	{
		if(temp->priority <= priority && temp->right_link == NULL)	{
			// temp->prioSrity < new_node->priority 그리고 temp->right_link가 NULL 
			temp->right_link = new_node;
			new_node->left_link = temp;
			break;
		}
		else if(temp->priority > priority && temp->left_link == NULL)	{
			// temp->priority > new_node->priority 그리고 temp->left_link가 NULL 
			queue[ID]->right_link = new_node;
			new_node->right_link = temp;
			temp->left_link = new_node;
			break;
		}
		else if(temp->priority <= priority)
			temp = temp->right_link;
		else	{
			// 그 외의 중간노드삽입
			temp = temp->left_link;

			new_node->right_link = temp->right_link;
			new_node->left_link = temp; 
			temp->right_link->left_link = new_node;
			temp->right_link = new_node;
			break;
		}
	}

	return 0;
}
int delete_queue(int priority)	{
	// 큐 삭제
	queue_pointer temp;

	int ID = SearchLv(priority);		// 큐 ID
	temp = queue[ID]->right_link;


	if(temp == NULL || temp->priority > priority)
		return 0;
	else if(temp->right_link == NULL)
		queue[ID]->right_link = NULL;
	else	{
		queue[ID]->right_link = temp->right_link;
		queue[ID]->right_link->left_link = NULL;
	}

	printf("[삭제노드]\t ID :%d \t\t priority : %d \t  computing_time: %d \n", ID+1, temp->priority, temp->computing_time);

	free(temp);
	
	return 0;
}
void print_queue(void)	{
	// 큐 출력
	queue_pointer temp;
	
	printf("---------------------------------------------------------------\n");	
	printf("Queue ID \t\t\t priority \t computing_time \n");
	printf("---------------------------------------------------------------\n");	
	for(int i=0;i<3;i++)
	{
		temp = queue[i]->right_link;

		while(temp != NULL)	{
			printf("%d \t\t\t\t %d \t\t %d \n", i+1, temp->priority, temp->computing_time);
			if(temp->right_link == NULL)
				break;
			temp=temp->right_link;
		}
		printf("\n");
	}
	printf("---------------------------------------------------------------\n");
}
void PrintError(char *msg)	{
	printf("> [ERROR] %s \n",msg);
	exit(0);
}
int SearchLv(int priority)	{
	int lv;

	if( priority > 0 && priority <= 10 )
		lv=0;
	else if(priority <= 20)
		lv=1;
	else if(priority <= 30)
		lv=2;
	else
		PrintError("QUEUE ID 구분 오류(priority가 0~30사이의 값이 아닙니다)");

	return lv;
}
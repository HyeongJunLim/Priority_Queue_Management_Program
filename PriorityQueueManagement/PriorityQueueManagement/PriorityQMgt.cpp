#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// ����
#define INSERT	0
#define DELETE	1
#define END	-1

// ����ü ����
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

// �Լ� ����
void initialize_queue(void);							// ť �ʱ�ȭ
int insert_queue(int priority, int computing_time);		// ť ����
int delete_queue(int priority);							// ť ����
void print_queue(void);									// ť ���
void PrintError(char *msg);								// �����޼��� ���
int SearchLv(int priority);								// ť ID ����

// ť ������ ����
head_pointer	queue[2];

int main(void)	{
	FILE	*fp;		// ���� ������
	int ret;			// ���ϰ�
	int type = 0, priority = 0, computing_time = 0;
	

	
	fp = fopen("input_file.txt","rt");
	if(fp == NULL)	{
		PrintError("fopen ����");
		exit(0);
	}

	// ť �ʱ�ȭ ����
	initialize_queue();

	
	while(1)	{
		// input_file.txt�κ��� type, priority, computing_time�� �޾ƿ�
		fscanf(fp,"%d	%d	%d",&type, &priority, &computing_time);
		
		// type ���� ���� INSERT , DELETE, END ����
		switch(type)	{
			case INSERT:	// ť ����	(0)
				insert_queue(priority, computing_time);
				break;
			case DELETE:	// ť ����	(1)
				delete_queue(priority);
				break;
			case END:		// �Է¿Ϸ�	(-1)
				print_queue();
				break;
		}
				
		if(type == END)
			break;
	}
	
	ret = fclose(fp);
	if(ret != 0)
		PrintError("fclose ����");

	return 0;
}


void initialize_queue(void)	{
	// ť �ʱ�ȭ
	for(int i=0;i<3; i++)
	{
		queue[i] = (head_pointer)malloc(sizeof(struct queue_head));
		queue[i]->left_link = NULL;
		queue[i]->right_link = NULL;
	}
}
int insert_queue(int priority, int computing_time)	{
	// ť ����
	queue_pointer new_node = (queue_pointer)malloc(sizeof(struct priority_queue));	// �����
	if(new_node == NULL)
		PrintError("�޸� �Ҵ� ����");
	new_node->left_link = new_node->right_link = NULL;
	new_node->priority = priority;
	new_node->computing_time = computing_time;

	int ID = SearchLv(priority);		// ť ID
		
	if( queue[ID]->right_link == NULL )	{	//  ù ���
		queue[ID]->right_link = new_node;
		return 0;
	}
	
	queue_pointer temp;					// ��ġã��� 
	temp = queue[ID]->right_link;

	while( temp != NULL )	{
		if(temp->priority <= priority && temp->right_link == NULL)	{
			// temp->prioSrity < new_node->priority �׸��� temp->right_link�� NULL 
			temp->right_link = new_node;
			new_node->left_link = temp;
			break;
		}
		else if(temp->priority > priority && temp->left_link == NULL)	{
			// temp->priority > new_node->priority �׸��� temp->left_link�� NULL 
			queue[ID]->right_link = new_node;
			new_node->right_link = temp;
			temp->left_link = new_node;
			break;
		}
		else if(temp->priority <= priority)
			temp = temp->right_link;
		else	{
			// �� ���� �߰�������
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
	// ť ����
	queue_pointer temp;

	int ID = SearchLv(priority);		// ť ID
	temp = queue[ID]->right_link;


	if(temp == NULL || temp->priority > priority)
		return 0;
	else if(temp->right_link == NULL)
		queue[ID]->right_link = NULL;
	else	{
		queue[ID]->right_link = temp->right_link;
		queue[ID]->right_link->left_link = NULL;
	}

	printf("[�������]\t ID :%d \t\t priority : %d \t  computing_time: %d \n", ID+1, temp->priority, temp->computing_time);

	free(temp);
	
	return 0;
}
void print_queue(void)	{
	// ť ���
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
		PrintError("QUEUE ID ���� ����(priority�� 0~30������ ���� �ƴմϴ�)");

	return lv;
}
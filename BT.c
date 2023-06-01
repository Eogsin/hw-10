/*
 * Binary Search Tree #2
 *
 * Data Structures
 *
 * School of Computer Science
 * at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>

// 이진 트리의 노드 구조체 정의
typedef struct node {
	int key;
	struct node *left;
	struct node *right;
} Node;

/* for stack */
#define MAX_STACK_SIZE	20     
Node* stack[MAX_STACK_SIZE];
int top = -1;

Node* pop();
void push(Node* aNode);

/* for queue */
#define MAX_QUEUE_SIZE	20
Node* queue[MAX_QUEUE_SIZE];
int front = -1;
int rear = -1;

// 이진 트리 초기화 함수
int initializeBST(Node** h);
void recursiveInorder(Node* ptr);	  /* recursive inorder traversal */
int insert(Node* head, int key);      /* insert a node to the tree */
int freeBST(Node* head); /* free all memories allocated to the tree */

/* functions that you have to implement */
void iterativeInorder(Node* ptr);     /* iterative inorder traversal */
void levelOrder(Node* ptr);	          /* level order traversal */
int deleteNode(Node* head, int key);  /* delete the node for the key */
Node* pop();
void push(Node* aNode);
Node* deQueue();
void enQueue(Node* aNode);

/* you may add your own defined functions if necessary */


void printStack();



int main()
{
	char command;
	int key;
	Node* head = NULL;

	do{
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initializeBST(&head);
			break;
		case 'q': case 'Q':
			freeBST(head);
			break;
		case 'i': case 'I':
			printf("Your Key = ");
			scanf("%d", &key);
			insert(head, key);
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(head, key);
			break;

		case 'r': case 'R':
			recursiveInorder(head->left);
			break;
		case 't': case 'T':
			iterativeInorder(head->left);
			break;

		case 'l': case 'L':
			levelOrder(head->left);
			break;

		case 'p': case 'P':
			printStack();
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

// 이진 트리 초기화 함수
int initializeBST(Node** h) {

	/* if the tree is not empty, then remove all allocated nodes from the tree*/
	if(*h != NULL)                  
		freeBST(*h);

	/* create a head node */
	*h = (Node*)malloc(sizeof(Node));
	(*h)->left = NULL;	/* root */
	(*h)->right = *h;
	(*h)->key = -9999;

	top = -1;

	front = rear = -1;

	return 1;
}

// 재귀적인 중위 순회 함수
void recursiveInorder(Node* ptr)
{
	if(ptr) {
		recursiveInorder(ptr->left);	  	
		printf(" [%d] ", ptr->key);
		recursiveInorder(ptr->right);
	}
}

/**
 * textbook: p 224
 */
// 반복적인 중위 순회 함수
void iterativeInorder(Node* node)
{
	for(;;)//무한루프 
	{
		for(; node; node = node->left)  //왼쪽으로 계속 이동하면서 스택에 노드를 추가
			push(node);					//pop함수를 이용하여 스텍에서 노드를 꺼내소서'node'에 할당
		node = pop();

		if(!node) break;				//만약 노드가 NULL이면 break
		printf(" [%d] ", node->key);	//노드의 킬값을 출력

		node = node->right;				//'node'를 오른쪽 자식 노드로 이동
	}
}

/**
 * textbook: p 225
 */
// 레벨 순회 함수
void levelOrder(Node* ptr)
{
	// int front = rear = -1;

	if(!ptr) return; /* empty tree */
//만약 주어진 노드 'ptr'가 NULL이라면 빈 트리이므로 함수를 종료 
	enQueue(ptr);
	//ptr을 큐에 삽입

	for(;;)//무한루프
	{
		ptr = deQueue();	//ptr은 큐에서 꺼낸다.
		if(ptr) {
			printf(" [%d] ", ptr->key);  //ptr이 NULL이 아니라면, 해당 노드의 키 값을 출력합니다.
			if(ptr->left)
				enQueue(ptr->left);     //ptr의 왼쪽 자식 노드가 존재한다면, 그 자식 노드를 큐에 삽입합니다.
			if(ptr->right)
				enQueue(ptr->right);	//'ptr'의 왼쪽 자식 노드가 존재한다면, 그 자식 노드를 큐에 삽입합니다.
		}
		else
			break;

	}

}

// 노드 삽입 함수
int insert(Node* head, int key)
{
	Node* newNode = (Node*)malloc(sizeof(Node));//노드 생성
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;

	if (head->left == NULL) {			//만약 트가 비어있으면 생상한 로드가 루트노드
		head->left = newNode;
		return 1;
	}

	/* head->left is the root */
	Node* ptr = head->left;

	Node* parentNode = NULL;
	while(ptr != NULL) {

		/* if there is a node for the key, then just return */
		//만약 입력한 키에 대한 노드가 있으면 그냥 반환 한다
		if(ptr->key == key) return 1;

		/* we have to move onto children nodes,
		 * keep tracking the parent using parentNode */
		//자식 노드로 이동하고 parentNode를 사용하여 부모를 계속 추적한다.
		parentNode = ptr;

		/* key comparison, if current node's key is greater than input key
		 * then the new node has to be inserted into the right subtree;
		 * otherwise the left subtree.
		 */
		//키 비교, 현재 노드의 키가 입력 키보다 크면 새 노드를 오른쪽 하위 트리에 삽입해야 합니다.
		if(ptr->key < key)
			ptr = ptr->right;
		//그렇지 않으면 왼쪽 하위 트리입니다.
		else
			ptr = ptr->left;
	}

	/* linking the new node to the parent */
	//새 노드를 상위 노드에 연결한다.
	if(parentNode->key > key)
		parentNode->left = newNode;
	else
		parentNode->right = newNode;
	return 1;
}

// 노드 삭제 함수
int deleteNode(Node* head, int key)
{
	if (head == NULL) {
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	if (head->left == NULL) {
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	/* head->left is the root */
	Node* root = head->left;

	Node* parent = NULL;
	Node* ptr = root;

	while((ptr != NULL)&&(ptr->key != key)) {
		if(ptr->key != key) {

			parent = ptr;	/* save the parent */

			if(ptr->key > key)
				ptr = ptr->left;
			else
				ptr = ptr->right;
		}
	}

	/* there is no node for the key */
	//키에 대한 노드가 없으면 
	if(ptr == NULL)
	{
		printf("No node for key [%d]\n ", key);
		return -1;
	}

	/*
	 * case 1: the node which has to be removed is a leaf node
	*/
	//리프 노드를 제거할 떄
	if(ptr->left == NULL && ptr->right == NULL)
	{
		if(parent != NULL) { /* parent exists, parent's left and right links are adjusted */
								//부모 존재, 부모의 왼쪽 및 오른쪽 링크 조정
			if(parent->left == ptr)
				parent->left = NULL;
			else
				parent->right = NULL;
		} else {
			/* parent is null, which means the node to be deleted is the root */
			//루트를 삭제할 때 
			head->left = NULL;

		}

		free(ptr);
		return 1;
	}

	/**
	 * case 2: if the node to be deleted has one child
	//자식 노득 하나인 경우
	 */
	if ((ptr->left == NULL || ptr->right == NULL))
	{
		Node* child;
		if (ptr->left != NULL)
			child = ptr->left;
		else
			child = ptr->right;

		if(parent != NULL)
		{
			if(parent->left == ptr)
				parent->left = child;
			else
				parent->right = child;
		} else {
			/* parent is null, which means the node to be deleted is the root
			 * and the root has one child. Therefore, the child should be the root
			 */
			 //parent는 null이며 삭제할 노드가 루트이고 루트에 자식이 하나 있음을 의미합니다. 
			 //그러므로 자식은 루트가 되어야 한다
			root = child;
		}

		free(ptr);
		return 1;
	}

	/**
	 * case 3: the node (ptr) has two children
	 *
	 * we have to find either the biggest descendant node in the left subtree of the ptr
	 * or the smallest descendant in the right subtree of the ptr.
	 *
	 * we will find the smallest descendant from the right subtree of the ptr.
	 *
	 */
	//노드(ptr)에는 두 개의 하위 항목이 있습니다.
	Node* candidate;
	parent = ptr;


	candidate = ptr->right;

	/* the smallest node is left deepest node in the right subtree of the ptr */
	
	//가장 작은 노드는 ptr의 오른쪽 하위 트리에서 왼쪽 가장 깊은 노드입니다
	while(candidate->left != NULL)
	{
		parent = candidate;
		candidate = candidate->left;
	}

	/* the candidate node is the right node which has to be deleted.
	 * note that candidate's left is null
	 */
	 //candidate 노드는 삭제해야 할 올바른 노드입니다.
	 // candidate의 왼쪽은 null입니다
	if (parent->right == candidate)
		parent->right = candidate->right;
	else
		parent->left = candidate->right;

	/* instead of removing ptr, we just change the key of ptr
	 * with the key of candidate node and remove the candidate node
	 */
	//ptr을 제거하는 대신 ptr의 키를 후보 노드의 키로 변경하고 후보 노드를 제거합니다.
	ptr->key = candidate->key;

	free(candidate);
	return 1;
}

// 노드 해제 함수
void freeNode(Node* ptr)
{
	if(ptr) {
		freeNode(ptr->left);
		freeNode(ptr->right);
		free(ptr);
	}
}

int freeBST(Node* head)
{

	if(head->left == head)
	{
		free(head);
		return 1;
	}

	Node* p = head->left;

	freeNode(p);

	free(head);
	return 1;
}

// 스택에서 노드를 팝하는 함수
Node* pop()
{
	if (top < 0) return NULL;
	return stack[top--];
}

// 스택에 노드를 푸시하는 함수
void push(Node* aNode)
{
	stack[++top] = aNode;
}

// 큐에서 노드를 디큐하는 함수
Node* deQueue()
{
	if (front == rear) {
		// printf("\n....Now Queue is empty!!\n" );
		return NULL;
	}

	front = (front + 1)%MAX_QUEUE_SIZE;
	return queue[front];
}

// 큐에 노드를 인큐하는 함수
void enQueue(Node* aNode)
{
	rear = (rear + 1)%MAX_QUEUE_SIZE;
	queue[rear] = aNode;
}

// 스택 내용을 출력하는 함수
void printStack()
{
	int i;
	printf("top = %d\n", top);
	for(i=0; i<=top; i++)
		printf(" [%d] ", stack[i]->key);

	printf("\n");
}
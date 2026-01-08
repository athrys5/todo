#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OPTIONS_NUM 5

const int MAX_LEN = 100; 
char menu[OPTIONS_NUM] = {'A', 'B', 'C', 'L', 'E'};

typedef struct Node {
	char *text;
	struct Node *next;
} Node;

void print_list(Node *head){
	Node *slider = head;
        int i = 0;	
	while(slider != NULL){
		printf("%d: %s\n", i, slider->text);
		slider = slider->next;
		i++;
	}
}

Node *new_node(char* line){
	Node *new = (Node*)malloc(sizeof(Node));
	new->text = strcpy((char*)malloc(strlen(line) + 1), line);
	new->next = NULL;
	return new;
}

int is_valid(char *v){
	int result = 0;
	
	if((int)*v > 90) 
		*v = (char)((int)*v - 32);
	
	for(int i = 0; i < OPTIONS_NUM; i++){
		if(*v == menu[i]) result = 1;
	} 

	return result;
}

char display_menu(){
	char c = '\0';
	int valid = 0;
	
	while(!valid){
		printf("Chose an option:\nA: Add item to the todo list\nB: Remove item from the todo list \nC: Clear Terminal\nL: Show List\nE: Exit program\n");
		scanf(" %c", &c);
		valid = is_valid(&c);
	}
	
	return c;
}

void add_item(Node **head){
	char item[MAX_LEN];
	Node *tail = *head; 
	
	printf("Enter new todo item:\n");
	scanf("%s", item);
	
	if(*head == NULL){
		*head = new_node(item);
		tail = *head;
	}else{
		while(tail->next != NULL) tail = tail->next;	
		
		tail->next = new_node(item);
		tail = tail->next;
	}
}

int list_length(Node *head){
	Node *tail = head;
	int count = 0;
	while(tail != NULL){
		tail = tail->next;
		count++;
	}
	return count;
}

void remove_node(Node **head, Node *prev, Node *current){
	if(prev == NULL){
		*head = (*head)->next; 
		free(current);
		current = NULL;
	}else{
		prev->next = current->next;
		free(current);
		current = NULL;
	}
	return;
	
}

void remove_item(Node **head){
	Node *tail = *head;
	Node *prev = NULL;
	int n = 0;
	int max = list_length(*head);
	
	if(max == 0){
		printf("No available item to delete\n");
		return; 
	}

	do{
		printf("Select the number corresponding to the item to delete\n");
		print_list(*head);
		scanf(" %d", &n);
		printf("The selected number is: %d\n", n);
		if(n >= max) printf("Invalid number. Chose a proper one.\n");		
	}while(n >= max);

	for(int i = 0; i < n; i++){
	       	prev = tail;
		tail = tail->next;
	}
	
	remove_node(&(*head), prev, tail);
	
	print_list(*head);

	return; 
}

void write_file(Node *head){
	FILE *f = fopen("todolist.txt","w");
	
	if(f == NULL){
		perror("Error opening the file");
		return;
	}

	while(head != NULL){
		fprintf(f, "%s\n", head->text);
		head = head->next;
	}
}

void clear(){
	printf("\033[2J\033[H");
	fflush(stdout);
}

int process_option(char opt, Node **head){
	switch(opt){
		case 'A':
			add_item(&(*head));
			return 0;
		case 'B':
			remove_item(&(*head));
			return 0;
		case 'C':
			clear();
			return 0;
		case 'L':
			print_list(*head);
			return 0;
	
		case 'E':
			write_file(*head);
			return 1;
		default:
			return 0;
	}
	return 1;
}

void init_list(Node **head){
	FILE *f = fopen("todolist.txt", "r");
	Node *tail = *head;
    	char line[MAX_LEN];
	
	if(f == NULL) return;

	while(fgets(line, MAX_LEN, f)) {
		tail = new_node(line);
		tail = tail->next;	
	}

	fclose(f);
}

int main(void){
	Node *head = NULL;
    	int exit = 0; 
    
	init_list(&head);

	while(!exit){
		char opt = display_menu();
		exit = process_option(opt, &head);
	}
     
	if(head != NULL) print_list(head);

	return 0;
}

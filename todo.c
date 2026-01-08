#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OPTIONS_NUM 3

const int MAX_LEN = 100; 
char menu[OPTIONS_NUM] = {'A', 'B', 'E'};

typedef struct Node {
	char *text;
	struct Node *next;
} Node;

void print_list(Node *head){
	Node *slider = head;
        int i = 1;	
	while(slider != NULL){
		printf("%d: %s\n", i, slider->text);
		slider = slider->next;
		i++;
	}
}

Node *crea_nodo(char* line){
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
		printf("Chose an option:\nA: Add item to the todo list\nB: Remove item from the todo list \nE: Exit program\n");
		scanf(" %c", &c);
		valid = is_valid(&c);
	}
	
	return c;
}

void add_item(Node **head){
	Node *tail = *head;
	FILE *file = fopen("todolist.txt", "a");
	char item[MAX_LEN];

	if(file == NULL){
		printf("Error opening the file\n");
		return;
	}

	printf("Enter new todo item:\n");
	scanf("%s", item);
	
	if(*head == NULL){
		*head = crea_nodo(item);
		tail = *head;
	}else{
		while(tail->next != NULL) tail = tail->next;	
		
		tail->next = crea_nodo(item);
		tail = tail->next;
	}

	fprintf(file, "%s\n", tail->text);
	
	fclose(file);
	return;
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
	FILE *file = fopen("todolist.txt", "w");
	int n = 1;
	int max = list_length(*head);
	
	if(max == 0){
		printf("No available item to delete\n");
		return; 
	}

	do{
		printf("Select the number corresponding to the item to delete\n");
		print_list(*head);
		scanf("%d", &n);
		if(n > max) printf("Invalid number. Chose a proper one.\n");		
	}while(n > max);

	for(int i = 1; i < n; i++){
	       	prev = tail;
		tail = tail->next;
	}
	
	remove_node(&(*head), prev, tail);
	
	print_list(*head);

	return; 
}

int process_option(char opt, Node **head){
	switch(opt){
		case 'A':
			add_item(&(*head));
			return 0;
		case 'B':
			remove_item(&(*head));
			break;
	}
	return 1;
}

int main(void){
	Node *head = NULL;
 	Node *tail = NULL;
    	FILE *file = fopen("todolist.txt", "r");
    	int i = 0;
    	char line[MAX_LEN];
    	int exit = 0; 
    	
	if(file == NULL){
        	file = fopen("todolist.txt", "w");
        	
		if(file == NULL){
            		perror("Error");
            		return 1;
        	}

        	fclose(file);
        	return 0;  // File creato ma vuoto, esci
    	}
    
	while(fgets(line, MAX_LEN, file)){
	
		if(head == NULL){
        		head = crea_nodo(line);
            		tail = head; 
      	 	} else {
			tail->next = crea_nodo(line);
           	 	tail = tail->next;
        	}
        
        	i++;
    	}
    
   	fclose(file);
    

    
	while(!exit){
		char opt = display_menu();
		exit = process_option(opt, &head);
	}
     
	if(head != NULL){
        	print_list(head);
    	}

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OPTIONS_NUM 3

const int MAX_LEN = 100; 
char menu[OPTIONS_NUM] = {'A', 'B', 'E'};

typedef struct Node {
	int val;
	char *text;
	struct Node *next;
} Node;

void print_list(Node *head){
	Node *slider = head; 
	while(slider != NULL){
		printf("%s", slider->text);
		slider = slider->next;
	}
}

Node *crea_nodo(int val, char* line){
	Node *new = (Node*)malloc(sizeof(Node));
	new->val = val;
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
		scanf("%c", &c);
		valid = is_valid(&c);
	}
	
	return c;
}

void add_item(Node *head){
	Node *tail = head;
	FILE *file = fopen("todolist.txt", "a");
	char item[MAX_LEN];

	if(file == NULL){
		printf("Error opening the file\n");
		return;
	}

	printf("Enter new todo item:\n");
	scanf("%s", item);
	
	while(tail->next != NULL){
		tail = tail->next;	
	}

	tail->next = crea_nodo(tail->val + 1, item);
	
	tail = tail->next;

	fprintf(file, "%s\n", tail->text);
	
	fclose(file);
	return;
}

void remove_item(Node *head){
	Node *tail = head;
	File *file = fopen("todolist.txt", "w");
	int n = 0;

	printf("Select the item to delete");

}

int process_option(char opt, Node *head){
	switch(opt) {
		case 'A':
			add_item(head);
		case 'B':
			remove_item(head);
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
        		head = crea_nodo(i, line);
            		tail = head; 
      	 	} else {
			tail->next = crea_nodo(i, line);
           	 	tail = tail->next;
        	}
        
        	i++;
    	}
    
   	fclose(file);
    

    
	while(!exit){
		char opt = display_menu();
		exit = process_option(opt, head);
	}
     
	if(head != NULL){
        	print_list(head);
    	}

	return 0;
}

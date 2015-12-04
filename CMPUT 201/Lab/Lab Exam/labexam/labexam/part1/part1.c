#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 128
#define NUM 5

struct Student {
    char name[MAX_NAME];
    int id;
    struct Student *next;
};

void print_class_list(struct Student *head);
void cleanup(struct Student *head);

int main() {

    struct Student *head, *temp;

    /* Add some student data */
    char *names[] = {"Bob", "Joe", "Fan", "Lee", "end"};
    int ids[] = {100, 101, 102, 103, -1};

    head = NULL;

    for (int i = NUM-1; i >= 0; i--) {
        temp = malloc(sizeof(struct Student));
	strncpy(temp->name, names[i], MAX_NAME);
	temp->id = ids[i];
	temp->next = head;
	head = temp;
    }

    print_class_list(head);

    cleanup(head);

    return(0);
} /* end of main */

void print_class_list(struct Student *head) {

    struct Student *current;
    
    current = head;
    while (&current->id > 0) {
        printf("%s\t%d\n", current->name, current->id);
	current = current->next;
    }
} /* end of compute_average */

void cleanup(struct Student *head) {

    struct Student *temp;

    while (head != NULL) {
        temp = head->next;
	free(head);
	head = temp;
    }

} /* end of cleanup */

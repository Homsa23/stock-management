#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct box {
    char reference[20];
    char name[20];
    int qantity;
    int selling_price;
    int purchase_price;
    struct box *next;
} box;

//**<adding function>***
box *add_a_part(box *head) {
    box *b = malloc(sizeof(box));
    if (!b) {
        printf("Memory allocation failed.\n");
        return head;
    }

    printf("Enter reference:\n");
    scanf("%s", b->reference);
    printf("Enter name:\n");
    scanf("%s", b->name);
    printf("Enter quantity: ");
    scanf("%d", &b->qantity);
    printf("Enter selling price: ");
    scanf("%d", &b->selling_price);
    printf("Enter purchase price: ");
    scanf("%d", &b->purchase_price);

    b->next = NULL;

    if (head == NULL) {
        printf("Part added successfully!\n");
        return b;
    }

    box *ptr = head;
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }

    ptr->next = b;
    printf("Part added successfully!\n");
    return head;
}

//***<display function>****
void display(box *head) {
    box *p = head;

    if (p == NULL) {
        printf("The list is empty.\n");
        return;
    }

    while (p != NULL) {
        printf("---<current items>---\n");
        printf("Reference: %s\nName: %s\nQuantity: %d\nPurchase price: %d\nSelling price: %d\n", 
               p->reference, p->name, p->qantity, p->purchase_price, p->selling_price);
        p = p->next;
        printf("------------------------\n");
    }
}

//***<save to file function>****
void saveToFile(box *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    box *current = head;
    while (current != NULL) {
        fprintf(file, "%s %s %d %d %d\n", current->reference, current->name, current->qantity, current->selling_price, current->purchase_price);
        current = current->next;
    }

    fclose(file);
    printf("Stock saved successfully to '%s'.\n", filename);
}

//***<load from file function>****
box *loadFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No existing stock file found. Starting with an empty stock.\n");
        return NULL;
    }

    box *head = NULL, *tail = NULL;

    while (!feof(file)) {
        box *b = malloc(sizeof(box));
        if (!b) {
            printf("Memory allocation failed.\n");
            break;
        }

        if (fscanf(file, "%s %s %d %d %d", b->reference, b->name, &b->qantity, &b->selling_price, &b->purchase_price) == 5) {
            b->next = NULL;
            if (head == NULL) {
                head = b;
                tail = b;
            } else {
                tail->next = b;
                tail = b;
            }
        } else {
            free(b);
            break;
        }
    }

    fclose(file);
    printf("Stock loaded successfully from '%s'.\n", filename);
    return head;
}

//****delete part function*****
box *deletePart(box *head, const char *name) {
    if (head == NULL) {
        printf("The stock is empty. Nothing to delete.\n");
        return NULL;
    }

    box *temp = head;
    box *prev = NULL;

    if (strcmp(head->name, name) == 0) {
        head = head->next;
        free(temp);
        temp = NULL;
        printf("Part with name '%s' deleted successfully.\n", name);
        return head;
    }

    while (temp != NULL && strcmp(temp->name, name) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Part with name '%s' not found.\n", name);
        return head;
    }

    prev->next = temp->next;
    free(temp);
    printf("Part with name '%s' deleted successfully.\n", name);
    return head;
}
//***modify function***
void modify(box *head, const char *reference) {
    box *ptr = head;
    if (head == NULL) {
        printf("The stock is empty, nothing to modify.\n");
        return;
    }
    while (ptr != NULL && strcmp(ptr->reference, reference) != 0) {
        ptr = ptr->next;
    }
    if (ptr == NULL) {
        printf("Part with reference %s does not exist.\n", reference);
        return;
    }

    printf("Enter new quantity: \n");
    scanf("%d", &ptr->qantity);
    printf("Enter new selling price: \n");
    scanf("%d", &ptr->selling_price);
    printf("Enter new purchase price: \n");
    scanf("%d", &ptr->purchase_price);

    printf("---<part successfully modified>---\n");
}

//***<main function>******
int main() {
    const char *filename = "stock.txt";
    int choice;
    box *head = loadFromFile(filename); // Load stock at the start
    char reference[20], name[20];

    while (1) {
        printf("---<stock management>---\n");
        printf("1. Add a part to the stock\n");
        printf("2. Modify a part\n");
        printf("3. Delete a part\n");
        printf("4. Save stock to file\n");
        printf("5. Exit\n");
        printf("Enter your choice:\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                head = add_a_part(head);
                break;

            case 2:
                printf("Enter the reference you want to modify:\n");
                scanf("%s", reference);
                modify(head, reference);
                break;

            case 3:
                printf("Enter the name of the item you want to delete: ");
                scanf("%s", name);
                head = deletePart(head, name);
                break;

            case 4:
                saveToFile(head, filename);
                break;

            case 5:
                saveToFile(head, filename); // Save before exiting
                printf("Exiting the program...\n");
                return 0;

            default:
                printf("---<invalid choice, please try again>---\n");
        }

        display(head);
    }

    return 0;
}

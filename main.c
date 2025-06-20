#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CONTACTS 100
#define MAX_NAME_LENGTH 100 // Increased for full name
#define MAX_PHONE_LENGTH 11 // Increased to 11 for 10 digits + null terminator
#define MAX_CITY_LENGTH 50
#define MAX_EMAIL_LENGTH 50
#define MAX_GROUP_LENGTH 20 // Maximum length of group name
#define FILENAME "contacts.txt"

typedef struct {
    char name[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char city[MAX_CITY_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char group[MAX_GROUP_LENGTH];
} Contact;

Contact contacts[MAX_CONTACTS];
int numContacts = 0;

void saveContactsToFile() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numContacts; i++) {
        fprintf(file, "%s,%s,%s,%s,%s\n", contacts[i].name, contacts[i].phone, contacts[i].city, contacts[i].email, contacts[i].group);
    }

    fclose(file);
}

void loadContactsFromFile() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No existing contacts file found.\n");
        return;
    }

    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%s\n", contacts[numContacts].name, contacts[numContacts].phone,
                  contacts[numContacts].city, contacts[numContacts].email, contacts[numContacts].group) != EOF) {
        numContacts++;
    }

    fclose(file);
}

int searchContact(const char *keyword, int option, const char *group) {
    for (int i = 0; i < numContacts; i++) {
        if ((strcmp(group, "All") == 0 || strcmp(contacts[i].group, group) == 0) && (
                (option == 1 && strstr(contacts[i].name, keyword) != NULL) ||
                (option == 2 && strstr(contacts[i].phone, keyword) != NULL) ||
                (option == 3 && strstr(contacts[i].city, keyword) != NULL) ||
                (option == 4 && strstr(contacts[i].email, keyword) != NULL))) {
            return i;
        }
    }
    return -1; // Contact not found
}

void editContact(int index) {
    printf("Enter new name: ");
    getchar(); // Consume newline character left in buffer
    fgets(contacts[index].name, sizeof(contacts[index].name), stdin);
    contacts[index].name[strcspn(contacts[index].name, "\n")] = '\0'; // Remove trailing newline
    printf("Enter new phone number (up to 10 numerical digits only): ");
    do {
        scanf("%s", contacts[index].phone);
        if (strlen(contacts[index].phone) != 10 || !isNumeric(contacts[index].phone)) {
            printf("Invalid phone number. Please enter exactly 10 numerical digits only: ");
        }
    } while (strlen(contacts[index].phone) != 10 || !isNumeric(contacts[index].phone));
    printf("Enter new city: ");
    scanf("%s", contacts[index].city);
    printf("Enter new email address: ");
    do {
        scanf("%s", contacts[index].email);
        if (!isValidEmail(contacts[index].email)) {
            printf("Invalid email address. Please enter a valid email address: ");
        }
    } while (!isValidEmail(contacts[index].email));
    printf("Enter new group: ");
    scanf("%s", contacts[index].group);
    printf("Contact edited successfully.\n");
    saveContactsToFile();
}

int isNumeric(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

int isValidEmail(const char *email) {
    int atCount = 0;
    int dotCount = 0;
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            atCount++;
        } else if (email[i] == '.') {
            dotCount++;
        }
    }
    return (atCount == 1 && dotCount >= 1);
}

void deleteContact(int index) {
    for (int i = index; i < numContacts - 1; i++) {
        contacts[i] = contacts[i + 1];
    }
    numContacts--;
    printf("Contact deleted successfully.\n");
    saveContactsToFile();
}

void displayContact(int index) {
    printf("| %-30s | %-15s | %-20s | %-30s | %-15s |\n", contacts[index].name, contacts[index].phone,
           contacts[index].city, contacts[index].email, contacts[index].group);
}

void displayAllContacts(const char *group) {
    printf("+--------------------------------+-----------------+----------------------+--------------------------------+-----------------+\n");
    printf("|              Name              |     Phone       |         City         |             Email              |      Group      |\n");
    printf("+--------------------------------+-----------------+----------------------+--------------------------------+-----------------+\n");

    if (numContacts == 0) {
        printf("|             No contacts found.                                         |\n");
    } else {
        for (int i = 0; i < numContacts; i++) {
            if (strcmp(group, "All") == 0 || strcmp(contacts[i].group, group) == 0) {
                displayContact(i);
            }
        }
    }
    printf("+--------------------------------+-----------------+----------------------+--------------------------------+-----------------+\n");
}

int main() {
    loadContactsFromFile();

    int choice;
    char keyword[MAX_NAME_LENGTH];
    int option;
    int index;
    char group[MAX_GROUP_LENGTH];

    do {
        printf("\nContact Management System\n");
        printf("1. Add Contact\n");
        printf("2. Display All Contacts\n");
        printf("3. Search Contact\n");
        printf("4. Edit Contact\n");
        printf("5. Delete Contact\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                Contact newContact;
                printf("Enter name: ");
                getchar(); // Consume newline character left in buffer
                fgets(newContact.name, sizeof(newContact.name), stdin);
                newContact.name[strcspn(newContact.name, "\n")] = '\0'; // Remove trailing newline
                printf("Enter phone number (up to 10 numerical digits only): ");
                do {
                    scanf("%s", newContact.phone);
                    if (strlen(newContact.phone) != 10 || !isNumeric(newContact.phone)) {
                        printf("Invalid phone number. Please enter exactly 10 numerical digits only: ");
                    }
                } while (strlen(newContact.phone) != 10 || !isNumeric(newContact.phone));
                printf("Enter city: ");
                scanf("%s", newContact.city);
                printf("Enter email address: ");
                do {
                    scanf("%s", newContact.email);
                    if (!isValidEmail(newContact.email)) {
                        printf("Invalid email address. Please enter a valid email address: ");
                    }
                } while (!isValidEmail(newContact.email));
                printf("Enter group: ");
                scanf("%s", newContact.group);
                if (numContacts < MAX_CONTACTS) {
                    contacts[numContacts++] = newContact;
                    printf("Contact added successfully.\n");
                    saveContactsToFile();
                } else {
                    printf("Cannot add more contacts. Contact list is full.\n");
                }
                break;
            }
            case 2:
                printf("Enter group (or type 'All' to display all contacts): ");
                scanf("%s", group);
                displayAllContacts(group);
                break;
            case 3:
                printf("Enter group (or type 'All' to search all contacts): ");
                scanf("%s", group);
                printf("Search by:\n");
                printf("1. Name\n");
                printf("2. Phone Number\n");
                printf("3. City\n");
                printf("4. Email\n");
                printf("Enter your choice: ");
                scanf("%d", &option);
                printf("Enter search keyword: ");
                scanf("%s", keyword);
                index = searchContact(keyword, option, group);
                if (index != -1) {
                    displayContact(index);
                } else {
                    printf("Contact not found.\n");
                }
                break;
            case 4:
                printf("Enter search keyword: ");
                scanf("%s", keyword);
                printf("Search by:\n");
                printf("1. Name\n");
                printf("2. Phone Number\n");
                printf("3. City\n");
                printf("4. Email\n");
                printf("Enter your choice: ");
                scanf("%d", &option);
                index = searchContact(keyword, option, "All");
                if (index != -1) {
                    editContact(index);
                }
                break;
            case 5:
                printf("Enter search keyword: ");
                scanf("%s", keyword);
                printf("Search by:\n");
                printf("1. Name\n");
                printf("2. Phone Number\n");
                printf("3. City\n");
                printf("4. Email\n");
                printf("Enter your choice: ");
                scanf("%d", &option);
                index = searchContact(keyword, option, "All");
                if (index != -1) {
                    deleteContact(index);
                }
                break;
            case 6:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 6);

    return 0;
}

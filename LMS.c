#include <stdio.h>
#include <string.h>

#define BOOKS_FILE "books.dat"
#define MEMBERS_FILE "members.dat"
#define MAX_BOOKS 100
#define MAX_MEMBERS 50
#define MAX_BORROWED 10

typedef struct {
    char title[100];
    char author[100];
    char isbn[13];
    int available;
} Book;

typedef struct {
    int memberId;
    char name[100];
    char borrowedBooks[MAX_BORROWED][13]; 
    int borrowedCount;
} Member;

Book books[MAX_BOOKS];
Member members[MAX_MEMBERS];
int bookCount = 0;
int memberCount = 0;

void saveBooks() {
    FILE *file = fopen(BOOKS_FILE, "wb");
    if (file) {
        fwrite(&bookCount, sizeof(int), 1, file);
        fwrite(books, sizeof(Book), bookCount, file);
        fclose(file);
        printf("Books saved successfully!\n");
    } else {
        printf("Error saving books!\n");
    }
}

void loadBooks() {
    FILE *file = fopen(BOOKS_FILE, "rb");
    if (file) {
        fread(&bookCount, sizeof(int), 1, file);
        fread(books, sizeof(Book), bookCount, file);
        fclose(file);
        printf("Books loaded successfully!\n");
    } else {
        printf("No books data found, starting with an empty library.\n");
    }
}

void saveMembers() {
    FILE *file = fopen(MEMBERS_FILE, "wb");
    if (file) {
        fwrite(&memberCount, sizeof(int), 1, file);
        fwrite(members, sizeof(Member), memberCount, file);
        fclose(file);
        printf("Members saved successfully!\n");
    } else {
        printf("Error saving members!\n");
    }
}

void loadMembers() {
    FILE *file = fopen(MEMBERS_FILE, "rb");
    if (file) {
        fread(&memberCount, sizeof(int), 1, file);
        fread(members, sizeof(Member), memberCount, file);
        fclose(file);
        printf("Members loaded successfully!\n");
    } else {
        printf("No members data found, starting with an empty member list.\n");
    }
}


void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Book storage is full!\n");
        return;
    }

    Book newBook;
    printf("Enter book title: ");
    scanf(" %[^\n]", newBook.title);
    printf("Enter book author: ");
    scanf(" %[^\n]", newBook.author);
    printf("Enter book ISBN: ");
    scanf(" %s", newBook.isbn);
    newBook.available = 1;

    books[bookCount++] = newBook;
    printf("Book added successfully!\n");
    saveBooks();
}

void removeBook() {
    char isbn[13];
    printf("Enter book ISBN to remove: ");
    scanf(" %s", isbn);

    for (int i = 0; i < bookCount; i++) {
        if (strcmp(books[i].isbn, isbn) == 0) {
            for (int j = i; j < bookCount - 1; j++) {
                books[j] = books[j + 1];
            }
            bookCount--;
            printf("Book removed successfully!\n");
            saveBooks();
            return;
        }
    }
    printf("Book not found!\n");
    
}

void searchBook() {
    char query[100];
    printf("Enter book title, author, or ISBN to search: ");
    scanf(" %[^\n]", query);

    for (int i = 0; i < bookCount; i++) {
        if (strstr(books[i].title, query) || strstr(books[i].author, query) || strcmp(books[i].isbn, query) == 0) {
            printf("Title: %s, Author: %s, ISBN: %s, Available: %s\n",
                   books[i].title, books[i].author, books[i].isbn,
                   books[i].available ? "Yes" : "No");
        }
    }
}

void listBooks() {
    for (int i = 0; i < bookCount; i++) {
        printf("Title: %s, Author: %s, ISBN: %s, Available: %s\n",
               books[i].title, books[i].author, books[i].isbn,
               books[i].available ? "Yes" : "No");
    }
}

void registerMember() {
    if (memberCount >= MAX_MEMBERS) {
        printf("Member storage is full!\n");
        return;
    }

    Member newMember;
    printf("Enter member name: ");
    scanf(" %[^\n]", newMember.name);
    printf("Enter member ID: ");
    scanf("%d", &newMember.memberId);
    newMember.borrowedCount = 0;

    members[memberCount++] = newMember;
    printf("Member registered successfully!\n");
    saveMembers();
}

void borrowBook() {
    int memberId;
    char isbn[13];

    printf("Enter member ID: ");
    scanf("%d", &memberId);
    printf("Enter book ISBN to borrow: ");
    scanf(" %s", isbn);

    for (int i = 0; i < memberCount; i++) {
        if (members[i].memberId == memberId) {
            if (members[i].borrowedCount >= MAX_BORROWED) {
                printf("Member has reached the borrowing limit!\n");
                return;
            }

            for (int j = 0; j < bookCount; j++) {
                if (strcmp(books[j].isbn, isbn) == 0 && books[j].available) {
                    books[j].available = 0;
                    strcpy(members[i].borrowedBooks[members[i].borrowedCount++], isbn);
                    printf("Book borrowed successfully!\n");
                    saveBooks();
                    saveMembers();
                    return;
                }
            }
            printf("Book not available or not found!\n");
            return;
        }
    }
    printf("Member not found!\n");
}

void returnBook() {
    int memberId;
    char isbn[13];

    printf("Enter member ID: ");
    scanf("%d", &memberId);
    printf("Enter book ISBN to return: ");
    scanf(" %s", isbn);

    for (int i = 0; i < memberCount; i++) {
        if (members[i].memberId == memberId) {
            for (int j = 0; j < members[i].borrowedCount; j++) {
                if (strcmp(members[i].borrowedBooks[j], isbn) == 0) {
                    books[j].available = 1;
                    for (int k = j; k < members[i].borrowedCount - 1; k++) {
                        strcpy(members[i].borrowedBooks[k], members[i].borrowedBooks[k + 1]);
                    }
                    members[i].borrowedCount--;
                    printf("Book returned successfully!\n");
                    saveBooks();
                    saveMembers();
                    return;
                }
            }
            printf("Book not borrowed by this member!\n");
            return;
        }
    }
    printf("Member not found!\n");
}

void listMembers() {
    for (int i = 0; i < memberCount; i++) {
        printf("Member ID: %d, Name: %s, Borrowed Books: %d\n",
               members[i].memberId, members[i].name, members[i].borrowedCount);
    }
}

int main() {
    loadBooks();
    loadMembers();
    int choice;
    while (1) {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. Remove Book\n");
        printf("3. Search Book\n");
        printf("4. List All Books\n");
        printf("5. Register Member\n");
        printf("6. Borrow Book\n");
        printf("7. Return Book\n");
        printf("8. List All Members\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                removeBook();
                break;
            case 3:
                searchBook();
                break;
            case 4:
                listBooks();
                break;
            case 5:
                registerMember();
                break;
            case 6:
                borrowBook();
                break;
            case 7:
                returnBook();
                break;
            case 8:
                listMembers();
                break;
            case 9:
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_FIELDS 4
#define RECORD_SIZE 1024

char *file_path = "library-records.txt";

//--------------General util-----------

int read_int(char message[]) {
  int number;

  printf("%s", message);
  scanf("%d", &number);
  while (getchar() != '\n' && getchar() != EOF)
    ;

  return number;
}

char *read_string(char message[]) {
  char *string = malloc(RECORD_SIZE);

  printf("%s", message);
  fgets(string, RECORD_SIZE, stdin);
  string[strcspn(string, "\n")] = '\0';

  return string;
}

char read_char(char message[]) {
  char ch;

  printf("%s", message);
  scanf("%c", &ch);
  while (getchar() != '\n' && getchar() != EOF)
    ;

  return ch;
}

//----------------General util----------

//---------------Program util---------------

void add_book_record(char *record) {
  FILE *records = fopen(file_path, "a"); // Opening the file.
  fprintf(records, "%s", record);
  fprintf(records, "\n");
  fclose(records);
}

void delete_book_record(char *file_path, int line_to_remove) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    perror("Error opening the file.");
    return;
  }

  FILE *temp = fopen("temp.txt", "w");
  if (temp == NULL) {
    perror("Error opening the file.");
    fclose(file);
    return;
  }

  char buffer[RECORD_SIZE];
  int line_number = 1;

  while (fgets(buffer, sizeof(buffer), file)) {
    if (line_number != line_to_remove)
      fputs(buffer, temp);
    line_number++;
  }

  fclose(temp);
  fclose(file);

  if (remove(file_path) != 0) {
    printf("Line didn't remove.\n");
    perror("Error deleting the file");
    return;
  }

  if (rename("temp.txt", file_path) != 0) {
    printf("Line didn't remove.\n");
    perror("Error renaming the file");
    return;
  }
  printf("Line removed successfully.\n");
}

int line_number(char *record) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    printf("Something went wrong\n");
    perror("Error opening the file.");
    return -1;
  }

  char temp_record[RECORD_SIZE];
  int number = 1;

  while (fgets(temp_record, sizeof(temp_record), file) != NULL) {
    if (strcmp(record, temp_record) == 0) {
      return number;
    }
    number++;
  }
  printf("No such record\n");
  return -1;
}

void tokenizing(char *tokens[4], char *record) {
  int token_count = 0;

  // Remove newline character if present
  record[strcspn(record, "\n")] = 0;

  // Make a copy of the string to avoid modifying the original record
  char *record_copy = strdup(record);

  char *token = strtok(record_copy, "#");

  while (token != NULL) {
    tokens[token_count] = token;
    token_count++;

    token = strtok(NULL, "#");
  }
}

void print_book_info(char *record_array[]) {
  printf("-----------------------------\n");
  printf("ID: %s\n", record_array[0]);
  printf("Book title: %s\n", record_array[1]);
  printf("Author(s): %s\n", record_array[2]);
  printf("Publisher: %s\n", record_array[3]);
  printf("-----------------------------\n");
}

void read_book_info(char *record) {

  char *id = malloc(256);
  char *name = malloc(256);
  char *author = malloc(256);
  char *publisher = malloc(256);

  id = read_string("Enter the ID: ");

  name = read_string("Enter the name: ");

  author = read_string("Enter the author(s): ");

  publisher = read_string("Enter the publisher(s): ");

  snprintf(record, RECORD_SIZE, "%s#%s#%s#%s", id, name, author, publisher);

  free(id);
  free(name);
  free(author);
  free(publisher);
}

//----------------Program util--------------

//--------------Functionality---------------

void add_book() {
  char record[256];

  read_book_info(record);

  add_book_record(record);
}

// Shouldn't we just delete the book using it's ID?
// why do we need to read the book info again?
void remove_book() {
  char record[RECORD_SIZE];
  char *fields[4];

  read_book_info(record);
  tokenizing(fields, record);
  print_book_info(fields);

  strcat(record, "\n");
  int line_to_remove = line_number(record);

  delete_book_record(file_path, line_to_remove);
}

void search_book(char *search_string, short int search) {
  FILE *records = fopen(file_path, "r");

  if (records == NULL) {
    printf("Error: The file is empty or there's no file at all.\n");
    return;
  }

  char record[RECORD_SIZE];

  char *record_array[NUMBER_OF_FIELDS];

  while (fgets(record, sizeof(record), records) != NULL) {
    tokenizing(record_array, record);
    // printf("The book is:%s\n", record_array[0]);
    switch (search) {
    case 0:
      if (strcmp(record_array[0], search_string) == 0) {
        printf("The book is:\n");
        print_book_info(record_array);
        return;
      }
      break;

    case 1:
      if (strcmp(record_array[1], search_string) == 0) {
        printf("The book is:\n");
        print_book_info(record_array);
        return;
      }
      break;

    default:
      printf("Undefined search...\n");
      break;
    }
  }

  printf("There's no such book...\n");
  fclose(records);
}

void show_all_books() {
  FILE *records = fopen(file_path, "r");

  char *record = malloc(RECORD_SIZE);
  char *record_info[NUMBER_OF_FIELDS];

  printf("_____________________________________________________________________"
         "_____________________________________________________________________"
         "______________________________________________\n");
  printf(" ID  |                         BOOK NAME                             "
         "  |                                   AUTHOR(s)                      "
         "                   |           Publisher(s)  \n");
  printf("_____________________________________________________________________"
         "_____________________________________________________________________"
         "______________________________________________\n");

  while (fgets(record, RECORD_SIZE, records) != NULL) {
    tokenizing(record_info, record);
    printf("%5s|%65s|%85s|%25s\n", record_info[0], record_info[1],
           record_info[2], record_info[3]);
  }

  fclose(records);
}

//--------------Functionality---------------

//---------------Screens------------

void Main_screen() {
  printf("--------------------------\n");
  printf("(1) Add book.\n");
  printf("(2) Search books by ID.\n");
  printf("(3) Search books by name.\n");
  printf("(4) Remove book. \n");
  printf("(5) Show all books. \n");
  printf("--------------------------\n");
}

void program_choice() {
  char choice;

  choice = read_int("Enter your choice: ");

  switch (choice) {
  case 1:
    system("clear");
    add_book();
    break;

  case 2:
    system("clear");

    char *book_id = malloc(RECORD_SIZE);

    book_id = read_string("Enter the ID: ");
    search_book(book_id, 0);

    free(book_id);
    break;

  case 3:
    system("clear");

    char *book_name = malloc(RECORD_SIZE);

    book_name = read_string("Enter the name: ");
    search_book(book_name, 1);

    free(book_name);
    break;

  case 4:
    system("clear");

    remove_book();
    break;

  case 5:
    system("clear");

    show_all_books();
    break;

  default:
    printf("Wroing input...\n");
    break;
  }
}

//---------------Screens------------

void run_program() {
  char choice;

  do {
    Main_screen();
    program_choice();

    choice = read_char("Do another operation? (y/n): ");

    system("clear");
  } while (choice == 'y' || choice == 'Y');
}

int main() {
  run_program();
  return 0;
}

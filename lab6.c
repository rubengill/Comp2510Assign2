#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Flag that we can use to determine what type of student 
typedef enum {
    DOMESTIC,
    INTERNATIONAL
} StudentType;

//Domestic student structure
typedef struct {
    char *firstName;
    char *lastName;
    char month[4];
    char *day;
    char *year;
    char gpa[6];
} DomesticStudent;

//International student structure 
typedef struct {
    char *firstName;
    char *lastName;
    char month[4];
    char *day;
    char *year;
    char gpa[6];
    int toefl;
} InternationalStudent;

// Defines a node in the linked list, which can either be a dStudent or iStudent
typedef struct StudentNode {
    // Flag to determine the type of student it is
    StudentType type;

    // Sets aside memory for an international student, which a domestic student can also occupy
    union {
        DomesticStudent dStudent;
        InternationalStudent iStudent;
    } student;

    // Link to the next node
    struct StudentNode *next;
} StudentNode;

// Trim leaing and trailing white space 
    char* trimWhiteSpace(char *buffer, FILE *fp_out) {
        char *start = buffer;
        char *end;

        // Trim leading space
        while(isspace((unsigned char)*start)) start++;

        // All spaces
        if(*start == 0) {
            // String is all spaces or empty
            fprintf(fp_out, "Error: Input string contains only whitespace!\n");
            exit(EXIT_FAILURE); // Exit immediately, no need to allocate emptyStr
        }

        // Trim trailing space
        end = start + strlen(start) - 1;
        while(end > start && isspace((unsigned char)*end)) end--;

        // Write new null terminator
        *(end + 1) = '\0';

        // Allocate memory for the trimmed string, + 2 for the character and null terminator
        int trimmedLength = end - start + 2; 
        char *newStr = (char*)malloc(trimmedLength);
        if (newStr == NULL) {
            printf("Error: Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        strcpy(newStr, start);
        return newStr;
    }

// Checks if a string can be converted to a double 
int isValidDouble(char *str) {
    char *endptr;
    strtod(str, &endptr);
    return *endptr == '\0' || *endptr == '\n';
}

// Checks if a string can be converted to an integer
int isValidInt(char *str) {
    char *endptr;
    strtol(str, &endptr, 10);
    return *endptr == '\0' || *endptr == '\n';
}

//Validate the specific dates


//Validate the birthday for the student 
void validateBirthday(char *birthday, char *month, int *day, int *year, FILE *fp_out) {
    char *token;
    int tokenCount = 0;
    int numMonth = sizeof(validMonths) / sizeof(validMonths[0]);
    int monthFound = 0;

    char *validMonths[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    token = strtok(token, "-");
    if (token == NULL) {
        fprintf(fp_out, "Error: Invalid birthday format\n");
        exit(EXIT_FAILURE); 
    }

    // Compare token with valid strings in month array 
     for (int i = 0; i < numMonth; i++) {
        if (strcmp(token, validMonths[i]) == 0) {
            strncpy(month, token, 3); 
            month[3] = '\0'; 
            monthFound = 1;
            // Don't need to check the other values 
            break;
        }
        tokenCount++;
    }

    if (!monthFound) {
        fprintf(fp_out, "Error: Month is not valid !");
        exit(EXIT_FAILURE);
    }

    token = strtok(NULL,  "-");
    // Validate the day
    if (token == NULL || (((*day = atoi(token)) <= 0 || ((*day = atoi(token)) >= 32)))) {
        fprintf(fp_out, "Error: Day is not valid!");
        exit(EXIT_FAILURE);
    }

    *day = (int)strtol(token, NULL, 10);
    tokenCount++;

    token = strtok(NULL,  "-");
    // Validate the year
    if (token == NULL || (((*year = atoi(token)) <= 2010 || ((*year = atoi(year)) >= 1950)))) {
        fprintf(fp_out, "Error: Year is not valid!");
        exit(EXIT_FAILURE);
    }

    *year = (int)strtol(token, NULL, 10);
    tokenCount++;

    //Check if there are more tokens
    if(strtok(NULL, "-") != NULL) {
        fprintf(fp_out, "Error: Too many tokens in the birthday line!\n");
        exit(EXIT_FAILURE);
    }        

    // Check if there were less than 3 tokens, which means the format is incorrect
    if (tokenCount < 3) {
        fprintf(fp_out, "Error: Not enough tokens in the birthday string!\n");
        exit(EXIT_FAILURE)
    }
}

// Parse the input line, and store the data in the appropiate variables defined in the main method 
void parseString(char *line, char **fName, char **lName, char **birthday, char *gpaArr, double *gpa, char *type, int *toefl, FILE *fp_out) {
    char *token;
    int tokenCount = 0;
    
    token = strtok(line, " ");

    if (token == NULL || !isalpha(*token)) {
        fprintf(fp_out, "Error: Invalid first name\n");
        exit(EXIT_FAILURE);
    }

    // Dereference double pointer to get the mem address stored in the pointer to a string, and change the string it points too 
    *fName = strdup(token);
    tokenCount++;

    token = strtok(NULL, " ");
    if (token == NULL || !isalpha(*token)) {
        fprintf(fp_out, "Error: Invalid last name\n");
        exit(EXIT_FAILURE);
    }

    // Store last name
    *lName = strdup(token);
    tokenCount++;

    // Validate birthday
    token = strtok(NULL, " ");
    if (token == NULL) {
        fprintf(fp_out, "Error: Invalid birthday\n");
        exit(EXIT_FAILURE);
    }
    *birthday = strdup(token);
    validateBirthday(birthday);
    tokenCount++;

    // Validate GPA 
    token = strtok(NULL, " ");
    if (token == NULL || !isValidDouble(token)) {
        fprintf(fp_out, "Error: Invalid GPA\n");
        exit(EXIT_FAILURE);
    }
    // Store the gpa String in gpaStr, so it can be passed into student structure. 
    strncpy(gpaArr, token, 5);
    gpaArr[5] = '\0';
    *gpa = strtod(token, NULL);
    if (*gpa < 0.0 || *gpa > 4.33) {
        fprintf(fp_out, "Error: GPA cannot be negative or greater than 4.33\n");
        exit(EXIT_FAILURE);
    }
    tokenCount++;

    // Validate the type
    token = strtok(NULL, " ");
    if (token == NULL || (token[0] != 'I' && token[0] != 'i' && token[0] != 'D' && token[0] != 'd')) {
        fprintf(fp_out, "Error: Invalid student type\n");
        exit(EXIT_FAILURE);
    }

    *type = token[0];
    tokenCount++;

    // Valid toefl is student is international
    if (*type == 'I' || *type == 'i') {
        token = strtok(NULL, " ");
        if(token == NULL || !isValidInt(token)) {
            fprintf(fp_out, "Error: Invalid TOEFL\n");
            exit(EXIT_FAILURE);
        }
        *toefl = (int)strtol(token, NULL, 10);
        if (*toefl < 0) {
            fprintf(fp_out, "Error: TOEFL score cannot be negative\n");
            exit(EXIT_FAILURE);
        } else if (*toefl > 120) {
	    fprintf(fp_out, "Error: TOEFL score cannot be more than 120\n");
	    exit(EXIT_FAILURE);
        }
        tokenCount++;
    }
    
    //Check if there are more tokens
    if(strtok(NULL, " ") != NULL) {
        fprintf(fp_out, "Error: Too many tokens in the line!\n");
        exit(EXIT_FAILURE);
    }
    
    // Gatekeeper to ensure that there are only 4 or 5 tokens 
    if (((*type == 'I' || *type == 'i') && tokenCount != 5) || ((*type == 'D' || *type == 'd') && tokenCount != 4)) {
        fprintf(fp_out, "Error: Incorrect number of data fields\n");
        exit(EXIT_FAILURE);
    }
}

// Create a domestic student 
DomesticStudent *createDStudent(char *fName, char *lName, char *gpaArr) {
    DomesticStudent *dStudent = (DomesticStudent *)malloc(sizeof(DomesticStudent));

    if (dStudent == NULL) {
        printf("Error: Can't create dStudent");
        exit(EXIT_FAILURE);
    }

    // Create another duplicate of names so when memory is freed there is no dangling pointers
    dStudent -> firstName = strdup(fName);
    if (dStudent -> firstName == NULL) {
        printf("Memory allocation failed for firstname\n");
        free(dStudent);
        exit(EXIT_FAILURE);
    }    
    dStudent -> lastName = strdup(lName);
    strncpy(dStudent -> gpa, gpaArr, sizeof(dStudent->gpa) - 1);
    dStudent -> gpa[sizeof(dStudent->gpa) - 1] = '\0';

    return dStudent;
}

// Create an international student 
InternationalStudent *createIStudent(char *fName, char *lName, char *gpaArr, int toefl) {
    InternationalStudent *iStudent = (InternationalStudent *)malloc(sizeof(InternationalStudent));

    if (iStudent == NULL) {
        printf("Error: Can't craete iStudent");
        exit(EXIT_FAILURE);
    }

    iStudent -> firstName = strdup(fName);
    iStudent -> lastName = strdup(lName);
    strncpy(iStudent -> gpa, gpaArr, sizeof(iStudent -> gpa) - 1);
    iStudent -> gpa[sizeof(iStudent->gpa) - 1] = '\0';
    iStudent -> toefl = toefl;

    return iStudent;
}

// Create a student node depending on the type 
StudentNode *createStudentNode(StudentType studentType, void *studentStruct) {
    // Create student node,
    StudentNode *newStudent = (StudentNode *)malloc(sizeof(StudentNode));

    if (newStudent == NULL) {
        printf("Error: Can't create Node");
        exit(EXIT_FAILURE);
    }

    newStudent -> type = studentType;

    if (studentType == INTERNATIONAL) {
        // Cast to appropiate student, and derefernce to get access the struct 
        InternationalStudent *original = (InternationalStudent *) studentStruct;
        // Deep copy each string
        newStudent -> student.iStudent.firstName = strdup(original -> firstName);
        newStudent -> student.iStudent.lastName = strdup(original -> lastName);
        strcpy(newStudent -> student.iStudent.gpa, original -> gpa);
        newStudent -> student.iStudent.toefl = original -> toefl;
    } else if (studentType == DOMESTIC) {
        DomesticStudent *original = (DomesticStudent *) studentStruct;
        newStudent -> student.dStudent.firstName = strdup(original -> firstName);
        newStudent -> student.dStudent.lastName = strdup(original -> lastName);
        strcpy(newStudent -> student.dStudent.gpa, original -> gpa);
    }

    newStudent -> next = NULL;

    return newStudent;
}

//Adds a student node to the end of the list 
void addToList(StudentNode **head, StudentNode *newNode) {
    if (*head == NULL) {
        *head = newNode;
    } else {
        StudentNode* current = *head;
        while(current -> next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Function to print information of an international student
void printInternationalStudent(InternationalStudent *student, FILE *fp_out) {
    fprintf(fp_out, "%s %s %s I %d\n", student -> firstName, student -> lastName, student -> gpa, student -> toefl);
}

// Function to print information of a domestic student
void printDomesticStudent(DomesticStudent *student, FILE *fp_out) {
    fprintf(fp_out, "%s %s %s D\n", student -> firstName, student -> lastName, student -> gpa);
}

//Print the Students
void printStudents(StudentNode *head, FILE *fp_out, int option) {
    // Iterate through the linked list
    for (StudentNode *current = head; current != NULL; current = current->next) {
        // Print based on the option provided
        switch (option) {
            // Domestic students only
            case 1: 
                if (current -> type == DOMESTIC) {
                    printDomesticStudent(&(current -> student.dStudent), fp_out);
                }
                break;
            // International students only
            case 2: 
                if (current -> type == INTERNATIONAL) {
                    printInternationalStudent(&(current -> student.iStudent), fp_out);
                }
                break;
            // / All students
            case 3: 
                if (current -> type == INTERNATIONAL) {
                    printInternationalStudent(&(current -> student.iStudent), fp_out);
                } else if (current -> type == DOMESTIC) {
                    printDomesticStudent(&(current -> student.dStudent), fp_out);
                }
                break;
            default:
                fprintf(fp_out, "Invalid option\n");
                break;
        }
    }
}

// Free the nodes in the list 
void freeList(StudentNode *head) {
    StudentNode *current = head;
    while (current != NULL) {
        StudentNode *next = current -> next;

        // Free student structs 
        if (current -> type == INTERNATIONAL) {
            free(current -> student.iStudent.firstName);
            free(current -> student.iStudent.lastName);
        } else if (current -> type == DOMESTIC) {
            free(current -> student.dStudent.firstName);
            free(current -> student.dStudent.lastName);
        }
        free(current);
        current = next;
    }
}

// Entry to the program
int main(int argc, char *argv[]) {

    if (argc != 4) {
        perror("Error: There must be 4 command line arguments");
        return 1;
    }

    // Get command line arguments
    char *inputFileName = argv[1];
    char *outputFileName = argv[2];
    int option = atoi(argv[3]);

    FILE *fp = fopen(inputFileName, "r");
    if (!fp) { 
        perror("Error: Can't find the output file");
        // Exits the program
        return 1;
    }

    FILE *fp_out = fopen(outputFileName, "w");
    if (!fp_out) {
        perror("Error: Can't open the output file.");
        fclose(fp);
        return 1;
    }

    // Gets written to the output file 
    if (option < 1 || option > 3) {
        fprintf(fp_out, "Error: Option must be between 1 and 3\n");
        fclose(fp_out);
        fclose(fp);
        return 1;
    }

    // Create the head of a node 
    StudentNode *head = NULL;

    char *fName = NULL;
    char *lName = NULL;

    char month[4] = " ";
    char *birthday = NULL;

    char gpaArr[6] = " ";

    int yearVal;
    int *yearPtr = &yearVal;

    int dayVal;
    int *dayPtr = &dayVal;

    double gpaVal;
    double *gpaPtr = &gpaVal; 

    char typeVal;
    char *typePtr = &typeVal;

    int toeflVal;
    int *toeflPtr = &toeflVal;

    
    char buffer[1000];
    
    // Get the line from the text tile
    while (fgets(buffer, sizeof(buffer), fp)) {
        
        // Check if the new line character is before the EOF 
        if (buffer[0] == '\n' && buffer[1] == '\0') {
             
        int next_char = fgetc(fp);
        if (next_char == EOF) {
            break;
        } else {
            // Handle as an error or skip as per your requirements
            fprintf(fp_out, "Error: Empty line found in input file.\n");
            exit(EXIT_FAILURE);
            }
        }

        typeVal = '\0';        
        strcpy(gpaArr, " ");
        strcpy(month, " ");
        
        // *line points to an array with no trailing white spaces
        char *line = trimWhiteSpace(buffer, fp_out);
 
        // Parse the string, assign values to appropiate variables 
        parseString(line, &fName, &lName, &birthday, gpaArr, gpaPtr, typePtr, toeflPtr, fp_out);

        // Create appropiate student struct, add to linked list 
        if ((typeVal == 'I' || typeVal == 'i')) {
           // Create Structure
            InternationalStudent *iStudent = createIStudent(fName, lName, gpaArr, toeflVal);

            // Create Node
            StudentNode *studentNode = createStudentNode(INTERNATIONAL, (void*)iStudent);

            //Add to linked list
            addToList(&head, studentNode);
        } else if ((typeVal == 'D' || typeVal == 'd')) {
            // Create Structure
            DomesticStudent *dStudent = createDStudent(fName, lName, gpaArr);
            
            // Create Node
            StudentNode *studentNode = createStudentNode(DOMESTIC, (void*)dStudent);

            //Add to linked list
            addToList(&head, studentNode);
        }
        //Free cleared trailing white space memory 
        free(line);
	    line = NULL;
        free(fName);
        free(lName);
        free(birthday);
        fName = NULL;
        lName = NULL;
        birthday = NULL;
    }

    printStudents(head, fp_out, option);

    freeList(head);

     // A numbers of everyone. AXXXX_AXXXX_AXXX format.
    char *ANum = "A00992793_";
    FILE *outputFile = fopen(ANum, "w");
    if (outputFile == NULL) {
    printf("Failed to create the output file.\n");
    return 1;
    }
    
    fclose(outputFile);
    
    fclose(fp);
    fclose(fp_out);

    return 0;
}

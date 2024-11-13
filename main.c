#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;
typedef struct Transaction Transaction;

struct Node {
    int id;
    int value;
    Transaction** transactions;
};

struct Transaction {
    char character;
    Node* next;
};

char* alphabet;
int alphabet_size = 0;
int node_size = 0;
int** matrix;

char* removeSpaces(char* str) {
    int i = 0, j = 0;
    while (str[i]) {
        if (str[i] != ' ') {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0';
    return str;
}

Node* createNode(int node_index) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->id = matrix[node_index][0];
    newNode->value = matrix[node_index][alphabet_size + 1];
    newNode->transactions = (Transaction**)malloc(sizeof(Transaction*) * alphabet_size);

    return newNode;
}

Node* buildMooreMachine(Node** nodes) {
    for (int i = 0; i < node_size; i++) {
        for (int j = 1; j < alphabet_size + 1; j++) {
            nodes[i]->transactions[j - 1] = malloc(sizeof(Transaction));
            nodes[i]->transactions[j - 1]->next = nodes[matrix[i][j]];
            nodes[i]->transactions[j - 1]->character = alphabet[j - 1];
        }
    }

    return nodes[0];
}

char* runMachine(Node* node, char* word) {
    Node* ptr = node;
    char* output = malloc(strlen(word) * sizeof(char));
    char* output_ptr = output;

    for(int j = 0; j < strlen(word); j++) {
        for(int i = 0; i < alphabet_size; i++) {
            if (ptr->transactions[i]->character == word[j]) {
                *output_ptr++ = (char)(ptr->value + '0');
                ptr = ptr->transactions[i]->next;
                break;
            }
        }
    }
    *output_ptr++ = (char)(ptr->value + '0');
    *output_ptr = '\0';
    return output;
}

void readFromInputFile() {
    int character; int size_buff = 10; int buff_index = 0; char* buffer = malloc(size_buff * sizeof(char));
    int Q_flag = 0; int E_flag = 0; int start_flag = 0;
    FILE* file = fopen("input.txt", "r");

    while ((character = fgetc(file)) != EOF) {
        if(buff_index == size_buff) {
            size_buff *= 2;
            buffer = realloc(buffer, size_buff * sizeof(char));
        }

        if (character == '{') {
            start_flag = 1;
            continue;
        }
        if (character == '}') {
            start_flag = 0;
            continue;
        }
        if (character == 'Q') {
            Q_flag = 1;
            E_flag = 0;
            continue;
        }
        if (character == 'E') {
            E_flag = 1;
            Q_flag = 0;
            continue;
        }
        if (start_flag && Q_flag && !isspace(character) && character != ',') {
            node_size++;
        }
        if (start_flag && E_flag && !isspace(character) && character != ',') {
            alphabet_size++;
            buffer[buff_index++] = character;
        }
    }

    buffer[buff_index] = '\0';
    alphabet = buffer;
    node_size /= 2;

    matrix = malloc(node_size * sizeof(int*));
    for (int i = 0; i < node_size; i++) {
        matrix[i] = malloc(sizeof(int) * (alphabet_size + 2));
    }

    fclose(file);
}

void readFromTable() {
    int buffer_size = 1024; int line_index = 0; char buffer[buffer_size];
    FILE* file = fopen("GECISTABLOSU.txt", "r");

    while(fgets(buffer, buffer_size, file)) {
        // Yeni satır karakterini kaldırın
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        if (line_index > 0) {
            // Başlık satırını atlıyoruz
            int column_index = 0;
            char *token = strtok(buffer, "");
            token = removeSpaces(token);

            for (int i = 0; i < strlen(token); i++) {
                matrix[line_index - 1][i] = (int)token[i] - '0';
            }
        }

        line_index++;
    }
}

int main(void)
{
    char* string = malloc(sizeof(char) * 1024);
    readFromInputFile();
    readFromTable();

    Node** nodes = malloc(sizeof(Node*) * node_size);

    for (int i = 0; i < node_size; i++) {
        nodes[i] = createNode(i);
    }

    Node* starter_node = buildMooreMachine(nodes);
    printf("Lütfen kontrol etmek istediğiniz string'i giriniz:");
    scanf("%99[^\n]", string);
    string = runMachine(starter_node, string);
    printf("%s\n", string);

    return 0;
}

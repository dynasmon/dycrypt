#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h> // Biblioteca para gerar hash SHA-1

#define MAX_TEXT_SIZE 1000

// Definindo a estrutura de uma lista duplamente encadeada
typedef struct Node {
    char character;
    struct Node* next;
    struct Node* prev;
} Node;

// Função para criar um novo nó
Node* create_node(char character) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->character = character;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

// Função para adicionar um caractere ao final da lista
typedef struct DoublyLinkedList {
    Node* head;
    Node* tail;
} DoublyLinkedList;

void append(DoublyLinkedList* list, char character) {
    Node* new_node = create_node(character);
    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        new_node->prev = list->tail;
        list->tail = new_node;
    }
}

// Função para criptografar usando a cifra de César
void encrypt(DoublyLinkedList* list, int shift) {
    Node* current = list->head;
    while (current != NULL) {
        char ch = current->character;

        if (ch >= 'a' && ch <= 'z') {
            ch = (ch - 'a' + shift) % 26 + 'a';
        } else if (ch >= 'A' && ch <= 'Z') {
            ch = (ch - 'A' + shift) % 26 + 'A';
        }

        current->character = ch;
        current = current->next;
    }
}

// Função para descriptografar usando a cifra de César
void decrypt(DoublyLinkedList* list, int shift) {
    Node* current = list->head;
    while (current != NULL) {
        char ch = current->character;

        if (ch >= 'a' && ch <= 'z') {
            ch = (ch - 'a' - shift + 26) % 26 + 'a';
        } else if (ch >= 'A' && ch <= 'Z') {
            ch = (ch - 'A' - shift + 26) % 26 + 'A';
        }

        current->character = ch;
        current = current->next;
    }
}

// Função para imprimir a lista
void print_list(DoublyLinkedList* list) {
    Node* current = list->head;
    while (current != NULL) {
        printf("%c", current->character);
        current = current->next;
    }
    printf("\n");
}

// Estrutura de um nó da árvore AVL
typedef struct AVLNode {
    char* line;
    unsigned char hash[SHA_DIGEST_LENGTH];
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

// Função para criar um novo nó da árvore AVL
AVLNode* create_avl_node(char* line) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->line = strdup(line);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    SHA1((unsigned char*)line, strlen(line), node->hash); // Gera o hash da linha
    return node;
}

// Função para obter a altura de um nó
int height(AVLNode* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Função para obter o valor máximo entre dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Rotação à direita
AVLNode* right_rotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Rotação à esquerda
AVLNode* left_rotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Função para obter o fator de balanceamento de um nó
int get_balance(AVLNode* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Função para inserir um nó na árvore AVL
AVLNode* insert_avl(AVLNode* node, char* line) {
    if (node == NULL)
        return create_avl_node(line);

    if (strcmp(line, node->line) < 0)
        node->left = insert_avl(node->left, line);
    else if (strcmp(line, node->line) > 0)
        node->right = insert_avl(node->right, line);
    else // Linhas iguais não são permitidas na árvore AVL
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = get_balance(node);

    // Rotação à direita
    if (balance > 1 && strcmp(line, node->left->line) < 0)
        return right_rotate(node);

    // Rotação à esquerda
    if (balance < -1 && strcmp(line, node->right->line) > 0)
        return left_rotate(node);

    // Rotação à esquerda-direita
    if (balance > 1 && strcmp(line, node->left->line) > 0) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    // Rotação à direita-esquerda
    if (balance < -1 && strcmp(line, node->right->line) < 0) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

// Função para imprimir o hash em formato hexadecimal
void print_hash(unsigned char hash[SHA_DIGEST_LENGTH]) {
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

// Função para verificar a autenticidade do documento
int verify_authenticity(AVLNode* root, unsigned char original_hash[SHA_DIGEST_LENGTH]) {
    if (root == NULL) {
        return 0;
    }
    // Compara o hash da raiz com o hash original
    if (memcmp(root->hash, original_hash, SHA_DIGEST_LENGTH) == 0) {
        return 1; // Documento é autêntico
    }
    return 0; // Documento não é autêntico
}

int main() {
    DoublyLinkedList list;
    list.head = NULL;
    list.tail = NULL;

    char text[MAX_TEXT_SIZE];
    int shift;
    int choice;

    printf("Digite o texto (max 1000 caracteres): ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0; // Remove newline character

    for (int i = 0; i < strlen(text); i++) {
        append(&list, text[i]);
    }

    printf("Digite o valor do deslocamento: ");
    scanf("%d", &shift);

    printf("Escolha uma opcao:\n1. Criptografar\n2. Descriptografar\n3. Autenticar Documento\n4. Verificar Autenticidade\n");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            encrypt(&list, shift);
            printf("Texto criptografado: ");
            print_list(&list);
            break;
        case 2:
            decrypt(&list, shift);
            printf("Texto descriptografado: ");
            print_list(&list);
            break;
        case 3: {
            AVLNode* root = NULL;
            printf("Digite o texto do documento (max 1000 caracteres, separando linhas por ponto e virgula): ");
            getchar(); // Limpa o buffer do stdin
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = 0;

            char* line = strtok(text, ";");
            while (line != NULL) {
                root = insert_avl(root, line);
                line = strtok(NULL, ";");
            }

            printf("Hash da raiz (código de autenticação): ");
            print_hash(root->hash);
            break;
        }
        case 4: {
            AVLNode* root = NULL;
            unsigned char original_hash[SHA_DIGEST_LENGTH];

            printf("Digite o texto do documento (max 1000 caracteres, separando linhas por ponto e virgula): ");
            getchar(); // Limpa o buffer do stdin
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = 0;

            char* line = strtok(text, ";");
            while (line != NULL) {
                root = insert_avl(root, line);
                line = strtok(NULL, ";");
            }

            printf("Digite o código de autenticação original: ");
            for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
                scanf("%02hhx", &original_hash[i]);
            }

            if (verify_authenticity(root, original_hash)) {
                printf("Documento é autêntico.\n");
            } else {
                printf("Documento não é autêntico.\n");
            }
            break;
        }
        default:
            printf("Opcao invalida!\n");
            break;
    }

    return 0;
}

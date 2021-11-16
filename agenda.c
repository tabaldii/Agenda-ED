#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define EXIT 10  // valor fixo para a opção que finaliza a aplicação

//Struct que representa a data.
typedef struct {
	int day;
	int month;
	int year;
} Date;

// Estrutura que contém os campos dos registros da agenda
struct MREC {
    char name[30];
    Date birth; 
    char email[40];
    char phone[15];
	struct MREC *next;
	struct MREC *prev;
};

// Tipo criado para instanciar variáveis do tipo agenda
typedef struct MREC Contact;

// Estrutura auxiliar para guardar os cabeças
typedef struct aux {
    Contact *first;
    Contact *tail;
} Heads;

//  Iniciar os cabeças
void init(Heads *pointers) {
    pointers->first = NULL;
    pointers->tail = NULL;
}

// Apresenta o menu da aplicação e retorna a opção selecionada
int menu() {

    int opc=EXIT+1;

    printf("\nMAIN MENU \n");
    printf("------------------------\n");
    printf("1 - INSERT CONTACT\n");
    printf("2 - INSERT POSITIONED CONTACT\n");
    printf("3 - DELETE CONTACT\n");
    printf("4 - UPGRADE CONTACT\n");
    printf("5 - SEARCH CONTACT\n");
    printf("6 - LIST ALL CONTACTS\n");
    printf("%d - EXIT THE PROGRAM\n", EXIT);
    printf("\n");
    printf("TYPE YOUR OPTION: ");
    scanf("%d", &opc);
    printf("------------------------\n");
    return opc;
}

// Função para checar e-mail já existente
int checkemail(Heads verify, char *email) {

    Contact *aux = verify.first;
    int cont;

    while (aux != NULL) {
        if (strcmp(aux->email, email) == 0) {
            cont = 1;
            break;
        }
        else {
            cont = 0;
        }
        aux = aux->next;
    }
    return cont;
}

// Permite o cadastro de um contato
Heads insContact(Heads insert) {

    Contact *new;
    char verify[40];
    int used;
    
    printf("E-mail: ");
    scanf("%s", verify);
    used = checkemail(insert, verify);

    if (used == 1) {
        printf("This email already exists!\n");
        return insert;
    }

    new = (Contact *)malloc(sizeof(Contact));
    strcpy(new->email, verify);
    printf("Name: ");
    scanf("%s", new->name);
    printf("Date: ");
    scanf("%d/%d/%d", &new->birth.day, &new->birth.month, &new->birth.year);
    printf("Phone: ");
    scanf("%s", new->phone);
    new->next = NULL;
	new->prev = NULL;

    Contact *aux = insert.tail;
    
    if (insert.first == NULL) {
        insert.tail = new;
        insert.first = new;
    }
    else {
        aux->next = new;
        new->prev = aux;
        insert.tail = new;
    }
    return insert;
}

// Permite o cadastro de um contato
// deve ser passado pelo menos o email
Heads insContactAfter(Heads insert, char *email) {

    Contact *new;
    char verify[40];
    int used;
    
    printf("E-mail: ");
    scanf("%s", verify);
    used = checkemail(insert, verify);
    
    if (used == 1) {
        printf("This email already exists!\n");
        return insert;
    }

    new = (Contact *)malloc(sizeof(Contact));
    strcpy(new->email, verify);
    printf("Name: ");
    scanf("%s", new->name);
    printf("Date: ");
    scanf("%d/%d/%d", &new->birth.day, &new->birth.month, &new->birth.year);
    printf("Phone: ");
    scanf("%s", new->phone);

    new->next = NULL;
	new->prev = NULL;

    if (insert.first == NULL) {
        insert.tail = new;
        insert.first = new;
    }
    else {
        Contact *aux = insert.first;

        while (aux!=NULL && strcmp(aux->email, email) != 0) {
            aux=aux->next;
        }
        if (aux==NULL) {
            printf("E-mail not found, will be inserted as first!\n");
            new->next = insert.first;
            insert.first->prev = new;
            insert.first = new;
        }
        else {
            if (aux->next == NULL){
                aux->next = new;
                new->prev = aux;
                new->next = NULL;
                insert.tail = new;
            }
            else {
                aux->next->prev = new;
                new->prev = aux;
                new->next = aux->next;
                aux->next = new;
            }
        }
    }
    return insert;
}

// Permite excluir um contato da agenda baseado no email
Heads delContact (Heads del, char *email) {

    Contact *aux = del.first;
    if (del.first == NULL) {
      printf("There is no contact yet!\n");
      return del;
    }

    while (aux != NULL) {
        if (strcmp(aux->email, email) == 0) {
            if (aux == del.first && aux == del.tail) {
                free(aux);
                del.first = NULL;
                del.tail = NULL;
                break;
            }
            else if (aux == del.first) {
                del.first = aux->next;
                del.first->prev = NULL;
                free(aux);
                break;
            }
            else if (aux == del.tail) {
                del.tail = aux->prev;
                del.tail->next = NULL;
                free(aux);
                break;
            }
            else {
                aux->prev->next = aux->next;
                aux->next->prev = aux->prev;
                free(aux);
                break;
            }
        }
        else if (aux->next == NULL && strcmp(aux->email, email) != 0) {
            printf("E-mail not found! Contact has not been deleted!\n");
        }
        aux = aux->next;
    }
    return del;
}

// Printar a data
void printDate(Date num) {
    printf("Date: %d/%d/%d\n", num.day, num.month, num.year);
}

// Lista o conteúdo da agenda (todos os campos)
void listContacts (Heads init) {
    Contact *aux;
    if (init.first == NULL) {
      printf("There is no contact yet!\n");
      return;
    }
    else {
        for (aux = init.first; aux != NULL; aux = aux->next) {
            printf("------------------------\n");
            printf("Name: %s\n", aux->name);
            printDate(aux->birth);
            printf("E-mail: %s\n", aux->email);
            printf("Phone: %s\n", aux->phone);
            printf("------------------------\n");
        }
    }
}

// Permite consultar um contato da agenda por nome
void queryContact (Heads init,  char *name) {
    Contact *aux;
    if (init.first == NULL) {
      printf("There is no contact yet!\n");
      return;
    }
    else {

        int cont = 0;
        for (aux = init.first; aux != NULL; aux = aux->next) {
            if (strcmp(aux->name, name) == 0) {
                printf("------------------------\n");
                printf("Name: %s\n", aux->name);
                printDate(aux->birth);
                printf("E-mail: %s\n", aux->email);
                printf("Phone: %s\n", aux->phone);
                printf("------------------------\n");
                cont += 1;
            }
        }
        if (cont == 0) {
            printf("E-mail not found!\n");
        }
    }
}

// Permite a atualização dos dados de um contato da agenda
Heads upContact (Heads up, char *email) {
    Contact *aux;
    if (up.first == NULL) {
      printf("There is no contact yet!\n");
      return up;
    }
    else {

        int cont = 0;
        for (aux = up.first; aux != NULL; aux = aux->next) {
            if (strcmp(aux->email, email) == 0) {
                printf("New name: ");
                scanf("%s", aux->name);
                printf("New date: ");
                scanf("%d/%d/%d", &aux->birth.day, &aux->birth.month, &aux->birth.year);
                printf("New phone: ");
                scanf("%s", aux->phone);
                cont += 1;

            }
        }
        if (cont == 0) {
            printf("E-mail not found!\n");
        }
    }
    return up;
}

// Limpeza de todos os contatos
void freeMem(Heads clear) {
    Contact *aux;
    while (clear.first != NULL) {
        aux = clear.first;
        clear.first = clear.first->next;
        free(aux);
    }
    return;
}

// Programa principal
int main() {

    int op=EXIT+1;
    char emailafter[40];
    char emaildel[40];
    char namesearch[30];
    char emailup[40];

    Heads MContact;
    init(&MContact);

    while (op!=EXIT) {

        op=menu();
        switch(op) {

            case 1 : 
            MContact = insContact(MContact);
            break;

            case 2 :
            printf("Add after which email? ");
            scanf("%s", emailafter);
            MContact = insContactAfter(MContact, emailafter);
            break;

            case 3 : 
            printf("Email to delete: ");
            scanf("%s", emaildel);
            MContact = delContact(MContact, emaildel);
            break;

            case 4 : 
            printf("Email to update: ");
            scanf("%s", emailup);
            upContact(MContact, emailup);
            break;

            case 5 :
            printf("Name to be searched: ");
            scanf("%s", namesearch);
            queryContact(MContact, namesearch);
            break;

            case 6 : listContacts(MContact);
            break;
          }
    }
    printf("FINISHING...\n");
    freeMem(MContact); // liberar toda a memória alocada
    return 0;
}




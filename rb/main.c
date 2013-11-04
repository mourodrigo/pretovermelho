//
//  main.c
//  rb
//
//  Created by Rodrigo Bueno Tomiosso on 07/10/13.
//  Copyright (c) 2013 Rodrigo Bueno Tomiosso. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BLACK 0
#define RED 1
#define ESQUERDA 0
#define DIREITA 1

struct nodo;
struct arvore;

struct nodo {
    int cor;
    int valor;
    struct nodo *nodos[2];
}Nodo;

struct arvore {
    struct nodo *raiz;
}Arvore;

int checaVermelho ( struct nodo *raiz );
struct nodo *singular ( struct nodo *raiz, int direcao );
struct nodo *duplo ( struct nodo *raiz, int direcao );
int verificaPropriedades ( struct nodo *raiz );
struct nodo *criaNodo ( int valor );
int verificaArvore(struct nodo *raiz);
//struct nodo *insereR ( struct nodo *raiz, int valor ); //teletar
//struct nodo *removeR ( struct nodo *raiz, int valor, int *feito );
//struct nodo *removeBalanceia ( struct nodo *raiz, int direcao, int *feito );
int insere ( struct arvore *tree, int valor );
int remover ( struct arvore *tree, int valor );


int checaVermelho ( struct nodo *raiz ){
    return raiz != NULL && raiz->cor == RED;
}

struct nodo *singular ( struct nodo *raiz, int direcao ){
    
    struct nodo *guarda = raiz->nodos[!direcao];
    
    raiz->nodos[!direcao] = guarda->nodos[direcao];
    guarda->nodos[direcao] = raiz;
    
    raiz->cor = RED;
    guarda->cor = BLACK;
    
    return guarda;
}

struct nodo *duplo ( struct nodo *raiz, int direcao ){
    raiz->nodos[!direcao] = singular ( raiz->nodos[!direcao], !direcao );
    return singular ( raiz, direcao );
}

int verificaPropriedades ( struct nodo *raiz ){
    int alturaEsquerda, alturaDireita;
    if ( raiz == NULL )
        return 1;
    else{
        printf("\nVerificando nodo: %d", raiz->valor);
        
        struct nodo *nodoEsquerda = raiz->nodos[ESQUERDA];
        struct nodo *nodoDireita = raiz->nodos[DIREITA];
        
        /* Cor consecutiva nos nodos */
        if ( checaVermelho ( raiz ) ) {
            if ( checaVermelho ( nodoEsquerda ) || checaVermelho ( nodoDireita ) ) {
                puts ( "Violacao vermelha" );
                return 0;
            }
        }
        alturaEsquerda = verificaPropriedades ( nodoEsquerda );
        alturaDireita = verificaPropriedades ( nodoDireita );
        
        /* Arvore de busca binaria irregular */
        if ( ( nodoEsquerda != NULL && nodoEsquerda->valor >= raiz->valor )
            || ( nodoDireita != NULL && nodoDireita->valor <= raiz->valor ) )
        {
            puts ( "Violacao de arvore binaria" );
            return 0;
        }
        
        /* Verificando altura negra */
        if ( alturaEsquerda != 0 && alturaDireita != 0 && alturaEsquerda != alturaDireita ) {
            puts ( "Violacao negra" );
            return 0;
        }
        
        /* Conta somente os nodos negros */
        if ( alturaEsquerda != 0 && alturaDireita != 0 )
            return checaVermelho ( raiz ) ? alturaEsquerda : alturaEsquerda + 1;//if else
        else
            return 0;
    }
}

int verificaArvore(struct nodo *raiz){
    
    if (verificaPropriedades(raiz)>0) {
        return 1;
    }else{
        return 0;
    }
    
}



struct nodo *criaNodo ( int valor ){
    struct nodo *novoNodo = (struct nodo*)malloc ( sizeof *novoNodo);
    
    if ( novoNodo != NULL ) {
        novoNodo->valor = valor;
        novoNodo->cor = RED;
        novoNodo->nodos[ESQUERDA] = NULL;
        novoNodo->nodos[DIREITA] = NULL;
    }
    
    return novoNodo;
}

/* TELETAR
struct nodo *insereR ( struct nodo *raiz, int valor )
{
    if ( raiz == NULL )
        raiz = criaNodo ( valor );
    else if ( valor != raiz->valor ) {
        int direcao = raiz->valor < valor;
        
        raiz->nodos[direcao] = insereR ( raiz->nodos[direcao], valor );
        
        if ( checaVermelho ( raiz->nodos[direcao] ) ) {
            if ( checaVermelho ( raiz->nodos[!direcao] ) ) {
                // Case 1
                raiz->cor = RED;
                raiz->nodos[ESQUERDA]->cor = BLACK;
                raiz->nodos[DIREITA]->cor = BLACK;
            }
            else {
                // Cases 2 & 3
                if ( checaVermelho ( raiz->nodos[direcao]->nodos[direcao] ) )
                    raiz = singular ( raiz, !direcao );
                else if ( checaVermelho ( raiz->nodos[direcao]->nodos[!direcao] ) )
                    raiz = duplo ( raiz, !direcao );
            }
        }
    }
    
    return raiz;
}


struct nodo *removeR ( struct nodo *raiz, int valor, int *feito ){
    if ( raiz == NULL )
        *feito = 1;
    else {
        int direcao;
        
        if ( raiz->valor == valor ) {
            if ( raiz->nodos[ESQUERDA] == NULL || raiz->nodos[DIREITA] == NULL ) {
                struct nodo *guarda =
                raiz->nodos[raiz->nodos[ESQUERDA] == NULL];
                
                // Case 0
                if ( checaVermelho ( raiz ) )
                    *feito = 1;
                else if ( checaVermelho ( guarda ) ) {
                    guarda->cor = BLACK;
                    *feito = 1;
                }
                
                free ( raiz );
                
                return guarda;
            }
            else {
                struct nodo *herdeiro = raiz->nodos[ESQUERDA];
                
                while ( herdeiro->nodos[DIREITA] != NULL )
                    herdeiro = herdeiro->nodos[DIREITA];
                
                raiz->valor = herdeiro->valor;
                valor = herdeiro->valor;
            }
        }
        
        direcao = raiz->valor < valor;
        raiz->nodos[direcao] = removeR ( raiz->nodos[direcao], valor, feito );
        
        if ( !*feito )
            raiz = removeBalanceia ( raiz, direcao, feito );
    }
    
    return raiz;
}


struct nodo *removeBalanceia ( struct nodo *raiz, int direcao, int *feito ){
    struct nodo *p = raiz;
    struct nodo *s = raiz->nodos[!direcao];
    
    // Case coruction, remover cor sibling
    if ( checaVermelho ( s ) ) {
        raiz = singular ( raiz, direcao );
        s = p->nodos[!direcao];
    }
    
    if ( s != NULL ) {
        if ( !checaVermelho ( s->nodos[ESQUERDA] ) && !checaVermelho ( s->nodos[DIREITA] ) ) {
            if ( checaVermelho ( p ) )
                *feito = 1;
            p->cor = BLACK;
            s->cor = RED;
        }
        else {
            int guarda = p->cor;
            int new_raiz = ( raiz == p );
            
            if ( checaVermelho ( s->nodos[!direcao] ) )
                p = singular ( p, direcao );
            else
                p = duplo ( p, direcao );
            
            p->cor = guarda;
            p->nodos[ESQUERDA]->cor = BLACK;
            p->nodos[DIREITA]->cor = BLACK;
            
            if ( new_raiz )
                raiz = p;
            else
                raiz->nodos[direcao] = p;
            
            *feito = 1;
        }
    }
    
    return raiz;
}

*/

int insere ( struct arvore *tree, int valor ){
    if ( tree->raiz == NULL ) {
        /* Empty tree case */
        tree->raiz = criaNodo ( valor );
        if ( tree->raiz == NULL )
            return 0;
    }
    else {
        struct nodo head = {0}; /* False tree raiz */
        
        struct nodo *g, *t;     /* Grandparent & parent */
        struct nodo *p, *q;     /* Iterator & parent */
        int direcao = 0, ultimo;
        
        /* Set up helpers */
        t = &head;
        g = p = NULL;
        q = t->nodos[DIREITA] = tree->raiz;
        
        /* Search down the tree */
        for ( ; ; ) {
            if ( q == NULL ) {
                /* Insert new node at the bottom */
                p->nodos[direcao] = q = criaNodo ( valor );
                if ( q == NULL )
                    return 0;
            }
            else if ( checaVermelho ( q->nodos[ESQUERDA] ) && checaVermelho ( q->nodos[DIREITA] ) ) {
                /* Color flip */
                q->cor = RED;
                q->nodos[ESQUERDA]->cor = BLACK;
                q->nodos[DIREITA]->cor = BLACK;
            }
            
            /* Fix cor violation */
            if ( checaVermelho ( q ) && checaVermelho ( p ) ) {
                int direcao2 = t->nodos[DIREITA] == g;
                
                if ( q == p->nodos[ultimo] )
                    t->nodos[direcao2] = singular ( g, !ultimo );
                else
                    t->nodos[direcao2] = duplo ( g, !ultimo );
            }
            
            /* Stop if found */
            if ( q->valor == valor )
                break;
            
            ultimo = direcao;
            direcao = q->valor < valor;
            
            /* Update helpers */
            if ( g != NULL )
                t = g;
            g = p, p = q;
            q = q->nodos[direcao];
        }
        
        /* Update raiz */
        tree->raiz = head.nodos[DIREITA];
    }
    
    /* Make raiz black */
    tree->raiz->cor = BLACK;
    
    return 1;
}


int remover ( struct arvore *tree, int valor ){
    if ( tree->raiz != NULL ) {
        struct nodo head = {0}; /* False tree raiz */
        struct nodo *q, *p, *g; /* Helpers */
        struct nodo *f = NULL;  /* Found item */
        int direcao = 1;
        
        /* Set up helpers */
        q = &head;
        g = p = NULL;
        q->nodos[DIREITA] = tree->raiz;
        
        /* Search and push a cor down */
        while ( q->nodos[direcao] != NULL ) {
            int ultimo = direcao;
            
            /* Update helpers */
            g = p, p = q;
            q = q->nodos[direcao];
            direcao = q->valor < valor;
            
            /* Save found node */
            if ( q->valor == valor )
                f = q;
            
            /* Push the cor node down */
            if ( !checaVermelho ( q ) && !checaVermelho ( q->nodos[direcao] ) ) {
                if ( checaVermelho ( q->nodos[!direcao] ) )
                    p = p->nodos[ultimo] = singular ( q, direcao );
                else if ( !checaVermelho ( q->nodos[!direcao] ) ) {
                    struct nodo *s = p->nodos[!ultimo];
                    
                    if ( s != NULL ) {
                        if ( !checaVermelho ( s->nodos[!ultimo] ) && !checaVermelho ( s->nodos[ultimo] ) ) {
                            /* Color flip */
                            p->cor = BLACK;
                            s->cor = RED;
                            q->cor = RED;
                        }
                        else {
                            int direcao2 = g->nodos[DIREITA] == p;
                            
                            if ( checaVermelho ( s->nodos[ultimo] ) )
                                g->nodos[direcao2] = duplo ( p, ultimo );
                            else if ( checaVermelho ( s->nodos[!ultimo] ) )
                                g->nodos[direcao2] = singular ( p, ultimo );
                            
                            /* Ensure correct coloring */
                            q->cor = g->nodos[direcao2]->cor = RED;
                            g->nodos[direcao2]->nodos[ESQUERDA]->cor = BLACK;
                            g->nodos[direcao2]->nodos[DIREITA]->cor = BLACK;
                        }
                    }
                }
            }
        }
        
        /* Replace and remover if found */
        if ( f != NULL ) {
            f->valor = q->valor;
            p->nodos[p->nodos[DIREITA] == q] =
            q->nodos[q->nodos[ESQUERDA] == NULL];
            free ( q );
        }
        
        /* Update raiz and make it black */
        tree->raiz = head.nodos[DIREITA];
        if ( tree->raiz != NULL )
            tree->raiz->cor = BLACK;
    }
    
    return 1;
}

struct nodo *search_node(int valorBusca, struct nodo *t){
    
	if (t == NULL){
		return NULL;
	}else{
		if(t->valor == valorBusca){
			return t;
		}else{
			struct nodo *found;
			found = search_node(valorBusca,t->nodos[ESQUERDA]);
            if(found == NULL){
                found = search_node(valorBusca,t->nodos[DIREITA]);
            }
        }
    }
    return NULL;
}

void Tree_inOrder(struct nodo *n){
    
    if(n==0)
        
        return;
    
    Tree_inOrder(n->nodos[ESQUERDA]);
    
    printf("%d ", n->valor);
    
    Tree_inOrder(n->nodos[DIREITA]);
    
}


void print_at_level_recursive(struct nodo *n, int desejado, int atual)
{
    
    if (n)
    {
        if (desejado == atual)
            printf("%d ", n->valor);
        else
        {
            print_at_level_recursive(n->nodos[ESQUERDA], desejado, atual + 1);
            print_at_level_recursive(n->nodos[DIREITA], desejado, atual + 1);
        }
    }
}


void menu(){
    
    printf("\n1 - Inserir");
    printf("\n2 - Remover");
    printf("\n3 - Listar elementos em orderm");
    printf("\n4 - Listar elementos de um nivel");
    printf("\n5 - Procurar elementos");
    printf("\n6 - Imprimir arvore, dinstinguir estrutura");
    printf("\n7 - Verificar propriedades preto-vermelho");
    printf("\n8 - Sair");
    
    printf("\nDigite opção: ");
    
}

int verificaNodo(struct nodo *n){
    if (n) {
        return 1;
    }else{
        return 0;
    }
}

void getStringLevel(struct nodo *pressNode, struct nodo *n, int desired, int current)
{
    
    
    if (n)
    {
        if (desired == current){
            struct nodo *aNode = NULL;
            
            if (pressNode) {
                aNode = pressNode;
                
                while (aNode->nodos[ESQUERDA]) {
                    aNode = aNode->nodos[ESQUERDA];
                }
                struct nodo *valueNode = criaNodo(n->valor);
                aNode->nodos[ESQUERDA] = valueNode;
                
            }else{
                pressNode = criaNodo(n->valor);
            }
            
        }else{
            
            getStringLevel(pressNode, n->nodos[ESQUERDA], desired, current + 1);
            getStringLevel(pressNode, n->nodos[DIREITA], desired, current + 1);
        
        }
    }
    
    
    
}

void printLevelTree(struct nodo *root){
    struct nodo *printnode = criaNodo(0);
    getStringLevel(printnode, root, 0, 0);
    printnode = printnode->nodos[ESQUERDA];
    char line[30] = "";
    
    while (printnode) {
        //printf("-> %d ", printnode->data);
        char data[80];
        if (printnode->valor) {
            sprintf(data, " %d", printnode->valor);
            //    puts(str);
            strcat(line, data);
        }
        printnode = printnode->nodos[ESQUERDA];
        
        
    }
    puts(line);
    
}

void center_print(const char *s, int width)
{
    int length = strlen(s);
    int i;
    for (i=0; i<=(width-length)/2; i++) {
        fputs(" ", stdout);
    }
    fputs(s, stdout);
    i += length;
    for (; i<=width; i++) {
        fputs(" ", stdout);
    }
}
int main(int argc, const char * argv[])
{
    
    
    struct arvore *raiz = malloc(sizeof(Nodo));
    
 //   int numero = 2;
    
    
    int valor = 0;
    
    int opcao = 0;
    while (opcao!=8) {
        
        menu();
        scanf("%d", &opcao);
        
        
        if (opcao!=8) {
            switch (opcao) {
                case 1:
                    
                    printf("\n Valor a ser inserido: ");
                    scanf("%d", &valor);
                    insere(raiz, valor);
                    
                    
                    break;
                case 2:
                    printf("\n Valor a ser removido: ");
                    scanf("%d", &valor);
                    
                    remover ( raiz, valor );
                    break;
                case 3:
                    Tree_inOrder(raiz->raiz);
                    break;
                case 4:
                    printf("\n Nivel a ser impresso: ");
                    scanf("%d", &valor);
                    print_at_level_recursive(raiz->raiz, valor, 0);
                    break;
                case 5:
                    printf("\n Valor à ser procurado: ");
                    scanf("%d", &valor);
                    
                    if(verificaNodo(search_node(valor, raiz->raiz))){
                        printf("TRUE");
                    }else{
                        printf("FALSE");
                    }
                    
                    break;
                case 6:
                    if(verificaArvore(raiz->raiz)){
                        printf("TRUE");
                    }else{
                        printf("FALSE");
                    }
                    break;
                case 7:
                    printf("\n Largura ");
                    scanf("%d", &valor);
                    center_print("mouro", valor);
                    break;
                default:
                    break;
            }
        }
        
    }
    
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}


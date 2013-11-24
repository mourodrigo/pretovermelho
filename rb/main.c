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

/*
#include <stdlib.h>
#include <string.h>
#include <fstream.h>
#include <ostream>
#include <vector>
#include <sstream>
#include <utility>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#ifdef WIN32 //|| _WIN32
#include <direct.h>
#define MKDIR(a) _mkdir(a)
#else
#include <sys/stat.h>
#define MKDIR(a) mkdir(a, 0777)  Aqui o 0777 define o modo como igual ao umask, ou seja as permissões que resultariam de um mkdir na shell
#endif
*/

#define PRETO 0
#define VERMELHO 1
#define ESQUERDA 0
#define DIREITA 1
#define NIL -0

struct nodo;
struct arvore;

struct nodo {
    int cor;
    int valor;
    struct nodo *nodos[2];
}Nodo;

struct nodoprint {
    char *valor;
    struct nodo *nodos[1];
}NodoPrint;

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
    if (raiz != NULL && raiz->cor == VERMELHO) {
        return 1;
    }else{
        return 0;
    }
 //   return raiz != NULL && raiz->cor == VERMELHO; //teletar
}

struct nodo *singular ( struct nodo *raiz, int direcao ){
    
    struct nodo *guarda = raiz->nodos[!direcao];
    
    raiz->nodos[!direcao] = guarda->nodos[direcao];
    guarda->nodos[direcao] = raiz;
    
    raiz->cor = VERMELHO;
    guarda->cor = PRETO;
    
    return guarda;
}

struct nodo *duplo ( struct nodo *raiz, int direcao ){
    raiz->nodos[!direcao] = singular ( raiz->nodos[!direcao], !direcao );
    return singular ( raiz, direcao );
}

int verificaPropriedades ( struct nodo *raiz ){
    int alturaEsquerda, alturaDireita;
    if ( raiz == NULL ){
        return 1;
    }else{
        printf("\nVerificando nodo: %d", raiz->valor);
        
        struct nodo *nodoEsquerda = raiz->nodos[ESQUERDA];
        struct nodo *nodoDireita = raiz->nodos[DIREITA];
        
        /* Cor consecutiva nos nodos */
        if ( checaVermelho ( raiz ) ) {
            if ( checaVermelho ( nodoEsquerda ) || checaVermelho ( nodoDireita ) ) {
                printf ( "Violacao vermelha" );
                return 0;
            }
        }
        alturaEsquerda = verificaPropriedades ( nodoEsquerda );
        alturaDireita = verificaPropriedades ( nodoDireita );
        
        /* Arvore de busca binaria irregular */
        if ( ( nodoEsquerda != NULL && nodoEsquerda->valor >= raiz->valor )
            || ( nodoDireita != NULL && nodoDireita->valor <= raiz->valor ) ){
            printf ( "Violacao de arvore binaria" );
            return 0;
        }
        
        /* Verificando altura negra */
        if ( alturaEsquerda != 0 && alturaDireita != 0 && alturaEsquerda != alturaDireita ) {
            printf ( "Violacao negra" );
            return 0;
        }
        
        /* Conta somente os nodos negros */
        if ( alturaEsquerda != 0 && alturaDireita != 0 ){
            return checaVermelho ( raiz ) ? alturaEsquerda : alturaEsquerda + 1;
        }else{
            return 0;
        }
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
        novoNodo->cor = VERMELHO;
        novoNodo->nodos[ESQUERDA] = NULL;
        novoNodo->nodos[DIREITA] = NULL;
    }
    
    return novoNodo;
}

struct nodoprint *criaNodoPrint ( char *valor ){

    struct nodoprint *novoNodo = (struct nodoprint*)malloc ( sizeof *novoNodo);
    
    if ( novoNodo != NULL ) {
        
        
        novoNodo->valor = valor;
        
        novoNodo->nodos[ESQUERDA] = NULL;
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
                raiz->cor = VERMELHO;
                raiz->nodos[ESQUERDA]->cor = PRETO;
                raiz->nodos[DIREITA]->cor = PRETO;
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
                    guarda->cor = PRETO;
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
            p->cor = PRETO;
            s->cor = VERMELHO;
        }
        else {
            int guarda = p->cor;
            int new_raiz = ( raiz == p );
            
            if ( checaVermelho ( s->nodos[!direcao] ) )
                p = singular ( p, direcao );
            else
                p = duplo ( p, direcao );
            
            p->cor = guarda;
            p->nodos[ESQUERDA]->cor = PRETO;
            p->nodos[DIREITA]->cor = PRETO;
            
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
        struct nodo sentinela = {0};
        
        struct nodo *g, *t;     // Nodo avo e pai
        struct nodo *p, *q;     // Nodo incremental e pai
        int direcao = 0, ultimo;
        
        // Definição dos notos auxiliares
        t = &sentinela;
        g = p = NULL;
        q = t->nodos[DIREITA] = tree->raiz;
        
        // Busca na árvore até ocorrer um break
        for ( ; ; ) {
            if ( q == NULL ) {
                // Insere novo nodo no final
                p->nodos[direcao] = q = criaNodo ( valor );
                if ( q == NULL )
                    return 0;
            }else if ( checaVermelho ( q->nodos[ESQUERDA] ) && checaVermelho ( q->nodos[DIREITA] ) ) {
                // Inverte cor
                q->cor = VERMELHO;
                q->nodos[ESQUERDA]->cor = PRETO;
                q->nodos[DIREITA]->cor = PRETO;
            }
            
            // Fix-up de cores
            if ( checaVermelho ( q ) && checaVermelho ( p ) ) {
                int direcao2 = t->nodos[DIREITA] == g;
                
                if ( q == p->nodos[ultimo] )
                    t->nodos[direcao2] = singular ( g, !ultimo );
                else
                    t->nodos[direcao2] = duplo ( g, !ultimo );
            }
            
            // Stop if found
            if ( q->valor == valor )
                break;
            
            ultimo = direcao;
            direcao = q->valor < valor;
            
            // Update helpers
            if ( g != NULL )
                t = g;
            g = p, p = q;
            q = q->nodos[direcao];
        }
        
        // Update raiz
        tree->raiz = sentinela.nodos[DIREITA];
    }
    
    // Make raiz PRETO
    tree->raiz->cor = PRETO;
    
    return 1;
}


int remover ( struct arvore *tree, int valor ){
    if ( tree->raiz != NULL ) {
        struct nodo sentinela = {0};
        struct nodo *q, *p, *g; /* Nodos auxiliares */
        struct nodo *f = NULL;  /* Nodo à ser encontrado */
        int direcao = 1;
        
        /* Definição dos nodos auxiliares */
        q = &sentinela;
        g = p = NULL;
        q->nodos[DIREITA] = tree->raiz;
        
        /* Localiza e  */
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
                            p->cor = PRETO;
                            s->cor = VERMELHO;
                            q->cor = VERMELHO;
                        }else {
                            int direcao2 = g->nodos[DIREITA] == p;
                            
                            if ( checaVermelho ( s->nodos[ultimo] ) )
                                g->nodos[direcao2] = duplo ( p, ultimo );
                            else if ( checaVermelho ( s->nodos[!ultimo] ) )
                                g->nodos[direcao2] = singular ( p, ultimo );
                            
                            /* Ensure correct coloring */
                            q->cor = g->nodos[direcao2]->cor = VERMELHO;
                            g->nodos[direcao2]->nodos[ESQUERDA]->cor = PRETO;
                            g->nodos[direcao2]->nodos[DIREITA]->cor = PRETO;
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
        
        /* Update raiz and make it PRETO */
        tree->raiz = sentinela.nodos[DIREITA];
        if ( tree->raiz != NULL )
            tree->raiz->cor = PRETO;
    }
    
    return 1;
}

struct nodo *search_node(int valorBusca, struct nodo *t){
    
	if (t == NULL){
		return NULL;
	}else{
        struct nodo *found;
        
        if(t->valor == valorBusca){
			return t;
		}else{
		//	struct nodo *found;
			found = search_node(valorBusca,t->nodos[ESQUERDA]);
            if(found == NULL){
                found = search_node(valorBusca,t->nodos[DIREITA]);
            }
        }
        return found;
    }
    return NULL;
}

void Tree_inOrder(struct nodo *n, FILE *outputFile){

    if(n==0){
        return;
    }
    Tree_inOrder(n->nodos[ESQUERDA], outputFile);
    if (outputFile) {
        char puts[30] = "";
        strtol(puts, (char **)NULL, n->valor);
 //       printf("puts[%s][%d]", puts, n->valor);
        sprintf(puts, "%d ",n->valor);
        fputs(puts, outputFile);
        printf(" %s", puts);
    }else{
        printf("%d ", n->valor);
    }
    
    Tree_inOrder(n->nodos[DIREITA], outputFile);
    
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
    
    
    //if (n)
    //{
        if (desired == current){
            
            struct nodo *aNode = NULL;
                
            if (pressNode) {
                aNode = pressNode;
                
                while (aNode->nodos[ESQUERDA]) {
                    aNode = aNode->nodos[ESQUERDA];
                }
                if (n) {
                    struct nodo *valueNode = criaNodo(n->valor);
                    valueNode->cor = n->cor;
                    aNode->nodos[ESQUERDA] = valueNode;
                    
                }else{
                    struct nodo *valueNode = criaNodo(NIL);
                    valueNode->cor = 0;
                    aNode->nodos[ESQUERDA] = valueNode;

                }
                
            }else{
                if (n) {
                    pressNode = criaNodo(n->valor);
                    pressNode->cor = n->cor;
                }else{
                    pressNode = criaNodo(NIL);
                    pressNode->cor = n->cor;
                    
                    
                }
                
            }

        }else{
            if (n->nodos[ESQUERDA]) {
                getStringLevel(pressNode, n->nodos[ESQUERDA], desired, current + 1);
            }else{
                pressNode->nodos[ESQUERDA] = criaNodo(NIL);
            }
            if (n->nodos[DIREITA]) {
                getStringLevel(pressNode, n->nodos[DIREITA], desired, current + 1);
            }else{
                pressNode->nodos[ESQUERDA] = criaNodo(NIL);
            }
        
        }
    //}
    
    
    
}


void center_print(const char *s, int width)
{
    int length = (int)strlen(s);
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

int alturaArvore(struct nodo *root) {
    
    if (!root){
        return 0;
    }else if(root->valor){
        int esquerda = 0;
        if (root->nodos[ESQUERDA]) {
            esquerda = alturaArvore(root->nodos[ESQUERDA]);
        }
        int direita = 0;
        if (root->nodos[DIREITA]) {
            direita = alturaArvore(root->nodos[DIREITA]);
        }
        return esquerda+direita+1;
    }
    return 0;
}

void printLevelTree(struct nodo *root){
    int x = 0;
    
    while (1) {
        
    
    //for (int x = 0; x<alturaArvore(root); x++) {
        struct nodo *printnode = criaNodo(0);
        
        getStringLevel(printnode, root, x, 0);
        if (!printnode->nodos[ESQUERDA]) {
            break;
        }
        printnode = printnode->nodos[ESQUERDA];
        char line[600] = "";
        //char *lines = "500";
        
        // struct nodoprint *imprime = criaNodoPrint(lines);
        //printf("%s", imprime->valor);
        
        while (printnode) {
            //printf("-> %d ", printnode->data);
            char data[80];
            if (printnode->valor == NIL) {
                
                strcat(line, " (nil)");
                
            }else{
                //for (int z = 0; z<=x; z++) {
                    strcat(line, "  ");
                //}
                if (printnode->cor) {
                    sprintf(data, "(%d)", printnode->valor);
                }else{
                    sprintf(data, "[%d]", printnode->valor);
                }
                //    puts(str);
                
                strcat(line, data);
                
            }
            
            printnode = printnode->nodos[ESQUERDA];
            
            
        }

        
        char nil[600] = "(nil)/";
        if (strlen(line) == strlen(nil)) {
            break;
        }
        printf("\n");
        center_print(line, 60);
        x++;
    }
 
    
}

void modoInterface(struct arvore *raiz){
    
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
                    Tree_inOrder(raiz->raiz, NULL);
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
                    
                    printLevelTree(raiz->raiz);
                    
                    
                    break;
                case 7:
                    
                    if(verificaArvore(raiz->raiz)){
                        printf("TRUE");
                    }else{
                        printf("FALSE");
                    }
                    
                    /*   printf("\n Largura ");
                     scanf("%d", &valor);
                     center_print("mouro", valor);
                     
                     */
                    break;
                default:
                    break;
            }
        }
        
    }
    

}

static char *fgetline(FILE *f)
{
    size_t len;
    char *line= fgetln(f, &len);
    if (line == 0) return 0;
    char *buf= malloc(len+1);
    if (buf == 0) return 0;
    memcpy(buf, line, len);
    buf[len]= '\0';
    return buf;
}


int main(int argc, const char * argv[])
{
    int debug = 1;
    
    struct arvore *raiz = malloc(sizeof(Nodo));

    
    if ( argc != 2 ) //numero de argumentos deve ser 2 para correta execução com nome do arquivo
    {
        /* argv[0] é o nome da aplicação sendo executada */
        printf( "Não foram passados argumentos, executando modo interface. \n\nPara passar argumentos execute: %s nomeDoArquivo\n\n", argv[0] );
        int larguraDisplay = 80;
        center_print("=================================", larguraDisplay);
        printf("\n");
        center_print("ÁRVORE RUBRO-NEGRA", larguraDisplay);
        printf("\n");
        center_print("=================================", larguraDisplay);
        printf("\n");
        modoInterface(raiz);
        
        
    }
    else
    {
    
        // argv[1] deve ser o nome do arquivo à ser aberto
        FILE *input = fopen( argv[1], "r" );
        if (debug) printf("Abrindo arquivo -> %s ", argv[1]);
        /* fopen retorna 0 se houve algum erro. */
        if ( input == 0 )
        {
            if (debug) printf("\n\nNão foi possível abrir o arquivo, executando modo interface.\n\n");
            modoInterface(raiz);
        }
        else
        {
            char *x;
            char *path = (char*)argv[1];
            FILE *output = fopen(strcat(strtok(path, "."), ".expected"), "a" );
            if (debug) printf("\n\noutput: %s \n\n",strcat(strtok(path, "."), ".output"));
            
            if (input) {
                int inputline = 0;
                int outputline = 0;
                while  ( ( x = fgetline(input) ))
                {
                    inputline++;
                    char *funcao, *valor;
                    char *explode = " ";
                    funcao = strtok(x, explode);
                    valor = strtok(NULL, explode);
                    
                    
                    if (inputline==994) {
                        
                    }
                    
                    if (strcmp(funcao, "insert")==0) {
                        if (valor) {
                            if (debug) printf("line %d || insert %d \n",inputline, atoi(valor));
                            insere(raiz, atoi(valor));
                        }else{
                            if (debug) printf("valor invalido");
                        }
                    
                    
                    
                    
                    }
                    
                    
                    else if (strcmp(funcao, "search")==0) {
                        if (valor) {
                            outputline++;
                            if (debug) printf("line %d || search %d ||",inputline, atoi(valor));
                            if(verificaNodo(search_node(atoi(valor), raiz->raiz))){
                                if (debug) printf(" outputline %d || TRUE", outputline);
                                fputs("true \n", output);
                                
                            }else{
                                fputs("false \n", output);

                                if (debug) printf(" outputline %d || FALSE ", outputline);
                            }
                            if (debug) printf("\n");
                        }else{
                            if (debug) printf("valor invalido");
                        }
                    
                    
                    
                    
                    }
                    
                    
                    
                    else if (strcmp(funcao, "delete")==0) {
                        if (valor) {
                            if (debug) printf("line %d || delete %d \n",inputline, atoi(valor));
                            
                            remover(raiz, atoi(valor));
                        }else{
                            if (debug) printf("valor invalido");
                        }
                    
                    
                    
                    }
                    
                    
                    
                    
                    else if (strcmp(x, "inorder\n")==0) {
                        outputline++;
                        if (debug) printf("line %d || inorder", inputline);
                        Tree_inOrder(raiz->raiz, output);
                        fputs("\n", output);
                        printf(" || outputline %d", outputline);
                        if (debug) printf("\n");
                    
                    
                    
                    }
                    printf("\n");
                }
                fclose(input);
                fclose(output);
            }
        }
    
        if (debug) printf("\n\n-----------------\n");
        if (debug) printf("\n\n-----------------\n");
        if (debug) printf("\n\nLeitura ok, enter para modo interface\n");
        
        
        getchar();
        getchar();
        
        modoInterface(raiz);
    
    }
    
    
    
    
    
    /*
    insere(raiz, 4321);
    insere(raiz, 212);
    insere(raiz, 3432);
    insere(raiz, 412);
    insere(raiz, 5435);
    insere(raiz, 636);
    insere(raiz, 7654);
    insere(raiz, 812);
    insere(raiz, 923);
    insere(raiz, 1230);
    insere(raiz, 135);
    insere(raiz, 116);
    insere(raiz, 172);
    insere(raiz, 18);
    insere(raiz, 19);
    insere(raiz, 21);
    insere(raiz, 33);
    insere(raiz, 22);
    insere(raiz, 33);
    insere(raiz, 44);
    insere(raiz, 55);
    insere(raiz, 66);
    insere(raiz, 77);
    insere(raiz, 88);
    insere(raiz, 99);
    insere(raiz, 98);
    insere(raiz, 87);
    insere(raiz, 76);
    insere(raiz, 65);
    insere(raiz, 43);
    */
 //   int numero = 2;
    
        // insert code here...
    
    return 0;
}


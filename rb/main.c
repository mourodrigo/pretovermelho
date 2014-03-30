//
//  main.c
//  rb
//
//  Created by Rodrigo Bueno Tomiosso & Andrey Baumhardt Ramos on 17/09/13.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define PRETO 0
#define VERMELHO 1
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
struct nodo *trocaSingular ( struct nodo *raiz, int direcao );
struct nodo *trocaDupla ( struct nodo *raiz, int direcao );
int verificaPropriedades ( struct nodo *raiz );
struct nodo *criaNodo ( int valor );
int verificaArvore(struct nodo *raiz);
int insere ( struct arvore *arvre, int valor );
int remover ( struct arvore *arvre, int valor );

int checaVermelho ( struct nodo *raiz ){
    //Se o nodo recebido n„o for NULL e a cor for VERMELHO retorna 1, caso contr·rio retorna 0
    if (raiz != NULL && raiz->cor == VERMELHO){
        return 1;
    }else{
        return 0;
    }
}

struct nodo *trocaSingular ( struct nodo *raiz, int direcao ){
    //Caso receba 0, a vari·vel guarda recebe o endereÁo de memÛria do filho da DIREITA do nodo
    //Se receber 1 no par‚metro direÁ„o ent„o guarda recebe o filho da ESQUERDA do nodo
    struct nodo *guarda = raiz->nodos[!direcao];
    //Caso direÁ„o seja 0: O nodo da DIREITA aponta para o endereÁo de memÛria do seu filho da ESQUERDA
    //Caso direÁ„o seja 1: O nodo da ESQUERDA aponta para o endereÁo de memÛria do seu filho da DIREITA
    raiz->nodos[!direcao] = guarda->nodos[direcao];
    //Caso direÁ„o seja 0: O nodo da ESQUERDA do nodo recebido aponta para o endereÁo de memÛria do prÛprio pai
    //Caso direÁ„o seja 1: O nodo da DIREITA do nodo recebido aponta para o endereÁo de memÛria do prÛprio pai
    guarda->nodos[direcao] = raiz;
    
    //Troca a cor do NODO recebido para VERMELHO e a cor do filho salvo em guarda para PRETO
    raiz->cor = VERMELHO;
    guarda->cor = PRETO;
    
    return guarda;
}

struct nodo *trocaDupla ( struct nodo *raiz, int direcao ){
	//S„o duas trocas singulares, funciona como a troca singular.
	// O nodo oposto a direÁ„o recebida recebe a primeira troca e a funÁ„o retorna a segudna troca
    raiz->nodos[!direcao] = trocaSingular ( raiz->nodos[!direcao], !direcao );
    return trocaSingular ( raiz, direcao );
}

int verificaPropriedades ( struct nodo *raiz ){
    int alturaEsquerda, alturaDireita;
    if( raiz == NULL ){
        return 1;
    }else{
        printf("\nVerificando propriedade do nodo: %d", raiz->valor);
        
        struct nodo *nodoEsquerda = raiz->nodos[ESQUERDA];
        struct nodo *nodoDireita = raiz->nodos[DIREITA];
        
        //Checa se o pai e os dois filhos s„o vermelhos
        if ( checaVermelho ( raiz ) ) {
            if ( checaVermelho ( nodoEsquerda ) || checaVermelho ( nodoDireita ) ) {
                printf ( "Violacao vermelha" );
                return 0;
            }
        }
        alturaEsquerda = verificaPropriedades ( nodoEsquerda );
        alturaDireita = verificaPropriedades ( nodoDireita );
        
        //Checa se o valor do filho da esquerda È menor que o do pai
        //e se o valor do filho da direita È maior que o pai
        if ( ( nodoEsquerda != NULL && nodoEsquerda->valor >= raiz->valor )
            || ( nodoDireita != NULL && nodoDireita->valor <= raiz->valor ) ){
            printf ( "Violacao de arvore binaria" );
            return 0;
        }
        
        //Se a altura a direita e a esquerda foram diferente de ZERO e DIFERENTES de si mesmas
        //a altura negra È violada
        if ( alturaEsquerda != 0 && alturaDireita != 0 && alturaEsquerda != alturaDireita ) {
            printf ( "Violacao negra" );
            return 0;
        }
        
        //If que conta os nodos negros se ambas alturas forem diferentes de ZERO
        if ( alturaEsquerda != 0 && alturaDireita != 0 ){
			//Se checaVermelho(raiz) retorna 1 ent„o o nodo È vermelho e n„o conta para a altura negra
			//logo o return ir· retornar o prÛprio valor da alturaEsquerda
			//Se o checaVermelho(raiz) retorna 0 ent„o o nodo È preto e logo dever· ser adicionado
			//na contagem da altura preta, ent„o returna ir· retornar alturaEsquerda + 1
			//alturaEsquerda tem o mesmo valor de alturaDireita neste ponto da funÁ„o
			//tendo em vista que se forem diferentes ocorre uma violaÁ„o negra
            return checaVermelho ( raiz ) ? alturaEsquerda : alturaEsquerda + 1;
        }else{
            return 0;
        }
    }
}

int verificaArvore(struct nodo *raiz){
	//FunÁ„o que verifica a ·vore aplicando o verificaPropriedades
	//Se verificaPropriedades retornar algo maior que 0, ser· o valor da altura negra
	//e a ·rvore consequentemente est· OK
    if (verificaPropriedades(raiz)>0){
        return 1;
    }else{
        return 0;
    }
}

struct nodo *criaNodo ( int valor ){
	//FunÁ„o que cria um novo nodo com o valor recebido
    struct nodo *novoNodo = (struct nodo*)malloc ( sizeof *novoNodo);
    if( novoNodo != NULL ){
        novoNodo->valor = valor;
        novoNodo->cor = VERMELHO;
        novoNodo->nodos[ESQUERDA] = NULL;
        novoNodo->nodos[DIREITA] = NULL;
    }
    return novoNodo;
}

int insere ( struct arvore *arvre, int valor ){
    if ( arvre->raiz == NULL ){
        //Inserindo em uma ·vore vazia
        arvre->raiz = criaNodo ( valor );
        if ( arvre->raiz == NULL )
            return 0;//Se depois de inserir continuar nulo retorna zero
    }else{
        struct nodo sentinela = {0};//Cria ·rea de memÛria para o sentinela
        struct nodo *g, *t;    // Nodo avo e pai
        struct nodo *p, *q;    // Nodo incremental e pai
        int direcao = 0, ultimo;
        
        //DefiniÁ„o dos nodos auxiliares
        t = &sentinela;//Faz T receber o endereÁo de memÛria do sentinela
        g = p = NULL;
        q = t->nodos[DIREITA] = arvre->raiz;//Faz o nodo da direita do sentinela apontar para a raiz da ·rvore junto com o q
        
        //Busca na ·rvore atÈ ocorrer um break
        for ( ; ; ) {
            if(q == NULL){//Na primeira vez n„o cai aqui
                // Insere novo nodo no final
                p->nodos[direcao] = q = criaNodo ( valor );
                if ( q == NULL )//Se continuar 0 depois de inserir n„o deu certo
                    return 0;
            }else if( checaVermelho ( q->nodos[ESQUERDA] ) && checaVermelho ( q->nodos[DIREITA] ) ){
                //Se Q n„o for nulo e seus dois filhos forem vermelhos, ent„o eles viram pretos e Q fica vermelho
                //Inverte cor
                q->cor = VERMELHO;
                q->nodos[ESQUERDA]->cor = PRETO;
                q->nodos[DIREITA]->cor = PRETO;
            }
            // Fix-up de cores
            if ( checaVermelho ( q ) && checaVermelho ( p ) ) {
                int direcao2 = t->nodos[DIREITA] == g;
                if ( q == p->nodos[ultimo] )
                    t->nodos[direcao2] = trocaSingular ( g, !ultimo );
                else
                    t->nodos[direcao2] = trocaDupla ( g, !ultimo );
            }
            //Se o valor j· existir na ·vore sai do laÁo
            if ( q->valor == valor )
                break;
            
            ultimo = direcao;//Salva a ˙ltima direÁ„o
            //Se o valor atual do nodo (q->valor) for menor que o valor a ser inserido
            //direÁ„o recebe 1 e a funÁ„o segue para a direita, se q->valor for maior que o valor atual
            //ent„o direÁ„o recebe 0 e a funÁ„o segue para a esquerda
            direcao = q->valor < valor;
            
            
            //Se G for diferente de null t recebe ele
            if ( g != NULL )
                t = g;
            //Faz o g apontar para o p
            //Faz p apontar para o q (atual nodo)
            //Faz q receber o nodo da direÁ„o que dever· ir
            g = p, p = q;
            q = q->nodos[direcao];
        }
        //Atualiza a raiz
        arvre->raiz = sentinela.nodos[DIREITA];
    }
    //Muda a cor da raiz pra PRETO
    arvre->raiz->cor = PRETO;
    return 1;
}

int remover ( struct arvore *arvre, int valor ){
    if ( arvre->raiz != NULL ) {
        struct nodo sentinela = {0};
        struct nodo *q, *p, *g; // Nodos auxiliares
        struct nodo *f = NULL;  // Nodo ‡ ser encontrado
        int direcao = 1;
        
        //DefiniÁ„o dos nodos auxiliares
        q = &sentinela;
        g = p = NULL;
        q->nodos[DIREITA] = arvre->raiz;
        
        //Localiza o nodo
        while ( q->nodos[direcao] != NULL ) {
            int ultimo = direcao;//Salva a ˙ltima direÁ„o
            
            //Atualiza auxiliares
            g = p, p = q;
            q = q->nodos[direcao];
            direcao = q->valor < valor;
            
            //Salva o nodo encontrado
            if ( q->valor == valor )
                f = q;
            
            //Joga a cor do nodo para baixo
            if ( !checaVermelho ( q ) && !checaVermelho ( q->nodos[direcao] ) ){//Se eu for e meu filho fomos pretos
                if ( checaVermelho ( q->nodos[!direcao] ) )//Se o filho oposto do anterior for vermelho
                    p = p->nodos[ultimo] = trocaSingular ( q, direcao );//Faz a troca
                else if ( !checaVermelho ( q->nodos[!direcao] ) ) {//Se o filho posto do anterior for preto
                    struct nodo *s = p->nodos[!ultimo];//faz S apontar para P que nada mais È que o antigo valor de Q
                    if ( s != NULL ) {//Se s N√O for NULL
                        if ( !checaVermelho ( s->nodos[!ultimo] ) && !checaVermelho ( s->nodos[ultimo] ) ){
                            //Se o nodo a esquerda e a direita de S forem pretos ent„o efetua a troca
                            //Faz a troca de cores
                            p->cor = PRETO;
                            s->cor = VERMELHO;
                            q->cor = VERMELHO;
                        }else {
                            int direcao2 = g->nodos[DIREITA] == p;
                            
                            if ( checaVermelho ( s->nodos[ultimo] ) )//Se este nodo for vermelho ent„o efetua troca dupla
                                g->nodos[direcao2] = trocaDupla ( p, ultimo );
                            else if ( checaVermelho ( s->nodos[!ultimo] ) )//Se este nodo for vermelho efetua troca simples
                                g->nodos[direcao2] = trocaSingular ( p, ultimo );
                            
                            //Garante a coloraÁ„o dos nodos
                            q->cor = g->nodos[direcao2]->cor = VERMELHO;
                            g->nodos[direcao2]->nodos[ESQUERDA]->cor = PRETO;
                            g->nodos[direcao2]->nodos[DIREITA]->cor = PRETO;
                        }
                    }
                }
            }
        }
        
        //Substitui e remove se encontrou
        if ( f != NULL ) {
            f->valor = q->valor;
            p->nodos[p->nodos[DIREITA] == q] =
            q->nodos[q->nodos[ESQUERDA] == NULL];
            free ( q );
        }
        
        //Faz a raiz PRETA
        arvre->raiz = sentinela.nodos[DIREITA];
        if ( arvre->raiz != NULL )
            arvre->raiz->cor = PRETO;
    }
    return 1;
}

struct nodo *procuraNodo(int valorBusca, struct nodo *t){
    //Se a ·vore for NULL
	if (t == NULL){
		return NULL;
	}else{
        struct nodo *found;//Ponteiro para nodo encontrado
        
        if(t->valor == valorBusca){//Se o valor do nodo atual for o desejado retorna o nodo atual
			return t;
		}else{
            //Entra aqui caso n„o encontrou o valor procurado
			found = procuraNodo(valorBusca,t->nodos[ESQUERDA]);//Procura para a esquerda
            if(found == NULL){//Caso n„o ache na esquerda
                found = procuraNodo(valorBusca,t->nodos[DIREITA]);//Procura para a direita
            }
        }
        return found;
    }
    return NULL;
}

void emOrdem(struct nodo *n, FILE *outputFile){
    
    if(n==0){
        return;
    }
    emOrdem(n->nodos[ESQUERDA], outputFile);
    if (outputFile) {
        char puts[30] = "";
        strtol(puts, (char **)NULL, n->valor);
        //       printf("puts[%s][%d]", puts, n->valor);
        
        sprintf(puts, "%d ",n->valor);
        fputs(puts, outputFile);
        // printf(" %s", puts);
    }else{
        printf("%d ", n->valor);
    }
    emOrdem(n->nodos[DIREITA], outputFile);
}

void imprimeRecursivo(struct nodo *n, int desejado, int atual){
    //Imprime os valores de um sÛ nÌvel
    if (n){
        if (desejado == atual)
            printf("%d ", n->valor);
        else{
            imprimeRecursivo(n->nodos[ESQUERDA], desejado, atual + 1);
            imprimeRecursivo(n->nodos[DIREITA], desejado, atual + 1);
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
    printf("\nDigite opÁ„o: ");
    
}

int verificaNodo(struct nodo *n){
    //FunÁ„o que verifica se um novo È v·lido ou n„o
    if (n) {
        return 1;
    }else{
        return 0;
    }
}

void criaListaNivel(struct nodo *pressNode, struct nodo *n, int desejado, int atual){
    //Retorna somente o nÌvel da ·vore desejado atravÈs de uma lista encadeada com os nodos
    //Coloca os dados encontrados dentro do nodo pressNode que funciona como uma lista encadeada
    if (desejado == atual){
        struct nodo *aNode = NULL;
        
        if (pressNode) {
            aNode = pressNode;
            
            while (aNode->nodos[ESQUERDA]){
                aNode = aNode->nodos[ESQUERDA];
            }
            if (n){
                struct nodo *valueNode = criaNodo(n->valor);
                valueNode->cor = n->cor;
                aNode->nodos[ESQUERDA] = valueNode;
                
            }else{
                struct nodo *valueNode = criaNodo(INT_MAX);
                valueNode->cor = 0;
                aNode->nodos[ESQUERDA] = valueNode;
                
            }
            
        }else{
            if(n){
                pressNode = criaNodo(n->valor);
                pressNode->cor = n->cor;
            }else{
                pressNode = criaNodo(INT_MAX);
                pressNode->cor = n->cor;
            }
        }
    }else{//Vai percorrendo atÈ achar o nÌvel desejado
        if (n->nodos[ESQUERDA]){
            criaListaNivel(pressNode, n->nodos[ESQUERDA], desejado, atual + 1);
        }else{
            pressNode->nodos[ESQUERDA] = criaNodo(INT_MAX);
        }
        if (n->nodos[DIREITA]){
            criaListaNivel(pressNode, n->nodos[DIREITA], desejado, atual + 1);
        }else{
            pressNode->nodos[ESQUERDA] = criaNodo(INT_MAX);
        }
        
    }
}

void impressaoCentralizada(const char *s, int width){
    //FunÁ„o que imprime centralizadamente na tela
    int length = (int)strlen(s);
    int i;
    for (i=0; i<=(width-length)/2; i++){
        fputs(" ", stdout);
    }
    fputs(s, stdout);
    i += length;
    for (; i<=width; i++){
        fputs(" ", stdout);
    }
}

int alturaArvore(struct nodo *root){
    //FunÁ„o que retorna a altura geral da ·vore
    if (!root){//Se a raiz for NULA retorna zero
        return 0;
    }else if(root->valor){//Se existir um valor que n„o for NULO
        int esquerda = 0;
        if (root->nodos[ESQUERDA]){//Conta todo o lado esquerdo
            esquerda = alturaArvore(root->nodos[ESQUERDA]);
        }
        int direita = 0;
        if (root->nodos[DIREITA]){//Conta todo o lado direito
            direita = alturaArvore(root->nodos[DIREITA]);
        }
        return esquerda+direita+1;//Retorna esquerda + direita + raiz
    }
    return 0;
}

void imprimeNiveisArvore(struct nodo *root){
    int x = 0;
    //Imprime toda a ·vore, nÌvel por nÌvel
    while (1){
        struct nodo *printnode = criaNodo(0);
        
        criaListaNivel(printnode, root, x, 0);
        if (!printnode->nodos[ESQUERDA]) {
            break;
        }
        printnode = printnode->nodos[ESQUERDA];
        char line[600] = "";
        
        while (printnode) {
            char data[80];
            if (printnode->valor == INT_MAX) {
                
                strcat(line, " (nil)");
                
            }else{
                strcat(line, "  ");
                if (printnode->cor) {
                    sprintf(data, "(%d)", printnode->valor);
                }else{
                    sprintf(data, "[%d]", printnode->valor);
                }
                strcat(line, data);
            }
            printnode = printnode->nodos[ESQUERDA];
        }
        
        char nil[600] = "(nil)/";
        if (strlen(line) == strlen(nil)){
            break;
        }
        printf("\n");
        impressaoCentralizada(line, 60);
        x++;
    }
}

void modoInterface(struct arvore *raiz){
    //FunÁ„o do modo interface
    int valor = 0;
    int opcao = 0;
    while (opcao!=8){
        menu();
        scanf("%d", &opcao);
        if (opcao!=8){
            switch (opcao){
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
                    emOrdem(raiz->raiz, NULL);
                    break;
                case 4:
                    printf("\n Nivel a ser impresso: ");
                    scanf("%d", &valor);
                    imprimeRecursivo(raiz->raiz, valor, 0);
                    break;
                case 5:
                    printf("\n Valor ‡ ser procurado: ");
                    scanf("%d", &valor);
                    
                    if(verificaNodo(procuraNodo(valor, raiz->raiz))){
                        printf("TRUE");
                    }else{
                        printf("FALSE");
                    }
                    break;
                case 6:
                    imprimeNiveisArvore(raiz->raiz);
                    break;
                case 7:
                    if(verificaArvore(raiz->raiz)){
                        printf("TRUE");
                    }else{
                        printf("FALSE");
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

static char *fgetline(FILE *f){
    //FunÁ„o que lÍ linha por linha do arquivo recebido
    size_t len;
    char *line;
    line = fgetln(f, &len);
    //line = fgetline(f);
    if (line == 0) return 0;
    char *buf= malloc(len+1);
    if (buf == 0) return 0;
    memcpy(buf, line, len);
    buf[len]= '\0';
    return buf;
}

int main(int argc, const char * argv[]){
    int debug = 1;
    
    struct arvore *raiz = malloc(sizeof(Nodo));
    
    if ( argc != 2 ) //numero de argumentos deve ser 2 para correta execuÁ„o com nome do arquivo
    {
        /* argv[0] È o nome da aplicaÁ„o sendo executada */
        printf( "N„o foram passados argumentos, executando modo interface. \n\nPara passar argumentos execute: %s nomeDoArquivo\n\n", argv[0] );
        int larguraDisplay = 80;
        impressaoCentralizada("=================================", larguraDisplay);
        printf("\n");
        impressaoCentralizada("¡RVORE RUBRO-NEGRA", larguraDisplay);
        printf("\n");
        impressaoCentralizada("=================================", larguraDisplay);
        printf("\n");
        modoInterface(raiz);
        
        
    }else{
        
        // argv[1] deve ser o nome do arquivo ‡ ser aberto
        FILE *arquivoEntrada = fopen( argv[1], "r" );
        if (debug) printf("Abrindo arquivo -> %s ", argv[1]);
        /* fopen retorna 0 se houve algum erro. */
        if ( arquivoEntrada == 0 ){
            if (debug) printf("\n\nN„o foi possÌvel abrir o arquivo, executando modo interface.\n\n");
            modoInterface(raiz);
        }else{
            char *x;
            char *path = (char*)argv[1]; //nome do arquivo de saÌda ser· nomeDoArquivoDeEntrada+.output
            FILE *arquivoSaida = fopen(strcat(strtok(path, "."), ".output"), "w+" );
            if (debug) printf("\n\nArquivo de saida: %s \n\n",strcat(strtok(path, "."), ".output"));
            if (arquivoEntrada) {
                int linhaEntrada = 0;
                int linhaSaida = 0;
                while  ( ( x = fgetline(arquivoEntrada) ))
                {
                    linhaEntrada++;
                    char *funcao, *valor;
                    char *explode = " ";
                    funcao = strtok(x, explode);
                    valor = strtok(NULL, explode);
                    
                    if (strcmp(funcao, "insert")==0){
                        if (valor){
                            if (debug) printf("linhaEntrada %d || insert %d \n",linhaEntrada, atoi(valor));
                            insere(raiz, atoi(valor));
                        }else{
                            if (debug) printf("valor invalido");
                        }
                    }else if(strcmp(funcao, "search")==0){
                        if (valor){
                            linhaSaida++;
                            if (debug) printf("linhaEntrada %d || search %d ||",linhaEntrada, atoi(valor));
                            if(verificaNodo(procuraNodo(atoi(valor), raiz->raiz))){
                                if (debug) printf(" linhaSaida %d || TRUE", linhaSaida);
                                fputs("true\n", arquivoSaida);
                            }else{
                                fputs("false\n", arquivoSaida);
                                if (debug) printf(" linhaSaida %d || FALSE ", linhaSaida);
                            }
                            if (debug) printf("\n");
                        }else{
                            if (debug) printf("valor invalido");
                        }
                        
                    }else if (strcmp(funcao, "delete")==0) {
                        if (valor) {
                            if (debug) printf("linhaEntrada %d || delete %d \n",linhaEntrada, atoi(valor));
                            
                            remover(raiz, atoi(valor));
                        }else{
                            if (debug) printf("valor invalido");
                        }
                    }else if (strcmp(x, "inorder\n")==0) {
                        linhaSaida++;
                        if (debug) printf("linhaEntrada %d || inorder", linhaEntrada);
                        emOrdem(raiz->raiz, arquivoSaida);
                        fseek(arquivoSaida, -sizeof(char), SEEK_CUR);
                        fputs("\n", arquivoSaida);
                        if (debug) printf(" || linhaSaida %d", linhaSaida);
                        if (debug) printf("\n");
                    }
                    if (debug) printf("\n");
                }
                fclose(arquivoEntrada);
                fclose(arquivoSaida);
            }
        }
        
        if (debug) printf("\n\n-----------------\n");
        if (debug) printf("\n\n-----------------\n");
        if (debug) printf("\n\nLeitura ok, enter para modo interface\n");
        if (debug) getchar();
        if (debug) modoInterface(raiz);
    }
    return 0;
}


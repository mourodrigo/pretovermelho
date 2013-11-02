//
//  main.c
//  rb
//
//  Created by Rodrigo Bueno Tomiosso on 07/10/13.
//  Copyright (c) 2013 Rodrigo Bueno Tomiosso. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
struct jsw_node;
struct jsw_tree;



struct jsw_node {
    int red;
    int data;
    struct jsw_node *link[2];
}Jsw_node;

struct jsw_tree {
    struct jsw_node *root;
}Jsw_tree;

int is_red ( struct jsw_node *root );
struct jsw_node *jsw_single ( struct jsw_node *root, int dir );
struct jsw_node *jsw_double ( struct jsw_node *root, int dir );
int jsw_rb_assert ( struct jsw_node *root );
struct jsw_node *make_node ( int data );
struct jsw_node *jsw_insert_r ( struct jsw_node *root, int data );
struct jsw_node *jsw_remove_r ( struct jsw_node *root, int data, int *done );
struct jsw_node *jsw_remove_balance ( struct jsw_node *root, int dir, int *done );
int jsw_insert ( struct jsw_tree *tree, int data );
int jsw_remove ( struct jsw_tree *tree, int data );



int is_red ( struct jsw_node *root )
{
    return root != NULL && root->red == 1;
}



struct jsw_node *jsw_single ( struct jsw_node *root, int dir ){

struct jsw_node *save = root->link[!dir];

root->link[!dir] = save->link[dir];
save->link[dir] = root;

root->red = 1;
save->red = 0;

return save;
}

struct jsw_node *jsw_double ( struct jsw_node *root, int dir )
{
    root->link[!dir] = jsw_single ( root->link[!dir], !dir );
    return jsw_single ( root, dir );
}

int jsw_rb_assert ( struct jsw_node *root )
{
    int lh, rh;
    
    if ( root == NULL )
        return 1;
    else {
        struct jsw_node *ln = root->link[0];
        struct jsw_node *rn = root->link[1];
        
        /* Consecutive red links */
        if ( is_red ( root ) ) {
            if ( is_red ( ln ) || is_red ( rn ) ) {
                puts ( "Red violation" );
                return 0;
            }
        }
        
        lh = jsw_rb_assert ( ln );
        rh = jsw_rb_assert ( rn );
        
        /* Invalid binary search tree */
        if ( ( ln != NULL && ln->data >= root->data )
            || ( rn != NULL && rn->data <= root->data ) )
        {
            puts ( "Binary tree violation" );
            return 0;
        }
        
        /* Black height mismatch */
        if ( lh != 0 && rh != 0 && lh != rh ) {
            puts ( "Black violation" );
            return 0;
        }
        
        /* Only count black links */
        if ( lh != 0 && rh != 0 )
            return is_red ( root ) ? lh : lh + 1;
        else
            return 0;
    }
}


struct jsw_node *make_node ( int data ){
    struct jsw_node *rn = (struct jsw_node*)malloc ( sizeof *rn);
    
    if ( rn != NULL ) {
        rn->data = data;
        rn->red = 1; /* 1 is red, 0 is black */
        rn->link[0] = NULL;
        rn->link[1] = NULL;
    }
    
    return rn;
}


struct jsw_node *jsw_insert_r ( struct jsw_node *root, int data )
{
    if ( root == NULL )
        root = make_node ( data );
    else if ( data != root->data ) {
        int dir = root->data < data;
        
        root->link[dir] = jsw_insert_r ( root->link[dir], data );
        
        if ( is_red ( root->link[dir] ) ) {
            if ( is_red ( root->link[!dir] ) ) {
                /* Case 1 */
                root->red = 1;
                root->link[0]->red = 0;
                root->link[1]->red = 0;
            }
            else {
                /* Cases 2 & 3 */
                if ( is_red ( root->link[dir]->link[dir] ) )
                    root = jsw_single ( root, !dir );
                else if ( is_red ( root->link[dir]->link[!dir] ) )
                    root = jsw_double ( root, !dir );
            }
        }
    }
    
    return root;
}


struct jsw_node *jsw_remove_r ( struct jsw_node *root, int data, int *done )
{
    if ( root == NULL )
        *done = 1;
    else {
        int dir;
        
        if ( root->data == data ) {
            if ( root->link[0] == NULL || root->link[1] == NULL ) {
                struct jsw_node *save =
                root->link[root->link[0] == NULL];
                
                /* Case 0 */
                if ( is_red ( root ) )
                    *done = 1;
                else if ( is_red ( save ) ) {
                    save->red = 0;
                    *done = 1;
                }
                
                free ( root );
                
                return save;
            }
            else {
                struct jsw_node *heir = root->link[0];
                
                while ( heir->link[1] != NULL )
                    heir = heir->link[1];
                
                root->data = heir->data;
                data = heir->data;
            }
        }
        
        dir = root->data < data;
        root->link[dir] = jsw_remove_r ( root->link[dir], data, done );
        
        if ( !*done )
            root = jsw_remove_balance ( root, dir, done );
    }
    
    return root;
}

struct jsw_node *jsw_remove_balance ( struct jsw_node *root, int dir, int *done )
{
    struct jsw_node *p = root;
    struct jsw_node *s = root->link[!dir];
    
    /* Case reduction, remove red sibling */
    if ( is_red ( s ) ) {
        root = jsw_single ( root, dir );
        s = p->link[!dir];
    }
    
    if ( s != NULL ) {
        if ( !is_red ( s->link[0] ) && !is_red ( s->link[1] ) ) {
            if ( is_red ( p ) )
                *done = 1;
            p->red = 0;
            s->red = 1;
        }
        else {
            int save = p->red;
            int new_root = ( root == p );
            
            if ( is_red ( s->link[!dir] ) )
                p = jsw_single ( p, dir );
            else
                p = jsw_double ( p, dir );
            
            p->red = save;
            p->link[0]->red = 0;
            p->link[1]->red = 0;
            
            if ( new_root )
                root = p;
            else
                root->link[dir] = p;
            
            *done = 1;
        }
    }
    
    return root;
}



int jsw_insert ( struct jsw_tree *tree, int data )
{
    if ( tree->root == NULL ) {
        /* Empty tree case */
        tree->root = make_node ( data );
        if ( tree->root == NULL )
            return 0;
    }
    else {
        struct jsw_node head = {0}; /* False tree root */
        
        struct jsw_node *g, *t;     /* Grandparent & parent */
        struct jsw_node *p, *q;     /* Iterator & parent */
        int dir = 0, last;
        
        /* Set up helpers */
        t = &head;
        g = p = NULL;
        q = t->link[1] = tree->root;
        
        /* Search down the tree */
        for ( ; ; ) {
            if ( q == NULL ) {
                /* Insert new node at the bottom */
                p->link[dir] = q = make_node ( data );
                if ( q == NULL )
                    return 0;
            }
            else if ( is_red ( q->link[0] ) && is_red ( q->link[1] ) ) {
                /* Color flip */
                q->red = 1;
                q->link[0]->red = 0;
                q->link[1]->red = 0;
            }
            
            /* Fix red violation */
            if ( is_red ( q ) && is_red ( p ) ) {
                int dir2 = t->link[1] == g;
                
                if ( q == p->link[last] )
                    t->link[dir2] = jsw_single ( g, !last );
                else
                    t->link[dir2] = jsw_double ( g, !last );
            }
            
            /* Stop if found */
            if ( q->data == data )
                break;
            
            last = dir;
            dir = q->data < data;
            
            /* Update helpers */
            if ( g != NULL )
                t = g;
            g = p, p = q;
            q = q->link[dir];
        }
        
        /* Update root */
        tree->root = head.link[1];
    }
    
    /* Make root black */
    tree->root->red = 0;
    
    return 1;
}


int jsw_remove ( struct jsw_tree *tree, int data )
{
    if ( tree->root != NULL ) {
        struct jsw_node head = {0}; /* False tree root */
        struct jsw_node *q, *p, *g; /* Helpers */
        struct jsw_node *f = NULL;  /* Found item */
        int dir = 1;
        
        /* Set up helpers */
        q = &head;
        g = p = NULL;
        q->link[1] = tree->root;
        
        /* Search and push a red down */
        while ( q->link[dir] != NULL ) {
            int last = dir;
            
            /* Update helpers */
            g = p, p = q;
            q = q->link[dir];
            dir = q->data < data;
            
            /* Save found node */
            if ( q->data == data )
                f = q;
            
            /* Push the red node down */
            if ( !is_red ( q ) && !is_red ( q->link[dir] ) ) {
                if ( is_red ( q->link[!dir] ) )
                    p = p->link[last] = jsw_single ( q, dir );
                else if ( !is_red ( q->link[!dir] ) ) {
                    struct jsw_node *s = p->link[!last];
                    
                    if ( s != NULL ) {
                        if ( !is_red ( s->link[!last] ) && !is_red ( s->link[last] ) ) {
                            /* Color flip */
                            p->red = 0;
                            s->red = 1;
                            q->red = 1;
                        }
                        else {
                            int dir2 = g->link[1] == p;
                            
                            if ( is_red ( s->link[last] ) )
                                g->link[dir2] = jsw_double ( p, last );
                            else if ( is_red ( s->link[!last] ) )
                                g->link[dir2] = jsw_single ( p, last );
                            
                            /* Ensure correct coloring */
                            q->red = g->link[dir2]->red = 1;
                            g->link[dir2]->link[0]->red = 0;
                            g->link[dir2]->link[1]->red = 0;
                        }
                    }
                }
            }
        }
        
        /* Replace and remove if found */
        if ( f != NULL ) {
            f->data = q->data;
            p->link[p->link[1] == q] =
            q->link[q->link[0] == NULL];
            free ( q );
        }
        
        /* Update root and make it black */
        tree->root = head.link[1];
        if ( tree->root != NULL )
            tree->root->red = 0;
    }
    
    return 1;
}

void Tree_inOrder(struct jsw_node *n) /*see declaration of TNode below*/

{
    
    if(n==0)
        
        return;
    
    Tree_inOrder(n->link[0]);
    
    printf("%d ", n->data);
    
    Tree_inOrder(n->link[1]);
    
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

int main(int argc, const char * argv[])
{
 
    
    struct jsw_tree *root = malloc(sizeof(Jsw_node));
    
                                          
    
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
                    jsw_insert(root, valor);
                    
                    
                    break;
                case 2:
                    printf("\n Valor a ser removido: ");
                    scanf("%d", &valor);
                    
                    jsw_remove ( root, valor );
                    break;
                case 3:
                    Tree_inOrder(root->root);
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
                case 7:
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


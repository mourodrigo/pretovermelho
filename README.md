#Arvore Preto Vermelha em C

Implementaço de uma árvore preto-vermelha. Com suporte para:

1. Inserir.
2. Remover.
3. Listar os elementos em ordem (in-order).
4. Listar os elementos de um determinado nível.
5. Procurar por elementos.
6. Imprimir a árvore, de maneira a ser possível distinguir sua estrutura.
7. Verificar as 4 propriedades da árvore preto-vermelha.

Quando os elementos são adicionado ou removidos, a árvore realiza um balanceamento automático.

O protótipo deverá le os comandos a serem executados da saída padrão com comandos suportados:

**insert n**
Insere o elemento n na árvore.

**remove n**
Remove o elemento n da árvore.

**inorder**
Imprime a lista de nodos da árvore, separados por um espaço (“ “) fazendo uma travessia in-order.

**level n**
Imprime a lista de nodos da árvore em determinado nível, separados por espaço

**search n**
Imprime “true” caso o elemento estiver na árvore, ou “false” caso o elemento não esteja na árvore

**verify**
Imprime “true” caso a árvore respeite as propriedades das árvores preto-vermelha, ou “false” caso o contrário

**print**
Imprime toda a árvore de maneira que a sua estrutura seja visualizável.

Um nodo preto n deverá ser impresso da seguinte forma: [n] ; já um nodo vermelho n deverá ser impresso da seguinte forma: (n). 
Isto é, os nodos vermelhos serão identificados por () e os pretos por [].

    $ ./redblacktree < teste1.input > output


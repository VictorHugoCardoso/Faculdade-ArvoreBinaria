#include "header.h"

// --------------------------------------------------------------------------

int power(int x, int y){					//x elevado por y
	int n,i;
	n=1;
	for(i=0;i<y;i++)
		n *= 10;
	return n;
}

int strtodig(char* s, int l){				//transforma uma string em um numero
	int i,n = 0;
	for(i=0;i<l;i++){
		n += (s[i] - '0') * power(10,l -1 -i);	//o caracter multiplicado pelo valor de sua posicao
	}
	return n;
}

// --------------------------------------------------------------------------

cabecalho* le_cabecalho(FILE* arq){
	cabecalho * cab = (cabecalho*) malloc(sizeof(cabecalho));
	fseek(arq,0,SEEK_SET);
	fread(cab,sizeof(cabecalho),1,arq);
	return cab;
}

//Função responsável por escrever um novo cabeçalho no arquivo
//Entrada: Ponteiro para o arquivo, ponteiro para o cabeçalho
//Retorno: void
void escreveCabecalho(FILE* arq, cabecalho* cab){
    fseek(arq,0,SEEK_SET); //posiciona no início do arquivo
    fwrite(cab,sizeof(cabecalho),1,arq);
}

//Função responsável por criar um cabeçalho vazio
//Entrada: Ponteiro para o arquivo
//Retorno: void
void cria_lista_vazia(FILE* arq){
    cabecalho * cab = (cabecalho*) malloc(sizeof(cabecalho));
    cab->pos_cabeca = -1;
    cab->pos_topo = 0;
    cab->pos_livre = -1;
    escreveCabecalho(arq,cab);
    free(cab);
 }

produto* le_produto(FILE* arq, int pos) {
	produto *x = (produto*)malloc(sizeof(produto));
	fseek(arq,sizeof(cabecalho)+ pos*sizeof(produto),SEEK_SET);
	fread(x,sizeof(produto),1,arq);
	return x;
}

//Função responsável por escrever um novo nó no arquivo
//Entrada: Ponteiro para o arquivo, ponteiro para o no, inteiro referente a posição
//Retorno: void
void escreveProduto(FILE* arq, produto* x, int pos){
    fseek(arq,sizeof(cabecalho) + pos*sizeof(produto),SEEK_SET);
    fwrite(x,sizeof(produto),1,arq);
}

// --------------------------------------------------------------------------

//Função responsável por inserir novos livros no arquivo binário
//Entrada: Ponteiro para o livro
//Retorno: void
void insere_produto_binario(FILE* arq, produto* p){
    cabecalho *cab = le_cabecalho(arq); //Cabeçalho do arquivo
    int posInserida;    //Variavel responsável por guardar onde o novo nó foi inserido para 
                        //atualizar para onde o pai do novo nó deve apontar

    if(cab->pos_cabeca == -1){ //Verifica se é a primeira informação a ser adicionado ao arquivo binário
        escreveProduto(arq, p, ++cab->pos_cabeca);
        posInserida = cab->pos_cabeca;
        cab->pos_topo++;
    }
    else if(cab->pos_livre == -1) { //Caso não existe posição livre, insere no final do arquivo
        escreveProduto(arq, p, cab->pos_topo);
        posInserida = cab->pos_topo;
        cab->pos_topo++;
    }
    else { //Caso exista posição livre, insere o no na primeira posição livre 
        produto* aux = le_produto(arq, cab->pos_livre);
        escreveProduto(arq, p, cab->pos_livre);
        posInserida = cab->pos_livre;
        cab->pos_livre = aux->r; //A lista de posições livres ficam salvas nos elementos r do nó
        free(aux);
    }
 
    //Atualiza as informações do cabeçalho
    escreveCabecalho(arq, cab); 
    printf("\n\ninseriu!\n");
    printf("cabeca:%d\n topo:%d\n livre:%d\n\n\n", cab->pos_cabeca,cab->pos_topo,cab->pos_livre);
 	
    produto* pai;
    int posPai = cab->pos_cabeca;
    int posPaiNext = cab->pos_cabeca;
    pai = le_produto(arq,posPaiNext);

    
    do{ //Loop responsável por percorrer a arvore e encontrar a posição do nó pai do nó inserido recentemente
        pai = le_produto(arq,posPaiNext);
        posPai = posPaiNext;
        if(p->codigo >= pai->codigo){
            posPaiNext = pai->r;
        }else{
            posPaiNext = pai->l;
        }
    } while(posPaiNext != -1);
    
 
    if(posInserida!= cab->pos_cabeca){ //Verifica se não é o primeiro elemento inserido
        if(p->codigo >= pai->codigo){
            pai->r = posInserida;
        }else{
            pai->l = posInserida;
        }
        escreveProduto(arq,pai,posPai); //Atualiza o valor de l ou r do nó pai
    }
	

    free(pai);
    free(cab);
}

//Função que realiza uma busca pelo codigo de um produto no arquivo de dados
//Entrada: código referente ao produto
//Retorno: posição do produto, no arquivo, ou -1 caso não exista
int posProduto(FILE* f,int code){

    cabecalho *cab = le_cabecalho(f); //Cabeçalho do arquivo
 
    produto* pai;
    int posPai;
     
    posPai = cab->pos_cabeca;
    pai = le_produto(f,posPai);
 
    do{ //Loop responsável por percorrer a arvore e encontrar o livro com o código inserido
        free(pai); //Evita memory leak
        pai = le_produto(f,posPai);
     
        if(code > pai->codigo)
            posPai = pai->r;
        else if(code < pai->codigo)
            posPai = pai->l;
        else
            break;
 
    } while((posPai != -1));
 
 
    free(pai);
 
    return posPai;
}

void imprimeProduto(produto* p){
    printf("========================================\n");
    printf("| Codigo: %d\n",p->codigo);
    printf("| Nome: %s\n",p->nome);
    printf("| Marca: %s\n",p->marca);
    printf("| Preco Unitario: R$ %.2f\n",p->valor);
    printf("| Quantidade em Estoque: %d\n",p->quantidade);
    printf("========================================\n\n\n");
}

// --------------------------------------------------------------------------

void imprimirEstoqueAux(FILE* f, int pos){

	if(pos != -1){
        produto* p = le_produto(f,pos);
        imprimirEstoqueAux(f, p->l);
        imprimeProduto(p);
        imprimirEstoqueAux(f, p->r);
        free(p);
    }

}

void imprimirEstoque(FILE* f){

	cabecalho* cab = le_cabecalho(f);

	printf("----------------------------------------\n");
	printf("|                ESTOQUE               |\n");
	printf("----------------------------------------\n\n");

	imprimirEstoqueAux(f, cab->pos_cabeca);

}

void imprimirArvoreAux(FILE* f, int i){
	if(i != -1){
        produto* p = le_produto(f,i);
        printf(" [%d,", p->codigo);
         
        if(p->l != -1){ //Verifica se o nó existe, ou é NULL
            imprimirArvoreAux(f,p->l);
        }else{
            printf("[]");
        }
 
        printf(",");
 
        if(p->r != -1){ //Verifica se o nó existe, ou é NULL
            imprimirArvoreAux(f,p->r);
        }else{
            printf("[]");
        }
 
        printf("]");
 
        free(p);
    }
}

void imprimirArvore(FILE* f){

	cabecalho* cab = le_cabecalho(f);

	printf("----------------------------------------\n");
	printf("|            ARVORE BINARIA            |\n");
	printf("----------------------------------------\n\n");

	imprimirArvoreAux(f, cab->pos_cabeca);

}

void inserirProduto(FILE* f){

	produto* p = (produto*) malloc(sizeof(produto));
	
	printf("Insira o Codigo:\n");
	fflush(stdin);
	scanf("%d",&p->codigo);

	printf("\nInsira o Nome do Produto:\n");
	fflush(stdin);
	scanf("%[^\n]",&p->nome);

	printf("\nInsira a Marca do Produto:\n");
	fflush(stdin);
	scanf("%[^\n]",&p->marca);

	printf("\nInsira o Valor Unitario:\n");
	fflush(stdin);
	scanf("%f",&p->valor);

	printf("\nInsira a Quantidade em estoque:\n");
	fflush(stdin);
	scanf("%d",&p->quantidade);

	p->l = -1;
    p->r = -1;

	insere_produto_binario(f,p);
	free(p);

}

void atualizarPreco(FILE* f){

	int codigo,novoValor;

	printf("Insira o Codigo:\n");
	fflush(stdin);
	scanf("%d",&codigo);

	printf("Insira o novo Preco:\n");
	fflush(stdin);
	scanf("%d",&novoValor);



}

void atualizarEstoque(FILE* f){
	int codigo,novoValor, pos;

	printf("Insira o Codigo:\n");
	fflush(stdin);
	scanf("%d",&codigo);

	printf("Insira a nova Quantidade:\n");
	fflush(stdin);
	scanf("%d",&novoValor);

    pos = posProduto(f,codigo);

    if(pos == -1){ //Verifica a existência do produto
        printf("\nPRODUTO INEXISTENTE!\n");
        scanf("%*c");// serve apenas para impressao
        scanf("%*c");
    }

    produto* p = le_produto(f,pos);

    p->quantidade = novoValor;

    escreveProduto(f,p,pos);

    free(p);	

}

//Função que devolve o menor entre os maiores elementos
//Entrada: No raiz a ser verificado
//Retorno: Posição do menor entre os maiores
int menorEntreMaiores(FILE *f, int pos){
    produto* p = le_produto(f,pos);
    int posprox;
    pos = posprox = p->r;
 
    while(posprox != -1){
        free(p);
        produto* p = le_produto(f,posprox);
        pos = posprox;
        posprox = p->l;
    }
    free(p);
     
    return pos;
}

//Função que devolve o maior entre os menores elementos
//Entrada: No raiz a ser verificado
//Retorno: Posição do maior entre os menores
int maiorEntreMenores(FILE *f, int pos){
    produto* p = le_produto(f,pos);
    int posprox;
    pos = posprox = p->l;
 
    while(posprox != -1){
        free(p);
        produto* p = le_produto(f,posprox);
        pos = posprox;
        posprox = p->r;
    }
    free(p);
 
    return pos;
}

//Função que copia as informações de l2 para l1;
//Entrada: ponteiro para o livro 1 e ponteiro para o livro 2 
//Retorno: void
void copiaProduto(produto* p1, produto* p2){
    p1->codigo = p2->codigo;
    strcpy(p1->nome, p2->nome);
    strcpy(p1->marca, p2->marca);
    p1->valor = p2->valor;
    p1->quantidade = p2->quantidade;
}

int removerProdutoAux(FILE *f, cabecalho *cab, int pos, int x){

	produto* p = le_produto(f,pos);

	if(x < p->codigo){ //Elemento está a esquerda
        p->l = removerProdutoAux(f,cab,p->l,x);
    }else{
    	if(x > p->codigo){ //Elemento está a direita
            p->r = removerProdutoAux(f,cab,p->r,x);
        }else{// Elemento encontrado
        	if((p->l == -1) && (p->r == -1)){ //Elemento folha
                p->r = cab->pos_livre;
                cab->pos_livre = pos;
                escreveProduto(f,p,pos);
                free(p);
                return -1;
            }else if(p->l == -1){ //Sem filhos a esquerda
                produto* pAux = le_produto(f,menorEntreMaiores(f,pos));
                copiaProduto(p, pAux);
                p->r = removeLivroAux(f,cab,p->r,pAux->codigo);
                free(pAux);
            }else{ //Sem filhos a direita ou com filhos para ambos os lados
                produto* pAux = le_produto(f,maiorEntreMenores(f,pos));
                copiaProduto(p, pAux);
                p->l = removeLivroAux(f,cab,p->l,pAux->codigo);
                free(pAux);
            }
        }
    }

    escreveProduto(f,p,pos);
    free(p);
 
    return pos;

}

void removerProduto(FILE* f){
	int codigo;

	printf("Insira o Codigo:\n");
	fflush(stdin);
	scanf("%d",&codigo);

	int pos = posProduto(f,codigo);

    if(pos==-1){
        printf("\nLivro inexistente\n");
        scanf("%*c");   // serve apenas para impressao
        scanf("%*c");
    }else{

	    cabecalho *cab = le_cabecalho(f); //Cabeçalho que será alterado 
	    removerProdutoAux(f, cab, cab->pos_cabeca, codigo);
	    escreveCabecalho(f,cab);

	    free(cab);

	}
}

void buscarProduto(FILE* f){

	int codigo,pos;

	printf("Insira o Codigo:\n");
	fflush(stdin);
	scanf("%d",&codigo);

	pos = posProduto(f,codigo);

	if(pos == -1){ //Verifica a existência do produto
        printf("\nPRODUTO INEXISTENTE\n");
        scanf("%*c");// serve apenas para impressao
        scanf("%*c");
    }

    produto* p = le_produto(f,pos);

    printf("\n");
    imprimeProduto(p);

    free(p);

}

//Auxiliar do quicksort
int Separa(produto* vet,int e, int d){

    int ax = 0;
    int j = 0,k = 0;
    produto pivo, temp;
 
    copiaProduto(&pivo,&vet[d]);
     
    j = e;
    for( k = e; k < d; k++){
        if(strcmp(vet[k].nome, pivo.nome) <= 0){
            copiaProduto(&temp,&vet[j]);
            copiaProduto(&vet[j],&vet[k]);
            copiaProduto(&vet[k],&temp);
            j++;
        }
    }
 
    copiaProduto(&vet[d],&vet[j]);
    copiaProduto(&vet[j],&pivo);
 
    return j;

}

//Função responsável por realizar o quicksort de um vetor de produtos
//Entrada: Ponteiro para vetor de produtos, e = 0, Quantidade de produtos - 1
//Retorno: void
void QuickSort(produto vet[],int e, int d) {
    int j;
    if( e < d){
        j = Separa(vet,e,d);
        QuickSort(vet,e,j-1);
        QuickSort(vet,j+1,d);
    }
}

//Função que conta recursivamente a quantidade de nós da árvore
//Entrada: File , posição do nó inicial
//Retorno: inteiro referente ao número de nós
int contaNos(FILE *f, int pos){
    if(pos == -1){
        return 0;
    }
    produto* p = le_produto(f, pos);
     
    int lNos, rNos;
 
    lNos = contaNos(f,p->l);
    rNos = contaNos(f,p->r);
 
    free(p);
 
    return 1 + lNos + rNos;
}


//Função que carrega pra memória um vetor de structs
//Entrada: File, ponteiro para o vetor de produtos, posição a ser lida, valor de controle da posição no vetor
//Retorno: int
int carregaVet(FILE *f, produto* vet, int pos, int i){
     
    if(pos == -1){
        return 0;
    }

    produto* p = le_produto(f, pos);

    vet[i]->codigo = p->codigo;
    strcpy(vet[i]->nome, p->nome);
    strcpy(vet[i]->marca, p->marca);
    vet[i]->valor = p->valor;
    vet[i]->quantidade = p->quantidade;

    if(p->l != -1){
        i = carregaVetAux(f,vet,p->l,++i);
    }
    if(p->r != -1){
        i = carregaVetAux(f,vet,p->r, ++i);
    }

    free(p);

    return i;
}

//Função responsável por gerar um arquivo texto com os elementos de um vetor de produtos
//Entrada: Ponteiro para o vetor de produtos, quantidade de produtos, nome do arquivo
//Retorno: void
void geraArqTxt(produto* vet, int n, char *nome){
    FILE *f = fopen(nome, "w");
    int i;
    for(i = 0; i < n; i++){
        fprintf(f, "Nome      : %s\n"  , vet[i].nome);
        fprintf(f, "Codigo    : %d\n"  , vet[i].codigo);
        fprintf(f, "Marca     : %s\n"  , vet[i].marca);
        fprintf(f, "Valor     : R$ %f\n"  , vet[i].valor);
        fprintf(f, "Quantidade: %d\n\n", vet[i].quantidade);
    }
    fclose(f);
}

void gerarListagemOrdenada(FILE* f){

    cabecalho *cab = le_cabecalho(f);
 
    int n = contaNos(f,cab->pos_cabeca); //Tamanho do vetor

    produto* vet = (produto*)malloc(sizeof(produto)*n); //Aloca o vetor de nós

    carregaVet(f,vet,cab->pos_cabeca, 0);

    free(cab);

    int i,j;

    QuickSort(vet,0,n-1);
         
    geraArqTxt(vet,n,"ListagemOrdenada.txt");
    printf("Listagem gerada com sucesso!!\n");

    free(vet);

}

void inicializa(FILE *e, char* txt){
	cria_lista_vazia(e);	//como o arquivo é sobrescrito, uma nova lista precisa ser criada
	char s[200];

	FILE *arq;
	if((arq = fopen(txt,"r")) == NULL){
		printf("Erro ao abrir o arquivo texto\n"); //erro na abertura do arquivo
	}

	fseek(arq,0,SEEK_SET);								//posiciona o cursor no comeco do arquivo texto

	produto *p = (produto*) malloc(sizeof(produto));		
	int i,k;
	char aux[10];

	while(fgets(s,100,arq) != 0){
		for(i=0;i<MAX_NOME;i++){
            p->nome[i] = '\0';
        }
        for(i=0;i<MAX_MARCA;i++){
            p->marca[i] = '\0';
        }

		k = 0;
		i = 0;

		while(s[i] != ':'){			
			aux[k] = s[i];
			i++;k++;			
		}
		
		p->codigo = strtodig(aux,k);

		k = 0;
		i += 1;

		while(s[i] != ':'){			
			p->nome[k] = s[i];	
			i++;k++;			
		}

		k = 0;
		i += 1;

		while(s[i] != ':'){			
			p->marca[k] = s[i];	
			i++;k++;			
		}

		k = 0;
		i += 1;

		while(s[i] != ':'){			
			aux[k] = s[i];	
			i++;k++;			
		}
		
		p->valor = 20; //asdasdasdasdasdasd


		i += 1;

		for(k=0; i<strlen(s)-1; i++,k++){	
			aux[k] = s[i];
		}

		p->quantidade = 30; //asdasdasdasdasdasd

		p->l = -1;
    	p->r = -1;

		insere_produto_binario(e,p);
	}
	
	fclose(arq);			
	free(e);
    printf("O arquivo foi inicializado com sucesso!\n");
}
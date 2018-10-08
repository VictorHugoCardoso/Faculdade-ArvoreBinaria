#ifndef LISTA_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_NOME 50
#define MAX_MARCA 50

// |ESTRUTURAS|

typedef struct produto{
	int codigo;
	char nome[MAX_NOME];
	char marca[MAX_MARCA];
	float valor;
	int quantidade;
	int l;
	int r;
}produto;

//estrutuda dos cabeçalhos
typedef struct cabecalhos{
	int pos_cabeca;					//posicao do inicio da lista
	int pos_topo;					//primeira posicao nao usada no fim do arquivo
	int pos_livre;					//posicao do inicio da lista de nos livres
}cabecalho;	

// |FUNÇÕES CABECALHO|

//le_cabecalho
cabecalho* le_cabecalho(FILE * arq);

//escreve_cabecalho
void escreve_cabecalho(FILE* arq, cabecalho* cab);

//cria_lista_vazia
void cria_lista_vazia(FILE* arq);

// |FUNÇÕES AUXILIARES|

int power(int x, int y); 					//eleva x a y

int strtodig(char* s, int l);				//transforma uma string em um numero

// |FUNÇÕES|

// Entrada: handle do arquivo binario (modo r) e a respectiva posição do produto
// Retorno: estrutura de produto
// Pré-condições: nenhum
// Pós-condições: retorna estrutura do produto procurado
produto* le_produto(FILE* arq, int pos);
void escreve_produto(FILE* arq, produto* x, int pos);	//escreve produto no arquivo
void insere_produto_binario(FILE* arq, produto* p);		//insere produto no arquivo

//Função que realiza uma busca pelo codigo de um produto no arquivo de dados
//Entrada: código referente ao produto
//Retorno: posição do produto, no arquivo, ou -1 caso não exista
int posProduto(FILE* f,int code);

//Função que devolve o menor entre os maiores elementos
//Entrada: No raiz a ser verificado
//Retorno: Posição do menor entre os maiores
int menorEntreMaiores(FILE *f, int pos);

//Função que devolve o maior entre os menores elementos
//Entrada: No raiz a ser verificado
//Retorno: Posição do maior entre os menores
int maiorEntreMenores(FILE *f, int pos);

//Função que copia as informações de p2 para p1;
//Entrada: ponteiro para o primeiro produto e ponteiro para o segundo produto 
//Retorno: void
void copiaProduto(produto* p1, produto* p2);

//Auxiliar do quicksort
int Separa(produto* vet,int e, int d);

//Função responsável por realizar o quicksort de um vetor de produtos
//Entrada: Ponteiro para vetor de produtos, e = 0, Quantidade de produtos - 1
//Retorno: void
void QuickSort(produto vet[],int e, int d);

//Função responsável por gerar um arquivo texto com os elementos de um vetor de produtos
//Entrada: Ponteiro para o vetor de produtos, quantidade de produtos, nome do arquivo
//Retorno: void
void geraArqTxt(produto* vet, int n, char *nome);

//Função que carrega pra memória um vetor de structs
//Entrada: File, ponteiro para o vetor de produtos, posição a ser lida, valor de controle da posição no vetor
//Retorno: int
int carregaVet(FILE *f, produto* vet, int pos, int i);

//Função que conta recursivamente a quantidade de nós da árvore
//Entrada: File , posição do nó inicial
//Retorno: inteiro referente ao número de nós
int contaNos(FILE *f, int pos);

void imprimeProduto(produto *p);

void imprimirEstoque(FILE* f);
void imprimirEstoqueAux(FILE* f, int pos);
void imprimirArvore(FILE* f);
void imprimirArvoreAux(FILE* f, int i);
void inserirProduto(FILE* f);
void atualizarPreco(FILE* f);
void atualizarEstoque(FILE* f);
int removerProdutoAux(FILE *f, cabecalho *cab, int pos, int x);
void removerProduto(FILE* f);
void buscarProduto(FILE* f);
void gerarListagemOrdenada(FILE* f);


// Entrada: handle do arquivo binario (modo w) e o nome do arquivo texto
// Retorno: codigo de erro
// Pré-condições: arquivo deve estar aberto e deve ser um arquivo de lista
// Pós-condições: as informacoes do arquivo texto sao carregadas no arquivo binario
void inicializa(FILE *e, char* txt);

#endif
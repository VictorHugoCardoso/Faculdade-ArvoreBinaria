#include "funcoes.c"

int main(){

	FILE *e;
	if((e = fopen("estoque.bin","r+b")) == NULL){
		printf("Erro na abertura do arquivo!\nArquivos sobrescritos\n");
		e = fopen("estoque.bin","w+b");                                   
	}
	int resp=1;
	
	while(1){
        system("cls");
        printf("         *---------------------------*\n");
        printf("         |  ESTOQUE DE SUPERMERCADO  |\n");
        printf("         *---------------------------*\n");
        printf("*--------------------------------------------*\n");
        printf("| |01| - Imprimir Estoque                    |\n");
        printf("| |02| - Imprimir Arvore Binaria             |\n");
        printf("*--------------------------------------------*\n");
        printf("| |03| - Inserir Produto                     |\n");
        printf("*--------------------------------------------*\n");
        printf("| |04| - Atualizar Preco                     |\n");
        printf("| |05| - Atualizar Estoque                   |\n");
        printf("*--------------------------------------------*\n");
        printf("| |06| - Remover Produto                     |\n");
        printf("*--------------------------------------------*\n");
        printf("| |07| - Buscar Produto                      |\n");
        printf("*--------------------------------------------*\n");
        printf("| |08| - Gerar Listagem Produtos             |\n");
        printf("*--------------------------------------------*\n");
        printf("| |10| - Carregar Arquivo                    |\n");
        printf("*--------------------------------------------*\n");
        printf("| |00| - Sair                                |\n");
        printf("*--------------------------------------------*\n\n");
        printf("ESCOLHA: ");
        fflush(stdin);
        scanf("%d", &resp);
        system("cls");
        
        switch(resp){
			case 1:
			     
                imprimirEstoque(e); 
				scanf("%*c");   // serve apenas para impressao
                scanf("%*c");
				break;
			case 2:

                imprimirArvore(e); 
                scanf("%*c");   // serve apenas para impressao
                scanf("%*c");
				break;
			case 3:

                inserirProduto(e);
				break;
			case 4:

                atualizarPreco(e);
				break;
			case 5:

                atualizarEstoque(e);
				break;
			case 6:

                removerProduto(e);
				break;
			case 7:

                buscarProduto(e);
                scanf("%*c");   // serve apenas para impressao
                scanf("%*c");
				break;
			case 8:

                gerarListagemOrdenada(e);
                scanf("%*c");   // serve apenas para impressao
                scanf("%*c");
				break;

			case 10:
                fclose(e);
                e = fopen("estoque.bin","w+b"); // reabre para sobrescrever
                inicializa(e,"produtos.txt"); // carrega o arquivo de texto para o arquivo binario
                fclose(e); // fecha o arquivo
                e = fopen("estoque.bin","r+b"); //  e reabre-o para leitura
                scanf("%*c");// serve apenas para impressao
                scanf("%*c");
                break;

			case 0:
				fclose(e);
                exit(1);
            default:
                printf("Invalido\n");
        }
    }
	return 0;
}

/****************************
 *                          *
 * author: Paulo Nicolau    *
 * number_UAB: 1800465      *
 *                          *
 ***************************/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>

#include <unistd.h>

/***********************
 * Constantes / Macro  *
 * ********************/
#define STR 50


/************************
 * Estruturas de Dados  *
 * *********************/

//Estrutura Unidade Curricular
typedef struct{
    int numero;
    char * nome;
} UC;

//Estrutura Lista Unidades Curiculares
typedef struct node{
    UC * atual;
    struct node * proxima;
} NodeUC;

/************
 * Headers  *
 * *********/
void apagaUC(UC * ucs);


/************************
 * Metodos Estruturas   *
 ***********************/

//Adicionar Elemento no fim da lista
NodeUC * adicionarNode(NodeUC * lista, NodeUC * novo){
    if(lista==NULL){
        return novo;
    }
    if(lista->proxima==NULL){
        lista->proxima = novo;
        return lista;
    }
    adicionarNode(lista->proxima, novo);
    return lista;
}

//Cria Nó do elemento a adicionar na lista
NodeUC * criarNodeUC(UC * unidade){
    NodeUC * novo = malloc(sizeof(NodeUC));
    if(!novo){
        printf("Erro: Não foi possivel alocar memoria para o elemento da lista");
        exit(1);
    }
    novo->atual = unidade;
    novo->proxima = NULL;
    return novo;
}

//Apagar Lista (Libertar Memoria)
void eliminarUCs(NodeUC * lista){
    if(lista!= NULL){
        if(lista->proxima!= NULL){
            eliminarUCs(lista->proxima);
        }
        apagaUC(lista->atual);
        lista->atual=NULL;
        free(lista);
        lista = NULL;
    }
}

/************************
 *  Funções Auxiliares  *
 * *********************/

//Imprimir UC
void imprimeUC(UC * ucs){
    printf("%d - %s\n", ucs->numero, ucs->nome);
}

//Funções para remover espaços brancos no inicio e fim do nome
void removerEspacosInit(char * palavra){
    int index = 0;
    while(palavra[index]==' '){
        index++;
    }
    for(int i=0; i< (int)strlen(palavra); i++){
        palavra[i]= palavra[index++];
    }
}
void removerEspacoEnd(char * palavra){
    int index = strlen(palavra)-1;
    while(palavra[index]==' '){
        index--;
    }
    palavra[index+1]='\0';
}
void removerEspacos(char * palavra){
    removerEspacosInit(palavra);
    removerEspacoEnd(palavra);
}

//Cria Unidades Curriculares
UC * criaUC(char * linha){
    UC * unidadeCurricular = malloc(sizeof(UC));
    unidadeCurricular->nome= malloc(STR * sizeof(char));
    if(!unidadeCurricular){
        printf("Erro: Não foi possivel alcar memoria para a unidade curricular");
        exit(1);
    }
    char * parte;
    parte = strtok(linha,"-");
    unidadeCurricular->numero=atoi(parte);
    parte = strtok(NULL,"\n");
    removerEspacos(parte);
    strcpy(unidadeCurricular->nome,parte);
    return unidadeCurricular;
}

//Liberta Memoria das Unidades Curriculares
void apagaUC(UC * ucs){
    if(ucs != NULL){
        free(ucs->nome);
        free(ucs);
        ucs = NULL;
    }
}

//Ler ficheiro  para criar Unidades Curriculares
NodeUC  * lerUC(){
    NodeUC  * listaUnidades =NULL;
    FILE * f;
    f = fopen("uc.txt", "r");
    char linha[STR];
    if(!f){
        printf("Erro: Não foi possivel abrir o ficheiro!");
        exit(1);
    }
    while (fgets(linha,STR,f)!= NULL){
        listaUnidades = adicionarNode(listaUnidades, criarNodeUC(criaUC(linha)));
    }
    fclose(f);
    return listaUnidades;
}

//Imprimir Lista 
void imprimirLista(NodeUC * uc){
    if(uc != NULL){
        imprimeUC(uc->atual);
        if(uc->proxima != NULL){
            imprimirLista(uc->proxima);
        }
    }
}



/************************
 *  Código Alinea A     *
 * *********************/

int contaCaracteres(NodeUC * lista){
    int contar = 0;
    while(lista!= NULL){
        contar += strlen(lista->atual->nome);
        lista = lista->proxima;
    }
    return contar;
}


/************************
 *  Função Principal    *
 * *********************/
int main() {
    //FILE *f=stdin; // ler os dados do stdin
    NodeUC  * listaUnidades=NULL;
    listaUnidades = lerUC();
    imprimirLista(listaUnidades);
    printf("%d ",contaCaracteres(listaUnidades));
    eliminarUCs(listaUnidades);
    listaUnidades=NULL;
}
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
#include <limits.h>

/***********************
 * Constantes / Macro  *
 * ********************/
#define STR 50
#define FILEPATH "uc.txt"

/************************
 * Estruturas de Dados  *
 * *********************/
typedef struct{
    int numero;
    char * nome;
} UC;

typedef struct node{
    UC * atual;
    struct node * proxima;
} NodeUC;

/************************
 * Metodos Estruturas   *
 ***********************/

void adicionarUC(NodeUC  * lista, UC * unidade){
    printf("D"); fflush(stdout);
    if(lista->atual == NULL){
        lista->atual = unidade;
        lista->proxima = NULL;
    } else{
        NodeUC * novo;
        novo->atual = unidade;
        novo->proxima = NULL;
        NodeUC * uc = lista->proxima;
        while(uc != NULL){
            uc = uc->proxima;
        }
        uc->proxima = novo;
    }
}

/************************
 *  Código Alinea A     *
 * *********************/

UC * criaUC(char * linha){
    printf("H");
    UC * unidadeCurricular = malloc(sizeof(UC));
    char * parte;
    parte = strtok(linha,"-");
    unidadeCurricular->numero = atoi(parte);
    unidadeCurricular->nome = strtok(linha," ");
    return unidadeCurricular;
}

void apagaUC(NodeUC * ucs){
    NodeUC * lista = ucs;
    while(lista != NULL){
        free(lista->atual);
        lista->atual = lista->proxima->atual;
        lista->proxima = lista->proxima->proxima;
    }
}

NodeUC  * lerUC(){
    NodeUC  * listaUnidades;
    listaUnidades->atual == NULL;
    listaUnidades->proxima == NULL;
    FILE * f;
    f = fopen("uc.txt", "r");
    char * linha;
    if(!f){
        printf("Erro: Não foi possivel abrir o ficheiro!");
        exit(1);
    }
    fgets(linha,STR,f);
    printf("%s", linha);
    fflush(stdout);
    while (linha != NULL){
        adicionarUC(listaUnidades, criaUC(linha));
    }
    fclose(f);
    return listaUnidades;
}

void imprimirUC(NodeUC * uc){
    while(uc->atual != NULL){
        printf("%d - %s", uc->atual->numero, uc->atual->nome);
        if(uc->proxima != NULL){
            imprimirUC(uc->proxima);
        }
    }
}



int contaCaracteres(){
    return 0;
}

/************************
 *  Função Principal    *
 * *********************/
int main() {
    //FILE *f=stdin; // ler os dados do stdin
    NodeUC  * listaUnidades;
    listaUnidades = lerUC();


   //imprimirUC(listaUnidades);
}
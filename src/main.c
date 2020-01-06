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
#define STR 100


/************************
 * Estruturas de Dados  *
 * *********************/

//Estrutura Atividades da UC
typedef struct{
    int idUC;
    int inicio;
    int fim;
    int realizado;
    int numSessao;
    char * nome;
} AtividadeUC;

//Estrutura Lista de Atividades
typedef struct nodeAct{
    AtividadeUC * atual;
    struct nodeAct * proxima;
} NodeAtividade;

//Estrutura Unidade Curricular
typedef struct{
    int numero;
    char * nome;
    NodeAtividade * atividades;
} UC;

//Estrutura Lista Unidades Curiculares
typedef struct node{
    UC * atual;
    struct node * proxima;
} NodeUC;

typedef struct{
    NodeUC * ucs;
    NodeAtividade * atividades;
} Dados;

/************
 * Headers  *
 * *********/
void apagaUC(UC * ucs);
void apagaAtividade(AtividadeUC * atividade);


/************************
 * Metodos Estruturas   *
 ***********************/

//Inicia valores de UC
void iniciaUC(UC * ucs){
    ucs->numero = 0;
    ucs->nome = NULL;
    ucs->atividades = NULL;
}

//Inicia valores de Atividade
void iniciaAtividade(AtividadeUC * atividade){
    atividade->idUC=0;
    atividade->inicio=0;
    atividade->fim=0;
    atividade->realizado=0;
    atividade->numSessao=0;
    atividade->nome=NULL;
}

//Inicia valores de Dados
void iniciaDados(Dados * dados){
    dados->ucs = NULL;
    dados->atividades=NULL;
}

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

//Adicionar Elemento no fim da lista
NodeAtividade * adicionarAtividade(NodeAtividade * lista, NodeAtividade * novo){
    if(lista==NULL){
        return novo;
    }
    if(lista->proxima==NULL){
        lista->proxima = novo;
        return lista;
    }
    adicionarAtividade(lista->proxima, novo);
    return lista;
}

//Cria Nó do elemento UC a adicionar na lista
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

//Criar Nó do elemento Atividade
NodeAtividade * criarAtividadeNode(AtividadeUC * atividade){
    NodeAtividade * novo = malloc(sizeof(NodeAtividade));
    if(!novo){
        printf("Erro: Não foi possivel alocar memoria para o elemento da lista");
        exit(1);
    }
    novo->atual = atividade;
    novo->proxima = NULL;
    return novo;
}


//Apagar Lista UC (Libertar Memoria)
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

//Apagar Lista Atividades (Libertar Memoria)
void eliminarAtividades(NodeAtividade * lista){
    if(lista!= NULL){
        if(lista->proxima!= NULL){
            eliminarAtividades(lista->proxima);
        }
        apagaAtividade(lista->atual);
        lista->atual=NULL;
        free(lista);
        lista = NULL;
    }
}

//Procurar UC
UC * procuraUC(NodeUC * lista, int id){
    if(lista==NULL){
        return NULL;
    }
    if(lista->atual!=NULL){
        if(lista->atual->numero == id){
            return lista->atual;
        } else{
            procuraUC(lista->proxima, id);
        }
    }
    return NULL;
}


/************************
 *  Funções Auxiliares  *
 * *********************/

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
    if(palavra){
        removerEspacosInit(palavra);
        removerEspacoEnd(palavra);
    }
    
}

//Liberta Memoria das Unidades Curriculares
void apagaUC(UC * ucs){
    if(ucs != NULL){
        free(ucs->nome);
        ucs->nome=NULL;
        eliminarAtividades(ucs->atividades);
        ucs->atividades=NULL;
        free(ucs);
        ucs = NULL;
    }
}

//Liberta Memoria das atividade
void apagaAtividade(AtividadeUC * atividade){
    if(atividade != NULL){
        free(atividade->nome);
        atividade->nome=NULL;
        free(atividade);
        atividade=NULL;
    }
}

//Liberta a memoria total
void libertarMemoria(Dados * dados){
    eliminarUCs(dados->ucs);
    eliminarAtividades(dados->atividades);
    free(dados);
    dados=NULL;
}

//Cria Unidades Curriculares
UC * criaUC(char * linha){  
    UC * unidadeCurricular = malloc(sizeof(UC));
    iniciaUC(unidadeCurricular);
    unidadeCurricular->nome= malloc(STR * sizeof(char));
    if(!unidadeCurricular || !unidadeCurricular->nome){
        printf("Erro: Não foi possivel alocar memoria para a unidade curricular");
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

//Cria Atividades
AtividadeUC * criaAtividade(char * linha){
    AtividadeUC * atividade = malloc(sizeof(AtividadeUC));
    iniciaAtividade(atividade);
    atividade->nome = malloc(STR * sizeof(char));
    if(!atividade || !atividade->nome){
        printf("Erro: Não foi possivel alocar memoria para a atividade");
        exit(1);
    }
    char * parte;
    parte = strtok(linha,"-");
    atividade->idUC = atoi(parte);
    for(int i=0; i<4; i++){
        parte = strtok(NULL, "-");
        switch (i){
            case 0:
                atividade->inicio = atoi(parte);
                break;
            case 1:
                atividade->fim = atoi(parte);
                break;
            case 2:
                atividade->realizado = atoi(parte);
                break;
            case 3:
                atividade->numSessao = atoi(parte);
                break;
        }
    }
    parte = strtok(NULL, "\n");
    removerEspacos(parte);
    strcpy(atividade->nome, parte);
    return atividade;
}

//Ler Ficheiro de Configurações
Dados  * lerFicheiro(char * ficheiro){
    Dados  * informacao=malloc(sizeof(Dados));
    iniciaDados(informacao);
    FILE * f;
    int linhaVazia=0;
    f = fopen(ficheiro, "r");
    if(!f){
        printf("Erro: Não foi possivel abrir o ficheiro!");
        exit(1);
    }
    char linha[STR];
    while(fgets(linha,STR,f)!=NULL){
        if(!strcmp(linha,"\n"))
            linhaVazia++;
        else {
            if(linhaVazia==0){
                informacao->ucs = adicionarNode(informacao->ucs, criarNodeUC(criaUC(linha)));
            } else if(linhaVazia==1){
                AtividadeUC * atividade = criaAtividade(linha);
                UC * uc = NULL;
                if((uc=procuraUC(informacao->ucs, atividade->idUC))!= NULL){
                    uc->atividades = adicionarAtividade(uc->atividades,criarAtividadeNode(atividade));
                } else{
                    informacao->atividades= adicionarAtividade(informacao->atividades,criarAtividadeNode(atividade));
                }
                //criaAtividade(linha,listaUnidades);
            } else{
                printf("r");
            } 
        }
    }
    fclose(f);
    return informacao;
}

//Imprimir UC
void imprimeUC(UC * ucs){
    printf("%d - %s\n", ucs->numero, ucs->nome);
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

void imprime(Dados * dados){

}

/************************
 *  Código Alinea A     *
 * *********************/

//Contar os caracteres do nome das UC da Lista
int contaCaracteres(NodeUC * lista){
    int contar = 0;
    while(lista!= NULL){
        contar += strlen(lista->atual->nome);
        lista = lista->proxima;
    }
    return contar;
}

/************************
 *  Código Alinea B    *
 * *********************/




/************************
 *  Função Principal    *
 * *********************/
int main() {
    //FILE *f=stdin; // ler os dados do stdin
    Dados * lista = NULL;

    //NodeUC  * listaUnidades=NULL;
    lista=lerFicheiro("uc.txt");
    printf("%d ",contaCaracteres(lista->ucs));
    imprimirLista(lista->ucs);
    
    
    libertarMemoria(lista);
    lista=NULL;
}
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
#include <math.h>
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
    float concluido;
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
int contaAtividadesUCsRealizada(NodeUC * ucs);
int contaAtividadesUCsRealizada(NodeUC * ucs);
int contarAtividadesRealizadasUC(Dados * dados);
int contaAtividadesUCs(NodeUC * ucs);
int contaTotalAtividades(Dados * dados);
int contaCaracteres(NodeUC * lista);
void calcularConcluidas(NodeUC * unidades);
int calcularValor(UC * unidade);


/************************
 * Metodos Estruturas   *
 ***********************/

//Inicia valores de UC
void iniciaUC(UC * ucs){
    ucs->numero = 0;
    ucs->concluido=0;
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

//Retorna Tamanho da lista de Atividades
int tamanhoAtividades(NodeAtividade * atividades){
    int contador = 0;
    while(atividades!=NULL){
        if(atividades->atual != NULL){
            contador++;
        }
        atividades = atividades->proxima;
    }
    return contador;
}

//Retorna tamanho da Lista de Unidades
int tamanhoUC(NodeUC * ucs){
    int contador =0;
    while(ucs!=NULL){
        if(ucs->atual != NULL){
            contador++;
        }
        ucs = ucs->proxima;
    }
    return contador;
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

void troca(NodeUC * atual, NodeUC * seguinte){
    UC * temp = atual->atual;
    atual->atual = seguinte->atual;
    seguinte->atual = temp;
}

//Ordena lista Unidades
void ordenarUC(NodeUC * lista){
    NodeUC * atual = NULL;
    for(int i =0; i< tamanhoUC(lista)-1; i++){
        atual = lista;
        while(atual!=NULL && atual->proxima !=NULL){
            if(atual->atual->concluido < atual->proxima->atual->concluido){
                troca(atual, atual->proxima);
            } else if(atual->atual->concluido == atual->proxima->atual->concluido){
                if(atual->atual->numero > atual->proxima->atual->numero){
                    troca(atual, atual->proxima);
                }
            }
            atual = atual->proxima;
        }
    }
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
        }
    }
    return procuraUC(lista->proxima, id);
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
                UC * uc = procuraUC(informacao->ucs, atividade->idUC);
                if(uc!= NULL){
                    uc->atividades = adicionarAtividade(uc->atividades,criarAtividadeNode(atividade));
                } else{
                    informacao->atividades= adicionarAtividade(informacao->atividades,criarAtividadeNode(atividade));
                }
            } else{
                
            } 
        }
    }
    fclose(f);
    calcularConcluidas(informacao->ucs); 
    return informacao;
}

//Imprimir UC
void imprimeUC(UC * ucs){
    printf("%s: UC%d %.1f%% Nota prevista: %d valores\n", ucs->nome, ucs->numero, ucs->concluido, calcularValor(ucs));
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
    printf("%d %d %d %d", contaCaracteres(dados->ucs), contaTotalAtividades(dados)
        , contaAtividadesUCs(dados->ucs), contaAtividadesUCsRealizada(dados->ucs));
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

//Contar Atividades Realizadas por UC
int atividadesUCRealizadas(UC * unidade){
    int contador=0;
    NodeAtividade * atividade= NULL;
    if(unidade!=NULL){
        atividade = unidade->atividades;
        while(atividade!= NULL){
            if(atividade->atual->realizado == 1){
                contador++;
            }
            atividade = atividade->proxima;
        }
    }
    return contador;
}

//Contar Atividades Realizadas em todas UC
int contaAtividadesUCsRealizada(NodeUC * ucs){
    int contador = 0;
    while(ucs!= NULL){
        contador+= atividadesUCRealizadas(ucs->atual);
        ucs = ucs->proxima;
    }
    return contador;
}

//Conta Atividades Realizadas das UC
int contarAtividadesRealizadasUC(Dados * dados){
    int contador = 0;
    contador = contaAtividadesUCsRealizada(dados->ucs);
    return contador;
}

//Conta atividades de uma Unidade
int atividadesUC(UC * uc){
    return tamanhoAtividades(uc->atividades);
}

//Conta Atividades de Todas as Unidades
int contaAtividadesUCs(NodeUC * ucs){
    int contador=0;
    while(ucs!= NULL){
        contador+= atividadesUC(ucs->atual);
        ucs= ucs->proxima;
    }
    return contador;
}

//Conta Total de Atividades
int contaTotalAtividades(Dados * dados){
    int contador =0;
    contador = contaAtividadesUCs(dados->ucs) + tamanhoAtividades(dados->atividades);
    return contador;
}

/************************
 *  Código Alinea C    *
 * *********************/

//Calcula Percentagem Realizada da Unidade
void calcularConcluido(UC * unidade){
    int atv = atividadesUCRealizadas(unidade);
    int real = atividadesUC(unidade);
    float percentagem = ((float)atv / real)*100;
    unidade->concluido = percentagem;
}

//Calcula Percentagens realizadas de todas as unidades
void calcularConcluidas(NodeUC * unidades){
    while(unidades!=NULL){
        calcularConcluido(unidades->atual);
        unidades = unidades->proxima;
    }
}

//Calcula Valor obtido
int calcularValor(UC * unidade){
    int valor=0;
    if(unidade->concluido ==100){
        valor = 20;
    } else if( unidade->concluido==0){
        valor = 0;
    } else if(unidade->concluido ==25){
        valor = 10;
    } else{
        valor = round((((unidade->concluido - 25)/75)*10)+10);
    }
    return valor;
}


/************************
 *  Função Principal    *
 * *********************/
int main() {
    //FILE *f=stdin; // ler os dados do stdin
    Dados * lista = NULL;

    //NodeUC  * listaUnidades=NULL;
    lista=lerFicheiro("uc.txt");
    imprimirLista(lista->ucs);
    ordenarUC(lista->ucs);
    
    imprimirLista(lista->ucs);
    imprime(lista);
    //lista = ordenarPorDisciplina(lista->ucs);
    //imprime(lista);
    libertarMemoria(lista);
    lista=NULL;
}
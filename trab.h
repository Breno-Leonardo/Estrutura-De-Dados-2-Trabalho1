#ifndef  TRAB_H
#define  TRAB_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>
    #define GRAU_MINIMO_INDICE 3
#define FATOR_CONJSEQ 2

#define MIN_CHAVE_REGISTRO (FATOR_CONJSEQ - 1)
#define MAX_CHAVE_REGISTRO (2 * FATOR_CONJSEQ - 1)
#define MIN_CHAVE_INDICE (GRAU_MINIMO_INDICE - 1)
#define MAX_CHAVE_INDICE (2 * GRAU_MINIMO_INDICE - 1)
#define MEIO_INDICE ((MAX_CHAVE_INDICE - 1) / 2)
#define MEIO_REGISTRO ((MAX_CHAVE_REGISTRO - 1) / 2)
typedef struct
{
    long qtd;
    long raiz;
    int init;
} Infos;

typedef struct
{
    char nome[20];
    long chave;
    long idade;
} Registro;

typedef struct
{
    long pos;
    long prox;
    long anterior;
    Registro registros[MAX_CHAVE_REGISTRO];
    long qtdRegistros;
    long conjPai;
    long num;
} ConjuntoRegistros;

typedef struct
{
    long chave;

    long conjEsq;
    long conjDir;
    long regEsq;
    long regDir;

} Indice;

typedef struct
{
    long pos;
    Indice indices[MAX_CHAVE_INDICE];
    long qtdIndices;
    long conjPai;
    long num;
} ConjuntoIndices;
    void insereRegistroNoConjunto(Registro registro, Registro registros[MAX_CHAVE_REGISTRO]);
    void aplicarMudancasConjuntoRegistro(FILE *registros, ConjuntoRegistros conj);
int retornarIndex(long pos, long vetor[1000], long qtdConj);
int retornarIndexRegistro(long pos, long vetor[5000], long con, long qtdConj);
void imprimirEmLargura(FILE *registros, FILE *indices);
void imprimirEmCrescente(FILE *registros, FILE *indices);
void criarConjChavesRegistros(Registro registros[]);
void aplicarMudancasConjuntoIndice(FILE *indices, ConjuntoIndices conj);
void criarConjChavesIndices(Indice indices[]);
void dividirConjuntoRegistro(ConjuntoRegistros conj, FILE *registros, FILE *indices, Registro regNovo);
void inserirRegistro(Registro regNovo, FILE *registros, FILE *indices);
void inserirIndice(long chaveIndice, FILE *registros, FILE *indices, long posEsq, long posDir);
void insereIndiceNoConjunto(Indice indice, Indice indices[MAX_CHAVE_INDICE]);
void criarIndiceZerado(Indice indice);
void dividirConjuntoIndice(ConjuntoIndices conj, FILE *indices, Indice indNovo);
void procurarOndeInserirIndice(FILE *indices, long posConjunto, Indice indice);
void procurarOndeInserirRegistro(Registro regNovo, long posConjunto, FILE *registros, FILE *indices);
void procurarRegistro(long chave, long posConjunto, FILE *registros, FILE *indices, long resp[2]);
void removerRealocando(long chave, Registro registros[MAX_CHAVE_REGISTRO]);
void remover(long chave, FILE *registros, FILE *indices);



#endif

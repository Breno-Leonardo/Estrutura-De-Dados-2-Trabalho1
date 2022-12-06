#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "trab.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void insereRegistroNoConjunto(Registro registro, Registro registros[MAX_CHAVE_REGISTRO])
{

    Registro regAux;
    regAux.chave = -1;
    for (int i = 0; i < MAX_CHAVE_REGISTRO; i++)
    {

        if (registros[i].chave == -1)
        {
            registros[i] = registro;
            return;
        }
        else if ((registros[i].chave > registro.chave) && (registros[i].chave != -1))
        {
            regAux = registros[i];
            registros[i] = registro;
            registro = regAux;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void aplicarMudancasConjuntoRegistro(FILE *registros, ConjuntoRegistros conj)
{
    fseek(registros, conj.pos, SEEK_SET);
    fwrite(&conj, sizeof(ConjuntoRegistros), 1, registros);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
int retornarIndex(long pos, long vetor[1000], long qtdConj)
{
    for (int i = 0; i < qtdConj; i++)
    {
        if (vetor[i] == pos)
            return i + 1;
    }
}
int retornarIndexRegistro(long pos, long vetor[5000], long con, long qtdConj)
{
    for (int i = 0; i < con; i++)
    {
        if (vetor[i] == pos)
            return i + 1 + qtdConj;
    }
}
void imprimirEmLargura(FILE *registros, FILE *indices)
{
    Infos infosRegistros;
    fseek(registros, 0, SEEK_SET);
    fread(&infosRegistros, sizeof(Infos), 1, registros);

    Infos infosIndices;
    fseek(indices, 0, SEEK_SET);
    fread(&infosIndices, sizeof(Infos), 1, indices);
    long vetor[5000];
    long vetorRegistro[5000];
    vetor[0] = infosIndices.raiz;
    ConjuntoIndices conjuntoIndices;
    long tamInicial = 0;
    long tamAtual = 0;
    long qtdConj = 1;
    while (qtdConj != infosIndices.qtd)
    {
        fseek(indices, vetor[tamInicial], SEEK_SET);
        fread(&conjuntoIndices, sizeof(ConjuntoIndices), 1, indices);
        for (int i = 0; i < conjuntoIndices.qtdIndices; i++)
        {
            if (conjuntoIndices.indices[i].conjEsq != -1)
            {
                tamAtual++;
                qtdConj++;
                vetor[tamAtual] = conjuntoIndices.indices[i].conjEsq;
            }
            if ((conjuntoIndices.indices[i].conjDir != -1) && (i == conjuntoIndices.qtdIndices - 1))
            {
                tamAtual++;
                qtdConj++;
                vetor[tamAtual] = conjuntoIndices.indices[i].conjDir;
            }
        }
        tamInicial++;
    }

    ConjuntoRegistros conjRegistro;
    fseek(registros, infosRegistros.raiz, SEEK_SET);
    fread(&conjRegistro, sizeof(ConjuntoRegistros), 1, registros);
    int con = 0;
    while (conjRegistro.prox != -1)
    {
        vetorRegistro[con] = conjRegistro.pos;
        fseek(registros, conjRegistro.prox, SEEK_SET);
        fread(&conjRegistro, sizeof(ConjuntoRegistros), 1, registros);
        con++;
    }
    if (conjRegistro.prox == -1)
    {
        vetorRegistro[con] = conjRegistro.pos;
        con++;
    }

    for (int i = 0; i < qtdConj; i++)
    {
        ConjuntoIndices conjInd;
        fseek(indices, vetor[i], SEEK_SET);
        fread(&conjInd, sizeof(ConjuntoIndices), 1, indices);
        printf("No: %d:", i + 1);

        for (int j = 0; j < conjInd.qtdIndices; j++)
        {

            if ((conjInd.indices[j].conjEsq != -1) && (conjInd.indices[j].conjDir != -1))
            {
                printf(" apontador: %d", retornarIndex(conjInd.indices[j].conjEsq, vetor, qtdConj));
                printf(" chave: %ld", conjInd.indices[j].chave);
                if (j == conjInd.qtdIndices - 1)
                {
                    printf(" apontador: %d", retornarIndex(conjInd.indices[j].conjEsq, vetor, qtdConj));
                }
            }
            else if ((conjInd.indices[j].conjEsq == -1) && (conjInd.indices[j].conjDir == -1))
            {
                printf(" apontador: %d", retornarIndexRegistro(conjInd.indices[j].regEsq, vetorRegistro, con, qtdConj));
                printf(" chave: %ld", conjInd.indices[j].chave);
                if (j == conjInd.qtdIndices - 1)
                {
                    printf(" apontador: %d", retornarIndexRegistro(conjInd.indices[j].regDir, vetorRegistro, con, qtdConj));
                }
            }
        }

        printf("\n");
    }

    conjRegistro;
    fseek(registros, infosRegistros.raiz, SEEK_SET);
    fread(&conjRegistro, sizeof(ConjuntoRegistros), 1, registros);
    con = 0;
    printf("No: %ld:", con + 1 + qtdConj);
    for (int j = 0; j < conjRegistro.qtdRegistros; j++)
    {

        printf(" chave: %ld", conjRegistro.registros[j].chave);
    }
    printf("\n");
    con++;
    while (conjRegistro.prox != -1)
    {
        vetorRegistro[con] = conjRegistro.pos;
        fseek(registros, conjRegistro.prox, SEEK_SET);
        fread(&conjRegistro, sizeof(ConjuntoRegistros), 1, registros);
        printf("No: %ld:", con + 1 + qtdConj);
        for (int j = 0; j < conjRegistro.qtdRegistros; j++)
        {

            printf(" chave: %ld", conjRegistro.registros[j].chave);
        }
        printf("\n");
        con++;
    }
}

void imprimirEmCrescente(FILE *registros, FILE *indices)
{
    Infos infosRegistros;
    fseek(registros, 0, SEEK_SET);
    fread(&infosRegistros, sizeof(Infos), 1, registros);

    long vetorRegistro[5000];

    ConjuntoRegistros conjRegistro;
    fseek(registros, infosRegistros.raiz, SEEK_SET);
    fread(&conjRegistro, sizeof(ConjuntoRegistros), 1, registros);
    int con = 0;
    while (conjRegistro.prox != -1)
    {
        vetorRegistro[con] = conjRegistro.pos;
        fseek(registros, conjRegistro.prox, SEEK_SET);
        fread(&conjRegistro, sizeof(ConjuntoRegistros), 1, registros);
        con++;
    }
    if (conjRegistro.prox == -1)
    {
        vetorRegistro[con] = conjRegistro.pos;
        con++;
    }

    fseek(registros, infosRegistros.raiz, SEEK_SET);
    fread(&conjRegistro, sizeof(ConjuntoRegistros), 1, registros);
    con = 0;
    for (int j = 0; j < conjRegistro.qtdRegistros; j++)
    {

        printf("%ld\n", conjRegistro.registros[j].chave);
    }

    con++;
    while (conjRegistro.prox != -1)
    {
        vetorRegistro[con] = conjRegistro.pos;
        fseek(registros, conjRegistro.prox, SEEK_SET);
        fread(&conjRegistro, sizeof(ConjuntoRegistros), 1, registros);
        for (int j = 0; j < conjRegistro.qtdRegistros; j++)
        {

            printf("%ld\n", conjRegistro.registros[j].chave);
        }

        con++;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void criarConjChavesRegistros(Registro registros[])
{
    for (int i = 0; i < MAX_CHAVE_REGISTRO; i++)
    {
        registros[i].chave = -1;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void aplicarMudancasConjuntoIndice(FILE *indices, ConjuntoIndices conj)
{
    fseek(indices, conj.pos, SEEK_SET);
    fwrite(&conj, sizeof(ConjuntoIndices), 1, indices);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void criarConjChavesIndices(Indice indices[])
{
    for (int i = 0; i < MAX_CHAVE_INDICE; i++)
    {
        indices[i].chave = -1;
        indices[i].regDir = -1;
        indices[i].regEsq = -1;
        indices[i].conjDir = -1;
        indices[i].conjEsq = -1;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void dividirConjuntoRegistro(ConjuntoRegistros conj, FILE *registros, FILE *indices, Registro regNovo)
{

    ConjuntoRegistros novoConjunto;
    criarConjChavesRegistros(novoConjunto.registros);
    long chaveMeio = conj.registros[MEIO_REGISTRO].chave;

    for (int i = MEIO_REGISTRO + 1; i < MAX_CHAVE_REGISTRO; i++)
    {
        novoConjunto.registros[i - (MEIO_REGISTRO + 1)] = conj.registros[i];
        conj.registros[i].chave = -1;
    }

    novoConjunto.prox = conj.prox;
    novoConjunto.anterior = conj.pos;
    fseek(registros, 0, SEEK_END);
    novoConjunto.pos = ftell(registros);
    conj.prox = novoConjunto.pos;
    conj.qtdRegistros = MEIO_REGISTRO + 1;
    novoConjunto.qtdRegistros = MEIO_REGISTRO;
    if (regNovo.chave <= chaveMeio)
    {
        insereRegistroNoConjunto(regNovo, conj.registros);
        conj.qtdRegistros++;
    }
    else
    {
        insereRegistroNoConjunto(regNovo, novoConjunto.registros);
        novoConjunto.qtdRegistros++;
    }
    aplicarMudancasConjuntoRegistro(registros, conj);
    aplicarMudancasConjuntoRegistro(registros, novoConjunto);

    inserirIndice(chaveMeio, registros, indices, conj.pos, novoConjunto.pos);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void inserirRegistro(Registro regNovo, FILE *registros, FILE *indices)
{
    Infos infosRegistros;
    fseek(registros, 0, SEEK_SET);
    fread(&infosRegistros, sizeof(Infos), 1, registros);

    Infos infosIndices;
    fseek(indices, 0, SEEK_SET);
    fread(&infosIndices, sizeof(Infos), 1, indices);

    if (infosIndices.init != 1) // nenhuma chave subiu
    {

        if (infosRegistros.init != 1 || infosRegistros.qtd == 0) // sem nenhum registro
        {

            infosRegistros.qtd = 1;
            infosRegistros.raiz = sizeof(Infos);
            infosRegistros.init = 1;
            fseek(registros, 0, SEEK_SET);
            fwrite(&infosRegistros, sizeof(Infos), 1, registros);

            ConjuntoRegistros novoConjuntoReg;
            novoConjuntoReg.pos = ftell(registros);
            novoConjuntoReg.prox = -1;
            novoConjuntoReg.anterior = -1;
            novoConjuntoReg.conjPai = -1;
            novoConjuntoReg.qtdRegistros = 1;
            criarConjChavesRegistros(novoConjuntoReg.registros);
            insereRegistroNoConjunto(regNovo, novoConjuntoReg.registros);

            aplicarMudancasConjuntoRegistro(registros, novoConjuntoReg);
        }
        else if (infosRegistros.init == 1 && infosRegistros.qtd > 0)
        {
            ConjuntoRegistros conjuntoAtual;
            fseek(registros, infosRegistros.raiz, SEEK_SET);
            fread(&conjuntoAtual, sizeof(ConjuntoRegistros), 1, registros);
            if (conjuntoAtual.qtdRegistros < MAX_CHAVE_REGISTRO)
            {

                insereRegistroNoConjunto(regNovo, conjuntoAtual.registros);
                conjuntoAtual.qtdRegistros++;
                aplicarMudancasConjuntoRegistro(registros, conjuntoAtual);
            }
            else
            {
                dividirConjuntoRegistro(conjuntoAtual, registros, indices, regNovo);
            }
        }
    }
    else if (infosIndices.init == 1)
    { // ja  tem alguma chave

        procurarOndeInserirRegistro(regNovo, infosIndices.raiz, registros, indices);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void inserirIndice(long chaveIndice, FILE *registros, FILE *indices, long posEsq, long posDir)
{
    Infos infosRegistros;
    fseek(registros, 0, SEEK_SET);
    fread(&infosRegistros, sizeof(Infos), 1, registros);

    Infos infosIndices;
    fseek(indices, 0, SEEK_SET);
    fread(&infosIndices, sizeof(Infos), 1, indices);
    ConjuntoRegistros conjRegistroEsq;
    fseek(registros, posEsq, SEEK_SET);
    fread(&conjRegistroEsq, sizeof(ConjuntoRegistros), 1, registros);
    ConjuntoRegistros conjRegistroDir;
    fseek(registros, posDir, SEEK_SET);
    fread(&conjRegistroDir, sizeof(ConjuntoRegistros), 1, registros);

    if (infosIndices.init != 1) // nenhuma chave subiu
    {                           // cria o conjunto raiz dos indices
        infosIndices.init = 1;
        infosIndices.qtd = 1;
        infosIndices.raiz = sizeof(infosIndices);
        fseek(indices, 0, SEEK_SET);
        fwrite(&infosIndices, sizeof(Infos), 1, indices);
        ConjuntoIndices conjInd;
        criarConjChavesIndices(conjInd.indices);

        Indice novoIndice;
        novoIndice.conjEsq = -1;
        novoIndice.conjDir = -1;
        novoIndice.regEsq = posEsq;
        novoIndice.regDir = posDir;
        novoIndice.chave = chaveIndice;

        insereIndiceNoConjunto(novoIndice, conjInd.indices);

        conjInd.pos = infosIndices.raiz;
        conjInd.conjPai = -1;
        conjInd.qtdIndices = 1;

        aplicarMudancasConjuntoIndice(indices, conjInd);
        conjRegistroEsq.conjPai = conjInd.pos;
        conjRegistroDir.conjPai = conjInd.pos;
        aplicarMudancasConjuntoRegistro(registros, conjRegistroEsq);
        aplicarMudancasConjuntoRegistro(registros, conjRegistroDir);
    }
    else if (infosIndices.init == 1)
    { // ja  tem alguma chave

        ConjuntoIndices conjInd;
        fseek(indices, infosIndices.raiz, SEEK_SET);
        fread(&conjInd, sizeof(ConjuntoIndices), 1, indices);
        Indice novoIndice;
        novoIndice.chave = chaveIndice;
        novoIndice.conjEsq = -1;
        novoIndice.conjDir = -1;
        novoIndice.regEsq = posEsq;
        novoIndice.regDir = posDir;

        procurarOndeInserirIndice(indices, conjInd.pos, novoIndice);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void insereIndiceNoConjunto(Indice indice, Indice indices[MAX_CHAVE_INDICE])
{

    Indice Aux;
    Aux.chave = -1;
    for (int i = 0; i < MAX_CHAVE_INDICE; i++)
    {

        if (indices[i].chave == -1)
        {
            indices[i] = indice;

            return;
        }
        else if ((indices[i].chave > indice.chave) && (indices[i].chave != -1))
        {
            Aux = indices[i];
            indices[i] = indice;
            indice = Aux;
            indice.regEsq = indices[i].regDir;
            indice.conjEsq = indices[i].conjDir;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void criarIndiceZerado(Indice indice)
{

    indice.chave = -1;
    indice.regDir = -1;
    indice.regEsq = -1;
    indice.conjDir = -1;
    indice.conjEsq = -1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dividirConjuntoIndice(ConjuntoIndices conj, FILE *indices, Indice indNovo)
{

    Infos infosIndices;
    fseek(indices, 0, SEEK_SET);
    fread(&infosIndices, sizeof(Infos), 1, indices);

    ConjuntoIndices novoConjunto;
    criarConjChavesIndices(novoConjunto.indices);
    fseek(indices, 0, SEEK_END);
    novoConjunto.pos = ftell(indices);

    long chaveMeio = conj.indices[MEIO_INDICE].chave;
    Indice indDoMeio = conj.indices[MEIO_INDICE];
    conj.indices[MEIO_INDICE].chave = -1;
    conj.indices[MEIO_INDICE].regDir = -1;
    conj.indices[MEIO_INDICE].regEsq = -1;
    conj.indices[MEIO_INDICE].conjDir = -1;
    conj.indices[MEIO_INDICE].conjEsq = -1;
    for (int i = MEIO_INDICE + 1; i < MAX_CHAVE_INDICE; i++)
    {
        novoConjunto.indices[i - (MEIO_INDICE + 1)] = conj.indices[i];
        conj.indices[i].chave = -1;
        conj.indices[i].conjEsq = -1;
        conj.indices[i].conjDir = -1;
        conj.indices[i].regEsq = -1;
        conj.indices[i].regDir = -1;
    }
    conj.qtdIndices = (MAX_CHAVE_INDICE - 1) / 2;
    novoConjunto.qtdIndices = (MAX_CHAVE_INDICE - 1) / 2;
    indDoMeio.conjEsq = conj.pos;
    indDoMeio.conjDir = novoConjunto.pos;
    indDoMeio.regDir = -1;
    indDoMeio.regEsq = -1;
    aplicarMudancasConjuntoIndice(indices, conj);
    aplicarMudancasConjuntoIndice(indices, novoConjunto);
    long paiRaiz = -1;
    if (conj.pos != infosIndices.raiz)
    { // nao é a raiz
        // insere no conjPai
        novoConjunto.conjPai = conj.conjPai;
        aplicarMudancasConjuntoIndice(indices, novoConjunto);

        ConjuntoIndices conjuntoPaiAux;
        fseek(indices, conj.conjPai, SEEK_SET);
        fread(&conjuntoPaiAux, sizeof(ConjuntoIndices), 1, indices);
        if (conjuntoPaiAux.qtdIndices < MAX_CHAVE_INDICE)
        {
            conjuntoPaiAux.qtdIndices++;
            infosIndices.qtd += 1;
            insereIndiceNoConjunto(indDoMeio, conjuntoPaiAux.indices);
            aplicarMudancasConjuntoIndice(indices, conjuntoPaiAux);
        }
        else
        {
            paiRaiz = 1;
        }
    }
    else
    {

        // se o conj que foi dividido for a raiz
        ConjuntoIndices conjSobe; // cria um novo conjunto para subir
        criarConjChavesIndices(conjSobe.indices);
        insereIndiceNoConjunto(indDoMeio, conjSobe.indices);
        conjSobe.qtdIndices = 1;
        conjSobe.conjPai = -1;
        fseek(indices, 0, SEEK_END);
        conjSobe.pos = ftell(indices);
        infosIndices.raiz = conjSobe.pos;
        infosIndices.qtd += 2;
        fseek(indices, 0, SEEK_SET);
        fwrite(&infosIndices, sizeof(Infos), 1, indices);
        conj.conjPai = conjSobe.pos;
        novoConjunto.conjPai = conjSobe.pos;
        aplicarMudancasConjuntoIndice(indices, conjSobe);
    }

    if (indNovo.chave <= chaveMeio)
    {
        insereIndiceNoConjunto(indNovo, conj.indices);
        conj.qtdIndices++;
    }
    else
    {
        insereIndiceNoConjunto(indNovo, novoConjunto.indices);
        novoConjunto.qtdIndices++;
    }

    aplicarMudancasConjuntoIndice(indices, conj);
    aplicarMudancasConjuntoIndice(indices, novoConjunto);
    fseek(indices, 0, SEEK_SET);
    fwrite(&infosIndices, sizeof(Infos), 1, indices);

    if (paiRaiz == 1)
    {
        ConjuntoIndices conjuntoPaiAux;
        fseek(indices, conj.conjPai, SEEK_SET);
        fread(&conjuntoPaiAux, sizeof(ConjuntoIndices), 1, indices);
        infosIndices.qtd += 1;
        fseek(indices, 0, SEEK_SET);
        fwrite(&infosIndices, sizeof(Infos), 1, indices);
        dividirConjuntoIndice(conjuntoPaiAux, indices, indDoMeio);
    }
    // procurarOndeInserirIndice(indices,  conj.conjPai, indNovo);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void procurarOndeInserirIndice(FILE *indices, long posConjunto, Indice indice)
{

    Infos infosIndices;
    fseek(indices, 0, SEEK_SET);
    fread(&infosIndices, sizeof(Infos), 1, indices);

    ConjuntoIndices conjuntoIndAtual;
    fseek(indices, posConjunto, SEEK_SET);
    fread(&conjuntoIndAtual, sizeof(ConjuntoIndices), 1, indices);
    for (int i = 0; i < conjuntoIndAtual.qtdIndices; i++)
    {

        // if (conjuntoIndAtual.indices[i].chave == -1)
        // {
        //     conjuntoIndAtual.indices[i] = indice;
        //     conjuntoIndAtual.qtdIndices++;
        //     aplicarMudancasConjuntoIndice(indices, conjuntoIndAtual);

        //     return;
        // }
        if ((conjuntoIndAtual.indices[i].chave < indice.chave) && (i == conjuntoIndAtual.qtdIndices - 1))
        { // maior que a ultima chave
            // if (conjuntoIndAtual.indices[i].conjDir != -1 && (conjuntoIndAtual.pos!=infosIndices.raiz))
            if (conjuntoIndAtual.indices[i].conjDir != -1)
            { // tem conjunto a direita
                procurarOndeInserirIndice(indices, conjuntoIndAtual.indices[i].conjDir, indice);
                return;
            }
            else
            { // nao tem conj a direita e é maior q a ultima chave do conjunto
                if (conjuntoIndAtual.qtdIndices < MAX_CHAVE_INDICE)
                {
                    insereIndiceNoConjunto(indice, conjuntoIndAtual.indices);
                    conjuntoIndAtual.qtdIndices++;
                    aplicarMudancasConjuntoIndice(indices, conjuntoIndAtual);
                    return;
                }
                else
                {
                    // divide conjunto indice
                    dividirConjuntoIndice(conjuntoIndAtual, indices, indice);
                    return;
                }
                return;
            }
        }
        else if (conjuntoIndAtual.indices[i].chave > indice.chave)
        {                                                  // achou uma chave maior
            if (conjuntoIndAtual.indices[i].conjEsq != -1) // se ele tem um conjEsq
            {
                procurarOndeInserirIndice(indices, conjuntoIndAtual.indices[i].conjEsq, indice);
                return;
            }
            else
            { // nao tem conj a esquerda, tem que inserir nesse
                if (conjuntoIndAtual.qtdIndices < MAX_CHAVE_INDICE)
                {
                    insereIndiceNoConjunto(indice, conjuntoIndAtual.indices);
                    conjuntoIndAtual.qtdIndices++;
                    aplicarMudancasConjuntoIndice(indices, conjuntoIndAtual);
                    return;
                }
                else
                {
                    // divide conjunto indice
                    dividirConjuntoIndice(conjuntoIndAtual, indices, indice);
                    return;
                }
            }
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void procurarOndeInserirRegistro(Registro regNovo, long posConjunto, FILE *registros, FILE *indices)
{

    ConjuntoIndices conjuntoIndAtual;
    fseek(indices, posConjunto, SEEK_SET);
    fread(&conjuntoIndAtual, sizeof(ConjuntoIndices), 1, indices);

    for (int i = 0; i < conjuntoIndAtual.qtdIndices; i++)
    {

        if ((conjuntoIndAtual.indices[i].chave < regNovo.chave) && (i == conjuntoIndAtual.qtdIndices - 1))
        { // maior que a ultima chave
            if (conjuntoIndAtual.indices[i].conjDir != -1)
            { // tem conjunto a direita

                procurarOndeInserirRegistro(regNovo, conjuntoIndAtual.indices[i].conjDir, registros, indices);
                return;
            }
            else
            { // nao tem conjIndices a direita e é maior q a ultima chave do conjunto

                // logo é um no folha

                ConjuntoRegistros conjRegistros;
                fseek(registros, conjuntoIndAtual.indices[i].regDir, SEEK_SET);
                fread(&conjRegistros, sizeof(ConjuntoRegistros), 1, registros);

                if (conjRegistros.qtdRegistros < MAX_CHAVE_REGISTRO)
                {

                    insereRegistroNoConjunto(regNovo, conjRegistros.registros);
                    conjRegistros.qtdRegistros++;
                    aplicarMudancasConjuntoRegistro(registros, conjRegistros);
                    return;
                }
                else
                {
                    dividirConjuntoRegistro(conjRegistros, registros, indices, regNovo);
                    return;
                }

                return;
            }
        }
        else if (conjuntoIndAtual.indices[i].chave >= regNovo.chave)
        { // achou uma chave maior
            if (conjuntoIndAtual.indices[i].conjEsq != -1)
            {
                procurarOndeInserirRegistro(regNovo, conjuntoIndAtual.indices[i].conjEsq, registros, indices);
                return;
            }
            else
            { // nao tem conj a esquerda entao é folha
                ConjuntoRegistros conjRegistros;
                fseek(registros, conjuntoIndAtual.indices[i].regEsq, SEEK_SET);
                fread(&conjRegistros, sizeof(ConjuntoRegistros), 1, registros);

                if (conjRegistros.qtdRegistros < MAX_CHAVE_REGISTRO)
                {

                    insereRegistroNoConjunto(regNovo, conjRegistros.registros);
                    conjRegistros.qtdRegistros++;
                    aplicarMudancasConjuntoRegistro(registros, conjRegistros);
                    return;
                }
                else
                {
                    dividirConjuntoRegistro(conjRegistros, registros, indices, regNovo);
                    return;
                }

                return;
            }
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void procurarRegistro(long chave, long posConjunto, FILE *registros, FILE *indices, long resp[2])
{

    Infos infosIndices;
    fseek(indices, 0, SEEK_SET);
    fread(&infosIndices, sizeof(Infos), 1, indices);
    if (infosIndices.init == 1)
    {
        ConjuntoIndices conjuntoIndAtual;
        fseek(indices, posConjunto, SEEK_SET);
        fread(&conjuntoIndAtual, sizeof(ConjuntoIndices), 1, indices);
        ConjuntoRegistros conjuntoRegAtual;
        for (int i = 0; i < conjuntoIndAtual.qtdIndices; i++)
        {

            if ((conjuntoIndAtual.indices[i].chave <= chave) && (i == conjuntoIndAtual.qtdIndices - 1))
            { // maior que a ultima chave
                if (conjuntoIndAtual.indices[i].conjDir != -1)
                { // tem conjunto a direita

                    procurarRegistro(chave, conjuntoIndAtual.indices[i].conjDir, registros, indices, resp);
                    return;
                }
                else
                { // nao tem conjIndices a direita e é maior q a ultima chave do conjunto
                    if (conjuntoIndAtual.indices[i].regDir != -1)
                    {

                        fseek(registros, conjuntoIndAtual.indices[i].regDir, SEEK_SET);
                        fread(&conjuntoRegAtual, sizeof(ConjuntoRegistros), 1, registros);
                        for (int i = 0; i < conjuntoRegAtual.qtdRegistros; i++)
                        {
                            if (conjuntoRegAtual.registros[i].chave == chave)
                            {

                                resp[0] = conjuntoRegAtual.pos;
                                resp[1] = conjuntoIndAtual.pos;
                                return;
                            }
                        }
                    }
                    if (conjuntoIndAtual.indices[i].regEsq != -1)
                    {

                        fseek(registros, conjuntoIndAtual.indices[i].regEsq, SEEK_SET);
                        fread(&conjuntoRegAtual, sizeof(ConjuntoRegistros), 1, registros);
                        for (int i = 0; i < conjuntoRegAtual.qtdRegistros; i++)
                        {
                            if (conjuntoRegAtual.registros[i].chave == chave)
                            {

                                resp[0] = conjuntoRegAtual.pos;
                                resp[1] = conjuntoIndAtual.pos;
                                return;
                            }
                        }
                    }
                }
            }
            else if (conjuntoIndAtual.indices[i].chave >= chave)
            { // achou uma chave maior
                if (conjuntoIndAtual.indices[i].conjEsq != -1)
                {
                    procurarRegistro(chave, conjuntoIndAtual.indices[i].conjEsq, registros, indices, resp);
                    return;
                }
                else
                {

                    if (conjuntoIndAtual.indices[i - 1].regEsq != -1 && (i > 0))
                    {
                        ConjuntoRegistros conjuntoRegAtual;
                        fseek(registros, conjuntoIndAtual.indices[i - 1].regEsq, SEEK_SET);
                        fread(&conjuntoRegAtual, sizeof(ConjuntoRegistros), 1, registros);
                        for (int i = 0; i < conjuntoRegAtual.qtdRegistros; i++)
                        {

                            if (conjuntoRegAtual.registros[i].chave == chave)
                            {

                                resp[0] = conjuntoRegAtual.pos;
                                resp[1] = conjuntoIndAtual.pos;
                                return;
                            }
                        }
                    }
                    if (conjuntoIndAtual.indices[i].regEsq != -1)
                    {

                        ConjuntoRegistros conjuntoRegAtual;
                        fseek(registros, conjuntoIndAtual.indices[i].regEsq, SEEK_SET);
                        fread(&conjuntoRegAtual, sizeof(ConjuntoRegistros), 1, registros);
                        for (int i = 0; i < conjuntoRegAtual.qtdRegistros; i++)
                        {

                            if (conjuntoRegAtual.registros[i].chave == chave)
                            {

                                resp[0] = conjuntoRegAtual.pos;
                                resp[1] = conjuntoIndAtual.pos;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        Infos infosRegistros;
        fseek(registros, 0, SEEK_SET);
        fread(&infosRegistros, sizeof(Infos), 1, registros);
        if (infosRegistros.init == 1)
        {
            ConjuntoRegistros conjuntoRegAtual;
            fseek(registros, infosRegistros.raiz, SEEK_SET);
            fread(&conjuntoRegAtual, sizeof(ConjuntoRegistros), 1, registros);

            for (int i = 0; i < conjuntoRegAtual.qtdRegistros; i++)
            {
                if (conjuntoRegAtual.registros[i].chave == chave)
                {
                    resp[0] = conjuntoRegAtual.pos;
                    resp[1] = -1;
                    return;
                }
            }
            while (conjuntoRegAtual.prox != -1)
            {
                fseek(registros, conjuntoRegAtual.prox, SEEK_SET);
                fread(&conjuntoRegAtual, sizeof(ConjuntoRegistros), 1, registros);
                for (int i = 0; i < conjuntoRegAtual.qtdRegistros; i++)
                {
                    if (conjuntoRegAtual.registros[i].chave == chave)
                    {
                        resp[0] = conjuntoRegAtual.pos;
                        resp[1] = -1;
                        return;
                    }
                }
            }
        }
    }
    resp[0] = -1;
    resp[1] = -1;
    return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void removerRealocando(long chave, Registro registros[MAX_CHAVE_REGISTRO])
{

    for (int i = 0; i < MAX_CHAVE_REGISTRO; i++)
    {
        if (registros[i].chave == chave)
        {
            for (int j = i; j < MAX_CHAVE_REGISTRO - 1; j++)
            {
                registros[j] = registros[j + 1];
            }
            registros[MAX_CHAVE_REGISTRO - 1].chave = -1;
            return;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void remover(long chave, FILE *registros, FILE *indices)
{
    Infos infosRegistros;
    fseek(registros, 0, SEEK_SET);
    fread(&infosRegistros, sizeof(Infos), 1, registros);

    Infos infosIndices;
    fseek(indices, 0, SEEK_SET);
    fread(&infosIndices, sizeof(Infos), 1, indices);

    long resp[2] = {-1, -1};
    procurarRegistro(chave, infosIndices.raiz, registros, indices, resp);

    if (resp[0] == -1)
    {
        printf("chave nao encontrada: %ld\n", chave);
    }
    else
    {
        printf("chave removida com sucesso: %ld\n", chave);
        ConjuntoRegistros conjRegistro;
        fseek(registros, resp[0], SEEK_SET);
        fread(&conjRegistro, sizeof(ConjuntoRegistros), 1, registros);

        if (conjRegistro.qtdRegistros > MIN_CHAVE_REGISTRO)
        {
            removerRealocando(chave, conjRegistro.registros);
            conjRegistro.qtdRegistros--;
            aplicarMudancasConjuntoRegistro(registros, conjRegistro);
        }
        else
        {

            if ((conjRegistro.anterior != -1) && (conjRegistro.prox == -1)) // esq
            {
                ConjuntoRegistros conjRegistroEsq;
                fseek(registros, conjRegistro.anterior, SEEK_SET);
                fread(&conjRegistroEsq, sizeof(ConjuntoRegistros), 1, registros);
                ConjuntoIndices conjInd;
                fseek(indices, resp[1], SEEK_SET);
                fread(&conjInd, sizeof(ConjuntoIndices), 1, indices);
                removerRealocando(chave, conjRegistro.registros);
                aplicarMudancasConjuntoRegistro(registros, conjRegistro);
                if (conjRegistroEsq.qtdRegistros > MIN_CHAVE_REGISTRO)
                { // ele da uma chave
                    Registro regAux = conjRegistroEsq.registros[conjRegistroEsq.qtdRegistros - 1];

                    removerRealocando(regAux.chave, conjRegistroEsq.registros);
                    conjRegistroEsq.qtdRegistros--;
                    aplicarMudancasConjuntoRegistro(registros, conjRegistroEsq);
                    int tam = conjInd.qtdIndices;
                    for (int i = 0; i < tam; i++)
                    {
                        if (conjInd.indices[i].regDir == conjRegistro.pos)
                        {
                            conjInd.indices[i].chave = regAux.chave;
                        }
                    }
                    aplicarMudancasConjuntoIndice(indices, conjInd);
                    insereRegistroNoConjunto(regAux, conjRegistro.registros);
                    aplicarMudancasConjuntoRegistro(registros, conjRegistro);
                }
                else
                { // aglutina
                    ConjuntoRegistros conjRegistroEsq;
                    fseek(registros, conjRegistro.anterior, SEEK_SET);
                    fread(&conjRegistroEsq, sizeof(ConjuntoRegistros), 1, registros);
                    ConjuntoIndices conjInd;
                    fseek(indices, resp[1], SEEK_SET);
                    fread(&conjInd, sizeof(ConjuntoIndices), 1, indices);
                    removerRealocando(chave, conjRegistro.registros);
                    conjRegistro.qtdRegistros--;
                    aplicarMudancasConjuntoRegistro(registros, conjRegistro);
                    if (infosRegistros.raiz == conjRegistro.pos)
                    {
                        infosRegistros.raiz = conjRegistroEsq.pos;
                        fseek(registros, 0, SEEK_SET);
                        fwrite(&infosRegistros, sizeof(Infos), 1, registros);
                    }

                    int tam = conjInd.qtdIndices;
                    for (int i = 0; i < tam; i++)
                    {
                        if (conjRegistro.registros[i].chave != -1)
                        {
                            insereRegistroNoConjunto(conjRegistro.registros[i], conjRegistroEsq.registros);
                            conjRegistroEsq.qtdRegistros++;
                        }
                    }
                    conjRegistroEsq.prox = conjRegistro.prox;
                    aplicarMudancasConjuntoRegistro(registros, conjRegistroEsq);
                    if (conjInd.pos == infosIndices.raiz)
                    { // se for raiz
                        for (int i = 0; i < conjInd.qtdIndices; i++)
                        {
                            conjInd.indices[i].chave = -1;
                            for (int j = i; j < conjInd.qtdIndices - 1; j++)
                            {
                                conjInd.indices[j] = conjInd.indices[j + 1];
                            }
                        }

                        conjInd.qtdIndices--;
                        aplicarMudancasConjuntoIndice(indices, conjInd);
                    }
                    else
                    {
                        // remover indice
                    }
                    // removerIndice();diminuir qtd
                }
            }

            else if ((conjRegistro.anterior == -1) && (conjRegistro.prox != -1)) // dir
            {
                ConjuntoRegistros conjRegistroDir;
                fseek(registros, conjRegistro.prox, SEEK_SET);
                fread(&conjRegistroDir, sizeof(ConjuntoRegistros), 1, registros);
                ConjuntoIndices conjInd;
                fseek(indices, resp[1], SEEK_SET);
                fread(&conjInd, sizeof(ConjuntoIndices), 1, indices);
                removerRealocando(chave, conjRegistro.registros);
                aplicarMudancasConjuntoRegistro(registros, conjRegistro);
                if (conjRegistroDir.qtdRegistros > MIN_CHAVE_REGISTRO)
                { // ele da uma chave
                    Registro regAux = conjRegistroDir.registros[0];

                    removerRealocando(regAux.chave, conjRegistroDir.registros);
                    conjRegistroDir.qtdRegistros--;
                    aplicarMudancasConjuntoRegistro(registros, conjRegistroDir);

                    for (int i = 0; i < conjInd.qtdIndices; i++)
                    {
                        if (conjInd.indices[i].regEsq == conjRegistro.pos)
                        {
                            conjInd.indices[i].chave = regAux.chave;
                        }
                    }
                    aplicarMudancasConjuntoIndice(indices, conjInd);
                    insereRegistroNoConjunto(regAux, conjRegistro.registros);
                    aplicarMudancasConjuntoRegistro(registros, conjRegistro);
                }
                else
                { // aglutina
                    ConjuntoRegistros conjRegistroDir;
                    fseek(registros, conjRegistro.prox, SEEK_SET);
                    fread(&conjRegistroDir, sizeof(ConjuntoRegistros), 1, registros);
                    ConjuntoIndices conjInd;
                    fseek(indices, resp[1], SEEK_SET);
                    fread(&conjInd, sizeof(ConjuntoIndices), 1, indices);
                    removerRealocando(chave, conjRegistro.registros);
                    conjRegistro.qtdRegistros--;
                    if (infosRegistros.raiz == conjRegistro.pos)
                    {
                        infosRegistros.raiz = conjRegistroDir.pos;
                        fseek(registros, 0, SEEK_SET);
                        fwrite(&infosRegistros, sizeof(Infos), 1, registros);
                    }

                    aplicarMudancasConjuntoRegistro(registros, conjRegistro);
                    int tam = conjRegistro.qtdRegistros;
                    for (int i = 0; i < tam; i++)
                    {
                        if (conjRegistro.registros[i].chave != -1)
                        {
                            insereRegistroNoConjunto(conjRegistro.registros[i], conjRegistroDir.registros);
                            conjRegistroDir.qtdRegistros++;
                        }
                    }
                    conjRegistroDir.anterior = conjRegistro.anterior;

                    aplicarMudancasConjuntoRegistro(registros, conjRegistroDir);
                    if (conjInd.pos == infosIndices.raiz)
                    { // se for raiz
                        for (int i = 0; i < conjInd.qtdIndices; i++)
                        {
                            if (conjInd.indices[i].regEsq == conjRegistro.pos)
                            {
                                conjInd.indices[i].chave = -1;
                                for (int j = i; j < conjInd.qtdIndices - 1; j++)
                                {
                                    conjInd.indices[j] = conjInd.indices[j + 1];
                                }
                            }
                        }
                        conjInd.qtdIndices--;
                        aplicarMudancasConjuntoIndice(indices, conjInd);
                    }
                    else
                    {
                        // remover indice
                    }
                }
            }

            else if ((conjRegistro.anterior != -1) && (conjRegistro.prox != -1))
            {
                ConjuntoRegistros conjRegistroDir;
                fseek(registros, conjRegistro.prox, SEEK_SET);
                fread(&conjRegistroDir, sizeof(ConjuntoRegistros), 1, registros);

                ConjuntoRegistros conjRegistroEsq;
                fseek(registros, conjRegistro.anterior, SEEK_SET);
                fread(&conjRegistroEsq, sizeof(ConjuntoRegistros), 1, registros);

                if (conjRegistroDir.qtdRegistros > conjRegistroEsq.qtdRegistros)
                {

                    ConjuntoRegistros conjRegistroEsq;
                    fseek(registros, conjRegistro.anterior, SEEK_SET);
                    fread(&conjRegistroEsq, sizeof(ConjuntoRegistros), 1, registros);
                    ConjuntoIndices conjInd;
                    fseek(indices, resp[1], SEEK_SET);
                    fread(&conjInd, sizeof(ConjuntoIndices), 1, indices);
                    removerRealocando(chave, conjRegistro.registros);
                    aplicarMudancasConjuntoRegistro(registros, conjRegistro);
                    if (conjRegistroEsq.qtdRegistros > MIN_CHAVE_REGISTRO)
                    { // ele da uma chave
                        Registro regAux = conjRegistroEsq.registros[conjRegistroEsq.qtdRegistros - 1];

                        removerRealocando(regAux.chave, conjRegistroEsq.registros);
                        conjRegistroEsq.qtdRegistros--;
                        aplicarMudancasConjuntoRegistro(registros, conjRegistroEsq);
                        int tam = conjInd.qtdIndices;
                        for (int i = 0; i < tam; i++)
                        {
                            if (conjInd.indices[i].regDir == conjRegistro.pos)
                            {
                                conjInd.indices[i].chave = regAux.chave;
                            }
                        }
                        aplicarMudancasConjuntoIndice(indices, conjInd);
                        insereRegistroNoConjunto(regAux, conjRegistro.registros);
                        aplicarMudancasConjuntoRegistro(registros, conjRegistro);
                    }
                    else
                    { // aglutina
                        ConjuntoRegistros conjRegistroEsq;
                        fseek(registros, conjRegistro.anterior, SEEK_SET);
                        fread(&conjRegistroEsq, sizeof(ConjuntoRegistros), 1, registros);
                        ConjuntoIndices conjInd;
                        fseek(indices, resp[1], SEEK_SET);
                        fread(&conjInd, sizeof(ConjuntoIndices), 1, indices);
                        removerRealocando(chave, conjRegistro.registros);
                        conjRegistro.qtdRegistros--;
                        aplicarMudancasConjuntoRegistro(registros, conjRegistro);
                        if (infosRegistros.raiz == conjRegistro.pos)
                        {
                            infosRegistros.raiz = conjRegistroEsq.pos;
                            fseek(registros, 0, SEEK_SET);
                            fwrite(&infosRegistros, sizeof(Infos), 1, registros);
                        }

                        int tam = conjInd.qtdIndices;
                        for (int i = 0; i < tam; i++)
                        {
                            if (conjRegistro.registros[i].chave != -1)
                            {
                                insereRegistroNoConjunto(conjRegistro.registros[i], conjRegistroEsq.registros);
                                conjRegistroEsq.qtdRegistros++;
                            }
                        }
                        conjRegistroEsq.prox = conjRegistro.prox;
                        aplicarMudancasConjuntoRegistro(registros, conjRegistroEsq);
                        if (conjInd.pos == infosIndices.raiz)
                        { // se for raiz
                            for (int i = 0; i < conjInd.qtdIndices; i++)
                            {
                                conjInd.indices[i].chave = -1;
                                for (int j = i; j < conjInd.qtdIndices - 1; j++)
                                {
                                    conjInd.indices[j] = conjInd.indices[j + 1];
                                }
                            }

                            conjInd.qtdIndices--;
                            aplicarMudancasConjuntoIndice(indices, conjInd);
                        }
                        else
                        {
                            // remover indice
                        }
                        // removerIndice();diminuir qtd
                    }
                }

                else
                {
                    ConjuntoRegistros conjRegistroEsq;
                    fseek(registros, conjRegistro.anterior, SEEK_SET);
                    fread(&conjRegistroEsq, sizeof(ConjuntoRegistros), 1, registros);
                    ConjuntoIndices conjInd;
                    fseek(indices, resp[1], SEEK_SET);
                    fread(&conjInd, sizeof(ConjuntoIndices), 1, indices);
                    removerRealocando(chave, conjRegistro.registros);
                    aplicarMudancasConjuntoRegistro(registros, conjRegistro);
                    if (conjRegistroEsq.qtdRegistros > MIN_CHAVE_REGISTRO)
                    { // ele da uma chave
                        Registro regAux = conjRegistroEsq.registros[conjRegistroEsq.qtdRegistros - 1];

                        removerRealocando(regAux.chave, conjRegistroEsq.registros);
                        conjRegistroEsq.qtdRegistros--;
                        aplicarMudancasConjuntoRegistro(registros, conjRegistroEsq);
                        int tam = conjInd.qtdIndices;
                        for (int i = 0; i < tam; i++)
                        {
                            if (conjInd.indices[i].regDir == conjRegistro.pos)
                            {
                                conjInd.indices[i].chave = regAux.chave;
                            }
                        }
                        aplicarMudancasConjuntoIndice(indices, conjInd);
                        insereRegistroNoConjunto(regAux, conjRegistro.registros);
                        aplicarMudancasConjuntoRegistro(registros, conjRegistro);
                    }
                    else
                    { // aglutina
                        ConjuntoRegistros conjRegistroEsq;
                        fseek(registros, conjRegistro.anterior, SEEK_SET);
                        fread(&conjRegistroEsq, sizeof(ConjuntoRegistros), 1, registros);
                        ConjuntoIndices conjInd;
                        fseek(indices, resp[1], SEEK_SET);
                        fread(&conjInd, sizeof(ConjuntoIndices), 1, indices);
                        removerRealocando(chave, conjRegistro.registros);
                        conjRegistro.qtdRegistros--;
                        aplicarMudancasConjuntoRegistro(registros, conjRegistro);
                        if (infosRegistros.raiz == conjRegistro.pos)
                        {
                            infosRegistros.raiz = conjRegistroEsq.pos;
                            fseek(registros, 0, SEEK_SET);
                            fwrite(&infosRegistros, sizeof(Infos), 1, registros);
                        }

                        int tam = conjInd.qtdIndices;
                        for (int i = 0; i < tam; i++)
                        {
                            if (conjRegistro.registros[i].chave != -1)
                            {
                                insereRegistroNoConjunto(conjRegistro.registros[i], conjRegistroEsq.registros);
                                conjRegistroEsq.qtdRegistros++;
                            }
                        }
                        conjRegistroEsq.prox = conjRegistro.prox;
                        aplicarMudancasConjuntoRegistro(registros, conjRegistroEsq);
                        if (conjInd.pos == infosIndices.raiz)
                        { // se for raiz
                            for (int i = 0; i < conjInd.qtdIndices; i++)
                            {
                                conjInd.indices[i].chave = -1;
                                for (int j = i; j < conjInd.qtdIndices - 1; j++)
                                {
                                    conjInd.indices[j] = conjInd.indices[j + 1];
                                }
                            }

                            conjInd.qtdIndices--;
                            aplicarMudancasConjuntoIndice(indices, conjInd);
                        }
                        else
                        {
                            // remover indice
                        }
                        // removerIndice();diminuir qtd
                    }
                }
            }
        }
    }
}

int main()
{
    FILE *indices = fopen("arvore", "rb+");
    FILE *registros = fopen("registros", "rb+");
    ConjuntoIndices indicesAtuais;
    ConjuntoRegistros registrosAtuais;
    Registro regAtual;
    Indice indAtual;
    char comando;
    Infos infosIndices;

    while ((comando != 'e'))
    {

        scanf("%c", &comando);
        if (comando == 'e')
        {
            break;
        }
        else if (comando == 'i')
        {
            fflush(stdin);
            Registro regNovo;

            scanf("%ld", &regNovo.chave);
            fflush(stdin);

            fflush(stdin);
            scanf("%s", regNovo.nome);

            scanf("%ld", &regNovo.idade);
            fflush(stdin);
            fseek(indices, 0, SEEK_SET);
            fread(&infosIndices, sizeof(Infos), 1, indices);

            long resp[2] = {-1, -1};
            procurarRegistro(regNovo.chave, infosIndices.raiz, registros, indices, resp);
            if (resp[0] == -1)
            {
                inserirRegistro(regNovo, registros, indices);
                printf("insercao com sucesso: %ld\n", regNovo.chave);
            }
            else
            {
                printf("chave ja existente: %ld\n", regNovo.chave);
            }
        }
        else if (comando == 'p')
        {
            fseek(indices, 0, SEEK_SET);
            fread(&infosIndices, sizeof(Infos), 1, indices);
            if (infosIndices.init == 1)
                imprimirEmLargura(registros, indices);
            fflush(stdin);
        }
        else if (comando == 'o')
        {
            Infos infosRegistros;
            fseek(registros, 0, SEEK_SET);
            fread(&infosRegistros, sizeof(Infos), 1, registros);
            if (infosRegistros.init != 1)
            {
                printf("árvore vazia\n");
            }
            else
                imprimirEmCrescente(registros, indices);
            fflush(stdin);
        }
        else if (comando == 'c')
        {
            fseek(indices, 0, SEEK_SET);
            fread(&infosIndices, sizeof(Infos), 1, indices);
            fflush(stdin);
            Registro regNovo;

            scanf("%ld", &regNovo.chave);
            fflush(stdin);

            long resp[2] = {-1, -1};
            procurarRegistro(regNovo.chave, infosIndices.raiz, registros, indices, resp);
            if (resp[0] == -1)
            {
                printf("chave nao encontrada: %ld\n", regNovo.chave);
            }
            else
            {
                ConjuntoRegistros conj;
                fseek(registros, resp[0], SEEK_SET);
                fread(&conj, sizeof(ConjuntoRegistros), 1, registros);
                Registro reg;
                for (int i = 0; i < conj.qtdRegistros; i++)
                {
                    if (conj.registros[i].chave == regNovo.chave)
                    {

                        printf("chave: %ld\n", conj.registros[i].chave);
                        printf("nome: %s\n", conj.registros[i].nome);
                        printf("idade: %ld\n", conj.registros[i].idade);
                    }
                }
            }
        }
        else if (comando == 'r')
        {
            fseek(indices, 0, SEEK_SET);
            fread(&infosIndices, sizeof(Infos), 1, indices);
            fflush(stdin);
            Registro regNovo;
            scanf("%ld", &regNovo.chave);
            fflush(stdin);

            remover(regNovo.chave, registros, indices);
        }
    }
    fclose(indices);
    fclose(registros);

    return 0;
}

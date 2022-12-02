#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct registro{
    char nome[28];
    int idade;   
} registro;

//Descrição do código:
// compara os três registros e retorna alfabeticamente o menor, se forem iguais, retorna o de menor idade
registro * comparaTresReg(registro * Reg1, registro * Reg2, registro * Reg3) {
    registro * menor;
    if(strcmp(Reg1->nome, Reg2->nome) < 0){
        if(strcmp(Reg1->nome, Reg3->nome) < 0){
            menor = Reg1;
        }else{
            menor = Reg3;
        }
    }else{
        if(strcmp(Reg2->nome, Reg3->nome) < 0){
            menor = Reg2;
        }else{
            menor = Reg3;
        }
    }
    return menor;
}

//Descrição do código:
//// compara os dois registros e retorna alfabeticamente o menor, se forem iguais, retorna o de menor idade
registro * comparaDoisReg(registro * Reg1, registro * Reg2){
    registro * menor;
    int valor = strcmp(Reg1->nome, Reg2->nome);
    if(valor != 0){
        if(valor < 0){
            menor = Reg1;
        }else{
            menor = Reg2;
        }
    }
    else{
        if(Reg2->idade < Reg1->idade){
            menor = Reg2;
        }else{
            menor = Reg1;
        }
    }
    return menor;
}

//Descrição do código:
//Utiliza-se de uma página auxiliar para armazenar os registros ordenados
//Apos a ordenação, a página auxiliar é escrita no arquivo de dados
//A função retorna o arquivo de dados ordenado, que é então utilizado na main para a ordenação final na main
FILE * mergeSortExterno(registro * Pagina, int QuantidadePorPagina, registro * PaginaAux, char * NomeArquivo){

    int Dobras = QuantidadePorPagina;
    int QuantidadePorDobra = 1;
    int PrimeiraDobra = 1;

    FILE * SaidaPagina = fopen(NomeArquivo, "w+b");

    while(Dobras > 0){
        if(PrimeiraDobra){
                PrimeiraDobra = 0;
        }
        else{
                //Caso não seja a primeira dobra, a pagina auxiliar é lida e escrito na página principal
                for(int i = 0; i < QuantidadePorPagina; i++) {
                    Pagina[i] = PaginaAux[i];
                }
        }


        int index = 0;
        for(int DobraAtual = 0; DobraAtual < Dobras; DobraAtual += 2){

            //variaveis para armazenar os indices de cada dobra
            int Posicao1 = DobraAtual * QuantidadePorDobra;
            int PosicaoFinal1 = Posicao1 + QuantidadePorDobra;
            int Posicao2 = PosicaoFinal1;
            int PosicaoFinal2 = Posicao2 + QuantidadePorDobra;

            while(Posicao1 < PosicaoFinal1 && Posicao2 < PosicaoFinal2 && Posicao2 < QuantidadePorPagina){
                registro * menor = comparaDoisReg(&Pagina[Posicao1], &Pagina[Posicao2]);

                if(menor == &Pagina[Posicao1]){
                    Posicao1++;
                    PaginaAux[index] = *menor;
                    index++;
                }else{
                    Posicao2++;
                    PaginaAux[index] = *menor;
                    index++;
                }
            }

            while(Posicao1 < PosicaoFinal1){
                PaginaAux[index] = Pagina[Posicao1];
                index++;
                Posicao1++;
            }

            while(Posicao2 < PosicaoFinal2){
                PaginaAux[index] = Pagina[Posicao2];
                index++;
                Posicao2++;
            }
        }

        QuantidadePorDobra *= 2;
        Dobras = Dobras / 2;
    }

    //Le a pagina e escreve no arquivo de pagina
    fwrite(PaginaAux, sizeof(registro), QuantidadePorPagina, SaidaPagina);
    fseek(SaidaPagina, 0, SEEK_SET);
    return SaidaPagina;
}

//Descrição do código:
//Recebendo as paginas ordenadas, a função faz a ordenação final no arquivo de saida, utilizando o metodo   merge sort
void juntarPaginasEmSaida(FILE * saida, registro * PaginaA, registro * PaginaB, registro * PaginaC, int QtdA, int QtcB, int QtdC){
    int i = 0, j = 0, k = 0;
    bool fimA = false, fimB = false, fimC = false;
    registro * menor;
    while(!(fimA || fimB || fimC)) {
        menor = comparaTresReg(&PaginaA[i], &PaginaB[j], &PaginaC[k]);
        fwrite(menor, sizeof(registro), 1, saida);
        if(menor == &PaginaA[i]){
            i++;
            if(i == QtdA)
                fimA = true;
        }else if(menor == &PaginaB[j]){
            j++;
            if(j == QtcB)
                fimB = true;
        }else{
            k++;
            if(k == QtdC)
                fimC = true;
        }
    }

    if(fimA){
        while(!(fimB || fimC)){
            menor = comparaDoisReg(&PaginaB[j], &PaginaC[k]);
            fwrite(menor, sizeof(registro), 1, saida);
            if(menor == &PaginaB[j]){
                j++;
                if(j == QtcB)
                    fimB = true;
            }else{
                k++;
                if(k == QtdC)
                    fimC = true;
            }
        }
    }else if(fimB){
        while(!(fimA || fimC)){
            menor = comparaDoisReg(&PaginaA[i], &PaginaC[k]);
            fwrite(menor, sizeof(registro), 1, saida);
            if(menor == &PaginaA[i]){
                i++;
                if(i == QtdA)
                    fimA = true;
            }else{
                k++;
                if(k == QtdC)
                    fimC = true;
            }
        }
    }else{
        while(!(fimA || fimB)){
            menor = comparaDoisReg(&PaginaA[i], &PaginaB[j]);
            fwrite(menor, sizeof(registro), 1, saida);
            if(menor == &PaginaA[i]){
                i++;
                if(i == QtdA)
                    fimA = true;
            }else{
                j++;
                if(j == QtcB)
                    fimB = true;
            }
        }
    }

    if(!fimA){
        while(i < QtdA){
            fwrite(&PaginaA[i], sizeof(registro), 1, saida);
            i++;
    }
    }

    if(!fimB){
        while(j < QtcB){
            fwrite(&PaginaB[j], sizeof(registro), 1, saida);
            j++;
        }
    }

    if(!fimC){
        while(k < QtdC){
            fwrite(&PaginaC[k], sizeof(registro), 1, saida);
            k++;
        }
    }
}

int main(int argc, char * argv[])
{
    if(argc != 4){
        return 0;
    }

    registro * paginaA = NULL;
    registro * paginaB = NULL;
    registro * paginaC = NULL;

    //entrada em binario
    FILE * entrada = fopen(argv[1], "rb");
    //saida em texto
    FILE * saida = fopen(argv[2], "wb");
    
    int QuantidadePorPagina = atoi(argv[3]);


    //alocar memoria para as paginas
    paginaA = (registro *) malloc(QuantidadePorPagina * sizeof(registro));
    paginaB = (registro *) malloc(QuantidadePorPagina * sizeof(registro));
    paginaC = (registro *) malloc(QuantidadePorPagina * sizeof(registro));

    //variaveis para controle de leitura
    int QTD_PaginaA = 0;
    int QTD_PaginaB = 0;
    int QTD_PaginaC = 0;
    int ret = 0;

    //ler a entrada e colocar nas paginas ordernar no arquivo da pagina ordenada
    for(int i = 0; i < QuantidadePorPagina; i++){
        if(ret = fread(&paginaA[i], sizeof(registro), 1, entrada))
            if(ret > 0)
                QTD_PaginaA++;
        else
            break;
    }
    FILE * PaginaAOrganizada = mergeSortExterno(paginaA, QTD_PaginaA, paginaB, "PaginaA.bin");

    //ler a entrada e colocar nas paginas ordernar no arquivo da pagina ordenada
    for(int i = 0; i < QuantidadePorPagina; i++){

        if(ret = fread(&paginaA[i], sizeof(registro), 1, entrada))
            if(ret > 0)
                QTD_PaginaB++;
        else
            break;
    }
    FILE * PaginaBOrganizada = mergeSortExterno(paginaA, QTD_PaginaB, paginaB, "PaginaB.bin");

    //ler a entrada e colocar nas paginas ordernar no arquivo da pagina ordenada
    for(int i = 0; i < QuantidadePorPagina; i++){

        if(fread(&paginaA[i], sizeof(registro), 1, entrada))
            if(ret > 0)
                QTD_PaginaC++;
        else
            break;
    }
    FILE * PaginaCOrganizada = mergeSortExterno(paginaA, QTD_PaginaC, paginaB, "PaginaC.bin");


    //Leitura das paginas organizadas na memoria
    fread(paginaA, sizeof(registro), QTD_PaginaA, PaginaAOrganizada);
    fread(paginaB, sizeof(registro), QTD_PaginaB, PaginaBOrganizada);
    fread(paginaC, sizeof(registro), QTD_PaginaC, PaginaCOrganizada);

    //Fechando os arquivos, e removendo eles do disco
    fclose(PaginaAOrganizada);
    fclose(PaginaBOrganizada);
    fclose(PaginaCOrganizada);
    remove("PaginaA.bin");
    remove("PaginaB.bin");
    remove("PaginaC.bin");

    //Fazendo a intercalacao das paginas para o arquivo de saida ordenado
    juntarPaginasEmSaida(saida, paginaA, paginaB, paginaC, QTD_PaginaA, QTD_PaginaB, QTD_PaginaC);

    //Fechando os arquivos e liberando a memoria
    fclose(entrada);
    fclose(saida);
    free(paginaA);
    free(paginaB);
    free(paginaC);


    return 0;
}

#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "algoritmo.h"
#include "utils.h"


// Leitura dos parametros ("info.txt") e dos dados do problema 
// Argumentos: Nome do ficheiro com os dados do problema e matriz a preencher com as coordenadas dos pontos
// Devolve a estrutura com os parametros

struct info init_data(int distan[MAXCITIES][MAXCITIES], char *nome_fich)
{
	struct info x;
	FILE *f;
	int i, j;
	char st[200];

	f = fopen("info.txt", "r");
	if(f == NULL)
	{
		printf("Ficheiro de parametros nao existe\n"); 
		exit(1);
	}

	// Leitura dos parametros 
	fscanf(f, " pop: %d", &(x.popsize));
	fscanf(f, " pm_swap: %f", &(x.pm_swap));
	fscanf(f, " pm_ins: %f", &(x.pm_ins));
	fscanf(f, " pm_inv: %f", &(x.pm_inv));
	fscanf(f, " pr: %f", &(x.pr));
	fscanf(f, " t_size: %d", &(x.t_size));
	fscanf(f, " max_gen: %d", &(x.numGenerations));
	fclose(f);

	f=fopen(nome_fich, "r");
	if(!f)
	{
		printf("Ficheiro SOP nao existe\n");
		exit(1);
	}

	// Leitura dados do problema TSP
	for(i=0; i<7; i++)
		fgets(st, 200, f);
	fscanf(f, "%d", &x.numCities);
    printf("Numero de cidades: %d\n\n", x.numCities);
	if(x.numCities > MAXCITIES)
	{
		printf("Invalid File\n");
		exit(1);
	}
	for(i=0; i < x.numCities; i++)
        for(j=0; j < x.numCities; j++) 
            fscanf(f, "%d", &distan[i][j]);
	fclose(f);


	return x;
}



void swap(int *a, int *b)
{
    int x;
    
    x=*a;
    *a=*b;
    *b=x;
}

//reparação de permutação aleatória
void repara_p(int tab[], int tam, int mat[MAXCITIES][MAXCITIES])
{
    int i=0, x, y, z;
    

    for(i=0; i<tam; i++)
    {

        x = tab[i];
		if(i+1<tam){
			y = tab[i+1];
			if(mat[x-1][y-1] == -1)
			{
				z = tab[i];
				tab[i] = tab[i+1];
				tab[i+1] = z;
			}
		}
    }
}

// Geracao de uma permutacao aleatoria (Knuth shuffle algorithm)
void generate_p(int tab[], int tam, int distan[MAXCITIES][MAXCITIES])
{
	int i;

	for(i=0; i < tam; i++)
        tab[i]=i+1;
    
	for(i=tam-1; i>=1; i--)
    {
        swap(&tab[i],&tab[random_l_h(1, tam-1)]);
    }
    for(i=0; i < tam; i++)
        repara_p(tab, tam, distan);
    
    
}

// Criacao da populacao inicial. O vector e alocado dinamicamente
// Argumeentos: Estrutura com parametros
// Devolve o vector com a populacao 
pchrom init_pop(struct info d, int distan[MAXCITIES][MAXCITIES])
{
	int i;
	pchrom p = malloc(sizeof(chrom)*d.popsize);
	if(p==NULL)
	{
		printf("Erro na alocacao de memoria\n");
		exit(1);
	}
	
	for(i=0; i<d.popsize; i++)
	{
		 generate_p((p+i)->chromosome, d.numCities, distan);
         (p+i)->distance = 0;
	}
	return p;
}


// Actualiza a melhor solucao encontrada
// Argumentos: populacao actual, estrutura com parametros e melhor solucao encontrada ate a geracao imediatamente anterior
// Devolve a melhor solucao encontrada ate a geracao actual
chrom get_best(pchrom pop, struct info d, chrom best)
{
	int i;
	
	for(i=0; i<d.popsize; i++)
	{
		if(best.distance > pop[i].distance)
			best=pop[i];
	}
	return best;
}


// Escreve uma solucao na consola
// Argumentos: solucao e estrutura com parametros
void write_best(chrom x, struct info d)
{
	int i;

	printf("\n\nBest solution: %d\n", x.distance);
	for(i=0; i<d.numCities; i++)
		printf("%d  ", x.chromosome[i]);
	putchar('\n');
}


// Funcoes auxiliares para gerar numerios aleatorios

// Inicializacao do gerador de numeros aleatorios
void init_rand()
{
	srand((unsigned)time(NULL));
}


// Devolve um valor inteiro distribuido uniformemente entre min e max
int random_l_h(int min, int max)
{
	return min + rand() % (max-min+1);
}


// Devolve um valor real distribuido uniformemente entre [0, 1]
float rand_01()
{
	return ((float)rand())/RAND_MAX;
}


// Simula o lancamento de uma moeda
int flip()
{
	if ((((float)rand()) / RAND_MAX) < 0.5)
		return 0;
	
	else
		return 1;
}

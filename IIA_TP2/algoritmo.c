#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include "algoritmo.h"
#include "utils.h"



// Seleccao por torneio binaria (tamanho de torneio: 2)
// Argumentos: populacao actual, parametros, pais
// A funcao preenche o vector pais de acordo com o resultados dos torneios
void binary_tournament(pchrom pop, struct info d, pchrom parents)
{
	int i, x1, x2;
    
	for(i=0; i<d.popsize;i++)
	{	
		x1 = random_l_h(1, d.popsize-2);
		
		do
			x2 = random_l_h(1, d.popsize-2);
		while(x1==x2);
		
		if((pop+x1)->distance < (pop+x2)->distance)		// Problema de minimizacão
			*(parents + i) = *(pop + x1);
		else
			*(parents + i) = *(pop + x2);
	}
}

// Seleccao por torneio variÃ¡vel (tamanho de torneio: > 2)
// Argumentos: populacao actual, parametros, pais
void torneio_variavel(pchrom pop, struct info d, pchrom parents){
    double melhor;
    int escolhido, i, x, k;
    
    
    for(i=0; i <d.popsize; i++) {
        for(k=0; k < d.t_size; k++){
            x = random_l_h(0,d.popsize-1);
            if(k==0){
                melhor = (pop+x)->distance;
                escolhido = x;
            }
            if((pop+x)->distance < melhor) {
                melhor = (pop+x)->distance;
                escolhido = x;
            }
        }
        *(parents+i) = *(pop+escolhido);
    }
}


// Operadores geneticos
// Argumentos: pais, estrutura com parametros, descendentes
void genetic_operators(pchrom parents, struct info d, pchrom offspring, int mat[MAXCITIES][MAXCITIES])
{
	recombination(parents, d, offspring, mat);
	mutation(d, offspring, mat);
}

// Chama a funcao cx_order que implementa a recombinacao por ordem (com probabilidade pr)
// Argumentos: pais, estrutura com parametros, descendentes
// A funcao preenche o vector descendentes com o resultado das operacoes de crossover
void recombination(pchrom parents, struct info d, pchrom offspring, int mat[MAXCITIES][MAXCITIES])
{
	int i; 

	for(i=0; i<d.popsize; i+=2)
	{
		if(rand_01() < d.pr)
		{
			cx_order((parents+i)->chromosome, (parents+i+1)->chromosome, (offspring+i)->chromosome, (offspring+i+1)->chromosome, d, mat);
		}
		else						
		{
			*(offspring+i) = *(parents+i);
			*(offspring+i+1) = *(parents+i+1);
		}
		(offspring+i)->distance = (offspring+i+1)->distance = 0;
	}
}

// Recombinacao por ordem
// Argumentos: pai1, pai2, descendente1, descendente2, estrutura com parametros
void cx_order(int p1[], int p2[], int d1[], int d2[], struct info d, int mat[MAXCITIES][MAXCITIES])
{
	int tab1[MAXCITIES+1]={0}, tab2[MAXCITIES+1]={0};		
	int point1, point2, index, i, aux=0, aux2=0, x;

	// seleccao dos pontos de corte
	point1 = random_l_h(1, d.numCities-2);
	do{
		point2 = random_l_h(1, d.numCities-2);
	}while(point1 == point2);
	if(point1 > point2)
	{
		i = point1;
		point1 = point2;
		point2 = i;
	}
    
	//copia das seccoes internas
	for(i = point1; i<=point2; i++)
	{
		d1[i]=p1[i];
		tab1[p1[i]]=1;
		d2[i]=p2[i];
		tab2[p2[i]]=1;
	}
    
	
	// preencher o resto do descendente 1
	index = (point2+1)%d.numCities;
	for(i=point2+1; i<d.numCities; i++)
	{
		if(tab1[p2[i]]==0)
		{
			d1[index]=p2[i];
			index = (index+1)%d.numCities;
		}
	}
	for(i=0; i<=point2; i++)
	{
		if(tab1[p2[i]]==0)
		{
			d1[index]=p2[i];
			index = (index+1)%d.numCities;
		}
	}
    
    
	// preencher o resto do descendente 2
	index = (point2+1)%d.numCities;
	for(i=point2+1; i<d.numCities; i++)
	{
		if(tab2[p1[i]]==0)
		{
			d2[index]=p1[i];
			index = (index+1)%d.numCities;
		}
	}
	for(i=0; i<=point2; i++)
	{
		if(tab2[p1[i]]==0)
		{
			d2[index]=p1[i];
			index = (index+1)%d.numCities;
        }
    }
    
    //Reparar Crossover para D1
    for(x=0; x < d.numCities; x++)
    {
        if(d1[x] == 1)
        {
            aux = d1[0];
            aux2 = d1[x];
            d1[0] = aux2;
            d1[x] = aux;
        }
    }
    for(x=0; x < d.numCities; x++)
    {
        if(d1[x] == d.numCities)
        {
            aux = d1[d.numCities-1];
            aux2 = d1[x];
            d1[d.numCities-1] = aux2;
            d1[x] = aux;
        }
    }
    //Reparar Crossover para D2
    for(x=0; x < d.numCities; x++)
    {
        if(d2[x] == 1)
        {
            aux = d2[0];
            aux2 = d2[x];
            d2[0] = aux2;
            d2[x] = aux;
        }
    }
    for(x=0; x < d.numCities; x++)
    {
        if(d2[x] == d.numCities)
        {
            aux = d2[d.numCities-1];
            aux2 = d2[x];
            d2[d.numCities-1] = aux2;
            d2[x] = aux;
        }
    }
    
    //Continuacao reparar
	/*
	for(i=0; i < d.numCities; i++)
    {
		repara_p(d1, d.numCities, mat);
		repara_p(d2, d.numCities, mat);
	}*/
    
}


// Chama as funcoes que implementam as operacoes de mutacao (de acordo com as respectivas probabilidades)
// Argumentos: estrutura de parametros e descendentes
// Na versao disponibilizada, apenas a mutacao swap esta implementada
void mutation(struct info d, pchrom offspring, int mat[MAXCITIES][MAXCITIES])
{
	int i;

	for(i=0; i< d.popsize; i++)
	{
		if(rand_01() < d.pm_swap)
			mutation_swap(d, (offspring+i)->chromosome, mat);
		if(rand_01() < d.pm_ins)
			mutation_ins(d, (offspring+i)->chromosome);
		if(rand_01() < d.pm_inv)
			mutation_inv(d, (offspring+i)->chromosome);
	}
}


// Mutacao swap
// Argumentos: estrutura de parametros e solucao a alterar
void mutation_swap(struct info d, int a[], int mat[MAXCITIES][MAXCITIES])
{
	int x, y, z, i;

	x=random_l_h(1,	d.numCities-2);
	do{
		y=random_l_h(1, d.numCities-2);
	}while(x==y);

	z=a[x];
	a[x]=a[y];
	a[y]=z;

	for(i=0; i < d.numCities; i++)
    {
        repara_p(a, d.numCities, mat);
    }

}


// Insertion mutation
//2 valores aleatorios e pega no 2 e mte ao lado do 1 deslocando o resto para a direita
void mutation_ins(struct info d, int a[])
{
	//printf("Nao implementada\n");
	int j, k, p1, p2, x;
	 p1 = random_l_h(1, d.numCities-2);
	 do {
        p2 = random_l_h(1,d.numCities-2);
    } while (p1 == p2);

	 if (p1 > p2) {
        x = p2;
        p2 = p1;
        p1 = x;
    }
	 j=1;
	 k=p1+1;
	 x = a[p2];
	 while(j<p2-p1){
		a[p2]=a[p2-1];
		p2--;
		j++;
	 }
	 a[p2]=x;

}

// Inversion mutation
//2 valores aleatorios e pega nos valores entre esses pontos e inverte a ordem
void mutation_inv(struct info d, int a[])
{
	//printf("Nao implementada\n");
	int aux[MAXCITIES], j, k, p1, p2, x;
    
    
    p1 = random_l_h(1, d.numCities-2);
    do {
        p2 = random_l_h(1,d.numCities-2);
    } while (p1 == p2);
    if (p1 > p2) {
        x = p2;
        p2 = p1;
        p1 = x;
    }
    j=0;
    k=p2;
    while (k >= p1) {
        aux[j] = a[k];
        j++;
        k--;
    }
    j = 0;
    k = p1;
    while (k <= p2) {
        a[k] = aux[j];
        j++;
        k++;
    }
}


//Trepa Colinas
void trepa_colinas(struct individual *sol, struct info d, int a[MAXCITIES][MAXCITIES], int itera)
{
    int i;
    struct individual vizinho;
    
    for (i=0; i < itera; i++) {
        vizinho = gera_vizinho(*sol,d,a);
        vizinho.distance = compute_dist(vizinho.chromosome, d, a);
        if(vizinho.distance < sol->distance)
            *sol = vizinho;
    }
}

//
struct individual gera_vizinho(struct individual sol, struct info d, int a[MAXCITIES][MAXCITIES])
{
    int p1, p2, aux,i;
    p1 = random_l_h(1,d.numCities-2);
    do {
        p2 = random_l_h(1,d.numCities-2);
    } while(p1==p2);
    aux = sol.chromosome[p1];
    sol.chromosome[p1] = sol.chromosome[p2];
    sol.chromosome[p2] = aux;
    return sol;
}


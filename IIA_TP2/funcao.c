#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "algoritmo.h"
#include "funcao.h"




// Calcula a qualidade de uma solucao (distancia percorrida)
// Argumentos: solucao, estrutura de parametros e matriz com dados do problema
// Devolve a distancia calculada
int compute_dist(int s[], struct info d, int distan[MAXCITIES][MAXCITIES])
{
	int total=0;
	int i,x,y, dist=0;

	for(i=0; i<d.numCities-1; i++)
    {
        x = s[i];
        y = s[i+1];
        if(distan[x-1][y-1] == -1)
        {
            dist = 9999999;
        }
        else
        {
            dist = distan[x-1][y-1];
           
        }
		total += dist;
    }
    // Distancia entre 2 pontos
	return total;
}


// Avaliacao da populacao
// Argumentos: populacao, estrutura com parametros e matriz com dados do problema
void evaluate(pchrom pop, struct info d, int distan[MAXCITIES][MAXCITIES])
{
	int i;
	
	for(i=0; i<d.popsize; i++)
	{
		(pop+i)->distance = compute_dist((pop+i)->chromosome, d, distan);
	}
	
}



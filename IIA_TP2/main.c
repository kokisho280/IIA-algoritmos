#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmo.h"
#include "utils.h"
#include "funcao.h"

//pop:  populacao actual
//parents: progenitores da pop seguinte

#define DEFAULT_RUNS	10

int main(int argc, char *argv[])
{
	char nome_fich[100];
	struct info parameters;		
	pchrom pop = NULL, parents = NULL;
	int r, runs, gen_actual, i;
	chrom best_run, best_ever;
	int distancias[MAXCITIES][MAXCITIES];
	float mbf = 0.0;

	
	if(argc == 3)
	{
		runs = atoi(argv[2]);
		strcpy(nome_fich, argv[1]);
	}
	else 
		if(argc == 2)
		{
			runs = DEFAULT_RUNS;
			strcpy(nome_fich, argv[1]);
		}else{
			runs = DEFAULT_RUNS;
			printf("Nome do Ficheiro: ");
			gets(nome_fich);
		}
	if(runs <= 0)
		return 0;

	init_rand();												
	parameters = init_data(distancias, nome_fich);			// Leitura de ficheiros e inicializacao da matriz de coordenadas
	
	


	for(r=0; r<runs; r++)
	{	
		pop = init_pop(parameters, distancias);								// Geracao da populacao inicial
		evaluate(pop, parameters, distancias);						// Avaliacao da populacao inicial
		
		// Trepa colinas
		for(i=0; i < parameters.popsize; i++){
			 trepa_colinas((pop+i),parameters, distancias,100);

			 //best_run=pop[i];
				//write_best(best_run, parameters);
				//getchar();


		}	
		
		gen_actual = 1;	
		best_run=pop[0];
		best_run = get_best(pop, parameters, best_run);			// Inicializar a melhor solucao encontrada
		parents = malloc(sizeof(chrom)*parameters.popsize);		// Reservar espaco para os pais
		if(parents==NULL)
		{
			printf("Erro na alocacao de memoria\n");
			exit(1);
		}
		// Main evolutionary loop
		
		while(gen_actual <= parameters.numGenerations)
		{

			binary_tournament(pop, parameters, parents);				// Torneio binario para encontrar os progenitores (ficam armazenados no vector parents)
			//torneio_variavel(pop, parameters, parents);         // Torneio variável para encontrar os progenitores 
			genetic_operators(parents, parameters, pop, distancias); 		// Aplicar operadores geneticos aos pais (os descendentes ficam armazenados no vector pop)
			evaluate(pop, parameters, distancias);					// Avaliar a nova populacao
			best_run = get_best(pop, parameters, best_run);		// Actualizar a melhor solucao encontrada
			gen_actual++;

		}
		
		// Escreve resultados da repeticao que terminou
		printf("\nRepeticao %d:", r);
		write_best(best_run, parameters);
		
		mbf += best_run.distance;
		if(r==0 || best_run.distance < best_ever.distance)
			best_ever = best_run;
		
				
		free(pop);
		free(parents);
	}
	
	// Escreve resultados globais
	printf("\n\nMBF: %.2f\n", mbf/r);
	
	printf("\nMelhor solucao encontrada");
	write_best(best_ever, parameters);
	


	

	return 0;
}

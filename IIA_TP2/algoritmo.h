
#define MAXCITIES 100		// Numero maximo de cidades

 
// Estrutura para armazenar parametros
struct info{
    int popsize;
    float pm_swap;
	float pm_ins;
	float pm_inv;
    float pr;
	int t_size;
    int numCities;
    int numGenerations;       
};

// Individuo (solucao)
typedef struct individual chrom, *pchrom;

struct individual{
    int chromosome[MAXCITIES];		// trajecto
	int distance;					// distancia (qualidade)
};


void binary_tournament(pchrom pop, struct info d, pchrom parents);

void torneio_variavel(pchrom pop, struct info d, pchrom parents);

void genetic_operators(pchrom parents, struct info d, pchrom offspring, int mat[MAXCITIES][MAXCITIES]);

void recombination(pchrom parents, struct info d, pchrom offspring, int mat[MAXCITIES][MAXCITIES]);

void mutation(struct info d, pchrom offspring, int mat[MAXCITIES][MAXCITIES]);

void cx_order(int p1[], int p2[], int d1[], int d2[], struct info d, int mat[MAXCITIES][MAXCITIES]);

void mutation_swap(struct info d, int a[], int mat[MAXCITIES][MAXCITIES]);

void mutation_ins(struct info d, int a[]);

void mutation_inv(struct info d, int a[]);

void repara_p(int tab[], int tam, int mat[MAXCITIES][MAXCITIES]);

void trepa_colinas(struct individual *sol, struct info d, int a[MAXCITIES][MAXCITIES], int itera);

struct individual gera_vizinho(struct individual sol, struct info d, int a[MAXCITIES][MAXCITIES]);
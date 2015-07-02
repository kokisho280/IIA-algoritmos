

struct info init_data(int distan[MAXCITIES][MAXCITIES], char *nome_fich);

void generate_p(int tab[], int tam, int distan[MAXCITIES][MAXCITIES]);

pchrom init_pop(struct info d, int distan[MAXCITIES][MAXCITIES]);

chrom get_best(pchrom pop, struct info d, chrom best);

void write_best(chrom x, struct info d);

void init_rand();

int random_l_h(int min, int max);

float rand_01();

int flip();


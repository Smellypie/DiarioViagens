#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <assert.h>

struct Cidade
{
	char *cidade;
	struct Cidade *seg;
};

struct Data
{
	int dia;
	int mes;
};

struct Ano
{
	int ano;
	int despesa;														//despesa total nesse ano
	int nDestinos;														//número de cidades/países visitados no total nesse ano
	int kmAviao;														//distância percorrida de avião durante esse ano
	int kmCarro;														//distância percorrida de carro durante esse ano
	int diasViagem;														//número total de dias de viagem nesse ano
	struct Viagem *viagens;												//lista de viagens que ocorreram nesse ano
	struct Ano *seg;
};

struct Viagem
{
	struct Data diaIni;													//dia do início da viagem(dia mes)
	int duracao;														//duração da viagem em dias
	int meioT;															//meio de transporte(0 - carro, 1 - avião, ...)
	int custo;															//custo total da viagem(euros)
	int kmPercorridos;													//quilómetros percorridos durante a viagem
	char *destinoP;														//destino da viagem(país)
	int nCidades;														//número de cidades visitadas durante a viagem
	struct Cidade *cidades;												//destino da viagem(vector de cidades)
	struct Viagem *seg;
};

struct Ano *calendario;													//os dados desta struct são os totais

void criaCalendario()													//Base da lista
{
	calendario = malloc(sizeof(struct Ano));
	calendario->despesa = 0;
	calendario->nDestinos = 0;
	calendario->kmAviao = 0;
	calendario->kmCarro = 0;
	calendario->diasViagem = 0;
	calendario -> ano = 0;
	calendario -> viagens = NULL;
	calendario -> seg = NULL;
}

char *ajustaMemoria(char *str, int len)									//cria uma nova string com apenas a memória necessária para str
{
	char *novo;
	int i;
	novo = malloc((len + 1) * sizeof(char));
	for(i = 0; i < len; i++)
	{
		*(novo + i) = *(str + i);
	}
	*(novo + len + 1) = '\0';
	free(str);
	return novo;
}

void actualizaDados(struct Ano *ano, struct Viagem *viagem)				//actualiza os valores dos custos ... totais e por ano
{
	ano->despesa += viagem->custo;
	//ano->nDestinos += viagem->
	if(viagem->meioT == 1)
	{
		ano->kmAviao += viagem->kmPercorridos;
	}
	if(viagem->meioT == 2)
	{
		ano->kmCarro += viagem->kmPercorridos;
	}
	ano->diasViagem += viagem->duracao;
	
	calendario->despesa += viagem->custo;
	//ano->nDestinos += viagem->
	if(viagem->meioT == 1)
	{
		calendario->kmAviao += viagem->kmPercorridos;
	}
	if(viagem->meioT == 2)
	{
		calendario->kmCarro += viagem->kmPercorridos;
	}
	calendario->diasViagem += viagem->duracao;
}

void adicionaAno(int ano)												//Cria a struct de um novo ano
{
	struct Ano novo, *aux;	
	if(calendario -> seg == NULL)
	{
		novo.ano = ano;
		novo.seg = NULL;
		calendario -> seg = &novo;
	}
	else
	{
		aux = calendario;
		while(aux -> seg != NULL && aux -> seg -> ano < ano)
		{
			aux = aux -> seg;
		}
		if(aux -> seg == NULL)
		{
			novo.ano = ano;
			novo.viagens = NULL;
			novo.seg = NULL;
			aux -> seg = &novo;
		}
		else
		{
			novo.ano = ano;
			novo.viagens = NULL;
			novo.seg = aux -> seg;
			aux -> seg = &novo;
		}
	}
}

struct Ano *procuraAno(int ano)											//verifica se determinado ano já está criado
{
	struct Ano *aux;
	aux = calendario;
	while(aux -> ano != ano && aux != NULL)
	{
		aux = aux -> seg;
	}
	if(aux == NULL)
	{
		return NULL;
	}
	else
	{
		return aux;
	}
}

int vemAntes(struct Data d1, struct Data d2)							//Compara as datas e devolve verdadeiro se a primeira data vem antes
{
	if(d1.mes < d2.mes)
	{
		return 1;
	}
	else
	{
		if(d1.mes > d2.mes)
		{
			return 0;
		}
		else //d1.mes == d2.mes
		{
			if(d1.dia < d2.dia)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
}

void insereCidade(struct Viagem *viagem, char *c)	//tem erro
{
	struct Cidade *cidade, *aux;
	cidade = malloc(sizeof(struct Cidade));
	if(viagem->cidades == NULL)
	{
		cidade->cidade = c;
		cidade->seg = NULL;
		viagem->cidades = cidade;
	}
	else
	{
		aux = viagem->cidades;
		while(aux->seg != NULL)
		{
			aux = aux->seg;
		}
		cidade->cidade = c;
		cidade->seg = NULL;
		aux->seg = cidade;
	}
}

void insereViagem(struct Ano *ano, struct Viagem *viagem)				//Insere viagem criada na lista de viagens ordenadas cronologicamente do respetivo ano
{
	struct Viagem *aux;
	if(ano -> viagens == NULL)
	{
		ano -> viagens = viagem;
	}
	else
	{
		if(vemAntes(viagem -> diaIni, ano -> viagens -> diaIni))
		{
			viagem -> seg = ano -> viagens;
			ano -> viagens = viagem;
		}
		else
		{
			aux = ano -> viagens;
			while(aux -> seg != NULL && vemAntes(aux -> seg -> diaIni, aux -> diaIni))
			{
				aux = aux -> seg;
			}
			if(aux -> seg == NULL)
			{
				aux -> seg = viagem;
			}
			else
			{
				viagem -> seg = aux -> seg;
				aux -> seg = viagem;
			}
		}
	}
}

void criaViagem()
{
	FILE *F1;
	int ano, dia, mes, duracao, meioT, custo, kmPercorridos, nCidades, i;
	char *cidade, *destinoP;
	struct Data auxD;
	struct Ano *auxA;
	struct Viagem *auxV;
	
	auxV = malloc(sizeof(struct Viagem));
	destinoP = malloc(50*sizeof(char));
	cidade = malloc(50*sizeof(char));
	
	F1=fopen("DiarioViagens.txt","a");									//acrecenta no fim do ficheiro
	fprintf(F1, "\n");													//nao sei se é preciso temos de testar
	
	printf("Dia de inicio da viagem(dd mm aaaa): \n");
	scanf("%d%d%d", &dia, &mes, &ano);
	fflush(stdin);
	fprintf(F1, "%d;%d;%d;", dia, mes, ano);
	
	printf("Duracao da viagem(em dias): \n");
	scanf("%d", &duracao);
	fflush(stdin);
	fprintf(F1, "%d;", duracao);
	
	printf("Meio de transporte usado:\n1 - Aviao\n2 - Carro\n");
	scanf("%d", &meioT);
	fflush(stdin);
	fprintf(F1, "%d;", meioT);
	
	printf("Distancia percorrida durante a viagem(em quilometros): ");
	scanf("%d", &kmPercorridos);
	fflush(stdin);
	fprintf(F1, "%d;", kmPercorridos);
	
	printf("Custo total da viagem(em euros): ");
	scanf("%d", &custo);
	fflush(stdin);
	fprintf(F1, "%d;", custo);
	
	printf("Destino da viagem\nPais: ");
	scanf("%s", destinoP);
	fflush(stdin);
	destinoP = ajustaMemoria(destinoP, strlen(destinoP));
	fprintf(F1, "%s;", destinoP);
	
	printf("Numero de cidades visitadas: ");
	scanf("%d", &nCidades);
	fflush(stdin);
	fprintf(F1, "%d", nCidades);
	
	for(i = 0; i < nCidades; i++)
	{
		printf("Cidade numero %d: ", i + 1);
		scanf("%s", cidade);
		fflush(stdin);
		fprintf(F1, ";%s", cidade);
	}
	
	fclose(F1);
	
	if(procuraAno(ano) == NULL)	//o erro esta aqui	
	{
		adicionaAno(ano);
	}
	auxA = procuraAno(ano);
	auxD.dia = dia;
	auxD.mes = mes;
	auxV -> diaIni = auxD;
	auxV -> destinoP = destinoP;
	auxV -> duracao = duracao;
	auxV -> meioT = meioT;
	auxV -> kmPercorridos = kmPercorridos;
	auxV -> custo = custo;
	auxV -> seg = NULL;
	actualizaDados(auxA, auxV);
	insereViagem(auxA, auxV);
	
}

void leFicheiro()
{
	FILE *F1;
	int dia, mes, ano, duracao, meioT, kmPercorridos, custo;
	char *destinoP;
	struct Data auxD;
	struct Ano *auxA;
	struct Viagem *auxV;
	
	auxV = malloc(sizeof(struct Viagem));
	auxA = malloc(sizeof(struct Ano));
	destinoP = malloc(50*sizeof(char));
	
	if(fopen("DiarioViagens.txt", "r") == NULL)
	{
		fopen("DiarioViagens.txt", "w");
		printf("Como nao foi encontrado nenhum ficheiro com o nome \"DiarioViagens.txt\",\nfoi criado um vazio\n");
	}
	else
	{
		F1 = fopen("DiarioViagens.txt", "r");
		while(fscanf(F1, "%d %d %d %s %d %d %d %d", &dia, &mes, &ano, destinoP, &duracao, &meioT, &kmPercorridos, &custo) != EOF)	//para isto funcionar as strings nao podem ter espaços 
		{
			if(procuraAno(ano) == NULL)
			{
				adicionaAno(ano);
			}
			auxA = procuraAno(ano);
			auxD.dia = dia;
			auxD.mes = mes;
			auxV -> diaIni = auxD;
			auxV -> destinoP = destinoP;
			
			auxV -> duracao = duracao;
			auxV -> meioT = meioT;
			auxV -> kmPercorridos = kmPercorridos;
			auxV -> custo = custo;
			auxV -> seg = NULL;
			insereViagem(auxA, auxV);
		}
		fclose(F1);
	}
}

struct Cidade *criaCidade(){
	struct Cidade *aux = malloc(sizeof(struct Cidade));
	aux -> seg = NULL;
	aux -> cidade = "Dummy";
	return aux;
	}

void imprimeCidades(){
	int jaexiste;
	struct Ano *auxA;
	struct Viagem *auxV;
	struct Cidade *auxC;
	struct Cidade *todas;
	struct Cidade *todasaux1;
	struct Cidade *todasaux2;
	struct Cidade *todasprox;
	auxA = malloc(sizeof(struct Ano));
	auxV = malloc(sizeof(struct Viagem));
	auxC = malloc(sizeof(struct Cidade));
	todas = malloc(sizeof(struct Cidade));
	todasaux1 = malloc(sizeof(struct Cidade));
	todasaux2 = malloc(sizeof(struct Cidade));
	todasaux1 = todas;
	for(auxA = calendario; auxA != NULL; auxA = auxA -> seg){								//ano a ano
		for(auxV = auxA -> viagens; auxV != NULL; auxV = auxV -> seg){						//viagem a viagem
			for(auxC = auxV -> cidades; auxC != NULL; auxC = auxC -> seg){					//cidade a cidade
				//printf("%s\n", auxC -> cidade);											//teste		
				for(todasaux2 = todas; todasaux2 != NULL; todasaux2 = todasaux2 -> seg){	//ver se a cidade ja foi guardada no 'todas'
					if(strcmp(todasaux2 -> cidade, auxC -> cidade) == 0){jaexiste = 1;}}	//verifica se ja existe
				if(jaexiste != 1){															//se a cidade nao existe
					//todasaux1 -> cidade = auxC -> cidade;									//guardar a cidade
					strcpy(todasaux1 -> cidade, auxC -> cidade);
					todasprox = criaCidade();												//criar o proximo sitio para guardar a cidade
					todasaux1 -> seg = todasprox;											//juntar à lista todas
					todasaux1 = todasaux1 -> seg;											//avanço do auxiliar
					jaexiste = 0;
					}
				}
			}
		}
	for(todasaux1 = todas; todasaux1 != NULL; todasaux1 = todasaux1 -> seg){				//cidade a cidade
		printf("%s\n", todasaux1 -> cidade);			//falta por por ordem alfabetica
		}
	}

void consultaInformacao(){
	printf("		Países e cidades visitadas \n");
	printf("Países:\n");
	printf("Total de países:\n");
	printf("Total de países por ano:\n");
	printf("Cidades:\n");
	printf("Total de cidades: %d\n", );
	printf("Total de cidades por ano: %d\n", );
	
	printf("		Número de quilómetros percorridos:\n");
	printf("Total de quilómetros percorridos:\n");
	printf("Total de quilómetros percorridos:\n");
	printf("Total de quilómetros percorridos de Aviao:\n");
	printf("Total de quilómetros percorridos de Carro:\n");
	
	printf("		Despesas:");
	printf("Total de despesas:\n");
	printf("Despesa viagem %d: %d\n", i, ??);
	
		
	printf("		Dias de viagem:");
	printf("Total de dias de viagem:\n");
	printf("Dias de viagem por viagem:\n");
	printf("Dias de viagem por ano:\n");
	}


int percentagem(int a, int b){
	int res;
	if(a == 0 && b == 0){
		return -1;
		}
	else{
		assert(a > 0);
		assert(b > 0);
		assert(a > b);
		res = (a*100)/b;
		return res;
		}	
	}

void consultaPercentagens(){
	printf("");
	printf("");
	printf("");
	printf("");
	printf("");
	
	
	
	
	}

void criaRelatorioe(){ //atençao a ordem alfabetica
	
	
	
	}


void criaRelatoriof(){ //atençao a ordem alfabetica
	char *data;
	char *ficheiro;
	FILE *fp;
	printf("Introduza a data atual (dd mm aaaa): \n");
	scanf("%s", data);
	
	sprintf(ficheiro,"Relatorio%s.txt",data);

	fp=fopen(ficheiro,"w");
 
	
	}


void menu()
{
	int o, s = 1;
	leFicheiro();
	while(s){
		printf("||||||||||||||||||||||||||||||||||||||||||||||||||\n");
		printf("||												||\n");
		printf("||				Diario de Viagens				||\n");
		printf("||												||\n");
		printf("||		1 - Adiciona viagem						||\n");
		printf("||		2 - Consultar informacao				||\n");
		printf("||		3 - Consultar informação percentual		||\n");
		printf("||		4 - Criar relatorio	(ecra)				||\n"); // dos países onde visitou mais cidades e onde gastou mais dinheiro
		printf("||		5 - Criar relatorio	(ficheiro)			||\n");
		printf("||		6 - Sair								||\n");
		printf("||												||\n");
		printf("||||||||||||||||||||||||||||||||||||||||||||||||||\n");
		scanf("%d", &o);
		if(s != 0){
			switch(o)
			{
				case 1:
				criaViagem();
				fflush(stdin);
				printf("Clique ENTER para continuar...");
				getchar();
				fflush(stdin);
				break;
				
				case 2:
				consultaInformacao();
				break;
				
				case 3:
				consultaPercentagens();
				break;
				
				case 4:
				criaRelatorioe();
				break;
				
				case 5:
				criaRelatoriof();
				break;
				
				case 6:
				s = 0;
				
				break;
				
				default:
				printf("Parametro invalido...");
			}
			
			
			}
	}
}

int main()
{
	menu();
	return 0;
}



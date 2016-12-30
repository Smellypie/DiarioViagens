#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

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
		//insereCidade(auxV, cidade);	tem erro
	}
	
	fclose(F1);
	
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

void menu()
{
	int o;
	printf("\tDiario de Viagens\n1 - Adiciona Viagem\n2 - ...\n");
	scanf("%d", &o);
	switch(o)
	{
		case 1:
		//adicionaViagem();
		break;
		case 2:
		//...
		default:
		printf("Parametro invalido...");
	}
}

int main()
{
	criaCalendario();
	leFicheiro();
	return 0;
}



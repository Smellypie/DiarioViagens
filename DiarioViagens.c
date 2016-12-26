#include <stdio.h>
#include <stdlib.h>

struct Data
{
	int dia;
	int mes;
};

struct Ano
{
	int ano;
	struct Viagem *viagens;												//lista de viagens que ocorreram nesse ano
	struct Ano *seg;
};

struct Viagem
{
	struct Data diaIni;													//dia do início da viagem(dia mes)
	int duracao;														//duração da viagem em dias
	char *destinoP;														//destino da viagem(país)
	char *destinoC;														//destino da viagem(cidade)
	int meioT;															//meio de transporte(0 - carro, 1 - avião, ...)
	int custo;															//custo total da viagem(euros)
	int kmPercorridos;													//quilómetros percorridos durante a viagem
	struct Viagem *seg;
};

struct Ano *calendario;

//leFicheiro

void criaCalendario()													//Base da lista
{
	calendario = malloc(sizeof(struct Ano));
	calendario -> ano = 0;
	calendario -> viagens = NULL;
	calendario -> seg = NULL;
}

void adicionaAno(int ano)												//Cria a struct de um novo ano (dá erro)
{
	struct Ano *aux, *aux2;
	aux = malloc(sizeof(struct Ano));
	if(calendario -> seg == NULL)
	{
		aux -> ano = ano;
		aux -> seg = NULL;
		calendario -> seg = aux;
	}
	else
	{
		if(calendario -> seg -> ano > ano)
		{
			aux -> ano = ano;
			aux -> viagens = NULL;
			aux -> seg = calendario -> seg;
			calendario -> seg = aux;
		}
		else
		{
			aux = calendario -> seg;
			while(aux -> ano < ano && aux != NULL)
			{
				aux2 = aux;
				aux = aux -> seg;
			}
			if(aux == NULL)
			{
				aux -> ano = ano;
				aux -> viagens = NULL;
				aux -> seg = NULL;
			}
			else
			{
				aux -> ano = ano;
				aux -> viagens = NULL;
				aux -> seg = aux2 -> seg;
				aux2 -> seg = aux;
			}
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

void insereViagem(struct Ano *ano, struct Viagem *viagem)				//Insere viagem criada na lista de viagens ordenadas cronologicamente do respetivo ano
{
	if(ano -> viagens == NULL)
	{
		ano -> viagens = viagem;
	}
	else
	{
		//Por acabar
	}
}

void adicionaViagem()
{
	int ano, dia, mes, duracao, meioT, custo, kmPercorridos;
	char *destinoC, *destinoP;
	struct Data auxD;
	struct Ano *auxA;
	struct Viagem *auxV;
	auxV = malloc(sizeof(struct Viagem));
	printf("Dia de inicio da viagem(dd mm aaaa): \n");
	scanf("%d%d%d", &dia, &mes, &ano);
	fflush(stdin);
	printf("Destino da viagem\nPais: ");
	scanf("%s", &destinoP);
	fflush(stdin);
	printf("Cidade: ");
	scanf("%s", &destinoC);
	fflush(stdin);
	printf("Duracao da viagem(em dias): \n");
	scanf("%d", &duracao);
	fflush(stdin);
	printf("Meio de transporte usado:\n1 - Aviao\n2 - Carro\n");
	scanf("%d", &meioT);
	fflush(stdin);
	printf("Distancia percorrida durante a viagem(em quilometros): ");
	scanf("%d", &kmPercorridos);
	fflush(stdin);
	printf("Custo total da viagem(em euros): ");
	scanf("%d", &custo);
	fflush(stdin);
	
	if(procuraAno(ano) == NULL)
	{
		adicionaAno(ano);
	}
	auxA = procuraAno(ano);
	auxD.dia = dia;
	auxD.mes = mes;
	auxV -> diaIni = auxD;
	auxV -> destinoC = destinoC;
	auxV -> destinoP = destinoP;
	auxV -> duracao = duracao;
	auxV -> meioT = meioT;
	auxV -> kmPercorridos = kmPercorridos;
	auxV -> custo = custo;
	auxV -> seg = NULL;
	insereViagem(auxA, auxV);
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
	//adicionaAno(2000);
	adicionaAno(2001);
	adicionaAno(2000);
	adicionaViagem();
	//printf("%d\n", calendario -> seg -> seg -> ano);
	//leFicheiro();
	//menu();
	return 0;
}


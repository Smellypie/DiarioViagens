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
	struct Viagem *lt;		//lista de viagens que ocorreram nesse ano
	struct Ano *seg;
};

struct Viagem
{
	struct Data diaIni;		//dia do início da viagem(dia mes)
	int duracao;			//duração da viagem em dias
	char *destinoP;			//destino da viagem(país)
	char *destinoC;			//destino da viagem(cidade)
	int meioT;			//meio de transporte(0 - carro, 1 - avião, ...)
	int custo;			//custo total da viagem(euros)
	int kmPercorridos;		//quilómetros percorridos durante a viagem
	struct Viagem *seg;
};

struct Ano *calendario;

//adicionaViagem
//leFicheiro

void criaCalendario()
{
	calendario = malloc(sizeof(struct Ano));
	calendario -> ano = 0;
	calendario -> lt = NULL;
	calendario -> seg = NULL;
}

void adicionaAno(int ano)	//cria a struct de um novo ano
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
			aux -> seg = calendario -> seg;
			calendario -> seg = aux;
		}
		else
		{
			aux = calendario -> seg;
			while(ano > aux -> ano || aux -> seg != NULL)
			{
				aux2 = aux;
				aux = aux -> seg;
			}
			if(aux -> seg == NULL)
			{
				aux -> ano = ano;
				aux -> seg = NULL;
				aux = aux2;
			}
			else
			{
				aux -> ano = ano;
				aux2 -> seg = aux;
			}
		}
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
	adicionaAno(2000);
	adicionaAno(1999);
	adicionaAno(2001);
	printf("%d\n", calendario -> seg -> seg -> ano);
	//leFicheiro();
	//menu();
	return 0;
}


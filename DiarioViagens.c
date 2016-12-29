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


void criaCalendario()													//Base da lista
{
	calendario = malloc(sizeof(struct Ano));
	calendario -> ano = 0;
	calendario -> viagens = NULL;
	calendario -> seg = NULL;
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

void insereViagem(struct Ano *ano, struct Viagem *viagem)				//Insere viagem criada na lista de viagens ordenadas cronologicamente do respetivo ano
{
	struct Viagem *act, *ant;
	if(ano -> viagens == NULL)
	{
		ano -> viagens = viagem;
	}
	else
	{
		if(!vemAntes(ano->viagens->diaIni, viagem->diaIni))
		{
			viagem->seg = ano->viagens;
			ano->viagens = viagem;
		}
		else
		{
			act = ano -> viagens;
			while(vemAntes(act -> diaIni, viagem -> diaIni) && act -> seg != NULL)	//as duas condições podem ser contrariados ao mesmo tempo
			{
				ant = act;
				act = act -> seg;
			}
			if(!vemAntes(act -> diaIni, viagem -> diaIni))				//nesse caso considera-se esta hipótese primeiro
			{
				viagem -> seg = act;
				ant->seg = viagem;
			}
			else
			{
				act -> seg = viagem;
			}
		}
	}
}

void leFicheiro (){
	FILE *F1;
	int dia, mes, ano, duracao, meioT, kmPercorridos, custo;
	char destinoP [20], destinoC[20];									// nao sei se pode ficar assim, dinamico?? dps tenho de mudar se nao for
	struct Data auxD;
	struct Ano *auxA;
	struct Viagem *auxV;
	auxV = malloc(sizeof(struct Viagem));
	F1=fopen("ficheiro1.txt","r");
	if(F1==NULL){printf("meter aqui o que acharmos melhor");}
	else{
		while(fscanf(F1, "%d %d %d %s %s %d %d %d %d\n", &dia, &mes, &ano, destinoP, destinoC, &duracao, &meioT, &kmPercorridos, &custo) != EOF){  //para isto funcionar as strings nao podem ter espaços 
			printf("%d", dia);
			if(procuraAno(ano) == NULL){adicionaAno(ano);}	//erro (se ficar so adiciona Ano funciona)
			puts("sss");
			auxA = procuraAno(ano);
			auxD.dia = dia;
			auxD.mes = mes;
			auxV -> diaIni = auxD; 					//unico que nao consegui testar para ja os restantes estao bem 
			auxV -> destinoC = destinoC;
			auxV -> destinoP = destinoP;
			auxV -> duracao = duracao;
			auxV -> meioT = meioT;
			auxV -> kmPercorridos = kmPercorridos;
			auxV -> custo = custo;
			auxV -> seg = NULL;
			insereViagem(auxA, auxV);								//erro no insere Viagem
			}
		}
	fclose(F1);
	}

void adicionaViagem()
{
	FILE *F1;
	int ano, dia, mes, duracao, meioT, custo, kmPercorridos;
	char *destinoC, *destinoP;
	struct Data auxD;
	struct Ano *auxA;
	struct Viagem *auxV;
	auxV = malloc(sizeof(struct Viagem));
	F1=fopen("ficheiro1.txt","a");										//acrecenta no fim do ficheiro a ou w+ ?
	//fprintf(F1, "\n")													//nao sei se é preciso temos de testar
	printf("Dia de inicio da viagem(dd mm aaaa): \n");
	scanf("%d%d%d", &dia, &mes, &ano);
	fflush(stdin);
	printf("Destino da viagem\nPais: ");
	scanf("%s", &destinoP);				//??
	fflush(stdin);
	printf("Cidade: ");
	scanf("%s", &destinoC);				//??
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
	
	fprintf(F1, "%d %d %d %s %s %d %d %d %d", dia, mes, ano, destinoP, destinoC, duracao, meioT, kmPercorridos, custo );
	fclose(F1);
	
	if(procuraAno(ano) == NULL)   //problema na procuraAno
	{
		adicionaAno(ano);
	}
	auxA = procuraAno(ano);			///problema na procuraAno
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
	insereViagem(auxA, auxV);     //problema na funçao
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



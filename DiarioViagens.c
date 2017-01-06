#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	int nViagens;														//número de países visitados nesse ano, ou seja, o número de viagens
	int nDestinos;														//número de cidades visitadas no total nesse ano
	int kmAviao;														//distância percorrida de avião durante esse ano
	int kmCarro;														//distância percorrida de carro durante esse ano
	int kmComboio;														//distância percorrida de comboio durante esse ano
	int diasViagem;														//número total de dias de viagem nesse ano
	struct Viagem *viagens;												//lista de viagens que ocorreram nesse ano
	struct Ano *seg;
};

struct Viagem
{
	struct Data diaIni;													//dia do início da viagem(dia mes)
	int duracao;														//duração da viagem em dias
	int meioT;															//meio de transporte(1 - avião, 2 - carro, 3 - comboio)
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
	calendario->kmComboio = 0;
	calendario->diasViagem = 0;
	calendario -> ano = 0;
	calendario -> viagens = NULL;
	calendario -> seg = NULL;
}

int verificaData(struct Data d, int ano)								//verifica a validade da data (1 - válida, 0 - inválida)
{
	if(d.mes < 1 || d.mes > 12)
	{
		return 0;
	}
	else
	{
		if(ano % 4 == 0)	//ano bissexto
		{
			switch(d.mes)
			{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:
				if(d.dia < 1 || d.dia > 31)
				{
					return 0;
				}
				else
				{
					return 1;
				}
				break;
				
				case 4:
				case 6:
				case 9:
				case 11:
				if(d.dia < 1 || d.dia > 30)
				{
					return 0;
				}
				else
				{
					return 1;
				}
				break;
				
				default:	//fevereiro
				if(d.dia < 1 || d.dia > 29)
				{
					return 0;
				}
				else
				{
					return 1;
				}
			}
		}
		else     //ano normal
		{
			switch(d.mes)
			{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:
				if(d.dia < 1 || d.dia > 31)
				{
					return 0;
				}
				else
				{
					return 1;
				}
				break;
				
				case 4:
				case 6:
				case 9:
				case 11:
				if(d.dia < 1 || d.dia > 30)
				{
					return 0;
				}
				else
				{
					return 1;
				}
				break;
				
				default:	//fevereiro
				if(d.dia < 1 || d.dia > 28)
				{
					return 0;
				}
				else
				{
					return 1;
				}
			}
		}
	}
}

char *ajustaMemoria(char *str, int len)									//cria uma nova string com apenas a memória necessária para str
{
	char *novo;
	int i;
	novo = malloc(len * sizeof(char));
	for(i = 0; i < len; i++)
	{
		*(novo + i) = *(str + i);
	}
	*(novo + len) = '\0';
	free(str);
	return novo;
}

void actualizaDados(struct Ano *ano, struct Viagem *viagem)				//actualiza os valores dos custos ... totais e por ano
{
	ano->despesa += viagem->custo;
	ano->nViagens++;
	ano->nDestinos += viagem->nCidades;
	if(viagem->meioT == 1)
	{
		ano->kmAviao += viagem->kmPercorridos;
	}
	if(viagem->meioT == 2)
	{
		ano->kmCarro += viagem->kmPercorridos;
	}
	if(viagem->meioT == 3)
	{
		ano->kmComboio += viagem->kmPercorridos;
	}
	ano->diasViagem += viagem->duracao;
	
	calendario->despesa += viagem->custo;
	calendario->nViagens++;
	calendario->nDestinos += viagem->nCidades;
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
	struct Ano *novo, *aux;
	novo = malloc(sizeof(struct Ano));
	novo->despesa = 0;
	novo->nViagens = 0;
	novo->nDestinos = 0;
	novo->kmAviao = 0;
	novo->kmCarro = 0;
	novo->diasViagem = 0;
	novo->ano = ano;
	novo->seg = NULL;
	novo->viagens = NULL;
	if(calendario -> seg == NULL)
	{
		calendario -> seg = novo;
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
			aux -> seg = novo;
		}
		else
		{
			novo->seg = aux -> seg;
			aux -> seg = novo;
		}
	}
}

struct Ano *procuraAno(int ano)											//verifica se determinado ano já está criado
{
	struct Ano *aux;
	if(calendario->seg == NULL)
	{
		return NULL;
	}
	else
	{
		aux = calendario->seg;
		while(aux->seg != NULL && aux->ano != ano)
		{
			aux = aux -> seg;
		}
		if(aux->ano == ano)
		{
			return aux;
		}
		else
		{
			return NULL;
		}
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

void insereCidade(struct Viagem *viagem, char *c)
{
	struct Cidade *cidade, *aux;
	char *novo;
	if(c != NULL)
	{
		novo = malloc(strlen(c)*sizeof(char));
		memcpy(novo, c, strlen(c));
	}
	else
	{
		novo = NULL;
	}
	cidade = malloc(sizeof(struct Cidade));
	if(viagem->cidades == NULL)
	{
		cidade->cidade = novo;
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
		cidade->cidade = novo;
		cidade->seg = NULL;
		aux->seg = cidade;
	}
}

void insereViagem(struct Ano *ano, struct Viagem *viagem)				//Insere viagem criada na lista de viagens ordenadas cronologicamente do respetivo ano
{
	struct Viagem *aux;
	if(ano->viagens == NULL)
	{
		ano->viagens = viagem;
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
	
	do
	{
		printf("Dia de inicio da viagem(dd mm aaaa): \n");
		scanf("%d%d%d", &dia, &mes, &ano);
		fflush(stdin);
		auxD.dia = dia;
		auxD.mes = mes;
		if(!verificaData(auxD, ano))
		{
			printf("Data invalida...\n");
		}
	}
	while(!verificaData(auxD, ano));
	if(procuraAno(ano) == NULL)
	{
		adicionaAno(ano);
	}
	auxA = procuraAno(ano);
	fprintf(F1, "%d;%d;%d;", dia, mes, ano);
	auxV -> diaIni = auxD;
	
	printf("Duracao da viagem(em dias): \n");
	scanf("%d", &duracao);
	fflush(stdin);
	fprintf(F1, "%d;", duracao);
	auxV -> duracao = duracao;
	
	printf("Meio de transporte usado:\n1 - Aviao\n2 - Carro\n3 - Comboio\n");
	scanf("%d", &meioT);
	fflush(stdin);
	fprintf(F1, "%d;", meioT);
	auxV -> meioT = meioT;
	
	printf("Distancia percorrida durante a viagem(em quilometros): ");
	scanf("%d", &kmPercorridos);
	fflush(stdin);
	fprintf(F1, "%d;", kmPercorridos);
	auxV -> kmPercorridos = kmPercorridos;
	
	printf("Custo total da viagem(em euros): ");
	scanf("%d", &custo);
	fflush(stdin);
	fprintf(F1, "%d;", custo);
	auxV -> custo = custo;
	
	printf("Destino da viagem\nPais: ");
	scanf("%s", destinoP);
	fflush(stdin);
	destinoP = ajustaMemoria(destinoP, strlen(destinoP));
	fprintf(F1, "%s;", destinoP);
	auxV -> destinoP = destinoP;
	
	printf("Numero de cidades visitadas: ");
	scanf("%d", &nCidades);
	fflush(stdin);
	fprintf(F1, "%d", nCidades);
	auxV->nCidades = nCidades;
	
	auxV->cidades = NULL;
	for(i = 0; i < nCidades; i++)
	{
		printf("Cidade numero %d: ", i + 1);
		scanf("%s", cidade);
		fflush(stdin);
		fprintf(F1, ";%s", cidade);
		insereCidade(auxV, cidade);
	}
	
	fprintf(F1, "\n");
	fclose(F1);
	
	auxV -> seg = NULL;
	actualizaDados(auxA, auxV);
	insereViagem(auxA, auxV);
}

void leFicheiro(FILE *F1, long int pos)
{
	int dia, mes, ano, duracao, meioT, kmPercorridos, custo, nCidades, i;
	char *destinoP, *info, c;
	struct Cidade *auxC;
	struct Data auxD;
	struct Ano *auxA;
	struct Viagem *auxV;
	
	fseek(F1, pos, SEEK_SET);
	
	c = fgetc(F1);
	if(c != EOF)
	{
		auxA = malloc(sizeof(struct Ano));
		auxV = malloc(sizeof(struct Viagem));
		destinoP = malloc(50 * sizeof(char));
		info = malloc(50 * sizeof(char));
		
		i = 0;
		while(c != ';')
		{
			*(info + i) = c;
			c = fgetc(F1);
			i++;
		}
		*(info + i) = '\0';
		dia = atoi(info);
		auxD.dia = dia;
		
		i = 0;
		c = fgetc(F1);
		while(c != ';')
		{
			*(info + i) = c;
			c = fgetc(F1);
			i++;
		}
		*(info + i) = '\0';
		mes = atoi(info);
		auxD.mes = mes;
		
		i = 0;
		c = fgetc(F1);
		while(c != ';')
		{
			*(info + i) = c;
			c = fgetc(F1);
			i++;
		}
		*(info + i) = '\0';
		ano = atoi(info);
		
		if(!verificaData(auxD, ano))
		{
			printf("Data invalida...");
		}
		else
		{
			auxV->diaIni = auxD;
			
			if(procuraAno(ano) == NULL)
			{
				adicionaAno(ano);
			}
			auxA = procuraAno(ano);
			
			i = 0;
			c = fgetc(F1);
			while(c != ';')
			{
				*(info + i) = c;
				c = fgetc(F1);
				i++;
			}
			*(info + i) = '\0';
			duracao = atoi(info);
			auxV->duracao = duracao;
			
			i = 0;
			c = fgetc(F1);
			while(c != ';')
			{
				*(info + i) = c;
				c = fgetc(F1);
				i++;
			}
			*(info + i) = '\0';
			meioT = atoi(info);
			auxV->meioT = meioT;
			
			i = 0;
			c = fgetc(F1);
			while(c != ';')
			{
				*(info + i) = c;
				c = fgetc(F1);
				i++;
			}
			*(info + i) = '\0';
			kmPercorridos = atoi(info);
			auxV->kmPercorridos = kmPercorridos;
			
			i = 0;
			c = fgetc(F1);
			while(c != ';')
			{
				*(info + i) = c;
				c = fgetc(F1);
				i++;
			}
			*(info + i) = '\0';
			custo = atoi(info);
			auxV->custo = custo;
			
			i = 0;
			c = fgetc(F1);
			while(c != ';')
			{
				*(info + i) = c;
				c = fgetc(F1);
				i++;
			}
			*(info + i) = '\0';
			memmove(destinoP, info, strlen(info) + 1);
			destinoP = ajustaMemoria(destinoP, strlen(destinoP));
			auxV->destinoP = destinoP;
			
			i = 0;
			c = fgetc(F1);
			while(c != ';')
			{
				*(info + i) = c;
				c = fgetc(F1);
				i++;
			}
			*(info + i) = '\0';
			nCidades = atoi(info);
			auxV->nCidades = nCidades;
			
			auxV->cidades = NULL;		
			insereCidade(auxV, NULL);
			i = 0;
			c = fgetc(F1);
			while(c != ';' && c != EOF && c != '\n')
			{
				*(info + i) = c;
				c = fgetc(F1);
				i++;
			}
			*(info + i) = '\0';
			auxV->cidades->cidade = malloc(strlen(info) * sizeof(char));
			memmove(auxV->cidades->cidade, info, strlen(info) + 1);
			
			auxC = auxV->cidades;
			while(c != EOF && c != '\n')
			{
				insereCidade(auxV, NULL);
				auxC = auxC->seg;
				i = 0;
				c = fgetc(F1);
				while(c != ';' && c != EOF && c != '\n')
				{
					*(info + i) = c;
					c = fgetc(F1);
					i++;
				}
				*(info + i) = '\0';
				auxC->cidade = malloc(strlen(info) * sizeof(char));
				memmove(auxC->cidade, info, strlen(info) + 1);
			}
			auxV->seg = NULL;
			auxA->viagens = NULL;
			actualizaDados(auxA, auxV);
			insereViagem(auxA, auxV);
			pos = ftell(F1);
			leFicheiro(F1, pos);
		}
	}
}

void carregaFicheiro()
{
	FILE *F1;
	long int pos = 0;
	if(fopen("DiarioViagens.txt", "r") == NULL)
	{
		F1 = fopen("DiarioViagens.txt", "w");
		printf("Como nao foi encontrado nenhum ficheiro com o nome \"DiarioViagens.txt\",\nfoi criado um vazio\n");
	}
	else
	{
		F1 = fopen("DiarioViagens.txt", "r");
		leFicheiro(F1, pos);
		fclose(F1);
	}
}

int percentagem(int a, int b)
{
	int res;
	if(b == 0)
	{ //se "a" ou "b" forem iguais a zero
		return -1;
	}
	else
	{
		assert(a >= 0);
		assert(b > 0);
		assert(a <= b);
		res = (a*100)/b;
		return res;
	}
}

int jaExiste(char *cidades, int *indices, char *nome, int i)			//verifica se nome ja existe em cidades
{
	int j;
	for(j = 0; j < i; j++)
	{
		if(strcmp(cidades + *(indices + j), nome) == 0)
		{
			return 1;
		}
	}
	return 0;
}

int contaCidades(int k)		// k = 0 sem printf's | k = 1 com printf's
{
	struct Ano *auxA;
	struct Viagem *auxV;
	struct Cidade *auxC;
	char *cidades;
	int *indices;
	int i, j;
	i = 0;
	j = 0;
	
	cidades = malloc(200 * sizeof(char));
	indices = malloc(20 * sizeof(int));
	
	*indices = 0;
	for(auxA = calendario->seg; auxA != NULL; auxA = auxA -> seg)
	{
		for(auxV = auxA -> viagens; auxV != NULL; auxV = auxV -> seg)
		{
			for(auxC = auxV -> cidades; auxC != NULL; auxC = auxC -> seg)
			{
				if(!jaExiste(cidades, indices, auxC->cidade, i))
				{
					memcpy(cidades + j, auxC->cidade, strlen(auxC->cidade));
					j += strlen(auxC->cidade);
					*(cidades + j) = '\0';
					j++;
					i++;
					*(indices + i) = j;
				}
			}
		}
	}
	
	if(k == 1)
	{
		for(j = 0; j < i; j++)
		{
			printf("%s\n", cidades + *(indices + j));
		}
	}
	
	free(cidades);
	free(indices);
	
	return i;
}

int contaCidadesAno(struct Ano *corrente)
{
	struct Viagem *auxV;
	struct Cidade *auxC;
	char *cidades;
	int *indices;
	int i, j;
	i = 0;
	j = 0;
	
	cidades = malloc(200 * sizeof(char));
	indices = malloc(20 * sizeof(int));
	
	*indices = 0;
	for(auxV = corrente -> viagens; auxV != NULL; auxV = auxV -> seg)
	{
		for(auxC = auxV -> cidades; auxC != NULL; auxC = auxC -> seg)
		{
			if(!jaExiste(cidades, indices, auxC->cidade, i))
			{
				memcpy(cidades + j, auxC->cidade, strlen(auxC->cidade));
				j += strlen(auxC->cidade);
				*(cidades + j) = '\0';
				j++;
				i++;
				*(indices + i) = j;
			}
		}
	}
	
	free(cidades);
	free(indices);
	
	return i;
}

int contaPaises(int k)		// k = 0 sem printf's | k = 1 com printf's
{
	struct Ano *auxA;
	struct Viagem *auxV;
	char *paises;
	int *indices;
	int i, j;
	i = 0;
	j = 0;
	
	paises = malloc(200 * sizeof(char));
	indices = malloc(20 * sizeof(int));
	
	*indices = 0;
	for(auxA = calendario->seg; auxA != NULL; auxA = auxA -> seg)
	{
		for(auxV = auxA -> viagens; auxV != NULL; auxV = auxV -> seg)
		{
			if(!jaExiste(paises, indices, auxV->destinoP, i))
			{
				memcpy(paises + j, auxV->destinoP, strlen(auxV->destinoP));
				j += strlen(auxV->destinoP);
				*(paises + j) = '\0';
				j++;
				i++;
				*(indices + i) = j;
			}
		}
	}
	
	if(k == 1)
	{
		for(j = 0; j < i; j++)
		{
			printf("%s\n", paises + *(indices + j));
		}
	}
	
	free(paises);
	free(indices);
	
	return i;
}

int contaPaisesAno(struct Ano *corrente)
{
	struct Viagem *auxV;
	char *paises;
	int *indices;
	int i, j;
	i = 0;
	j = 0;
	
	paises = malloc(200 * sizeof(char));
	indices = malloc(20 * sizeof(int));
	
	*indices = 0;
	for(auxV = corrente->viagens; auxV != NULL; auxV = auxV->seg)
	{
		if(!jaExiste(paises, indices, auxV->destinoP, i))
		{
			memcpy(paises + j, auxV->destinoP, strlen(auxV->destinoP));
			j += strlen(auxV->destinoP);
			*(paises + j) = '\0';
			j++;
			i++;
			*(indices + i) = j;
		}
	}
	
	free(paises);
	free(indices);
	
	return i;
}

void imprimeDespesaViagem()
{
	struct Ano *auxA;
	struct Viagem *auxV;

	for(auxA = calendario->seg; auxA != NULL; auxA = auxA->seg)
	{
		for(auxV = auxA->viagens; auxV != NULL; auxV = auxV->seg)
		{
			printf("Viagem %d/%d/%d: %d de despesa.\n",auxV->diaIni.dia,auxV->diaIni.mes,auxA->ano,auxV->custo);
		}
	}
}

void imprimeDespesaAno()
{
	struct Ano *auxA;

	for(auxA = calendario->seg; auxA != NULL; auxA = auxA->seg)
	{
		printf("Ano %d: %d de despesa.\n", auxA->ano, auxA->despesa);
	}
}

void despesaViagemPercentagem()
{
	struct Ano *auxA;
	struct Viagem *auxV;

	for(auxA = calendario->seg; auxA != NULL; auxA = auxA->seg)
	{
		for(auxV = auxA->viagens; auxV != NULL; auxV = auxV->seg)
		{
			printf("Viagem %d/%d/%d: %d%%.\n",auxV->diaIni.dia, auxV->diaIni.mes, auxA->ano, percentagem(auxV->custo, calendario->despesa));
		}
	}
}

void despesaAnoPercentagem()
{
	struct Ano *auxA;
	
	for(auxA = calendario->seg; auxA != NULL; auxA = auxA -> seg)
	{
		printf("Ano %d: %d%%.\n",auxA->ano,percentagem(auxA->despesa, calendario->despesa));
	}
}

void imprimeDiasViagem()
{
	struct Ano *auxA;
	struct Viagem *auxV;

	for(auxA = calendario->seg; auxA != NULL; auxA = auxA->seg)
	{
		for(auxV = auxA->viagens; auxV != NULL; auxV = auxV->seg)
		{
			printf("Viagem %d/%d/%d: %d dias.\n",auxV->diaIni.dia, auxV->diaIni.mes, auxA->ano, auxV->duracao);
		}
	}
}

void imprimeDiasViagemPercentagem()
{
	struct Ano *auxA;
	struct Viagem *auxV;

	for(auxA = calendario->seg; auxA != NULL; auxA = auxA->seg)
	{
		for(auxV = auxA->viagens; auxV != NULL; auxV = auxV->seg)
		{
			printf("Viagem %d/%d/%d: %d%%.\n",auxV->diaIni.dia,auxV->diaIni.mes, auxA->ano, percentagem(auxV->duracao,calendario->diasViagem));
		}
	}
}

void imprimeDiasViagemAno()
{
	struct Ano *auxA;

	for(auxA = calendario->seg; auxA != NULL; auxA = auxA->seg)
	{
		printf("Ano %d: %d dias de viagem.\n",auxA->ano, auxA->diasViagem);
	}
}

void imprimeDiasViagemAnoPercentagem()
{
	struct Ano *auxA;

	for(auxA = calendario->seg; auxA != NULL; auxA = auxA->seg)
	{
		printf("Ano %d: %d%%.\n", auxA->ano, percentagem(auxA->diasViagem, calendario->diasViagem));
	}
}

void consultaInformacao()
{
	int k;
	struct Ano *corrente;

	printf("\n\tPaises e cidades visitadas\n");

	printf("\nPaises:\n");
	k = contaPaises(1);
	printf("\nTotal de paises: %d\n", k);

	printf("\nTotal de paises por ano: \n" );
	for(corrente = calendario->seg; corrente != NULL; corrente = corrente->seg)
	{
		k = contaPaisesAno(corrente);
		printf("%d: %d paises\n",corrente->ano,k);
	}

	printf("\nCidades:\n");
	k = contaCidades(1);
	
	printf("\nTotal de cidades: %d\n", k);

	printf("\nTotal de cidades por ano:\n" );
	for(corrente = calendario->seg; corrente != NULL; corrente = corrente->seg)
	{
		printf("%d: %d cidades\n", corrente->ano, contaCidadesAno(corrente));
	}

	printf("\n\tNumero de quilometros percorridos:\n\n");
	for(corrente = calendario->seg; corrente != NULL; corrente = corrente->seg)
	{
		printf("%d: %dkm\n", corrente->ano, corrente->kmCarro + corrente->kmAviao);
	}
	
	printf("\nTotal de quilometros percorridos: %d\n", (calendario->kmCarro + calendario->kmAviao + calendario->kmComboio));
	
	printf("Total de quilometros percorridos de Aviao: %d\n", calendario->kmAviao);
	printf("Total de quilometros percorridos de Carro: %d\n", calendario->kmCarro);
	printf("Total de quilometros percorridos de Comboio: %d\n\n", calendario->kmComboio);

	printf("\n\tDespesas:\n");
	printf("\nTotal de despesas: %d\n", calendario->despesa);

	printf("\nDespesa por viagem :\n");
	imprimeDespesaViagem();

	printf("\nDespesa por ano :\n");
	imprimeDespesaAno();

	printf("\n\tDias de viagem:\n");
	printf("\nTotal de dias de viagem: %d\n", calendario->diasViagem);

	printf("\nDias de viagem por viagem:\n");
	imprimeDiasViagem();

	printf("\nDias de viagem por ano:\n");
	imprimeDiasViagemAno();
}

void consultaPercentagens()
{
	struct Ano *corrente;
	
	printf("Valor percentual do numero de paises visitados por ano: \n" );
	for(corrente = calendario->seg; corrente != NULL; corrente = corrente->seg)
	{
		printf("%d: %d%%\n ", corrente->ano, percentagem(contaPaisesAno(corrente), contaPaises(0)));
	}

	printf("Valor percentual de cidades visitados por ano:\n");
	for(corrente = calendario; corrente != NULL; corrente = corrente->seg)
	{
		printf("%d: %d%%\n ",corrente->ano, percentagem(contaCidadesAno(corrente), contaCidades(0)));
	}

	printf("Valor percentual de quilómetros percorridos por ano:\n");
	for(corrente = calendario; corrente != NULL; corrente = corrente->seg)
	{
		printf("%d: %d%%\n ", corrente->ano, percentagem(corrente->kmCarro + corrente->kmAviao, calendario->kmAviao + calendario->kmCarro));
	}
	
	printf("Valor percentual de quilómetros percorridos de Aviao: %d\n", percentagem(corrente->kmAviao, calendario->kmAviao));
	printf("Valor percentual de quilómetros percorridos de Carro: %d\n", percentagem(corrente->kmCarro, calendario->kmCarro));
	
	printf("Valor percentual de despesas por viagem :\n");
	despesaViagemPercentagem(calendario->despesa);		//pega no k, faz a percentagem gasta em cada uma das viagens em relação a k e imprime no terminal

	printf("Valor percentual de despesas por ano :\n");
	despesaAnoPercentagem(calendario->despesa);
	
	printf("Valor percentual da duracao de cada viagem:\n");
	imprimeDiasViagemPercentagem(calendario->diasViagem);

	printf("Valor percentual dos dias de viagem por ano:\n");
	imprimeDiasViagemAnoPercentagem(calendario->diasViagem);
}
/*
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
*/
void menu()
{
	int o, s = 1;
	while(s){
		printf("||||||||||||||||||||||||||||||||||||||||||||||||||\n");
		printf("||                                              ||\n");
		printf("||              Diario de Viagens               ||\n");
		printf("||                                              ||\n");
		printf("||      1 - Adiciona viagem                     ||\n");
		printf("||      2 - Consultar informacao                ||\n");
		printf("||      3 - Consultar informacao percentual     ||\n");
		printf("||      4 - Criar relatorio (ecra)              ||\n"); // dos países onde visitou mais cidades e onde gastou mais dinheiro
		printf("||      5 - Criar relatorio (ficheiro)          ||\n");
		printf("||      6 - Sair                                ||\n");
		printf("||                                              ||\n");
		printf("||||||||||||||||||||||||||||||||||||||||||||||||||\n");
		scanf("%d", &o);
		switch(o)
		{
			case 1:
			criaViagem();
			fflush(stdin);
			printf("Clique ENTER para continuar...");
			getchar();
			break;

			case 2:
			consultaInformacao();
			fflush(stdin);
			printf("Clique ENTER para continuar...");
			getchar();
			break;

			case 3:
			consultaPercentagens();
			fflush(stdin);
			printf("Clique ENTER para continuar...");
			getchar();
			break;

			case 4:
			//criaRelatorioe();
			fflush(stdin);
			printf("Clique ENTER para continuar...");
			getchar();
			break;

			case 5:
			//criaRelatoriof();
			fflush(stdin);
			printf("Clique ENTER para continuar...");
			getchar();
			break;

			case 6:
			s = 0;
			break;

			default:
			printf("Parametro invalido...\n");
		}
	}
}

int main()
{
	criaCalendario();
	carregaFicheiro();
	//printf("%d\n", calendario->seg->kmCarro);
	menu();
	return 0;
}

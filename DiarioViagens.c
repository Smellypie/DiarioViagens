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
	calendario->despesa = 0;											//inicialização da base com todos os valores iguais a 0
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
	for(i = 0; i < len; i++)											//passa o conteúdo de str para novo
	{
		*(novo + i) = *(str + i);
	}
	*(novo + len) = '\0';
	free(str);															//apaga str
	return novo;
}

void actualizaDados(struct Ano *ano, struct Viagem *viagem)				//actualiza os valores dos custos ... totais e por ano
{
	ano->despesa += viagem->custo;
	ano->nViagens++;
	ano->nDestinos += viagem->nCidades;
	if(viagem->meioT == 1)												//verifica o meio de transporte usado
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
	if(viagem->meioT == 1)												//verifica o meio de transporte usado
	{
		calendario->kmAviao += viagem->kmPercorridos;
	}
	if(viagem->meioT == 2)
	{
		calendario->kmCarro += viagem->kmPercorridos;
	}
	if(viagem->meioT == 3)
	{
		calendario->kmComboio += viagem->kmPercorridos;
	}
	calendario->diasViagem += viagem->duracao;
}

void adicionaAno(int ano)												//Cria a struct de um novo ano
{
	struct Ano *novo, *aux;
	novo = malloc(sizeof(struct Ano));
	novo->despesa = 0;													//inicialização da struct Ano com todos os valores iguais a 0
	novo->nViagens = 0;
	novo->nDestinos = 0;
	novo->kmAviao = 0;
	novo->kmCarro = 0;
	novo->kmComboio = 0;
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
	printf("\nVALORES PERCENTUAIS ARREDONDADOS AS UNIDADES\n");
	printf("\nValor percentual do numero de paises visitados por ano: \n");
	for(corrente = calendario->seg; corrente != NULL; corrente = corrente->seg)
	{
		printf("%d: %d%%\n", corrente->ano, percentagem(contaPaisesAno(corrente), contaPaises(0)));
	}

	printf("\nValor percentual de cidades visitados por ano:\n");
	for(corrente = calendario->seg; corrente != NULL; corrente = corrente->seg)
	{
		printf("%d: %d%%\n",corrente->ano, percentagem(contaCidadesAno(corrente), contaCidades(0)));
	}

	printf("\nValor percentual de quilometros percorridos por ano:\n");
	for(corrente = calendario->seg; corrente != NULL; corrente = corrente->seg)
	{
		printf("%d: %d%%\n", corrente->ano, percentagem(corrente->kmCarro + corrente->kmAviao + corrente->kmComboio, calendario->kmAviao + calendario->kmCarro + calendario->kmComboio));
	}
	
	printf("\nValor percentual de quilometros percorridos de Aviao: %d%%\n", percentagem(calendario->kmAviao, calendario->kmAviao + calendario->kmCarro + calendario->kmComboio));
	printf("Valor percentual de quilometros percorridos de Carro: %d%%\n", percentagem(calendario->kmCarro, calendario->kmAviao + calendario->kmCarro + calendario->kmComboio));
	printf("Valor percentual de quilometros percorridos de Comboio: %d%%\n", percentagem(calendario->kmComboio, calendario->kmAviao + calendario->kmCarro + calendario->kmComboio));
	
	printf("\nValor percentual de despesas por viagem :\n");
	despesaViagemPercentagem(calendario->despesa);		//pega no k, faz a percentagem gasta em cada uma das viagens em relação a k e imprime no terminal

	printf("\nValor percentual de despesas por ano :\n");
	despesaAnoPercentagem(calendario->despesa);
	
	printf("\nValor percentual da duracao de cada viagem:\n");
	imprimeDiasViagemPercentagem(calendario->diasViagem);

	printf("\nValor percentual dos dias de viagem por ano:\n");
	imprimeDiasViagemAnoPercentagem(calendario->diasViagem);
}

struct Cidade *criaCidade()
{
	struct Cidade *auxC;
	
	auxC = malloc (sizeof(struct Cidade));
	
	return auxC;
	
}

void freeCidade(struct Cidade *cidade)
{
	struct Cidade *aux1;
	struct Cidade *aux2;

	//Dar free a todos os locais de memoria, comecando do fim para o inicio
	while(cidade->seg!=NULL)
	{
		aux1=cidade;
		while(aux1->seg!=NULL)
		{
			aux2=aux1;
			aux1=aux1->seg;
		}
		free(aux1);
		aux2->seg=NULL;
	}

	free(cidade);
}

void freeViagem(struct Viagem *viagem)
{
	struct Viagem *aux1;
	struct Viagem *aux2;
	
	//Dar free a todos os locais de memoria, comecando do fim para o inicio
	while(viagem->seg!=NULL)
	{
		aux1=viagem;
		while(aux1->seg!=NULL)
		{
			aux2=aux1;
			aux1=aux1->seg;
		}
		free(aux1);
		freeCidade(aux1->cidades);
		aux2->seg=NULL;
	}

	//da free a primeira posicao de memoria
	free(viagem);
}

void criaRelatorioE(int o){ //atençao a ordem alfabetica
	struct Ano *auxA;
	struct Viagem *auxV;
	struct Viagem *auxV2;
	struct Viagem *listaP; //lista de paises
	struct Cidade *auxC;
	struct Cidade *auxC2;
	struct Cidade *auxC4;
	char *paises;
	char *data;
	char *ficheiro;
	FILE *fp;
	int *indices;
	int  i, j, jaexiste, contador;
	float a, b, c;
	a=0;
	b=0;
	a = 0;
	b = 0;
	c = 0;
	i = 0;
	j = 0;
	jaexiste = 0;
	
	ficheiro= malloc(50 * sizeof(char));
	data = malloc(20 * sizeof(char));
	paises = malloc(200 * sizeof(char));
	indices = malloc(20 * sizeof(int));
	listaP = malloc (sizeof(struct Viagem));
	auxC = malloc (sizeof(struct Cidade));
	
	listaP->cidades=auxC;
	auxC->seg=NULL;

	*indices = 0;
	for(auxA = calendario->seg; auxA != NULL; auxA = auxA -> seg)					//ano a ano
	{
		for(auxV = auxA -> viagens; auxV != NULL; auxV = auxV -> seg)				//viagem a viagem
		{
			if(!jaExiste(paises, indices, auxV->destinoP, i))						//"matriz" do luis
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
	auxV = listaP;
	for(j = 0; j != i; j++)															//Inserir os países	na listaP			
	{
		auxV -> destinoP = malloc(60*sizeof(char));
		strcpy(auxV -> destinoP,paises + *(indices + j));
		auxV2 = malloc(sizeof(struct Viagem));
		auxV2 -> seg = NULL;
		auxV2 -> destinoP = "Dummy";
		auxV -> seg = auxV2;
		auxV = auxV -> seg;
	}
	
	for(auxV = listaP; auxV != NULL; auxV = auxV -> seg)							//Inicializar o struct cidade da listaP
	{
		auxV -> custo = 0;//Inicializar o custo a zero
		auxV -> cidades = malloc(sizeof(struct Cidade));
		auxV -> cidades -> seg = NULL;
		auxV -> cidades -> cidade = malloc(50*sizeof(char));
	}
	
	for(auxA = calendario->seg; auxA != NULL; auxA = auxA -> seg)//ano a ano
	{
		for(auxV = auxA -> viagens; auxV != NULL; auxV = auxV -> seg)//viagem a viagem
		{
			for(auxV2 = listaP; auxV2 != NULL; auxV2= auxV2 -> seg)//para cada viagem analisa a listaP
			{
				if(strcmp(auxV -> destinoP, auxV2 -> destinoP) == 0)//se o nome do pais na lista for igual ao nome do pais da viagem
				{
					auxV2->custo=auxV2->custo + auxV->custo;//adicionar o custo da viagem ao custo total desse pais
					for(auxC = auxV -> cidades; auxC != NULL; auxC = auxC -> seg)//adicionar cidade da viagem a ao pais da listaP
					{
						for(auxC2 = auxV2 -> cidades; auxC2 != NULL; auxC2 = auxC2 -> seg)//ver todas as cidades ja existentes nesse pais da listaP
						{
						
							if(strcmp(auxC -> cidade, auxC2 -> cidade) == 0)//se esse pais ja existir não faz nada
							{
								jaexiste = 1;
							}
							auxC4=auxC2;//ponteiro anterior
						}
						if(jaexiste != 1)//como o pais nao existe, addiciona-o ao struct Cidade dessa lista p
						{
							auxC4 -> seg = malloc(sizeof(struct Cidade));
							auxC4->seg->cidade = malloc(20*sizeof(char));
							strcpy(auxC4 -> cidade, auxC -> cidade);
							strcpy(auxC4 -> seg -> cidade, "Dummy");
							auxC4 -> seg -> seg = NULL;
						}
						else
						{
							jaexiste = 0;
						}
						
					}
				}
			}
		}	
	}
	
	for(auxV = listaP; auxV != NULL; auxV = auxV -> seg)//olhando para todos os paises da listaP
	{
		contador = 0;
		for(auxC = auxV -> cidades; auxC != NULL; auxC = auxC -> seg)//Olhando para todas as cidades
		{
			if(strcmp(auxC->cidade, "Dummy") != 0)//Conta todas as cidades válidas
			{
				contador++;
			}
		}
		auxV -> nCidades = contador;//Guarda quantas cidades foram visitadas nesse pais
	}
	
	for(auxV = listaP; auxV != NULL; auxV = auxV -> seg)//olhando para todos os paises da listaP
	{
		if(strcmp(auxV->destinoP, "Dummy")!=0) //Para todos os países válidos
		{
			c = c + auxV -> custo;		//soma o custo
			a = a + auxV -> nCidades;	//soma o numero de cidades
			b++;
		}
	}
	
	a = a / b;		//a passa a ter o valor da media de cidades
	c = c / b;		//a passa a ter o valor da media do custo
	
	if(o==1)
	{
		printf("Paises onde visitou mais cidades:\n");
		for(auxV = listaP; auxV != NULL; auxV = auxV -> seg)
		{
			if(auxV -> nCidades >= a && strcmp(auxV->destinoP, "Dummy")!=0)//Para todos os paises válidos
			{
				printf("%s\n", auxV -> destinoP);
			}
		}
		
		printf("Paises onde se gastou mais dinheiro:\n");
		for(auxV = listaP; auxV != NULL; auxV = auxV -> seg)
		{
			if(auxV -> custo >= c && strcmp(auxV->destinoP, "Dummy")!=0)//Para todos os paises válidos
			{
				printf("%s\n", auxV -> destinoP);
			}
		}
		
	}
	
	if(o==2)
	{
		printf("Introduza a data atual (Relatorio_\"data\"): \n");
		scanf("%s", data);
		sprintf(ficheiro,"Relatorio_%s.txt",data);
		fp=fopen(ficheiro,"w");
		
		fprintf(fp,"Paises onde visitou mais cidades:\n");
		for(auxV = listaP; auxV != NULL; auxV = auxV -> seg)
		{
			if(auxV -> nCidades >= a && strcmp(auxV->destinoP, "Dummy")!=0)//Para todos os paises válidos
			{
				fprintf(fp,"%s\n", auxV -> destinoP);
			}
		}
		
		fprintf(fp,"Paises onde se gastou mais dinheiro:\n");
		for(auxV = listaP; auxV != NULL; auxV = auxV -> seg)
		{
			if(auxV -> custo >= c && strcmp(auxV->destinoP, "Dummy")!=0)//Para todos os paises válidos
			{
				fprintf(fp,"%s\n", auxV -> destinoP);
			}
		}
		fclose(fp);
	}
	
	
	

	freeViagem(listaP);
	free(paises);
	free(indices);
	free(auxV2);
	
	}

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
		system("cls");
		switch(o)
		{
			case 1:
			criaViagem();
			fflush(stdin);
			printf("Clique ENTER para continuar...");
			getchar();
			system("cls");
			break;

			case 2:
			consultaInformacao();
			fflush(stdin);
			printf("Clique ENTER para continuar...");
			getchar();
			system("cls");
			break;

			case 3:
			consultaPercentagens();
			fflush(stdin);
			printf("Clique ENTER para continuar...");
			getchar();
			system("cls");
			break;

			case 4:
			criaRelatorioE(1);
			fflush(stdin);
			printf("Clique ENTER para continuar...");
			getchar();
			system("cls");
			break;

			case 5:
			criaRelatorioE(2);
			fflush(stdin);
			printf("Clique ENTER para continuar...");
			getchar();
			system("cls");
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
	menu();
	return 0;
}

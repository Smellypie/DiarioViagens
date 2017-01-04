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
	
	printf("Dia de inicio da viagem(dd mm aaaa): \n");
	scanf("%d%d%d", &dia, &mes, &ano);
	fflush(stdin);
	fprintf(F1, "%d;%d;%d;", dia, mes, ano);
	if(procuraAno(ano) == NULL)
	{
		adicionaAno(ano);
	}
	auxA = procuraAno(ano);
	auxD.dia = dia;
	auxD.mes = mes;
	auxV -> diaIni = auxD;
	
	printf("Duracao da viagem(em dias): \n");
	scanf("%d", &duracao);
	fflush(stdin);
	fprintf(F1, "%d;", duracao);
	auxV -> duracao = duracao;
	
	printf("Meio de transporte usado:\n1 - Aviao\n2 - Carro\n");
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

void leFicheiro()
{
	FILE *F1;
	int dia, mes, ano, duracao, meioT, kmPercorridos, custo, nCidades, i;
	char *destinoP, *info, c;
	struct Cidade *auxC;
	struct Data auxD;
	struct Ano *auxA;
	struct Viagem auxV;
	
	auxA = malloc(sizeof(struct Ano));
	destinoP = malloc(50 * sizeof(char));
	info = malloc(50 * sizeof(char));
	
	if(fopen("DiarioViagens.txt", "r") == NULL)
	{
		F1 = fopen("DiarioViagens.txt", "w");
		printf("Como nao foi encontrado nenhum ficheiro com o nome \"DiarioViagens.txt\",\nfoi criado um vazio\n");
	}
	else
	{
		F1 = fopen("DiarioViagens.txt", "r");
		i = 0;
		c = fgetc(F1);
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
		auxV.diaIni = auxD;
		
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
		auxV.duracao = duracao;
		
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
		auxV.meioT = meioT;
		
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
		auxV.kmPercorridos = kmPercorridos;
		
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
		auxV.custo = custo;
		
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
		auxV.destinoP = destinoP;
		
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
		auxV.nCidades = nCidades;
		
		auxV.cidades = NULL;		
		insereCidade(&auxV, NULL);
		i = 0;
		c = fgetc(F1);
		while(c != ';' && c != EOF && c != '\n')
		{
			*(info + i) = c;
			c = fgetc(F1);
			i++;
		}
		*(info + i) = '\0';
		auxV.cidades->cidade = malloc(strlen(info) * sizeof(char));
		memmove(auxV.cidades->cidade, info, strlen(info));
		
		auxC = auxV.cidades;
		while(c != EOF && c != '\n')
		{
			insereCidade(&auxV, NULL);
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
			memmove(auxC->cidade, info, strlen(info));
		}
		
		auxV.seg = NULL;
		auxA->viagens = NULL;
		insereViagem(auxA, &auxV);
		fclose(F1);
	}
}

struct Cidade *criaCidade()
{
	struct Cidade *aux = malloc(sizeof(struct Cidade));
	aux -> seg = NULL;
	aux -> cidade = "Dummy";
	return aux;
}


int percentagem(int a, int b){
	int res;
	if(a == 0 || b == 0){ //se "a" ou "b" forem iguais a zero
		return -1;
		}
	else{
		assert(a > 0);
		assert(b > 0);
		assert(a < b);
		res = (a*100)/b;
		return res;
		}
	}

int imprimeCidades()
{
	int jaexiste;
	int conta;
	struct Ano *auxA;
	struct Viagem *auxV;
	struct Cidade *auxC;
	struct Cidade *todas;
	struct Cidade *todasaux1;
	struct Cidade *todasaux2;
	struct Cidade *todasprox;
	conta=0;
	jaexiste=0;
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
		if(strcmp(todasaux1 -> cidade,"Dummy") != 0){						//Se o nomme da cidade for diferente do dummy(pode haver algumas partes do todas que teem Dummy escrito la dentro)
			printf("%s\n", todasaux1 -> cidade);			//falta por por ordem alfabetica
			conta++;
			}
		}
	return conta;
	}

int contaCidades()
{
	int jaexiste;
	int conta;
	struct Ano *auxA;
	struct Viagem *auxV;
	struct Cidade *auxC;
	struct Cidade *todas;
	struct Cidade *todasaux1;
	struct Cidade *todasaux2;
	struct Cidade *todasprox;
	conta=0;
	jaexiste=0;
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
		if(strcmp(todasaux1 -> cidade,"Dummy") != 0){						//Se o nomme da cidade for diferente do dummy(pode haver algumas partes do todas que teem Dummy escrito la dentro)
			conta++;
			}
		}
	return conta;
	}

int contaCidadesAno(struct Ano *corrente){

	int jaexiste;
	int conta;
	struct Viagem *auxV;
	struct Cidade *auxC;
	struct Cidade *todas;
	struct Cidade *todasaux1;
	struct Cidade *todasaux2;
	struct Cidade *todasprox;
	conta=0;
	jaexiste=0;
	auxV = malloc(sizeof(struct Viagem));
	auxC = malloc(sizeof(struct Cidade));
	todas = malloc(sizeof(struct Cidade));
	todasaux1 = malloc(sizeof(struct Cidade));
	todasaux2 = malloc(sizeof(struct Cidade));
	todasaux1 = todas;

	for(auxV = corrente->viagens; auxV != NULL; auxV = auxV -> seg){							//viagem a viagem
		for(auxC = auxV -> cidades; auxC != NULL; auxC = auxC -> seg){						//cidade a cidade

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

	for(todasaux1 = todas; todasaux1 != NULL; todasaux1 = todasaux1 -> seg){				//cidade a cidade
		if(strcmp(todasaux1 -> cidade,"Dummy") != 0){						//Se o nomme da cidade for diferente do dummy(pode haver algumas partes do todas que teem Dummy escrito la dentro)
			conta++;
			}
		}
	return conta;



}



int imprimePaises(){
	int jaexiste;
	int conta;
	struct Ano *auxA;
	struct Viagem *auxV;
	struct Cidade *todas;
	struct Cidade *todasaux1;
	struct Cidade *todasaux2;
	struct Cidade *todasprox;

	conta=0;
	jaexiste=0;
	auxA = malloc(sizeof(struct Ano));
	auxV = malloc(sizeof(struct Viagem));
	todas = malloc(sizeof(struct Cidade));
	todasaux1 = malloc(sizeof(struct Cidade));
	todasaux2 = malloc(sizeof(struct Cidade));
	todasaux1 = todas;
	for(auxA = calendario; auxA != NULL; auxA = auxA -> seg){								//ano a ano
		for(auxV = auxA -> viagens; auxV != NULL; auxV = auxV -> seg){						//viagem a viagem
			for(todasaux2 = todas; todasaux2 != NULL; todasaux2 = todasaux2 -> seg){	//ver se o pais ja foi guardada no 'todas'
				if(strcmp(todasaux2 -> cidade, auxV -> destinoP) == 0){jaexiste = 1;}}	//verifica se ja existe
			if(jaexiste != 1){															//se o pais nao existe
				strcpy(todasaux1 -> cidade, auxV -> destinoP);
				todasprox = criaCidade();												//criar o proximo sitio para guardar o pais
				todasaux1 -> seg = todasprox;											//juntar à lista todas
				todasaux1 = todasaux1 -> seg;											//avanço do auxiliar
				jaexiste = 0;
				}
			}

		}

	for(todasaux1 = todas; todasaux1 != NULL; todasaux1 = todasaux1 -> seg){				//pais a pais
		if(strcmp(todasaux1 -> cidade,"Dummy") != 0){									//Se o nomme da cidade for diferente do dummy(pode haver algumas partes do todas que teem Dummy escrito la dentro)
			printf("%s\n", todasaux1 -> cidade);											//falta por por ordem alfabetica
			conta++;
			}
		}
	return conta;
	}
	
int contaPaises(){
	int jaexiste;
	int conta;
	struct Ano *auxA;
	struct Viagem *auxV;
	struct Cidade *todas;
	struct Cidade *todasaux1;
	struct Cidade *todasaux2;
	struct Cidade *todasprox;

	conta=0;
	jaexiste=0;
	auxA = malloc(sizeof(struct Ano));
	auxV = malloc(sizeof(struct Viagem));
	todas = malloc(sizeof(struct Cidade));
	todasaux1 = malloc(sizeof(struct Cidade));
	todasaux2 = malloc(sizeof(struct Cidade));
	todasaux1 = todas;
	for(auxA = calendario; auxA != NULL; auxA = auxA -> seg){								//ano a ano
		for(auxV = auxA -> viagens; auxV != NULL; auxV = auxV -> seg){						//viagem a viagem
			for(todasaux2 = todas; todasaux2 != NULL; todasaux2 = todasaux2 -> seg){	//ver se o pais ja foi guardada no 'todas'
				if(strcmp(todasaux2 -> cidade, auxV -> destinoP) == 0){jaexiste = 1;}}	//verifica se ja existe
			if(jaexiste != 1){															//se o pais nao existe
				strcpy(todasaux1 -> cidade, auxV -> destinoP);
				todasprox = criaCidade();												//criar o proximo sitio para guardar o pais
				todasaux1 -> seg = todasprox;											//juntar à lista todas
				todasaux1 = todasaux1 -> seg;											//avanço do auxiliar
				jaexiste = 0;
				}
			}

		}

	for(todasaux1 = todas; todasaux1 != NULL; todasaux1 = todasaux1 -> seg){				//pais a pais
		if(strcmp(todasaux1 -> cidade,"Dummy") != 0){									//Se o nomme da cidade for diferente do dummy(pode haver algumas partes do todas que teem Dummy escrito la dentro)
			conta++;
			}
		}
	return conta;
	}



int contaPaisesAno(struct Ano *corrente){
	int jaexiste;
	int conta;
	struct Viagem *auxV;
	struct Cidade *todas;
	struct Cidade *todasaux1;
	struct Cidade *todasaux2;
	struct Cidade *todasprox;

	conta=0;
	jaexiste=0;
	auxV = malloc(sizeof(struct Viagem));
	todas = malloc(sizeof(struct Cidade));
	todasaux1 = malloc(sizeof(struct Cidade));
	todasaux2 = malloc(sizeof(struct Cidade));
	todasaux1 = todas;

		for(auxV = corrente -> viagens; auxV != NULL; auxV = auxV -> seg){						//viagem a viagem
			for(todasaux2 = todas; todasaux2 != NULL; todasaux2 = todasaux2 -> seg){	//ver se o pais ja foi guardada no 'todas'
				if(strcmp(todasaux2 -> cidade, auxV -> destinoP) == 0){jaexiste = 1;}}	//verifica se ja existe
			if(jaexiste != 1){															//se o pais nao existe
				strcpy(todasaux1 -> cidade, auxV -> destinoP);
				todasprox = criaCidade();												//criar o proximo sitio para guardar o pais
				todasaux1 -> seg = todasprox;											//juntar à lista todas
				todasaux1 = todasaux1 -> seg;											//avanço do auxiliar
				jaexiste = 0;
				}
			}



	for(todasaux1 = todas; todasaux1 != NULL; todasaux1 = todasaux1 -> seg){				//pais a pais
		if(strcmp(todasaux1 -> cidade,"Dummy") != 0){									//Se o nomme do paise for diferente do dummy(pode haver algumas partes do todas que teem Dummy escrito la dentro)
			conta++;
			}
		}
	return conta;
	}
int contaKmAviaoAno(struct Ano *corrente){
	int conta;
	conta=corrente->kmAviao;
	return conta;
}
int contaKmCarroAno(struct Ano *corrente){
	int conta;
	conta=corrente->kmCarro;
	return conta;
}

int contaKmAno(struct Ano *corrente){
	int conta,aux;
	conta=0;
	aux=contaKmAviaoAno(corrente);
	conta=conta + aux;
	aux=contaKmCarroAno(corrente);
	conta=conta + aux;
	return aux;
}



int despesasAno(){

	int despesa;
	struct Ano *auxA;
	despesa=0;
	auxA = malloc(sizeof(struct Ano));

	for(auxA = calendario; auxA != NULL; auxA = auxA -> seg){								//ano a ano
		despesa=despesa+auxA->despesa;
		}
	return despesa;
}

void imprimeDespesaViagem(){
	struct Ano *auxA;
	struct Viagem *auxV;

	auxA = malloc(sizeof(struct Ano));
	auxV = malloc(sizeof(struct Viagem));

	for(auxA = calendario; auxA != NULL; auxA = auxA -> seg){								//ano a ano
		for(auxV = auxA -> viagens; auxV != NULL; auxV = auxV -> seg){						//viagem a viagem
			printf("Viagem %d/%d/%d, %d de despesa.\n",auxV->diaIni.dia,auxV->diaIni.mes,auxA->ano,auxV->custo);
			}
		}
}

void imprimeDespesaAno(){
	struct Ano *auxA;
	auxA = malloc(sizeof(struct Ano));
	for(auxA = calendario; auxA != NULL; auxA = auxA -> seg){								//ano a ano
			printf("Ano %d, %d de despesa.\n",auxA->ano,auxA->despesa);
		}
	}


void percentagemDespesaViagem(int k){

	struct Ano *auxA;
	struct Viagem *auxV;

	auxA = malloc(sizeof(struct Ano));
	auxV = malloc(sizeof(struct Viagem));

	for(auxA = calendario; auxA != NULL; auxA = auxA -> seg){								//ano a ano
		for(auxV = auxA -> viagens; auxV != NULL; auxV = auxV -> seg){						//viagem a viagem
			printf("Viagem %d/%d/%d, %d %%.\n",auxV->diaIni.dia,auxV->diaIni.mes,auxA->ano,percentagem(auxV->custo,k));
			}
		}
}

void percentagemDespesaAno(int k){
	struct Ano *auxA;
	
	auxA = malloc(sizeof(struct Ano));
	for(auxA = calendario; auxA != NULL; auxA = auxA -> seg){								//ano a ano
			printf("Ano %d, %d %%.\n",auxA->ano,percentagem(auxA->despesa,k));
		}
	}

int totalDiasViagem(){

	int soma;
	struct Ano *auxA;
	struct Viagem *auxV;
	soma=0;
	auxA = malloc(sizeof(struct Ano));
	auxV = malloc(sizeof(struct Viagem));

	for(auxA = calendario; auxA != NULL; auxA = auxA -> seg){								//ano a ano
		for(auxV = auxA -> viagens; auxV != NULL; auxV = auxV -> seg){						//viagem a viagem
			soma=soma+auxV->duracao;
			}
		}
	return soma;
}

void imprimeDiasViagem(){

	struct Ano *auxA;
	struct Viagem *auxV;

	auxA = malloc(sizeof(struct Ano));
	auxV = malloc(sizeof(struct Viagem));

	for(auxA = calendario; auxA != NULL; auxA = auxA -> seg){								//ano a ano
		for(auxV = auxA -> viagens; auxV != NULL; auxV = auxV -> seg){						//viagem a viagem
			printf("Viagem %d/%d/%d, %d dias.\n",auxV->diaIni.dia,auxV->diaIni.mes,auxA->ano,auxV->duracao);
			}
		}
}

void imprimeDiasViagemPercentagem(int k){
	struct Ano *auxA;
	struct Viagem *auxV;

	auxA = malloc(sizeof(struct Ano));
	auxV = malloc(sizeof(struct Viagem));

	for(auxA = calendario; auxA != NULL; auxA = auxA -> seg){								//ano a ano
		for(auxV = auxA -> viagens; auxV != NULL; auxV = auxV -> seg){						//viagem a viagem
			printf("Viagem %d/%d/%d, %d %%.\n",auxV->diaIni.dia,auxV->diaIni.mes,auxA->ano,percentagem(auxV->duracao,k));
			}
		}

	}

void imprimeDiasAno(){

	int soma;
	struct Ano *auxA;
	struct Viagem *auxV;
	soma=0;
	auxA = malloc(sizeof(struct Ano));
	auxV = malloc(sizeof(struct Viagem));

	for(auxA = calendario; auxA != NULL; auxA = auxA -> seg){								//ano a ano
		for(auxV = auxA -> viagens; auxV != NULL; auxV = auxV -> seg){						//viagem a viagem
			soma=soma+auxV->duracao;
			}
		printf("Ano %d, %d dias de viagem.\n",auxA->ano,auxV->duracao);
		soma=0;
		}
}

void imprimeDiasAnoPercentagem(int k){

	int soma;
	struct Ano *auxA;
	struct Viagem *auxV;
	soma=0;
	auxA = malloc(sizeof(struct Ano));
	auxV = malloc(sizeof(struct Viagem));

	for(auxA = calendario; auxA != NULL; auxA = auxA -> seg){								//ano a ano
		for(auxV = auxA -> viagens; auxV != NULL; auxV = auxV -> seg){						//viagem a viagem
			soma=soma+auxV->duracao;
			}
		printf("Ano %d, %d %%.\n",auxA->ano,percentagem(auxV->duracao,k));
		soma=0;
		}
}

void consultaInformacao(){
	int i,k;
	struct Ano *corrente;

	printf("		Países e cidades visitadas \n");

	printf("Países:\n");
	k=imprimePaises();
	imprimePaises();
	printf("Total de países: %d\n", k);

	printf("Total de países por ano: \n" );
	for(corrente=calendario;corrente!=NULL;corrente= corrente -> seg){
			k=contaPaisesAno(corrente);
			printf("Em %d, %d paises.\n ",corrente->ano,k);
			}

	printf("Cidades:\n");
	k=imprimeCidades();

	printf("Total de cidades: %d\n", k);

	printf("Total de cidades por ano:\n" );
	for(corrente=calendario;corrente!=NULL;corrente= corrente -> seg){
		k=contaCidadesAno(corrente);
		printf("Em %d, %d cidades.\n ",corrente->ano,k);
		}

	printf("		Número de quilómetros percorridos:\n");
	i=0;
	for(corrente=calendario;corrente!=NULL;corrente= corrente -> seg){
			k=contaKmAno(corrente);
			i=i+k;
			}
	printf("Total de quilómetros percorridos: %d\n",i);

	printf("Total de quilómetros percorridos por ano:\n");
	for(corrente=calendario;corrente!=NULL;corrente= corrente -> seg){
			k=contaKmAno(corrente);
			printf("Em %d, %d quilometros.\n ",corrente->ano,k);
			}

	i=0;
	for(corrente=calendario;corrente!=NULL;corrente= corrente -> seg){
			k=contaKmAviaoAno(corrente);
			i=i+k;
			}
	printf("Total de quilómetros percorridos de Aviao: %d\n",i);

	i=0;
	for(corrente=calendario;corrente!=NULL;corrente= corrente -> seg){
			k=contaKmCarroAno(corrente);
			i=i+k;
			}
	printf("Total de quilómetros percorridos de Carro: %d\n",i);

	printf("		Despesas:");
	k=despesasAno();
	printf("Total de despesas: %d\n",k);

	printf("Despesa por viagem :\n");
	imprimeDespesaViagem();

	printf("Despesa por ano :\n");
	imprimeDespesaAno();

	printf("		Dias de viagem:");
	k=totalDiasViagem();
	printf("Total de dias de viagem: %d\n",k);

	printf("Dias de viagem por viagem:\n");
	imprimeDiasViagem();

	printf("Dias de viagem por ano:\n");
	imprimeDiasAno();
	}

void consultaPercentagens(){ //esta tudo arredondado as unidades!!!! ????? e agr?
	int i,k,n;
	struct Ano *corrente;

	n=contaPaises();

	printf("Valor percentual de países visitados por ano: \n" );
	for(corrente=calendario;corrente!=NULL;corrente= corrente -> seg){
			k=contaPaisesAno(corrente);

			printf("Em %d, %d %%.\n ",corrente->ano,percentagem(k,n));
			}

	n=contaCidades();

	printf("Valor percentual de cidades visitados por ano:\n" );
	for(corrente=calendario;corrente!=NULL;corrente= corrente -> seg){
		k=contaCidadesAno(corrente);
		printf("Em %d, %d %%.\n ",corrente->ano,percentagem(k,n));
		}

	i=0;
	for(corrente=calendario;corrente!=NULL;corrente= corrente -> seg){
			k=contaKmAno(corrente);
			i=i+k;
			}

	printf("Valor percentual de quilómetros percorridos por ano:\n");
	for(corrente=calendario;corrente!=NULL;corrente= corrente -> seg){
			k=contaKmAno(corrente);
			printf("Em %d, %d %%.\n ",corrente->ano,percentagem(k, i));
			}
	n=i;
	i=0;
	for(corrente=calendario;corrente!=NULL;corrente= corrente -> seg){
			k=contaKmAviaoAno(corrente);
			i=i+k;
			}
	printf("Valor percentual de quilómetros percorridos de Aviao: %d\n",percentagem(i,n));

	i=0;
	for(corrente=calendario;corrente!=NULL;corrente= corrente -> seg){
			k=contaKmCarroAno(corrente);
			i=i+k;
			}
	printf("Valor percentual de quilómetros percorridos de Carro: %d\n",percentagem(i,n));

	k=despesasAno();

	printf("Valor percentual de despesas por viagem :\n");
	percentagemDespesaViagem(k);		//pega no k, faz a percentagem gasta em cada uma das viagens em relação a k e imprime no terminal

	printf("Valor percentual de despesas por ano :\n");
	percentagemDespesaAno(k);

	k=totalDiasViagem();

	printf("Valor percentual da duracao de cada viagem:\n");
	imprimeDiasViagemPercentagem(k);

	printf("Valor percentual dos dias de viagem por ano:\n");
	imprimeDiasAnoPercentagem(k);

	}

void criaRelatorioe(){ //atençao a ordem alfabetica



	}

/*
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
	leFicheiro();
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
				//criaRelatorioe();
				break;

				case 5:
				//criaRelatoriof();
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
	criaCalendario();
	menu();
	return 0;
}

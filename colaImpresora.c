/*
* colaImpresora.c

 *
 *  Created on: 16/11/2014
 *      Author: dani95rico
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define cola_MAX 9+1                   /*TamaÃ±o maximo de la cola. Es una cola circular con un hueco libre.*/
#define nombre_MAX 12+1                  /*Cada fichero debe tener una longitud maxima de 12 caracteres mas el \0.*/
#define almacen_MAX 6                    /*El tamaÃ±o maximo del almacen donde se almacenaran ficheros es 6.*/
#define extension_MAX 4                  /*TamaÃ±o de la extension que se aÃ±adira a cada fichero es 4 ya que aÃ±adiremos ".txt".*/

typedef char tNombre[nombre_MAX];        /*Es el tipo de cada fichero, sera un char con una longitud de 12+1 (nombre_MAX).*/

typedef struct{                          /*Estructura que almacenara los ficheros que son proporcionados por *argv[].*/
	tNombre ficheros[almacen_MAX];
} tAlmacen;

typedef struct{                          /*Estructura que representa la cola, en la cual aÃ±adiremos y eliminaremos ficheros.*/
	tNombre nombre[cola_MAX];
	int inicio;                          /*Indice de control de la cola (inicio).*/
	int final;                           /*Indice de control de la cola (final).*/
} tCola;

void iniciarCola(tCola *puntero);                                   /*Funcion encargada de inicializar la cola mediante los indices de control.*/
void copiarFicherosAlmacen(char *argv[],tAlmacen _ficheros[]);             /*Funcion encargada de copiar los ficheros desde *argv[] al almacen.*/
void extension(tAlmacen _ficheros[]);                             /*Funcion encargada de aÃ±adir la extension ".txt" a cada uno de los ficheros.*/
void mostrarAlmacen(tAlmacen _ficheros[]);               /*Funcion que imprime por pantalla los ficheros del almacen con la extension incluida.*/
int realizarTirada(int _valor);                          /*Funcion encargada de realizar una tirada de un dado de 6 caras, valores entre 1 y 6.*/
void ficherosPorLote(tCola *puntero,const tAlmacen _ficheros[],int numero,int _n); /*Funcion encargada de encolar los n primeros ficheros del almacen.*/
void mostrarCola(tCola puntero);                          /*Funcion encargada de mostrar por pantalla la cola con los fichero que hayan dentro.*/
int colaLlena(tCola puntero);                                         /*Funcion encargada de determinar si la cola esta llena.*/
int colaVacia(tCola puntero);                                         /*Funcion encargada de determinar si la cola esta vacia.*/
void encolar(tCola *puntero,const tNombre _ficheros,int _n);                /*Funcion encargada de aÃ±adir un fichero a la cola.*/
void desencolar(tCola *puntero,tNombre _desencolado,int _n);                /*Funcion encargada de sacar un fichero de la cola.*/


int main(int argc,char *argv[])
{
	tAlmacen ficheros[almacen_MAX];  /*Variable donde guardaremos los nombres de los ficheros en el almacen.*/
	tCola puntero;                   /*Variable con la que iremos recorriendo la estructura de la cola.*/
	tNombre desencolado;             /*Variable donde guardaremos los elementos desencolados.*/
	int valor;                       /*Variable donde guardaremos el valor de la tirada del dado.*/
	int i;                           /*Indice con el que recorreremos bucles.*/
	int correcto;                    /*Variable que usaremos para saber si el programa da algun tipo de error.*/
	char tecla;                      /*Teclaa sin utilidad que usaremos solo para continuar el programa.*/
	char eleccion;                   /*Variable que usaremos por si queremos repetir el proceso o salir del programa.*/
	int n;                           /*Variable que almacena el numero de fichero que hay en la cola.*/

	n=0;
	valor=0;
	printf("\nTeclee cualquier tecla para comenzar con el programa...");
	fflush(stdout);
	scanf("%s",&tecla);
	fflush(stdin);

	if(argc==7)
	{
		correcto=1;
		i=1;
		while(i<argc && correcto)
		{
			if(strlen(argv[i])>=(nombre_MAX-extension_MAX))
			{
				correcto=0;
				i++;
				printf("ERROR: NOMBRE DE FICHERO DEMASIADO LARGO. LA LONGITUD MAXIMA DE CADA FICHERO ES DE 8 CARACTERES.");
				fflush(stdout);
			}
			else
				i++;
		}

	if(correcto)
	{
		iniciarCola(&puntero);
		copiarFicherosAlmacen(argv,ficheros);
		extension(ficheros);
		printf("\nTRAZA: Los nombres de los ficheros son:\n");
		fflush(stdout);
		mostrarAlmacen(ficheros);
		printf("FIN DE TRAZA");
		fflush(stdout);

		printf("\nTRAZA: Se ha inicializado la cola. inicio=%d  final=%d",puntero.inicio,puntero.final);
		fflush(stdout);
		do
		{
		printf("\nTRAZA: Se procedera a simular la tirada del dado... ");
		fflush(stdout);
		valor=realizarTirada(valor);

		printf("\nEl numero de ficheros que se aÃ±adiran a la cola es: %d",valor);
		fflush(stdout);

		if(valor>=1 && valor <=6)
		{
			printf("\nTRAZA: Presiona una tecla para continuar...");
			fflush(stdout);
			scanf("%s",&tecla);
			fflush(stdin);

			ficherosPorLote(&puntero,ficheros,valor,n);
			printf("\nSe han podido encolar %d ficheros",valor);
			fflush(stdout);

			printf("\nTRAZA: Despues de encolar los indices de control de la cola son:\nTRAZA: final=%d   inicio=%d\n",puntero.final,puntero.inicio);
			fflush(stdout);

			printf("\nTRAZA: El contenido de la cola es:\n");
			fflush(stdout);
			mostrarCola(puntero);
			printf("FIN DE TRAZA\n");
			fflush(stdout);

			if(colaVacia(puntero))
			{
				printf("ERROR: COLA VACIA.");
				fflush(stdout);
			}
			 else
			 {
				printf("\nTRAZA: Se va a proceder a desencolar un fichero\nTRAZA: Teclee cualquier letra para desencolar un fichero...");
				fflush(stdout);
				scanf("%s",&tecla);
				fflush(stdin);

				desencolar(&puntero,desencolado,n);
				printf("\nEl fichero desencolado es: %s",desencolado);
				fflush(stdout);
				printf("\nTRAZA: Despues de desencolar los indices de control de la cola son:");
				fflush(stdout);
				printf("\nTRAZA: final:%d   inicio:%d\n",puntero.final,puntero.inicio);
				fflush(stdout);
				printf("\nTRAZA: El contenido de la cola es:\n");
				fflush(stdout);
				mostrarCola(puntero);
				printf("FIN DE TRAZA\n");
				fflush(stdout);
			 }
		}
		printf("\nDesea continuar con el programa? Si(s) No(cualquier otra tecla) ");
		fflush(stdout);
		scanf("%s",&eleccion);
		fflush(stdin);
		} while(eleccion=='s');
	}
	}
	else
		printf("ERROR: NO HAY 6 FICHEROS.");
		fflush(stdout);

		printf("\nPROGRAMA FINALIZADO.\n");
		fflush(stdout);
	return 0;
}

void iniciarCola(tCola *puntero)
{
	puntero->inicio=0;
	puntero->final=0;
}

void copiarFicherosAlmacen(char *argv[],tAlmacen _ficheros[])
{
	int i;
	for(i=0;i<almacen_MAX;i++)
	{
		strcpy(_ficheros->ficheros[i],argv[i+1]);
	}
}

void extension(tAlmacen _ficheros[])
{
	int i;
	for(i=0;i<almacen_MAX;i++)
	{
		strcat(_ficheros->ficheros[i],".txt");
	}
}

void mostrarAlmacen(tAlmacen _ficheros[])
{
	int i;
	for(i=0;i<almacen_MAX;i++)
	{
		printf("\tnombreFichero[%d] es %s\n",i,_ficheros->ficheros[i]);
		fflush(stdout);
	}
}

int realizarTirada(int _valor)
{
	int tirada;
	time_t t;
	srand((unsigned) time(&t));
	tirada=1+(rand()%6);
	_valor=tirada;
return _valor;
}

void ficherosPorLote(tCola *puntero,const tAlmacen _ficheros[],int numero,int _n)
{
	int i;
	for(i=0;i<numero;i++)
	{

			encolar(puntero,_ficheros->ficheros[i],_n);
	}
}

int colaLlena(tCola puntero)
{
	int llena;
	llena=1;
	if((puntero.final+1)%cola_MAX!=puntero.inicio)
	{
		llena=0;
	}
return llena;
}

int colaVacia(tCola puntero)
{
	int vacia;
	vacia=1;
	if(puntero.inicio!=puntero.final)
	{
		vacia=0;
	}
return vacia;
}

void encolar(tCola *puntero,const tNombre _ficheros,int _n)
{

	if(colaLlena(*puntero))
			{
				puntero->inicio=(((puntero->inicio)+1)%cola_MAX)-1;
				_n--;
			}
			else
				strcpy(puntero->nombre[puntero->final],_ficheros);
				puntero->final=((puntero->final)+_n)%cola_MAX;
				_n++;
}

void desencolar(tCola *puntero,tNombre _desencolado,int _n)
{
	strcpy(_desencolado,puntero->nombre[puntero->inicio]);
	puntero->inicio=(((puntero->inicio)+1)%cola_MAX)-1;
	_n--;
}

void mostrarCola(tCola puntero)
{
	int i;
	if(puntero.inicio<puntero.final)
	{
		for(i=(puntero.inicio);i<(puntero.final);i++)
		{
			printf("%s\n",puntero.nombre[i]);
			fflush(stdout);
		}
	}

	if(puntero.inicio>puntero.final)
		{
			for(i=(puntero.inicio);i<10;i++)
			{
				printf("%s\n",puntero.nombre[i]);
				fflush(stdout);
			}
			for(i=(puntero.final);i<(puntero.inicio);i++)
			{
				printf("%s\n",puntero.nombre[i]);
				fflush(stdout);
			}
		}
}


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

#define nombre_impresora_MAX (10+1)       /*Longitud maxima que podra tener cada nombre de cada impresora.*/
#define impresoras_MAX 4			      /*Tamaï¿½o del array de la lista de impresoras, sera 4 con un hueco libre.*/
#define cola_MAX (9+1)                    /*TamaÃ±o maximo de la cola. Es una cola circular con un hueco libre.*/
#define nombre_MAX (12+1)                 /*Cada fichero debe tener una longitud maxima de 12 caracteres mas el \0.*/
#define almacen_MAX 6                     /*El tamaÃ±o maximo del almacen donde se almacenaran ficheros es 6.*/
#define extension_MAX 4                   /*TamaÃ±o de la extension que se aÃ±adira a cada fichero es 4 ya que aÃ±adiremos ".txt".*/
#define TRUE 1                            /*Valor verdadero del valor booleano (en este caso el 1).*/
#define FALSE 0                           /*Valor falso del valor booleano (el 0).*/


typedef char tNombre[nombre_MAX];        /*Es el tipo de cada fichero, sera un char con una longitud de 12+1 (nombre_MAX).*/
typedef char tNombreImpresora[nombre_impresora_MAX]; /*Es el tipo de cada nombre de las impresoras, sera un char con una longitud de 12+1 (nombre_MAX).*/

typedef struct{                          /*Estructura que almacenara los ficheros que son proporcionados por *argv[].*/
	tNombre ficheros[almacen_MAX];       /*Variable que almacena todos los ficheros metidos por argv[].*/
} tAlmacen;

typedef struct{                          /*Estructura que representa la cola, en la cual aï¿½adiremos y eliminaremos ficheros.*/
	tNombre nombre[cola_MAX];            /*Variable que almacena el nombre de cada fichero en la cola.*/
	int inicio;                          /*Indice de control de la cola (inicio).*/
	int final;                           /*Indice de control de la cola (final).*/
} tCola;

typedef struct{                          /*Estructura que representa cada impresora, en la cual almacenara cada fichero.*/
	tNombreImpresora nombreI;            /*Variable que almacenara los nombres de cada impresora.*/
	int ficherosPendientes;              /*Variable que contara el numero de ficheros pendientes de cada impresora.*/
	tCola colaImpresora;                 /*Variable por la cual accederemos a la cola de cada impresora.*/
} tImpresora;

typedef struct{                         /*Estructura que servira para tener una lista de todas las impresoras para acceder a la que queramos.*/
	tImpresora impresora;               /*Variable que almacenara cada estructura de impresora.*/
	int ocupado;                        /*Variable que servira para saber si una posicion del array esta ocupada o no.*/
} tImprimir;

typedef tImprimir tListaImpresoras[impresoras_MAX];

void iniciarCola(tListaImpresoras lista);                                   /*Funcion encargada de inicializar la cola mediante los indices de control.*/
void copiarFicherosAlmacen(char *argv[],tAlmacen _ficheros[]);             /*Funcion encargada de copiar los ficheros desde *argv[] al almacen.*/
void extension(tAlmacen _ficheros[]);                             /*Funcion encargada de aï¿½adir la extension ".txt" a cada uno de los ficheros.*/
void mostrarAlmacen(tAlmacen _ficheros[]);               /*Funcion que imprime por pantalla los ficheros del almacen con la extension incluida.*/
int realizarTirada(int _valor);                          /*Funcion encargada de realizar una tirada de un dado de 6 caras, valores entre 1 y 6.*/
void ficherosPorLote(tListaImpresoras lista,const tAlmacen _ficheros[],int numero,int pos); /*Funcion encargada de encolar los n primeros ficheros del almacen.*/
void mostrarCola(tListaImpresoras lista,int pos);                          /*Funcion encargada de mostrar por pantalla la cola con los fichero que hayan dentro.*/
int colaLlena(tListaImpresoras lista,int pos);                                         /*Funcion encargada de determinar si la cola esta llena.*/
int colaVacia(tListaImpresoras lista,int pos);                                         /*Funcion encargada de determinar si la cola esta vacia.*/
void encolar(tListaImpresoras lista,const tNombre _ficheros,int pos);                /*Funcion encargada de aï¿½adir un fichero a la cola.*/
void desencolar(tListaImpresoras lista,tNombre _desencolado,int pos);                /*Funcion encargada de sacar un fichero de la cola.*/


void iniciarLista(tListaImpresoras lista);             /*Funcion encargada de inicializar la lista de impresoras.*/
int haySitio(const tListaImpresoras lista,int *hueco); /*Funcion encargada de buscar si hay algun sitio en el array de impresoras.*/
void agregarImpresora(tListaImpresoras lista,tNombreImpresora nombre,int hueco); /*Funcion encargada de agregar una impresora en el hueco correspondiente.*/
void listaImpresoras(tListaImpresoras lista);        /*Funcion encargada de mostrar por pantalla la lista de impresoras que hay hasta ese momento.*/
int listaVacia(tListaImpresoras lista);              /*Funcion encargada de determinar si la lista de impresoras esta vacia.*/
int listaLlena(tListaImpresoras lista);              /*Funcion encargada de determinar si la lista de impresoras esta llena.*/
int localizarImpresora(const tNombreImpresora nombre,const tListaImpresoras lista,int *pos); /*Funcion encargada de localizar una impresora que tenga el mismo nombre.*/
void ficherosPendientes(tListaImpresoras lista);     /*Funcion encargada de mostrar por pantalla todos los ficheros pendientes de cada impresora.*/
int borrarImpresora(tImpresora impresora,tNombreImpresora nombre,tListaImpresoras lista); /*Funcion encargada de eliminar la impresora que digamos.*/

int main(int argc,char *argv[])
{
	tAlmacen ficheros[almacen_MAX];  /*Variable donde guardaremos los nombres de los ficheros en el almacen.*/
	tNombre desencolado;             /*Variable donde guardaremos los elementos desencolados.*/
	int valor;                       /*Variable donde guardaremos el valor de la tirada del dado.*/
	int i;                           /*Indice con el que recorreremos bucles.*/
	int correcto;                    /*Variable que usaremos para saber si el programa da algun tipo de error.*/
	char tecla;                      /*Teclaa sin utilidad que usaremos solo para continuar el programa.*/
	int eleccion;                    /*Variable que usaremos por si queremos repetir el proceso o salir del programa.*/

	tListaImpresoras lista;          /*Variable que utilizaremos para acceder a cada impresora y sus correspondientes colas.*/
	tImpresora impresora;            /*Variable que contiene las impresoras.*/
	tNombreImpresora nombre;         /*Variable que es utilizado para el nombre de las impresoras.*/
	int posicion;                    /*Variable utilizada para determinar la posicion en la que se encuentra una impresora.*/
	int sitio;                       /*Variable utilizada para determinar si hay sitio, saber su posicion.*/

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
		iniciarLista(lista);
		iniciarCola(lista);
		copiarFicherosAlmacen(argv,ficheros);
		extension(ficheros);
		printf("\nTRAZA: Los nombres de los ficheros son:\n");
		fflush(stdout);
		mostrarAlmacen(ficheros);
		printf("FIN DE TRAZA");
		fflush(stdout);

		printf("\nTRAZA: La lista de impresoras ha sido inicializada\n");
		fflush(stdout);
		do
		{
			printf("\nOPCIONES DISPONIBLES: ");
			fflush(stdout);
			printf("\n\t1. AÃ±adir impresora.\n\t2. Mandar ficheros por lote a una impresora.");
			fflush(stdout);
			printf("\n\t3. Listar todos los ficheros pendientes de imprimir.");
			fflush(stdout);
			printf("\n\t4. Imprimir un fichero.\n\t5. Borrar una impresora.");
			fflush(stdout);
			printf("\n\t6. Salir de la aplicacion.");
			fflush(stdout);
			printf("\n\t\tIntroduzca una opcion: ");
			fflush(stdout);
			scanf("%d",&eleccion);
			fflush(stdin);

			switch(eleccion)
			{
				case 1:
							 if(haySitio(lista,&sitio))
							 {
							 printf("Introduzca el nombre de la impresora: ");
							 fflush(stdout);
							 scanf("%s",nombre);
							 fflush(stdin);
							 if(localizarImpresora(nombre,lista,&posicion))
							 {
								 printf("ERROR: LA IMPRESORA YA ESTA EN LA LISTA.");
								 fflush(stdout);
							 }
							 else
							 {
								 agregarImpresora(lista,nombre,sitio);
								 printf("Se ha agregado la impresora: %s\n",nombre);
								 fflush(stdout);
								 for(i=0;i<impresoras_MAX;i++)
								 {
									 if(lista[i].ocupado)
									 {
										 printf("**********");
										 fflush(stdout);
										 printf("\nImpresora que ocupa la posicion %d del array: %s",i,lista[i].impresora.nombreI);
										 fflush(stdout);
										 printf("\nFicheros pendientes en la cola: %d\n",lista[i].impresora.ficherosPendientes);
										 fflush(stdout);
										 mostrarCola(lista,i);
									 }
								 }
							 }
							 }
							 else
								 printf("ERROR: LISTA DE IMPRESORAS LLENA.");
							 	 fflush(stdout);
					break;

				 case 2:
					 	 if(listaVacia(lista))
					 	 {
					 		 printf("ERROR: NO HAY IMPRESORAS.");
					 		 fflush(stdout);
					 	 }
					 	  else
					 	  {
							 printf("La lista de impresoras disponibles es: ");
							 fflush(stdout);
							 listaImpresoras(lista);
							 printf("\nIntroduzca el nombre de la impresora: ");
							 fflush(stdout);
							 scanf("%s",nombre);
							 fflush(stdin);
							 if(localizarImpresora(nombre,lista,&posicion))
							 {
								 valor=realizarTirada(valor);
								 if(valor>=1 && valor <=6)
								 {
									ficherosPorLote(lista,ficheros,valor,posicion);
									printf("Se han podido encolar %d ficheros",valor);
									fflush(stdout);
									printf("\nTRAZA: Despues de encolar los indices de control de la impresora son:\n");
									fflush(stdout);
									printf("TRAZA: final:%d   inicio:%d\n",lista[posicion].impresora.colaImpresora.final,lista[posicion].impresora.colaImpresora.inicio);
									printf("TRAZA: El contenido de la impresora es:\n");
									fflush(stdout);
									mostrarCola(lista,posicion);
									printf("FIN DE TRAZA\n");
									fflush(stdout);
								 }
							 }
							  else
							 	 printf("ERROR: NO SE ENCUENTRA LA IMPRESORA.");
								 fflush(stdout);
					 	  }
					break;

				case 3:
					 	if(listaVacia(lista))
						{
							printf("ERROR: NO HAY IMPRESORAS.");
							fflush(stdout);
						}
						 else
							 ficherosPendientes(lista);
					break;

				case 4:
						if(listaVacia(lista))
						{
							printf("ERROR: NO HAY IMPRESORAS.");
							fflush(stdout);
						}
						  else
						  {
							  printf("La lista de impresoras disponibles es: ");
							  fflush(stdout);
							  listaImpresoras(lista);
							  printf("\nIntroduzca el nombre de la impresora: ");
							  fflush(stdout);
							  scanf("%s",nombre);
							  fflush(stdin);
							  if(localizarImpresora(nombre,lista,&posicion))
							  {
								  if(colaVacia(lista,posicion))
								  {
									  printf("ERROR: NO HAY ARCHIVOS PARA IMPRIMIR.");
									  fflush(stdout);
								  }
								   else
								   {
									  desencolar(lista,desencolado,posicion);
									   printf("TRAZA: Despues de desencolar los indices de control de la cola son:");
									   fflush(stdout);
									   printf("\nTRAZA: final:%d   inicio:%d\n",lista[posicion].impresora.colaImpresora.final,lista[posicion].impresora.colaImpresora.inicio);
									   fflush(stdout);
									   printf("TRAZA: El contenido de la cola es:\n");
									   fflush(stdout);
									   mostrarCola(lista,posicion);
									   printf("FIN DE TRAZA\n");
									   fflush(stdout);
									   printf("Archivo impreso: %s",desencolado);
									   fflush(stdout);
								   }
							  }
							   else
								   printf("ERROR: NO SE ENCUENTRA LA IMPRESORA.");
							  	   fflush(stdout);
						  }
					break;

				case 5:
						if(listaVacia(lista))
						{
							printf("ERROR: NO HAY IMPRESORAS.");
							fflush(stdout);
						}
						  else
						  {
							  printf("La lista de impresoras disponibles es: ");
							  fflush(stdout);
							  listaImpresoras(lista);
							  printf("\nIntroduzca el nombre de la impresora: ");
							  fflush(stdout);
							  scanf("%s",nombre);
							  fflush(stdin);
							  if(localizarImpresora(nombre,lista,&posicion))
							  {
								  borrarImpresora(impresora,nombre,lista);
								  printf("Se ha borrado la impresora: %s",nombre);
								  fflush(stdout);
							  }
							   else
								   printf("ERROR: NO SE ENCUENTRA LA IMPRESORA.");
							  	   fflush(stdout);
						  }
					break;

				case 6:
						printf("\nPROGRAMA FINALIZADO.\n");
						fflush(stdout);
						return 0;
					break;

				default:
						printf("ERROR: No es una eleccion correcta.\n");
						fflush(stdout);
					break;
				}
				} while(eleccion!=6);
			}
	}

	else
		printf("ERROR: NO HAY 6 FICHEROS.");
		fflush(stdout);

		printf("\nPROGRAMA FINALIZADO.\n");
		fflush(stdout);
	return 0;
}

void iniciarCola(tListaImpresoras lista)
{
	int i;
	for(i=0;i<impresoras_MAX;i++)
	{
		lista[i].impresora.colaImpresora.inicio=0;
		lista[i].impresora.colaImpresora.final=0;
	}
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

void ficherosPorLote(tListaImpresoras lista,const tAlmacen _ficheros[],int numero,int pos)
{
	int i;
	for(i=0;i<numero;i++)
	{
		encolar(lista,_ficheros->ficheros[i],pos);
	}
}

int colaLlena(tListaImpresoras lista,int pos)
{
	int llena;
	llena=TRUE;
	if(lista[pos].impresora.ficherosPendientes>=10)
	{
		llena=FALSE;
	}
return llena;
}

int colaVacia(tListaImpresoras lista,int pos)
{
	int vacia;
	vacia=TRUE;
	if(lista[pos].impresora.ficherosPendientes)
	{
		vacia=FALSE;
	}
return vacia;
}

void encolar(tListaImpresoras lista,const tNombre _ficheros,int pos)
{
	strcpy(lista[pos].impresora.colaImpresora.nombre[lista[pos].impresora.colaImpresora.final],_ficheros);
	lista[pos].impresora.colaImpresora.final=((lista[pos].impresora.colaImpresora.final)+1)%cola_MAX;
	lista[pos].impresora.ficherosPendientes=(lista[pos].impresora.ficherosPendientes)+1;
}

void desencolar(tListaImpresoras lista,tNombre _desencolado,int pos)
{
	strcpy(_desencolado,lista[pos].impresora.colaImpresora.nombre[lista[pos].impresora.colaImpresora.inicio]);
	lista[pos].impresora.colaImpresora.inicio=(((lista[pos].impresora.colaImpresora.inicio)+1)%cola_MAX);
	lista[pos].impresora.ficherosPendientes=(lista[pos].impresora.ficherosPendientes)-1;
}

void mostrarCola(tListaImpresoras lista,int pos)
{
	int i;
	if((lista[pos].impresora.colaImpresora.inicio)<(lista[pos].impresora.colaImpresora.final))
	{
		for(i=(lista[pos].impresora.colaImpresora.inicio);i<(lista[pos].impresora.colaImpresora.final);i++)
		{
			printf("%s\n",lista[pos].impresora.colaImpresora.nombre[i]);
			fflush(stdout);
		}
	}

	if((lista[pos].impresora.colaImpresora.inicio)>(lista[pos].impresora.colaImpresora.final))
		{
			for(i=(lista[pos].impresora.colaImpresora.inicio);i<(cola_MAX-1);i++)
			{
				printf("%s\n",lista[pos].impresora.colaImpresora.nombre[i]);
				fflush(stdout);
			}
			for(i=(lista[pos].impresora.colaImpresora.final);i<((lista[pos].impresora.colaImpresora.inicio)-1);i++)
			{
				printf("%s\n",lista[pos].impresora.colaImpresora.nombre[i]);
				fflush(stdout);
			}
		}
}

int haySitio(const tListaImpresoras lista,int *hueco)
{
	int i;
	int encontrado;
	encontrado=0;
	for(i=0;(i<impresoras_MAX) && !encontrado;i++)
	{
		if(lista[i].ocupado==0)
		{
			encontrado=1;
			*hueco=i;
		}
	}
	return encontrado;
}

void iniciarLista(tListaImpresoras lista)
{
	int i;
	for(i=0;i<impresoras_MAX;i++)
	{
		lista[i].ocupado=0;
		lista[i].impresora.ficherosPendientes=0;
	}
}

void agregarImpresora(tListaImpresoras lista,tNombreImpresora nombre,int hueco)
{
	strcpy(lista[hueco].impresora.nombreI,nombre);
	lista[hueco].ocupado=1;
	lista[hueco].impresora.ficherosPendientes=0;
}

void listaImpresoras(tListaImpresoras lista)
{
	int i;
	for(i=0;i<impresoras_MAX;i++)
	{
		if(lista[i].ocupado)
		{
		printf("%s  ",lista[i].impresora.nombreI);
		}
	}
}

int listaLlena(tListaImpresoras lista)
{
	int llena;
	llena=TRUE;
	int i;
	for(i=0;i<impresoras_MAX;i++)
		{
			if(!lista[i].ocupado)
			{
				llena=FALSE;
			}
		}
return llena;
}

int listaVacia(tListaImpresoras lista)
{
	int vacia;
	vacia=TRUE;
	int i;
	for(i=0;i<impresoras_MAX;i++)
	{
		if(lista[i].ocupado)
		{
			vacia=FALSE;
		}
	}
	return vacia;
}

int localizarImpresora(const tNombreImpresora nombre,const tListaImpresoras lista,int *pos)
{
	int i;
	int encontrado;
	encontrado=0;
	for(i=0;(i<impresoras_MAX)&& !encontrado;i++)
	{
		if(lista[i].ocupado)
		{
			if(!strcmp(nombre,lista[i].impresora.nombreI))
			{
				encontrado=1;
				*pos=i;
			}
		}
	}
	return encontrado;
}

void ficherosPendientes(tListaImpresoras lista)
{
	int i;
	int j;
	for(i=0;i<impresoras_MAX;i++)
	{
		if(lista[i].ocupado)
		{
		printf("*****Impresora %s*****\n",lista[i].impresora.nombreI);
		fflush(stdout);
		printf("Ficheros pendientes:\n");
		fflush(stdout);
		for((j=lista[i].impresora.colaImpresora.inicio);(j<lista[i].impresora.colaImpresora.final);j++)
		{
			printf("%s\n",lista[i].impresora.colaImpresora.nombre[j]);
			fflush(stdout);
		}
		}
	}
}

int borrarImpresora(tImpresora impresora,tNombreImpresora nombre,tListaImpresoras lista)
{
	int posicion;
	int encontrado;
	encontrado=localizarImpresora(nombre,lista,&posicion);
	if(encontrado)
	{
			lista[posicion].ocupado=0;
			lista[posicion].impresora.colaImpresora.inicio=0;
			lista[posicion].impresora.colaImpresora.final=0;
	}
	return encontrado;
}

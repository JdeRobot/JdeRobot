

#include "reemplazar.h"



/* Halla la longitud de una cadena, hace las veces de strlen */
int longitud(char *cadena)
{
   int i;
   for(i=0; ;i++)
     if(cadena[i]=='\0')
       break;
   return i;
}

/* Halla la posicion de aparicion de una subcadena en una cadena */
int posicion(char *cadena, char *subcadena)
{
   int i, j;

   for(i=0; i<longitud(cadena); i++)
   {
       if(cadena[i]==subcadena[0])
       {
     //if(longitud(cadena)-i < longitud(subcadena))
     //   return -1;

     for(j=0; j<longitud(subcadena); j++)
     {
        if(cadena[i+j]!=subcadena[j])
      break;
     }
     if(j==longitud(subcadena))
       return i;
       }
   }

   return -1;
}

/*
   Busca una subcadena en una cadena, y la reemplaza por otra;
   retorna el numero de veces que se hizo el reemplazo
*/
int reemplazar2(char *cadena, char *subcadena, char *reemplazo)
{
   int ocurrencias = 0;
   int i, indice, posiciones, temp, l_cadena, l_subcadena, l_reemplazo;
   int tam;
   char * cadena_aux;

   l_cadena = longitud(cadena);
   l_reemplazo = longitud(reemplazo);
   l_subcadena = longitud(subcadena);

   for(;;)
   {
     indice = posicion(cadena, subcadena);
     if(indice==-1)
   	break;
     else
     {
	printf("NO -1");
	tam = l_subcadena-l_reemplazo;
	if (tam < 0)
	{
		printf("0");
		for(i=indice; i<indice+l_subcadena; i++)
       			cadena_aux[i-indice] = cadena[i];
		cadena_aux[i] = '\0';
		printf("1");
		for(i=indice; i<indice+l_reemplazo; i++)
       			cadena[i]=reemplazo[i-indice];
		printf("2");
		for(i=indice+l_reemplazo; i<longitud(cadena_aux); i++)
       			cadena[i]=cadena_aux[i-indice-l_reemplazo];
	}else
   	{
	for(i=indice; i<indice+l_reemplazo; i++)
       		cadena[i]=reemplazo[i-indice];

   	/* Si la longitud del reemplazo es menor que la de la subcadena,
     	recorre caracteres siguientes, posiciones a la izquierda */
  	 posiciones=l_subcadena-l_reemplazo;
   	if(posiciones>0)
      		for(temp=i; temp<=l_cadena; temp++)
         		cadena[temp]=cadena[temp+posiciones];
	}
   	ocurrencias++;
     }
   }

   return ocurrencias;
}


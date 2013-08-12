
#ifdef __cplusplus
extern "C" {
#endif

#if !defined __REEMPLAZAR_H
#define __REEMPLAZAR_H


#include <stdio.h>



/* Halla la longitud de una cadena, hace las veces de strlen */
int longitud(char *cadena);

/* Halla la posicion de aparicion de una subcadena en una cadena */
int posicion(char *cadena, char *subcadena);

/*
   Busca una subcadena en una cadena, y la reemplaza por otra;
   retorna el numero de veces que se hizo el reemplazo
*/
int reemplazar(char *cadena, char *subcadena, char *reemplazo);



#endif

#ifdef __cplusplus
}
#endif

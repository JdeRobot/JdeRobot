/*
 *
 *  Copyright (C) 1997-2008 JDE Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *
 *  Authors : Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

/* Version 2.5
Novedades: abre los ficheros en modo binario, incluye <string.h>, no chequea que la linea con reglas acabe en "\n" simplemente coge el primer consecuente. De este modo este interprete de los ficheros con reglas y etiquetas no necesita que las lineas esten bien acabadas en "\n" y por eso puede digerir ficheros de texto generados en entornos MSDOS donde el cambio de linea se expresa como dos caracteres: "\r y \n".

OJO si se quiere depurar compilar definiendo la constante VERBOSE */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fuzzylib.h"	

#ifdef VERBOSE
#define DEBUG 1
#else 
#define DEBUG 0
#endif

typedef struct NodoVariable{	
  char nombre[MAX_NOMBRE];  /* nombre variable fisica: velocidad, posicion...*/ 
  int numero;
  float *valor; /* Si ==NULL entonces es la variable es interna del controlador borroso, y esta almacenada en el campo valor_interno */
  float valor_interno;
  long int ultima_actualizacion;
} VariableFuzzyTipo;

typedef struct NodoEtiqueta{
  char nombre[MAX_NOMBRE];
  int numero;
  int numero_variable;
  float parametros[PARAM_PER_ETIQ];
} EtiquetaFuzzyTipo;


typedef struct NodoController{
  char nombre[MAX_NOMBRE];
  int numero;
  EtiquetaFuzzyTipo *etiquetas;
  int numetiquetas;
  VariableFuzzyTipo *variables;
  int numvariables;
  int *reglas;
  int numreglas;
  int numelementos; /* cada antecedente y consecuente es un elemento de una regla */
  int maximo_variables; /* las que tienen memoria reservada */
  int maximo_etiquetas; /* las que tienen memoria reservada */
  int maximo_elementos; /* los que tienen memoria reservada */
  /* faltan los parametros internos de funcionamiento: AND=max, OR=producto, inferencia centro de masas... */

} ControladorFuzzyTipo;

ControladorFuzzyTipo *raiz=NULL;
int numcontroladores=0; 
int controladores_utilizados=0;
int maximo_controladores=0; /* los que tienen memoria reservada */

long int clock=1; /* OJO tiene que empezar en 1 y no en 0. Para que el mecanismo de evitar bucles de actualizacion de variables internas funcione incluso con la primera llamada. El campo "ultima_actualizacion" de las variables se inicializa a 0 */

FILE *fuzzyfile;
int numlinea=0;

#define ANTECEDENTES -1
#define FINREGLA -2
/* dentro de la memoria que expresa las reglas, -1 separa los consecuentes de los antecedentes. -2 indica el fin de la regla */

EtiquetaFuzzyTipo *fc_etiqueta_new(int controlador)
{
  if (raiz[controlador].numetiquetas >= raiz[controlador].maximo_etiquetas) 
   {raiz[controlador].etiquetas=realloc(raiz[controlador].etiquetas,(raiz[controlador].maximo_etiquetas+20)*sizeof(EtiquetaFuzzyTipo));
   raiz[controlador].maximo_etiquetas+=20;
   }

 if (raiz[controlador].etiquetas==NULL) {printf(" Error pidiendo memoria dinamica para etiquetas\n"); exit(0);}
 else {raiz[controlador].numetiquetas++;
 
 ((raiz[controlador].etiquetas)[raiz[controlador].numetiquetas-1]).nombre[0]='\0';
 ((raiz[controlador].etiquetas)[raiz[controlador].numetiquetas-1]).numero=raiz[controlador].numetiquetas-1;
 return &((raiz[controlador].etiquetas)[raiz[controlador].numetiquetas-1]);
 }}


VariableFuzzyTipo *fc_variable_new(int controlador)
{
  if (raiz[controlador].numvariables >= raiz[controlador].maximo_variables) 
    { raiz[controlador].variables=realloc(raiz[controlador].variables,(raiz[controlador].maximo_variables+20)*sizeof(VariableFuzzyTipo));
    raiz[controlador].maximo_variables+=20;
    }

 if (raiz[controlador].variables==NULL) {printf(" Error pidiendo memoria dinamica para variables\n"); exit(0);}
 else {raiz[controlador].numvariables++;
 (raiz[controlador].variables)[raiz[controlador].numvariables-1].nombre[0]='\0';
 (raiz[controlador].variables)[raiz[controlador].numvariables-1].numero=raiz[controlador].numvariables-1;
 (raiz[controlador].variables)[raiz[controlador].numvariables-1].valor=NULL;
 (raiz[controlador].variables)[raiz[controlador].numvariables-1].valor_interno=-1.; 
 (raiz[controlador].variables)[raiz[controlador].numvariables-1].ultima_actualizacion=0;
 return &((raiz[controlador].variables)[raiz[controlador].numvariables-1]);
 }}


int fc_controlador_new()
{
int i,hueco=-1;

/* antes de pedir mas memoria mira a ver si hay algun hueco desocupado en el array, de algun controlador que se cerro. Si no hay huecos hasta "controladores_utilizados", explora si lo hay de "controladores_utilizados" a "maximo_controladores". Si tampoco lo hay hace un realloc. */

  for(i=0;i<controladores_utilizados;i++) 
    if (raiz[i].numero==-1) {hueco=i;break;}

  if ((hueco==-1)&&(controladores_utilizados >= maximo_controladores))
    {raiz=realloc(raiz,(controladores_utilizados+10)*sizeof(ControladorFuzzyTipo));
    maximo_controladores+=10;
    if (raiz==NULL) {printf(" Error pidiendo memoria dinamica para nuevo controlador \n"); exit(0);}
    if (DEBUG) printf("Ha habido realloc (%ld) ",(long int)raiz); 
    hueco=controladores_utilizados++;
  }
  else if (hueco>-1) { if (DEBUG) printf("Aprovecha hueco existente ");}
  else { hueco=controladores_utilizados++; if (DEBUG) printf("Bastaba con la memoria existente ");}

  numcontroladores++;
  raiz[hueco].nombre[0]='\0';
  raiz[hueco].numero=hueco;
  raiz[hueco].etiquetas=NULL;
  raiz[hueco].numetiquetas=0;
  raiz[hueco].variables=NULL;
  raiz[hueco].numvariables=0;
  raiz[hueco].reglas=0;
  raiz[hueco].numreglas=0;
  raiz[hueco].numelementos=0;
  raiz[hueco].maximo_variables=0;
  raiz[hueco].maximo_etiquetas=0;
  raiz[hueco].maximo_elementos=0; 

  if (DEBUG) printf(" Controlador numero %d (%d reales, %d ocupados de %d reservados)\n",hueco,numcontroladores,controladores_utilizados,maximo_controladores);
  return hueco;
}

void fc_close(int controlador)
     /* se libera la memoria de los controladores que "se borran", sus etiquetas, variables y reglas, pero no la de su entrada en el array, para no complicar el codigo. No se espera mucho dinamismo en el numero de controladores */
{
 if ((controlador<0)||(controlador>controladores_utilizados)||(raiz[controlador].numero==-1)) return;

 if (DEBUG) printf(" Cerramos controlador %d \n",raiz[controlador].numero);
free(raiz[controlador].etiquetas);
free(raiz[controlador].variables);
free(raiz[controlador].reglas); 
raiz[controlador].etiquetas=NULL;
raiz[controlador].variables=NULL;
raiz[controlador].reglas=NULL;
raiz[controlador].numero=-1;
numcontroladores--;

}

int variable2numero(int controlador, char *variable)
{
int i;

for(i=0; i<raiz[controlador].numvariables; i++)
if (strcmp(((raiz[controlador].variables)[i]).nombre,variable)==0) return ((raiz[controlador].variables)[i]).numero;

return -1;
}

int etiqueta2numero(int controlador, char *variable, char *etiqueta)
     /* OJO para que el mismo nombre de etiqueta pueda ser usado en distintas variables (con sus definiciones distintas) el nombre completo de la etiqueta queda univocamente definido por el par "variable","etiqueta") */
{int i;

for(i=0; i<raiz[controlador].numetiquetas; i++)
if ((strcmp(((raiz[controlador].etiquetas)[i]).nombre,etiqueta)==0)&&
    (strcmp((raiz[controlador].variables)[(raiz[controlador].etiquetas)[i].numero_variable].nombre,variable)==0)  )
return ((raiz[controlador].etiquetas)[i]).numero;

return -1;
}

int fc_inserta_elemento(int controlador, int numero)
{
  if (raiz[controlador].numelementos >= raiz[controlador].maximo_elementos) 
   {raiz[controlador].reglas=realloc(raiz[controlador].reglas,(raiz[controlador].maximo_elementos+200)*sizeof(int));
   raiz[controlador].maximo_elementos+=200;
   }

 if (raiz[controlador].reglas==NULL) {printf(" Error pidiendo memoria dinamica para etiquetas\n"); exit(0);}
 else {raiz[controlador].numelementos++;
  (raiz[controlador].reglas)[raiz[controlador].numelementos-1]=numero;
  if (DEBUG) printf(" %d ",numero); 
  return numero;
 }
}


#define ESPACIO ' '
#define COMENTARIO '#'
#define SREGLA "IF"
#define SETIQUETA "etiqueta"
#define SCONTROLADOR "controlador"

int readline(int controlador, FILE *myfile)
{ 
static char buffer[MAX_LINE_LENGTH];
static char codigo[MAX_NOMBRE];
static char variable[MAX_NOMBRE];
static char etiqueta[MAX_NOMBRE];

int i=0,j=0, fallo=0; /* indica fallo digiriendo una linea del fichero */
EtiquetaFuzzyTipo *e;
VariableFuzzyTipo *v;
int old_numelementos;
int numet;

/* Detecta EOF al comienzo de linea, y salta las lineas de comentario, tambien los blancos al comienzo de la linea y las lineas en blanco. Detecta si es una linea con etiqueta o con regla y la inserta adecuadamente en el controlador que se esta construyendo. Si la linea no se digiere bien, porque no esta en el formato adecuado lo indica por consola y la ignora.
 */
do {
buffer[0]=fgetc(myfile);
numlinea++;
if (buffer[0]==EOF) return EOF; 
if (buffer[0]==ESPACIO) {do buffer[0]=fgetc(myfile); while (buffer[0]==ESPACIO);}
if (buffer[0]=='\n') { if (DEBUG) printf("linea (%d) en blanco\n",numlinea);} 
if (buffer[0]==COMENTARIO) {do buffer[0]=fgetc(myfile); while ((buffer[0]!='\n')&&(buffer[0]!=EOF)); if (DEBUG) printf("linea (%d) comentario\n",numlinea);}
}while((buffer[0]==COMENTARIO)||(buffer[0]==EOF)||(buffer[0]=='\n'));

/* Captura la linea entera y luego la procesa con sscanf hasta interpretarla por completo. No lo podemos hacer directamente con fscanf porque esta funcion no distingue espacio en blanco de \n */
while((buffer[i]!='\n')&&(buffer[i]!=EOF)) buffer[++i]=fgetc(myfile);
buffer[i]='\0';

codigo[0]='\0';
sscanf(buffer,"%s",codigo);

if (strcmp(codigo,SETIQUETA)==0)
  {while((buffer[j]!='\n')&&(buffer[j]!=' ')&&(buffer[j]!='\0')) j++;
  j++; 
  /* avanza el "puntero" j, para que supere la primera palabra, ya leida, y el espacio en blanco siguiente */

  e=fc_etiqueta_new(controlador);
  fallo =0;
    
  if (sscanf(&buffer[j],"%s %s = %f %f %f %f",variable,e->nombre,&(e->parametros[0]),&(e->parametros[1]),&(e->parametros[2]),&(e->parametros[3]))!=6) fallo=1;
  else 
    { if ((e->numero_variable=variable2numero(controlador,variable))==-1) /* variable no esta en la lista, hay que crearla */
      {v=fc_variable_new(controlador);
      strcpy(v->nombre,variable);
      e->numero_variable=v->numero;
      }
    else /* la variable de esta etiqueta ya existia */;

    if (etiqueta2numero(controlador,variable,e->nombre)!=raiz[controlador].numetiquetas-1)
      {printf("(linea %d) Etiqueta %s %s repetida, me quedo con la primera definicion, ignoro la segunda\n",numlinea, variable, e->nombre); 
      fallo=1;}
    if ((e->parametros[0]<=e->parametros[1])&&(e->parametros[1]<=e->parametros[2])&&(e->parametros[2]<=e->parametros[3])) ; /* parametros en orden */
    else 
      {printf("(linea %d) Etiqueta %s %s con parametros inadecuados, deben estar en orden. La ignoro.\n",numlinea, variable, e->nombre); 
      fallo=1;}
    
}

  if (fallo) 
    { printf(" Fallo de conversion inesperado en linea (%d)\n",numlinea); 
    raiz[controlador].numetiquetas--;
    return 0; }
  else {
    if (DEBUG) printf(" Linea (%d) con etiqueta borrosa (%s:%s) = [%.2f,%.2f,%.2f,%.2f]\n",numlinea,(raiz[controlador].variables)[e->numero_variable].nombre,e->nombre,e->parametros[0],e->parametros[1],e->parametros[2],e->parametros[3]);
    return 1;
  }
}

else if (strcmp(codigo,SREGLA)==0)
  /* Si la regla no tiene el formato adecuado o sus variables/etiquetas no han sido definidas aun, la regla se ignora, NO se incorpora al controlador */

  {while((buffer[j]!='\n')&&(buffer[j]!=' ')&&(buffer[j]!='\0')) j++; j++; 
  /* avanza el "puntero" j, para que supere la primera palabra, ya leida, y el espacio en blanco siguiente */

  old_numelementos=raiz[controlador].numelementos;
  fallo = 0;
 
 
  if (sscanf(&buffer[j],"( %s = %s )",variable,etiqueta)!=2) fallo=1;
  else { /* continua leyendo el resto del antecedente */
    if ((numet=etiqueta2numero(controlador,variable,etiqueta))==-1) fallo=1;
    fc_inserta_elemento(controlador,numet);
    for(i=0;i<5;i++) {while (buffer[j]==ESPACIO) j++;while((buffer[j]!='\n')&&(buffer[j]!=' ')&&(buffer[j]!='\0')) j++; j++; }
    while (sscanf(&buffer[j],"AND ( %s = %s )",variable,etiqueta)==2) 
      { if ((numet=etiqueta2numero(controlador,variable,etiqueta))==-1) fallo=1;
	fc_inserta_elemento(controlador,numet);
	for(i=0;i<6;i++) {while (buffer[j]==ESPACIO) j++;while((buffer[j]!='\n')&&(buffer[j]!=' ')&&(buffer[j]!='\0')) j++; j++;} 
      }

    if (sscanf(&buffer[j],"THEN ( %s = %s )",variable,etiqueta)!=2) fallo=1;
    else { /* continua leyendo el resto del consecuente */
      fc_inserta_elemento(controlador,ANTECEDENTES);
      if ((numet=etiqueta2numero(controlador,variable,etiqueta))==-1) fallo=1;
      fc_inserta_elemento(controlador,numet);
      for(i=0;i<6;i++) {while (buffer[j]==ESPACIO) j++;while((buffer[j]!='\n')&&(buffer[j]!=' ')&&(buffer[j]!='\0')) j++; j++; }
    while (sscanf(&buffer[j],"AND ( %s = %s )",variable,etiqueta)==2) 
      { if ((numet=etiqueta2numero(controlador,variable,etiqueta))==-1) fallo=1;
	fc_inserta_elemento(controlador,numet);
	for(i=0;i<6;i++) {while (buffer[j]==ESPACIO) j++;while((buffer[j]!='\n')&&(buffer[j]!=' ')&&(buffer[j]!='\0')) j++; j++; }
      }
    fc_inserta_elemento(controlador,FINREGLA);
    /*if (sscanf(&buffer[j]," \n")==EOF) fallo=1; 
     Al comentar esta parte no analiza el resto de la linea del fichero y asi no le molesta que acabe en \n o en los caracteres de fin de linea para MSDOS */

    }
  }
  
  if (fallo) 
  { printf(" Fallo de conversion inesperado en linea (%d)\n",numlinea); 
    raiz[controlador].numelementos=old_numelementos;
    return 0; }
  else {
    if (DEBUG) printf(" Linea (%d) con regla borrosa\n",numlinea);
    raiz[controlador].numreglas++;
    return 1;
  }

  }
else if (strcmp(codigo,SCONTROLADOR)==0) 
  {while((buffer[j]!='\n')&&(buffer[j]!=' ')&&(buffer[j]!='\0')) j++;j++; 
  /* avanza el "puntero" j, para que supere la primera palabra, ya leida, y el espacio en blanco siguiente */
   if (sscanf(&buffer[j],"%s",raiz[controlador].nombre)!=1) 
    { printf(" Fallo de conversion inesperado en linea (%d)\n",numlinea); 
    return 0; 
    } 
   else 
    { if (DEBUG) printf(" Linea (%d) con nombre del controlador: %s\n",numlinea,raiz[controlador].nombre);
    return 1;
    }

}

else { if (DEBUG) printf(" Linea (%d) no reconocida\n",numlinea);  return 0;} 

}

int fc_open(char *filename)
{
int j;

  if((fuzzyfile=fopen(filename,"rb"))==NULL){
    printf("Can not open file: %s\n",filename);
    exit(0);
  }
  else {
    if (DEBUG) printf("Fuzzy file opened (%s)\n",filename);  

    /* Crea el nodo controlador y lo inserta en la lista de nodos */
    j=fc_controlador_new();

    /* lee las lineas y aquellas que entiende las inserta correctamente en las estructuras del nodo controlador recien creado, filtra las lineas de comentario */
    numlinea=0;
    fc_inserta_elemento(j,FINREGLA); 
 /* Este primer comienzo es necesario para luego tratar por igual a todas las reglas cuando se buscan los antecedentes, incluida la primera */ 
   do {} while(readline(j,fuzzyfile)!=EOF);

    fclose(fuzzyfile);

    if ((raiz[j].variables==NULL)||(raiz[j].etiquetas==NULL)) 
      {fc_close(j); /* analogo a "borrar" incluso la memoria asociada en el array de controladores */
      printf(" Fichero sin informacion sobre variables borrosas\n"); 
      return -1;
      }
    else if (raiz[j].numreglas==0) 
      {fc_close(j);
      printf(" Fichero sin reglas borrosas\n"); 
      return -1;
      }
    else
      {
	printf(" Fuzzy file (%s) analized and closed:\n Controlador %s,(numero %d), %d variables con %d etiquetas y %d reglas\n\n",filename
	       ,raiz[j].nombre, j, raiz[j].numvariables, raiz[j].numetiquetas, raiz[j].numreglas);
	return j;
  }
  }}


void fc_save(int controlador, char *filename)
{
  int i,j;
  int primero=0; /* para señalar al primer antecendete */
  
  if ((controlador<0)||(controlador>controladores_utilizados)||(raiz[controlador].numero==-1)) return;

  if((fuzzyfile=fopen(filename,"wb"))==NULL){
    printf("Can not open file: %s\n",filename);
    exit(0);
  }
  else {
    if (DEBUG) printf("Fuzzy file opened (%s) to write in \n",filename); 
    
    fprintf(fuzzyfile,"#Fichero generado automaticamente\n# %d variables con %d etiquetas y %d reglas\n\n",
	    raiz[controlador].numvariables, 
	    raiz[controlador].numetiquetas, 
	    raiz[controlador].numreglas);
    fprintf(fuzzyfile,"controlador %s\n", raiz[controlador].nombre);
	    
    
    /* imprimimos las etiquetas borrosas definidas */
    fprintf(fuzzyfile,"\n# Etiquetas y variables borrosas \n\n");
    for(i=0; i<raiz[controlador].numetiquetas; i++)
      fprintf(fuzzyfile,"etiqueta %s %s  =  %.2f %.2f %.2f %.2f\n",      
	      (raiz[controlador].variables)[(raiz[controlador].etiquetas)[i].numero_variable].nombre,
	      (raiz[controlador].etiquetas)[i].nombre,
	      ((raiz[controlador].etiquetas)[i]).parametros[0],
	      ((raiz[controlador].etiquetas)[i]).parametros[1],
	      ((raiz[controlador].etiquetas)[i]).parametros[2],
	      ((raiz[controlador].etiquetas)[i]).parametros[3]);
  }

  /* imprimimos ahora las reglas borrosas */
  fprintf(fuzzyfile,"\n# Reglas borrosas \n\n ");
  j=1; /* "puntero" para leer la memoria de las reglas, el primer entero es "FINREGLA" para unificar la busqueda de antecedentes y que sea igual para todas las reglas, incluida la primera */
  for(i=0; i<raiz[controlador].numreglas; i++)
    { 
      fprintf(fuzzyfile," IF");
      primero=1;
      while((raiz[controlador].reglas)[j++]!=ANTECEDENTES) 
	{ if (!primero) fprintf(fuzzyfile," AND");
	else primero=0;
	
	fprintf(fuzzyfile," ( %s = %s )",
		(raiz[controlador].variables)[(raiz[controlador].etiquetas)[(raiz[controlador].reglas)[j-1]].numero_variable].nombre,
		(raiz[controlador].etiquetas)[(raiz[controlador].reglas)[j-1]].nombre);
	}

      fprintf(fuzzyfile,"  THEN  ");
      primero=1;
    while((raiz[controlador].reglas)[j++]!=FINREGLA) 
      { if (!primero) fprintf(fuzzyfile," AND");
      else primero=0;

      fprintf(fuzzyfile," ( %s = %s )",
	      (raiz[controlador].variables)[(raiz[controlador].etiquetas)[(raiz[controlador].reglas)[j-1]].numero_variable].nombre,
	      (raiz[controlador].etiquetas)[(raiz[controlador].reglas)[j-1]].nombre);
      }
    }

}

float verifica(int controlador, int etiqueta, float crisp)
     /* Devuelve el grado [0..1] en el que el valor crisp pertenece al conjunto borroso apuntado por etiqueta. El conjunto borroso se define como un trapecio con 4 parametros, que degenera en un triangulo cuando p1=p2. Si p0 es igual a p1, o p2 a p3 eso se consideran subidas verticales, y asi el conjunto borroso puede degenerar a rectangulos si se desea */
{
EtiquetaFuzzyTipo *e;

e=&(raiz[controlador].etiquetas)[etiqueta];

if (DEBUG) printf(" Parametros %.2f %.2f %.2f %.2f, crisp= %.2f \n",e->parametros[0],e->parametros[1],e->parametros[2],e->parametros[3],crisp);


if ((crisp > e->parametros[0]) && (crisp < e->parametros[1]))
  return ((crisp - e->parametros[0])/(e->parametros[1] - e->parametros[0]));
else if ((crisp > e->parametros[2]) && (crisp < e->parametros[3]))
  return ((e->parametros[3] - crisp)/(e->parametros[3] - e->parametros[2]));
else if ((crisp >= e->parametros[1]) && (crisp <= e->parametros[2]))
  return 1;
else return 0;
}

float fc_AND(float acumulado, float antecedente)
     /* AND de antecedentes con producto. Esta funcion debe ser conmutativa, que no importe el orden en que son incorporan los distintos antecedentes. Por ello permite una evaluacion: acumulado:nuevo. Por ejemplo el minimo y el producto cumplen esto. */
{
if (DEBUG) printf(" acumulado %.2f, antecedente %.2f \n",acumulado,antecedente); 
return (acumulado*antecedente);
}

int fc_inferencia(int controlador, int etiqueta, float antecedente, float *output)
     /* Composicion de varias reglas por centro de masas, la aplicacion de una regla se traduce en una salida y una masa. La salida de un conjunto de reglas con el mismo consecuente es el centro de masas de las salidas de cada de ellas. Antecedente es la altura a la que se trunca el trapecio */

{
float p0,p1,p2,p3;
float masatrozo1, cdmtrozo1, masatrozo2, cdmtrozo2, masatrozo3, cdmtrozo3;

p0=(raiz[controlador].etiquetas)[etiqueta].parametros[0];
p1=(raiz[controlador].etiquetas)[etiqueta].parametros[1];
p2=(raiz[controlador].etiquetas)[etiqueta].parametros[2];
p3=(raiz[controlador].etiquetas)[etiqueta].parametros[3];

if (antecedente==0.) 
  {/* el cero no influye en las medias, de modo que esta regla no influye si no es aplicable y hay otras que si lo son */
   *output=0.0;} 

else if ((antecedente!=0.)&&((p0!=p1)||(p1!=p2)||(p2!=p3)))
  {
/* Para el calculo del centro de masas y la masa del trapecio de la etiqueta truncado a la altura "antecedente" descomponemos en tres trozos: la subida, la meseta y la bajada. Esto vale para etiquetas triangulares (masa de meseta=0), cuadradas (masa subida y bajada =0) y trapezoidales (masa de subida o de bajada nula) */

    masatrozo1=(p1-p0)*antecedente*antecedente/2;
    cdmtrozo1= p0+ 2*(p1-p0)*antecedente/3;
    masatrozo2= antecedente*(p3-p0-antecedente*(p3-p2)-antecedente*(p1-p0));
    cdmtrozo2=(p3-p0-antecedente*(p3-p2)-antecedente*(p1-p0))/2 + antecedente*(p1-p0) + p0;
    masatrozo3=(p3-p2)*antecedente*antecedente/2;
    cdmtrozo3=p3-2*antecedente*(p3-p2)/3;
    
    if (DEBUG) printf(" etiqueta %d: %.2f, %.2f, %.2f, %.2f\n",etiqueta,p0,p1,p2,p3);
    if (DEBUG) printf(" m1=%f, x1=%f,  m2=%f, x2=%f,  m3=%f, x3=%f \n",masatrozo1, cdmtrozo1, masatrozo2, cdmtrozo2, masatrozo3, cdmtrozo3);

    *output = (masatrozo1*cdmtrozo1+masatrozo2*cdmtrozo2+masatrozo3*cdmtrozo3)/(masatrozo1 + masatrozo2 + masatrozo3); 
  }

else if ((antecedente!=0.)&&(p0==p1)&&(p1==p2)&&(p2==p3))
  {
    /* singleton con antecedente no nulo. */
    *output = p0;
  if (DEBUG) printf(" singleton en %.2f\n",p0);}


return 1;
}

int fc_output(int controlador, char *varname, float *output)
     /* Mezcla de reglas por centro de masas.
(a) Si se llama con una variable de salida enlazada, se calcula la salida adecuada aplicando las reglas, se entrega en *output y la variable enlazada se modifica. Se devuelve el numero de reglasaplicadas.
(b) Si se llama con una variable interna se calcula la salida aplicando las reglas y se entrega en *output. Se devuelve el numero de reglasaplicadas o -1 si no hay ninguna.
(c) Si se llama con una variable de entrada enlazada no hay reglas aplicables ni aplicadas. No se llama a ninguna regla ni se modifica ningun valor. Se devuelve -1; 
*/

{
 int i=0, reglasleidas=0, consecuentes=0;
 int j=1; /* "puntero" para recorrer la memoria de reglas */
 int reglasaplicables=0, reglasaplicadas=0;

 float antecedente=1;
 float outputregla=0;
 float sumaparcial=0, masatotal=0;
 int variable_output=0, variable_antecedente;

 if ((controlador<0)||(controlador>controladores_utilizados)||(raiz[controlador].numero==-1)) return -1;

 variable_output=variable2numero(controlador,varname);
 if (variable_output==-1) return -1;
 else if (DEBUG) printf(" Actualizacion de %s \n",(raiz[controlador].variables)[variable_output].nombre);


  while(reglasleidas<raiz[controlador].numreglas)
  {
    if (raiz[controlador].reglas[j]==ANTECEDENTES) consecuentes=1;
    else if (raiz[controlador].reglas[j]==FINREGLA) {consecuentes=0;reglasleidas++;}
    else if (consecuentes==1)
      {
	if (raiz[controlador].etiquetas[(raiz[controlador].reglas[j])].numero_variable!=variable_output) ;
	/* ignora un consecuente que no es el que interesa ahora */
	else {
	  reglasaplicables++;
	  i=j; antecedente=1;
	  while(raiz[controlador].reglas[i]!=ANTECEDENTES) i--;
	  i--;
	  while(raiz[controlador].reglas[i]!=FINREGLA) 
	    {
	      variable_antecedente = (raiz[controlador].etiquetas)[raiz[controlador].reglas[i]].numero_variable;
	     
	      if ((raiz[controlador].variables)[variable_antecedente].valor==NULL) 
		{
		  /* Si la variable antecedente es interna se lanza una actualizacion si no esta ya actualizada. Esto se averigua mirando si la ultima_actualizacion es igual al "clock". Si lo es es que ya en esta llamada a fc_output se actualizo, y entonces se coge el valor ya actualizado, no se vuelve a llamar a la rutina de actualizacion */
		  if ((raiz[controlador].variables)[variable_antecedente].ultima_actualizacion!=clock)
		  fc_output(controlador,(raiz[controlador].variables)[variable_antecedente].nombre,
			    &((raiz[controlador].variables)[variable_antecedente].valor_interno));
		  
		antecedente=fc_AND(antecedente, 
		       verifica(controlador,raiz[controlador].reglas[i],
				(raiz[controlador].variables[variable_antecedente]).valor_interno));
		}
	      else {
		/* Si la variable antecedente NO es interna, que esta enlazada a una variable en la memoria del codigo usuario entonces se supone que ya esta actualizada */
		antecedente=fc_AND(antecedente,
		    verifica(controlador,raiz[controlador].reglas[i],
			      (*((raiz[controlador].variables)[variable_antecedente].valor))));
		if (DEBUG) printf(" Verifico %s = %f \n",(raiz[controlador].variables)[variable_antecedente].nombre,*((raiz[controlador].variables)[variable_antecedente].valor));
		    }
	      i--;
	    }
	  if (antecedente!=0) 
	    {reglasaplicadas++;
	    fc_inferencia(controlador,raiz[controlador].reglas[j],antecedente,&outputregla);
	    /* En cualquier caso la masa de la regla, que pondera la influencia de esta regla en la salida final del controlador borroso es justo el grado en el que se aplica su antecedente */
	    /* solo cuando el antecedente es no nulo se averigua lo que esa regla recomienda, si es nulo no se pierde el tiempo en "inferir" esa regla */ 
	    masatotal+=antecedente;
	    sumaparcial+=antecedente*outputregla;
	    if (DEBUG) printf(" Masa regla %.2f, salida regla %.2f\n",antecedente,outputregla);}
	  else if (DEBUG) printf(" Masa regla nula\n");
	}
      }
    j++;
  }

 /*printf(" Aplicadas %d reglas de las %d aplicables para dar como salida %f \n",reglasaplicadas,reglasaplicables,sumaparcial/masatotal);*/

  if (reglasaplicables==0) 
    { if (DEBUG) printf(" No hay reglas aplicables para actualizar %s \n",(raiz[controlador].variables)[variable_output].nombre);
    return -1;
    }
  else if ((masatotal==0)||(reglasaplicadas==0)) 
    { if (DEBUG) printf(" Las reglas aplicables para %s no son aplicables al no cumplir sus antecedentes\n",(raiz[controlador].variables)[variable_output].nombre);
    return -1;
    }
  else if ((raiz[controlador].variables)[variable_output].valor==NULL) 
    {
      raiz[controlador].variables[variable_output].valor_interno=sumaparcial/masatotal;
      raiz[controlador].variables[variable_output].ultima_actualizacion=clock;
      *output=raiz[controlador].variables[variable_output].valor_interno;
      /* Se llamo fc_output para consultar/actualizar una variable interna. Refresca el campo ultima actualizacion para evita que en la misma llamada a fc_output se actualice mas de una vez la misma variable interna. De este modo se salvan bucles. */
      return reglasaplicadas;
    }
  else /* se llamo fc_output para actualizar/consultar una variable enlazada.*/
    {
    *raiz[controlador].variables[variable_output].valor=sumaparcial/masatotal;
    raiz[controlador].variables[variable_output].ultima_actualizacion=clock++;
    *output=sumaparcial/masatotal;
    return reglasaplicadas;
    }
}

int fc_link(int controlador, char *varname, float *varpointer)
     /* Si varpointer==NULL entonces la variable se desliga y se hace interna al controlador borroso, que es como "nace" por defecto */
{
int v;

 if ((controlador<0)||(controlador>controladores_utilizados)||(raiz[controlador].numero==-1)) return -1;

 v=variable2numero(controlador,varname);
 if (v==-1) return -1;
 else {
 (raiz[controlador].variables)[v].valor=varpointer;
 (raiz[controlador].variables)[v].valor_interno=-1;
 if (DEBUG) printf(" Enlazo variable %s, ahora vale %.2f\n",varname,*((raiz[controlador].variables)[v].valor));
 return 0;
 }
}

 

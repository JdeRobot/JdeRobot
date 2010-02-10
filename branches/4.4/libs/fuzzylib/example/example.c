#include <unistd.h>
#include <fuzzylib.h>
#include <stdio.h>

int k,k2;
float temperatura, apertura_puerta;
float ventilador;

int main()
{
  /*
    printf(" Hola caracola\n");
    k=fc_open("test1.fzz");
    k2=fc_open("test2.fzz");
    if (k>-1) fc_save(k,"test4.fzz");
    fc_close(k);
    k=fc_open("test2.fzz");
    if (k2>-1) fc_save(k2,"test3.fzz");
  */

k=fc_open("test.fzz");
temperatura = 20.2;
apertura_puerta = 15;

fc_link(k,"temperatura",&temperatura);
fc_link(k,"puerta",&apertura_puerta);
fc_link(k,"ventilador",&ventilador);

fc_output(k,"ventilador",&ventilador);
printf("Ventilador = %f \n",ventilador); 

_exit(0);
}

#include "Color.h"
#include <sstream>
#include <string>


Color::Color(unsigned char rojo, unsigned char verde, unsigned char azul){
	R=rojo;
	G=verde;
	B=azul;
}
	
unsigned char Color::getRojo()const{
	return R;
}
unsigned char Color::getAzul()const{
	return G;
}

unsigned char Color::getVerde()const{
	return B;
}

void Color::convertirAGris(){
	R=G=B=rgb2Gris();
}

Color Color::getGris()const{
	unsigned char gris=rgb2Gris();
	Color equivalenteGris(gris,gris,gris);
	return equivalenteGris;
}
	
bool Color::operator==(const Color& color)const{
		return ((R==color.R)&&(G==color.G)&&(B==color.B));
}

unsigned char Color::rgb2Gris()const{
	return (unsigned char)(R*0.3+G*0.59+B*0.11);
}

const char* Color::toString()const{
	std::stringstream s;
	s<<"R="<<(int)R<<" G="<<(int)G<<" B="<<(int)B;
	return ((s.str()).c_str());
}

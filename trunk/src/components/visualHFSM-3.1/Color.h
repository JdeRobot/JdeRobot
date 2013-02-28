#ifndef COLOR_H_
#define COLOR_H_

class Color{
public:
	Color(unsigned char rojo, unsigned char verde, unsigned char azul);
	
	unsigned char getRojo()const;
	unsigned char getAzul()const;
	unsigned char getVerde()const;
	
	void convertirAGris();
	Color getGris()const;
	
	bool operator==(const Color& color)const;
	const char* toString()const;
private:
	unsigned char rgb2Gris()const;
	unsigned char R,G,B;
};

#endif /*COLOR_H_*/

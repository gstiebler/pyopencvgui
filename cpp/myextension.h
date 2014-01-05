#ifndef _MY_EXTENSION
#define _MY_EXTENSION

#include "base.h"
#include "Image8Bits.h"
#include "ImageRGB.h"
#include <set>

class SeismicProcess
{
public:
	SeismicProcess(uchar *srcImgData, uchar *dstImgData, int height, int width, int numPixelsString, int xD, int yD );

	~SeismicProcess();

	void executar( int offset );
	
private:
		
	enum FE_RESPONSE { E_ALL_BLACK = -1, E_ALL_WHITE = -2, E_PREVIOUS_POINT = -3, E_SPECIAL_POINT = -4 };

	char _vX[16], _vY[16], _vXi[16], _vYi[16];

	Image8Bits src;
	ImageRGB dst;

	int _numPixelsString;
	int _xD;
	int _yD;

	Cor violeta; // somente mais claros em volta. 
	Cor vermelho; // somente pretos em volta
	Cor azul;
	Cor azulClaro; // Primeira trilha voltou para o primeiro pixel. sumTurns > 0
	Cor verdeClaro; // sumTurns >= 0
	Cor verdeEscuro;
	Cor azulMax; // first black pixel. Primeiro pixel escuro partindo do primeiro pixel de debug
	Cor verdeMax; // segunda trilha
	Cor vermelhoMax; // primeira trilha
	Cor amarelo; // X e Y selecionado de debug
	Cor branco;
	Cor preto;
	Cor rosa;

	void initDirections();

	int normalize( int value );

	void calcSumTurn( int &sumTurns, int blackIndex, int lastBlackIndex );

	int findFirstWhite( int startIndex, char *vX, char *vY, Point &ini, uchar selfValue );
	int findFirstBlack( int startIndex, char *vX, char *vY, Point &ini, Point &previousPoint, Point &specialPoint, uchar selfValue );

	
	int findWhiteBlackEdge( Point &pointIni, char *vX, char *vY, uchar startIndex, Point &previousPoint, Point &specialPoint, uchar selfValue);
	int findBlackWhiteEdge( Point &pointIni, char *vX, char *vY, uchar startIndex, Point &previousPoint, Point &specialPoint, uchar selfValue);

	double quadrantCorrection( double ang, int x, int y);

	Cor paintSumTurn(int sumTurns, int x, int y );

	Cor processPixel( int x, int y, int selfValue );
};

#endif
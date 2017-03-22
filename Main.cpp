#include "Header.h"
#include <stdio.h>
#include <iostream>

void main(int argc, char **argv)
{
	Color **pixelsArray;
	header h;
	DIB d;
	ReadBMP("lena.bmp", pixelsArray, h, d);
	SplitBMPFile("lena.bmp", h, d, pixelsArray, 3, 3);
	system("pause");
}


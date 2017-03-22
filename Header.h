#include <stdint.h>
#include <stdio.h>
#define Max 100
#pragma pack(1)
struct MagicNumber
{
	unsigned char first;
	unsigned char last;
};
struct Color
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};

struct header
{
	MagicNumber mg;
	uint32_t size;
	uint16_t AppSpecific1;
	uint16_t AppSpecific2;
	uint32_t StartOffset;
};

struct DIB
{
	uint32_t DIBsize;
	uint32_t width;
	uint32_t height;
	uint16_t ColorPlane;
	uint16_t BitsPerPixel;
	uint32_t CompressMethod;
	uint32_t BitmapSize;
	uint32_t HorizontalResolution;
	uint32_t VerticalResolution;
	uint32_t ColorCount;
	uint32_t ImportantColorCount;
};

header BMPHeader(FILE *f);
DIB BMPDIB(FILE* f);
void ReadBMP(char source[], Color **&pixelsArray, header &h, DIB &d);
void WriteBMP(char des[], header h, DIB d, Color **pixelsArray, int hStart, int hEnd, int wStart, int wEnd);
void ReleasePixelsArray(Color **pixelsArray, DIB d);
void SplitBMPFile(char *source, header h, DIB d, Color **pixelsArray, int hSplit, int dSplit);
char *GetFileName(char *source);
void GenerateFilename(char *source, int i, int j, char filename[]);
void intToStr(int n, char s[]);
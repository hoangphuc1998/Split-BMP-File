#include "Header.h"
#include <string.h>
#include <iostream>

header BMPHeader(FILE *f)
{
	if (f == NULL)
	{
		printf("Error");
	}
	else
	{
		fseek(f, 0, SEEK_SET);
		header h;
		fread(&h, sizeof(header), 1, f);
		return h;
	}
}

DIB BMPDIB(FILE* f)
{
	if (f == NULL)
	{
		printf("Error");
	}
	else
	{
		DIB d;
		fread(&d, sizeof(DIB), 1, f);
		return d;
	}
}

void ReadBMP(char source[], Color **&pixelsArray, header &h, DIB &d)
{
	FILE *f = fopen(source, "rb");
	if (f == NULL)
	{
		printf("Error in accessing bitmap file\n");
		return;
	}
	h = BMPHeader(f);
	d = BMPDIB(f);
	//Create the pixels array
	pixelsArray = new Color *[d.height];
	int padding = (4 - (d.width * (d.BitsPerPixel / 8)) % 4) % 4;
	for (int i = 0; i < d.height; i++)
	{
		pixelsArray[i] = new  Color[d.width];
	}
	//Begin reading pixels array
	for (int i = 0; i < d.height; i++)
	{
		fread(pixelsArray[i], sizeof(Color), d.width, f);
		//Skip padding
		fseek(f, padding, SEEK_CUR);
	}
	printf("Read BMP file success\n");
	fclose(f);
}

void WriteBMP(char des[], header h, DIB d, Color **pixelsArray, int hStart, int hEnd, int wStart, int wEnd)
{
	FILE *f = fopen(des, "wb");
	if (f == NULL)
	{
		printf("Error in writing bitmap file\n");
		return;
	}
	fwrite(&h, sizeof(header), 1, f);
	fwrite(&d, sizeof(DIB), 1, f);
	int padding = (4 - (d.width * (d.BitsPerPixel / 8)) % 4) % 4;
	for (int i = hStart; i < hEnd; i++)
	{
		for (int j = wStart; j < wEnd; j++)
		{
			fwrite(&pixelsArray[i][j], sizeof(Color), 1, f);
		}
		//skip padding
		for (int k = 0; k < padding; k++)
		{
			unsigned char skip = 0;
			fwrite(&skip, 1, 1, f);
		}
	}
	fclose(f);
}

void ReleasePixelsArray(Color **pixelsArray, DIB d)
{
	for (int i = 0; i < d.height; i++)
		delete[]pixelsArray[i];
	delete[]pixelsArray;
}

void SplitBMPFile(char *source, header h, DIB d, Color **pixelsArray, int hSplit, int wSplit)
{
	header HeaderSplit = h;
	DIB DIBSplit = d;
	DIBSplit.height = DIBSplit.height / hSplit;
	DIBSplit.width = DIBSplit.width / wSplit;
	for (int k = 1; k <= hSplit; k++)
	{
		if (k == hSplit) DIBSplit.height = d.height - (DIBSplit.height*(hSplit - 1));
		for (int l = 1; l <= wSplit; l++)
		{
			if (l == wSplit) DIBSplit.width = d.width - (DIBSplit.width*(wSplit - 1));
			//Change the information
			int padding = (4 - (DIBSplit.width * (DIBSplit.BitsPerPixel / 8)) % 4) % 4;
			DIBSplit.BitmapSize = DIBSplit.height*(DIBSplit.width + padding);
			HeaderSplit.size = DIBSplit.BitmapSize + HeaderSplit.StartOffset;
			char filename[100];
			GenerateFilename(source, hSplit - k + 1, l, filename);
			//--------------------------------
			int hStart, hEnd, wStart, wEnd;
			if (k == hSplit)
			{
				if (l == wSplit)
				{
					hStart = d.height - DIBSplit.height;
					hEnd = d.height;
					wStart = d.width - DIBSplit.width;
					wEnd = d.width;
				}
				else
				{
					hStart = d.height - DIBSplit.height;
					hEnd = d.height;
					wStart = (l - 1)*DIBSplit.width;
					wEnd = l*DIBSplit.width;
				}
			}
			else
			{
				if (l == wSplit)
				{
					hStart = (k - 1)*DIBSplit.height;
					hEnd = k*DIBSplit.height;
					wStart = d.width - DIBSplit.width;
					wEnd = d.width;
				}
				else
				{
					hStart = (k - 1)*DIBSplit.height;
					hEnd = k*DIBSplit.height;
					wStart = (l - 1)*DIBSplit.width;
					wEnd = l*DIBSplit.width;
				}
			}
			WriteBMP(filename, HeaderSplit, DIBSplit, pixelsArray, hStart, hEnd, wStart, wEnd);
		}
	}
	ReleasePixelsArray(pixelsArray, d);
}


void GenerateFilename(char *source, int i, int j, char filename[])
{
	char s1[10], s2[10];
	intToStr(i, s1);
	intToStr(j, s2);
	int n = strlen(source);
	strcpy(filename, source);
	filename[n - 4] = '\0';
	strcat(filename, s1);
	n = strlen(filename);
	filename[n] = 'x';
	filename[n + 1] = '\0';
	strcat(filename, s2);
	char *s = ".bmp";
	strcat(filename, s);
}

void intToStr(int n, char s[])
{
	int k = 0;
	while (n > 0)
	{
		int chuSo = n % 10;
		n = n / 10;
		s[k] = chuSo + '0';
		k++;
	}
	s[k] = '\0';
	//Reverse string
	for (int i = 0; i < strlen(s) / 2; i++)
	{
		s[i] = s[strlen(s) - i - 1];
	}
}
#include <stdio.h>
#include <time.h>

int main ()
{
	clock_t start, stop;

	start = clock();						// Note the start time for profiling purposes.

	FILE *fIn = fopen("f3.bmp","r");			// Input File name
	FILE *fOut = fopen("f3_blur.bmp","w+");		    	// Output File name

	int i,j,y, x;
	unsigned char byte[54];

	for(i=0;i<54;i++)						// lee encabezado
	{									
		byte[i] = getc(fIn);								
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);			// write the header back

	
    //calcula tamaños  de la imagen del encabezado, altura ancho y profundidad por bits
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width;					// tamaño de la imàgen

	unsigned char buffer[size][3];				
	unsigned char out[size][3];					

	for(i=0;i<size;i++)						// lee cada dato de la imàgen
	{
		buffer[i][2]=getc(fIn);					// blue
		buffer[i][1]=getc(fIn);					// green
		buffer[i][0]=getc(fIn);					// red
	}

	float v=1.0 / 9.0;						//  blurrring kernel
	float kernel[3][3]={{v,v,v},
						{v,v,v},
						{v,v,v}};

	for(x=1;x<height-1;x++)
	{					
		for(y=1;y<width-1;y++)
		{
			float sum0= 0.0;
			float sum1= 0.0;
			float sum2= 0.0;
			for(i=-1;i<=1;++i)
			{
				for(j=-1;j<=1;++j)
				{	
					// matrix multiplication with kernel with every color plane
					sum0=sum0+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][0];
					sum1=sum1+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][1];
					sum2=sum2+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][2];
				}
			}
			out[(x)*width+(y)][0]=sum0;
			out[(x)*width+(y)][1]=sum1;
			out[(x)*width+(y)][2]=sum2;
		}
	}

	for(i=0;i<size;i++)						//write image data back to the file
	{
		putc(out[i][2],fOut);
		putc(out[i][1],fOut);
		putc(out[i][0],fOut);
	}
		
	fclose(fOut);
	fclose(fIn);

	stop = clock();
	printf("\nCLOCKS_PER_SEC = %ld\n",stop-start); //1000000
	printf("%lf ms\n",((double)(stop-start) * 1000.0)/CLOCKS_PER_SEC );
	return 0;
}
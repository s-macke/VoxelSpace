/*
 *  This file contains the code to 
 *  extract the maps from the game 
 *  Comanche
 */

#include<png.h>

unsigned char buffer[1024*1024];
unsigned int palette[256];
unsigned char palettergb[256*3];
unsigned int imagewidth;
unsigned int imageheight;

const char *maps[] =
{
"C1W",        "D1",
"C2W",        "D2",
"C3",         "D3", 
"C4",         "D4",
"C5W",        "D5",
"C6W",        "D6",
"C7W",        "D7",
"C8",         "D6",
"C9W",        "D9",
"C10W",       "D10",  
"C11W",       "D11",  
"C12W",       "D11",
"C13",        "D13",   
"C14",        "D14",   
"C14W",       "D14",  
"C15",        "D15",   
"C16W",       "D16",  
"C17W",       "D17",  
"C18W",       "D18",  
"C19W",       "D19",  
"C20W",       "D20",  
"C21",        "D21",   
"C22W",       "D22",  
"C23W",       "D21",
"C24W",       "D24",  
"C25W",       "D25",  
"C26W",       "D18",  
"C27W",       "D15",  
"C28W",       "D25",
"C29W",       "D16"
};

void LoadDTA(const char *filename)
{
    for(int i=0; i<1024*1024; i++)
    {
        buffer[i] = 0;
    }

    FILE *file = fopen(filename, "rb");

    if (file == NULL) 
    {
        printf("file not found %s\n", filename);
        return;
    }
    unsigned int width=0, height=0;

    fseek(file, 8, SEEK_SET);
    fread(&width,  2, 1, file);
    fread(&height, 2, 1, file);
    printf("%i\n", width);
    printf("%i\n", height);
    width++;
    height++;
    imagewidth = width;
    imageheight = height;

    fseek(file, 0x80, SEEK_SET);

    unsigned int pos = 0;
    unsigned int line = 0;
    unsigned int x=0;
    unsigned int color=0;

while(1)
{
    int len = 1;
    fread(&x, 1, 1, file);
    color = x;
    if (x > 0xc0)
    {
        fread(&color, 1, 1, file);
        len = x & 0x3F;
    }

    for(int i=0; i<len; i++)
    {
        buffer[pos + line*1024] = color;
        pos++;

        if (pos >= width)
        {
            line++;
            pos = 0;
        }
    }
    if (line == height) break;
}

printf("%i\n", line);
printf("%i\n", ftell(file));

    fread(&x, 1, 1, file);

    for(unsigned int i=0; i<256; i++)
    {
        fread(&r, 1, 1, file);
        fread(&g, 1, 1, file);
        fread(&b, 1, 1, file);
        palette[i] = r | (g<<8) | (b<<16);
        palettergb[3*i+0] = r;
        palettergb[3*i+1] = g;
        palettergb[3*i+2] = b;
    }
    fclose(file);
}


void SavePNG(const char *filename, bool usepalette)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        return;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
       return;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
       png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
       return;
    }
    
    if (setjmp(png_jmpbuf(png_ptr)))
    {
       png_destroy_write_struct(&png_ptr, &info_ptr);
       fclose(fp);
       return;
    }
    
    png_init_io(png_ptr, fp);
    
    //    png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
    if (usepalette)
    {
        png_set_IHDR(png_ptr, info_ptr, 
        imagewidth, imageheight, 8, 
           PNG_COLOR_TYPE_PALETTE, 
           PNG_INTERLACE_NONE,
           PNG_COMPRESSION_TYPE_DEFAULT, 
           PNG_FILTER_TYPE_DEFAULT);
            
        png_set_PLTE(png_ptr, info_ptr, (png_color*)palettergb, 256);
    } else
    {
        png_set_IHDR(png_ptr, info_ptr, 
        imagewidth, imageheight, 8, 
           PNG_COLOR_TYPE_GRAY, 
           PNG_INTERLACE_NONE,
           PNG_COMPRESSION_TYPE_DEFAULT, 
           PNG_FILTER_TYPE_DEFAULT);
    }
    png_write_info(png_ptr, info_ptr);

    for (int y=0; y<imageheight; y++)
    {
        png_bytep row_pointer = (png_bytep)&buffer[y*1024];
        png_write_row(png_ptr, row_pointer);
    }

    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    fclose(fp);
}

void SaveMap(const char *filename, unsigned char *buffer, unsigned int size)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        return;
    }
    fwrite(buffer, size, 1, fp);
    fclose(fp);
}

int main()
{


for(int i=1; i<=25; i++)
{
    if (i == 8) continue;
    if (i == 12) continue;
    if (i == 23) continue;
    char filename[512];
    sprintf(filename, "/path/to/Comanche/D%i", i);
    LoadDTA(filename);
    sprintf(filename, "D%i.png", i);
    SavePNG(filename, false);
}


for(int i=0; i<30; i++)
{
    char filename[512];

    sprintf(filename, "/path/to/Comanche/%s.DTA", maps[2*i+0]);
    LoadDTA(filename);
    sprintf(filename, "data/map%i.color", i);
    SaveMap(filename, buffer, 1024*1024);
    sprintf(filename, "data/map%i.palette", i);
    SaveMap(filename, palettergb, 256*3);


    sprintf(filename, "/path/to/Comanche/%s.DTA", maps[2*i+1]);
    LoadDTA(filename);
    
    if (imagewidth == 512)
    {
        char temp[1024*1024];
        for(int ii=0; ii<1024*1024; ii++) temp[ii] = buffer[ii];
    
        for(int jj=0; jj<512; jj++)
        for(int ii=0; ii<512; ii++)
        {
            buffer[((ii<<1)+0) + ((jj<<1)+0)*1024] =  temp[((ii+0)&511) + ((jj+0)&511)*1024];
            buffer[((ii<<1)+1) + ((jj<<1)+0)*1024] = (temp[((ii+0)&511) + ((jj+0)&511)*1024] + temp[((ii+1)&511) + ((jj+0)&511)*1024]) / 2;
            buffer[((ii<<1)+0) + ((jj<<1)+1)*1024] = (temp[((ii+0)&511) + ((jj+0)&511)*1024] + temp[((ii+0)&511) + ((jj+1)&511)*1024]) / 2;
            buffer[((ii<<1)+1) + ((jj<<1)+1)*1024] = 
            (
            (int)
            temp[((ii+0)&511) + ((jj+0)&511)*1024] + 
            temp[((ii+1)&511) + ((jj+0)&511)*1024] + 
            temp[((ii+0)&511) + ((jj+1)&511)*1024] + 
            temp[((ii+1)&511) + ((jj+1)&511)*1024]
            ) / 4;
        }
    }
    sprintf(filename, "data/map%i.height", i);
    SaveMap(filename, buffer, 1024*1024);
}

/*
    LoadDTA("/cygdrive/w/UseNext/COMANCHE/C1");
    SavePNG("C1.png", true);
    LoadDTA("/cygdrive/w/UseNext/COMANCHE/C2");
    SavePNG("C2.png", true);
    LoadDTA("/cygdrive/w/UseNext/COMANCHE/C3");
    SavePNG("C3.png", true);
    LoadDTA("/cygdrive/w/UseNext/COMANCHE/C4");
    SavePNG("C4.png", true);
    LoadDTA("/cygdrive/w/UseNext/COMANCHE/C5");
    SavePNG("C5.png", true);
    LoadDTA("/cygdrive/w/UseNext/COMANCHE/C6");
    SavePNG("C6.png", true);
    LoadDTA("/cygdrive/w/UseNext/COMANCHE/C7");
    SavePNG("C7.png", true);
    LoadDTA("/cygdrive/w/UseNext/COMANCHE/C8");
    SavePNG("C8.png", true);

    LoadDTA("/cygdrive/w/UseNext/COMANCHE/C9W");
    SavePNG("C9W.png", true);
    LoadDTA("/cygdrive/w/UseNext/COMANCHE/C10W");
    SavePNG("C10W.png", true);
    LoadDTA("/cygdrive/w/UseNext/COMANCHE/C11W");
    SavePNG("C11W.png", true);
    LoadDTA("/cygdrive/w/UseNext/COMANCHE/C12W");
    SavePNG("C12W.png", true);

for(int i=1; i<=11; i++)
{
    char filename[512];
    sprintf(filename, "/cygdrive/w/UseNext/comanch2/D%i", i);
    LoadDTA(filename);
    sprintf(filename, "D%i.png", i);
    SavePNG(filename, false);
}
*/

//    LoadDTA("/cygdrive/w/UseNext/COMANCHE/CREDIT1");
//    FILE *file = fopen("w:/UseNext/COMANCHE/C1", "rb");
//    FILE *file = fopen("w:/UseNext/COMANCHE/D1", "rb");
    
    return 0;
}

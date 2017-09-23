/*
 *  This file contains the code to 
 *  extract the maps from the game 
 *  Comanche3
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include<png.h>
#include <fnmatch.h>

typedef unsigned char ubyte;
// -------------------------------------

unsigned int flength(FILE *fp)
{
    fseek(fp, 0L, SEEK_END);
    unsigned int sz = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    return sz;
}

// -------------------------------------
// Reverse engenineered uncompress from Editor.exe from Comanche Gold

unsigned int GetBits(unsigned int &bitbyte_offset, ubyte **compptr, unsigned int bit_length)
{
    *compptr += bitbyte_offset >> 3;
    unsigned int bit_offset = (bitbyte_offset & 7);
    bitbyte_offset = bit_offset + bit_length;
    unsigned int eax = *((unsigned int*)*compptr);
    eax = (eax >> bit_offset) &((1<<bit_length)-1);
    return eax;
}

void Decompress(ubyte *compptr, ubyte *rawptr, unsigned int size)
{
    unsigned int bit_length = 9;
    unsigned int bitbyte_offset = 0;
    unsigned int table2_size = 0x200;
    unsigned int table2_index = 0x102;
    unsigned int var2 = 0;
    unsigned int var3 = 0;
    unsigned int table1_index = 0;
    ubyte varb1 = 0;
    ubyte varb2 = 0;
    ubyte* rawptrend = rawptr + size;
    ubyte table1[0x100000];

    unsigned int stack[0x10000];
    unsigned int stack_index = 0;
    
    while(1)
    {
        if (rawptr >= rawptrend) break;
        unsigned int eax = GetBits(bitbyte_offset, &compptr, bit_length);
        if (eax == 0x101) break;
        if (eax == 0x100) // reset
        {
            bit_length = 9;
            table2_size = 0x200;
            table2_index = 0x102;
            eax = GetBits(bitbyte_offset, &compptr, bit_length);
            table1_index = eax;
            var3 = eax;
            varb1 = eax&0xFF;
            varb2 = eax&0xFF;
            *rawptr = eax&0xFF;
            rawptr++;
            continue;            
        }
        table1_index = eax;
        var2 = eax;
        if (eax >= table2_index)
        {
            eax = var3;
            table1_index = eax;
            eax  = (eax & 0xFFFFFF00) | varb2;
            stack[stack_index] = eax;
            stack_index++;
        }
        loc_430036:
        if (table1_index <= 0xFF)
        { 
            eax = table1_index;
            varb2 = eax&0xFF;
            varb1 = eax&0xFF;
            stack[stack_index] = eax;
            stack_index++;
            if (stack_index != 0)
            {
                do
                {
                stack_index--;
                eax = stack[stack_index];
                *rawptr = eax&0xFF;
                rawptr++;
                } 
                while (stack_index != 0);                
            } 
            table1[table2_index*3 +2] = varb1;
            table1[table2_index*3 +0] = var3 & 0xFF;
            table1[table2_index*3 +1] = (var3>>8) & 0xFF;
            table2_index++;
            var3 = var2;
            if (table2_index < table2_size) continue;
            if (bit_length >= 0xD) continue;
            bit_length++;
            table2_size <<= 1;
            continue;
        } else
        {
            eax  = table1[table1_index*3+2];
            stack[stack_index] = eax;
            stack_index++;
            eax  = table1[table1_index*3+0]  | (table1[table1_index*3+1] << 8);
            table1_index = eax;
            goto loc_430036;
        }        
    }    
}
// -------------------------------------

class Image
{
    public:
    unsigned int width;
    unsigned int height;
    ubyte *buffer;
    bool usepalette;
    unsigned char pal[256*3];
    
    void SetWidthHeight(unsigned int w, unsigned int h)
    {
        width = w;
        height = h;
        buffer = new unsigned char[w*h];
    }
    
    Image()
    {
            buffer = NULL;
            usepalette = true;
    }
    ~Image()
    {
        if (buffer != NULL) delete[] buffer;
    }    
};

void SavePNG(const char *filename, const Image &image)
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
if (image.usepalette)
{
    png_set_IHDR(png_ptr, info_ptr, 
    image.width, image.height, 8, 
       PNG_COLOR_TYPE_PALETTE, 
       PNG_INTERLACE_NONE,
       PNG_COMPRESSION_TYPE_DEFAULT, 
       PNG_FILTER_TYPE_DEFAULT);
        
    png_set_PLTE(png_ptr, info_ptr, (png_color*)image.pal, 256);
} else
{
    png_set_IHDR(png_ptr, info_ptr, 
    image.width, image.height, 8, 
       PNG_COLOR_TYPE_GRAY, 
       PNG_INTERLACE_NONE,
       PNG_COMPRESSION_TYPE_DEFAULT, 
       PNG_FILTER_TYPE_DEFAULT);
}       
    png_write_info(png_ptr, info_ptr);
    
    for (int y=0; y<image.height; y++)
    {
        png_bytep row_pointer = (png_bytep)&image.buffer[y*image.width];
        png_write_row(png_ptr, row_pointer);
    }

    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    fclose(fp);
}
// -------------------------------------

void SaveMap(const char *filename, const Image &image)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp) return;
    fwrite(image.buffer, image.width * image.height, 1, fp);
    fclose(fp);
}

// -------------------------------------

bool LoadPCX(const char* filename, Image &image)
{
    FILE *fp;
    if ( (fp = fopen(filename, "rb")) == NULL ) return false;

    unsigned int complen = flength(fp);
    unsigned int head, width, height;
    
    // read header
    fread(&head, 4, 1, fp); 
    if (head != 0x31505A4C) //LZP1
    {
        fclose(fp);
        return false;
    }
    fread(&width, 4, 1, fp); 
    fread(&height, 4, 1, fp);
    printf("  width: %i\n", width);
    printf("  height: %i\n", height);
    image.SetWidthHeight(width, height);
    fread(&image.pal, 256, 3, fp); 

    fread(&head, 4, 1, fp);
    if (head == 0x50726543) // Cerp
    {
        fseek(fp, 0x400, SEEK_CUR);
    }
    
    complen -= ftell(fp);
    unsigned char *comp;
    if ( (comp = (unsigned char*)malloc(complen)) == NULL )
    {
        return false;
    }
    fread(comp, complen, 1, fp);
    Decompress(comp, image.buffer, width*height);
    fclose(fp);
    return true;
}

int main(int argc,char *argv[])
{
  DIR *dp;
  struct dirent *ep;     
  dp = opendir (".");

  if (dp != NULL)
  {
    while (ep = readdir (dp))
    {
        if (fnmatch("*.PCX", ep->d_name, 0) == 0)
        {
            fprintf(stderr, "Open: %s\n", ep->d_name);
            Image image;
            if (LoadPCX(ep->d_name, image))
            {
                char filename[1024]; filename[0] = 0;
                strcpy(filename, ep->d_name);
                unsigned int len = strlen(filename);
                if (len >= 3)
                {
                    filename[len-3] = 'p';
                    filename[len-2] = 'n';
                    filename[len-1] = 'g';
                    fprintf(stderr, "Save: %s\n", filename);
                    SavePNG(filename, image);
                }
                
                
            } else
            {
                printf("  Warning: Skipping file. Reading of file failed or file format unknown\n");
            }
            
        }
    }
    (void) closedir (dp);
  }
  else
    perror ("Couldn't open the directory");

/*
    Image image;
    if (!LoadPCX("C2M3_C.PCX", image))
    {
        return 1;
    }
    SavePNG("C2M3_C.PNG", image);
*/
    

return 0;

}





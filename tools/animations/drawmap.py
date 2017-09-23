import math
from PIL import Image, ImageDraw, ImageFont, ImageOps

# -----------------------------------------------------

colorimg = Image.open("C1W.png")
colormap = colorimg.load()

heightimg = Image.open("D1.png")
heightmap = heightimg.load()
pal = colorimg.palette.getdata()[1];

screen = Image.new("RGB", (512, 512))
screenmap = screen.load()

# -----------------------------------------------------

def Store():
    screen.save("images/out%03d.gif" % (Store.idx,), "GIF")
    Store.idx += 1
Store.idx = 0

# -----------------------------------------------------

def PrintBorder(title):
    draw = ImageDraw.Draw(screen)
    fnt = ImageFont.truetype('/usr/share/fonts/TTF/UbuntuMono-B.ttf', 20)
    draw.rectangle([(128, 128), (128+256, 128+256)], outline=(255,255,255,128))
    draw.text((200, 105), "1024 pixels", font=fnt, fill=(255,255,255,128))
    draw.text((0, 0), title, font=fnt, fill=(255,255,255,128))

    txt = Image.new('L', (512, 512))
    d = ImageDraw.Draw(txt)
    d.text((200, 105), "1024 pixels", font=fnt, fill=(255))
    w = txt.rotate(-90)
    screen.paste( ImageOps.colorize(w, (0,0,0), (255,255,255)), (0, 0),  w)



def DrawPeriodicMap():

    for j in range(0, 512):
        for i in range(0, 512):
            screenmap[i, j] = 0

    for j in range(128, 128+256):
        for i in range(128, 128+256):
            c =  colormap[((i<<2)+512) & 1023, ((j<<2)+512) & 1023]
            screenmap[i, j] = (pal[c*3+2]<< 16) | (pal[c*3+1] << 8) | pal[c*3+0]
    PrintBorder("Color Map")
    Store()

    for j in range(0, 512):
        for i in range(0, 512):
            screenmap[i, j] = 0

    for j in range(0, 512):
        for i in range(0, 512):
            c =  colormap[((i<<2)+512) & 1023, ((j<<2)+512) & 1023]
            screenmap[i, j] = (pal[c*3+2]<< 16) | (pal[c*3+1] << 8) | pal[c*3+0]
    PrintBorder("Color Map")
    Store()

#--------------------

    for j in range(0, 512):
        for i in range(0, 512):
            screenmap[i, j] = 0

    for j in range(128, 128+256):
        for i in range(128, 128+256):
            h = heightmap[((i<<2)+512) & 1023, ((j<<2)+512) & 1023]
            screenmap[i, j] = (h<<16) | (h << 8) | h
    PrintBorder("Height Map")
    Store()

    for j in range(0, 512):
        for i in range(0, 512):
            screenmap[i, j] = 0

    for j in range(0, 512):
        for i in range(0, 512):
            h = heightmap[((i<<2)+512) & 1023, ((j<<2)+512) & 1023]
            screenmap[i, j] = (h<<16) | (h << 8) | h
    PrintBorder("Height Map")
    Store()

#--------------------

DrawPeriodicMap()


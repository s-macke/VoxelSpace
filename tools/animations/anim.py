import math
from PIL import Image, ImageDraw, ImageFont
import numpy as np

# -----------------------------------------------------

def Init(width, height, colorfilename, heightfilename):
    global colorimg, colormap, heightimg, heightmap, pal, screen, screenmap
    colorimg = Image.open("../../maps/" + colorfilename)
    colormap = colorimg.load()

    heightimg = Image.open("../../maps/" + heightfilename)
    heightmap = heightimg.load()

    # the colormap uses a color palette
    pal = colorimg.palette.getdata()[1];

    screen = Image.new("RGB", (width, height))
    screenmap = screen.load()

# -----------------------------------------------------

class Point:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

# -----------------------------------------------------

def DrawVerticalLine(x, ytop, ybottom, c):
    if (ytop >= ybottom): return
    if (ytop < 0): ytop = 0

    rgb = (pal[c*3+2]<< 16) | (pal[c*3+1] << 8) | pal[c*3+0]
    if screen.width != 700:
        for j in range(math.floor(ytop), math.floor(ybottom)):
            screenmap[x+1024, j] = rgb
    else:
        for j in range(math.floor(ytop), math.floor(ybottom)):
            screenmap[x, j] = rgb


# -----------------------------------------------------

def Store():
    Store.n -= 1
    if Store.n <= 0:
        screen.save("images/out%03d.gif" % (Store.idx,), "GIF")
        Store.idx += 1
        Store.n = Store.modulo
        Store.modulo += 3
Store.idx = 0
Store.n = 0
Store.modulo = 10

# -----------------------------------------------------

def Horline(p1, p2, offset, scale, horizon, pmap):
    n = 700
    dx = (p2.x - p1.x) / n
    dy = (p2.y - p1.y) / n
    for i in range(0, n):
        xi = math.floor(p1.x) & 1023
        yi = math.floor(p1.y) & 1023
        xmap = (math.floor(p1.x) - pmap.x+256)
        ymap = (math.floor(p1.y) - pmap.y+256)
        if screen.width != 700:
            if (xmap<512) and (ymap<512):
                if (xmap>=0) and (ymap>=0):
                    screenmap[xmap, ymap] = 0xFFFFFF
                    screenmap[xmap+512, ymap] = 0xFFFFFF
        DrawVerticalLine(i, (heightmap[xi, yi]+offset)*scale+horizon, 511, colormap[xi, yi])
        p1.x += dx
        p1.y += dy
        Store()

# -----------------------------------------------------

hidden = np.zeros(700)

def HorlineHidden(p1, p2, offset, scale, horizon, pmap):
    n = 700
    dx = (p2.x - p1.x) / n
    dy = (p2.y - p1.y) / n
    for i in range(0, n):
        xi = math.floor(p1.x) & 1023
        yi = math.floor(p1.y) & 1023
        xmap = (math.floor(p1.x) - pmap.x+256)
        ymap = (math.floor(p1.y) - pmap.y+256)
        if screen.width != 700:
            if (xmap<512) and (ymap<512):
                if (xmap>=0) and (ymap>=0):
                    screenmap[xmap, ymap] = 0xFFFFFF
                    screenmap[xmap+512, ymap] = 0xFFFFFF
        heightonscreen = (heightmap[xi, yi] + offset) * scale + horizon
        DrawVerticalLine(i, heightonscreen, hidden[i], colormap[xi, yi])
        if heightonscreen < hidden[i]:
            hidden[i] = heightonscreen
        p1.x += dx
        p1.y += dy
#        Store()

# -----------------------------------------------------

def Rotate(p, phi):
    xtemp = p.x *  math.cos(phi) + p.y * math.sin(phi)
    ytemp = p.x * -math.sin(phi) + p.y * math.cos(phi)
    return Point(xtemp, ytemp)

# -----------------------------------------------------

def ClearAndDrawMaps(pmap):
    if screen.width == 700:
        for j in range(0, 512):
            for i in range(0, 700):
                screenmap[i, j] = 0xffa366
    else:
        for j in range(0, 512):
            for i in range(0, 512):
                h = heightmap[(i+pmap.x-256) & 1023, (j+pmap.y-256) & 1023]
                c =  colormap[(i+pmap.x-256) & 1023, (j+pmap.y-256) & 1023]
                screenmap[i,     j] = (pal[c*3+2]<< 16) | (pal[c*3+1] << 8) | pal[c*3+0]
                screenmap[i+512, j] = (h<<16) | (h << 8) | h

        for j in range(0, 512):
            for i in range(0, 700):
                screenmap[i+1024, j] = 0xffa366

# -----------------------------------------------------

def DrawBackToFront(p, phi, height, distance, pmap):
    ClearAndDrawMaps(pmap)
    for z in range(distance, 1, -2):
        pl = Point(-z, -z)
        pr = Point( z, -z)
        pl = Rotate(pl, phi)
        pr = Rotate(pr, phi)
        Horline(
            Point(p.x + pl.x, p.y + pl.y),
            Point(p.x + pr.x, p.y + pr.y),
            -height, -1./z*240., +120, pmap)

# -----------------------------------------------------

def DrawFrontToBack(p, phi, height, distance, pmap):
    ClearAndDrawMaps(pmap)
    dz = 1
    z = 5

    for i in range(0, 700):
        hidden[i] = 511

    while z < distance:
        pl = Point(-z, -z)
        pr = Point( z, -z)
        pl = Rotate(pl, phi)
        pr = Rotate(pr, phi)
        HorlineHidden(
            Point(p.x + pl.x, p.y + pl.y),
            Point(p.x + pr.x, p.y + pr.y),
            -height, -1./z*240., +100, pmap)
        z += dz
        #dz += 0.1

# -----------------------------------------------------

#Init(512+512+700, 512, "C1W.png", "D1.png")

#DrawBackToFront(Point(230, 0), 0, 50, 240, Point(230, 0))
#DrawFrontToBack(Point(230, 0), 0, 50, 400, Point(230, 0))

#for i in range(0, 360, 10):
#    print(i)
#    DrawFrontToBack(Point(590, 175), i/180.*3.141592, 50, 240, Point(590, 175))
#    Store.n=1
#    Store()

Init(700, 512, "C7W.png", "D7.png")
for i in range(0, 64):
    print(i)
    DrawFrontToBack(Point(670, 500 - i*16), 0, 120, 800, Point(670, 500 - i*16))
    Store.n=1
    Store()

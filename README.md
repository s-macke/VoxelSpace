# Voxel-Space
Landscape rendering in 20 lines of code

* [Project demo][project demo] page

## History

Let us go back to the year 1992. Processing power were 1000 times slower and acceleration via a GPU were unknown or unaffordable.
3D games graphics used very simple rendering algorithms and showed mostly polyons with a simple color.
![Game Gunship 2000 in 1991](images/gunship2000-1991.gif)

It was during that year Novalogic published the game Comanche. 
![Game Comanche in 1992](images/comanche-1992.gif)

The graphics were awesome and in my opinion 3-5 years ahead of its time. You see a lot more details, shading and even shadows. And all this with the same processing power as other 3D games.

## Technique

Comanche uses a technique called voxel space similar to raycasting. To display the landscape a 1024*1024 one byte height map and a 1024*1024 color map is used which you can download on this site. The algorithm draws vertical lines by shooting a ray from the camera into the map. The following figure demonstrate this technique.

![Line by line](images/linebyline.gif)

 * Clear Screen.
 * For visible surface determination start from the back and render to the front
 * Determine the line on the map, which corresponds to the same optical distance from the observer. Consider the field of view and persective correction.
 * Segment the line so that it matches the number of columns of the screen.
 * Load the height and color from the 2D maps corresponding of the segment of the line.
 * Do some perspective corrections for the height coordinate.
 * Draw a vertical line with the corresponding color with the height retrieved from the perspective correction.

The core algorithm contains in its simplest form only a few lines of code:

```python
def Draw(p, height, horizon, screen_width):
    # Draw from back to the front (high z coordinate to low z coordinate)
    for z in range(240, 1, -1):
        # Find line on map. This calculation corresponds to a field of view of 90°
        pleft = Point(-z + p.x, -z + p.y)
        pright = Point( z + p.x, -z + p.y)        
        # segment the line
        dx = (pright.x-pleft.x) / screen_width
        # Draw vertical line for each segment
        for i in range(0, screen_width):
            DrawVerticalLine(i, 
                (height - heightmap[pleft.x, pleft.y]) / z * 120. + horizon,
                colormap[pleft.x, pleft.y])
            p1eft.x += dx
            
# call the drawing function with position, height, horizon line and screen width parameter
Draw( Point(0, 0), 50, 120, 700 )
```
There are of course a lot of tricks to get higher performance not mentioned here.

## Maps
[color](maps/C1W.png),
[height](maps/D1.png)

![C1W.png](images/thumbnails/C1W.png)
![D1.png](images/thumbnails/D1.png)

[color](maps/C2W.png),
[height](maps/D2.png)

![C2W.png](images/thumbnails/C2W.png)
![D2.png](images/thumbnails/D2.png)

[color](maps/C3.png),
[height](maps/D3.png)

![C3.png](images/thumbnails/C3.png)
![D3.png](images/thumbnails/D3.png)

[color](maps/C4.png),
[height](maps/D4.png)

![C4.png](images/thumbnails/C4.png)
![D4.png](images/thumbnails/D4.png)

[color](maps/C5W.png),
[height](maps/D5.png)

![C5W.png](images/thumbnails/C5W.png)
![D5.png](images/thumbnails/D5.png)

[color](maps/C6W.png),
[height](maps/D6.png)

![C6W.png](images/thumbnails/C6W.png)
![D6.png](images/thumbnails/D6.png)

[color](maps/C7W.png),
[height](maps/D7.png)

![C7W.png](images/thumbnails/C7W.png)
![D7.png](images/thumbnails/D7.png)

[color](maps/C8.png),
[height](maps/D6.png)

![C8.png](images/thumbnails/C8.png)
![D6.png](images/thumbnails/D6.png)

[color](maps/C9W.png),
[height](maps/D9.png)

![C9W.png](images/thumbnails/C9W.png)
![D9.png](images/thumbnails/D9.png)

[color](maps/C10W.png),
[height](maps/D10.png)

![C10W.png](images/thumbnails/C10W.png)
![D10.png](images/thumbnails/D10.png)

[color](maps/C11W.png),
[height](maps/D11.png)

![C11W.png](images/thumbnails/C11W.png)
![D11.png](images/thumbnails/D11.png)

[color](maps/C12W.png),
[height](maps/D11.png)

![C12W.png](images/thumbnails/C12W.png)
![D11.png](images/thumbnails/D11.png)

[color](maps/C13.png),
[height](maps/D13.png)

![C13.png](images/thumbnails/C13.png)
![D13.png](images/thumbnails/D13.png)

[color](maps/C14.png),
[height](maps/D14.png)

![C14.png](images/thumbnails/C14.png)
![D14.png](images/thumbnails/D14.png)

[color](maps/C14W.png),
[height](maps/D14.png)

![C14W.png](images/thumbnails/C14W.png)
![D14.png](images/thumbnails/D14.png)

[color](maps/C15.png),
[height](maps/D15.png)

![C15.png](images/thumbnails/C15.png)
![D15.png](images/thumbnails/D15.png)

[color](maps/C16W.png),
[height](maps/D16.png)

![C16W.png](images/thumbnails/C16W.png)
![D16.png](images/thumbnails/D16.png)

[color](maps/C17W.png),
[height](maps/D17.png)

![C17W.png](images/thumbnails/C17W.png)
![D17.png](images/thumbnails/D17.png)

[color](maps/C18W.png),
[height](maps/D18.png)

![C18W.png](images/thumbnails/C18W.png)
![D18.png](images/thumbnails/D18.png)

[color](maps/C19W.png),
[height](maps/D19.png)

![C19W.png](images/thumbnails/C19W.png)
![D19.png](images/thumbnails/D19.png)

[color](maps/C20W.png),
[height](maps/D20.png)

![C20W.png](images/thumbnails/C20W.png)
![D20.png](images/thumbnails/D20.png)

[color](maps/C21.png),
[height](maps/D21.png)

![C21.png](images/thumbnails/C21.png)
![D21.png](images/thumbnails/D21.png)

[color](maps/C22W.png),
[height](maps/D22.png)

![C22W.png](images/thumbnails/C22W.png)
![D22.png](images/thumbnails/D22.png)

[color](maps/C23W.png),
[height](maps/D21.png)

![C23W.png](images/thumbnails/C23W.png)
![D21.png](images/thumbnails/D21.png)

[color](maps/C24W.png),
[height](maps/D24.png)

![C24W.png](images/thumbnails/C24W.png)
![D24.png](images/thumbnails/D24.png)

[color](maps/C25W.png),
[height](maps/D25.png)

![C25W.png](images/thumbnails/C25W.png)
![D25.png](images/thumbnails/D25.png)

[color](maps/C26W.png),
[height](maps/D18.png)

![C26W.png](images/thumbnails/C26W.png)
![D18.png](images/thumbnails/D18.png)

[color](maps/C27W.png),
[height](maps/D15.png)

![C27W.png](images/thumbnails/C27W.png)
![D15.png](images/thumbnails/D15.png)

[color](maps/C28W.png),
[height](maps/D25.png)

![C28W.png](images/thumbnails/C28W.png)
![D25.png](images/thumbnails/D25.png)

[color](maps/C29W.png),
[height](maps/D16.png)

![C29W.png](images/thumbnails/C29W.png)
![D16.png](images/thumbnails/D16.png)



[project demo]: https://s-macke.github.io/VoxelSpace/VoxelSpace.html

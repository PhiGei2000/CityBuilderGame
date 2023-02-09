# Terrain
## Surface classification
To determine the building behaviour it is necessary to classificate the surface structure.
### Surface vector
The surface vector describes the height differences of a surface. For the heights of the 4 corners h<sub>0</sub>,...,h<sub>3</sub> is the surface vector given by: <code>(h<sub>1</sub>-h<sub>0</sub>,h<sub>2</sub>-h<sub>0</sub>,h<sub>3</sub>-h<sub>0</sub>)</code>
### Surface types
![image](./terrainSurfaces.png)

low corners | high corners | type | surface vector | position in picture |
 :---: | :---: | :---: | :---: | --- |
3 | 1 | outer corner | (1,1,1) | left front and back |
4 | 0 | flat | (0,0,0) | center front |
2 | 2 | diagonal | (1,1,0) | center back |
1 | 3 | inner corner | (1,1,1) | right front and back |

The building behaviour depends on the gradient (i.e. roads an be build on a flat surface (0,0) and on a diagonal surface (1,0)).

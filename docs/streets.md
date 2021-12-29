# Streets
## Directions
The postitve z-Axis points in "south" direction. Therefor the positive x-Axis points in "east", the negative z-Axis in "north" and the negative x-Axis in "west" direction.

## Street types
| ID | Street Type | Connected "north" | Connected "east" | Connected "south" | Connected "west" |
| :---: | :--- | :---: | :---: | :---: | :---:|
| 0 | NOT_CONNECTED | 0 | 0 | 0 | 0 |
| 1 | END_SOUTH | 1 | 0 | 0 | 0 |
| 2 | END_WEST | 0 | 1 | 0 | 0 |
| 3 | CURVE_NORTH_EAST | 1 | 1 | 0 | 0 |
| 4 | END_NORTH | 0 | 0 | 1 | 0 |
| 5 | VERTICAL | 1 | 0 | 1 | 0 |
| 6 | CURVE_SOUTH_EAST | 0 | 1 | 1 | 0 |
| 7 | CROSSING_VERTICAL_WEST | 1 | 1 | 1 | 0 |
| 8 | END_EAST | 0 | 0 | 0 | 1 |
| 9 | CURVE_NORTH_WEST | 1 | 0 | 0 | 1 |
| 10 | HORIZONTAL | 0 | 1 | 0 | 1 |
| 11 | CROSSING_HORIZONTAL_NORTH | 1 | 1 | 0 | 1 |
| 12 | CURVE_SOUTH_EAST | 0 | 0 | 1 | 1 |
| 13 | CROSSING_VERTICAL_EAST | 1 | 0 | 1 | 1 |
| 14 | CROSSING_HORIZONTAL_SOUTH | 0 | 1 | 1 | 1 |
| 15 | CROSSING_HORIZONTAL_VERTICAL | 1 | 1 | 1 | 1 |

## Modifing streets
If a street is build next to an existing street, the exisiting street gets a new connection. This connection results from the direction in which the new street is placed.
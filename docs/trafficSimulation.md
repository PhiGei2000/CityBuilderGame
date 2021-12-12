Traffic Simulation
===
Streets
---
Streets were representated by numbers
| value | binary | street type | 
|:---:|---:|---|
|0 |00000| single street (not connected)|
|1 |10000| end bottom |
|2 |01000| end right |
|3 |11000| top left curve |
|4 |00100| end top |
|5 |10100| vertical street |
|6 |01100| left bottom curve |
|7 |11100| vertical street with connection left |
|8 |00010| end left |
|9 |10010| top right curve |
|10 |01010| horizontal street |
|11 |11010| horizontal street with connection top |
|12 |00110| bottom right curve |
|13 |10110| vertical street with connection right |
|14 |01110| horizontal street with connection bottom |
|15 |11110| crossing |
|16 |11111| no street

These values have been chosen so that streets can easily combined. For example a horizontal street (10101) and a curve from top to left (11100) become a horizontal street with a connection to the top (11101).\
Merging streets together is aquivalent to perform a binary OR operation on their values.
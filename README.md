# Assignment 3

## Usage
```./GIS <database file> <script> <log file>```

## Notes
- The database file is wiped every time the program is run.
- If script file is not found, error is printed and program exits.
- The log file is wiped every time the program is run.
- Two index types
    - ```name index```
        - ```Feature Name``` and ```State```
        - uses hashtable
        - probing function: (n^2 + n)/2
        - initial size of 1024, resize automatically, double at lambda = 0.7
    - ```coordinate index```
        - bucket pr quadtree
        - each leaf stores K data objects
- Uses buffer pool
    - stores 15 records
    - uses least recently used


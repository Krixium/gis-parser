# Assignment 3

## Build
Requires:
    - CMake 3.0.0 or later
    - C++ 14 or later
    - make

Steps:
    1. Make a folder named ```build``` and enter into it.
    2. Run the command ```cmake ..`` from the build folder to generate CMake files.
    3. Run ```make```.

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


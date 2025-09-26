# InvaderC
> console-based extensible 2D shooting game
## program info
- Smooth console rendering with double buffering  
- Real-time keyboard input for movement and shooting  
- Structured 2D object management and collision detection  
- Customizable sprites and animations  
- Modular design and cross-platform build support (not yet)  

## build/installation 
0. clone or donwnload this repo
    - clone 
        ```bash
        git clone https://github.com/esillileu/InvaderC.git
        cd InvaderC
        ```
    - download zip
        - unzip -> go inside of the folder
### Window
1. Install [CMake](https://cmake.org/download/) 
2. install [Visual Studio 2022](https://visualstudio.microsoft.com/ko/downloads/). make sure you checked ***Desktop Development using C++*** 
3. make build folder
    ```bash
    mkdir -p out/build
    cd out/build
    ```
4. make build cache with preset
    ```bash
    cmake ../.. ----preset msvc-debug
    camke --build msvc-x64-debug
    ```
5. start game with conhost
    ```bash
    conhost.exe .\msvc-x64-debug\Debug\InvaderC.exe
    ```

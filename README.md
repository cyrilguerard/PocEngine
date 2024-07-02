# PocEngine

Learning Vulkan by developing a POC game engine. 

## Prerequisites

- Windows 10/11 64 bits (may work on other OS)
- Visual Studio 2022
- Vulkan SDK
- CMake

## Build

- Open the root folder in VS
- Menu Build > Build All (Ctrl + Shift + B)

## Run

- Select the executable & run using the tool bar in VS

### demo-01-simple-window

Initialize Vulkan API and POCEngine lib to display a resizable window.

![Screenshot](/demo-01-simple-window.png?raw=true)

### demo-02-draw-simple-shape

Load a scene with a mesh of one red triangle.

![Screenshot](/demo-02-draw-simple-shape.png?raw=true)

### demo-03-depth-test

Load a scene with a mesh of three triangles with different Z-axis, use depth-test feature.

![Screenshot](/demo-03-depth-test.png?raw=true)


## Features

 - Vulkan context initialization
 - Display the game window
 - Draw simple shapes
 - Depth tests
 - more to come...

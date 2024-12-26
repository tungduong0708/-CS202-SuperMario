# Super Mario Project

## Overview
This project is a recreation of the classic Super Mario game using modern C++ and various libraries such as Raylib for graphics and Box2D for physics. The game includes multiple states, such as the main menu, settings, gameplay, and more. It also features various game objects, characters, and animation effects, all managed through a robust architecture using design patterns like Singleton, Prototype, and Abstract Factory.

## Features
- **Multiple Game States**: Main menu, settings, gameplay, pause, game over, victory, etc.
- **Character and Object Management**: Using creators and visitors to manage game objects and characters.
- **Animation Effects**: Various animation effects for actions like scoring, collecting coins, and character transformations.
- **Physics Engine**: Box2D is used for realistic physics simulation.
- **Graphics**: Raylib is used for rendering graphics.

## Project Structure
- `src/`: Contains the source code for the game.
- `include/`: Contains the header files for the game.
- `resources/`: Contains the resources like images, sounds, and fonts used in the game.
- `build/`: Contains the build files and executables.

## Prerequisites
- **C++ Compiler**: A modern C++ compiler that supports C++20.
- **CMake**: Version 3.10 or higher.
- **Raylib**: For graphics rendering.
- **Box2D**: For physics simulation.

## Setup Tutorial

### Requirements
Make sure to install the C/C++ extension pack in your editor to run the project.

### Steps to Set Up

1. **Pull the Repository**:  
   Go to GitHub and pull the repository. I have added the `CMakeLists.txt` file, which will handle the configuration automatically.

2. **Automatic Configuration**:  
   If the configuration doesn't happen automatically, follow these steps:
   
   - Open **CMake Tools** in your editor.
   - Hover over "No Kit Selected" and click the pencil icon next to it.
   - Select the GCC version installed on your system (e.g., I am using GCC 14.2). Once selected, it should look like this:

3. **Manually Configure**:  
   If the project doesn't automatically configure after selecting the GCC version, manually select the `CMakeLists.txt` file and press `Ctrl + S` to save.

4. **Build the Project**:  
   Click on the right-most arrow icon at the top of the window and select **SuperMario** to build the project.

5. **Run or Debug**:  
   Once the project is built, use the right-most arrow to run the project in release mode or the bug icon for debugging in the bottom bar.

6. **Fix Include Path Issues**:  
   If the include paths in `.cpp` files are marked in red, go to `.vscode/c_cpp_properties.json`. Add the following `includePath` entries:

   ```json
   "includePath": [
       "${default}",
       "${workspaceFolder}/include",
       "${workspaceFolder}/build/_deps/raylib-build/raylib/include",
       "${workspaceFolder}/build/_deps/box2d-src/include"
   ]

## Running the Game
After building the project, the executable will be located in the `bin` directory. Run the executable to start the game.

## License
This project is for educational purposes and is provided as-is. Please do not copy it for your own projects without proper attribution.

## Acknowledgements
- **Raylib**: For providing a simple and easy-to-use graphics library.
- **Box2D**: For providing a robust physics engine.
- **CMake**: For managing the build process.

Enjoy playing the game and exploring the code!

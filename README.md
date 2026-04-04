# My2DMind


![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-0E1128?logo=unrealengine&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?logo=c%2B%2B&logoColor=white)
![License](https://img.shields.io/badge/license-MIT-green)

**My2DMind** is an innovative project that features a 2D character navigating a 3D world with a Pokémon-like gameplay theme. Developed using Unreal Engine 5 and C++, this project highlights advanced gameplay mechanics, character control, and world interaction.


## Live Demo

[![Watch Gameplay Demo](https://img.youtube.com/vi/ZocUvVXtif4/maxresdefault.jpg)](https://youtu.be/ZocUvVXtif4)

> Click the thumbnail above to watch the full demo video.

## Key Components

### Source
- **CharacterController.cpp:** Manages the 2D character's movements, interactions, and animations within the 3D environment.
- **WorldManager.cpp:** Oversees the 3D world setup, including environmental interactions and dynamic elements.
- **GameplayLogic.cpp:** Implements the core gameplay mechanics, such as quest handling and object collection.

## Features

- **2D Character in 3D World:** Integrates a 2D character seamlessly into a 3D environment.
- **Advanced Gameplay Mechanics:** Includes Pokémon-like gameplay elements, such as quests and interactions.
- **Unreal Engine 5 Integration:** Leverages the power of Unreal Engine 5 for enhanced graphics and performance.

This project serves as a comprehensive example for developers interested in blending 2D and 3D gameplay elements using Unreal Engine 5.
[YouTube Video](https://youtu.be/ZocUvVXtif4?si=KIxYEW614bNxPQZ3)

![7](https://github.com/khaled71612000/2D-Game-Pokemon-Unreal3D/assets/59780800/09a20c5b-5623-4d5d-abd3-e890b4a1148a)
![image](https://github.com/khaled71612000/My2DMind/assets/59780800/e149479c-88d3-4ce9-aac9-9ec5012f2d38)
![image](https://github.com/khaled71612000/My2DMind/assets/59780800/b18faedc-c9ca-4874-a3d6-d6d59766faea)



## Architecture

Key classes and systems (from source code):

- **`My2DMind`** (extends `ModuleRules`) — core game class


## Tech Stack

| Technology | Role |
|---|---|
| Unreal Engine 4/5 | Game engine (Blueprints + C++) |
| C++ | Gameplay systems, custom components |
| HLSL / USF | Custom shader authoring |
| Git LFS | Large asset version control |


## Getting Started

### Prerequisites
- [Unreal Engine](https://www.unrealengine.com/en-US/download) 4.x or 5.x
- Visual Studio 2019 or 2022 with **Desktop development with C++**
- Git LFS (`git lfs install`)

### Setup
```bash
git lfs install
git clone https://github.com/khaled71612000/2D-Game-Pokemon-Unreal3D.git
```
1. Right-click the `.uproject` → **Generate Visual Studio project files**
2. Open `.sln` → Build (**Development Editor | Win64**)
3. Launch via Unreal Editor

<!-- repo-rank: 8 -->
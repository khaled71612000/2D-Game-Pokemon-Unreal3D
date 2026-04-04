# 2D Game Pokemon Unreal3D

> An innovative Unreal Engine project featuring a 2D character navigating a 3D world with Pokemon-like gameplay mechanics including turn-based encounters and collectible creatures.

![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-0E1128?logo=unrealengine&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?logo=c%2B%2B&logoColor=white)
![License](https://img.shields.io/badge/license-MIT-green)

## Overview

An innovative Unreal Engine project featuring a 2D character navigating a 3D world with Pokemon-like gameplay mechanics including turn-based encounters and collectible creatures.

Built with **Unreal Engine** and **C++**, demonstrating professional game development patterns: the Actor-Component model, Unreal's reflection system (UPROPERTY/UFUNCTION), Blueprint interoperability, and optimized gameplay systems.

## Features

- Unreal Engine gameplay framework (GameMode, GameState, PlayerController)
- Custom C++ Actor and Component classes
- Blueprint-C++ interoperability
- Physics and collision systems
- Optimized asset loading

## Technologies Used

| Technology | Details |
|------------|---------|
| Unreal Engine | 4.x / 5.x |
| C++ | Modern C++17 |
| Blueprints | Visual scripting |
| Chaos Physics | Physics simulation |

## Screenshots / Demo

> *No screenshots available yet.*

## Getting Started

### Prerequisites

- [Unreal Engine](https://www.unrealengine.com/en-US/download) 4.x or 5.x
- Visual Studio 2019 or 2022 with **Desktop development with C++**
- Git LFS

### Installation

```bash
git lfs install
git clone https://github.com/khaled71612000/2D-Game-Pokemon-Unreal3D.git
cd 2D-Game-Pokemon-Unreal3D
```

1. Right-click `.uproject` → **Generate Visual Studio project files**
2. Open `.sln` in Visual Studio
3. Set config: **Development Editor | Win64**
4. Build (Ctrl+Shift+B) then launch the editor

## Project Structure

```
2D-Game-Pokemon-Unreal3D/
├── Source/                  # C++ source files
  Source/My2DMind/My2DMind.cpp
  Source/My2DMind/My2DMind.h
  Source/My2DMind/My2DMindGameModeBase.cpp
  Source/My2DMind/My2DMindGameModeBase.h
  Source/My2DMind/Private/AnimationCharacter.cpp
  Source/My2DMind/Private/EnemyCharacter.cpp
├── Content/                 # Assets, blueprints, levels
├── Config/                  # Project settings
└── 2D-Game-Pokemon-Unreal3D.uproject
```

## License

[MIT License](LICENSE)

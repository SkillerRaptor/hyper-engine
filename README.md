﻿# HyperEngine
![](https://img.shields.io/badge/license-MIT-yellow)
![](https://github.com/SkillerRaptor/HyperEngine/workflows/build-windows/badge.svg)
![](https://github.com/SkillerRaptor/HyperEngine/workflows/build-linux/badge.svg)
[![Trello](https://img.shields.io/badge/board-trello-blue.svg)](https://trello.com/b/xiuhLb05/hyperengine)

The HyperEngine only supports OpenGL 3.3 and OpenGL 4.6, but will support Vulkan, DirectX11 and DirectX12 in the future. 
The Engine is layed out to work on Windows x64 and Linux.

# Building
In order to build the engine, you will need to follow these steps:
1. Start cloning the repository by typing in your command prompt <code>git clone --recursive <a href="https://github.com/SkillerRaptor/HyperEngine">https://github.com/SkillerRaptor/HyperEngine</a></code>.
2. Run the script with the name of your operating system in the [Scripts](https://github.com/SkillerRaptor/HyperEngine/tree/master/Scripts) folder.<br>(For Windows: [`Win-GenProjects.bat`](https://github.com/SkillerRaptor/HyperEngine/blob/master/Scripts/Win-GenProjects.bat), For Linux: [`Linux-GenProjects.sh`](https://github.com/SkillerRaptor/HyperEngine/blob/master/Scripts/Linux-GenProjects.sh)) 
3. After premake generates the project files. You can open the project and run the engine.

# Screenshots
![alt-text](https://i.imgur.com/I3OJPW7.png)

## Dependencies & License
-   [GLAD](https://github.com/Dav1dde/glad/blob/master/LICENSE) Copyright (c) MIT, David Herberth 2013-2020
-   [glfw](https://github.com/glfw/glfw/blob/master/LICENSE.md) Copyright (c) zlib, Camilla Löwy 2006-2019
-   [glm](https://github.com/g-truc/glm/blob/master/copying.txt) Copyright (c) The Happy Bunny License, G-Truc Creation 2005
-   [Dear ImGui](https://github.com/ocornut/imgui/blob/master/LICENSE.txt) Copyright (c) MIT, Omar Cornut 2014-2020
-   [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo/blob/master/LICENSE) Copyright (c) MIT, Cedric Guillemet 2016
-   [stb_image](https://github.com/nothings/stb/blob/master/LICENSE) Copyright (c) MIT, Sean Barrett 2017

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)

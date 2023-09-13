## Johnnie
A software tool for rendering animated 3D models

<p align="center">
	<img src="Screenshots/Johnnie.gif?raw=true" width=700 height=394 alt="animated">
</p>  

### Description

Program was written as a part of engineering thesis for computer science degree. The goal of this thesis was to make a software tool rendering both static and animated 3D models using skeletal animation as an animation technique,
and modern OpenGL as an rendering backend. The result of the implementation work is a comprehensive software tool that meets the established requirements. The application allows user to load selected models and animations, 
supporting three model file formats. Rich graphical user interface offers several visualization modes, tools for manual loading object textures, editing lighting parameters, as well as manipulation of other parameters of the virtual world.

The codebase is divided into two modules - engine module compiled into static libary and sandbox application called 'Johnnie'. Majority of the code is reusable and resides on the engine side. The client application basically implements the whole UI that integrates engine features exposed via API.

Features:
* Static / skeletal mesh loading
* Loading and playing skeletal animations
* Model material specification - albedo / diffuse / specular textures loading 
* Point and directional lights (old school Blinn-Phong lighting model)
* Shadow mapping
* Logging / profiling tools
* Scalable and easy to use API

### Building for Windows

```
git clone https://github.com/09pawel0898/Johnnie
```

**Microsoft Visual Studio** 
Enter `Scripts/` directory, run `GenerateProjectFiles_VS2019.bat` script for VS2019 project generation. Navigate to `Build/`, open `Project.sln` and build `Johnnie` target.

### Screenshots

<p align="center">
	<img src="Screenshots/Johnnie2.gif?raw=true" width=700 height=394 alt="animated">
	<img src="Screenshots/Johnnie3.png?raw=true" width=700 height=394 alt="animated">
</p>  

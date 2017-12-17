# Asiergine  

Asiergine is a 3D Game Engine created by Carlos Cabreira and Asier Iglesias, in OpenGL and C++. The project starts as a 
educational work for the Game Engines subject on the Politecnical University of Catalunya, Spain.
![Teamphoto](https://i.imgur.com/k9t5vBT.jpg)  

Our goal was to achieve a working 3D space with support of the basic elements of a game engine: game object hierarchy, components, rendering modes, time management and more.
We also created Wwished, a library that integrates the Wwise audio engine ina simple, quick manner.

### Work log    
**Carlos Cabreira**   
 - Camera rendering: OpenGl 3D space drawing of texturized geometry
 - Import capabilities: Asiergine can import geometry and textures from FBX files
 - Scene serialization: Asiergine saves data to own binary format, .carca
 - Component utility: different components can be added to game objects to define their behaviour in the game
 - Time managment: the game speed can be controlled in real time. Also, the user can pause the game in middle of execution.
 - Game Object hierarchy: all game objects follow a hierarchy that inherits transformations from parents to childs. Game objects can be 
   transformed using visual gizmos
 - Resource management: meshes, scenes and textures are structured in a Library system that avoids double loading and improves      performance
 - Wwise Audio Engine integration: via own Wwished library, Asiergine can take advantage of the powerful audio engine to implement music and FX in games.

**Asier:**   
 - Engine UI: organization and coding of all visual elements and options in the editor window
 - Component and object edition: creation, deletion and modification of the different elements of the engine
 - Scene quadtree: static objects are included in a 3D Quadtree that accelerates processing.
 - Camera culling system: optimization to avoid drawing 3D objects that are not in camera. Accelerated via Quadtree.
 - Asset explorer: see all resources available, from meshes to soundbanks.
 

### Sub-system   
The main core of our game engine is the integration of the Wwise audio engine with a few functions:
- Two background musics tracks blend between each other in loop.
- Static and moving gameobjects with spatial audio source.
- Tunnel effect in an area of the level that distorts the audio.

**Engine video:**   


![Code repository](https://github.com/carcasanchez/Asiergine/tree/master)   

<dl>
  <a href="https://github.com/carcasanchez/Asiergine/releases/download/Assignment3/Asiergine_v1.0.zip" class="btn">Download Latest Build</a>
</dl>
   
![License](https://github.com/carcasanchez/Asiergine/blob/Assignment2%E2%80%8B/LICENSE)   

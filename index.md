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
 

## About Wwished library

 Wwished is a own library, created to integrate the Wwise audio engine in a quick, easy fashion.    
Wwished sacrifices depth and complexity in order to offer a simpler approach.   
The only thing you need to do, in order to use it, is download the Wwise SDK, add "$(WWISESDK)\include" to your additional include directories,   
and add "Wwished.h" to your project. 

Warning: by default, "Wwished.h" uses a bunch of libraries that are placed in the "3DEngine/AK/" route. Be sure that this libraries are placed properly in your project, or rename the routes in "Wwished.h".   
   
In the Audio module, Wwished is mainly called three times:   

- Wwished::InitWwished(): receives the language and the Sounbanl directory. Is called on start, and it takes care of load the Init soundbank and all Wwise initial proccess;
- Wwished::ProccessAudio(): called each post-update, this function renders all the audio and reproduces sound.
- Wwished::CloseWwished(): called on exit, takes care of shut down Wwise functionallity in a safe way.

Wwished holds three main aspects: Emitters, Events and Utility.    
Emitters are the synonymous of Wwise GameObjects. They marks the spatial position of a sound or a listener.   
Events are triggers that Wwise receives and plays in the way you configure. Wwished only stores the name and the Emitter of the Event:   
the rest of the work must be done on Wwise.   
Last, Wwished utility includes things like StopAllSounds, LoadAudioBank, Pause/Restart sounds, state changes and more.   

## About audio in Asiergine

 In order to create audio in a Asiergine scene, you must first load a Wwise soundbank. The soundbank must be placed in the Assets/Sounbanks folder.
To load a Sounbank, go to Configuration->Audio, and type the name of the bank you want to load. In the case of this release, only the Main bank is available.

After loading a bank, you must add a Listener, usually in the Camera object. Select the camera, add an Audio Component to it, and select the type Listener.
You can only have one listener in scene.

 Now, the only thing you need to do is populate the scene with sound.
You have various options: 

- FX: just set the Audio Type in Component Audio to FX. You must now add Events to this component. 
Events must be defined in Wwise first. In the Component Editor, you can add or delete events, and specify when this events will trigger.
By default, only two possibilities are included (when pressing the E key, or when the game starts running).

- Music: just set the Audio Type in Component Audio to Music. As with FX, you must specify the Event that will trigger the music. Also, you 
have the possibility of add a State group to make transitions between two states. Just create the group and states in Wwise, and specify in the 
Component the name of the Group, the two states and the time between changes.
All music starts on play.

- Reverb areas: add a Reverb Component to any object, and transform the box to fit the desired size. In the Component editor, add the name of the effect you want (previously defined on Wwise), and its value. Any object that is inside the box will note the marked effect.



**Engine video:**   


![Code repository](https://github.com/carcasanchez/Asiergine/tree/master)   

<dl>
  <a href="https://github.com/carcasanchez/Asiergine/releases/download/Assignment3/Asiergine_v1.0.zip" class="btn">Download Latest Build</a>
</dl>
   
![License](https://github.com/carcasanchez/Asiergine/blob/Assignment2%E2%80%8B/LICENSE)   

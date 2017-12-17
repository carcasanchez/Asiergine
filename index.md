# Asiergine  

 
 <a href="https://gyazo.com/415d9b12faa2e0a3c8f1f321805f3c14"><img src="https://i.gyazo.com/415d9b12faa2e0a3c8f1f321805f3c14.png" alt="https://gyazo.com/415d9b12faa2e0a3c8f1f321805f3c14" width="1272.6666666666667"/></a>
   
Asiergine is a 3D Game Engine created by Carlos Cabreira and Asier Iglesias, in OpenGL and C++. The project starts as a 
educational work for the Game Engines subject on the Politecnical University of Catalunya, Spain.   
   
![Teamphoto](https://fotos.subefotos.com/6b4f779ac922cbc05ae5dbc6f18cfe52o.jpg)  

Our goal was to achieve a working 3D space with support of the basic elements of a game engine: game object hierarchy, components, rendering modes, time management and more.
We also created Wwished, a library that integrates the Wwise audio engine in a simple, quick manner.


<dl>
  <a href="https://github.com/carcasanchez/Asiergine/blob/Assignment2%E2%80%8B/LICENSE" class="btn">Asiergine is under the Mit license. Click here if you want more information.</a>
</dl>

Timelaspse: Create a Scene
<iframe width="560" height="315" src="https://youtu.be/gcuZhsAth6M" frameborder="0" gesture="media" allow="encrypted-media" allowfullscreen></iframe>

### Work log    
**Carlos Cabreira**   
 - Camera rendering: OpenGl 3D space drawing of texturized geometry   
   ![scene](https://i.gyazo.com/333d625f836b61bd5ce3ffe4a6ee2d77.gif)
 - Import capabilities: Asiergine can import geometry and textures from FBX files
 - Scene serialization: Asiergine saves data to own binary format, .carca
 - Component utility: different components can be added to game objects to define their behaviour in the game
 - Time managment: the game speed can be controlled in real time. Also, the user can pause the game in middle of execution.
 - Game Object hierarchy: all game objects follow a hierarchy that inherits transformations from parents to childs. Game objects can be 
   transformed using visual gizmos
 - Resource management: meshes, scenes and textures are structured in a Library system that avoids double loading and improves      performance
 - Wwise Audio Engine integration: via own Wwished library, Asiergine can take advantage of the powerful audio engine to implement music and FX in games.

**Asier Iglesias:**   
 - Engine UI: organization and coding of all visual elements and options in the editor window
 - Component and object edition: creation, deletion and modification of the different elements of the engine
 - Scene quadtree: static objects are included in a 3D Quadtree that accelerates processing.
 <a href="https://gyazo.com/bf1805fc276b0556ebbaff85b9ce819c"><img src="https://i.gyazo.com/bf1805fc276b0556ebbaff85b9ce819c.png" alt="https://gyazo.com/bf1805fc276b0556ebbaff85b9ce819c" width="774.6666666666666"/></a>
 - Camera culling system: optimization to avoid drawing 3D objects that are not in camera. Accelerated via Quadtree.
 - Asset explorer: see all resources available, from meshes to soundbanks.
 ![assets](https://i.gyazo.com/bc0eaf62e08f177ab7bdf26e57b6fec8.gif)

 

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

![music](https://i.gyazo.com/02da2c48a3fa3d02dbbe245e3eb958d3.gif)

- Reverb areas: add a Reverb Component to any object, and transform the box to fit the desired size. In the Component editor, add the name of the effect you want (previously defined on Wwise), and its value. Any object that is inside the box will note the marked effect.

![reverb](https://i.gyazo.com/9227baeea8f899e0cf366693cfc33692.gif)

Timelapse: Setting up the audio
<iframe width="560" height="315" src="https://www.youtube.com/embed/6UPPgBBpLww?ecver=1" frameborder="0" gesture="media" allow="encrypted-media" allowfullscreen></iframe>

<dl>
  <a href="https://github.com/carcasanchez/Asiergine/tree/master" class="btn">Check the Engine code
    here</a>
</dl>


<dl>
  <a href="https://github.com/carcasanchez/Asiergine/releases/download/Assignment3/Asiergine_v1.0.zip" class="btn">Download Latest Build</a>
</dl>


   


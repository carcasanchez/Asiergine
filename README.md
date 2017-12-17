# Asiergine v1.0

3D Engine created by Carlos Cabreira and Asier Iglesias in OpenGL and C++ with educational purposes.

Asiergine is under the MIT license. If you want to know more, please, see this link:
https://github.com/carcasanchez/Asiergine/blob/master/LICENSE

Code repository:
https://github.com/carcasanchez/Asiergine

Link to the web:
https://carcasanchez.github.io/Asiergine/

## Controls

-Left Click to select object
-Right Click to active first person camera movement
-WASD to displace the camera
-Mouse wheel to Zoom in and out
-Alt+Left Click to active orbital control
-Alt+Right Click to pan the camera
-F to focus on selected object
-R to reset camera to 0, 0, 0
- 1, 2 and 3 to change the Gizmos between Translation, Rotation and Scale mode


-F1 to show vertex normals
-F2 to show bounding boxes

The release comes with a scene that you can load from File>LoadScene. This scene features a demo of different aspects of our audio system.
Press play to run it, and press E to trigger a Shot sound.
 
## About Wwished library

 Wwished is a own library, created to integrate the Wwise audio engine in a quick, easy fashion. 
Wwished sacrifices depth and complexity in order to offer a simpler approach.
The only thing you need to do, in order to use it, is download the Wwise SDK, add "$(WWISESDK)\include" to your additional include directories,
and add "Wwished.h" to your project. 

Warning: by default, "Wwished.h" uses a bunch of libraries that are placed in the "3DEngine/AK/" route. Be sure that this libraries are placed properly 
in your project, or rename the routes in "Wwished.h".

## About audio in Asiergine

 In order to create audio in a Asiergine scene, you must first load a Wwise soundbank. The soundbank must be placed in the Assets/Sounbanks folder.
To load a Sounbank, go to Configuration->Audio, and type the name of the bank you want to load. In the case of this release, only the Main bank is available.

After loading a bank, you must add a Listener, usually in the Camera object. Select the camera, add an Audio Component to it, and select the type Listener.
You can only have one listener in scene.

 Now, the only thing you need to do is populate the scene with sound.
You have various options: 

-FX: just set the Audio Type in Component Audio to FX. You must now add Events to this component. 
Events must be defined in Wwise first. In the Component Editor, you can add or delete events, and specify when this events will trigger.
By default, only two possibilities are included (when pressing the E key, or when the game starts running).

-Music: just set the Audio Type in Component Audio to Music. As with FX, you must specify the Event that will trigger the music. Also, you 
have the possibility of add a State group to make transitions between two states. Just create the group and states in Wwise, and specify in the 
Component the name of the Group, the two states and the time between changes.
All music starts on play.

-Reverb areas: add a Reverb Component to any object, and transform the box to fit the desired size. In the Component editor, add the name 
of the effect you want (previously defined on Wwise), and its value. Any object that is inside the box will note the marked effect.


## Demo scene

 This release comes with a audio demo scene. The elements in this scene are:
- A Main audio bank loaded
- A Listener added to the camera object
- A music obj with transitions between two musics every 30 secs
- A Shot obj that triggers the Shot event when E is pressed (only when game is running)
- A Train that moves from left to right and plays when the game starts
- A Tunnel area where a special reverb effect is applied to the Train and the Shot

 Please, note that the point of reference for 3D audio is the camera object, which owns the listener. If you want to test spatial audio, 
move the audio objects around the camera frustum.


# Changelog

### v1.0
-Wwise is integrated in engine via own Wwished library. 
-Added various musics and FX
-Added Reverb areas

### v0.8
-Implemented resource manager: meshes and textures will only load once
-Reestructured file system: separation between Library and Assets 
-Transformation gizmos implemented
-Mouse picking implemented
-Asset window added to editor
-Quadtree heavily optimized and used with frustum culling
-Components and empty game object creation option added
-Numerous bugs fixed
-Heavy stabilization of meshes and textures
-Graphic redesign

### v0.7.7
-Camera Module migrated to MathGeoLib frustum. Removed gl::math
-Implemented scene serialization: save and load to own format .carca
-Added file system and library generation
-Added first attemp of Quadtree
-Added Play Game and more time options
-Numerous minor bugs solved  

### v0.7.5
-Big migration to component system
-Added fake camera and frustum culling
-Created importer from FBX to own format .carca
-Created methods to load components and objects from own format .carca
WARNING: drag and dropping fbx to the editor will create a library folder with .carca of the objects in the file.
All of these features are still experimental. Trying to load some models could make the program crash.

### v0.7
-Added Inspector Window to show all geometry transform, polycount and textures.
-Solved numerous bugs and crashes when load a FBX file
-Reorganized code to be more human readable
-Solved camera bug when its aligned to the Y axis
-Solved numerous little load and save bugs

### v0.6.1
-Polished camera control
-Textures could be loaded from image file 
-Solved memory leaks and minor bugs

### v0.6
-Texture load from FBX

### v0.5
-Polished camera control
-Geometry load from fbx added
-Geometry and vertex normals drawing added

### v0.4
-Added unpolished camera control
-Fixed some FPS measure bug
-Fixed Cap FPS bugs
-Added Rendering options
-Added Index and Array rendering

### v0.2
-Added Configuration window, with screen and framerate options 
-Added Hardware Specs window
-Added About Engine window
-Added Profiler window
-Integrated MMGR memory manager
-Integrated JSON manager. Now the engine saves data to a config.json file

### v0.1
-Added main menu bar
-Added Exit option
-Added Show Example window option
-Added Console
-Added Random Geometry Generator to Console
-Added Random float generator to Console
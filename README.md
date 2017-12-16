# Asiergine v1.0

3D Engine created by Carlos Cabreira and Asier Iglesias in OpenGL and C++ with educational purposes.

Asiergine is under the MIT license. If you want to know more, please, see this link:
https://github.com/carcasanchez/Asiergine/blob/master/LICENSE

Code repository:
https://github.com/carcasanchez/Asiergine

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


 
## About Wwished library

- Add audio banks in Configuration->Audio (in this case, Main)
- Create a Component Audio in camera and set to Listener type
- Create a Component Audio in any obj and set it to the desired type:

Valid FX name events are Train and Shot
  
Music: set event name to Play_Music
	set Group to music1
	set states to Overworld and Battle
	Set time to whatever
music will play when app starts running

-To add a reverb area, create a Reverb Area with name Corridor and any value. Scale it with Gizmos

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
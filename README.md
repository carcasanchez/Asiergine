# Asiergine v0.8

3D Engine created by Carlos Cabreira and Asier Iglesias in OpenGL and C++ with educational purposes.

Asiergine is under the MIT license. If you want to know more, please, see this link:
https://github.com/carcasanchez/Asiergine/blob/master/LICENSE

Code repository:
https://github.com/carcasanchez/Asiergine/tree/Second_Asignment

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

## About Quadtree
 
 The Quadtree only will generate if you have any static object in scene. If you make static an object (and viceversa), all its children
will become static.
 To see the Quadtree drawn in scene, the menu Bake Quadtree (in the Configuration Window) must be opened.
 In this menu, you can change the size of the quadtree and the maximum number of objects each node will admit before division.
 If you want to active frustum culling, go to Configuration Window -> Rendering -> Frustum Culling.
 
## About Assets and Library

 Asiergine works with four main assets folders: fbx, textures, meshes and scenes
 Also, meshes and textures will be imported to the Library to improve performance.
 We recomend you not to delete any of these folders. 

 Drag and drop any FBX file from anywhere in the computer to add their hierarchy, meshes and textures to the scene.
 If you drop a image file, the engine will import it.
 Any of these files will be cloned in its respective Assets folder. 

 The engine always reimports all meshes and textures in start, but if
 you delete something from Assets or Library in execution, try to drop the file again in the editor to reimport it.
 
 Also, don't change .meta.json files if you want the engine to work properly!

## Extra features (for innovation grade)
-Trasformation gizmos
-Scene serialized to own binary format (.carca)
-Textures and meshes can be changed in their respective component windows
-Light is now a component that can be added and deleted

# Changelog

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
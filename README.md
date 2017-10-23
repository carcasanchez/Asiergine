#Asiergine v0.7.5

3D Engine created by Carlos Cabreira and Asier Iglesias in OpenGL and C++ with educational purposes.

Asiergine is under the MIT license. If you want to know more, please, see this link:
https://github.com/carcasanchez/Asiergine/blob/master/LICENSE

Code repository:
https://github.com/carcasanchez/Asiergine

#Changelog

###v0.7.5
-Big migration to component system
-Added fake camera and frustum culling
-Created importer from FBX to own format .carca
-Created methods to load components and objects from own format .carca
WARNING: drag and dropping fbx to the editor will create a library folder with .carca of the objects in the file.
All of these features are still experimental. Trying to load some models could make the program crash.

###v0.7
-Added Inspector Window to show all geometry transform, polycount and textures.
-Solved numerous bugs and crashes when load a FBX file
-Reorganized code to be more human readable
-Solved camera bug when its aligned to the Y axis
-Solved numerous little load and save bugs

###v0.6.1
-Polished camera control
-Textures could be loaded from image file 
-Solved memory leaks and minor bugs

###v0.6
-Texture load from FBX

###v0.5
-Polished camera control
-Geometry load from fbx added
-Geometry and vertex normals drawing added

###v0.4
-Added unpolished camera control
-Fixed some FPS measure bug
-Fixed Cap FPS bugs
-Added Rendering options
-Added Index and Array rendering


###v0.2
-Added Configuration window, with screen and framerate options 
-Added Hardware Specs window
-Added About Engine window
-Added Profiler window
-Integrated MMGR memory manager
-Integrated JSON manager. Now the engine saves data to a config.json file

###v0.1
-Added main menu bar
-Added Exit option
-Added Show Example window option
-Added Console
-Added Random Geometry Generator to Console
-Added Random float generator to Console
#Asiergine v0.7

---LINK TO REPOSITORY

3D Game Engine made by Asier Iglesias and Carlos Cabreira, based on OpenGL.
This game engine is being created for educational purposes. In this latest version, it allows you to load
3D models from FBX files by dragging and dropping them into the editor window. Texture loading is also
supported (directly from FBX and manually by drag and drop). Asiergine only supports one texture and one scene at once, 
though multiple meshes could be loaded if the are in the same scene.

The Inspector Window allows you to see different information about the loaded models (transform, polycount).
You can change numerous options about rendering and the engine itself in the Configuration Window. 

All libraries under usage are documented and linked in the About the Engine window.

Feel free to report any error or asking us any question. 

Camera Controls-----
-WASD + Right Mouse: first person control
-Alt Left + Left Mouse: orbit around reference
-Mouse Wheel: Zoom
-Alf Left + Right Mouse: move the camera in the view plane
-F key: adapt the camera to the loaded geometry
-R key: reset the camera to the 0, 0, 0 position


Asiergine is under the MIT license. If you want to know more, please, see this link:
https://github.com/carcasanchez/Asiergine/blob/master/LICENSE

#Changelog

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
#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <fstream>
#include <map>
#include <utility>
#include <experimental\filesystem>
#include "Brofiler\Brofiler.h"
#include "MathGeoLib\include\Math.h"

// Deletes a buffer
#define RELEASE( x ) \
    {                \
    if( x != NULL )  \
	    {            \
      delete x;      \
	  x = NULL;      \
	    }            \
}

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI
#define M_PI_2 1.57079632679
#define M_PI 3.14159265359

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#define DEBUG_NORMALS_KEY SDL_SCANCODE_F1
#define DEBUG_BOXES_KEY SDL_SCANCODE_F2

#define SCENE_SAVETAG 00
#define OBJECT_SAVETAG 01
#define MESH_SAVETAG 02

#define FORMAT_EXTENSION ".carca"
#define TEXTURE_EXTENSION ".dds"

#define MIN_SCENE_POINT_X -20.f
#define MIN_SCENE_POINT_Y -20.f
#define MIN_SCENE_POINT_Z -20.f
#define MAX_SCENE_POINT_X 20.f
#define MAX_SCENE_POINT_Y 20.f
#define MAX_SCENE_POINT_Z 20.f

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

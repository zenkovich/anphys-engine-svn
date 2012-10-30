#ifndef ENGINE_INCL_H
#define ENGINE_INCL_H

#include "engine_options.h"

//engine
#include "engine.h"

//engine scene
#include "scene/scene.h"
#include "scene/scene_stuff.h"
#include "scene/object.h"
#include "scene/object_component.h"
#include "scene/graphics_object_component.h"
#include "scene/physics_rigid_body_object_component.h"
#include "scene/render3d_object_component.h"

//utils
#include "../util/utils.h"

#include "render_system/render_frame.h"

//render
#include "../render/render.h"
#include "../render/cameras_manager.h"
#include "../render/camera.h"
#include "../render/camera_3d_mouse.h"
#include "../render/light_manager.h"
#include "../render/light.h"
#include "../render/material_manager.h"
#include "../render/material.h"
#include "../render/texture_manager.h"
#include "../render/texture.h"
#include "../render/surface_material_manager.h"
#include "../render/surface_material.h"
#include "../render/mesh_part.h"
#include "../render/render_object.h"
#include "../render/object_manager.h"
#include "../render/render_3d_object.h"
#include "../render/render_3d_object_mesh.h"
#include "../render/render_scene_base_interface.h"
#include "../render/scene_manager.h"
#include "../render/multi_object.h"
#include "../render/multi_object_container.h"
#include "../render/multi_3d_object_container.h"

//spec render
#ifdef RENDER_D3D8

#include "../render/d3d8/render_d3d8.h"
#include "../render/d3d8/texture_d3d8.h"
#include "../render/d3d8/material_d3d8.h"
#include "../render/d3d8/light_d3d8.h"
#include "../render/d3d8/surface_material_d3d8.h"
#include "../render/d3d8/camera.h"
#include "../render/d3d8/camera_3d_mouse_control.h"
#include "../render/d3d8/mesh_data_manager_d3d8.h"
#include "../render/d3d8/mesh_part_d3d8.h"
#include "../render/d3d8/render_3d_object_mesh_d3d8.h"


#endif //RENDER_D3D8



//platforms
#ifdef PLATFORM_WIN32

#include "../win32_platform/application.h"
#include "../win32_platform/simple_window.h"
#include "../win32_platform/windows_manager.h"

//render frame
#include "render_system/render_frame_win32_window/render_frame_win32_window.h"

#endif //PLATFORM_WIN32


//log system
extern cLogSystem* gLogSystem;
//extern cLogStreamInConsoleAndFile* gLog;

//render objects
typedef grRenderMultiObject<grRender3DObjectMesh, gr3DObjectConditionContainer> grRenderMulti3DObject;

#endif //ENGINE_INCL_H
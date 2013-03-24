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
#include "scene/components/graphics_object_component.h"
#include "scene/components/physics_rigid_body_object_component.h"
#include "scene/components/physics_static_body_object_component.h"
#include "scene/components/render3d_object_component.h"

//utils
#include "util/utils.h"

#include "frame_system/render_frame.h"

//render
#include "render/render.h"
#include "render/2d_render/render_2d.h"
#include "render/camera/cameras_manager.h"
#include "render/camera/camera.h"
#include "render/camera/camera_3d_mouse.h"
#include "render/camera/camera_2d.h"
#include "render/light_manager/light_manager.h"
#include "render/light_manager/light.h"
#include "render/material_manager/material_manager.h"
#include "render/material_manager/material.h"
#include "render/texture_manager/texture_manager.h"
#include "render/texture_manager/texture.h"
#include "render/surface/surface_material_manager.h"
#include "render/surface/surface_material.h"
#include "render/render_objects/3d/mesh/mesh_part.h"
#include "render/render_objects/render_object.h"
#include "render/render_objects/object_manager.h"
#include "render/render_objects/3d/render_3d_object.h"
#include "render/render_objects/3d/mesh/render_3d_object_mesh.h"
#include "render/render_objects/2d/render_2d_object_mesh.h"
#include "render/scenes/simple_scene/render_scene_base_interface.h"
#include "render/scenes/scene_manager.h"
#include "render/render_objects/multi_object/multi_object.h"
#include "render/render_objects/multi_object/multi_object_container.h"
#include "render/render_objects/multi_object/multi_3d_object_container.h"

//spec render
#ifdef RENDER_D3D8

#include "spec_render/render_d3d8/render_d3d8.h"
#include "spec_render/render_d3d8/texture/texture_d3d8.h"
#include "spec_render/render_d3d8/material/material_d3d8.h"
#include "spec_render/render_d3d8/light/light_d3d8.h"
#include "spec_render/render_d3d8/surface/surface_material_d3d8.h"
#include "spec_render/render_d3d8/camera/camera.h"
#include "spec_render/render_d3d8/objects/mesh/mesh_data_manager_d3d8.h"
#include "spec_render/render_d3d8/objects/mesh/mesh_part_d3d8.h"
#include "spec_render/render_d3d8/objects/mesh/render_3d_object_mesh_d3d8.h"


#endif //RENDER_D3D8



//platforms
#ifdef PLATFORM_WIN32

#include "engine/frame_system/win32_framesystem/application.h"
#include "engine/frame_system/win32_framesystem/simple_window.h"
#include "engine/frame_system/win32_framesystem/windows_manager.h"

//render frame
#include "engine/frame_system/win32_render_frame/render_frame_win32_window.h"

#endif //PLATFORM_WIN32


//log system
extern cLogSystem* gLogSystem;

//physics
#include "physics/physics_engine.h"
#include "physics/objects/physics_object.h"
#include "physics/scenes/physics_scene.h"
#include "physics/objects/rigid_object.h"
#include "physics/objects/static_object.h"
#include "physics/stuff/inertia.h"

//render objects
typedef grRenderMultiObject<grRender3DObjectMesh, gr3DObjectConditionContainer> grRenderMulti3DObject;

#endif //ENGINE_INCL_H
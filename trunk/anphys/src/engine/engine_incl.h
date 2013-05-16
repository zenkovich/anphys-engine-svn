#ifndef ENGINE_INCL_H
#define ENGINE_INCL_H

#include "engine_options.h"

//engine
#include "engine.h"

//engine scene
#include "scene/components/graphics_object_component.h"
#include "scene/components/physics_rigid_body_object_component.h"
#include "scene/components/physics_static_body_object_component.h"
#include "scene/components/render3d_object_component.h"
#include "scene/object.h"
#include "scene/object_component.h"
#include "scene/scene.h"
#include "scene/scene_stuff.h"

//utils
#include "util/utils.h"

#include "frame_system/render_frame.h"

//render
#include "render/camera/camera_2d.h"
#include "render/camera/camera_3d.h"
#include "render/camera/camera_3d_mouse.h"
#include "render/light_manager/light.h"
#include "render/light_manager/light_manager.h"
#include "render/material_manager/material.h"
#include "render/material_manager/material_manager.h"
#include "render/render.h"
#include "render/render_objects/2d/render_2d_object_mesh.h"
#include "render/render_objects/2d/sprite.h"
#include "render/render_objects/3d/mesh/mesh_part.h"
#include "render/render_objects/3d/mesh/render_3d_object_mesh.h"
#include "render/render_objects/3d/render_3d_object.h"
#include "render/render_objects/multi_object/multi_3d_object_container.h"
#include "render/render_objects/multi_object/multi_object.h"
#include "render/render_objects/multi_object/multi_object_container.h"
#include "render/render_objects/object_manager.h"
#include "render/render_objects/render_object.h"
#include "render/render_state/2d_render_state.h"
#include "render/render_state/render_state_interface.h"
#include "render/render_state/simple_3d_render_state.h"
#include "render/render_target/backbuffer_render_target.h"
#include "render/render_target/render_target_interface.h"
#include "render/render_target/stencil_buffer_render_target.h"
#include "render/render_target/texture_render_target.h"
#include "render/scenes/scene_manager.h"
#include "render/scenes/simple_scene/render_scene_base_interface.h"
#include "render/surface/surface_material.h"
#include "render/surface/surface_material_manager.h"
#include "render/texture_manager/texture.h"
#include "render/texture_manager/texture_manager.h"

//spec render
#ifdef RENDER_D3D8

#include "spec_render/render_d3d8/light/light_d3d8.h"
#include "spec_render/render_d3d8/material/material_d3d8.h"
#include "spec_render/render_d3d8/objects/mesh/mesh_data_manager_d3d8.h"
#include "spec_render/render_d3d8/objects/mesh/mesh_part_d3d8.h"
#include "spec_render/render_d3d8/objects/mesh/render_3d_object_mesh_d3d8.h"
#include "spec_render/render_d3d8/render_d3d8.h"
#include "spec_render/render_d3d8/surface/surface_material_d3d8.h"
#include "spec_render/render_d3d8/texture/texture_d3d8.h"


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
#include "physics/CD/collision_detection/box_box_cd.h"
#include "physics/CD/collision_geometry/box_collision_geometry.h"
#include "physics/CD/collision_geometry/landscape_geometry.h"
#include "physics/CD/collision_geometry/collision.h"
#include "physics/CD/collision_geometry/collision_geometry.h"
#include "physics/CD/collision_geometry/collision_point.h"
#include "physics/objects/physics_object.h"
#include "physics/objects/rigid_object.h"
#include "physics/objects/static_object.h"
#include "physics/physics_engine.h"
#include "physics/scenes/physics_scene.h"
#include "physics/stuff/inertia.h"

//UI
#include "ui/ui_border.h"
#include "ui/ui_checkbox.h"
#include "ui/ui_font.h"
#include "ui/ui_label.h"
#include "ui/ui_layout_widgets.h"
#include "ui/ui_lines_geometry.h"
#include "ui/ui_manager.h"
#include "ui/ui_scroll_area.h"
#include "ui/ui_scrollbar.h"
#include "ui/ui_simple_stuff.h"
#include "ui/ui_sprite.h"
#include "ui/ui_state.h"
#include "ui/ui_text_edit.h"
#include "ui/ui_widget.h"
#include "ui/ui_window.h"

//input
#include "input/input_messenger.h"

//render objects
typedef grRenderMultiObject<grRender3DObjectMesh, gr3DObjectConditionContainer> grRenderMulti3DObject;

#endif //ENGINE_INCL_H
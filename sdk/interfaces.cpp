#include "interfaces.h"
#include "utils/memory_utils.h"


void interfaces::init_interfaces()
{
	engine = memory_utils::capture_interface<c_engine_client>("engine.dll", "VEngineClient015");
	client = memory_utils::capture_interface<c_client>("client.dll", "VClient017");
	entity_list = memory_utils::capture_interface<v_client_entity_list>("client.dll", "VClientEntityList003");
	surface = memory_utils::capture_interface<i_surface>("vguimatsurface.dll", "VGUI_Surface030");
	input_system = memory_utils::capture_interface<i_input_system>("inputsystem.dll", "InputSystemVersion001");
	lua_shared = memory_utils::capture_interface<c_lua_shared>("lua_shared.dll", "LUASHARED003");
	material_system = memory_utils::capture_interface<i_material_system>("materialsystem.dll", "VMaterialSystem080");
	model_render = memory_utils::capture_interface<iv_model_render>("engine.dll", "VEngineModel016");
	render_view = memory_utils::capture_interface<iv_render_view>("engine.dll", "VEngineRenderView014");
	prediction = memory_utils::capture_interface<i_prediction>("client.dll", "VClientPrediction001");
	engine_trace = memory_utils::capture_interface<i_engine_trace>("engine.dll", "EngineTraceClient003");
	panel = memory_utils::capture_interface<i_panel>("vgui2.dll", "VGUI_Panel009");
	game_movement = memory_utils::capture_interface<i_game_movement>("client.dll", "GameMovement001");
}
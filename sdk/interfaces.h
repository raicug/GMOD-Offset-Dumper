#pragma once
#include <memory>

#include "utils/md5_check_sum.h"

#include "interfaces/client.h"
#include "interfaces/engine.h"
#include "interfaces/surface.h"
#include "interfaces/i_input_system.h"
#include "interfaces/i_client_mode.h"
#include "interfaces/v_client_entity_list.h"
#include "interfaces/lua_shared.h"
#include "interfaces/i_material_system.h"
#include "interfaces/iv_model_render.h"
#include "interfaces/i_render_view.h"
#include "interfaces/c_global_vars.h"
#include "interfaces/i_view_render.h"
#include "interfaces/random.h"
#include "interfaces/i_prediction.h"
#include "interfaces/i_engine_trace.h"
#include "interfaces/i_panel.h"
#include "interfaces/i_game_movement.h"
#include "interfaces/c_input.h"

namespace interfaces
{
	inline c_engine_client* engine;
	inline c_client* client;
	inline i_surface* surface;
	inline i_input_system* input_system;
	inline i_client_mode* client_mode;
	inline v_client_entity_list* entity_list;
	inline c_lua_shared* lua_shared;
	inline i_material_system* material_system;
	inline i_mat_render_context* render_context;
	inline iv_model_render* model_render;
	inline iv_render_view* render_view;
	inline c_global_vars* global_vars;
	inline i_view_render* view_render;
	inline c_uniform_random_stream* random;
	inline i_prediction* prediction;
	inline i_engine_trace* engine_trace;
	inline i_panel* panel;
	inline i_game_movement* game_movement;
	inline i_move_helper* move_helper;
	inline c_input* input;
	
	void init_interfaces();
}
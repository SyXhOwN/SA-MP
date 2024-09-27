
#pragma once

#define MAX_SCRIPT_VARS	16	// Size of our variable saving array

// Super Simplified SA Version
struct GAME_SCRIPT_THREAD	// 0xE0 bytes total.
{
	//BYTE _gap0[224];
	BYTE Pad1[0x14];		// 0x00 [0x14]
	DWORD dwScriptIP;		// 0x14 [0x04]
	BYTE Pad2[0x24];		// 0x18 [0x24]
	DWORD dwLocalVar[18];	// 0x3C [0x48]
	BYTE Pad3[0x41];		// 0x84 [0x41]
	DWORD dwIfFlag;			// 0xC5 [0x04]
	BYTE Pad4[0xF];			// 0xC9 [0x0F]
	DWORD dwIndexSomething;	// 0xD8 [0x04]	(indexes into 0xA49960)
	BYTE bHasLocalCopy;		// 0xDC [0x01]	(has local copy of something, if 1, then uses .data:00A48960 CTheScripts_LocalCopy instead of var0)
	BYTE Pad5[0x3];			// 0xDD [0x03]
};

struct SCRIPT_COMMAND		//	Params
{							//		i = integer
	WORD OpCode;			//		f = float
	char Params[MAX_SCRIPT_VARS]; //	v = variable
};							//		s = string

void InitScripting();
int ScriptCommand(const SCRIPT_COMMAND* pScriptCommand, ...);	// The main scripting function. See notes.

const SCRIPT_COMMAND create_arrow_above_actor			= { 0x0187, "iv" };

const SCRIPT_COMMAND request_model						= { 0x0247, "i" };		// (CAR_*|BIKE_*|BOAT_*|WEAPON_*|OBJECT_*)
const SCRIPT_COMMAND load_requested_models				= { 0x038B, "" };		// -/-
const SCRIPT_COMMAND create_car							= { 0x00A5, "ifffv" };	// (CAR_*|BIKE_*|BOAT_*), x, y, z, var_car

const SCRIPT_COMMAND create_actor						= { 0x009A, "iifffv" };	// PEDTYPE_*, #MODEL, x, y, z, var_actor

const SCRIPT_COMMAND fade								= { 0x016A, "ii" };		// (time in ms), FADE_*

const SCRIPT_COMMAND set_current_time					= { 0x00C0, "ii" };		// Hours, Minutes

const SCRIPT_COMMAND create_player						= { 0x0053, "vfffv" };	// 0, x, y, z, PLAYER_CHAR
const SCRIPT_COMMAND create_actor_from_player			= { 0x01F5, "vv" };		// PLAYER_CHAR, PLAYER_ACTOR

const SCRIPT_COMMAND select_interior					= { 0x04BB, "i" };		// INTERIOR_*

const SCRIPT_COMMAND destroy_car						= { 0x00A6, "i" };		// var_car
const SCRIPT_COMMAND set_car_z_angle					= { 0x0175, "if" };		// var_car, angle

const SCRIPT_COMMAND is_model_available					= { 0x0248, "i" };		// #MODEL

const SCRIPT_COMMAND toggle_widescreen					= { 0x02A3, "i" };		// widescreen(1/0)
const SCRIPT_COMMAND set_camera_behind_player			= { 0x0373, "" };		// -/-

const SCRIPT_COMMAND camera_on_vehicle					= { 0x0158, "iii" };	// var_car, ukn, ukn
const SCRIPT_COMMAND camera_on_actor					= { 0x0159, "iii" };	// var_actor, ukn, ukn

const SCRIPT_COMMAND point_camera						= { 0x0160, "fffi" };	// x, y, z, type
const SCRIPT_COMMAND restore_camera_jumpcut				= { 0x02EB, "" };		// -/-
const SCRIPT_COMMAND set_camera_position				= { 0x015F, "ffffff" }; // x, y, z, vx, vy, vz

const SCRIPT_COMMAND tie_marker_to_car					= { 0x0161, "iiiv" };	// var_car, ukn, ukn, var_marker

const SCRIPT_COMMAND disable_marker						= { 0x0164, "i" };		// var_marker
const SCRIPT_COMMAND set_marker_color					= { 0x0165, "ii" };		// var_marker, color

const SCRIPT_COMMAND create_radar_marker_without_sphere	= { 0x04CE, "fffiv" };	// x, y, z, marker_type, var_marker
const SCRIPT_COMMAND show_on_radar						= { 0x0168, "ii" };		// var_marker, size

const SCRIPT_COMMAND create_icon_marker_sphere			= { 0x02A7, "fffiv" };

const SCRIPT_COMMAND is_actor_near_point_3d				= { 0x00FE, "iffffffi" };//	var_actor, x, y, z, rx, ry, rz, b

const SCRIPT_COMMAND get_line_of_sight					= { 0x06BD, "ffffffiiiii" }; // x1, y1, z1, x2, y2, z2, solid, vehicle, actor, obj, fx

const SCRIPT_COMMAND add_to_player_money				= { 0x0109, "ii" };		// PLAYER_CHAR, money

const SCRIPT_COMMAND has_car_sunk						= { 0x02bf, "i" };	// CarID
const SCRIPT_COMMAND is_car_wrecked						= { 0x0119, "i" }; // CarID
const SCRIPT_COMMAND get_ground_z						= { 0x02ce, "fffv" };	// x, y, z, var_ground_z

const SCRIPT_COMMAND create_racing_checkpoint			= { 0x06d5,	"ifffffffv"};	// type, x, y, z, nx, ny. nz, size, $id (n=next checkpoint)
/* 0       = Street Checkpoint with arrow to the next checkpoint
   1 and 2 = Street Checkpoint
   3 and 4 = Flight Checkpoint (circle) */
const SCRIPT_COMMAND destroy_racing_checkpoint			= { 0x06d6,	"i" };

const SCRIPT_COMMAND toggle_player_controllable 		= { 0x01B4, "ii" };

const SCRIPT_COMMAND set_actor_armed_weapon				= { 0x01b9, "ii" };
const SCRIPT_COMMAND get_actor_armed_weapon				= { 0x0470, "iv" };

const SCRIPT_COMMAND put_actor_in_car					= { 0x036A, "ii" };

const SCRIPT_COMMAND set_actor_immunities				= { 0x02ab, "iiiiii" };
const SCRIPT_COMMAND set_car_immunities					= { 0x02ac, "iiiiii" };
const SCRIPT_COMMAND set_actor_can_be_decapitated		= { 0x0446, "ii" };
const SCRIPT_COMMAND create_object						= { 0x0107, "ifffv" };

const SCRIPT_COMMAND set_object_rotation				= { 0x0453, "ifff" };

const SCRIPT_COMMAND play_sound							= { 0x018c, "fffi" };

const SCRIPT_COMMAND remove_actor_from_car_and_put_at	= { 0x0362, "ifff" };
const SCRIPT_COMMAND send_actor_to_car_driverseat		= { 0x05CB, "iii" };
const SCRIPT_COMMAND send_actor_to_car_passenger		= { 0x05CA, "iiii" };
const SCRIPT_COMMAND make_actor_leave_car				= { 0x05CD, "ii" };

const SCRIPT_COMMAND get_car_z_angle					= { 0x0174, "iv" };

const SCRIPT_COMMAND create_train					    = { 0x06D8, "ifffiv" };
const SCRIPT_COMMAND destroy_train						= { 0x07bd, "i" };

const SCRIPT_COMMAND enable_train_traffic				= { 0x06d7, "i" };

const SCRIPT_COMMAND lock_car							= { 0x0519, "ii" };
const SCRIPT_COMMAND put_train_at						= { 0x07c7, "ifff" };

const SCRIPT_COMMAND release_model						= { 0x0249,	"i" };
const SCRIPT_COMMAND get_active_interior				= { 0x077e, "v" };
const SCRIPT_COMMAND get_actor_z_angle					= { 0x0172, "iv" };
const SCRIPT_COMMAND set_actor_z_angle					= { 0x0173, "if" };

const SCRIPT_COMMAND enter_passenger_driveby   			= { 0x0713, "iiiffffiii" };
const SCRIPT_COMMAND lock_actor							= { 0x04d7, "ii" };
const SCRIPT_COMMAND refresh_streaming_at				= { 0x04E4, "ff" };
const SCRIPT_COMMAND put_actor_in_car2					= { 0x0430, "iii" };

const SCRIPT_COMMAND kill_actor							= { 0x0321, "i" };

const SCRIPT_COMMAND set_actor_animation_set  		 	= { 0x0245, "is" };
const SCRIPT_COMMAND request_animation					= { 0x04ED, "s" };
const SCRIPT_COMMAND is_animation_loaded				= { 0x04EE, "s" };
const SCRIPT_COMMAND release_animation					= { 0x04EF, "s" };
const SCRIPT_COMMAND apply_animation					= { 0x0812, "issfiiiii" }; // actor,animation,library,floatunk,bool,bool,bool,bool,int
const SCRIPT_COMMAND is_actor_performing_anim   		= { 0x0611, "is" };
const SCRIPT_COMMAND set_actor_weapon_droppable 		= { 0x087e, "ii" };
const SCRIPT_COMMAND set_actor_money					= { 0x03fe, "ii" };

const SCRIPT_COMMAND text_clear_all						= { 0x00be, "" };

const SCRIPT_COMMAND set_car_hydraulics					= { 0x07FF,	"ii" }; //iCar, i (0=off/1=on)

const SCRIPT_COMMAND attach_object_to_actor				= { 0x069b, "iiffffff" };

const SCRIPT_COMMAND set_object_scaling					= { 0x08d2, "if" };

const SCRIPT_COMMAND create_pickup						= { 0x0213, "iifffv" };
const SCRIPT_COMMAND create_pickup_with_ammo			= { 0x032b, "iiifffv" };
const SCRIPT_COMMAND is_pickup_picked_up				= { 0x0214, "i" };

const SCRIPT_COMMAND destroy_pickup						= { 0x0215, "i" };

const SCRIPT_COMMAND change_car_skin					= { 0x06ED,	"ii" };
const SCRIPT_COMMAND add_car_component					= { 0x06E7, "iiv" };	// CAR, COMPONENT, COMPONENT VAR NAME
const SCRIPT_COMMAND is_component_available				= { 0x06EA, "i" };
const SCRIPT_COMMAND request_car_component				= { 0x06E9, "i" };
const SCRIPT_COMMAND remove_component					= { 0x06E8, "ii" };

const SCRIPT_COMMAND show_on_radar2						= { 0x018b, "ii" };

const SCRIPT_COMMAND find_ground_z						= { 0x02CE, "fffv" };

const SCRIPT_COMMAND toggle_actor_cellphone				= { 0x0729, "ii" };

const SCRIPT_COMMAND actor_task_handsup					= { 0x5c4, "ii" };

const SCRIPT_COMMAND toggle_car_tires_vulnerable		= { 0x053f, "ii" };
const SCRIPT_COMMAND set_fighting_style					= { 0x07fe, "iii" };

const SCRIPT_COMMAND link_vehicle_to_interior			= { 0x0840, "ii" };
const SCRIPT_COMMAND destroy_object						= { 0x0108, "i" };
const SCRIPT_COMMAND create_radar_marker_icon			= { 0x0570, "fffiv" };
const SCRIPT_COMMAND put_trailer_on_cab					= { 0x893, "ii" };
const SCRIPT_COMMAND detach_trailer_from_cab			= { 0x7AC, "ii" };

const SCRIPT_COMMAND create_explosion_with_radius		= { 0x0948, "fffii" };

const SCRIPT_COMMAND enable_zone_names					= { 0x09BA, "i" };

const SCRIPT_COMMAND remove_panel						= { 0x08DA, "i" };
const SCRIPT_COMMAND create_panel						= { 0x08D4, "sfffiiiiv" };
const SCRIPT_COMMAND set_panel_column_data				= { 0x08DB, "iisssssssssssss" };
const SCRIPT_COMMAND set_panel_column_width				= { 0x09DB, "iii" };
const SCRIPT_COMMAND set_panel_row_enable				= { 0x08D9, "iii" };

const SCRIPT_COMMAND get_panel_active_row				= { 0x08D7, "iv" };

const SCRIPT_COMMAND destroy_particle					= { 0x650, "i" };
const SCRIPT_COMMAND attach_particle_to_actor2			= { 0x066a, "siffffffiv" };
const SCRIPT_COMMAND make_particle_visible				= { 0x64c, "i" };

const SCRIPT_COMMAND set_actor_dicision					= { 0x60B, "ii" };

const SCRIPT_COMMAND set_player_drunk_visuals			= { 0x052c, "ii" }; // player, severity (0-255)
const SCRIPT_COMMAND handling_responsiveness			= { 0x03fd, "ii" }; // player, severity (0-255)

// ---

const SCRIPT_COMMAND restore_camera_to_user_defined		= { 0x0925, "" };
const SCRIPT_COMMAND set_camera_position_to				= { 0x0936, "ffffffii" };
const SCRIPT_COMMAND point_camera_transverse_to			= { 0x0920, "ffffffii" };
const SCRIPT_COMMAND lock_camera_target_point			= { 0x092F, "i" };
const SCRIPT_COMMAND lock_camera_position				= { 0x0930, "i" };

const SCRIPT_COMMAND load_wav							= { 0x03CF, "ii" };
const SCRIPT_COMMAND is_wav_loaded						= { 0x03D0, "i" };
const SCRIPT_COMMAND play_wav							= { 0x03D1, "i" };
const SCRIPT_COMMAND unload_wav							= { 0x040D, "i" };
const SCRIPT_COMMAND set_wav_at_location				= { 0x03D7, "ifff" };

const SCRIPT_COMMAND create_marker_at					= { 0x02A8, "fffiv" };

const SCRIPT_COMMAND attach_object_to_car				= { 0x0681, "iiffffff" };
const SCRIPT_COMMAND is_object_attached					= { 0x0685, "i" };
const SCRIPT_COMMAND attach_object_to_object			= { 0x069A, "iiffffff" };

const SCRIPT_COMMAND task_pick_up_object			= { 0x070A, "iifffiissi" };


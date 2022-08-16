#include "ModelsService.h"

#include "Settings.h"
#include "CModelInfo.h"
#include "Logger.h"
#include "fmt/core.h"

namespace SQLite
{
	/// definition of the assertion handler enabled when SQLITECPP_ENABLE_ASSERT_HANDLER is defined in the project (CMakeList.txt)
	void assertion_failed(const char* apFile, const long apLine, const char* apFunc, const char* apExpr, const char* apMsg)
	{
		// Print a message to the standard error output stream, and abort the program.
		std::stringstream error;
		error << apFile << ":" << apLine << ":" << " error: assertion failed (" << apExpr << ") in " << apFunc << "() with message \"" << apMsg << "\"\n";
		Logger::getInstance().log(error.str());
		std::abort();
	}
}

ModelsService::ModelsService():
		db_("LDYOM/models.sqlite3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
	SQLite::Statement query(this->db_, "PRAGMA foreign_keys = ON;");
	query.exec();
}

void ModelsService::Init() {
	pedModels_ = { 0, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288 };
	auto pedModels = Settings::getInstance().get<std::vector<int>>("data.pedModels");
	if (pedModels.has_value())
		pedModels_.insert(pedModels_.end(), pedModels->begin(), pedModels->end());

	weaponIds_ = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46 };
	auto weaponIds = Settings::getInstance().get<std::vector<int>>("data.weaponModels");
	if (weaponIds.has_value())
		weaponIds_.insert(weaponIds_.end(), weaponIds->begin(), weaponIds->end());


	specialsPed_ = { "andre", "bbthin", "bb", "cat", "cesar", "claude", "dwayne", "emmet", "forelli", "janitor", "jethro", "jizzy", "hern", "kendl", "maccer", "maddogg", "ogloc", "paul", "pulaski", "rose", "ryder", "ryder3", "sindaco", "smoke", "smokev", "suzie", "sweet", "tbone", "tenpen", "torino", "truth", "wuzimu", "zero", "gangrl2", "copgrl1", "copgrl2", "crogrl1", "crogrl2", "gungrl1", "gungrl2", "mecgrl2", "nurgrl2", "ryder2", "cdeput", "sfpdm1", "lvpdm1", "csbmydj", "psycho", "csmech", "csomost", "wmycd2" };
	auto specialsPed = Settings::getInstance().get<std::vector<std::string>>("data.specialPedModels");
	if (specialsPed.has_value())
		specialsPed_.insert(specialsPed_.end(), specialsPed->begin(), specialsPed->end());

	vehicleModels_ = { 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611 };
	auto vehicleModels = Settings::getInstance().get<std::vector<int>>("data.vehicleModels");
	if (vehicleModels.has_value())
		vehicleModels_.insert(vehicleModels_.end(), vehicleModels->begin(), vehicleModels->end());

	const char* packNames[] = {"PED","BOMBER","POOL","ON_LOOKERS","GANGS","PAULNMAC","BOX","Attractors","BAR","BASEBALL","BD_FIRE","BEACH","benchpress","BLOWJOBZ","CAMERA","CAR","CAR_CHAT","CASINO","COP_AMBIENT","CRACK","CRIB","DAM_JUMP","DANCING","DEALER","DODGE","FOOD","Freeweights","GFUNK","HEIST9","INT_HOUSE","INT_OFFICE","INT_SHOP","KISSING","LOWRIDER","MEDIC","MISC","OTB","PARK","PLAYIDLES","POLICE","RAPPING","RIOT","ROB_BANK","RUNNINGMAN","SCRATCHING","SEX","SHOP","SMOKING","SnM","STRIP","SUNBATHE","SWAT","VENDING","WOP"};
	const std::vector<std::vector<std::string>> animationNames = {{"abseil","ARRESTgun","ATM","BIKE_elbowL","BIKE_elbowR","BIKE_fallR","BIKE_fall_off","BIKE_pickupL","BIKE_pickupR","BIKE_pullupL","BIKE_pullupR","bomber","CAR_alignHI_LHS","CAR_alignHI_RHS","CAR_align_LHS","CAR_align_RHS","CAR_closedoorL_LHS","CAR_closedoorL_RHS","CAR_closedoor_LHS","CAR_closedoor_RHS","CAR_close_LHS","CAR_close_RHS","CAR_crawloutRHS","CAR_dead_LHS","CAR_dead_RHS","CAR_doorlocked_LHS","CAR_doorlocked_RHS","CAR_fallout_LHS","CAR_fallout_RHS","CAR_getinL_LHS","CAR_getinL_RHS","CAR_getin_LHS","CAR_getin_RHS","CAR_getoutL_LHS","CAR_getoutL_RHS","CAR_getout_LHS","CAR_getout_RHS","car_hookertalk","CAR_jackedLHS","CAR_jackedRHS","CAR_jumpin_LHS","CAR_LB","CAR_LB_pro","CAR_LB_weak","CAR_LjackedLHS","CAR_LjackedRHS","CAR_Lshuffle_RHS","CAR_Lsit","CAR_open_LHS","CAR_open_RHS","CAR_pulloutL_LHS","CAR_pulloutL_RHS","CAR_pullout_LHS","CAR_pullout_RHS","CAR_Qjacked","CAR_rolldoor","CAR_rolldoorLO","CAR_rollout_LHS","CAR_rollout_RHS","CAR_shuffle_RHS","CAR_sit","CAR_sitp","CAR_sitpLO","CAR_sit_pro","CAR_sit_weak","CAR_tune_radio","CLIMB_idle","CLIMB_jump","CLIMB_jump2fall","CLIMB_jump_B","CLIMB_Pull","CLIMB_Stand","CLIMB_Stand_finish","cower","Crouch_Roll_L","Crouch_Roll_R","DAM_armL_frmBK","DAM_armL_frmFT","DAM_armL_frmLT","DAM_armR_frmBK","DAM_armR_frmFT","DAM_armR_frmRT","DAM_LegL_frmBK","DAM_LegL_frmFT","DAM_LegL_frmLT","DAM_LegR_frmBK","DAM_LegR_frmFT","DAM_LegR_frmRT","DAM_stomach_frmBK","DAM_stomach_frmFT","DAM_stomach_frmLT","DAM_stomach_frmRT","DOOR_LHinge_O","DOOR_RHinge_O","DrivebyL_L","DrivebyL_R","Driveby_L","Driveby_R","DRIVE_BOAT","DRIVE_BOAT_back","DRIVE_BOAT_L","DRIVE_BOAT_R","Drive_L","Drive_LO_l","Drive_LO_R","Drive_L_pro","Drive_L_pro_slow","Drive_L_slow","Drive_L_weak","Drive_L_weak_slow","Drive_R","Drive_R_pro","Drive_R_pro_slow","Drive_R_slow","Drive_R_weak","Drive_R_weak_slow","Drive_truck","DRIVE_truck_back","DRIVE_truck_L","DRIVE_truck_R","Drown","DUCK_cower","endchat_01","endchat_02","endchat_03","EV_dive","EV_step","facanger","facgum","facsurp","facsurpm","factalk","facurios","FALL_back","FALL_collapse","FALL_fall","FALL_front","FALL_glide","FALL_land","FALL_skyDive","Fight2Idle","FightA_1","FightA_2","FightA_3","FightA_block","FightA_G","FightA_M","FIGHTIDLE","FightShB","FightShF","FightSh_BWD","FightSh_FWD","FightSh_Left","FightSh_Right","flee_lkaround_01","FLOOR_hit","FLOOR_hit_f","fucku","gang_gunstand","gas_cwr","getup","getup_front","gum_eat","GunCrouchBwd","GunCrouchFwd","GunMove_BWD","GunMove_FWD","GunMove_L","GunMove_R","Gun_2_IDLE","GUN_BUTT","GUN_BUTT_crouch","Gun_stand","handscower","handsup","HitA_1","HitA_2","HitA_3","HIT_back","HIT_behind","HIT_front","HIT_GUN_BUTT","HIT_L","HIT_R","HIT_walk","HIT_wall","Idlestance_fat","idlestance_old","IDLE_armed","IDLE_chat","IDLE_csaw","Idle_Gang1","IDLE_HBHB","IDLE_ROCKET","IDLE_stance","IDLE_taxi","IDLE_tired","Jetpack_Idle","JOG_femaleA","JOG_maleA","JUMP_glide","JUMP_land","JUMP_launch","JUMP_launch_R","KART_drive","KART_L","KART_LB","KART_R","KD_left","KD_right","KO_shot_face","KO_shot_front","KO_shot_stom","KO_skid_back","KO_skid_front","KO_spin_L","KO_spin_R","pass_Smoke_in_car","phone_in","phone_out","phone_talk","Player_Sneak","Player_Sneak_walkstart","roadcross","roadcross_female","roadcross_gang","roadcross_old","run_1armed","run_armed","run_civi","run_csaw","run_fat","run_fatold","run_gang1","run_left","run_old","run_player","run_right","run_rocket","Run_stop","Run_stopR","Run_Wuzi","SEAT_down","SEAT_idle","SEAT_up","SHOT_leftP","SHOT_partial","SHOT_partial_B","SHOT_rightP","Shove_Partial","Smoke_in_car","sprint_civi","sprint_panic","Sprint_Wuzi","swat_run","Swim_Tread","Tap_hand","Tap_handP","turn_180","Turn_L","Turn_R","WALK_armed","WALK_civi","WALK_csaw","Walk_DoorPartial","WALK_drunk","WALK_fat","WALK_fatold","WALK_gang1","WALK_gang2","WALK_old","WALK_player","WALK_rocket","WALK_shuffle","WALK_start","WALK_start_armed","WALK_start_csaw","WALK_start_rocket","Walk_Wuzi","WEAPON_crouch","woman_idlestance","woman_run","WOMAN_runbusy","WOMAN_runfatold","woman_runpanic","WOMAN_runsexy","WOMAN_walkbusy","WOMAN_walkfatold","WOMAN_walknorm","WOMAN_walkold","WOMAN_walkpro","WOMAN_walksexy","WOMAN_walkshop"},
	{"BOM_PLANT_IN","BOM_PLANT_LOOP","BOM_PLANT_CROUCH_IN","BOM_PLANT_CROUCH_OUT","BOM_PLANT_2IDLE"},
	{"POOL_XLONG_SHOT","POOL_XLONG_START","POOL_LONG_SHOT","POOL_LONG_START","POOL_MED_START","POOL_MED_SHOT","POOL_SHORT_SHOT","POOL_CHALKCUE"},
	{"LKUP_LOOP","POINTUP_IN","SHOUT_01"},
	{"DEALER_DEAL","DEALER_IDLE","drnkbr_prtl","drnkbr_prtl_F","DRUGS_BUY","hndshkaa","hndshkba","hndshkca","hndshkcb","hndshkda","hndshkea","hndshkfa","hndshkfa_swt","Invite_No","Invite_Yes","leanIDLE","leanIN","leanOUT","prtial_gngtlkA","prtial_gngtlkB","prtial_gngtlkC","prtial_gngtlkD","prtial_gngtlkE","prtial_gngtlkF","prtial_gngtlkG","prtial_gngtlkH","prtial_hndshk_01","prtial_hndshk_biz_01","shake_cara","shake_carK","shake_carSH","smkcig_prtl","smkcig_prtl_F"},
	{"PISS_IN","PISS_LOOP","PISS_OUT","PNM_LOOP_A","PNM_ARGUE2_A","PNM_ARGUE1_A"},
	{"boxhipin","bxwlko","catch_box","bxshwlk","bxhwlki","boxshup","boxhipup","boxshdwn","bxhipwlk","bxshwlki"},
	{"Stepsit_in","Stepsit_loop","Stepsit_out"},
	{"Barcustom_get","Barcustom_loop","Barcustom_order","BARman_idle","Barserve_bottle","Barserve_give","Barserve_glass","Barserve_in","Barserve_loop","Barserve_order","dnk_stndF_loop","dnk_stndM_loop"},
	{"Bat_1","Bat_2","Bat_3","Bat_4","Bat_Hit_1","Bat_block","Bat_Hit_2","Bat_Hit_3","Bat_IDLE","Bat_M"},
	{"BD_GF_Wave","BD_Panic_01","BD_Panic_02","BD_Panic_03","BD_Panic_04","BD_Panic_Loop","Grlfrd_Kiss_03","M_smklean_loop","Playa_Kiss_03","wash_up"},
	{"bather","Lay_Bac_Loop","ParkSit_M_loop","ParkSit_W_loop","SitnWait_loop_W"},
	{"gym_bp_celebrate","gym_bp_down","gym_bp_getoff","gym_bp_geton","gym_bp_up_A","gym_bp_up_B","gym_bp_up_smooth"},
	{"BJ_Car_End_W","BJ_Car_Loop_P","BJ_Car_Loop_W","BJ_Car_Start_P","BJ_Car_Start_W","BJ_Couch_End_P","BJ_Couch_End_W","BJ_Couch_Loop_P","BJ_Couch_Loop_W","BJ_Couch_Start_P","BJ_Couch_Start_W","BJ_Stand_End_P","BJ_Stand_End_W","BJ_Stand_Loop_P","BJ_Stand_Loop_W","BJ_Stand_Start_P","BJ_Stand_Start_W"},
	{"camcrch_cmon","camcrch_idleloop","camcrch_stay","camcrch_to_camstnd","camstnd_cmon","camstnd_idleloop","camstnd_lkabt","camstnd_to_camcrch","piccrch_in","piccrch_out","piccrch_take","picstnd_in","picstnd_out","picstnd_take"},
	{"Fixn_Car_Loop","Fixn_Car_Out","flag_drop"},
	{"carfone_in","carfone_loopA","carfone_loopA_to_B","carfone_loopB_to_A","carfone_out","CAR_Sc1_BL","CAR_Sc1_BR","CAR_Sc1_FL","CAR_Sc1_FR","CAR_Sc2_FL","CAR_Sc3_BR","CAR_Sc3_FL","CAR_Sc3_FR","CAR_Sc4_FL","car_talkm_in","car_talkm_loop","car_talkm_out"},
	{"cards_in","cards_loop","cards_lose","cards_out","cards_pick_01","cards_pick_02","cards_raise","cards_win","dealone","manwinb","manwind","Roulette_bet","Roulette_in","Roulette_loop","Roulette_lose","Roulette_out","Roulette_win","Slot_bet_01","Slot_bet_02","Slot_in","Slot_lose_out","Slot_Plyr","Slot_wait","Slot_win_out","wof"},
	{"Copbrowse_in","Copbrowse_loop","Copbrowse_nod","Copbrowse_out","Copbrowse_shake","Coplook_in","Coplook_loop","Coplook_nod","Coplook_out","Coplook_shake","Coplook_think","Coplook_watch"},
	{"Bbalbat_Idle_01","Bbalbat_Idle_02","crckdeth1","crckdeth2","crckdeth3","crckdeth4","crckidle1","crckidle2","crckidle3","crckidle4"},
	{"CRIB_Console_Loop","CRIB_Use_Switch","PED_Console_Loop","PED_Console_Loose","PED_Console_Win"},
	{"DAM_Dive_Loop","DAM_Land","DAM_Launch","Jump_Roll","SF_JumpWall"},
	{"bd_clap","bd_clap1","dance_loop","DAN_Down_A","DAN_Left_A","DAN_Loop_A","DAN_Right_A","DAN_Up_A","dnce_M_a","dnce_M_b","dnce_M_c","dnce_M_d","dnce_M_e"},
	{"DEALER_DEAL","DEALER_IDLE","DEALER_IDLE_01","DEALER_IDLE_02","DEALER_IDLE_03","DRUGS_BUY","shop_pay"},
	{"Cover_Dive_01","Cover_Dive_02","Crushed","Crush_Jump"},
	{"EAT_Burger","EAT_Chicken","EAT_Pizza","EAT_Vomit_P","EAT_Vomit_SK","FF_Dam_Bkw","FF_Dam_Fwd","FF_Dam_Left","FF_Dam_Right","FF_Die_Bkw","FF_Die_Fwd","FF_Die_Left","FF_Die_Right","FF_Sit_Eat1","FF_Sit_Eat2","FF_Sit_Eat3","FF_Sit_In","FF_Sit_In_L","FF_Sit_In_R","FF_Sit_Look","FF_Sit_Loop","FF_Sit_Out_180","FF_Sit_Out_L_180","FF_Sit_Out_R_180","SHP_Thank","SHP_Tray_In","SHP_Tray_Lift","SHP_Tray_Lift_In","SHP_Tray_Lift_Loop","SHP_Tray_Lift_Out","SHP_Tray_Out","SHP_Tray_Pose","SHP_Tray_Return"},
	{"Dance_B1","Dance_B2","Dance_B3","Dance_B4","Dance_B5","Dance_B6","Dance_B7","Dance_B8","Dance_B9","Dance_B10","Dance_B11","Dance_B12","Dance_B13","Dance_B14","Dance_B15","Dance_B16","Dance_G1","Dance_G2","Dance_G3","Dance_G4","Dance_G5","Dance_G6","Dance_G7","Dance_G8","Dance_G9","Dance_G10","Dance_G11","Dance_G12","Dance_G13","Dance_G14","Dance_G15","Dance_G16","dance_loop"},
	{"gym_barbell","gym_free_A","gym_free_B","gym_free_celebrate","gym_free_down","gym_free_loop","gym_free_pickup","gym_free_putdown","gym_free_up_smooth"},
	{"CAS_G2_GasKO","swt_wllpk_L","swt_wllpk_L_back","swt_wllpk_R","swt_wllpk_R_back","swt_wllshoot_in_L","swt_wllshoot_in_R","swt_wllshoot_out_L","swt_wllshoot_out_R","Use_SwipeCard"},
	{"BED_In_L","BED_In_R","BED_Loop_L","BED_Loop_R","BED_Out_L","BED_Out_R","LOU_In","LOU_Loop","LOU_Out","wash_up"},
	{"FF_Dam_Fwd","OFF_Sit_2Idle_180","OFF_Sit_Bored_Loop","OFF_Sit_Crash","OFF_Sit_Drink","OFF_Sit_Idle_Loop","OFF_Sit_In","OFF_Sit_Read","OFF_Sit_Type_Loop","OFF_Sit_Watch"},
	{"shop_cashier","shop_in","shop_lookA","shop_lookB","shop_loop","shop_out","shop_pay","shop_shelf"},
	{"BD_GF_Wave","gfwave2","GF_CarArgue_01","GF_CarArgue_02","GF_CarSpot","GF_StreetArgue_01","GF_StreetArgue_02","gift_get","gift_give","Grlfrd_Kiss_01","Grlfrd_Kiss_02","Grlfrd_Kiss_03","Playa_Kiss_01","Playa_Kiss_02","Playa_Kiss_03"},
	{"F_smklean_loop","lrgirl_bdbnce","lrgirl_hair","lrgirl_hurry","lrgirl_idleloop","lrgirl_idle_to_l0","lrgirl_l0_bnce","lrgirl_l0_loop","lrgirl_l0_to_l1","lrgirl_l12_to_l0","lrgirl_l1_bnce","lrgirl_l1_loop","lrgirl_l1_to_l2","lrgirl_l2_bnce","lrgirl_l2_loop","lrgirl_l2_to_l3","lrgirl_l345_to_l1","lrgirl_l3_bnce","lrgirl_l3_loop","lrgirl_l3_to_l4","lrgirl_l4_bnce","lrgirl_l4_loop","lrgirl_l4_to_l5","lrgirl_l5_bnce","lrgirl_l5_loop","M_smklean_loop","M_smkstnd_loop","prtial_gngtlkB","prtial_gngtlkC","prtial_gngtlkD","prtial_gngtlkE","prtial_gngtlkF","prtial_gngtlkG","prtial_gngtlkH","RAP_A_Loop","RAP_B_Loop","RAP_C_Loop","Sit_relaxed","Tap_hand"},
	{"CPR"},
	{"bitchslap","BMX_celebrate","BMX_comeon","bmx_idleloop_01","bmx_idleloop_02","bmx_talkleft_in","bmx_talkleft_loop","bmx_talkleft_out","bmx_talkright_in","bmx_talkright_loop","bmx_talkright_out","bng_wndw","bng_wndw_02","Case_pickup","door_jet","GRAB_L","GRAB_R","Hiker_Pose","Hiker_Pose_L","Idle_Chat_02","KAT_Throw_K","KAT_Throw_O","KAT_Throw_P","PASS_Rifle_O","PASS_Rifle_Ped","PASS_Rifle_Ply","pickup_box","Plane_door","Plane_exit","Plane_hijack","Plunger_01","Plyrlean_loop","plyr_shkhead","Run_Dive","Scratchballs_01","SEAT_LR","Seat_talk_01","Seat_talk_02","SEAT_watch","smalplane_door","smlplane_door"},
	{"betslp_in","betslp_lkabt","betslp_loop","betslp_out","betslp_tnk","wtchrace_cmon","wtchrace_in","wtchrace_loop","wtchrace_lose","wtchrace_out","wtchrace_win"},
	{"Tai_Chi_in","Tai_Chi_Loop","Tai_Chi_Out"},
	{"shift","shldr","stretch","strleg","time"},
	{"CopTraf_Away","CopTraf_Come","CopTraf_Left","CopTraf_Stop","COP_getoutcar_LHS","Cop_move_FWD","crm_drgbst_01","Door_Kick","plc_drgbst_01","plc_drgbst_02"},
	{"Laugh_01","RAP_A_IN","RAP_A_Loop","RAP_A_OUT","RAP_B_IN","RAP_B_Loop","RAP_B_OUT","RAP_C_Loop"},
	{"RIOT_ANGRY","RIOT_ANGRY_B","RIOT_challenge","RIOT_CHANT","RIOT_FUKU","RIOT_PUNCHES","RIOT_shout"},
	{"CAT_Safe_End","CAT_Safe_Open","CAT_Safe_Open_O","CAT_Safe_Rob","SHP_HandsUp_Scr"},
	{"Dance_B1","Dance_B2","Dance_B3","Dance_B4","Dance_B5","Dance_B6","Dance_B7","Dance_B8","Dance_B9","Dance_B10","Dance_B11","Dance_B12","Dance_B13","Dance_B14","Dance_B15","Dance_B16","Dance_G1","Dance_G2","Dance_G3","Dance_G4","Dance_G5","Dance_G6","Dance_G7","Dance_G8","Dance_G9","Dance_G10","Dance_G11","Dance_G12","Dance_G13","Dance_G14","Dance_G15","Dance_G16","dance_loop"},
	{"scdldlp","scdlulp","scdrdlp","scdrulp","sclng_l","sclng_r","scmid_l","scmid_r","scshrtl","scshrtr","sc_ltor","sc_rtol"},
	{"SEX_1to2_P","SEX_1to2_W","SEX_1_Cum_P","SEX_1_Cum_W","SEX_1_Fail_P","SEX_1_Fail_W","SEX_1_P","SEX_1_W","SEX_2to3_P","SEX_2to3_W","SEX_2_Fail_P","SEX_2_Fail_W","SEX_2_P","SEX_2_W","SEX_3to1_P","SEX_3to1_W","SEX_3_Fail_P","SEX_3_Fail_W","SEX_3_P","SEX_3_W"},
	{"ROB_2Idle","ROB_Loop","ROB_Loop_Threat","ROB_Shifty","ROB_StickUp_In","SHP_Duck","SHP_Duck_Aim","SHP_Duck_Fire","SHP_Gun_Aim","SHP_Gun_Duck","SHP_Gun_Fire","SHP_Gun_Grab","SHP_Gun_Threat","SHP_HandsUp_Scr","SHP_Jump_Glide","SHP_Jump_Land","SHP_Jump_Launch","SHP_Rob_GiveCash","SHP_Rob_HandsUp","SHP_Rob_React","SHP_Serve_End","SHP_Serve_Idle","SHP_Serve_Loop","SHP_Serve_Start","Smoke_RYD" },
	{"F_smklean_loop","M_smklean_loop","M_smkstnd_loop","M_smk_drag","M_smk_in","M_smk_loop","M_smk_out","M_smk_tap"},
	{"SnM_Caned_Idle_P","SnM_Caned_Idle_W","SnM_Caned_P","SnM_Caned_W","SnM_Cane_Idle_P","SnM_Cane_Idle_W","SnM_Cane_P","SnM_Cane_W","SpankedP","SpankedW","Spanked_IdleP","Spanked_IdleW","SpankingP","SpankingW","Spanking_endP","Spanking_endW","Spanking_IdleP","Spanking_IdleW","Spanking_SittingIdleP","Spanking_SittingIdleW","Spanking_SittingP","Spanking_SittingW"},
	{"PLY_CASH","PUN_CASH","PUN_HOLLER","PUN_LOOP","strip_A","strip_B","strip_C","strip_D","strip_E","strip_F","strip_G","STR_A2B","STR_B2A","STR_B2C","STR_C1","STR_C2","STR_C2B","STR_Loop_A","STR_Loop_B","STR_Loop_C" },
	{"batherdown","batherup","Lay_Bac_in","Lay_Bac_out","ParkSit_M_IdleA","ParkSit_M_IdleB","ParkSit_M_IdleC","ParkSit_M_in","ParkSit_M_out","ParkSit_W_idleA","ParkSit_W_idleB","ParkSit_W_idleC","ParkSit_W_in","ParkSit_W_out","SBATHE_F_LieB2Sit","SBATHE_F_Out","SitnWait_in_W","SitnWait_out_W"},
	{"gnstwall_injurd","JMP_Wall1m_180","Rail_fall","Rail_fall_crawl","swt_breach_01","swt_breach_02","swt_breach_03","swt_go","swt_lkt","swt_sty","swt_vent_01","swt_vent_02","swt_vnt_sht_die","swt_vnt_sht_in","swt_vnt_sht_loop","swt_wllpk_L","swt_wllpk_L_back","swt_wllpk_R","swt_wllpk_R_back","swt_wllshoot_in_L","swt_wllshoot_in_R","swt_wllshoot_out_L","swt_wllshoot_out_R"},
	{"VEND_Drink2_P","VEND_Drink_P","vend_eat1_P","VEND_Eat_P","VEND_Use","VEND_Use_pt2"},
	{"Dance_B1","Dance_B2","Dance_B3","Dance_B4","Dance_B5","Dance_B6","Dance_B7","Dance_B8","Dance_B9","Dance_B10","Dance_B11","Dance_B12","Dance_B13","Dance_B14","Dance_B15","Dance_B16","Dance_G1","Dance_G2","Dance_G3","Dance_G4","Dance_G5","Dance_G6","Dance_G7","Dance_G8","Dance_G9","Dance_G10","Dance_G11","Dance_G12","Dance_G13","Dance_G14","Dance_G15","Dance_G16","dance_loop"}
	};
	for (int i = 0; i < std::size(packNames); ++i) {
		animations_[packNames[i]] = animationNames[i];
	}
	if (auto userAnimations = Settings::getInstance().get<std::map<std::string, std::vector<std::string>>>("data.pedAnimations"); userAnimations.has_value()) {
		for (auto [packName, newAnims] : userAnimations.value()) {
			if (animations_.contains(packName)) {
				auto& anims = animations_[packName];
				anims.reserve(anims.size() + newAnims.size());
				anims.insert(anims.end(), newAnims.begin(), newAnims.end());
			} else {
				animations_[packName] = newAnims;
			}
		}
	}
	packsNames_.reserve(animations_.size());
	for (auto [packName, _] : animations_)
		packsNames_.emplace_back(packName);

	this->particlesNames_ = { "blood_heli","boat_prop","camflash","carwashspray","cement","cloudfast","coke_puff","coke_trail","cigarette_smoke","explosion_barrel","explosion_crate","explosion_door","exhale","explosion_fuel_car","explosion_large","explosion_medium","explosion_molotov","explosion_small","explosion_tiny","extinguisher","flame","fire","fire_med","fire_large","flamethrower","fire_bike","fire_car","gunflash","gunsmoke","insects","heli_dust","jetpack","jetthrust","nitro","molotov_flame","overheat_car","overheat_car_electric","prt_boatsplash","prt_cardebris","prt_collisionsmoke","prt_glass","prt_gunshell","prt_sand","prt_sand2","prt_smokeII_3_expand","prt_smoke_huge","prt_spark","prt_spark_2","prt_splash","prt_wake","prt_watersplash","prt_wheeldirt","petrolcan","puke","riot_smoke","spraycan","smoke30lit","smoke30m","smoke50lit","shootlight","smoke_flare","tank_fire","teargas","teargasAD","tree_hit_fir","tree_hit_palm","vent","vent2","water_hydrant","water_ripples","water_speed","water_splash","water_splash_big","water_splsh_sml","water_swim","waterfall_end","water_fnt_tme","water_fountain","wallbust","WS_factorysmoke" };
	this->pedBones_ = { {1,"BONE_PELVIS1"},{2,"BONE_PELVIS"},{3,"BONE_SPINE1"},{4,"BONE_UPPERTORSO"},{5,"BONE_NECK"},{6,"BONE_HEAD2"},{7,"BONE_HEAD1"},{8,"BONE_HEAD"},{21,"BONE_RIGHTUPPERTORSO"},{22,"BONE_RIGHTSHOULDER"},{23,"BONE_RIGHTELBOW"},{24,"BONE_RIGHTWRIST"},{25,"BONE_RIGHTHAND"},{26,"BONE_RIGHTTHUMB"},{31,"BONE_LEFTUPPERTORSO"},{32,"BONE_LEFTSHOULDER"},{33,"BONE_LEFTELBOW"},{34,"BONE_LEFTWRIST"},{35,"BONE_LEFTHAND"},{36,"BONE_LEFTTHUMB"},{41,"BONE_LEFTHIP"},{42,"BONE_LEFTKNEE"},{43,"BONE_LEFTANKLE"},{44,"BONE_LEFTFOOT"},{51,"BONE_RIGHTHIP"},{52,"BONE_RIGHTKNEE"},{53,"BONE_RIGHTANKLE"},{54,"BONE_RIGHTFOOT"}};
}

int ModelsService::validPedModel(const int model) {
	int returnModel = 0;
	for (const int pedModel : getInstance().getPedModels()) {
		if (pedModel <= model)
			returnModel = pedModel;
	}
	return returnModel;
}

int ModelsService::validWeaponModel(const int model) {
	int returnModel = 0;
	for (const int weaponModel : getInstance().getWeaponIds()) {
		if (weaponModel <= model)
			returnModel = weaponModel;
	}
	return returnModel;
}

int ModelsService::validVehiceModel(int model) {
	int returnModel = 0;
	for (const int pedModel : getInstance().getVehicleModels()) {
		if (pedModel <= model)
			returnModel = pedModel;
	}
	return returnModel;
}

bool ModelsService::validObjectModel(const int model) const {
	return CModelInfo::GetModelInfo(model) != nullptr;
}

std::optional<int> ModelsService::addNewTag(const std::string& tagName) {
	try {
		SQLite::Statement query(this->db_, "INSERT INTO tags (name) VALUES(?) RETURNING id;");
		query.bind(1, tagName);
		if (query.executeStep()) {
			loadModelTags();
			return query.getColumn(0).getInt();
		}
	}
	catch (std::exception& e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
	}
	return std::nullopt;
}

bool ModelsService::deleteTag(int tagId) {
	try {
		SQLite::Statement query(this->db_, "DELETE FROM tags WHERE id = ?;");
		query.bind(1, tagId);
		query.exec();
		loadModelTags();
		return true;
	}
	catch (std::exception& e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		return false;
	}
}

bool ModelsService::hasObjectInfo(int objectModel) {
	return true;
}

bool ModelsService::setModelToTag(int modelId, int tagId) {
	try {
		SQLite::Statement query(this->db_, "INSERT INTO tag_to_models(tag_id, model_id) VALUES(?, ?);");
		query.bind(1, tagId);
		query.bind(2, modelId);
		query.exec();
		return true;
	}
	catch (std::exception& e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		return false;
	}
}

bool ModelsService::clearModelToTag(int modelId, int tagId) {
	try {
		SQLite::Statement query(this->db_, "DELETE FROM tag_to_models WHERE tag_id = ? AND model_id = ?");
		query.bind(1, tagId);
		query.bind(2, modelId);
		query.exec();
		return true;
	} catch (std::exception& e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		return false;
	}
}

bool ModelsService::newTitleNameModel(int modelId, const std::string& newTitleName) {
	try {
		SQLite::Statement query(this->db_, "UPDATE models SET title_name= ? WHERE model_id= ?;");
		query.bind(1, newTitleName);
		query.bind(2, modelId);
		query.exec();
		return true;
	}
	catch (std::exception& e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		return false;
	}
}

std::set<int> ModelsService::getModelTags(int model) {
	std::set<int> tags;
	try {
		SQLite::Statement query(this->db_, "SELECT tag_id FROM tag_to_models WHERE model_id = ?");
		query.bind(1, model);
		while (query.executeStep()) {
			tags.emplace(query.getColumn(0));
		}
	}
	catch (std::exception& e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		throw;
	}
	return tags;
}

const auto ICONS_MODELS_PATH = "LDYOM/icons_models.sqlite3";
std::optional<std::unique_ptr<Texture>> ModelsService::getModelIcon(const int modelId) {
	if (!std::filesystem::exists(ICONS_MODELS_PATH))
		return std::nullopt;
	if (!this->iconDb_.has_value()) {
		this->iconDb_ = SQLite::Database(ICONS_MODELS_PATH);
	}
	try {
		SQLite::Statement query(this->iconDb_.value(), "SELECT icon FROM icons WHERE model_id = ?;");
		query.bind(1, modelId);
		if (query.executeStep()) {
			const auto iconData = query.getColumn(0).getBlob();
			const int iconDataSize = query.getColumn(0).getBytes();
			return utils::LoadTextureRequiredFromMemory(iconData, iconDataSize);
		}
		return std::nullopt;
	}
	catch (std::exception& e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		throw;
	}
}

void ModelsService::loadModelCategories() {
	this->modelCategories_ = std::vector<ObjectModelCategory>();
	try {
		SQLite::Statement query(this->db_, "SELECT * FROM categories;");
		while (query.executeStep()) {
			this->modelCategories_->emplace_back(
				query.getColumn("id"),
				query.getColumn("category_name"),
				query.getColumn("parent_id")
			);
		}
	}
	catch (std::exception& e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		throw;
	}
}

std::vector<ObjectModelCategory>& ModelsService::getObjectModelCategories() {
	if (!this->modelCategories_.has_value()) {
		loadModelCategories();
	}
	return this->modelCategories_.value();
}

void ModelsService::loadModelTags() {
	this->modelTags_ = std::vector<ObjectModelTag>();
	try {
		SQLite::Statement query(this->db_, "SELECT * FROM tags;");
		while (query.executeStep()) {
			this->modelTags_->emplace_back(
				query.getColumn("id"),
				query.getColumn("name")
			);
		}
	}
	catch (std::exception& e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		throw;
	}
}

std::vector<ObjectModelTag>& ModelsService::getObjectModelTags() {
	if (!this->modelTags_.has_value()) {
		loadModelTags();
	}
	return this->modelTags_.value();
}

std::map<int, ObjectInfo> ModelsService::getObjectModelsBySearch(const std::string& searchText, std::set<int>& categories,
                                                                 int objectBehaviourHasCollision, int objectBehaviourDestroyByHit, int objectBehaviourSeeByTime, int countOnPage, int page, int& countModels) {
	std::map<int, ObjectInfo> models{};

	try {
		auto baseQuery = fmt::format("FROM category_to_models JOIN models ON category_to_models.model_id = models.model_id WHERE (CAST(models.model_id as TEXT) LIKE '%{}%' OR models.title_name LIKE '%{}%' OR models.model_name LIKE '%{}%') ", searchText, searchText, searchText);

		if (objectBehaviourHasCollision != -1) {
			baseQuery += fmt::format(" AND models.has_collision = {}", objectBehaviourHasCollision);
		}
		if (objectBehaviourDestroyByHit != -1) {
			baseQuery += fmt::format(" AND models.is_destroy_by_hit = {}", objectBehaviourDestroyByHit);
		}
		if (objectBehaviourSeeByTime != -1) {
			baseQuery += fmt::format(" AND models.see_by_time = {}", objectBehaviourSeeByTime);
		}

		if (!categories.empty()) {
			std::set categoriesRaw(categories);
			const auto & objectModelCategories = getObjectModelCategories();
			int i = 0;
			while(i < static_cast<int>(objectModelCategories.size())) {
				if (categoriesRaw.contains(objectModelCategories[i].parentId)) {
					categoriesRaw.emplace(objectModelCategories[i].id);
				}
				i++;
			}

			std::string strCategories;
			for (int category : categoriesRaw)
				strCategories += fmt::format("{},", category);
			strCategories.pop_back();
			baseQuery += fmt::format(" AND category_id IN({})", strCategories);
		}


		std::string getQuery = fmt::format("SELECT models.* {} LIMIT {}, {}", baseQuery, (page - 1) * countOnPage, countOnPage);
		std::string countQuery = "SELECT COUNT(*) " + baseQuery;

		SQLite::Statement query(this->db_, getQuery.c_str());
		while (query.executeStep()) {
			models.emplace(query.getColumn("model_id").getInt(),
				ObjectInfo {
					query.getColumn("model_id"),
					query.getColumn("title_name"),
					query.getColumn("model_name"),
					query.getColumn("has_collision").getInt() == 1,
					query.getColumn("is_destroy_by_hit").getInt() == 1,
					query.getColumn("see_by_time").getInt() == 1,
					query.getColumn("icon_url")
				});
		}
		SQLite::Statement queryCount(this->db_, countQuery.c_str());
		if (queryCount.executeStep()) {
			countModels = queryCount.getColumn(0);
		}
	}
	catch (std::exception& e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		throw;
	}

	return models;
}

std::map<int, ObjectInfo> ModelsService::getObjectModelsByTag(int tagId, int page, int countOnPage, int& countModels) {
	std::map<int, ObjectInfo> models{};

	try {
		auto baseQuery = "FROM models JOIN tag_to_models ON models.model_id = tag_to_models.model_id WHERE tag_id = ?";

		std::string getQuery = fmt::format("SELECT models.* {} LIMIT {}, {}", baseQuery, (page - 1) * countOnPage, countOnPage);
		std::string countQuery = fmt::format("SELECT COUNT(*) {}", baseQuery);

		SQLite::Statement query(this->db_, getQuery.c_str());
		query.bind(1, tagId);
		while (query.executeStep()) {
			models.emplace(query.getColumn("model_id").getInt(),
				ObjectInfo{
					query.getColumn("model_id"),
					query.getColumn("title_name"),
					query.getColumn("model_name"),
					query.getColumn("has_collision").getInt() == 1,
					query.getColumn("is_destroy_by_hit").getInt() == 1,
					query.getColumn("see_by_time").getInt() == 1,
					query.getColumn("icon_url")
				});
		}
		SQLite::Statement queryCount(this->db_, countQuery.c_str());
		queryCount.bind(1, tagId);
		if (queryCount.executeStep()) {
			countModels = queryCount.getColumn(0);
		}
	}
	catch (std::exception& e) {
		std::stringstream error;
		error << "SQLite exception: " << e.what() << std::endl;
		Logger::getInstance().log(error.str());
		throw;
	}

	return models;
}

std::vector<int>& ModelsService::getPedModels() {
	return pedModels_;
}

std::vector<int>& ModelsService::getWeaponIds() {
	return weaponIds_;
}

std::vector<std::string>& ModelsService::getSpecialsPed() {
	return specialsPed_;
}

std::vector<int>& ModelsService::getVehicleModels() {
	return vehicleModels_;
}

std::vector<std::string>& ModelsService::getParticlesNames() {
	return particlesNames_;
}

std::map<int, std::string>& ModelsService::getPedBones() {
	return pedBones_;
}

std::vector<std::string>& ModelsService::getPacksNames() {
	return packsNames_;
}

std::map<std::string, std::vector<std::string>>& ModelsService::getAnimations() {
	return animations_;
}

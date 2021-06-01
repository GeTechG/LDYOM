#include "Init.h"


#include <CClock.h>
#include <CHud.h>
#include <CRadar.h>
#include <sol/sol.hpp>


#include "libs/coro_wait.h"
#include "CCamera.h"
#include "CMessages.h"
#include "CTheScripts.h"

extern unsigned int get_car_color_rgba(unsigned char id);
extern inline double rad(float degrees);
extern inline double deg(float radians);
extern void printLog(std::string print_text);
extern vector<std::string> namesAudioFiles;
extern std::vector<std::string> get_filename_list(const std::string& path, const std::string& extension);
extern std::string* nameCurrPack;
extern std::string replace_symb(std::string& str);
extern CPed *playerPed;
extern bool updateSphere;
extern std::string UTF8_to_CP1251(std::string const& utf8);
extern void GXTEncode(std::string& str);
extern bool mission_started;
extern bool storyline_started;
extern bool is_utf8(const char* string);
extern coro_wait instance;
extern bool defeat;
extern void failMission();
void rotateVec2(float& x, float& y, float angle);

float camera_zoom = 5.0f;
float camera_angle[2] = { 45.0f, 0.0f };
bool storylineMode = false;
bool editmodeCamera = false;
bool editmodeTimetarg = false;
bool editmodeTeleportPlayer = false;
bool editmodeNodeAnimActor = false;
bool editmodeNodePathActor = false;
bool editmodeNodePathActorCar = false;
bool editActor = false;
bool editmodeCar = false;
bool editPlayer = false;
bool editmodeTimemiss = false;
PDIRECT3DTEXTURE9 weaponsAtlas;
PDIRECT3DTEXTURE9 pedsSkinAtlas;
PDIRECT3DTEXTURE9 blipsAtlas;
//nodeEditmode = nil,
const int ID_Cars[212] = { 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611 };
const int ID_Weapons[44] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46 };
vector <std::string> Particle_name = { "blood_heli","boat_prop","camflash","carwashspray","cement","cloudfast","coke_puff","coke_trail","cigarette_smoke","explosion_barrel","explosion_crate","explosion_door","exhale","explosion_fuel_car","explosion_large","explosion_medium","explosion_molotov","explosion_small","explosion_tiny","extinguisher","flame","fire","fire_med","fire_large","flamethrower","fire_bike","fire_car","gunflash","gunsmoke","insects","heli_dust","jetpack","jetthrust","nitro","molotov_flame","overheat_car","overheat_car_electric","prt_boatsplash","prt_cardebris","prt_collisionsmoke","prt_glass","prt_gunshell","prt_sand","prt_sand2","prt_smokeII_3_expand","prt_smoke_huge","prt_spark","prt_spark_2","prt_splash","prt_wake","prt_watersplash","prt_wheeldirt","petrolcan","puke","riot_smoke","spraycan","smoke30lit","smoke30m","smoke50lit","shootlight","smoke_flare","tank_fire","teargas","teargasAD","tree_hit_fir","tree_hit_palm","vent","vent2","water_hydrant","water_ripples","water_speed","water_splash","water_splash_big","water_splsh_sml","water_swim","waterfall_end","water_fnt_tme","water_fountain","wallbust","WS_factorysmoke" };
vector <int> ID_Actors = { 0, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288 };
vector <std::string> ID_Spec_Actors = { "andre", "bbthin", "bb", "cat", "cesar", "claude", "dwayne", "emmet", "forelli", "janitor", "jethro", "jizzy", "hern", "kendl", "maccer", "maddogg", "ogloc", "paul", "pulaski", "rose", "ryder", "ryder3", "sindaco", "smoke", "smokev", "suzie", "sweet", "tbone", "tenpen", "torino", "truth", "wuzimu", "zero", "gangrl2", "copgrl1", "copgrl2", "crogrl1", "crogrl2", "gungrl1", "gungrl2", "mecgrl2", "nurgrl2", "ryder2", "cdeput", "sfpdm1", "lvpdm1", "csbmydj", "psycho", "csmech", "csomost", "wmycd2" };
vector <std::string> Anim_name = { "PED","BOMBER","POOL","ON_LOOKERS","GANGS","PAULNMAC","BOX","Attractors","BAR","BASEBALL","BD_FIRE","BEACH","benchpress","BLOWJOBZ","CAMERA","CAR","CAR_CHAT","CASINO","COP_AMBIENT","CRACK","CRIB","DAM_JUMP","DANCING","DEALER","DODGE","FOOD","Freeweights","GFUNK","HEIST9","INT_HOUSE","INT_OFFICE","INT_SHOP","KISSING","LOWRIDER","MEDIC","MISC","OTB","PARK","PLAYIDLES","POLICE","RAPPING","RIOT","ROB_BANK","RUNNINGMAN","SCRATCHING","SEX","SHOP","SMOKING","SnM","STRIP","SUNBATHE","SWAT","VENDING","WOP" };
vector <vector <std::string>> Anim_list = { {"abseil","ARRESTgun","ATM","BIKE_elbowL","BIKE_elbowR","BIKE_fallR","BIKE_fall_off","BIKE_pickupL","BIKE_pickupR","BIKE_pullupL","BIKE_pullupR","bomber","CAR_alignHI_LHS","CAR_alignHI_RHS","CAR_align_LHS","CAR_align_RHS","CAR_closedoorL_LHS","CAR_closedoorL_RHS","CAR_closedoor_LHS","CAR_closedoor_RHS","CAR_close_LHS","CAR_close_RHS","CAR_crawloutRHS","CAR_dead_LHS","CAR_dead_RHS","CAR_doorlocked_LHS","CAR_doorlocked_RHS","CAR_fallout_LHS","CAR_fallout_RHS","CAR_getinL_LHS","CAR_getinL_RHS","CAR_getin_LHS","CAR_getin_RHS","CAR_getoutL_LHS","CAR_getoutL_RHS","CAR_getout_LHS","CAR_getout_RHS","car_hookertalk","CAR_jackedLHS","CAR_jackedRHS","CAR_jumpin_LHS","CAR_LB","CAR_LB_pro","CAR_LB_weak","CAR_LjackedLHS","CAR_LjackedRHS","CAR_Lshuffle_RHS","CAR_Lsit","CAR_open_LHS","CAR_open_RHS","CAR_pulloutL_LHS","CAR_pulloutL_RHS","CAR_pullout_LHS","CAR_pullout_RHS","CAR_Qjacked","CAR_rolldoor","CAR_rolldoorLO","CAR_rollout_LHS","CAR_rollout_RHS","CAR_shuffle_RHS","CAR_sit","CAR_sitp","CAR_sitpLO","CAR_sit_pro","CAR_sit_weak","CAR_tune_radio","CLIMB_idle","CLIMB_jump","CLIMB_jump2fall","CLIMB_jump_B","CLIMB_Pull","CLIMB_Stand","CLIMB_Stand_finish","cower","Crouch_Roll_L","Crouch_Roll_R","DAM_armL_frmBK","DAM_armL_frmFT","DAM_armL_frmLT","DAM_armR_frmBK","DAM_armR_frmFT","DAM_armR_frmRT","DAM_LegL_frmBK","DAM_LegL_frmFT","DAM_LegL_frmLT","DAM_LegR_frmBK","DAM_LegR_frmFT","DAM_LegR_frmRT","DAM_stomach_frmBK","DAM_stomach_frmFT","DAM_stomach_frmLT","DAM_stomach_frmRT","DOOR_LHinge_O","DOOR_RHinge_O","DrivebyL_L","DrivebyL_R","Driveby_L","Driveby_R","DRIVE_BOAT","DRIVE_BOAT_back","DRIVE_BOAT_L","DRIVE_BOAT_R","Drive_L","Drive_LO_l","Drive_LO_R","Drive_L_pro","Drive_L_pro_slow","Drive_L_slow","Drive_L_weak","Drive_L_weak_slow","Drive_R","Drive_R_pro","Drive_R_pro_slow","Drive_R_slow","Drive_R_weak","Drive_R_weak_slow","Drive_truck","DRIVE_truck_back","DRIVE_truck_L","DRIVE_truck_R","Drown","DUCK_cower","endchat_01","endchat_02","endchat_03","EV_dive","EV_step","facanger","facgum","facsurp","facsurpm","factalk","facurios","FALL_back","FALL_collapse","FALL_fall","FALL_front","FALL_glide","FALL_land","FALL_skyDive","Fight2Idle","FightA_1","FightA_2","FightA_3","FightA_block","FightA_G","FightA_M","FIGHTIDLE","FightShB","FightShF","FightSh_BWD","FightSh_FWD","FightSh_Left","FightSh_Right","flee_lkaround_01","FLOOR_hit","FLOOR_hit_f","fucku","gang_gunstand","gas_cwr","getup","getup_front","gum_eat","GunCrouchBwd","GunCrouchFwd","GunMove_BWD","GunMove_FWD","GunMove_L","GunMove_R","Gun_2_IDLE","GUN_BUTT","GUN_BUTT_crouch","Gun_stand","handscower","handsup","HitA_1","HitA_2","HitA_3","HIT_back","HIT_behind","HIT_front","HIT_GUN_BUTT","HIT_L","HIT_R","HIT_walk","HIT_wall","Idlestance_fat","idlestance_old","IDLE_armed","IDLE_chat","IDLE_csaw","Idle_Gang1","IDLE_HBHB","IDLE_ROCKET","IDLE_stance","IDLE_taxi","IDLE_tired","Jetpack_Idle","JOG_femaleA","JOG_maleA","JUMP_glide","JUMP_land","JUMP_launch","JUMP_launch_R","KART_drive","KART_L","KART_LB","KART_R","KD_left","KD_right","KO_shot_face","KO_shot_front","KO_shot_stom","KO_skid_back","KO_skid_front","KO_spin_L","KO_spin_R","pass_Smoke_in_car","phone_in","phone_out","phone_talk","Player_Sneak","Player_Sneak_walkstart","roadcross","roadcross_female","roadcross_gang","roadcross_old","run_1armed","run_armed","run_civi","run_csaw","run_fat","run_fatold","run_gang1","run_left","run_old","run_player","run_right","run_rocket","Run_stop","Run_stopR","Run_Wuzi","SEAT_down","SEAT_idle","SEAT_up","SHOT_leftP","SHOT_partial","SHOT_partial_B","SHOT_rightP","Shove_Partial","Smoke_in_car","sprint_civi","sprint_panic","Sprint_Wuzi","swat_run","Swim_Tread","Tap_hand","Tap_handP","turn_180","Turn_L","Turn_R","WALK_armed","WALK_civi","WALK_csaw","Walk_DoorPartial","WALK_drunk","WALK_fat","WALK_fatold","WALK_gang1","WALK_gang2","WALK_old","WALK_player","WALK_rocket","WALK_shuffle","WALK_start","WALK_start_armed","WALK_start_csaw","WALK_start_rocket","Walk_Wuzi","WEAPON_crouch","woman_idlestance","woman_run","WOMAN_runbusy","WOMAN_runfatold","woman_runpanic","WOMAN_runsexy","WOMAN_walkbusy","WOMAN_walkfatold","WOMAN_walknorm","WOMAN_walkold","WOMAN_walkpro","WOMAN_walksexy","WOMAN_walkshop"},
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

Actor::Actor(const char* name, float x, float y, float z, float angle, int lastTarget) {
	strcpy(this->name, name);
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
	this->angle = angle;
	this->startC = lastTarget;
	Command<Commands::GET_AREA_VISIBLE>(&this->interiorID);
}

Actor::Actor(const Actor& actor) {
	strcpy(this->name, actor.name);
	memcpy(pos, actor.pos, 3 * sizeof(float));
	angle = actor.angle;
	modelID = actor.modelID;
	modelType = actor.modelType;
	slotSkin = actor.slotSkin;
	startC = actor.startC;
	endC = actor.endC;
	shouldNotDie = actor.shouldNotDie;
	health = actor.health;
	weapon = actor.weapon;
	ammo = actor.ammo;
	group = actor.group;
	randomSpawn = actor.randomSpawn;
	accuracy = actor.accuracy;
	useTarget = actor.useTarget;
	headshot = actor.headshot;
	interiorID = actor.interiorID;
	stayInSamePlace = actor.stayInSamePlace;
	kindaStayInSamePlace = actor.kindaStayInSamePlace;
}

void Actor::updateEditorPed() {
	this->removeEditorPed();
	int modell;
	if (this->modelType == 0) {
		modell = this->modelID;
		CStreaming::RequestModel(modell, 0);
		CStreaming::LoadAllRequestedModels(false);
	}
	else if (this->modelType == 1) {
		const std::string modell_n = ID_Spec_Actors[this->modelID];
		CStreaming::RequestSpecialChar(this->slotSkin - 1, modell_n.c_str(), 0);
		CStreaming::LoadAllRequestedModels(false);
		modell = 290 + this->slotSkin - 1;
	}
	this->editorPed = new CCivilianPed(static_cast<ePedType>(4), modell);
	this->editorPed->SetPosn(this->pos[0], this->pos[1], this->pos[2]);
	this->editorPed->SetHeading((float)rad(this->angle));
	this->editorPed->m_bUsesCollision = false;
	this->editorPed->m_bStreamingDontDelete = 1;
	this->editorPed->m_nCreatedBy = 2;
	CWorld::Add(this->editorPed);
	int weap_modell;
	Command<Commands::GET_WEAPONTYPE_MODEL>(ID_Weapons[this->weapon], &weap_modell);
	if (weap_modell != -1) {
		CStreaming::RequestModel(weap_modell, 0);
		CStreaming::LoadAllRequestedModels(false);
	}
	this->editorPed->GiveWeapon(static_cast<eWeaponType>(ID_Weapons[this->weapon]), ammo, false);
	this->editorPed->SetCurrentWeapon(static_cast<eWeaponType>(ID_Weapons[this->weapon]));
	CStreaming::RemoveAllUnusedModels();
}

void Actor::removeEditorPed() {
	if (this->editorPed != nullptr) {
		CWorld::Remove(this->editorPed);
		this->editorPed->Remove();
		delete this->editorPed;
		this->editorPed = nullptr;
	}
}

void Actor::updateMissionPed() {
	this->removeMissionPed();
	int modell;
	if (this->modelType == 0) {
		modell = this->modelID;
		CStreaming::RequestModel(modell, 0);
		CStreaming::LoadAllRequestedModels(false);
	}
	else if (this->modelType == 1) {
		const std::string modell_n = ID_Spec_Actors[this->modelID];
		CStreaming::RequestSpecialChar(this->slotSkin - 1, modell_n.c_str(), 0);
		CStreaming::LoadAllRequestedModels(false);
		modell = 290 + this->slotSkin - 1;
	}
	int pedType = 23 + this->group;
	int pedHandl;
	Command<Commands::CREATE_CHAR>(pedType, modell, this->pos[0], this->pos[1], this->pos[2], &pedHandl);
	this->missionPed = CPools::GetPed(pedHandl);
	this->missionPed->DisablePedSpeech(1);
	this->missionPed->SetPosn(this->pos[0], this->pos[1], this->pos[2]);
	Command<Commands::SET_CHAR_HEADING>(this->missionPed, this->angle);
	Command<Commands::SET_CHAR_STAY_IN_SAME_PLACE>(this->missionPed, (int)stayInSamePlace);
	Command<Commands::SET_CHAR_KINDA_STAY_IN_SAME_PLACE>(this->missionPed, (int)kindaStayInSamePlace);
	this->missionPed->m_bUsesCollision = false;
	this->missionPed->m_fHealth = this->health;
	this->missionPed->m_nWeaponAccuracy = this->accuracy;
	Command<Commands::SET_CHAR_SUFFERS_CRITICAL_HITS>(this->missionPed, (int)this->headshot);
	if (this->group == 0)
	{
		int g;
		Command<Commands::GET_PLAYER_GROUP>(0, &g);
		Command<Commands::SET_GROUP_MEMBER>(g, this->missionPed);
	}
	while (mission_started && DistanceBetweenPoints(TheCamera.GetPosition(),missionPed->GetPosition()) > 100.0f)
	{
		this_coro::wait(0ms);
	}
	this->missionPed->m_bUsesCollision = true;
	int weap_modell;
	Command<Commands::GET_WEAPONTYPE_MODEL>(ID_Weapons[this->weapon], &weap_modell);
	if (weap_modell != -1) {
		CStreaming::RequestModel(weap_modell, 0);
		CStreaming::LoadAllRequestedModels(false);
	}
	this->missionPed->GiveWeapon(static_cast<eWeaponType>(ID_Weapons[this->weapon]), ammo, false);
	this->missionPed->SetCurrentWeapon(static_cast<eWeaponType>(ID_Weapons[this->weapon]));
	CStreaming::RemoveAllUnusedModels();
	if (this->shouldNotDie)
		instance.add_to_queue([&]()
		{
			while (mission_started && missionPed != nullptr)
			{
				if (Command<Commands::IS_CHAR_DEAD>(missionPed))
				{
					defeat = true;
					mission_started = false;
					CMessages::ClearMessages(true);
					failMission();
				}
				this_coro::wait(0ms);
			}
		});
}

void Actor::removeMissionPed() {
	if (this->missionPed != nullptr) {
		CWorld::Remove(this->missionPed);
		this->missionPed->Remove();
		delete this->missionPed;
		this->missionPed = nullptr;
	}
}

TargetCheckpoint::TargetCheckpoint(const char* name, float x, float y, float z) {
	strcpy(this->name, name);
	this->type = 0;
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
}

TargetCheckpoint::TargetCheckpoint(const TargetCheckpoint& target) {
	strcpy(name, target.name);
	type = target.type;
	memcpy(pos, target.pos, 3 * sizeof(float));
	radius = target.radius;
	strcpy(text, target.text);
	textTime = target.textTime;
	colorBlip = target.colorBlip;
	onWhat = target.onWhat;
	comeBackVehicle = target.comeBackVehicle;
	strcpy(textComeBackVehicle, target.textComeBackVehicle);
	colorBlipComeBackVehicle = target.colorBlipComeBackVehicle;
}

TargetCar::TargetCar(const char* name) {
	strcpy(this->name, name);
	this->type = 1;
}

TargetCar::TargetCar(const TargetCar& target) {
	strcpy(name, target.name);
	type = target.type;
	car = target.car;
	strcpy(text, target.text);
	textTime = target.textTime;
	colorBlip = target.colorBlip;
}

TargetActor::TargetActor(const char* name) {
	strcpy(this->name, name);
	this->type = 2;
}

TargetActor::TargetActor(const TargetActor& target) {
	strcpy(name, target.name);
	type = target.type;
	actor = target.actor;
	strcpy(text, target.text);
	textTime = target.textTime;
	colorBlip = target.colorBlip;
	killGroup = target.killGroup;
	hit = target.hit;
}

TargetCutscene::TargetCutscene(const char* name, float x, float y, float z) {
	strcpy(this->name, name);
	this->type = 3;
	this->targetType = 0;
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
}

TargetCutscene::TargetCutscene(const TargetCutscene& target) {
	strcpy(name, target.name);
	type = target.type;
	targetType = target.targetType;
	tied = target.tied;
	tiedID = target.tiedID;
	follow = target.follow;
	followID = target.followID;
	memcpy(pos, target.pos, 3 * sizeof(float));
	memcpy(rotate, target.rotate, 3 * sizeof(int));
	strcpy(text, target.text);
	time = target.time;
	moveCam = target.moveCam;
	widescreen = target.widescreen;
}

TargetCountdown::TargetCountdown(const char* name) {
	strcpy(this->name, name);
	this->type = 3;
	this->targetType = 1;
}

TargetCountdown::TargetCountdown(const TargetCountdown& target) {
	strcpy(name, target.name);
	type = target.type;
	targetType = target.targetType;
	time = target.time;
}

TargetTimeout::TargetTimeout(const char* name) {
	strcpy(this->name, name);
	this->type = 3;
	this->targetType = 2;
}

TargetTimeout::TargetTimeout(const TargetTimeout& target) {
	strcpy(name, target.name);
	type = target.type;
	targetType = target.targetType;
	strcpy(text, target.text);
	time = target.time;
}

TargetWeather::TargetWeather(const char* name) {
	strcpy(this->name, name);
	this->type = 3;
	this->targetType = 3;
}

TargetWeather::TargetWeather(const TargetWeather& target) {
	strcpy(name, target.name);
	type = target.type;
	targetType = target.targetType;
	weather = target.weather;
}
TargetTime::TargetTime(const char* name) {
	strcpy(this->name, name);
	this->type = 3;
	this->targetType = 4;
}

TargetTime::TargetTime(const TargetTime& target) {
	strcpy(name, target.name);
	type = target.type;
	targetType = target.targetType;
	memcpy(time, target.time, 2 * sizeof(signed char));
}

TargetTraffic::TargetTraffic(const char* name) {
	strcpy(this->name, name);
	this->type = 3;
	this->targetType = 5;
}

TargetTraffic::TargetTraffic(const TargetTraffic& target) {
	strcpy(name, target.name);
	type = target.type;
	targetType = target.targetType;
	peds = target.peds;
	cars = target.cars;
}

TargetObject::TargetObject(const char* name) {
	strcpy(this->name, name);
	this->type = 4;
}

TargetObject::TargetObject(const TargetObject& target) {
	strcpy(name, target.name);
	type = target.type;
	object = target.object;
	strcpy(text, target.text);
	textTime = target.textTime;
	colorBlip = target.colorBlip;
	typeUse = target.typeUse;
	weapon = target.weapon;
}

TargetPickup::TargetPickup(const char* name) {
	strcpy(this->name, name);
	this->type = 5;
}

TargetPickup::TargetPickup(const TargetPickup& target) {
	strcpy(name, target.name);
	type = target.type;
	pickup = target.pickup;
	strcpy(text, target.text);
	textTime = target.textTime;
	colorBlip = target.colorBlip;
}

TargetTeleport::TargetTeleport(const char* name, float x, float y, float z, float angle, int interiorID) {
	strcpy(this->name, name);
	this->type = 6;
	this->targetType = 0;
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
	this->angle = angle;
	this->interiorID = interiorID;
}

TargetTeleport::TargetTeleport(const TargetTeleport& target) {
	strcpy(name, target.name);
	type = target.type;
	targetType = target.targetType;
	memcpy(pos, target.pos, 3 * sizeof(float));
	angle = target.angle;
	modelID = target.modelID;
	modelType = target.modelType;
	health = target.health;
	weapon = target.weapon;
	ammo = target.ammo;
	interiorID = target.interiorID;
}
TargetAnimation::TargetAnimation(const char* name) {
	strcpy(this->name, name);
	this->type = 6;
	this->targetType = 1;
}

TargetAnimation::TargetAnimation(const TargetAnimation& target) {
	strcpy(name, target.name);
	type = target.type;
	targetType = target.targetType;
	pack = target.pack;
	anim = target.anim;
	looped = target.looped;
	blend = target.blend;
}

TargetTeleportToVehicle::TargetTeleportToVehicle(const char* name) {
	strcpy(this->name, name);
	this->type = 6;
	this->targetType = 2;
}

TargetTeleportToVehicle::TargetTeleportToVehicle(const TargetTeleportToVehicle& target) {
	strcpy(name, target.name);
	type = target.type;
	targetType = target.targetType;
	car = target.car;
	carPlace = target.carPlace;
}

TargetLevelWanted::TargetLevelWanted(const char* name) {
	strcpy(this->name, name);
	this->type = 6;
	this->targetType = 3;
}

TargetLevelWanted::TargetLevelWanted(const TargetLevelWanted& target) {
	strcpy(name, target.name);
	type = target.type;
	targetType = target.targetType;
	levelWanted = target.levelWanted;
}

TargetRemoveWeapons::TargetRemoveWeapons(const char* name) {
	strcpy(this->name, name);
	this->type = 6;
	this->targetType = 4;
}

TargetRemoveWeapons::TargetRemoveWeapons(const TargetRemoveWeapons& target) {
	strcpy(name, target.name);
	type = target.type;
	targetType = target.targetType;
}

Dialog::Dialog(const Dialog& dialog) {
	strcpy(text, dialog.text);
	textTime = dialog.textTime;
}

TargetDialog::TargetDialog(const char* name) {
	strcpy(this->name, name);
	this->type = 6;
	this->targetType = 5;
}

TargetDialog::TargetDialog(const TargetDialog& target) {
	strcpy(name, target.name);
	type = target.type;
	targetType = target.targetType;
	dialog = target.dialog;
	dialogs = target.dialogs;
}

TargetMoney::TargetMoney(const char* name) {
	strcpy(this->name, name);
	this->type = 6;
	this->targetType = 6;
}

TargetMoney::TargetMoney(const TargetMoney& target) {
	strcpy(name, target.name);
	type = target.type;
	targetType = target.targetType;
	money = target.money;
}

TargetWaitSignal::TargetWaitSignal(const char* name) {
	strcpy(this->name, name);
	this->type = 7;
}

TargetWaitSignal::TargetWaitSignal(const TargetWaitSignal& target) {
	strcpy(name, target.name);
	type = target.type;
}

TargetDestroyVehicle::TargetDestroyVehicle(const char* name) {
	strcpy(this->name, name);
	this->type = 8;
}

TargetDestroyVehicle::TargetDestroyVehicle(const TargetDestroyVehicle& target) {
	strcpy(name, target.name);
	type = target.type;

	vehicle = target.vehicle;
	strcpy(text, target.text);
	textTime = target.textTime;
	colorBlip = target.colorBlip;
	typeDamage = target.typeDamage;
}

Car::Car(const char* name, float x, float y, float z, float angle, int lastTarget) {
	strcpy(this->name, name);
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
	this->angle = angle;
	this->startC = lastTarget;
}

Car::Car(const Car& car) {
	strcpy(this->name, car.name);
	memcpy(pos, car.pos, 3 * sizeof(float));
	angle = car.angle;
	modelID = car.modelID;
	startC = car.startC;
	endC = car.endC;
	shouldNotDie = car.shouldNotDie;
	health = car.health;
	bulletproof = car.bulletproof;
	fireproof = car.fireproof;
	explosionproof = car.explosionproof;
	collisionproof = car.collisionproof;
	meleeproof = car.meleeproof;
	tiresVulnerability = car.tiresVulnerability;
	extendedColor = car.extendedColor;
	memcpy(primaryColor, car.primaryColor, 4 * sizeof(float));
	memcpy(secondaryColor, car.secondaryColor, 4 * sizeof(float));
	locked = car.locked;
	colors = car.colors;
	component_type_A = car.component_type_A;
	component_type_B = car.component_type_B;
	useTarget = car.useTarget;
	strcpy(this->numberplate, car.numberplate);
	numberplate_city = car.numberplate_city;
	this->updateEditorCar();
}

void Car::updateEditorCar(bool recolor) {
	removeEditorCar();
	CStreaming::RequestModel(modelID, 0);
	CStreaming::LoadAllRequestedModels(false);
	Command<Commands::SET_CAR_MODEL_COMPONENTS>(modelID, component_type_A, component_type_B);
	if (std::strcmp(numberplate,"") != 0)
		Command<Commands::CUSTOM_PLATE_FOR_NEXT_CAR>(modelID, numberplate);
	Command<Commands::CUSTOM_PLATE_DESIGN_FOR_NEXT_CAR>(modelID, numberplate_city);
	switch (reinterpret_cast<CVehicleModelInfo *>(CModelInfo::ms_modelInfoPtrs[modelID])->m_nVehicleType) {
	case VEHICLE_MTRUCK:
		editorCar = new CMonsterTruck(modelID, 2);
		break;
	case VEHICLE_QUAD:
		editorCar = new CQuadBike(modelID, 2);
		break;
	case VEHICLE_HELI:
		editorCar = new CHeli(modelID, 2);
		break;
	case VEHICLE_PLANE:
		editorCar = new CPlane(modelID, 2);
		break;
	case VEHICLE_BIKE:
		editorCar = new CBike(modelID, 2);
		reinterpret_cast<CBike *>(editorCar)->m_nDamageFlags |= 0x10;
		break;
	case VEHICLE_BMX:
		editorCar = new CBmx(modelID, 2);
		reinterpret_cast<CBmx *>(editorCar)->m_nDamageFlags |= 0x10;
		break;
	case VEHICLE_TRAILER:
		editorCar = new CTrailer(modelID, 2);
		break;
	case VEHICLE_BOAT:
		editorCar = new CBoat(modelID, 2);
		break;
	default:
		editorCar = new CAutomobile(modelID, 2, true);
		break;
	}
	editorCar->SetPosn(pos[0], pos[1], pos[2]);
	editorCar->SetHeading((float)rad(angle));
	editorCar->m_bStreamingDontDelete = 1;
	CWorld::Add(editorCar);
	editorCar->m_nStatus = 4;
	editorCar->m_bUsesCollision = false;
	editorCar->m_nVehicleFlags.bEngineOn = 0;
	Command<Commands::SET_CAR_AS_MISSION_CAR>(editorCar);
	Command<Commands::FREEZE_CAR_POSITION>(editorCar, true);
	editorCar->m_nVehicleFlags.bCanBeDamaged = false;
	CStreaming::RemoveAllUnusedModels();

	if (recolor) {
		colors.clear();
		for (auto comp : components::get_all_vehicle_components(editorCar)) {
			for (auto obj : comp.getObjects()) {
				for (auto mat : obj.getMaterials()) {
					const CRGBA rgba = mat.getColor();
					CRGBA colorr;
					unsigned char typee;
					if (rgba.r == 0x3C && rgba.g == 0xFF && rgba.b == 0x00) {
						colorr.Set(get_car_color_rgba(editorCar->m_nPrimaryColor));
						primaryColor[0] = colorr.a / 255.0f;
						primaryColor[1] = colorr.b / 255.0f;
						primaryColor[2] = colorr.g / 255.0f;
						primaryColor[3] = colorr.r / 255.0f;
						typee = 0;
					} else if (rgba.r == 0xFF && rgba.g == 0x00 && rgba.b == 0xAF) {
						colorr.Set(get_car_color_rgba(editorCar->m_nSecondaryColor));
						secondaryColor[0] = colorr.a / 255.0f;
						secondaryColor[1] = colorr.b / 255.0f;
						secondaryColor[2] = colorr.g / 255.0f;
						secondaryColor[3] = colorr.r / 255.0f;
						typee = 1;
					} else {
						typee = 2;
					}
					std::array<float,4> color_rgba{rgba.r / 255.0f, rgba.g / 255.0f, rgba.b / 255.0f, rgba.a / 255.0f};
					colors.push_back(std::make_pair(typee, color_rgba));

				}
			}
		}
	} else {
		int i = 0;
		for (auto comp : components::get_all_vehicle_components(editorCar)) {
			for (auto obj : comp.getObjects()) {
				for (auto mat : obj.getMaterials()) {
					const float new_r = colors.at(i).second[0], new_g = colors.at(i).second[1], new_b = colors.at(i).second[2], new_a = colors.at(i).second[3];
					const CRGBA old = mat.getColor();
					if (!(floorf(new_r * 255) == old.r && floorf(new_g * 255) == old.g && floorf(new_b * 255) == old.b && floorf(new_a * 255) == old.a))
						mat.setColor(CRGBA(new_r * 255, new_g * 255, new_b * 255, new_a * 255));

					i++;
				}
			}
		}
	}
}

void Car::removeEditorCar() {
	if (this->editorCar != nullptr) {
		CWorld::Remove(this->editorCar);
		this->editorCar->Remove();
		delete this->editorCar;
		this->editorCar = nullptr;
	}
}

void Car::updateMissionCar()
{
	removeMissionCar();
	
	CStreaming::RequestModel(modelID, 0);
	CStreaming::LoadAllRequestedModels(false);
	Command<Commands::SET_CAR_MODEL_COMPONENTS>(modelID, component_type_A, component_type_B);
	if (std::strcmp(numberplate, "") != 0)
		Command<Commands::CUSTOM_PLATE_FOR_NEXT_CAR>(modelID, numberplate);
	Command<Commands::CUSTOM_PLATE_DESIGN_FOR_NEXT_CAR>(modelID, numberplate_city);
	switch (reinterpret_cast<CVehicleModelInfo *>(CModelInfo::ms_modelInfoPtrs[modelID])->m_nVehicleType) {
	case VEHICLE_MTRUCK:
		missionCar = new CMonsterTruck(modelID, 2);
		break;
	case VEHICLE_QUAD:
		missionCar = new CQuadBike(modelID, 2);
		break;
	case VEHICLE_HELI:
		missionCar = new CHeli(modelID, 2);
		break;
	case VEHICLE_PLANE:
		missionCar = new CPlane(modelID, 2);
		break;
	case VEHICLE_BIKE:
		missionCar = new CBike(modelID, 2);
		reinterpret_cast<CBike *>(missionCar)->m_nDamageFlags |= 0x10;
		break;
	case VEHICLE_BMX:
		missionCar = new CBmx(modelID, 2);
		reinterpret_cast<CBmx *>(missionCar)->m_nDamageFlags |= 0x10;
		break;
	case VEHICLE_TRAILER:
		missionCar = new CTrailer(modelID, 2);
		break;
	case VEHICLE_BOAT:
		missionCar = new CBoat(modelID, 2);
		break;
	default:
		missionCar = new CAutomobile(modelID, 2, true);
		break;
	}
	missionCar->SetPosn(pos[0], pos[1], pos[2]);
	missionCar->SetHeading((float)rad(angle));
	missionCar->m_nStatus = 4;
	missionCar->m_nVehicleFlags.bEngineOn = 0;
	CWorld::Add(missionCar);
	Command<Commands::SET_CAR_AS_MISSION_CAR>(missionCar);
	Command<Commands::FREEZE_CAR_POSITION>(missionCar, true);
	while (mission_started && DistanceBetweenPoints(TheCamera.GetPosition(), missionCar->GetPosition()) > 100.0f)
	{
		this_coro::wait(0ms);
	}
	Command<Commands::FREEZE_CAR_POSITION>(missionCar, false);
	missionCar->m_fHealth = static_cast<float>(health);
	missionCar->m_nDoorLock = locked ? eCarLock::CARLOCK_LOCKED : eCarLock::CARLOCK_UNLOCKED;
	missionCar->m_nPhysicalFlags.bBulletProof = static_cast<unsigned>(bulletproof);
	missionCar->m_nPhysicalFlags.bCollisionProof = static_cast<unsigned>(collisionproof);
	missionCar->m_nPhysicalFlags.bExplosionProof = static_cast<unsigned>(explosionproof);
	missionCar->m_nPhysicalFlags.bFireProof = static_cast<unsigned>(fireproof);
	missionCar->m_nPhysicalFlags.bMeeleProof = static_cast<unsigned>(meleeproof);
	missionCar->m_nVehicleFlags.bTyresDontBurst = static_cast<unsigned char>(tiresVulnerability);
	int i = 0;
	for (auto comp : components::get_all_vehicle_components(missionCar)) {
		for (auto obj : comp.getObjects()) {
			for (auto mat : obj.getMaterials()) {
				const float new_r = colors.at(i).second[0], new_g = colors.at(i).second[1], new_b = colors.at(i).second[2], new_a = colors.at(i).second[3];
				const CRGBA old = mat.getColor();
				if (!(floorf(new_r * 255) == old.r && floorf(new_g * 255) == old.g && floorf(new_b * 255) == old.b && floorf(new_a * 255) == old.a))
					mat.setColor(CRGBA(new_r * 255, new_g * 255, new_b * 255, new_a * 255));

				i++;
			}
		}
	}
	if (this->shouldNotDie)
		instance.add_to_queue([&](){
			while (mission_started && missionCar != nullptr)
			{
				if (Command<Commands::IS_CAR_DEAD>(missionCar))
				{
					defeat = true;
					mission_started = false;
					CMessages::ClearMessages(true);
					failMission();
				}
				this_coro::wait(0ms);
			}
		});
}

void Car::removeMissionCar()
{
	if (this->missionCar != nullptr) {
		CWorld::Remove(this->missionCar);
		this->missionCar->Remove();
		delete this->missionCar;
		this->missionCar = nullptr;
	}
}

Train::Train(const char* name, float x, float y, float z, int lastTarget) {
	strcpy(this->name, name);
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
	this->startC = lastTarget;
}

Train::Train(const Train& train) {
	strcpy(this->name, train.name);
	memcpy(pos, train.pos, 3 * sizeof(float));
	this->rotation =  train.rotation;
	modelID = train.modelID;
	startC = train.startC;
	endC = train.endC;
	useTarget = train.useTarget;
	speed = train.speed;
	cruise_speed = train.cruise_speed;
	this->updateEditorTrain();
}

void Train::updateEditorTrain() {
	removeEditorTrain();

	int trainModel = 0;
	int carriageModel = 0;
	switch (modelID)
	{
	case 0:
	case 3:
	case 6:
	case 10:
	case 12:
	case 13:
		trainModel = MODEL_FREIGHT;
		carriageModel = MODEL_FREIFLAT;
		break;
	case 1:
	case 2:
	case 4:
	case 5:
	case 7:
	case 11:
	case 15:
		trainModel = MODEL_STREAK;
		carriageModel = MODEL_STREAKC;
		break;
	case 8:
	case 9:
	case 14:
		trainModel = MODEL_TRAM;
		carriageModel = MODEL_TRAM;
		break;
	default: break;
	}
	CStreaming::RequestModel(trainModel, 0);
	CStreaming::RequestModel(carriageModel, 0);
	CStreaming::LoadAllRequestedModels(false);
	CStreaming::LoadRequestedModels();
	this_coro::wait(100);
	
	int trainHandle;
	Command<Commands::CREATE_MISSION_TRAIN>(modelID, pos[0], pos[1], pos[2], static_cast<int>(rotation), &trainHandle);
	editorTrain = static_cast<CTrain*>(CPools::GetVehicle(trainHandle));
	editorTrain->m_bStreamingDontDelete = 1;
	editorTrain->m_nStatus = 4;
	editorTrain->m_nDoorLock = eCarLock::CARLOCK_LOCKED;
	editorTrain->m_nVehicleFlags.bEngineOn = 0;
	Command<Commands::SET_CAR_AS_MISSION_CAR>(editorTrain);
	Command<Commands::FREEZE_CAR_POSITION>(editorTrain, true);
	editorTrain->m_nVehicleFlags.bCanBeDamaged = false;
	CStreaming::RemoveAllUnusedModels();
}

void Train::removeEditorTrain() {
	if (this->editorTrain != nullptr) {
		Command<Commands::DELETE_MISSION_TRAIN>(CPools::GetVehicleRef(editorTrain));
		this->editorTrain = nullptr;
	}
}

void Train::updateMissionTrain()
{
	removeMissionTrain();
	
	int trainModel = 0;
	int carriageModel = 0;
	switch (modelID)
	{
	case 0:
	case 3:
	case 6:
	case 10:
	case 12:
	case 13:
		trainModel = MODEL_FREIGHT;
		carriageModel = MODEL_FREIFLAT;
		break;
	case 1:
	case 2:
	case 4:
	case 5:
	case 7:
	case 11:
	case 15:
		trainModel = MODEL_STREAK;
		carriageModel = MODEL_STREAKC;
		break;
	case 8:
	case 9:
	case 14:
		trainModel = MODEL_TRAM;
		carriageModel = MODEL_TRAM;
		break;
	default: break;
	}
	CStreaming::RequestModel(trainModel, 0);
	CStreaming::RequestModel(carriageModel, 0);
	CStreaming::LoadAllRequestedModels(false);
	CStreaming::LoadRequestedModels();
	this_coro::wait(0);

	int trainHandle;
	Command<Commands::CREATE_MISSION_TRAIN>(modelID, pos[0], pos[1], pos[2], static_cast<int>(rotation), &trainHandle);
	missionTrain = static_cast<CTrain*>(CPools::GetVehicle(trainHandle));
	missionTrain->m_bStreamingDontDelete = 1;
	missionTrain->m_nStatus = 4;
	missionTrain->m_nVehicleFlags.bEngineOn = 0;
	Command<Commands::SET_CAR_AS_MISSION_CAR>(missionTrain);
	missionTrain->m_nVehicleFlags.bCanBeDamaged = false;
	CStreaming::RemoveAllUnusedModels();
	Command<Commands::FREEZE_CAR_POSITION>(missionTrain, true);
	while (mission_started && DistanceBetweenPoints(TheCamera.GetPosition(), missionTrain->GetPosition()) > 100.0f)
	{
		this_coro::wait(0ms);
	}
	Command<Commands::FREEZE_CAR_POSITION>(missionTrain, false);
	Command<Commands::SET_TRAIN_CRUISE_SPEED>(trainHandle, cruise_speed);
	Command<Commands::SET_TRAIN_SPEED>(trainHandle, speed);
}

void Train::removeMissionTrain()
{
	if (this->missionTrain != nullptr) {
		Command<Commands::DELETE_MISSION_TRAIN>(CPools::GetVehicleRef(missionTrain));
		this->missionTrain = nullptr;
	}
}

Object::Object(const char* name, float x, float y, float z, int lastTarget) {
	strcpy(this->name, name);
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
	this->startC = lastTarget;
}

Object::Object(const Object& object) {
	strcpy(this->name, object.name);
	memcpy(pos, object.pos, 3 * sizeof(float));
	memcpy(this->rotation, object.rotation, 3 * sizeof(int));
	modelID = object.modelID;
	startC = object.startC;
	endC = object.endC;
	useTarget = object.useTarget;
	this->updateEditorObject();
}

void Object::updateEditorObject() {
	removeEditorObject();
	CStreaming::RequestModel(modelID, 0);
	CStreaming::LoadAllRequestedModels(false);

	editorObject = CObject::Create(modelID);
	CStreaming::RemoveAllUnusedModels();
	editorObject->SetPosn(pos[0], pos[1], pos[2]);
	editorObject->SetOrientation(rad(rotation[0]), rad(rotation[1]), rad(rotation[2]));
	editorObject->m_nObjectType = eObjectType::OBJECT_MISSION;
	CWorld::Add(editorObject);
	Command<Commands::SET_OBJECT_DYNAMIC>(editorObject, 0);
}

void Object::removeEditorObject() {
	if (this->editorObject != nullptr) {
		CWorld::Remove(this->editorObject);
		this->editorObject->Remove();
		delete this->editorObject;
		this->editorObject = nullptr;
	}
}

void Object::updateMissionObject()
{
	removeMissionObject();
	
	CStreaming::RequestModel(modelID, 0);
	CStreaming::LoadAllRequestedModels(false);

	missionObject = CObject::Create(modelID);
	CStreaming::RemoveAllUnusedModels();
	missionObject->SetPosn(pos[0], pos[1], pos[2]);
	missionObject->SetOrientation(rad(rotation[0]), rad(rotation[1]), rad(rotation[2]));
	CWorld::Add(missionObject);
	missionObject->m_nObjectType = eObjectType::OBJECT_MISSION;
	Command<0x0550>(missionObject, 1);
	Command<Commands::SET_OBJECT_DYNAMIC>(missionObject, 0);
}

void Object::removeMissionObject()
{
	if (this->missionObject != nullptr) {
		CWorld::Remove(this->missionObject);
		this->missionObject->Remove();
		delete this->missionObject;
		this->missionObject = nullptr;
	}
}

Particle::Particle(const char* name, float x, float y, float z, int lastTarget) {
	strcpy(this->name, name);
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
	this->startC = lastTarget;
}

Particle::Particle(const Particle& particle) {
	strcpy(this->name, particle.name);
	memcpy(pos, particle.pos, 3 * sizeof(float));
	memcpy(this->rotation, particle.rotation, 3 * sizeof(int));
	modelID = particle.modelID;
	startC = particle.startC;
	endC = particle.endC;
	useTarget = particle.useTarget;
	tied = particle.tied;
	tiedID = particle.tiedID;
	this->updateEditorParticle();
}

void Particle::updateEditorParticle() {
	removeEditorParticle();
	CStreaming::RequestModel(327, 0);
	CStreaming::LoadAllRequestedModels(false);

	editorParticle.second = CObject::Create(327);
	editorParticle.second->m_nObjectType = eObjectType::OBJECT_MISSION;
	const std::string* modell = &Particle_name[modelID];
	Command<Commands::CREATE_FX_SYSTEM_ON_OBJECT_WITH_DIRECTION>(modell->c_str(), editorParticle.second, 0.0f, 0.0f, 0.0f, (float)rotation[0], (float)rotation[1], (float)rotation[2], 1, &editorParticle.first);
	Command<Commands::PLAY_FX_SYSTEM>(editorParticle.first);
	editorParticle.second->m_bIsVisible = false;
	editorParticle.second->SetPosn(pos[0], pos[1], pos[2]);
	//Command<Commands::SET_OBJECT_ROTATION>(editorParticle.second, (float)rotation[0], (float)rotation[1], (float)rotation[2]);
	CStreaming::RemoveAllUnusedModels();
	CWorld::Add(editorParticle.second);
}

void Particle::removeEditorParticle() {
	if (this->editorParticle.first != NULL) {
		CWorld::Remove(this->editorParticle.second);
		this->editorParticle.second->Remove();
		Command<Commands::KILL_FX_SYSTEM>(this->editorParticle.first);
		delete this->editorParticle.second;
		this->editorParticle.first = NULL;
		this->editorParticle.second = nullptr;
	}
}

void Particle::updateMissionParticle(void* void_mission) {
	removeMissionParticle();
	
	Mission* mission = static_cast<Mission*>(void_mission);
	CStreaming::RequestModel(327, 0);
	CStreaming::LoadAllRequestedModels(false);

	const std::string* modell = &Particle_name[modelID];

	if (tied == 0)
	{
		missionParticle.second = CObject::Create(327);
		missionParticle.second->m_nObjectType = eObjectType::OBJECT_MISSION;
		Command<Commands::CREATE_FX_SYSTEM_ON_OBJECT_WITH_DIRECTION>(modell->c_str(), missionParticle.second, 0, 0, 0, (float)rotation[0], (float)rotation[1], (float)rotation[2], 1, &missionParticle.first);
		Command<Commands::PLAY_FX_SYSTEM>(missionParticle.first);
		missionParticle.second->m_bIsVisible = false;
		missionParticle.second->SetPosn(pos[0], pos[1], pos[2]);
		//Command<Commands::SET_OBJECT_ROTATION>(missionParticle.second, (float)rotation[0], (float)rotation[1], (float)rotation[2]);
		CStreaming::RemoveAllUnusedModels();
		CWorld::Add(missionParticle.second);
	}
	else if (tied == 1)
	{
		CVector& tiedPos = mission->list_actors[tiedID]->missionPed->GetPosition();
		float angleTied = static_cast<float>(deg(mission->list_actors[tiedID]->missionPed->GetHeading()));
		float xx = pos[0] - tiedPos.x, xy = pos[1] - tiedPos.y, xz = pos[2] - tiedPos.z;
		rotateVec2(xx, xy, -angleTied);
		Command<Commands::CREATE_FX_SYSTEM_ON_CHAR_WITH_DIRECTION>(modell->c_str(), mission->list_actors[tiedID]->missionPed, xx, xy, xz, (float)rotation[0], (float)rotation[1] - angleTied, (float)rotation[2], 1, &missionParticle.first);
		Command<Commands::PLAY_FX_SYSTEM>(missionParticle.first);
	}
	else if (tied == 2)
	{
		CVector& tiedPos = mission->list_cars[tiedID]->missionCar->GetPosition();
		float angleTied = static_cast<float>(deg(mission->list_cars[tiedID]->missionCar->GetHeading()));
		float xx = pos[0] - tiedPos.x, xy = pos[1] - tiedPos.y, xz = pos[2] - tiedPos.z;
		rotateVec2(xx, xy, -angleTied);
		Command<Commands::CREATE_FX_SYSTEM_ON_CAR_WITH_DIRECTION>(modell->c_str(), mission->list_cars[tiedID]->missionCar, xx, xy, xz, (float)rotation[0], (float)rotation[1] - angleTied, (float)rotation[2], 1, &missionParticle.first);
		Command<Commands::PLAY_FX_SYSTEM>(missionParticle.first);
	}
	else if (tied == 3)
	{
		CVector& tiedPos = mission->list_objects[tiedID]->missionObject->GetPosition();
		float angleTied = static_cast<float>(deg(mission->list_objects[tiedID]->missionObject->GetHeading()));
		float xx = pos[0] - tiedPos.x, xy = pos[1] - tiedPos.y, xz = pos[2] - tiedPos.z;
		rotateVec2(xx, xy, -angleTied);
		Command<Commands::CREATE_FX_SYSTEM_ON_OBJECT_WITH_DIRECTION>(modell->c_str(), mission->list_objects[tiedID]->missionObject, xx, xy, xz, (float)rotation[0], (float)rotation[1] - angleTied, (float)rotation[2], 1, &missionParticle.first);
		Command<Commands::PLAY_FX_SYSTEM>(missionParticle.first);
	}
}

void Particle::removeMissionParticle() {
	if (this->missionParticle.first != NULL) {
		if (this->tied == 0) {
			CWorld::Remove(this->missionParticle.second);
			this->missionParticle.second->Remove();
		}
		Command<Commands::KILL_FX_SYSTEM>(this->missionParticle.first);
		delete this->missionParticle.second;
		this->missionParticle.first = NULL;
		this->missionParticle.second = nullptr;
	}
}

Pickup::Pickup(const char* name, float x, float y, float z, int lastTarget) {
	strcpy(this->name, name);
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
	this->startC = lastTarget;
}

Pickup::Pickup(const Pickup& particle) {
	strcpy(this->name, particle.name);
	memcpy(pos, particle.pos, 3 * sizeof(float));
	modelID = particle.modelID;
	type = particle.type;
	spawnType = particle.spawnType;
	weapon = particle.weapon;
	ammo = particle.ammo;
	startC = particle.startC;
	endC = particle.endC;
	useTarget = particle.useTarget;
	this->updateEditorPickup();
}

void Pickup::updateEditorPickup() {
	removeEditorPickup();

	if (type == 0) {
		int weap_modell;
		Command<Commands::GET_WEAPONTYPE_MODEL>(ID_Weapons[this->weapon], &weap_modell);
		if (weap_modell != -1) {
			CStreaming::RequestModel(weap_modell, 0);
			CStreaming::LoadAllRequestedModels(false);
		}
		editorPickup = CPickups::GenerateNewOne_WeaponType(CVector(pos[0], pos[1], pos[2]), (eWeaponType)ID_Weapons[this->weapon], 9, ammo, false, nullptr);
	} 
	else {
		int md;
		switch (type)
		{
		case 1:
			md = 1240;
			break;
		case 2:
			md = 1242;
			break;
		case 3:
			md = 1247;
			break;
		case 4:
			md = 1241;
			break;
		default:
			md = modelID;
		}
		
		CStreaming::RequestModel(md, 0);
		CStreaming::LoadAllRequestedModels(false);

		editorPickup = CPickups::GenerateNewOne(CVector(pos[0], pos[1], pos[2]), md, 9, 0, 0, false, nullptr);
	}
	CStreaming::RemoveAllUnusedModels();
}

void Pickup::removeEditorPickup() {
	if (this->editorPickup != NULL) {
		CPickups::RemovePickUp(this->editorPickup);
		this->editorPickup = NULL;
	}
}

void Pickup::updateMissionPickup() {
	removeMissionPickup();
	
	int spawn_t = 3;
	if (spawnType == 1)
		spawn_t = 2;
	else if (spawnType == 2)
		spawn_t = 15;

	
	
	if (type == 0) {
		int weap_modell;
		Command<Commands::GET_WEAPONTYPE_MODEL>(ID_Weapons[this->weapon], &weap_modell);
		if (weap_modell != -1) {
			CStreaming::RequestModel(weap_modell, 0);
			CStreaming::LoadAllRequestedModels(false);
		}
		missionPickup = CPickups::GenerateNewOne_WeaponType(CVector(pos[0], pos[1], pos[2]), (eWeaponType)ID_Weapons[this->weapon], spawn_t, ammo, false, nullptr);
	}
	else {
		int md;
		switch (type)
		{
		case 1:
			md = 1240;
			break;
		case 2:
			md = 1242;
			break;
		case 3:
			md = 1247;
			break;
		case 4:
			md = 1241;
			break;
		default:
			md = modelID;
		}

		CStreaming::RequestModel(md, 0);
		CStreaming::LoadAllRequestedModels(false);

		missionPickup = CPickups::GenerateNewOne(CVector(pos[0], pos[1], pos[2]), md, spawn_t, ammo, 0, false, nullptr);
	}
	CStreaming::RemoveAllUnusedModels();
}

void Pickup::removeMissionPickup() {
	if (this->missionPickup != NULL) {
		CPickups::RemovePickUp(this->missionPickup);
		this->missionPickup = NULL;
	}
}

Explosion::Explosion(const char* name, float x, float y, float z, int lastTarget) {
	strcpy(this->name, name);
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
	this->startC = lastTarget;
}

Explosion::Explosion(const Explosion& explosion) {
	strcpy(this->name, explosion.name);
	memcpy(pos, explosion.pos, 3 * sizeof(float));
	type = explosion.type;
	typeExplosion = explosion.typeExplosion;
	sizeFire = explosion.sizeFire;
	propagationFire = explosion.propagationFire;
	startC = explosion.startC;
	endC = explosion.endC;
	useTarget = explosion.useTarget;
	this->updateEditorExplosion();
}

void Explosion::updateEditorExplosion() {
	removeEditorExplosion();

	if (type == 0) {
		Command<Commands::START_SCRIPT_FIRE>(pos[0], pos[1], pos[2], 0, sizeFire, &editorFire);
	}
	else {
		CStreaming::RequestModel(1654, 0);
		CStreaming::LoadAllRequestedModels(false);

		editorExplosion = CObject::Create(1654);
		editorExplosion->SetPosn(pos[0], pos[1], pos[2]);
		editorExplosion->m_bUsesCollision = false;
		CWorld::Add(editorExplosion);
		
		CStreaming::RemoveAllUnusedModels();
	}
}

void Explosion::removeEditorExplosion() {
	if (this->editorExplosion != nullptr) {
		CWorld::Remove(this->editorExplosion);
		this->editorExplosion->Remove();
		delete this->editorExplosion;
		this->editorExplosion = nullptr;
	}
	if (this->editorFire != NULL) {
		Command<Commands::REMOVE_SCRIPT_FIRE>(this->editorFire);
		this->editorFire = NULL;
	}
}

void Explosion::updateMissionExplosion() {
	removeMissionExplosion();
	
	if (type == 0) {
		Command<Commands::START_SCRIPT_FIRE>(pos[0], pos[1], pos[2], (int)propagationFire, sizeFire, &missionFire);
	}
	else {
		Command<Commands::ADD_EXPLOSION>(pos[0], pos[1], pos[2], typeExplosion);
	}
}

void Explosion::removeMissionExplosion() {
	if (this->missionFire != NULL) {
		Command<Commands::REMOVE_SCRIPT_FIRE>(this->missionFire);
		this->missionFire = NULL;
	}
}

Audio::Audio(const char* name, float x, float y, float z, int lastTarget) {
	strcpy(this->name, name);
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
	this->startC = lastTarget;
}

Audio::Audio(const Audio& explosion) {
	strcpy(this->name, explosion.name);
	memcpy(pos, explosion.pos, 3 * sizeof(float));
	sound = explosion.sound;
	audio3D = explosion.audio3D;
	audio3DType = explosion.audio3DType;
	audio3DAttach = explosion.audio3DAttach;
	repeat = explosion.repeat;
	startC = explosion.startC;
	endC = explosion.endC;
	useTarget = explosion.useTarget;
	this->updateEditorAudio();
}

void Audio::updateEditorAudio(bool _new) {
	removeEditorAudio();

	if (this->audio3D && this->audio3DType == 0) {

		if (_new) {
			const CVector playerPos = playerPed->GetPosition();
			this->pos[0] = playerPos.x;
			this->pos[1] = playerPos.y;
			this->pos[2] = playerPos.z;
		}
		CStreaming::RequestModel(2231, 0);
		CStreaming::LoadAllRequestedModels(false);

		this->object = CObject::Create(2231);
		this->object->SetPosn(this->pos[0], this->pos[1], this->pos[2]);
		this->object->m_bUsesCollision = false;

		CWorld::Add(this->object);
	}
}

void Audio::loadAudiosList() {
	std::stringstream path;
	path << "LDYOM//Missions_packs//" << replace_symb(UTF8_to_CP1251(*nameCurrPack)) << "//audio";
	//path << "LDYOM//Missions_packs//" << "test" << "//audio";
	namesAudioFiles = get_filename_list(path.str(), ".mp3");
}

void Audio::loadAudio()
{
	if (namesAudioFiles.empty())
		return;

	std::stringstream path;
	path << "LDYOM//Missions_packs//" << replace_symb(UTF8_to_CP1251(*nameCurrPack)) << "//audio//" << namesAudioFiles[sound] << ".mp3";

	if (audio3D) {
		Command<0x0AC1>(path.str().c_str(), &missionAudio); //load3dAudioStream
	}
	else {
		Command<0x0AAC>(path.str().c_str(), &missionAudio); //loadAudioStream
	}
	if (repeat)
		Command<0x0AC0>(missionAudio, 1);
}

void Audio::unloadAudio() {
	if (namesAudioFiles.empty())
		return;
	Command<0x0AAE>(missionAudio);
}

void Audio::play(Mission* mission) {
	
	if (namesAudioFiles.empty())
		return;

	this_coro::wait(0);
	
	int state;
	Command<0x0AB9>(missionAudio, &state);
	if (state == 1)
	{
		return;
	}
	
	if (audio3D)
	{
		switch (audio3DType)
		{
		case 0:
			Command<0x0AC2>(missionAudio, pos[0], pos[1], pos[2]);
			break;
		case 1:
			Command<0x0AC4>(missionAudio, mission->list_actors[audio3DAttach]->missionPed);
			break;
		case 2:
			Command<0x0AC5>(missionAudio, mission->list_cars[audio3DAttach]->missionCar);
			break;
		case 3:
			Command<0x0AC3>(missionAudio, mission->list_objects[audio3DAttach]->missionObject);
			break;
		default:
			break;
		}
	}
	Command<0x0AAD>(missionAudio, 1);
}

void Audio::stop()
{
	if (namesAudioFiles.empty())
		return;
	
	Command<0x0AAD>(missionAudio, 0);
}

void Audio::removeEditorAudio() {
	if (this->object != nullptr) {
		CWorld::Remove(this->object);
		this->object->Remove();
		delete this->object;
		this->object = nullptr;
	}
}

Player::Player(float x, float y, float z, float angle) {
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
	this->angle = angle;
	Command<Commands::GET_AREA_VISIBLE>(&this->interiorID);
	printLog("pl test");
}

Player::Player(const Player& player) {
	memcpy(pos, player.pos, 3 * sizeof(float));
	angle = player.angle;
	modelID = player.modelID;
	modelType = player.modelType;
	health = player.health;
	weapon = player.weapon;
	ammo = player.ammo;
	interiorID = player.interiorID;
}

void Player::updateEditorPed() {
	this->removeEditorPed();
	int modell;
	if (this->modelType == 0) {
		modell = this->modelID;
		CStreaming::RequestModel(modell, 0);
		CStreaming::LoadAllRequestedModels(false);
	}
	else if (this->modelType == 1) {
		const std::string modell_n = ID_Spec_Actors[this->modelID];
		CStreaming::RequestSpecialChar(8, modell_n.c_str(), 0);
		CStreaming::LoadAllRequestedModels(false);
		modell = 290 + 9 - 1;
	}
	this->editorPed = new CCivilianPed(static_cast<ePedType>(4), modell);
	this->editorPed->SetPosn(this->pos[0], this->pos[1], this->pos[2]);
	this->editorPed->SetHeading((float)rad(this->angle));
	this->editorPed->m_bUsesCollision = false;
	this->editorPed->m_nCreatedBy = 2;
	CWorld::Add(this->editorPed);
	int weap_modell;
	Command<Commands::GET_WEAPONTYPE_MODEL>(ID_Weapons[this->weapon], &weap_modell);
	if (weap_modell != -1) {
		CStreaming::RequestModel(weap_modell, 0);
		CStreaming::LoadAllRequestedModels(false);
	}
	this->editorPed->GiveWeapon(static_cast<eWeaponType>(ID_Weapons[this->weapon]), ammo, false);
	this->editorPed->SetCurrentWeapon(static_cast<eWeaponType>(ID_Weapons[this->weapon]));
	CStreaming::RemoveAllUnusedModels();
}

void Player::removeEditorPed() {
	if (this->editorPed != nullptr) {
		CWorld::Remove(this->editorPed);
		this->editorPed->Remove();
		delete this->editorPed;
		this->editorPed = nullptr;
	}
}

Mission::Mission() {
	name = langt("newMiss");
	name.append("_");
	name.append(std::to_string(std::time(nullptr)));
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 9; j++) {
			if (i + 1 != j) {
				groupRelations[i][j] = 2;
			}
			else {
				groupRelations[i][j] = -1;
			}
		}
	}
}

Mission::~Mission() {
	for (auto v : this->list_targets) {
		delete v;
	}
	for (auto v : this->list_actors) {
		delete v;
	}
	for (auto v : this->list_cars) {
		delete v;
	}
	for (auto v : this->list_trains) {
		delete v;
	}
	for (auto v : this->list_objects) {
		delete v;
	}
	for (auto v : this->list_particles) {
		delete v;
	}
	for (auto v : this->list_pickups) {
		delete v;
	}
	for (auto v : this->list_explosions) {
		delete v;
	}
	for (auto v : this->list_audios) {
		delete v;
	}
	printLog("miss delete");
}

void Mission::removeEditorEntity()
{
	for (auto i : list_actors)
	{
		i->removeEditorPed();
	}
	for (auto i : list_cars)
	{
		i->removeEditorCar();
	}
	for (auto i : list_trains)
	{
		i->removeEditorTrain();
	}
	for (auto i : list_objects)
	{
		i->removeEditorObject();
	}
	for (auto i : list_pickups)
	{
		i->removeEditorPickup();
	}
	for (auto i : list_particles)
	{
		i->removeEditorParticle();
	}
	for (auto i : list_explosions)
	{
		i->removeEditorExplosion();
	}
	for (auto i : list_audios)
	{
		i->removeEditorAudio();
	}
	this->player.removeEditorPed();
}

void Mission::updateEditorEntity()
{
	updateSphere = true;
	for (auto i : list_actors)
	{
		i->updateEditorPed();
	}
	for (auto i : list_cars)
	{
		i->updateEditorCar();
	}
	for (auto i : list_trains)
	{
		i->updateEditorTrain();
	}
	for (auto i : list_objects)
	{
		i->updateEditorObject();
	}
	for (auto i : list_pickups)
	{
		i->updateEditorPickup();
	}
	for (auto i : list_particles)
	{
		i->updateEditorParticle();
	}
	for (auto i : list_explosions)
	{
		i->updateEditorExplosion();
	}
	for (auto i : list_audios)
	{
		i->updateEditorAudio();
	}
	player.updateEditorPed();
}

void Mission::removeMissionEntity()
{
	for (auto i : list_actors)
	{
		i->removeMissionPed();
	}
	for (auto i : list_cars)
	{
		i->removeMissionCar();
	}
	for (auto i : list_trains)
	{
		i->removeMissionTrain();
	}
	for (auto i : list_objects)
	{
		i->removeMissionObject();
	}
	for (auto i : list_pickups)
	{
		i->removeMissionPickup();
	}
	for (auto i : list_particles)
	{
		i->removeMissionParticle();
	}
	for (auto i : list_explosions)
	{
		i->removeMissionExplosion();
	}
}

CheckpointStoryline::CheckpointStoryline(const char* name, float x, float y, float z)
{
	strcpy(this->name, name);
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
}

CheckpointStoryline::CheckpointStoryline(const CheckpointStoryline& checkpoint)
{
	strcpy(this->name, checkpoint.name);
	memcpy(pos, checkpoint.pos, 3 * sizeof(float));
	colorBlip = checkpoint.colorBlip;
	iconMarker = checkpoint.iconMarker;
	indexSphere = checkpoint.indexSphere;
	useMission = checkpoint.useMission;
	startC = checkpoint.startC;
	gotoMission = checkpoint.gotoMission;
	timeStart = checkpoint.timeStart;
}

void CheckpointStoryline::updateEditorCheckpoint()
{
	this->removeEditorCheckpoint();
	Command<Commands::ADD_SPRITE_BLIP_FOR_CONTACT_POINT>(pos[0], pos[1], pos[2], iconMarker, &marker);
	Command<Commands::CHANGE_BLIP_SCALE>(marker, 3);
	if (colorBlip > 0)
	{
		Command<Commands::CHANGE_BLIP_DISPLAY>(marker, 3);
		CRadar::ChangeBlipColour(marker, this->colorBlip - 1);
	} else
		Command<Commands::CHANGE_BLIP_DISPLAY>(marker, 1);
}

void CheckpointStoryline::removeEditorCheckpoint()
{
	if (this->marker != NULL)
	{
		CRadar::ClearBlip(marker);
		marker = NULL;
	}
}

Storyline::Storyline(std::string& missPack)
{
	this->missPack = missPack;
}

Storyline::~Storyline()
{
	for (auto v : this->list_checkpoints) {
		delete v;
	}
}

void Storyline::updateEditorEntity()
{
	for (auto checkpoint_storyline : this->list_checkpoints)
	{
		checkpoint_storyline->updateEditorCheckpoint();
	}
}

void Storyline::removeEditorEntity()
{
	for (auto checkpoint_storyline : this->list_checkpoints)
	{
		checkpoint_storyline->removeEditorCheckpoint();
	}
}

extern vector<std::string> namesMissionPacks;
extern vector<vector<std::string>> namesMissions;
extern Storyline* currentStorylinePtr;

vector<std::string> getStorylineMissionsNames()
{
	int pack_idx = 0;
	for (int i = 0; i < namesMissionPacks.size(); ++i)
	{
		if (namesMissionPacks[i] == currentStorylinePtr->missPack)
		{
			pack_idx = i;
			break;
		}
	}
	return namesMissions[pack_idx];
}

void addLDYOMClasses(sol::state& lua)
{
	auto mission_class = lua.new_usertype<Mission>("Mission", sol::no_constructor);

	auto target_class = lua.new_usertype<Target>("Target", sol::no_constructor);
	target_class["type"] = &Target::type;
	target_class["name"] = &Target::name;
	target_class["targetType"] = &Target::targetType;

	auto actor_class = lua.new_usertype<Actor>("Actor", sol::no_constructor);
	actor_class["updateMissionPed"] = &Actor::updateMissionPed;
	actor_class["removeMissionPed"] = &Actor::removeMissionPed;
	actor_class["missionPed"] = &Actor::missionPed;
	actor_class["editorPed"] = &Actor::editorPed;

	auto car_class = lua.new_usertype<Car>("Car", sol::no_constructor);
	car_class["updateMissionCar"] = &Car::updateMissionCar;
	car_class["removeMissionCar"] = &Car::removeMissionCar;
	car_class["missionCar"] = &Car::missionCar;
	car_class["editorCar"] = &Car::editorCar;
	car_class["pos"] = &Car::pos;
	car_class["angle"] = &Car::angle;

	auto train_class = lua.new_usertype<Train>("Train", sol::no_constructor);
	train_class["updateMissionTrain"] = &Train::updateMissionTrain;
	train_class["removeMissionTrain"] = &Train::removeMissionTrain;
	train_class["missionTrain"] = &Train::missionTrain;
	train_class["editorTrain"] = &Train::editorTrain;
	
	auto object_class = lua.new_usertype<Object>("Object", sol::no_constructor);
	object_class["updateMissionObject"] = &Object::updateMissionObject;
	object_class["removeMissionObject"] = &Object::removeMissionObject;
	object_class["missionObject"] = &Object::missionObject;
	object_class["editorObject"] = &Object::editorObject;

	auto particle_class = lua.new_usertype<Particle>("Particle", sol::no_constructor);
	particle_class["updateMissionParticle"] = &Particle::updateMissionParticle;
	particle_class["removeMissionParticle"] = &Particle::removeMissionParticle;
	particle_class["missionParticle"] = &Particle::missionParticle;
	particle_class["editorParticle"] = &Particle::editorParticle;

	auto pickup_class = lua.new_usertype<Pickup>("Pickup", sol::no_constructor);
	pickup_class["updateMissionPickup"] = &Pickup::updateMissionPickup;
	pickup_class["removeMissionPickup"] = &Pickup::removeMissionPickup;
	pickup_class["missionPickup"] = &Pickup::missionPickup;
	pickup_class["editorPickup"] = &Pickup::editorPickup;

	auto explosion_class = lua.new_usertype<Explosion>("Explosion", sol::no_constructor);
	explosion_class["updateMissionExplosion"] = &Explosion::updateMissionExplosion;
	explosion_class["removeMissionExplosion"] = &Explosion::removeMissionExplosion;
	explosion_class["missionFire"] = &Explosion::missionFire;
	explosion_class["editorExplosion"] = &Explosion::editorExplosion;
	explosion_class["editorFire"] = &Explosion::editorFire;

	auto audio_class = lua.new_usertype<Audio>("Audio", sol::no_constructor);
	audio_class["play"] = &Audio::play;
	audio_class["stop"] = &Audio::stop;
	audio_class["missionAudio"] = &Audio::missionAudio;

	mission_class["list_targets"] = &Mission::list_targets;
	mission_class["list_actors"] = &Mission::list_actors;
	mission_class["list_cars"] = &Mission::list_cars;
	mission_class["list_trains"] = &Mission::list_trains;
	mission_class["list_objects"] = &Mission::list_objects;
	mission_class["list_particles"] = &Mission::list_particles;
	mission_class["list_pickups"] = &Mission::list_pickups;
	mission_class["list_explosions"] = &Mission::list_explosions;
	mission_class["list_audios"] = &Mission::list_audios;


	auto storyline_class = lua.new_usertype<Storyline>("Storyline", sol::no_constructor);

	auto checkpointStoryline_class = lua.new_usertype<CheckpointStoryline>("CheckpointStoryline", sol::no_constructor);
	checkpointStoryline_class["activate"] = &CheckpointStoryline::activate;
	
	storyline_class.set("list_checkpoints", &Storyline::list_checkpoints);

	auto cvector_class = lua.new_usertype<CVector>("CVector", sol::no_constructor);
	cvector_class["x"] = &CVector::x;
	cvector_class["y"] = &CVector::y;
	cvector_class["z"] = &CVector::z;
}
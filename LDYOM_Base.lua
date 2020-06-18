script_authors('SKIC','SIZZZ')
script_version('Beta 0.7.0')
require 'libstd.deps' {
    'fyp:mimgui',
    'kikito:middleclass',
 }

imgui = require 'mimgui'
new = imgui.new
bitser = require 'ldyom.bitser'
class = require 'middleclass'
nodes = require 'ldyom.nodes'
ffi = require 'ffi'
inicfg = require 'inicfg'
vkeys = require 'vkeys'
mimgui_addons = require "ldyom.mimgui_addons"
lfs = require 'ldyom.lfs_ffi'
faicons = require 'ldyom.fAwesome5'
carSelector = require 'ldyom.carSelector'
mad = require 'MoonAdditions'
local memory = require 'memory'
encoding = require 'encoding'
require 'ldyom.TextToGTX'
local mp = require 'ldyom.mission_player'
cyr = encoding.CP1251
encoding.default = 'UTF-8'

local pedsSkinAtlas
local weaponsAtlas

ID_Actors = {0, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288}

ID_Spec_Actors = {'andre','bbthin','bb','cat','cesar','claude',"dwayne",'emmet','forelli','janitor','jethro','jizzy','hern','kendl','maccer','maddogg','ogloc','paul','pulaski','rose','ryder','ryder3','sindaco','smoke','smokev','suzie','sweet','tbone','tenpen','torino','truthy','wuzimu','zero','gangrl2','copgrl1', 'copgrl2','crogrl1', 'crogrl2','gungrl1','gungrl2','mecgrl2','nurgrl2','ryder2','cdeput','sfpdm1','lvpdm1','csbmydj','psycho','csmech','csomost','wmycd2'}

ID_Cars = {400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611}

Anims = {
	['Anim_name'] = {'PED','BOMBER','POOL','ON_LOOKERS','GANGS','PAULNMAC','BOX','Attractors','BAR','BASEBALL','BD_FIRE','BEACH','benchpress','BLOWJOBZ','CAMERA','CAR','CAR_CHAT','CASINO','COP_AMBIENT','CRACK','CRIB','DAM_JUMP','DANCING','DEALER','DODGE','FOOD','Freeweights','GFUNK','HEIST9','INT_HOUSE','INT_OFFICE','INT_SHOP','KISSING','LOWRIDER','MEDIC','MISC','OTB','PARK','PLAYIDLES','POLICE','RAPPING','RIOT','ROB_BANK','RUNNINGMAN','SCRATCHING','SEX','SHOP','SMOKING','SnM','STRIP','SUNBATHE','SWAT','VENDING','WOP'},
	['Anim_list'] = {{'abseil','ARRESTgun','ATM','BIKE_elbowL','BIKE_elbowR','BIKE_fallR','BIKE_fall_off','BIKE_pickupL','BIKE_pickupR','BIKE_pullupL','BIKE_pullupR','bomber','CAR_alignHI_LHS','CAR_alignHI_RHS','CAR_align_LHS','CAR_align_RHS','CAR_closedoorL_LHS','CAR_closedoorL_RHS','CAR_closedoor_LHS','CAR_closedoor_RHS','CAR_close_LHS','CAR_close_RHS','CAR_crawloutRHS','CAR_dead_LHS','CAR_dead_RHS','CAR_doorlocked_LHS','CAR_doorlocked_RHS','CAR_fallout_LHS','CAR_fallout_RHS','CAR_getinL_LHS','CAR_getinL_RHS','CAR_getin_LHS','CAR_getin_RHS','CAR_getoutL_LHS','CAR_getoutL_RHS','CAR_getout_LHS','CAR_getout_RHS','car_hookertalk','CAR_jackedLHS','CAR_jackedRHS','CAR_jumpin_LHS','CAR_LB','CAR_LB_pro','CAR_LB_weak','CAR_LjackedLHS','CAR_LjackedRHS','CAR_Lshuffle_RHS','CAR_Lsit','CAR_open_LHS','CAR_open_RHS','CAR_pulloutL_LHS','CAR_pulloutL_RHS','CAR_pullout_LHS','CAR_pullout_RHS','CAR_Qjacked','CAR_rolldoor','CAR_rolldoorLO','CAR_rollout_LHS','CAR_rollout_RHS','CAR_shuffle_RHS','CAR_sit','CAR_sitp','CAR_sitpLO','CAR_sit_pro','CAR_sit_weak','CAR_tune_radio','CLIMB_idle','CLIMB_jump','CLIMB_jump2fall','CLIMB_jump_B','CLIMB_Pull','CLIMB_Stand','CLIMB_Stand_finish','cower','Crouch_Roll_L','Crouch_Roll_R','DAM_armL_frmBK','DAM_armL_frmFT','DAM_armL_frmLT','DAM_armR_frmBK','DAM_armR_frmFT','DAM_armR_frmRT','DAM_LegL_frmBK','DAM_LegL_frmFT','DAM_LegL_frmLT','DAM_LegR_frmBK','DAM_LegR_frmFT','DAM_LegR_frmRT','DAM_stomach_frmBK','DAM_stomach_frmFT','DAM_stomach_frmLT','DAM_stomach_frmRT','DOOR_LHinge_O','DOOR_RHinge_O','DrivebyL_L','DrivebyL_R','Driveby_L','Driveby_R','DRIVE_BOAT','DRIVE_BOAT_back','DRIVE_BOAT_L','DRIVE_BOAT_R','Drive_L','Drive_LO_l','Drive_LO_R','Drive_L_pro','Drive_L_pro_slow','Drive_L_slow','Drive_L_weak','Drive_L_weak_slow','Drive_R','Drive_R_pro','Drive_R_pro_slow','Drive_R_slow','Drive_R_weak','Drive_R_weak_slow','Drive_truck','DRIVE_truck_back','DRIVE_truck_L','DRIVE_truck_R','Drown','DUCK_cower','endchat_01','endchat_02','endchat_03','EV_dive','EV_step','facanger','facgum','facsurp','facsurpm','factalk','facurios','FALL_back','FALL_collapse','FALL_fall','FALL_front','FALL_glide','FALL_land','FALL_skyDive','Fight2Idle','FightA_1','FightA_2','FightA_3','FightA_block','FightA_G','FightA_M','FIGHTIDLE','FightShB','FightShF','FightSh_BWD','FightSh_FWD','FightSh_Left','FightSh_Right','flee_lkaround_01','FLOOR_hit','FLOOR_hit_f','fucku','gang_gunstand','gas_cwr','getup','getup_front','gum_eat','GunCrouchBwd','GunCrouchFwd','GunMove_BWD','GunMove_FWD','GunMove_L','GunMove_R','Gun_2_IDLE','GUN_BUTT','GUN_BUTT_crouch','Gun_stand','handscower','handsup','HitA_1','HitA_2','HitA_3','HIT_back','HIT_behind','HIT_front','HIT_GUN_BUTT','HIT_L','HIT_R','HIT_walk','HIT_wall','Idlestance_fat','idlestance_old','IDLE_armed','IDLE_chat','IDLE_csaw','Idle_Gang1','IDLE_HBHB','IDLE_ROCKET','IDLE_stance','IDLE_taxi','IDLE_tired','Jetpack_Idle','JOG_femaleA','JOG_maleA','JUMP_glide','JUMP_land','JUMP_launch','JUMP_launch_R','KART_drive','KART_L','KART_LB','KART_R','KD_left','KD_right','KO_shot_face','KO_shot_front','KO_shot_stom','KO_skid_back','KO_skid_front','KO_spin_L','KO_spin_R','pass_Smoke_in_car','phone_in','phone_out','phone_talk','Player_Sneak','Player_Sneak_walkstart','roadcross','roadcross_female','roadcross_gang','roadcross_old','run_1armed','run_armed','run_civi','run_csaw','run_fat','run_fatold','run_gang1','run_left','run_old','run_player','run_right','run_rocket','Run_stop','Run_stopR','Run_Wuzi','SEAT_down','SEAT_idle','SEAT_up','SHOT_leftP','SHOT_partial','SHOT_partial_B','SHOT_rightP','Shove_Partial','Smoke_in_car','sprint_civi','sprint_panic','Sprint_Wuzi','swat_run','Swim_Tread','Tap_hand','Tap_handP','turn_180','Turn_L','Turn_R','WALK_armed','WALK_civi','WALK_csaw','Walk_DoorPartial','WALK_drunk','WALK_fat','WALK_fatold','WALK_gang1','WALK_gang2','WALK_old','WALK_player','WALK_rocket','WALK_shuffle','WALK_start','WALK_start_armed','WALK_start_csaw','WALK_start_rocket','Walk_Wuzi','WEAPON_crouch','woman_idlestance','woman_run','WOMAN_runbusy','WOMAN_runfatold','woman_runpanic','WOMAN_runsexy','WOMAN_walkbusy','WOMAN_walkfatold','WOMAN_walknorm','WOMAN_walkold','WOMAN_walkpro','WOMAN_walksexy','WOMAN_walkshop'},
	{"BOM_PLANT_IN","BOM_PLANT_LOOP","BOM_PLANT_CROUCH_IN","BOM_PLANT_CROUCH_OUT","BOM_PLANT_2IDLE"},
	{"POOL_XLONG_SHOT","POOL_XLONG_START","POOL_LONG_SHOT","POOL_LONG_START","POOL_MED_START","POOL_MED_SHOT","POOL_SHORT_SHOT","POOL_CHALKCUE"},
	{"LKUP_LOOP","POINTUP_IN","SHOUT_01"},
	{'DEALER_DEAL','DEALER_IDLE','drnkbr_prtl','drnkbr_prtl_F','DRUGS_BUY','hndshkaa','hndshkba','hndshkca','hndshkcb','hndshkda','hndshkea','hndshkfa','hndshkfa_swt','Invite_No','Invite_Yes','leanIDLE','leanIN','leanOUT','prtial_gngtlkA','prtial_gngtlkB','prtial_gngtlkC','prtial_gngtlkD','prtial_gngtlkE','prtial_gngtlkF','prtial_gngtlkG','prtial_gngtlkH','prtial_hndshk_01','prtial_hndshk_biz_01','shake_cara','shake_carK','shake_carSH','smkcig_prtl','smkcig_prtl_F'},
	{"PISS_IN","PISS_LOOP","PISS_OUT","PNM_LOOP_A","PNM_ARGUE2_A","PNM_ARGUE1_A"},
	{"boxhipin","bxwlko","catch_box","bxshwlk","bxhwlki","boxshup","boxhipup","boxshdwn","bxhipwlk","bxshwlki"},
	{'Stepsit_in','Stepsit_loop','Stepsit_out'},
	{'Barcustom_get','Barcustom_loop','Barcustom_order','BARman_idle','Barserve_bottle','Barserve_give','Barserve_glass','Barserve_in','Barserve_loop','Barserve_order','dnk_stndF_loop','dnk_stndM_loop'},
	{'Bat_1','Bat_2','Bat_3','Bat_4','Bat_Hit_1','Bat_block','Bat_Hit_2','Bat_Hit_3','Bat_IDLE','Bat_M'},
	{'BD_GF_Wave','BD_Panic_01','BD_Panic_02','BD_Panic_03','BD_Panic_04','BD_Panic_Loop','Grlfrd_Kiss_03','M_smklean_loop','Playa_Kiss_03','wash_up'},
	{'bather','Lay_Bac_Loop','ParkSit_M_loop','ParkSit_W_loop','SitnWait_loop_W'},
	{'gym_bp_celebrate','gym_bp_down','gym_bp_getoff','gym_bp_geton','gym_bp_up_A','gym_bp_up_B','gym_bp_up_smooth'},
	{'BJ_Car_End_W','BJ_Car_Loop_P','BJ_Car_Loop_W','BJ_Car_Start_P','BJ_Car_Start_W','BJ_Couch_End_P','BJ_Couch_End_W','BJ_Couch_Loop_P','BJ_Couch_Loop_W','BJ_Couch_Start_P','BJ_Couch_Start_W','BJ_Stand_End_P','BJ_Stand_End_W','BJ_Stand_Loop_P','BJ_Stand_Loop_W','BJ_Stand_Start_P','BJ_Stand_Start_W'},
	{'camcrch_cmon','camcrch_idleloop','camcrch_stay','camcrch_to_camstnd','camstnd_cmon','camstnd_idleloop','camstnd_lkabt','camstnd_to_camcrch','piccrch_in','piccrch_out','piccrch_take','picstnd_in','picstnd_out','picstnd_take'},
	{'Fixn_Car_Loop','Fixn_Car_Out','flag_drop'},
	{'carfone_in','carfone_loopA','carfone_loopA_to_B','carfone_loopB_to_A','carfone_out','CAR_Sc1_BL','CAR_Sc1_BR','CAR_Sc1_FL','CAR_Sc1_FR','CAR_Sc2_FL','CAR_Sc3_BR','CAR_Sc3_FL','CAR_Sc3_FR','CAR_Sc4_FL','car_talkm_in','car_talkm_loop','car_talkm_out'},
	{'cards_in','cards_loop','cards_lose','cards_out','cards_pick_01','cards_pick_02','cards_raise','cards_win','dealone','manwinb','manwind','Roulette_bet','Roulette_in','Roulette_loop','Roulette_lose','Roulette_out','Roulette_win','Slot_bet_01','Slot_bet_02','Slot_in','Slot_lose_out','Slot_Plyr','Slot_wait','Slot_win_out','wof'},
	{'Copbrowse_in','Copbrowse_loop','Copbrowse_nod','Copbrowse_out','Copbrowse_shake','Coplook_in','Coplook_loop','Coplook_nod','Coplook_out','Coplook_shake','Coplook_think','Coplook_watch'},
	{'Bbalbat_Idle_01','Bbalbat_Idle_02','crckdeth1','crckdeth2','crckdeth3','crckdeth4','crckidle1','crckidle2','crckidle3','crckidle4'},
	{'CRIB_Console_Loop','CRIB_Use_Switch','PED_Console_Loop','PED_Console_Loose','PED_Console_Win'},
	{'DAM_Dive_Loop','DAM_Land','DAM_Launch','Jump_Roll','SF_JumpWall'},
	{'bd_clap','bd_clap1','dance_loop','DAN_Down_A','DAN_Left_A','DAN_Loop_A','DAN_Right_A','DAN_Up_A','dnce_M_a','dnce_M_b','dnce_M_c','dnce_M_d','dnce_M_e'},
	{'DEALER_DEAL','DEALER_IDLE','DEALER_IDLE_01','DEALER_IDLE_02','DEALER_IDLE_03','DRUGS_BUY','shop_pay'},
	{'Cover_Dive_01','Cover_Dive_02','Crushed','Crush_Jump'},
	{'EAT_Burger','EAT_Chicken','EAT_Pizza','EAT_Vomit_P','EAT_Vomit_SK','FF_Dam_Bkw','FF_Dam_Fwd','FF_Dam_Left','FF_Dam_Right','FF_Die_Bkw','FF_Die_Fwd','FF_Die_Left','FF_Die_Right','FF_Sit_Eat1','FF_Sit_Eat2','FF_Sit_Eat3','FF_Sit_In','FF_Sit_In_L','FF_Sit_In_R','FF_Sit_Look','FF_Sit_Loop','FF_Sit_Out_180','FF_Sit_Out_L_180','FF_Sit_Out_R_180','SHP_Thank','SHP_Tray_In','SHP_Tray_Lift','SHP_Tray_Lift_In','SHP_Tray_Lift_Loop','SHP_Tray_Lift_Out','SHP_Tray_Out','SHP_Tray_Pose','SHP_Tray_Return'},
	{'Dance_B1','Dance_B2','Dance_B3','Dance_B4','Dance_B5','Dance_B6','Dance_B7','Dance_B8','Dance_B9','Dance_B10','Dance_B11','Dance_B12','Dance_B13','Dance_B14','Dance_B15','Dance_B16','Dance_G1','Dance_G2','Dance_G3','Dance_G4','Dance_G5','Dance_G6','Dance_G7','Dance_G8','Dance_G9','Dance_G10','Dance_G11','Dance_G12','Dance_G13','Dance_G14','Dance_G15','Dance_G16','dance_loop'},
	{'gym_barbell','gym_free_A','gym_free_B','gym_free_celebrate','gym_free_down','gym_free_loop','gym_free_pickup','gym_free_putdown','gym_free_up_smooth'},
	{'CAS_G2_GasKO','swt_wllpk_L','swt_wllpk_L_back','swt_wllpk_R','swt_wllpk_R_back','swt_wllshoot_in_L','swt_wllshoot_in_R','swt_wllshoot_out_L','swt_wllshoot_out_R','Use_SwipeCard'},
	{'BED_In_L','BED_In_R','BED_Loop_L','BED_Loop_R','BED_Out_L','BED_Out_R','LOU_In','LOU_Loop','LOU_Out','wash_up'},
	{'FF_Dam_Fwd','OFF_Sit_2Idle_180','OFF_Sit_Bored_Loop','OFF_Sit_Crash','OFF_Sit_Drink','OFF_Sit_Idle_Loop','OFF_Sit_In','OFF_Sit_Read','OFF_Sit_Type_Loop','OFF_Sit_Watch'},
	{'shop_cashier','shop_in','shop_lookA','shop_lookB','shop_loop','shop_out','shop_pay','shop_shelf'},
	{'BD_GF_Wave','gfwave2','GF_CarArgue_01','GF_CarArgue_02','GF_CarSpot','GF_StreetArgue_01','GF_StreetArgue_02','gift_get','gift_give','Grlfrd_Kiss_01','Grlfrd_Kiss_02','Grlfrd_Kiss_03','Playa_Kiss_01','Playa_Kiss_02','Playa_Kiss_03'},
	{'F_smklean_loop','lrgirl_bdbnce','lrgirl_hair','lrgirl_hurry','lrgirl_idleloop','lrgirl_idle_to_l0','lrgirl_l0_bnce','lrgirl_l0_loop','lrgirl_l0_to_l1','lrgirl_l12_to_l0','lrgirl_l1_bnce','lrgirl_l1_loop','lrgirl_l1_to_l2','lrgirl_l2_bnce','lrgirl_l2_loop','lrgirl_l2_to_l3','lrgirl_l345_to_l1','lrgirl_l3_bnce','lrgirl_l3_loop','lrgirl_l3_to_l4','lrgirl_l4_bnce','lrgirl_l4_loop','lrgirl_l4_to_l5','lrgirl_l5_bnce','lrgirl_l5_loop','M_smklean_loop','M_smkstnd_loop','prtial_gngtlkB','prtial_gngtlkC','prtial_gngtlkD','prtial_gngtlkE','prtial_gngtlkF','prtial_gngtlkG','prtial_gngtlkH','RAP_A_Loop','RAP_B_Loop','RAP_C_Loop','Sit_relaxed','Tap_hand'},
	{'CPR'},
	{'bitchslap','BMX_celebrate','BMX_comeon','bmx_idleloop_01','bmx_idleloop_02','bmx_talkleft_in','bmx_talkleft_loop','bmx_talkleft_out','bmx_talkright_in','bmx_talkright_loop','bmx_talkright_out','bng_wndw','bng_wndw_02','Case_pickup','door_jet','GRAB_L','GRAB_R','Hiker_Pose','Hiker_Pose_L','Idle_Chat_02','KAT_Throw_K','KAT_Throw_O','KAT_Throw_P','PASS_Rifle_O','PASS_Rifle_Ped','PASS_Rifle_Ply','pickup_box','Plane_door','Plane_exit','Plane_hijack','Plunger_01','Plyrlean_loop','plyr_shkhead','Run_Dive','Scratchballs_01','SEAT_LR','Seat_talk_01','Seat_talk_02','SEAT_watch','smalplane_door','smlplane_door'},
	{'betslp_in','betslp_lkabt','betslp_loop','betslp_out','betslp_tnk','wtchrace_cmon','wtchrace_in','wtchrace_loop','wtchrace_lose','wtchrace_out','wtchrace_win'},
	{'Tai_Chi_in','Tai_Chi_Loop','Tai_Chi_Out'},
	{'shift','shldr','stretch','strleg','time'},
	{'CopTraf_Away','CopTraf_Come','CopTraf_Left','CopTraf_Stop','COP_getoutcar_LHS','Cop_move_FWD','crm_drgbst_01','Door_Kick','plc_drgbst_01','plc_drgbst_02'},
	{'Laugh_01','RAP_A_IN','RAP_A_Loop','RAP_A_OUT','RAP_B_IN','RAP_B_Loop','RAP_B_OUT','RAP_C_Loop'},
	{'RIOT_ANGRY','RIOT_ANGRY_B','RIOT_challenge','RIOT_CHANT','RIOT_FUKU','RIOT_PUNCHES','RIOT_shout'},
	{'CAT_Safe_End','CAT_Safe_Open','CAT_Safe_Open_O','CAT_Safe_Rob','SHP_HandsUp_Scr'},
	{'Dance_B1','Dance_B2','Dance_B3','Dance_B4','Dance_B5','Dance_B6','Dance_B7','Dance_B8','Dance_B9','Dance_B10','Dance_B11','Dance_B12','Dance_B13','Dance_B14','Dance_B15','Dance_B16','Dance_G1','Dance_G2','Dance_G3','Dance_G4','Dance_G5','Dance_G6','Dance_G7','Dance_G8','Dance_G9','Dance_G10','Dance_G11','Dance_G12','Dance_G13','Dance_G14','Dance_G15','Dance_G16','dance_loop'},
	{'scdldlp','scdlulp','scdrdlp','scdrulp','sclng_l','sclng_r','scmid_l','scmid_r','scshrtl','scshrtr','sc_ltor','sc_rtol'},
	{'SEX_1to2_P','SEX_1to2_W','SEX_1_Cum_P','SEX_1_Cum_W','SEX_1_Fail_P','SEX_1_Fail_W','SEX_1_P','SEX_1_W','SEX_2to3_P','SEX_2to3_W','SEX_2_Fail_P','SEX_2_Fail_W','SEX_2_P','SEX_2_W','SEX_3to1_P','SEX_3to1_W','SEX_3_Fail_P','SEX_3_Fail_W','SEX_3_P','SEX_3_W'},
	{'ROB_2Idle','ROB_Loop','ROB_Loop_Threat','ROB_Shifty','ROB_StickUp_In','SHP_Duck','SHP_Duck_Aim','SHP_Duck_Fire','SHP_Gun_Aim','SHP_Gun_Duck','SHP_Gun_Fire','SHP_Gun_Grab','SHP_Gun_Threat','SHP_HandsUp_Scr','SHP_Jump_Glide','SHP_Jump_Land','SHP_Jump_Launch','SHP_Rob_GiveCash','SHP_Rob_HandsUp','SHP_Rob_React','SHP_Serve_End','SHP_Serve_Idle','SHP_Serve_Loop','SHP_Serve_Start','Smoke_RYD' },
	{'F_smklean_loop','M_smklean_loop','M_smkstnd_loop','M_smk_drag','M_smk_in','M_smk_loop','M_smk_out','M_smk_tap'},
	{'SnM_Caned_Idle_P','SnM_Caned_Idle_W','SnM_Caned_P','SnM_Caned_W','SnM_Cane_Idle_P','SnM_Cane_Idle_W','SnM_Cane_P','SnM_Cane_W','SpankedP','SpankedW','Spanked_IdleP','Spanked_IdleW','SpankingP','SpankingW','Spanking_endP','Spanking_endW','Spanking_IdleP','Spanking_IdleW','Spanking_SittingIdleP','Spanking_SittingIdleW','Spanking_SittingP','Spanking_SittingW'},
	{'PLY_CASH','PUN_CASH','PUN_HOLLER','PUN_LOOP','strip_A','strip_B','strip_C','strip_D','strip_E','strip_F','strip_G','STR_A2B','STR_B2A','STR_B2C','STR_C1','STR_C2','STR_C2B','STR_Loop_A','STR_Loop_B','STR_Loop_C' },
	{'batherdown','batherup','Lay_Bac_in','Lay_Bac_out','ParkSit_M_IdleA','ParkSit_M_IdleB','ParkSit_M_IdleC','ParkSit_M_in','ParkSit_M_out','ParkSit_W_idleA','ParkSit_W_idleB','ParkSit_W_idleC','ParkSit_W_in','ParkSit_W_out','SBATHE_F_LieB2Sit','SBATHE_F_Out','SitnWait_in_W','SitnWait_out_W'},
	{'gnstwall_injurd','JMP_Wall1m_180','Rail_fall','Rail_fall_crawl','swt_breach_01','swt_breach_02','swt_breach_03','swt_go','swt_lkt','swt_sty','swt_vent_01','swt_vent_02','swt_vnt_sht_die','swt_vnt_sht_in','swt_vnt_sht_loop','swt_wllpk_L','swt_wllpk_L_back','swt_wllpk_R','swt_wllpk_R_back','swt_wllshoot_in_L','swt_wllshoot_in_R','swt_wllshoot_out_L','swt_wllshoot_out_R'},
	{'VEND_Drink2_P','VEND_Drink_P','vend_eat1_P','VEND_Eat_P','VEND_Use','VEND_Use_pt2'},
	{'Dance_B1','Dance_B2','Dance_B3','Dance_B4','Dance_B5','Dance_B6','Dance_B7','Dance_B8','Dance_B9','Dance_B10','Dance_B11','Dance_B12','Dance_B13','Dance_B14','Dance_B15','Dance_B16','Dance_G1','Dance_G2','Dance_G3','Dance_G4','Dance_G5','Dance_G6','Dance_G7','Dance_G8','Dance_G9','Dance_G10','Dance_G11','Dance_G12','Dance_G13','Dance_G14','Dance_G15','Dance_G16','dance_loop'}
	}
}

ID_Weapons = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46}

Particle_name = {"blood_heli","boat_prop","camflash","carwashspray","cement","cloudfast","coke_puff","coke_trail","cigarette_smoke","explosion_barrel","explosion_crate","explosion_door","exhale","explosion_fuel_car","explosion_large","explosion_medium","explosion_molotov","explosion_small","explosion_tiny","extinguisher","flame","fire","fire_med","fire_large","flamethrower","fire_bike","fire_car","gunflash","gunsmoke","insects","heli_dust","jetpack","jetthrust","nitro","molotov_flame","overheat_car","overheat_car_electric","prt_boatsplash","prt_cardebris","prt_collisionsmoke","prt_glass","prt_gunshell","prt_sand","prt_sand2","prt_smokeII_3_expand","prt_smoke_huge","prt_spark","prt_spark_2","prt_splash","prt_wake","prt_watersplash","prt_wheeldirt","petrolcan","puke","riot_smoke","spraycan","smoke30lit","smoke30m","smoke50lit","shootlight","smoke_flare","tank_fire","teargas","teargasAD","tree_hit_fir","tree_hit_palm","vent","vent2","water_hydrant","water_ripples","water_speed","water_splash","water_splash_big","water_splsh_sml","water_swim","waterfall_end","water_fnt_tme","water_fountain","wallbust","WS_factorysmoke"}


vr = {
    mainMenu = new.bool(),
    settings = new.bool(),
    targets = new.bool(),
    actors = new.bool(),
    cars = new.bool(),
    objects = new.bool(),
    particles = new.bool(),
    pickups = new.bool(),
    explosions = new.bool(),
    audios = new.bool(),
    player = new.bool(),
    groupRelations = new.bool(),
    mission_packs = new.bool(),
    tools = new.bool(),
    info = new.bool(),
    temp_var = {
        theme_curr = new.int(0),
        curr_lang = new.int(0),
        list_name_targets = {},
        list_name_actors = {},
        list_name_cars = {},
        list_name_objects = {},
        list_name_particles = {},
        list_name_pickups = {},
        list_name_explosions = {},
        list_name_audios = {},
        list_audios_name = {},
        list_name_mission_packs = {},
        list_name_missions = {},
        tools_var = {
            tp_actor = new.int(),
            tp_car = new.int(),
            tp_object = new.int(),
        },
        selTarget = new.int(0),
        selTypeTarget = new.int(0),
        updateSphere = false,
        moveTarget = -1,
        enexMarker = new.bool(true)
    },
    current_target = new.int(0),
    list_targets = {},
    current_actor = new.int(0),
    current_car = new.int(0),
    current_object = new.int(0),
    current_particle = new.int(0),
    current_pickup = new.int(0),
    current_explosion = new.int(0),
    current_audio = new.int(0),
    current_mission_pack = new.int(0),
    current_mission = new.int(0),
    list_actors = {},
    list_cars = {},
    list_objects = {},
	list_particles = {},
	list_pickups = {},
	list_explosions = {},
    list_audios = {},
    missData = {
        groupRelations = {},
        player = {
            ['pos'] = new.float[3](884,-1221,16),
            ['angle'] = new.int(0),
            ['modelId'] = new.int(0),
            ['weapon'] = new.int(1),
            ['ammo'] = new.int(1),
            ['health'] = new.int(100),
            ['interiorId'] = new.int(0),
            ['modelType'] = new.ImU8(0)
        }
    },
    camera_zoom = 5,
    camera_angle = {45,0}
}
LanguageList = {}
langt = {}

function imgui.TextColoredRGB(text)
    local style = imgui.GetStyle()
    local colors = style.Colors
    local ImVec4 = imgui.ImVec4
    
    local explode_argb = function(argb)
        local a = bit.band(bit.rshift(argb, 24), 0xFF)
        local r = bit.band(bit.rshift(argb, 16), 0xFF)
        local g = bit.band(bit.rshift(argb, 8), 0xFF)
        local b = bit.band(argb, 0xFF)
        return a, r, g, b
    end

    local getcolor = function(color)
        if color:sub(1, 6):upper() == 'SSSSSS' then
            local r, g, b = colors[1].x, colors[1].y, colors[1].z
            local a = tonumber(color:sub(7, 8), 16) or colors[1].w * 255
            return ImVec4(r, g, b, a / 255)
        end
        local color = type(color) == 'string' and tonumber(color, 16) or color
        if type(color) ~= 'number' then return end
        local r, g, b, a = explode_argb(color)
        return imgui.ImVec4(r, g, b, a)
    end

    local render_text = function(text_)
        for w in text_:gmatch('[^\r\n]+') do
            local text, colors_, m = {}, {}, 1
            w = w:gsub('{(......)}', '{%1FF}')
            while w:find('{........}') do
                local n, k = w:find('{........}')
                local color = getcolor(w:sub(n + 1, k - 1))
                if color then
                    text[#text], text[#text + 1] = w:sub(m, n - 1), w:sub(k + 1, #w)
                    colors_[#colors_ + 1] = color
                    m = n
                end
                w = w:sub(1, n - 1) .. w:sub(k + 1, #w)
            end
            if text[0] then
                for i = 0, #text do
                    imgui.TextColored(colors_[i] or colors[1], text[i])
                    imgui.SameLine(nil, 0)
                end
                imgui.NewLine()
            else imgui.Text(w) end
        end
    end
    render_text(text)
end

function rgba_to_int(r,g,b,a)
	local argb = b  -- b
	argb = bit.bor(argb, bit.lshift(g, 8))  -- g
	argb = bit.bor(argb, bit.lshift(r, 16)) -- r
	argb = bit.bor(argb, bit.lshift(a, 24)) -- a
	return argb
end

function optimAngle(ang)
    return ang % 360
end

function rotateVec2(x,y, angle)
    local nx,ny = 0
    nx = x * math.cos(math.rad(angle)) - y * math.sin(math.rad(angle))
    ny = x * math.sin(math.rad(angle)) + y * math.cos(math.rad(angle))
    return nx,ny
end

function table.moveCell(table,inn,outt)
    local new_t = {}
    for i = 1,#table do
        if i ~= inn then
            new_t[#new_t+1] = table[i]
        end
        if i == outt then
            new_t[#new_t+1] = table[inn]
        end
    end
    return new_t
end

function fif(condition, if_true, if_false)
    if condition then return if_true else return if_false end
end

function for_each_vehicle_material(car, func)
    local i = 1
	for _, comp in ipairs(mad.get_all_vehicle_components(car)) do
		for _, obj in ipairs(comp:get_objects()) do
			for _, mat in ipairs(obj:get_materials()) do
                func(i,mat, comp, obj)
                i = i + 1
			end
		end
	end
end

function get_car_color_rgba(id)
	local col_table = memory.getuint32(0x4C8390, true)
	local clr = memory.getuint32(col_table + (id * 4))
	return clr
end

function deletedir(dir)
    for file in lfs.dir(dir) do
        local file_path = dir..'/'..file
        if file ~= "." and file ~= ".." then
            if lfs.attributes(file_path, 'mode') == 'file' then
                os.remove(file_path)
            elseif lfs.attributes(file_path, 'mode') == 'directory' then
                deletedir(file_path)
            end
        end
    end
    lfs.rmdir(dir)
end

imgui.OnInitialize(function ()
    local theme = require('Theme\\'..vr.Data.Settings.curr_theme)
    theme.apply_custom_style()

    pedsSkinAtlas = imgui.CreateTextureFromFile(getWorkingDirectory().."\\lib\\ldyom\\images\\peds.jpg")
    weaponsAtlas = imgui.CreateTextureFromFile(getWorkingDirectory().."\\lib\\ldyom\\images\\weapons.png")

    local config = imgui.ImFontConfig()
    config.MergeMode = true
    config.PixelSnapH = true
    config.SizePixels = 14.0;
    config.FontDataOwnedByAtlas = false
    config.GlyphOffset.y = 1.0 -- смещение на 1 пиксеот вниз
    local fa_glyph_ranges = new.ImWchar[3]({ faicons.min_range, faicons.max_range, 0 })
    -- icon
    local faicon = imgui.GetIO().Fonts:AddFontFromMemoryCompressedBase85TTF(faicons.get_font_data_base85(), config.SizePixels, config, fa_glyph_ranges)
end)



local res = {}
res.x,res.y = getScreenResolution()

--Main menu
imgui.OnFrame(function() return vr.mainMenu[0] end,
function()
    imgui.SetNextWindowSize(imgui.ImVec2(200,400),imgui.Cond.Appearing)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x/2-100,res.y/2-200),imgui.Cond.Always)
    imgui.Begin(langt['mainMenu'],vr.mainMenu, imgui.WindowFlags.AlwaysAutoResize)


    local size_b = imgui.ImVec2(160,0)
    if imgui.Button(faicons.ICON_RUNNING..' '..langt['targets'],size_b) then
        vr.mainMenu[0] = false
        vr.targets[0] = true
    end

    if imgui.Button(faicons.ICON_MALE..' '..langt['actors'],size_b) then
        vr.actors[0] = not vr.actors[0]
        vr.mainMenu[0] = false
    end
    if imgui.Button(faicons.ICON_HANDSHAKE..' '..langt['groupRelations'],size_b) then
        vr.groupRelations[0] = not vr.groupRelations[0]
        vr.mainMenu[0] = false
    end
    if imgui.Button(faicons.ICON_CARS..' '..langt['cars'],size_b) then
      vr.cars[0] = not vr.cars[0]
      vr.mainMenu[0] = false
    end
    if imgui.Button(faicons.ICON_ARCHIVE..' '..langt['objects'],size_b) then
      vr.objects[0] = not vr.objects[0]
      vr.mainMenu[0] = false
    end
    if imgui.Button(faicons.ICON_STARS..' '..langt['particles'],size_b) then
      vr.particles[0] = true
      vr.mainMenu[0] = false
    end
    if imgui.Button(faicons.ICON_HEART..' '..langt['pickups'],size_b) then
      vr.pickups[0] = true
      vr.mainMenu[0] = false
    end
    if imgui.Button(faicons.ICON_BOMB..' '..langt['explosions'],size_b) then
      vr.explosions[0] = true
      vr.mainMenu[0] = false
    end
    if imgui.Button(faicons.ICON_VOLUME_UP..' '..langt['audio'],size_b) then
        vr.audios[0] = true
        vr.mainMenu[0] = false
    end
    if imgui.Button(faicons.ICON_PARKING..' '..langt['player'],size_b) then
      vr.player[0] = true
      vr.mainMenu[0] = false
      lockPlayerControl(true)
    end
    imgui.Separator()
    if imgui.Button(faicons.ICON_THEATER_MASKS..' '..langt['missionPacks'],size_b) then
      vr.mission_packs[0] = not vr.mission_packs[0]
      vr.mainMenu[0] = false
    end
    imgui.Separator()
    if imgui.Button(faicons.ICON_PLAY..' '..langt['missionStart'],size_b) then
      vr.miss_start = true
      vr.mainMenu[0] = false
    end
    if imgui.Button(faicons.ICON_TOOLS..' '..langt['tools'],size_b) then
      vr.mainMenu[0] = false
      vr.tools[0] = true
    end
    imgui.Separator()
    if imgui.Button(faicons.ICON_INFO..' '..langt['info'],size_b) then
      vr.mainMenu[0] = false
      vr.info[0] = true
    end

    if imgui.Button(faicons.ICON_CONGS..' '..langt['settings'],size_b) then
        vr.mainMenu[0] = false
        vr.settings[0] = true
    end

    imgui.End()
end)

--target
imgui.OnFrame(function() return vr.targets[0] end,
function()

    local isWindow = false

    imgui.SetNextWindowSize(imgui.ImVec2(270,410),imgui.Cond.Appearing)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x-270,0),imgui.Cond.Appearing)
    imgui.Begin(faicons.ICON_RUNNING..' '..langt['targets'],nil, imgui.WindowFlags.AlwaysAutoResize)

    --List
    imgui.SetNextItemWidth(255)
    imgui.ListBoxStr_arr('', vr.current_target,new('const char* const [?]', #vr.temp_var.list_name_targets, vr.temp_var.list_name_targets),#vr.temp_var.list_name_targets,15)

    if imgui.Button(langt['add']) then
        imgui.OpenPopup("addT")
    end
    if #vr.list_targets > 0 then
        imgui.SameLine()
        if imgui.Button(langt['rename']) then
            imgui.OpenPopup("rename")
        end
        imgui.SameLine()
        if imgui.Button(langt['delete']) then
            imgui.OpenPopup(faicons.ICON_TRASH_ALT.." "..langt['delete'])
        end
        imgui.SameLine()
        if imgui.Button(langt['cut']) then
            vr.temp_var.moveTarget = vr.current_target[0]+1
        end
        if vr.temp_var.moveTarget ~= -1 then
            imgui.SameLine()
            if imgui.Button(langt['paste']) then
                vr.list_targets = table.moveCell(vr.list_targets,vr.temp_var.moveTarget,vr.current_target[0]+1)
                vr.temp_var.list_name_targets = table.moveCell(vr.temp_var.list_name_targets,vr.temp_var.moveTarget,vr.current_target[0]+1)
                vr.temp_var.moveTarget = -1
            end
        end
    end

    isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()
    -- if imgui.BeginPopupContextItem('hee') then
    --     imgui.InputText('',vr.buf_edit_targets_name,ffi.sizeof(vr.buf_edit_targets_name))
    --     if imgui.Button(langt['apply']) and vr.buf_edit_targets_name[0] ~= '' then
    --     vr.temp_var.list_targets[vr.lb_cur_targets[0]+1]['Name'] =  ffi.string(vr.buf_edit_targets_name)
    --     vr.temp_var.list_name_targets[vr.lb_cur_targets[0]+1] = ffi.string(vr.buf_edit_targets_name)
    --     imgui.StrCopy(vr.buf_edit_targets_name,'')
    --     imgui.CloseCurrentPopup()
    --     end
    --     imgui.PushItemWidth(-1);
    --     imgui.PopItemWidth();
    --     imgui.EndPopup();
    -- end

    -- add target popup
    if imgui.BeginPopup("addT") then
        if imgui.Combo(langt['typeTarget'],vr.temp_var.selTarget,new('const char* const [?]', #vr.temp_var.type_targets_name, vr.temp_var.type_targets_name),#vr.temp_var.type_targets_name) then
            vr.temp_var.selTypeTarget[0] = 0
            if vr.temp_var.selTarget[0] == 3 then
                vr.temp_var.target_type = decodeJson(langt['target_type'])
            elseif vr.temp_var.selTarget[0] == 6 then
                vr.temp_var.target_type = decodeJson(langt['target_type_ev'])
            end
        end

        if vr.temp_var.selTarget[0] == 3 or vr.temp_var.selTarget[0] == 6 then
            imgui.Combo(langt['type'],vr.temp_var.selTypeTarget,new('const char* const [?]', #vr.temp_var.target_type, vr.temp_var.target_type),#vr.temp_var.target_type)
        end
        if imgui.Button(langt['create']) then
            vr.list_targets[#vr.list_targets+1] = {
                ['name'] = new.char[65](langt['target'].." #"..tostring(#vr.list_targets+1).." : "..vr.temp_var.type_targets_name[vr.temp_var.selTarget[0]+1]),
                ['type'] = new.int(vr.temp_var.selTarget[0]),
            }
            local px,py,pz = getCharCoordinates(PLAYER_PED)
            if vr.temp_var.selTarget[0] == 0 then
                vr.list_targets[#vr.list_targets].data = {
                    ['pos'] = new.float[3](px,py,pz),
                    ['radius'] = new.float(2),
                    ['text'] = new.char[129](),
                    ['textTime'] = new.float(2),
                    ['colorBlip'] = new.int(0),
                }
            elseif vr.temp_var.selTarget[0] == 1 then
                vr.list_targets[#vr.list_targets].data = {
                    ['car'] = new.int(),
                    ['text'] = new.char[129](),
                    ['textTime'] = new.float(2),
                    ['colorBlip'] = new.int(0),
                }
            elseif vr.temp_var.selTarget[0] == 2 then
                vr.list_targets[#vr.list_targets].data = {
                    ['actor'] = new.int(),
                    ['text'] = new.char[129](),
                    ['textTime'] = new.float(2),
                    ['colorBlip'] = new.int(0),
                    ['killGroup'] = new.bool(false),
                }
            elseif vr.temp_var.selTarget[0] == 3 then
                vr.list_targets[#vr.list_targets].targetType = new.int(vr.temp_var.selTypeTarget[0])
                imgui.StrCopy(vr.list_targets[#vr.list_targets].name, langt['target'].." #"..#vr.list_targets.." : "..vr.temp_var.target_type[vr.temp_var.selTypeTarget[0]+1])
                if vr.temp_var.selTypeTarget[0] == 0 then
                    vr.list_targets[#vr.list_targets].data = {
                        ['tied'] = new.int(0),
                        ['tiedId'] = new.int(0),
                        ['follow'] = new.int(0),
                        ['followId'] = new.int(0),
                        ['pos'] = new.float[3](px,py,pz),
                        ['rotate'] = new.int[3](0,0,0),
                        ['text'] = new.char[129](),
                        ['time'] = new.float(2),
                        ['moveCam'] = new.bool(false),
                    }
                elseif vr.temp_var.selTypeTarget[0] == 1 then
                    vr.list_targets[#vr.list_targets].data = {
                        ['time'] = new.int(2),
                    }
                elseif vr.temp_var.selTypeTarget[0] == 2 then
                    vr.list_targets[#vr.list_targets].data = {
                        ['text'] = new.char[129](),
                        ['time'] = new.float(2),
                    }
                elseif vr.temp_var.selTypeTarget[0] == 3 then
                    vr.list_targets[#vr.list_targets].data = {
                        ['weather'] = new.int(0)
                    }
                elseif vr.temp_var.selTypeTarget[0] == 4 then
                    vr.list_targets[#vr.list_targets].data = {
                        ['time'] = new.int[2](0,0)
                    }
                elseif vr.temp_var.selTypeTarget[0] == 5 then
                    vr.list_targets[#vr.list_targets].data = {
                        ['peds'] = new.int(0),
                        ['cars'] = new.int(0)
                    }
                end
            elseif vr.temp_var.selTarget[0] == 4 then
                vr.list_targets[#vr.list_targets].data = {
                    ['object'] = new.int(),
                    ['text'] = new.char[129](),
                    ['textTime'] = new.float(2),
                    ['colorBlip'] = new.int(0),
                    ['type'] = new.int(0),
                    ['weapon'] = new.int(1),
                }
            elseif vr.temp_var.selTarget[0] == 5 then
                vr.list_targets[#vr.list_targets].data = {
                    ['pickup'] = new.int(),
                    ['text'] = new.char[129](),
                    ['textTime'] = new.float(2),
                    ['colorBlip'] = new.int(0),
                }
            elseif vr.temp_var.selTarget[0] == 6 then
                vr.list_targets[#vr.list_targets].targetType = new.int(vr.temp_var.selTypeTarget[0])
                imgui.StrCopy(vr.list_targets[#vr.list_targets].name, langt['target'].." #"..#vr.list_targets.." : "..vr.temp_var.target_type[vr.temp_var.selTypeTarget[0]+1])
                if vr.temp_var.selTypeTarget[0] == 0 then
                    local angle = getCharHeading(PLAYER_PED)
                    vr.list_targets[#vr.list_targets].data = {
                        ['pos'] = new.float[3](px,py,pz),
                        ['angle'] = new.float(angle),
                        ['modelId'] = new.int(0),
                        ['modelType'] = new.int(0),
                        ['health'] = new.int(100),
                        ['weapon'] = new.int(1),
                        ['ammo'] = new.int(0),
                        ['interiorId'] = new.int(0),
                    }
                elseif vr.temp_var.selTypeTarget[0] == 1 then
                    vr.list_targets[#vr.list_targets].data = {
                        ['pack'] = new.int(0),
                        ['anim'] = new.int(0),
                        ['looped'] = new.bool(false)
                    }
                elseif vr.temp_var.selTypeTarget[0] == 2 then
                    vr.list_targets[#vr.list_targets].data = {
                        ['car'] = new.int(0),
                        ['carPlace'] = new.int(0),
                    }
                elseif vr.temp_var.selTypeTarget[0] == 3 then
                    vr.list_targets[#vr.list_targets].data = {
                        ['levelWanted'] = new.int(0),
                    }
                elseif vr.temp_var.selTypeTarget[0] == 5 then
                    vr.list_targets[#vr.list_targets].data = {
                        ['dialog'] = new.int(0),
                        ['dialogs'] = {},
                    }
                elseif vr.temp_var.selTypeTarget[0] == 6 then
                    vr.list_targets[#vr.list_targets].data = {
                        ['money'] = new.int(0),
                    }
                end
            end
            local targets_name = {}
            for i = 1,#vr.list_targets do
                targets_name[#targets_name+1] = vr.list_targets[i].name
            end
            vr.temp_var.list_name_targets = targets_name
            vr.current_target[0] = #vr.list_targets-1
            vr.temp_var.selTarget[0] = 0
            vr.temp_var.selTypeTarget[0] = 0
            imgui.CloseCurrentPopup()
        end

        imgui.EndPopup()
    end

    --delete
    if imgui.BeginPopupModal(faicons.ICON_TRASH_ALT.." "..langt['delete'], nil,imgui.WindowFlags.AlwaysAutoResize) then
        imgui.Text(langt['deleteQues'])

        local size_b = imgui.ImVec2(100,0)

        if imgui.Button(langt['yes'],size_b) then
            table.remove(vr.list_targets,vr.current_target[0]+1)
            table.remove(vr.temp_var.list_name_targets,vr.current_target[0]+1)
            if vr.current_target[0] > 0 then 
                vr.current_target[0] = vr.current_target[0] - 1
            end
            imgui.CloseCurrentPopup()
        end
        imgui.SameLine()
        if imgui.Button(langt['no'],size_b) then
            imgui.CloseCurrentPopup()
        end
    end

    --Rename popup
    if imgui.BeginPopup("rename") then

        imgui.InputText('',vr.list_targets[vr.current_target[0]+1].name,ffi.sizeof(vr.list_targets[vr.current_target[0]+1].name))

        if imgui.Button(langt['close']) then imgui.CloseCurrentPopup() end

        imgui.EndPopup()
    end

    imgui.End()

    -- target render
    if #vr.list_targets > 0 then
        imgui.SetNextWindowSize(imgui.ImVec2(400,360),imgui.Cond.Always)
        imgui.SetNextWindowPos(imgui.ImVec2(res.x-670,0),imgui.Cond.Appearing)
        imgui.Begin(faicons.ICON_RUNNING..' '..langt['target'],nil, imgui.WindowFlags.AlwaysAutoResize)

        --Checkpoint
        lockPlayerControl(true)
        if vr.list_targets[vr.current_target[0]+1].type[0] == 0 then
            if imgui.Button(faicons.ICON_STREET_VIEW) then
                local px,py,pz = getCharCoordinates(PLAYER_PED)
                vr.list_targets[vr.current_target[0]+1]['data'].pos[0] = px
                vr.list_targets[vr.current_target[0]+1]['data'].pos[1] = py
                vr.list_targets[vr.current_target[0]+1]['data'].pos[2] = pz
            end
            if imgui.IsItemHovered() then
                imgui.SetTooltip(langt['playerCoordinates'])
            end
            imgui.SameLine()
            imgui.PushItemWidth(270)
            imgui.InputFloat3(langt['position'],vr.list_targets[vr.current_target[0]+1]['data']['pos'],"%.6f")
            imgui.PushItemWidth(-130)
            if imgui.DragFloat(langt['radiusCheckpoint'],vr.list_targets[vr.current_target[0]+1]['data']['radius'],0.05,0,100) then
                vr.updateSphere = true
            end
            imgui.Combo(langt['colorMarker'],vr.list_targets[vr.current_target[0]+1]['data']['colorBlip'],new('const char* const [?]', #vr.temp_var.targets_marker_color, vr.temp_var.targets_marker_color),#vr.temp_var.targets_marker_color)
            imgui.InputText(langt['textTarget'],vr.list_targets[vr.current_target[0]+1]['data']['text'],ffi.sizeof(vr.list_targets[vr.current_target[0]+1]['data']['text']))
            imgui.InputFloat(langt['timeText'],vr.list_targets[vr.current_target[0]+1]['data']['textTime'])
        

            imgui.SetNextWindowBgAlpha(0.50)
            imgui.SetNextWindowPos(imgui.ImVec2(0, 0), imgui.Cond.Always)
            imgui.SetNextWindowSize(imgui.ImVec2(220,60),imgui.Cond.Always)
            imgui.Begin("info",nil,imgui.WindowFlags.NoDecoration + imgui.WindowFlags.AlwaysAutoResize + imgui.WindowFlags.NoSavedSettings + imgui.WindowFlags.NoMove + imgui.WindowFlags.NoInputs)

            imgui.Text(vr.temp_var.infoOverlay[1])
            imgui.Text(vr.temp_var.infoOverlay[2])
            imgui.Text(vr.temp_var.infoOverlay[3])

            imgui.End()

            isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()

            --edit
            local cx,cy,cz = vr.list_targets[vr.current_target[0]+1]['data'].pos[0],vr.list_targets[vr.current_target[0]+1]['data'].pos[1],vr.list_targets[vr.current_target[0]+1]['data'].pos[2]
            cx = cx + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
            cy = cy + (vr.camera_zoom*math.cos(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
            cz = cz + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[2])))
            setFixedCameraPosition(cx,cy,cz)
            pointCameraAtPoint(vr.list_targets[vr.current_target[0]+1]['data'].pos[0],vr.list_targets[vr.current_target[0]+1]['data'].pos[1],vr.list_targets[vr.current_target[0]+1]['data'].pos[2],2)
            if not isWindow then
                if imgui.IsMouseDragging(2) then
                    local dt = imgui.GetIO().MouseDelta
                    vr.camera_angle[1],vr.camera_angle[2] = vr.camera_angle[1] + dt.x, vr.camera_angle[2] + dt.y
                end
                if imgui.GetIO().MouseWheel ~= 0 then
                    vr.camera_zoom = vr.camera_zoom + (vr.camera_zoom * imgui.GetIO().MouseWheel)/4
                    if vr.camera_zoom < 1 then vr.camera_zoom = 1 end
                end
            end
            if isKeyDown(vkeys.VK_UP) then
                vr.list_targets[vr.current_target[0]+1]['data'].pos[0] = vr.list_targets[vr.current_target[0]+1]['data'].pos[0] + 0.1
            elseif isKeyDown(vkeys.VK_DOWN) then
                vr.list_targets[vr.current_target[0]+1]['data'].pos[0] = vr.list_targets[vr.current_target[0]+1]['data'].pos[0] - 0.1
            end
            if isKeyDown(vkeys.VK_LEFT) then
                vr.list_targets[vr.current_target[0]+1]['data'].pos[1] = vr.list_targets[vr.current_target[0]+1]['data'].pos[1] + 0.1
            elseif isKeyDown(vkeys.VK_RIGHT) then
                vr.list_targets[vr.current_target[0]+1]['data'].pos[1] = vr.list_targets[vr.current_target[0]+1]['data'].pos[1] - 0.1
            end
        elseif vr.list_targets[vr.current_target[0]+1].type[0] == 1 then

            imgui.Combo(langt['car'],vr.list_targets[vr.current_target[0]+1]['data']['car'],new('const char* const [?]', #vr.temp_var.list_name_cars, vr.temp_var.list_name_cars),#vr.temp_var.list_name_cars)
            imgui.Combo(langt['colorMarker'],vr.list_targets[vr.current_target[0]+1]['data']['colorBlip'],new('const char* const [?]', #vr.temp_var.targets_marker_color, vr.temp_var.targets_marker_color),#vr.temp_var.targets_marker_color)
            imgui.InputText(langt['textTarget'],vr.list_targets[vr.current_target[0]+1]['data']['text'],ffi.sizeof(vr.list_targets[vr.current_target[0]+1]['data']['text']))
            imgui.InputFloat(langt['timeText'],vr.list_targets[vr.current_target[0]+1]['data']['textTime'])
        elseif vr.list_targets[vr.current_target[0]+1].type[0] == 2 then

            imgui.Combo(langt['actor'],vr.list_targets[vr.current_target[0]+1]['data']['actor'],new('const char* const [?]', #vr.temp_var.list_name_actors, vr.temp_var.list_name_actors),#vr.temp_var.list_name_actors)
            mimgui_addons.ToggleButton(langt['killGroup'],vr.list_targets[vr.current_target[0]+1]['data'].killGroup)
            imgui.Combo(langt['colorMarker'],vr.list_targets[vr.current_target[0]+1]['data']['colorBlip'],new('const char* const [?]', #vr.temp_var.targets_marker_color, vr.temp_var.targets_marker_color),#vr.temp_var.targets_marker_color)
            imgui.InputText(langt['textTarget'],vr.list_targets[vr.current_target[0]+1]['data']['text'],ffi.sizeof(vr.list_targets[vr.current_target[0]+1]['data']['text']))
            imgui.InputFloat(langt['timeText'],vr.list_targets[vr.current_target[0]+1]['data']['textTime'])
        elseif vr.list_targets[vr.current_target[0]+1].type[0] == 3 then
            if vr.list_targets[vr.current_target[0]+1].targetType[0] == 0 then
                if imgui.Button(faicons.ICON_STREET_VIEW) then
                    local px,py,pz = getCharCoordinates(PLAYER_PED)
                    vr.list_targets[vr.current_target[0]+1]['data'].pos[0] = px
                    vr.list_targets[vr.current_target[0]+1]['data'].pos[1] = py
                    vr.list_targets[vr.current_target[0]+1]['data'].pos[2] = pz
                end
                if imgui.IsItemHovered() then
                    imgui.SetTooltip(langt['playerCoordinates'])
                end
                imgui.SameLine()
                imgui.PushItemWidth(270)
                imgui.InputFloat3(langt['position'],vr.list_targets[vr.current_target[0]+1]['data'].pos,"%.6f")
                imgui.PushItemWidth(-130)
                imgui.DragInt3(langt['rotate'],vr.list_targets[vr.current_target[0]+1]['data'].rotate,1,-360,360,"%d°")
                
                if imgui.Button(langt['edithand']) then
                    printHelpForever("HOBJ")
                    vr.temp_var.editmodeCamera = true
                    vr.targets[0] = false
                end
                if imgui.SliderInt(langt['tied'], vr.list_targets[vr.current_target[0]+1]['data'].tied, 0, 2, vr.temp_var.CutscenePos[vr.list_targets[vr.current_target[0]+1]['data'].tied[0]+1]) then
                    vr.list_targets[vr.current_target[0]+1]['data'].tiedId[0] = 0
                end
                if vr.list_targets[vr.current_target[0]+1]['data'].tied[0] == 1 then
                    imgui.Combo(langt['actor'],vr.list_targets[vr.current_target[0]+1]['data']['tiedId'],new('const char* const [?]', #vr.temp_var.list_name_actors, vr.temp_var.list_name_actors),#vr.temp_var.list_name_actors)
                elseif vr.list_targets[vr.current_target[0]+1]['data'].tied[0] == 2 then
                    imgui.Combo(langt['car'],vr.list_targets[vr.current_target[0]+1]['data']['tiedId'],new('const char* const [?]', #vr.temp_var.list_name_cars, vr.temp_var.list_name_cars),#vr.temp_var.list_name_cars)
                end
                if imgui.SliderInt(langt['follow'], vr.list_targets[vr.current_target[0]+1]['data'].follow, 0, 2, vr.temp_var.CutsceneLook[vr.list_targets[vr.current_target[0]+1]['data'].follow[0]+1]) then
                    vr.list_targets[vr.current_target[0]+1]['data'].followId[0] = 0
                end
                if vr.list_targets[vr.current_target[0]+1]['data'].follow[0] == 1 then
                    imgui.Combo(langt['actor'],vr.list_targets[vr.current_target[0]+1]['data']['followId'],new('const char* const [?]', #vr.temp_var.list_name_actors, vr.temp_var.list_name_actors),#vr.temp_var.list_name_actors)
                elseif vr.list_targets[vr.current_target[0]+1]['data'].follow[0] == 2 then
                    imgui.Combo(langt['car'],vr.list_targets[vr.current_target[0]+1]['data']['followId'],new('const char* const [?]', #vr.temp_var.list_name_cars, vr.temp_var.list_name_cars),#vr.temp_var.list_name_cars)
                end
                mimgui_addons.ToggleButton(langt['movecam'],vr.list_targets[vr.current_target[0]+1]['data'].moveCam)
                imgui.InputText(langt['text'],vr.list_targets[vr.current_target[0]+1]['data']['text'],ffi.sizeof(vr.list_targets[vr.current_target[0]+1]['data']['text']))
                imgui.InputFloat(langt['timecutscene'],vr.list_targets[vr.current_target[0]+1]['data']['time'])   
            elseif vr.list_targets[vr.current_target[0]+1].targetType[0] == 1 then
                imgui.InputInt(langt['time'],vr.list_targets[vr.current_target[0]+1]['data']['time'])  
            elseif vr.list_targets[vr.current_target[0]+1].targetType[0] == 2 then
                imgui.InputText(langt['text'],vr.list_targets[vr.current_target[0]+1]['data']['text'],ffi.sizeof(vr.list_targets[vr.current_target[0]+1]['data']['text']))
                imgui.InputFloat(langt['timeout'],vr.list_targets[vr.current_target[0]+1]['data']['time'])
            elseif vr.list_targets[vr.current_target[0]+1].targetType[0] == 3 then
                imgui.Combo(langt['weather'],vr.list_targets[vr.current_target[0]+1]['data'].weather,new('const char* const [?]', #vr.temp_var.Weather_arr, vr.temp_var.Weather_arr),#vr.temp_var.Weather_arr)
                if imgui.Button(langt['preview']) then
                    forceWeatherNow(vr.list_targets[vr.current_target[0]+1]['data'].weather[0])
                end
                if imgui.Button(langt['unPreview']) then
                    forceWeatherNow(0)
                end
            elseif vr.list_targets[vr.current_target[0]+1].targetType[0] == 4 then
                if imgui.Button(langt['timeGameMiss']) then
                    printHelpForever("HMTIM")
                    vr.temp_var.editmodeTimetarg = true
                    vr.targets[0] = false
                end
            elseif vr.list_targets[vr.current_target[0]+1].targetType[0] == 5 then
                imgui.SliderInt(langt['countPed'],vr.list_targets[vr.current_target[0]+1]['data'].peds,0,2,vr.temp_var.countTraffic[vr.list_targets[vr.current_target[0]+1]['data'].peds[0]+1])
                imgui.SliderInt(langt['countCar'],vr.list_targets[vr.current_target[0]+1]['data'].cars,0,2,vr.temp_var.countTraffic[vr.list_targets[vr.current_target[0]+1]['data'].cars[0]+1])
            end
        elseif vr.list_targets[vr.current_target[0]+1].type[0] == 4 then
            imgui.Combo(langt['object'],vr.list_targets[vr.current_target[0]+1]['data']['object'],new('const char* const [?]', #vr.temp_var.list_name_objects, vr.temp_var.list_name_objects),#vr.temp_var.list_name_objects)
            imgui.SliderInt(langt['type'],vr.list_targets[vr.current_target[0]+1]['data'].type,0,3,vr.temp_var.targetTypeObj[vr.list_targets[vr.current_target[0]+1]['data'].type[0]+1])
            if vr.list_targets[vr.current_target[0]+1]['data'].type[0] == 3 then
                if imgui.ImageButton(weaponsAtlas, imgui.ImVec2(52,52),imgui.ImVec2((vr.list_targets[vr.current_target[0]+1]['data'].weapon[0]-1)*0.02272727272,0), imgui.ImVec2(vr.list_targets[vr.current_target[0]+1]['data'].weapon[0]*0.02272727272,1)) then
                    imgui.OpenPopup('weapon')
                end

                imgui.SameLine()
                imgui.Text(langt["weapon"])

                --weapon popup
                if imgui.BeginPopup('weapon') then
                    imgui.BeginChild('weapon',imgui.ImVec2(200,450))
            
                    for i = 1,#ID_Weapons do
                        imgui.PushIDStr(tostring(i))
                        if imgui.ImageButton(weaponsAtlas,imgui.ImVec2(52,52),imgui.ImVec2((i-1)*0.02272727272,0),imgui.ImVec2(i*0.02272727272,1)) then
                            vr.list_targets[vr.current_target[0]+1]['data'].weapon[0] = i
                        end
                        if imgui.IsItemHovered() then
                            imgui.SetTooltip(vr.temp_var.weap_names[i])
                        end
                        imgui.PopID()
                        if i % 3 ~= 0 then
                            imgui.SameLine()
                        end
                    end
            
                    imgui.EndChild()
                    imgui.EndPopup()
                end

            end
            imgui.Combo(langt['colorMarker'],vr.list_targets[vr.current_target[0]+1]['data']['colorBlip'],new('const char* const [?]', #vr.temp_var.targets_marker_color, vr.temp_var.targets_marker_color),#vr.temp_var.targets_marker_color)
            imgui.InputText(langt['text'],vr.list_targets[vr.current_target[0]+1]['data']['text'],ffi.sizeof(vr.list_targets[vr.current_target[0]+1]['data']['text']))
            imgui.InputFloat(langt['timeText'],vr.list_targets[vr.current_target[0]+1]['data']['textTime'])

        elseif vr.list_targets[vr.current_target[0]+1].type[0] == 5 then
            imgui.Combo(langt['pickup'],vr.list_targets[vr.current_target[0]+1]['data']['pickup'],new('const char* const [?]', #vr.temp_var.list_name_pickups, vr.temp_var.list_name_pickups),#vr.temp_var.list_name_pickups)
            imgui.Combo(langt['colorMarker'],vr.list_targets[vr.current_target[0]+1]['data']['colorBlip'],new('const char* const [?]', #vr.temp_var.targets_marker_color, vr.temp_var.targets_marker_color),#vr.temp_var.targets_marker_color)
            imgui.InputText(langt['text'],vr.list_targets[vr.current_target[0]+1]['data']['text'],ffi.sizeof(vr.list_targets[vr.current_target[0]+1]['data']['text']))
            imgui.InputFloat(langt['timeText'],vr.list_targets[vr.current_target[0]+1]['data']['textTime'])
        elseif vr.list_targets[vr.current_target[0]+1].type[0] == 6 then
            if vr.list_targets[vr.current_target[0]+1].targetType[0] == 0 then

                imgui.PushIDStr("position")
                if imgui.Button(faicons.ICON_STREET_VIEW) then
                    local px,py,pz = getCharCoordinates(PLAYER_PED)
                    pz = getGroundZFor3dCoord(px,py,pz)
                    vr.list_targets[vr.current_target[0]+1]['data'].pos[0] = px
                    vr.list_targets[vr.current_target[0]+1]['data'].pos[1] = py
                    vr.list_targets[vr.current_target[0]+1]['data'].pos[2] = pz
                end
                imgui.PopID()
                if imgui.IsItemHovered() then
                    imgui.SetTooltip(langt['playerCoordinates'])
                end

                imgui.SameLine()
                imgui.PushItemWidth(270)
                if imgui.InputFloat3(langt['position'],vr.list_targets[vr.current_target[0]+1]['data']['pos'],"%.6f") then
                    local xx,xy,xz = vr.list_targets[vr.current_target[0]+1]['data']['pos'][0],vr.list_targets[vr.current_target[0]+1]['data']['pos'][1],vr.list_targets[vr.current_target[0]+1]['data']['pos'][2]
                end

                imgui.PushIDStr("angle")
                if imgui.Button(faicons.ICON_STREET_VIEW) then
                    local angle = getCharHeading(PLAYER_PED)
                    vr.list_targets[vr.current_target[0]+1]['data'].angle[0] = angle
                end
                imgui.PopID()
                if imgui.IsItemHovered() then
                    imgui.SetTooltip(langt['playerAngle'])
                end

                imgui.SameLine()
                imgui.DragFloat(langt['angle'], vr.list_targets[vr.current_target[0]+1]['data'].angle,1,0,360) 

                imgui.Separator()

                if imgui.SliderInt('',vr.list_targets[vr.current_target[0]+1]['data'].modelType,0,1,tostring(fif(vr.list_targets[vr.current_target[0]+1]['data'].modelType[0] == 0,langt['ped'],langt['specActor']))) then
                    vr.list_targets[vr.current_target[0]+1]['data']['modelId'][0] = 0
                end

                
                --skin popup open
                if vr.list_targets[vr.current_target[0]+1]['data'].modelType[0] == 0 then
                    if imgui.Button(faicons.ICON_TSHIRT) then
                        imgui.OpenPopup('skins')
                    end
                    

                    imgui.SameLine()

                    if imgui.InputInt(langt['model'],vr.list_targets[vr.current_target[0]+1]['data'].modelId) then
                        local id_a = 0
                        for v = 1,#ID_Actors do
                            if vr.list_targets[vr.current_target[0]+1]['data']['modelId'][0] <= ID_Actors[v] then
                                id_a = ID_Actors[v]
                                break
                            end
                        end
                        vr.list_targets[vr.current_target[0]+1]['data']['modelId'][0] = id_a
                    end
                else
                    imgui.Combo(langt['model'], vr.list_targets[vr.current_target[0]+1]['data']['modelId'], new('const char* const [?]', #ID_Spec_Actors, ID_Spec_Actors),#ID_Spec_Actors)
                end
                imgui.Separator()
                if imgui.TreeNodeStr(langt['сhartics']) then
                    imgui.InputInt(langt['countlive'],vr.list_targets[vr.current_target[0]+1]['data']['health'])
                    
                    if imgui.ImageButton(weaponsAtlas, imgui.ImVec2(52,52),imgui.ImVec2((vr.list_targets[vr.current_target[0]+1]['data'].weapon[0]-1)*0.02272727272,0), imgui.ImVec2(vr.list_targets[vr.current_target[0]+1]['data'].weapon[0]*0.02272727272,1)) then
                        imgui.OpenPopup('weapon')
                    end

                    imgui.SameLine()
                    imgui.Text(langt["weapon"])

                    if imgui.InputInt(langt['ammo'], vr.list_targets[vr.current_target[0]+1]['data'].ammo) then
                        if vr.list_targets[vr.current_target[0]+1]['data'].ammo[0] < 0 then vr.list_targets[vr.current_target[0]+1]['data'].ammo[0] = 0 end 
                    end

                    --weapon popup
                    if imgui.BeginPopup('weapon') then
                        imgui.BeginChild('weapon',imgui.ImVec2(200,450))
                
                        for i = 1,#ID_Weapons do
                            imgui.PushIDStr(tostring(i))
                            if imgui.ImageButton(weaponsAtlas,imgui.ImVec2(52,52),imgui.ImVec2((i-1)*0.02272727272,0),imgui.ImVec2(i*0.02272727272,1)) then
                                vr.list_targets[vr.current_target[0]+1]['data'].weapon[0] = i
                            end
                            if imgui.IsItemHovered() then
                                imgui.SetTooltip(vr.temp_var.weap_names[i])
                            end
                            imgui.PopID()
                            if i % 3 ~= 0 then
                                imgui.SameLine()
                            end
                        end
                
                        imgui.EndChild()
                        imgui.EndPopup()
                    end

                    imgui.TreePop()
                end

                imgui.Separator()

                if imgui.Button(langt['edithand']) then
                    vr.temp_var.editmodeTeleportPlayer = true
                    vr.targets[0] = false
                    lockPlayerControl(false)
                end

                --skin popup
                if imgui.BeginPopup('skins') then
                    imgui.BeginChild('skins',imgui.ImVec2(210,450))
            

                    for i = 1,#ID_Actors do
                        imgui.PushIDStr(tostring(i))
                        if imgui.ImageButton(pedsSkinAtlas,imgui.ImVec2(55,100),imgui.ImVec2(((i-1)*55)/14630,0),imgui.ImVec2((i*55)/14630,1)) then
                            vr.list_targets[vr.current_target[0]+1]['data']['modelId'][0] = ID_Actors[i]
                        end
                        imgui.PopID()
                        if i % 3 ~= 0 then
                            imgui.SameLine()
                        end
                    end
            
                    imgui.EndChild()
                    imgui.EndPopup()
                end
            elseif vr.list_targets[vr.current_target[0]+1].targetType[0] == 1 then
                if imgui.Combo(langt['pack'],vr.list_targets[vr.current_target[0]+1]['data'].pack,new('const char* const [?]', #Anims.Anim_name, Anims.Anim_name),#Anims.Anim_name) then
                    vr.list_targets[vr.current_target[0]+1]['data'].anim[0] = 0
                end
                imgui.Combo(langt['anim'],vr.list_targets[vr.current_target[0]+1]['data'].anim,new('const char* const [?]', #Anims.Anim_list[vr.list_targets[vr.current_target[0]+1]['data'].pack[0]+1], Anims.Anim_list[vr.list_targets[vr.current_target[0]+1]['data'].pack[0]+1]),#Anims.Anim_list[vr.list_targets[vr.current_target[0]+1]['data'].pack[0]+1])
                mimgui_addons.ToggleButton(langt['looped'],vr.list_targets[vr.current_target[0]+1]['data'].looped)
                if imgui.Button(langt['preview']) then
                    if not hasAnimationLoaded(Anims['Anim_name'][vr.list_targets[vr.current_target[0]+1]['data'].pack[0]+1]) then
                        requestAnimation(Anims['Anim_name'][vr.list_targets[vr.current_target[0]+1]['data'].pack[0]+1])
                    end
                    lockPlayerControl(true)
                    local anims = Anims['Anim_list'][vr.list_targets[vr.current_target[0]+1]['data'].pack[0]+1]
                    taskPlayAnim(PLAYER_PED, anims[vr.list_targets[vr.current_target[0]+1]['data'].anim[0]+1], Anims['Anim_name'][vr.list_targets[vr.current_target[0]+1]['data'].pack[0]+1], 1.0, vr.list_targets[vr.current_target[0]+1]['data'].looped[0], false, false, false, -1)
                end
            elseif vr.list_targets[vr.current_target[0]+1].targetType[0] == 2 then
                imgui.Combo(langt['car'],vr.list_targets[vr.current_target[0]+1]['data']['car'],new('const char* const [?]', #vr.temp_var.list_name_cars, vr.temp_var.list_name_cars),#vr.temp_var.list_name_cars)
                imgui.SliderInt(langt['place'],vr.list_targets[vr.current_target[0]+1]['data'].carPlace,0,3,vr.temp_var.place_car[vr.list_targets[vr.current_target[0]+1]['data'].carPlace[0]+1])
            elseif vr.list_targets[vr.current_target[0]+1].targetType[0] == 3 then
                imgui.SliderInt(langt['levelp'],vr.list_targets[vr.current_target[0]+1]['data'].levelWanted,0,6,"%d stars")
            elseif vr.list_targets[vr.current_target[0]+1].targetType[0] == 5 then
                if imgui.Button(langt['add']) then
					vr.list_targets[vr.current_target[0]+1]['data']['dialogs'][#vr.list_targets[vr.current_target[0]+1]['data']['dialogs']+1] = {
					['text'] = new.char[129](),
					['textTime'] = new.float(0)
					}
                end
                imgui.SameLine()
                local rang = {}
                for i = 1,#vr.list_targets[vr.current_target[0]+1]['data']['dialogs'] do
                    rang[#rang+1] = tostring(i)
                end
                imgui.Combo(langt['number'],vr.list_targets[vr.current_target[0]+1]['data']['dialog'],new('const char* const [?]',#rang,rang),#rang)
				imgui.SameLine()
                if imgui.Button(langt['delete']) then
                    table.remove(vr.list_targets[vr.current_target[0]+1]['data']['dialogs'], vr.list_targets[vr.current_target[0]+1]['data']['dialog'][0]+1)
                    vr.list_targets[vr.current_target[0]+1]['data']['dialog'][0] = vr.list_targets[vr.current_target[0]+1]['data']['dialog'][0]-1
                end
				for d = 1,#vr.list_targets[vr.current_target[0]+1]['data']['dialogs'] do
					if imgui.TreeNodeStr(tostring(d)) then
                        imgui.InputText(langt['text'],vr.list_targets[vr.current_target[0]+1]['data']['dialogs'][d]['text'],ffi.sizeof(vr.list_targets[vr.current_target[0]+1]['data']['dialogs'][d]['text']))
                        imgui.InputFloat(langt['time'],vr.list_targets[vr.current_target[0]+1]['data']['dialogs'][d]['textTime'])
                        imgui.TreePop()
					end
                end
            elseif vr.list_targets[vr.current_target[0]+1].targetType[0] == 6 then
                imgui.DragInt(langt['add'],vr.list_targets[vr.current_target[0]+1]['data'].money,100,nil,nil,"%d$")
            end
        end
        imgui.End()
    end
end)

--actor
imgui.OnFrame(function() return vr.actors[0] end,
function()

    local isWindow = false

    if not vr.temp_var.editActor then

        imgui.SetNextWindowSize(imgui.ImVec2(270,410),imgui.Cond.Appearing)
        imgui.SetNextWindowPos(imgui.ImVec2(res.x-270,0),imgui.Cond.Appearing)
        imgui.Begin(faicons.ICON_MALE..' '..langt['actors'],nil, imgui.WindowFlags.AlwaysAutoResize)

        --List
        imgui.SetNextItemWidth(255)
        imgui.ListBoxStr_arr('', vr.current_actor,new('const char* const [?]', #vr.temp_var.list_name_actors, vr.temp_var.list_name_actors),#vr.temp_var.list_name_actors,15)

        if imgui.Button(langt['add']) then
            vr.list_actors[#vr.list_actors+1] = {
                ['name'] = new.char[65](langt['actor'].." #"..#vr.list_actors),
            }
            local px,py,pz = getCharCoordinates(PLAYER_PED)
            pz = getGroundZFor3dCoord(px,py,pz)
            local angle = getCharHeading(PLAYER_PED)
            vr.list_actors[#vr.list_actors].data = {
                ['pos'] = new.float[3](px,py,pz),
                ['angle'] = new.float(angle),
                ['modelId'] = new.int(0),
                ['modelType'] = new.int(0),
                ['slotSkin'] = new.int(1),
                ['startC'] = new.int(0),
                ['endC'] = new.int(0),
                ['shouldNotDie'] = new.bool(false),
                ['health'] = new.int(100),
                ['weapon'] = new.int(1),
                ['ammo'] = new.int(0),
                ['group'] = new.int(1),
                ["randomSpawn"] = new.bool(false),
                ["accuracy"] = new.int(false),
            }
            local actors_name = {}
            for i = 1,#vr.list_actors do
                actors_name[#actors_name+1] = vr.list_actors[i].name
            end
            vr.temp_var.list_name_actors = actors_name
            vr.current_actor[0] = #vr.list_actors-1
            upd_actor:run(vr.current_actor[0]+1)
        end
        if #vr.list_actors > 0 then
            imgui.SameLine()
            if imgui.Button(langt['rename']) then
                imgui.OpenPopup("rename")
            end
            imgui.SameLine()
            if imgui.Button(langt['delete']) then
                imgui.OpenPopup(faicons.ICON_TRASH_ALT.." "..langt['delete'])
            end
        end

        isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()


        if imgui.BeginPopupModal(faicons.ICON_TRASH_ALT.." "..langt['delete'], nil,imgui.WindowFlags.AlwaysAutoResize) then
            imgui.Text(langt['deleteQues'])

            local size_b = imgui.ImVec2(100,0)

            if imgui.Button(langt['yes'],size_b) then
                deleteChar(vr.list_actors[vr.current_actor[0]+1]['data']['char'])
                table.remove(vr.list_actors,vr.current_actor[0]+1)
                table.remove(vr.temp_var.list_name_actors,vr.current_actor[0]+1)
                if vr.current_actor[0] > 0 then 
                    vr.current_actor[0] = vr.current_actor[0] - 1
                end
                imgui.CloseCurrentPopup()
            end
            imgui.SameLine()
            if imgui.Button(langt['no'],size_b) then
                imgui.CloseCurrentPopup()
            end
        end

        --Rename popup
        if imgui.BeginPopup("rename") then

            imgui.InputText('',vr.list_actors[vr.current_actor[0]+1].name,ffi.sizeof(vr.list_actors[vr.current_actor[0]+1].name))

            if imgui.Button(langt['close']) then imgui.CloseCurrentPopup() end

            imgui.EndPopup()
        end

        imgui.End()

        -- actor render
        if #vr.list_actors > 0 then
            imgui.SetNextWindowSize(imgui.ImVec2(400,360),imgui.Cond.Always)
            imgui.SetNextWindowPos(imgui.ImVec2(res.x-670,0),imgui.Cond.Appearing)
            imgui.Begin(faicons.ICON_MALE..' '..langt['actor'],nil, imgui.WindowFlags.AlwaysAutoResize)


            lockPlayerControl(true)
            imgui.PushIDStr("position")
            if imgui.Button(faicons.ICON_STREET_VIEW) then
                local px,py,pz = getCharCoordinates(PLAYER_PED)
                pz = getGroundZFor3dCoord(px,py,pz)
                vr.list_actors[vr.current_actor[0]+1]['data'].pos[0] = px
                vr.list_actors[vr.current_actor[0]+1]['data'].pos[1] = py
                vr.list_actors[vr.current_actor[0]+1]['data'].pos[2] = pz
                setCharCoordinates(vr.list_actors[vr.current_actor[0]+1]['data']['char'],px,py,pz)
            end
            imgui.PopID()
            if imgui.IsItemHovered() then
                imgui.SetTooltip(langt['playerCoordinates'])
            end

            imgui.SameLine()
            imgui.PushItemWidth(270)
            if imgui.InputFloat3(langt['position'],vr.list_actors[vr.current_actor[0]+1]['data']['pos'],"%.6f") then
                local xx,xy,xz = vr.list_actors[vr.current_actor[0]+1]['data']['pos'][0],vr.list_actors[vr.current_actor[0]+1]['data']['pos'][1],vr.list_actors[vr.current_actor[0]+1]['data']['pos'][2]
                setCharCoordinates(vr.list_actors[vr.current_actor[0]+1]['data']['char'],xx,xy,xz)
            end

            imgui.PushIDStr("angle")
            if imgui.Button(faicons.ICON_STREET_VIEW) then
                local angle = getCharHeading(PLAYER_PED)
                vr.list_actors[vr.current_actor[0]+1]['data'].angle[0] = angle
                setCharHeading(vr.list_actors[vr.current_actor[0]+1]['data']['char'], angle)
            end
            imgui.PopID()
            if imgui.IsItemHovered() then
                imgui.SetTooltip(langt['playerAngle'])
            end

            imgui.SameLine()
            if imgui.DragFloat(langt['angle'], vr.list_actors[vr.current_actor[0]+1]['data'].angle,1,0,360) then
                setCharHeading(vr.list_actors[vr.current_actor[0]+1]['data']['char'], vr.list_actors[vr.current_actor[0]+1]['data']['angle'][0])
            end

            imgui.Separator()

            if imgui.SliderInt('',vr.list_actors[vr.current_actor[0]+1]['data'].modelType,0,1,tostring(fif(vr.list_actors[vr.current_actor[0]+1]['data'].modelType[0] == 0,langt['ped'],langt['specActor']))) then
                vr.list_actors[vr.current_actor[0]+1]['data']['modelId'][0] = 0
                upd_actor:run(vr.current_actor[0]+1)
            end

            
            --skin popup open
            if vr.list_actors[vr.current_actor[0]+1]['data'].modelType[0] == 0 then
                if imgui.Button(faicons.ICON_TSHIRT) then
                    imgui.OpenPopup('skins')
                end
                

                imgui.SameLine()

                if imgui.InputInt(langt['model'],vr.list_actors[vr.current_actor[0]+1]['data'].modelId) then
                    local id_a = 0
                    for v = 1,#ID_Actors do
                        if vr.list_actors[vr.current_actor[0]+1]['data']['modelId'][0] <= ID_Actors[v] then
                            id_a = ID_Actors[v]
                            break
                        end
                    end
                    vr.list_actors[vr.current_actor[0]+1]['data']['modelId'][0] = id_a
                    upd_actor:run(vr.current_actor[0]+1)
                end
            else
                if imgui.SliderInt(langt['slot'], vr.list_actors[vr.current_actor[0]+1]['data'].slotSkin, 1, 10) or imgui.Combo(langt['model'], vr.list_actors[vr.current_actor[0]+1]['data']['modelId'], new('const char* const [?]', #ID_Spec_Actors, ID_Spec_Actors),#ID_Spec_Actors) then
                    upd_actor:run(vr.current_actor[0]+1)
                end
            end
            imgui.Separator()

            if imgui.TreeNodeStr(langt['сhartics']) then
                imgui.InputInt(langt['countlive'],vr.list_actors[vr.current_actor[0]+1]['data']['health'])
                
                if imgui.ImageButton(weaponsAtlas, imgui.ImVec2(52,52),imgui.ImVec2((vr.list_actors[vr.current_actor[0]+1]['data'].weapon[0]-1)*0.02272727272,0), imgui.ImVec2(vr.list_actors[vr.current_actor[0]+1]['data'].weapon[0]*0.02272727272,1)) then
                    imgui.OpenPopup('weapon')
                end

                imgui.SameLine()
                imgui.Text(langt["weapon"])

                if imgui.InputInt(langt['ammo'], vr.list_actors[vr.current_actor[0]+1]['data'].ammo) then
                    if vr.list_actors[vr.current_actor[0]+1]['data'].ammo[0] < 0 then vr.list_actors[vr.current_actor[0]+1]['data'].ammo[0] = 0 end 
                end

                imgui.DragInt(langt['accuracy'], vr.list_actors[vr.current_actor[0]+1]['data'].accuracy, 0.5, 0, 100, "%d %%")                

                 --weapon popup
                if imgui.BeginPopup('weapon') then
                    imgui.BeginChild('weapon',imgui.ImVec2(200,450))
            
                    for i = 1,#ID_Weapons do
                        imgui.PushIDStr(tostring(i))
                        if imgui.ImageButton(weaponsAtlas,imgui.ImVec2(52,52),imgui.ImVec2((i-1)*0.02272727272,0),imgui.ImVec2(i*0.02272727272,1)) then
                            vr.list_actors[vr.current_actor[0]+1]['data'].weapon[0] = i
                            upd_actor:run(vr.current_actor[0]+1)
                        end
                        if imgui.IsItemHovered() then
                            imgui.SetTooltip(vr.temp_var.weap_names[i])
                        end
                        imgui.PopID()
                        if i % 3 ~= 0 then
                            imgui.SameLine()
                        end
                    end
            
                    imgui.EndChild()
                    imgui.EndPopup()
                end

                imgui.SliderInt(langt['group'], vr.list_actors[vr.current_actor[0]+1]['data'].group, 0,8,vr.temp_var.name_groups[vr.list_actors[vr.current_actor[0]+1]['data'].group[0]+1])
                
                imgui.TreePop()
            end
            
            mimgui_addons.ToggleButton(langt['randomSpawnW'],vr.list_actors[vr.current_actor[0]+1]['data'].randomSpawn)
            mimgui_addons.ToggleButton(langt['should_live'],vr.list_actors[vr.current_actor[0]+1]['data']['shouldNotDie'])
            

            imgui.Separator()

            if imgui.Button(langt['edithand']) then
                vr.temp_var.editActor = true
                deleteChar(vr.list_actors[vr.current_actor[0]+1]['data']['char'])


                
                local modelId = vr.list_actors[vr.current_actor[0]+1]['data']['modelId'][0]
                local weapon = vr.list_actors[vr.current_actor[0]+1]['data']['weapon'][0]
                if vr.list_actors[vr.current_actor[0]+1]['data']['modelType'][0] == 0 then
                    modell = vr.list_actors[vr.current_actor[0]+1]['data']['modelId'][0]
                    requestModel(modell)
                    while not hasModelLoaded(modell) do
                        wait(0)
                    end
                else
                    local modell_n = ID_Spec_Actors[vr.list_actors[vr.current_actor[0]+1]['data']['modelId'][0]+1]
                    loadSpecialCharacter(modell_n,vr.list_actors[vr.current_actor[0]+1]['data']['slotSkin'][0])
                    while not hasSpecialCharacterLoaded(vr.list_actors[vr.current_actor[0]+1]['data']['slotSkin'][0]) do
                        wait(0)
                    end
                    modell = 290 + vr.list_actors[vr.current_actor[0]+1]['data']['slotSkin'][0]-1
                end
                setPlayerModel(PLAYER_HANDLE, modell)
                local xx,xy,xz = vr.list_actors[vr.current_actor[0]+1]['data']['pos'][0],vr.list_actors[vr.current_actor[0]+1]['data']['pos'][1],vr.list_actors[vr.current_actor[0]+1]['data']['pos'][2]
                setCharCoordinates(PLAYER_PED, xx, xy, xz)
                setCharHeading(PLAYER_PED, vr.list_actors[vr.current_actor[0]+1]['data']['angle'][0])
            end
            
            imgui.PushItemWidth(150)
            imgui.Separator()
            
            local list_tg_m = {langt['toend']}
            for ltg = 1,#vr.temp_var.list_name_targets do
                list_tg_m[#list_tg_m+1] = vr.temp_var.list_name_targets[ltg]
            end
            imgui.Combo(langt['app_on'],vr.list_actors[vr.current_actor[0]+1]['data']['startC'],new('const char* const [?]',#vr.temp_var.list_name_targets,vr.temp_var.list_name_targets),#vr.temp_var.list_name_targets)
            imgui.Combo(langt['dis_after'],vr.list_actors[vr.current_actor[0]+1]['data']['endC'],new('const char* const [?]',#list_tg_m,list_tg_m),#list_tg_m)
            

            --edit
            imgui.SetNextWindowBgAlpha(0.50)
            imgui.SetNextWindowPos(imgui.ImVec2(0, 0), imgui.Cond.Always)
            imgui.SetNextWindowSize(imgui.ImVec2(220,60),imgui.Cond.Always)
            imgui.Begin("info",nil,imgui.WindowFlags.NoDecoration + imgui.WindowFlags.AlwaysAutoResize + imgui.WindowFlags.NoSavedSettings + imgui.WindowFlags.NoMove + imgui.WindowFlags.NoInputs)

            imgui.Text(vr.temp_var.infoOverlay[1])
            imgui.Text(vr.temp_var.infoOverlay[2])

            imgui.End()

            isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()

            
            local cx,cy,cz = vr.list_actors[vr.current_actor[0]+1]['data'].pos[0],vr.list_actors[vr.current_actor[0]+1]['data'].pos[1],vr.list_actors[vr.current_actor[0]+1]['data'].pos[2]
            cx = cx + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
            cy = cy + (vr.camera_zoom*math.cos(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
            cz = cz + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[2])))
            setFixedCameraPosition(cx,cy,cz)
            pointCameraAtPoint(vr.list_actors[vr.current_actor[0]+1]['data'].pos[0],vr.list_actors[vr.current_actor[0]+1]['data'].pos[1],vr.list_actors[vr.current_actor[0]+1]['data'].pos[2]+1,2)
            if not isWindow then
                if imgui.IsMouseDragging(2) then
                    local dt = imgui.GetIO().MouseDelta
                    vr.camera_angle[1],vr.camera_angle[2] = vr.camera_angle[1] + dt.x, vr.camera_angle[2] + dt.y
                end
                if imgui.GetIO().MouseWheel ~= 0 then
                    vr.camera_zoom = vr.camera_zoom + (vr.camera_zoom * imgui.GetIO().MouseWheel)/4
                    if vr.camera_zoom < 1 then vr.camera_zoom = 1 end
                end
            end

            --skin popup
            if imgui.BeginPopup('skins') then
                imgui.BeginChild('skins',imgui.ImVec2(210,450))
        

                for i = 1,#ID_Actors do
                    imgui.PushIDStr(tostring(i))
                    if imgui.ImageButton(pedsSkinAtlas,imgui.ImVec2(55,100),imgui.ImVec2(((i-1)*55)/14630,0),imgui.ImVec2((i*55)/14630,1)) then
                        vr.list_actors[vr.current_actor[0]+1]['data']['modelId'][0] = ID_Actors[i]
                        upd_actor:run(vr.current_actor[0]+1)
                    end
                    imgui.PopID()
                    if i % 3 ~= 0 then
                        imgui.SameLine()
                    end
                end
        
                imgui.EndChild()
                imgui.EndPopup()
            end

            

            imgui.End()
        end
    else
        imgui.SetNextWindowBgAlpha(0.50)
        imgui.SetNextWindowPos(imgui.ImVec2(0, 0), imgui.Cond.Always)
        imgui.SetNextWindowSize(imgui.ImVec2(220,20),imgui.Cond.Always)
        imgui.Begin("info",nil,imgui.WindowFlags.NoDecoration + imgui.WindowFlags.AlwaysAutoResize + imgui.WindowFlags.NoSavedSettings + imgui.WindowFlags.NoMove + imgui.WindowFlags.NoInputs)

        imgui.Text(vr.temp_var.infoOverlay[4])

        imgui.End()
    end
end)

--car
imgui.OnFrame(function() return vr.cars[0] end,
function()

    local isWindow = false

    if not vr.temp_var.editcar then

        imgui.SetNextWindowSize(imgui.ImVec2(270,410),imgui.Cond.Appearing)
        imgui.SetNextWindowPos(imgui.ImVec2(res.x-270,0),imgui.Cond.Appearing)
        imgui.Begin(faicons.ICON_MALE..' '..langt['cars'],nil, imgui.WindowFlags.AlwaysAutoResize)

        --List
        imgui.SetNextItemWidth(255)
        imgui.ListBoxStr_arr('', vr.current_car,new('const char* const [?]', #vr.temp_var.list_name_cars, vr.temp_var.list_name_cars),#vr.temp_var.list_name_cars,15)

        if imgui.Button(langt['add']) then
            vr.list_cars[#vr.list_cars+1] = {
                ['name'] = new.char[65](langt['car'].." #"..#vr.list_cars),
            }
            local px,py,pz = getCharCoordinates(PLAYER_PED)
            pz = getGroundZFor3dCoord(px,py,pz)
            local angle = getCharHeading(PLAYER_PED)
            vr.list_cars[#vr.list_cars].data = {
                ['pos'] = new.float[3](px,py,pz),
                ['angle'] = new.float(angle),
                ['modelId'] = new.int(400),
                ['slotSkin'] = new.int(1),
                ['startC'] = new.int(0),
                ['endC'] = new.int(0),
				['shouldNotDie'] = new.bool(false),
				['health'] = new.int(1000),
				['bulletproof'] = new.bool(false),
				['fireproof'] = new.bool(false),
				['explosionproof'] = new.bool(false),
				['collisionproof'] = new.bool(false),
				['meleeproof'] = new.bool(false),
                ['tiresVulnerability'] = new.bool(false),
                ['extendedColor'] = new.bool(false),
                ['primaryColor'] = new.float[4](),
                ['secondaryColor'] = new.float[4](),

            }
            local cars_name = {}
            for i = 1,#vr.list_cars do
                cars_name[#cars_name+1] = vr.list_cars[i].name
            end
            vr.temp_var.list_name_cars = cars_name
            vr.current_car[0] = #vr.list_cars-1
            upd_car:run(vr.current_car[0]+1,true)
        end
        if #vr.list_cars > 0 then
            imgui.SameLine()
            if imgui.Button(langt['rename']) then
                imgui.OpenPopup("rename")
            end
            imgui.SameLine()
            if imgui.Button(langt['delete']) then
                imgui.OpenPopup(faicons.ICON_TRASH_ALT.." "..langt['delete'])
            end
        end

        isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()


        if imgui.BeginPopupModal(faicons.ICON_TRASH_ALT.." "..langt['delete'], nil,imgui.WindowFlags.AlwaysAutoResize) then
            imgui.Text(langt['deleteQues'])

            local size_b = imgui.ImVec2(100,0)

            if imgui.Button(langt['yes'],size_b) then
                deleteCar(vr.list_cars[vr.current_car[0]+1]['data']['car'])
                table.remove(vr.list_cars,vr.current_car[0]+1)
                table.remove(vr.temp_var.list_name_cars,vr.current_car[0]+1)
                if vr.current_car[0] > 0 then 
                    vr.current_car[0] = vr.current_car[0] - 1
                end
                imgui.CloseCurrentPopup()
            end
            imgui.SameLine()
            if imgui.Button(langt['no'],size_b) then
                imgui.CloseCurrentPopup()
            end
        end

        --Rename popup
        if imgui.BeginPopup("rename") then

            imgui.InputText('',vr.list_cars[vr.current_car[0]+1].name,ffi.sizeof(vr.list_cars[vr.current_car[0]+1].name))

            if imgui.Button(langt['close']) then imgui.CloseCurrentPopup() end

            imgui.EndPopup()
        end

        imgui.End()

        -- car render
        if #vr.list_cars > 0 then
            imgui.SetNextWindowSize(imgui.ImVec2(400,360),imgui.Cond.Always)
            imgui.SetNextWindowPos(imgui.ImVec2(res.x-670,0),imgui.Cond.Appearing)
            imgui.Begin(faicons.ICON_MALE..' '..langt['car'],nil, imgui.WindowFlags.AlwaysAutoResize)


            lockPlayerControl(true)
            imgui.PushIDStr("position")
            if imgui.Button(faicons.ICON_STREET_VIEW) then
                local px,py,pz = getCharCoordinates(PLAYER_PED)
                pz = getGroundZFor3dCoord(px,py,pz)
                vr.list_cars[vr.current_car[0]+1]['data'].pos[0] = px
                vr.list_cars[vr.current_car[0]+1]['data'].pos[1] = py
                vr.list_cars[vr.current_car[0]+1]['data'].pos[2] = pz
                setCarCoordinates(vr.list_cars[vr.current_car[0]+1]['data']['car'],px,py,pz)
            end
            imgui.PopID()
            if imgui.IsItemHovered() then
                imgui.SetTooltip(langt['playerCoordinates'])
            end

            imgui.SameLine()
            imgui.PushItemWidth(270)
            if imgui.InputFloat3(langt['position'],vr.list_cars[vr.current_car[0]+1]['data']['pos'],"%.6f") then
                local xx,xy,xz = vr.list_cars[vr.current_car[0]+1]['data']['pos'][0],vr.list_cars[vr.current_car[0]+1]['data']['pos'][1],vr.list_cars[vr.current_car[0]+1]['data']['pos'][2]
                setCarCoordinates(vr.list_cars[vr.current_car[0]+1]['data']['car'],xx,xy,xz)
            end

            imgui.PushIDStr("angle")
            if imgui.Button(faicons.ICON_STREET_VIEW) then
                local angle = getCharHeading(PLAYER_PED)
                vr.list_cars[vr.current_car[0]+1]['data'].angle[0] = angle
                setCarHeading(vr.list_cars[vr.current_car[0]+1]['data']['car'], angle)
            end
            imgui.PopID()
            if imgui.IsItemHovered() then
                imgui.SetTooltip(langt['playerAngle'])
            end

            imgui.SameLine()
            if imgui.DragFloat(langt['angle'], vr.list_cars[vr.current_car[0]+1]['data'].angle,1,0,360) then
                setCarHeading(vr.list_cars[vr.current_car[0]+1]['data']['car'], vr.list_cars[vr.current_car[0]+1]['data']['angle'][0])
            end

            if imgui.Button(langt['selectCar']) then
                carSelector.showw(s)
                vr.cars[0] = false
            end

            imgui.Separator()

            if imgui.TreeNodeStr(langt['сhartics']) then
                
                mimgui_addons.ToggleButton(langt['extendedColor'],vr.list_cars[vr.current_car[0]+1]['data'].extendedColor)
                
                if vr.list_cars[vr.current_car[0]+1]['data'].extendedColor[0] then
                    if imgui.TreeNodeStr(langt['colors']) then
                        for_each_vehicle_material(vr.list_cars[vr.current_car[0]+1]['data'].car,function(i,mat, comp, obj)
                            
                            imgui.PushIDInt(i)
                            imgui.PushItemWidth(200)
                            if imgui.ColorEdit4(tostring(i), vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2],imgui.ColorEditFlags.AlphaBar+imgui.ColorEditFlags.NoInputs+imgui.ColorEditFlags.NoLabel) then
                                local new_r, new_g, new_b, a = vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][0],vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][1],vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][2],vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][3]
                                mat:set_color(new_r*255, new_g*255, new_b*255, a*255)
                            end
                            imgui.SameLine()
                            imgui.Text(string.format('%s:%s:%s', comp.name, mat:get_texture() and mat:get_texture().name or '[no texture]', bit.tohex(mat:get_pointer())))
                            imgui.PopItemWidth()
                            imgui.PopID()

                        end)
                    end
                else
                    if imgui.ColorEdit4("primaryColorE", vr.list_cars[vr.current_car[0]+1]['data'].primaryColor,imgui.ColorEditFlags.AlphaBar+imgui.ColorEditFlags.NoInputs+imgui.ColorEditFlags.NoLabel) then
                        for_each_vehicle_material(vr.list_cars[vr.current_car[0]+1]['data'].car,function(i,mat, comp, obj)
                            if vr.list_cars[vr.current_car[0]+1]['data'].colors[i][1] == 0 then
                                local r, g, b, a = vr.list_cars[vr.current_car[0]+1]['data'].primaryColor[0],vr.list_cars[vr.current_car[0]+1]['data'].primaryColor[1],vr.list_cars[vr.current_car[0]+1]['data'].primaryColor[2],vr.list_cars[vr.current_car[0]+1]['data'].primaryColor[3]
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][0] = r
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][1] = g
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][2] = b
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][3] = a
                                    mat:set_color(r*255, g*255, b*255, a*255)
                                end
                        end)
                    end
                    imgui.SameLine()
                    imgui.Text(langt['primaryColor'])
                    
                    if imgui.ColorEdit4("secondaryColorE", vr.list_cars[vr.current_car[0]+1]['data'].secondaryColor,imgui.ColorEditFlags.AlphaBar+imgui.ColorEditFlags.NoInputs+imgui.ColorEditFlags.NoLabel) then
                        for_each_vehicle_material(vr.list_cars[vr.current_car[0]+1]['data'].car,function(i,mat, comp, obj)
                            if vr.list_cars[vr.current_car[0]+1]['data'].colors[i][1] == 1 then
                                local r, g, b, a = vr.list_cars[vr.current_car[0]+1]['data'].secondaryColor[0],vr.list_cars[vr.current_car[0]+1]['data'].secondaryColor[1],vr.list_cars[vr.current_car[0]+1]['data'].secondaryColor[2],vr.list_cars[vr.current_car[0]+1]['data'].secondaryColor[3]
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][0] = r
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][1] = g
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][2] = b
                                    vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][3] = a
                                    mat:set_color(r*255, g*255, b*255, a*255)
                            end
                        end)
                    end
                    imgui.SameLine()
                    imgui.Text(langt['secondaryColor'])
                end
                
                imgui.Separator()
                
                imgui.InputInt(langt['countlive'],vr.list_cars[vr.current_car[0]+1]['data']['health'])
                
                mimgui_addons.ToggleButton(vr.temp_var.car_unbreak[1],vr.list_cars[vr.current_car[0]+1]['data']['bulletproof'])
                mimgui_addons.ToggleButton(vr.temp_var.car_unbreak[2],vr.list_cars[vr.current_car[0]+1]['data']['fireproof'])
                mimgui_addons.ToggleButton(vr.temp_var.car_unbreak[3],vr.list_cars[vr.current_car[0]+1]['data']['explosionproof'])
                mimgui_addons.ToggleButton(vr.temp_var.car_unbreak[4],vr.list_cars[vr.current_car[0]+1]['data']['collisionproof'])
                mimgui_addons.ToggleButton(vr.temp_var.car_unbreak[5],vr.list_cars[vr.current_car[0]+1]['data']['meleeproof'])
                mimgui_addons.ToggleButton(vr.temp_var.car_unbreak[6],vr.list_cars[vr.current_car[0]+1]['data']['tiresVulnerability'])
                
                imgui.TreePop()


            end

            mimgui_addons.ToggleButton(langt['should_live'],vr.list_cars[vr.current_car[0]+1]['data']['shouldNotDie'])
            



            imgui.Separator()

            if imgui.Button(langt['edithand']) then
                vr.temp_var.editmodeCar = true
                deleteCar(vr.list_cars[vr.current_car[0]+1]['data']['car'])
                lockPlayerControl(fasle)
                vr.cars[0] = false
            end
            
            imgui.PushItemWidth(150)
            imgui.Separator()
            
            local list_tg_m = {langt['toend']}
            for ltg = 1,#vr.temp_var.list_name_targets do
                list_tg_m[#list_tg_m+1] = vr.temp_var.list_name_targets[ltg]
            end
            imgui.Combo(langt['app_on'],vr.list_cars[vr.current_car[0]+1]['data']['startC'],new('const char* const [?]',#vr.temp_var.list_name_targets,vr.temp_var.list_name_targets),#vr.temp_var.list_name_targets)
            imgui.Combo(langt['dis_after'],vr.list_cars[vr.current_car[0]+1]['data']['endC'],new('const char* const [?]',#list_tg_m,list_tg_m),#list_tg_m)
            

            --edit
            imgui.SetNextWindowBgAlpha(0.50)
            imgui.SetNextWindowPos(imgui.ImVec2(0, 0), imgui.Cond.Always)
            imgui.SetNextWindowSize(imgui.ImVec2(220,60),imgui.Cond.Always)
            imgui.Begin("info",nil,imgui.WindowFlags.NoDecoration + imgui.WindowFlags.AlwaysAutoResize + imgui.WindowFlags.NoSavedSettings + imgui.WindowFlags.NoMove + imgui.WindowFlags.NoInputs)

            imgui.Text(vr.temp_var.infoOverlay[1])
            imgui.Text(vr.temp_var.infoOverlay[2])

            imgui.End()

            isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()

            
            local cx,cy,cz = vr.list_cars[vr.current_car[0]+1]['data'].pos[0],vr.list_cars[vr.current_car[0]+1]['data'].pos[1],vr.list_cars[vr.current_car[0]+1]['data'].pos[2]
            cx = cx + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
            cy = cy + (vr.camera_zoom*math.cos(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
            cz = cz + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[2])))
            setFixedCameraPosition(cx,cy,cz)
            pointCameraAtPoint(vr.list_cars[vr.current_car[0]+1]['data'].pos[0],vr.list_cars[vr.current_car[0]+1]['data'].pos[1],vr.list_cars[vr.current_car[0]+1]['data'].pos[2]+1,2)
            if not isWindow then
                if imgui.IsMouseDragging(2) then
                    local dt = imgui.GetIO().MouseDelta
                    vr.camera_angle[1],vr.camera_angle[2] = vr.camera_angle[1] + dt.x, vr.camera_angle[2] + dt.y
                end
                if imgui.GetIO().MouseWheel ~= 0 then
                    vr.camera_zoom = vr.camera_zoom + (vr.camera_zoom * imgui.GetIO().MouseWheel)/4
                    if vr.camera_zoom < 1 then vr.camera_zoom = 1 end
                end
            end

            imgui.End()
        end
    else
        imgui.SetNextWindowBgAlpha(0.50)
        imgui.SetNextWindowPos(imgui.ImVec2(0, 0), imgui.Cond.Always)
        imgui.SetNextWindowSize(imgui.ImVec2(220,20),imgui.Cond.Always)
        imgui.Begin("info",nil,imgui.WindowFlags.NoDecoration + imgui.WindowFlags.AlwaysAutoResize + imgui.WindowFlags.NoSavedSettings + imgui.WindowFlags.NoMove + imgui.WindowFlags.NoInputs)

        imgui.Text(vr.temp_var.infoOverlay[4])

        imgui.End()
    end
end)

--object
imgui.OnFrame(function() return vr.objects[0] end,
function()
    lockPlayerControl(true)
    local isWindow = false

    imgui.SetNextWindowSize(imgui.ImVec2(270,410),imgui.Cond.Appearing)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x-270,0),imgui.Cond.Appearing)
    imgui.Begin(faicons.ICON_MALE..' '..langt['objects'],nil, imgui.WindowFlags.AlwaysAutoResize)

    --List
    imgui.SetNextItemWidth(255)
    imgui.ListBoxStr_arr('', vr.current_object,new('const char* const [?]', #vr.temp_var.list_name_objects, vr.temp_var.list_name_objects),#vr.temp_var.list_name_objects,15)

    if imgui.Button(langt['add']) then
        vr.list_objects[#vr.list_objects+1] = {
            ['name'] = new.char[65](langt['object'].." #"..#vr.list_objects),
        }
        local px,py,pz = getCharCoordinates(PLAYER_PED)
        pz = getGroundZFor3dCoord(px,py,pz)
        local angle = getCharHeading(PLAYER_PED)
        vr.list_objects[#vr.list_objects].data = {
            ['pos'] = new.float[3](px,py,pz),
            ['rotate'] = new.int[3](0,0,0),
            ['modelId'] = new.int(),
            ['startC'] = new.int(0),
            ['endC'] = new.int(0),
        }
        local objects_name = {}
        for i = 1,#vr.list_objects do
            objects_name[#objects_name+1] = vr.list_objects[i].name
        end
        vr.temp_var.list_name_objects = objects_name
        vr.current_object[0] = #vr.list_objects-1
        upd_object:run(vr.current_object[0]+1)
    end
    if #vr.list_objects > 0 then
        imgui.SameLine()
        if imgui.Button(langt['rename']) then
            imgui.OpenPopup("rename")
        end
        imgui.SameLine()
        if imgui.Button(langt['delete']) then
            imgui.OpenPopup(faicons.ICON_TRASH_ALT.." "..langt['delete'])
        end
    end

    isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()


    if imgui.BeginPopupModal(faicons.ICON_TRASH_ALT.." "..langt['delete'], nil,imgui.WindowFlags.AlwaysAutoResize) then
        imgui.Text(langt['deleteQues'])

        local size_b = imgui.ImVec2(100,0)

        if imgui.Button(langt['yes'],size_b) then
            deleteObject(vr.list_objects[vr.current_object[0]+1]['data']['obj'])
            table.remove(vr.list_objects,vr.current_object[0]+1)
            table.remove(vr.temp_var.list_name_objects,vr.current_object[0]+1)
            if vr.current_object[0] > 0 then
                vr.current_object[0] = vr.current_object[0] - 1
            end
            imgui.CloseCurrentPopup()
        end
        imgui.SameLine()
        if imgui.Button(langt['no'],size_b) then
            imgui.CloseCurrentPopup()
        end
    end

    --Rename popup
    if imgui.BeginPopup("rename") then

        imgui.InputText('',vr.list_objects[vr.current_object[0]+1].name,ffi.sizeof(vr.list_objects[vr.current_object[0]+1].name))

        if imgui.Button(langt['close']) then imgui.CloseCurrentPopup() end

        imgui.EndPopup()
    end

    imgui.End()

    -- object render
    if #vr.list_objects > 0 then
        imgui.SetNextWindowSize(imgui.ImVec2(400,360),imgui.Cond.Always)
        imgui.SetNextWindowPos(imgui.ImVec2(res.x-670,0),imgui.Cond.Appearing)
        imgui.Begin(faicons.ICON_ARCHIVE..' '..langt['object'],nil, imgui.WindowFlags.AlwaysAutoResize)
    
        imgui.PushIDStr("position")
        if imgui.Button(faicons.ICON_STREET_VIEW) then
            local px,py,pz = getCharCoordinates(PLAYER_PED)
            pz = getGroundZFor3dCoord(px,py,pz)
            vr.list_objects[vr.current_object[0]+1]['data'].pos[0] = px
            vr.list_objects[vr.current_object[0]+1]['data'].pos[1] = py
            vr.list_objects[vr.current_object[0]+1]['data'].pos[2] = pz
            setObjectCoordinates(vr.list_objects[vr.current_object[0]+1]['data']['obj'],px,py,pz)
        end
        imgui.PopID()
        if imgui.IsItemHovered() then
            imgui.SetTooltip(langt['playerCoordinates'])
        end

        imgui.SameLine()
        imgui.PushItemWidth(270)
        if imgui.InputFloat3(langt['position'],vr.list_objects[vr.current_object[0]+1]['data']['pos'],"%.6f") then
            local xx,xy,xz = vr.list_objects[vr.current_object[0]+1]['data']['pos'][0],vr.list_objects[vr.current_object[0]+1]['data']['pos'][1],vr.list_objects[vr.current_object[0]+1]['data']['pos'][2]
            setObjectCoordinates(vr.list_objects[vr.current_object[0]+1]['data']['obj'],xx,xy,xz)
        end
        if imgui.DragInt3(langt['rotate'],vr.list_objects[vr.current_object[0]+1]['data']['rotate'],0.3,-360,360,"%d°") then
            local xr,yr,zr = vr.list_objects[vr.current_object[0]+1]['data']['rotate'][0],vr.list_objects[vr.current_object[0]+1]['data']['rotate'][1],vr.list_objects[vr.current_object[0]+1]['data']['rotate'][2]
            setObjectRotation(vr.list_objects[vr.current_object[0]+1]['data']['obj'],xr,yr,zr)
        end
        imgui.InputInt(langt['model'],vr.list_objects[vr.current_object[0]+1]['data'].modelId)
        if imgui.Button(langt['apply']) then
            upd_object:run(vr.current_object[0]+1)
        end
        local list_tg_m = {langt['toend']}
        for ltg = 1,#vr.temp_var.list_name_targets do
            list_tg_m[#list_tg_m+1] = vr.temp_var.list_name_targets[ltg]
        end
        imgui.Combo(langt['app_on'],vr.list_objects[vr.current_object[0]+1]['data']['startC'],new('const char* const [?]',#vr.temp_var.list_name_targets,vr.temp_var.list_name_targets),#vr.temp_var.list_name_targets)
        imgui.Combo(langt['dis_after'],vr.list_objects[vr.current_object[0]+1]['data']['endC'],new('const char* const [?]',#list_tg_m,list_tg_m),#list_tg_m)
        

        --edit
        local cx,cy,cz = vr.list_objects[vr.current_object[0]+1]['data'].pos[0],vr.list_objects[vr.current_object[0]+1]['data'].pos[1],vr.list_objects[vr.current_object[0]+1]['data'].pos[2]
        cx = cx + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
        cy = cy + (vr.camera_zoom*math.cos(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
        cz = cz + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[2])))
        setFixedCameraPosition(cx,cy,cz)
        pointCameraAtPoint(vr.list_objects[vr.current_object[0]+1]['data'].pos[0],vr.list_objects[vr.current_object[0]+1]['data'].pos[1],vr.list_objects[vr.current_object[0]+1]['data'].pos[2],2)
        if not isWindow then
            if imgui.IsMouseDragging(2) then
                local dt = imgui.GetIO().MouseDelta
                vr.camera_angle[1],vr.camera_angle[2] = vr.camera_angle[1] + dt.x, vr.camera_angle[2] + dt.y
            end
            if imgui.GetIO().MouseWheel ~= 0 then
                vr.camera_zoom = vr.camera_zoom + (vr.camera_zoom * imgui.GetIO().MouseWheel)/4
                if vr.camera_zoom < 1 then vr.camera_zoom = 1 end
            end
        end
        if isKeyDown(vkeys.VK_UP) then
            vr.list_objects[vr.current_object[0]+1]['data'].pos[0] = vr.list_objects[vr.current_object[0]+1]['data'].pos[0] + 0.1
            local xx,xy,xz = vr.list_objects[vr.current_object[0]+1]['data']['pos'][0],vr.list_objects[vr.current_object[0]+1]['data']['pos'][1],vr.list_objects[vr.current_object[0]+1]['data']['pos'][2]
            setObjectCoordinates(vr.list_objects[vr.current_object[0]+1]['data']['obj'],xx,xy,xz)
        elseif isKeyDown(vkeys.VK_DOWN) then
            vr.list_objects[vr.current_object[0]+1]['data'].pos[0] = vr.list_objects[vr.current_object[0]+1]['data'].pos[0] - 0.1
            local xx,xy,xz = vr.list_objects[vr.current_object[0]+1]['data']['pos'][0],vr.list_objects[vr.current_object[0]+1]['data']['pos'][1],vr.list_objects[vr.current_object[0]+1]['data']['pos'][2]
            setObjectCoordinates(vr.list_objects[vr.current_object[0]+1]['data']['obj'],xx,xy,xz)
        end
        if isKeyDown(vkeys.VK_LEFT) then
            vr.list_objects[vr.current_object[0]+1]['data'].pos[1] = vr.list_objects[vr.current_object[0]+1]['data'].pos[1] + 0.1
            local xx,xy,xz = vr.list_objects[vr.current_object[0]+1]['data']['pos'][0],vr.list_objects[vr.current_object[0]+1]['data']['pos'][1],vr.list_objects[vr.current_object[0]+1]['data']['pos'][2]
            setObjectCoordinates(vr.list_objects[vr.current_object[0]+1]['data']['obj'],xx,xy,xz)
        elseif isKeyDown(vkeys.VK_RIGHT) then
            vr.list_objects[vr.current_object[0]+1]['data'].pos[1] = vr.list_objects[vr.current_object[0]+1]['data'].pos[1] - 0.1
            local xx,xy,xz = vr.list_objects[vr.current_object[0]+1]['data']['pos'][0],vr.list_objects[vr.current_object[0]+1]['data']['pos'][1],vr.list_objects[vr.current_object[0]+1]['data']['pos'][2]
            setObjectCoordinates(vr.list_objects[vr.current_object[0]+1]['data']['obj'],xx,xy,xz)
        end
        if isKeyDown(vkeys.VK_Q) then
            vr.list_objects[vr.current_object[0]+1]['data'].pos[2] = vr.list_objects[vr.current_object[0]+1]['data'].pos[2] + 0.1
            local xx,xy,xz = vr.list_objects[vr.current_object[0]+1]['data']['pos'][0],vr.list_objects[vr.current_object[0]+1]['data']['pos'][1],vr.list_objects[vr.current_object[0]+1]['data']['pos'][2]
            setObjectCoordinates(vr.list_objects[vr.current_object[0]+1]['data']['obj'],xx,xy,xz)
        elseif isKeyDown(vkeys.VK_E) then
            vr.list_objects[vr.current_object[0]+1]['data'].pos[2] = vr.list_objects[vr.current_object[0]+1]['data'].pos[2] - 0.1
            local xx,xy,xz = vr.list_objects[vr.current_object[0]+1]['data']['pos'][0],vr.list_objects[vr.current_object[0]+1]['data']['pos'][1],vr.list_objects[vr.current_object[0]+1]['data']['pos'][2]
            setObjectCoordinates(vr.list_objects[vr.current_object[0]+1]['data']['obj'],xx,xy,xz)
        end
    end

end)

--particle
imgui.OnFrame(function() return vr.particles[0] end,
function()
    lockPlayerControl(true)
    local isWindow = false

    imgui.SetNextWindowSize(imgui.ImVec2(270,410),imgui.Cond.Appearing)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x-270,0),imgui.Cond.Appearing)
    imgui.Begin(faicons.ICON_STARS..' '..langt['particles'],nil, imgui.WindowFlags.AlwaysAutoResize)

    --List
    imgui.SetNextItemWidth(255)
    imgui.ListBoxStr_arr('', vr.current_particle,new('const char* const [?]', #vr.temp_var.list_name_particles, vr.temp_var.list_name_particles),#vr.temp_var.list_name_particles,15)

    if imgui.Button(langt['add']) then
        vr.list_particles[#vr.list_particles+1] = {
            ['name'] = new.char[65](langt['particle'].." #"..#vr.list_particles),
        }
        local px,py,pz = getCharCoordinates(PLAYER_PED)
        pz = getGroundZFor3dCoord(px,py,pz)
        local angle = getCharHeading(PLAYER_PED)
        vr.list_particles[#vr.list_particles].data = {
            ['pos'] = new.float[3](px,py,pz),
            ['rotate'] = new.int[3](0,0,0),
            ['modelId'] = new.int(),
            ['tied'] = new.int(0),
            ['tiedId'] = new.int(0),
            ['startC'] = new.int(0),
            ['endC'] = new.int(0),
            ['prtcl'] = {}
        }
        local particles_name = {}
        for i = 1,#vr.list_particles do
            particles_name[#particles_name+1] = vr.list_particles[i].name
        end
        vr.temp_var.list_name_particles = particles_name
        vr.current_particle[0] = #vr.list_particles-1
        upd_particle:run(vr.current_particle[0]+1)
    end
    if #vr.list_particles > 0 then
        imgui.SameLine()
        if imgui.Button(langt['rename']) then
            imgui.OpenPopup("rename")
        end
        imgui.SameLine()
        if imgui.Button(langt['delete']) then
            imgui.OpenPopup(faicons.ICON_TRASH_ALT.." "..langt['delete'])
        end
    end

    isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()


    if imgui.BeginPopupModal(faicons.ICON_TRASH_ALT.." "..langt['delete'], nil,imgui.WindowFlags.AlwaysAutoResize) then
        imgui.Text(langt['deleteQues'])

        local size_b = imgui.ImVec2(100,0)

        if imgui.Button(langt['yes'],size_b) then
            killFxSystem(vr.list_particles[vr.current_particle[0]+1]['data']['prtcl'][1])
            deleteObject(vr.list_particles[vr.current_particle[0]+1]['data']['prtcl'][2])
            table.remove(vr.list_particles,vr.current_particle[0]+1)
            table.remove(vr.temp_var.list_name_particles,vr.current_particle[0]+1)
            if vr.current_particle[0] > 0 then
                vr.current_particle[0] = vr.current_particle[0] - 1
            end
            imgui.CloseCurrentPopup()
        end
        imgui.SameLine()
        if imgui.Button(langt['no'],size_b) then
            imgui.CloseCurrentPopup()
        end
    end

    --Rename popup
    if imgui.BeginPopup("rename") then

        imgui.InputText('',vr.list_particles[vr.current_particle[0]+1].name,ffi.sizeof(vr.list_particles[vr.current_particle[0]+1].name))

        if imgui.Button(langt['close']) then imgui.CloseCurrentPopup() end

        imgui.EndPopup()
    end

    imgui.End()

    -- particle render
    if #vr.list_particles > 0 then
        imgui.SetNextWindowSize(imgui.ImVec2(400,360),imgui.Cond.Always)
        imgui.SetNextWindowPos(imgui.ImVec2(res.x-670,0),imgui.Cond.Appearing)
        imgui.Begin(faicons.ICON_ARCHIVE..' '..langt['particle'],nil, imgui.WindowFlags.AlwaysAutoResize)
    
        imgui.PushIDStr("position")
        if imgui.Button(faicons.ICON_STREET_VIEW) then
            local px,py,pz = getCharCoordinates(PLAYER_PED)
            pz = getGroundZFor3dCoord(px,py,pz)
            vr.list_particles[vr.current_particle[0]+1]['data'].pos[0] = px
            vr.list_particles[vr.current_particle[0]+1]['data'].pos[1] = py
            vr.list_particles[vr.current_particle[0]+1]['data'].pos[2] = pz
            setObjectCoordinates(vr.list_particles[vr.current_particle[0]+1]['data']['prtcl'][2],px,py,pz)
        end
        imgui.PopID()
        if imgui.IsItemHovered() then
            imgui.SetTooltip(langt['playerCoordinates'])
        end

        imgui.SameLine()
        imgui.PushItemWidth(270)
        if imgui.InputFloat3(langt['position'],vr.list_particles[vr.current_particle[0]+1]['data']['pos'],"%.6f") then
            local xx,xy,xz = vr.list_particles[vr.current_particle[0]+1]['data']['pos'][0],vr.list_particles[vr.current_particle[0]+1]['data']['pos'][1],vr.list_particles[vr.current_particle[0]+1]['data']['pos'][2]
            setObjectCoordinates(vr.list_particles[vr.current_particle[0]+1]['data']['prtcl'][2],xx,xy,xz)
        end
        if imgui.DragInt3(langt['rotate'],vr.list_particles[vr.current_particle[0]+1]['data']['rotate'],0.3,-360,360,"%d°") then
            upd_particle:run(vr.current_particle[0]+1)
        end
        if imgui.Combo(langt['particle'],vr.list_particles[vr.current_particle[0]+1]['data'].modelId,new('const char* const [?]',#Particle_name,Particle_name),#Particle_name) then
            upd_particle:run(vr.current_particle[0]+1)
        end
        if imgui.SliderInt(langt['tied'], vr.list_particles[vr.current_particle[0]+1]['data'].tied, 0, 2, vr.temp_var.CutscenePos[vr.list_particles[vr.current_particle[0]+1]['data'].tied[0]+1]) then
            vr.list_particles[vr.current_particle[0]+1]['data'].tiedId[0] = 0
        end
        if vr.list_particles[vr.current_particle[0]+1]['data'].tied[0] == 1 then
            imgui.Combo(langt['actor'],vr.list_particles[vr.current_particle[0]+1]['data']['tiedId'],new('const char* const [?]', #vr.temp_var.list_name_actors, vr.temp_var.list_name_actors),#vr.temp_var.list_name_actors)
        elseif vr.list_particles[vr.current_particle[0]+1]['data'].tied[0] == 2 then
            imgui.Combo(langt['car'],vr.list_particles[vr.current_particle[0]+1]['data']['tiedId'],new('const char* const [?]', #vr.temp_var.list_name_cars, vr.temp_var.list_name_cars),#vr.temp_var.list_name_cars)
        elseif vr.list_particles[vr.current_particle[0]+1]['data'].tied[0] == 3 then
            imgui.Combo(langt['object'],vr.list_particles[vr.current_particle[0]+1]['data']['tiedId'],new('const char* const [?]', #vr.temp_var.list_name_objects, vr.temp_var.list_name_objects),#vr.temp_var.list_name_objects)
        end
        local list_tg_m = {langt['toend']}
        for ltg = 1,#vr.temp_var.list_name_targets do
            list_tg_m[#list_tg_m+1] = vr.temp_var.list_name_targets[ltg]
        end
        imgui.Combo(langt['app_on'],vr.list_particles[vr.current_particle[0]+1]['data']['startC'],new('const char* const [?]',#vr.temp_var.list_name_targets,vr.temp_var.list_name_targets),#vr.temp_var.list_name_targets)
        imgui.Combo(langt['dis_after'],vr.list_particles[vr.current_particle[0]+1]['data']['endC'],new('const char* const [?]',#list_tg_m,list_tg_m),#list_tg_m)
        

        --edit
        local cx,cy,cz = vr.list_particles[vr.current_particle[0]+1]['data'].pos[0],vr.list_particles[vr.current_particle[0]+1]['data'].pos[1],vr.list_particles[vr.current_particle[0]+1]['data'].pos[2]
        cx = cx + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
        cy = cy + (vr.camera_zoom*math.cos(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
        cz = cz + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[2])))
        setFixedCameraPosition(cx,cy,cz)
        pointCameraAtPoint(vr.list_particles[vr.current_particle[0]+1]['data'].pos[0],vr.list_particles[vr.current_particle[0]+1]['data'].pos[1],vr.list_particles[vr.current_particle[0]+1]['data'].pos[2],2)
        if not isWindow then
            if imgui.IsMouseDragging(2) then
                local dt = imgui.GetIO().MouseDelta
                vr.camera_angle[1],vr.camera_angle[2] = vr.camera_angle[1] + dt.x, vr.camera_angle[2] + dt.y
            end
            if imgui.GetIO().MouseWheel ~= 0 then
                vr.camera_zoom = vr.camera_zoom + (vr.camera_zoom * imgui.GetIO().MouseWheel)/4
                if vr.camera_zoom < 1 then vr.camera_zoom = 1 end
            end
        end
        if isKeyDown(vkeys.VK_UP) then
            vr.list_particles[vr.current_particle[0]+1]['data'].pos[0] = vr.list_particles[vr.current_particle[0]+1]['data'].pos[0] + 0.01*vr.camera_zoom
            local xx,xy,xz = vr.list_particles[vr.current_particle[0]+1]['data']['pos'][0],vr.list_particles[vr.current_particle[0]+1]['data']['pos'][1],vr.list_particles[vr.current_particle[0]+1]['data']['pos'][2]
            setObjectCoordinates(vr.list_particles[vr.current_particle[0]+1]['data']['prtcl'][2],xx,xy,xz)
        elseif isKeyDown(vkeys.VK_DOWN) then
            vr.list_particles[vr.current_particle[0]+1]['data'].pos[0] = vr.list_particles[vr.current_particle[0]+1]['data'].pos[0] - 0.01*vr.camera_zoom
            local xx,xy,xz = vr.list_particles[vr.current_particle[0]+1]['data']['pos'][0],vr.list_particles[vr.current_particle[0]+1]['data']['pos'][1],vr.list_particles[vr.current_particle[0]+1]['data']['pos'][2]
            setObjectCoordinates(vr.list_particles[vr.current_particle[0]+1]['data']['prtcl'][2],xx,xy,xz)
        end
        if isKeyDown(vkeys.VK_LEFT) then
            vr.list_particles[vr.current_particle[0]+1]['data'].pos[1] = vr.list_particles[vr.current_particle[0]+1]['data'].pos[1] + 0.01*vr.camera_zoom
            local xx,xy,xz = vr.list_particles[vr.current_particle[0]+1]['data']['pos'][0],vr.list_particles[vr.current_particle[0]+1]['data']['pos'][1],vr.list_particles[vr.current_particle[0]+1]['data']['pos'][2]
            setObjectCoordinates(vr.list_particles[vr.current_particle[0]+1]['data']['prtcl'][2],xx,xy,xz)
        elseif isKeyDown(vkeys.VK_RIGHT) then
            vr.list_particles[vr.current_particle[0]+1]['data'].pos[1] = vr.list_particles[vr.current_particle[0]+1]['data'].pos[1] - 0.01*vr.camera_zoom
            local xx,xy,xz = vr.list_particles[vr.current_particle[0]+1]['data']['pos'][0],vr.list_particles[vr.current_particle[0]+1]['data']['pos'][1],vr.list_particles[vr.current_particle[0]+1]['data']['pos'][2]
            setObjectCoordinates(vr.list_particles[vr.current_particle[0]+1]['data']['prtcl'][2],xx,xy,xz)
        end
        if isKeyDown(vkeys.VK_Q) then
            vr.list_particles[vr.current_particle[0]+1]['data'].pos[2] = vr.list_particles[vr.current_particle[0]+1]['data'].pos[2] + 0.01*vr.camera_zoom
            local xx,xy,xz = vr.list_particles[vr.current_particle[0]+1]['data']['pos'][0],vr.list_particles[vr.current_particle[0]+1]['data']['pos'][1],vr.list_particles[vr.current_particle[0]+1]['data']['pos'][2]
            setObjectCoordinates(vr.list_particles[vr.current_particle[0]+1]['data']['prtcl'][2],xx,xy,xz)
        elseif isKeyDown(vkeys.VK_E) then
            vr.list_particles[vr.current_particle[0]+1]['data'].pos[2] = vr.list_particles[vr.current_particle[0]+1]['data'].pos[2] - 0.01*vr.camera_zoom
            local xx,xy,xz = vr.list_particles[vr.current_particle[0]+1]['data']['pos'][0],vr.list_particles[vr.current_particle[0]+1]['data']['pos'][1],vr.list_particles[vr.current_particle[0]+1]['data']['pos'][2]
            setObjectCoordinates(vr.list_particles[vr.current_particle[0]+1]['data']['prtcl'][2],xx,xy,xz)
        end
    end

end)

--pickup
imgui.OnFrame(function() return vr.pickups[0] end,
function()
    lockPlayerControl(true)
    local isWindow = false

    imgui.SetNextWindowSize(imgui.ImVec2(270,410),imgui.Cond.Appearing)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x-270,0),imgui.Cond.Appearing)
    imgui.Begin(faicons.ICON_STARS..' '..langt['pickups'],nil, imgui.WindowFlags.AlwaysAutoResize)

    --List
    imgui.SetNextItemWidth(255)
    imgui.ListBoxStr_arr('', vr.current_pickup,new('const char* const [?]', #vr.temp_var.list_name_pickups, vr.temp_var.list_name_pickups),#vr.temp_var.list_name_pickups,15)

    if imgui.Button(langt['add']) then
        vr.list_pickups[#vr.list_pickups+1] = {
            ['name'] = new.char[65](langt['pickup'].." #"..#vr.list_pickups),
        }
        local px,py,pz = getCharCoordinates(PLAYER_PED)
        pz = getGroundZFor3dCoord(px,py,pz)
        local angle = getCharHeading(PLAYER_PED)
        vr.list_pickups[#vr.list_pickups].data = {
            ['pos'] = new.float[3](px,py,pz),
            ['type'] = new.int(0),
            ['spawnType'] = new.int(0),
            ['weapon'] = new.int(2),
            ['ammo'] = new.int(0),
            ['modelId'] = new.int(0),
            ['startC'] = new.int(0),
            ['endC'] = new.int(0),
        }
        local pickups_name = {}
        for i = 1,#vr.list_pickups do
            pickups_name[#pickups_name+1] = vr.list_pickups[i].name
        end
        vr.temp_var.list_name_pickups = pickups_name
        vr.current_pickup[0] = #vr.list_pickups-1
        upd_pickup:run(vr.current_pickup[0]+1)
    end
    if #vr.list_pickups > 0 then
        imgui.SameLine()
        if imgui.Button(langt['rename']) then
            imgui.OpenPopup("rename")
        end
        imgui.SameLine()
        if imgui.Button(langt['delete']) then
            imgui.OpenPopup(faicons.ICON_TRASH_ALT.." "..langt['delete'])
        end
    end

    isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()


    if imgui.BeginPopupModal(faicons.ICON_TRASH_ALT.." "..langt['delete'], nil,imgui.WindowFlags.AlwaysAutoResize) then
        imgui.Text(langt['deleteQues'])

        local size_b = imgui.ImVec2(100,0)

        if imgui.Button(langt['yes'],size_b) then
            removePickup(vr.list_pickups[vr.current_pickup[0]+1]['data']['pick'])
            table.remove(vr.list_pickups,vr.current_pickup[0]+1)
            table.remove(vr.temp_var.list_name_pickups,vr.current_pickup[0]+1)
            if vr.current_pickup[0] > 0 then
                vr.current_pickup[0] = vr.current_pickup[0] - 1
            end
            imgui.CloseCurrentPopup()
        end
        imgui.SameLine()
        if imgui.Button(langt['no'],size_b) then
            imgui.CloseCurrentPopup()
        end
    end

    --Rename popup
    if imgui.BeginPopup("rename") then

        imgui.InputText('',vr.list_pickups[vr.current_pickup[0]+1].name,ffi.sizeof(vr.list_pickups[vr.current_pickup[0]+1].name))

        if imgui.Button(langt['close']) then imgui.CloseCurrentPopup() end

        imgui.EndPopup()
    end

    imgui.End()

    -- pickup render
    if #vr.list_pickups > 0 then
        imgui.SetNextWindowSize(imgui.ImVec2(400,360),imgui.Cond.Always)
        imgui.SetNextWindowPos(imgui.ImVec2(res.x-670,0),imgui.Cond.Appearing)
        imgui.Begin(faicons.ICON_ARCHIVE..' '..langt['pickup'],nil, imgui.WindowFlags.AlwaysAutoResize)
    
        imgui.PushIDStr("position")
        if imgui.Button(faicons.ICON_STREET_VIEW) then
            local px,py,pz = getCharCoordinates(PLAYER_PED)
            pz = getGroundZFor3dCoord(px,py,pz)
            vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[0] = px
            vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[1] = py
            vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[2] = pz
            upd_pickup:run(vr.current_pickup[0]+1)
        end
        imgui.PopID()
        if imgui.IsItemHovered() then
            imgui.SetTooltip(langt['playerCoordinates'])
        end

        imgui.SameLine()
        imgui.PushItemWidth(270)
        if imgui.InputFloat3(langt['position'],vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'],"%.6f") then
            local xx,xy,xz = vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'][0],vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'][1],vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'][2]
            setObjectCoordinates(vr.list_pickups[vr.current_pickup[0]+1]['data']['pick'][2],xx,xy,xz)
        end
        imgui.SliderInt(langt['spawn'], vr.list_pickups[vr.current_pickup[0]+1]['data'].spawnType, 0, 2, vr.temp_var.spawn_type_pickup[vr.list_pickups[vr.current_pickup[0]+1]['data'].spawnType[0]+1])
        if imgui.Combo(langt['type'],vr.list_pickups[vr.current_pickup[0]+1]['data']['type'],new('const char* const [?]', #vr.temp_var.type_pickup, vr.temp_var.type_pickup),#vr.temp_var.type_pickup) then
            upd_pickup:run(vr.current_pickup[0]+1)
        end
        if vr.list_pickups[vr.current_pickup[0]+1]['data']['type'][0] == 0 then
            imgui.Separator()
            if imgui.ImageButton(weaponsAtlas, imgui.ImVec2(52,52),imgui.ImVec2((vr.list_pickups[vr.current_pickup[0]+1]['data'].weapon[0]-1)*0.02272727272,0), imgui.ImVec2(vr.list_pickups[vr.current_pickup[0]+1]['data'].weapon[0]*0.02272727272,1)) then
                imgui.OpenPopup('weapon')
            end

            imgui.SameLine()
            imgui.Text(langt["weapon"])

            if imgui.InputInt(langt['ammo'], vr.list_pickups[vr.current_pickup[0]+1]['data'].ammo) then
                if vr.list_pickups[vr.current_pickup[0]+1]['data'].ammo[0] < 0 then vr.list_pickups[vr.current_pickup[0]+1]['data'].ammo[0] = 0 end 
            end

             --weapon popup
             if imgui.BeginPopup('weapon') then
                imgui.BeginChild('weapon',imgui.ImVec2(200,450))
        
                for i = 2,#ID_Weapons do
                    imgui.PushIDStr(tostring(i))
                    if imgui.ImageButton(weaponsAtlas,imgui.ImVec2(52,52),imgui.ImVec2((i-1)*0.02272727272,0),imgui.ImVec2(i*0.02272727272,1)) then
                        vr.list_pickups[vr.current_pickup[0]+1]['data'].weapon[0] = i
                        upd_pickup:run(vr.current_pickup[0]+1)
                    end
                    if imgui.IsItemHovered() then
                        imgui.SetTooltip(vr.temp_var.weap_names[i])
                    end
                    imgui.PopID()
                    if (i-1) % 3 ~= 0 then
                        imgui.SameLine()
                    end
                end
        
                imgui.EndChild()
                imgui.EndPopup()
            end

        elseif vr.list_pickups[vr.current_pickup[0]+1]['data']['type'][0] == 5 then
            imgui.InputInt(langt['model'],vr.list_pickups[vr.current_pickup[0]+1]['data'].modelId)
            if imgui.Button(langt['apply']) then
                upd_pickup:run(vr.current_pickup[0]+1)
            end
        end
        imgui.Separator()
        local list_tg_m = {langt['toend']}
        for ltg = 1,#vr.temp_var.list_name_targets do
            list_tg_m[#list_tg_m+1] = vr.temp_var.list_name_targets[ltg]
        end
        imgui.Combo(langt['app_on'],vr.list_pickups[vr.current_pickup[0]+1]['data']['startC'],new('const char* const [?]',#vr.temp_var.list_name_targets,vr.temp_var.list_name_targets),#vr.temp_var.list_name_targets)
        imgui.Combo(langt['dis_after'],vr.list_pickups[vr.current_pickup[0]+1]['data']['endC'],new('const char* const [?]',#list_tg_m,list_tg_m),#list_tg_m)
        

        --edit
        local cx,cy,cz = vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[0],vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[1],vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[2]
        cx = cx + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
        cy = cy + (vr.camera_zoom*math.cos(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
        cz = cz + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[2])))
        setFixedCameraPosition(cx,cy,cz)
        pointCameraAtPoint(vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[0],vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[1],vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[2],2)
        if not isWindow then
            if imgui.IsMouseDragging(2) then
                local dt = imgui.GetIO().MouseDelta
                vr.camera_angle[1],vr.camera_angle[2] = vr.camera_angle[1] + dt.x, vr.camera_angle[2] + dt.y
            end
            if imgui.GetIO().MouseWheel ~= 0 then
                vr.camera_zoom = vr.camera_zoom + (vr.camera_zoom * imgui.GetIO().MouseWheel)/4
                if vr.camera_zoom < 1 then vr.camera_zoom = 1 end
            end
        end
        if isKeyDown(vkeys.VK_UP) then
            vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[0] = vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[0] + 0.01*vr.camera_zoom
            local xx,xy,xz = vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'][0],vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'][1],vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'][2]
            upd_pickup:run(vr.current_pickup[0]+1)
        elseif isKeyDown(vkeys.VK_DOWN) then
            vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[0] = vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[0] - 0.01*vr.camera_zoom
            local xx,xy,xz = vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'][0],vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'][1],vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'][2]
            upd_pickup:run(vr.current_pickup[0]+1)
        end
        if isKeyDown(vkeys.VK_LEFT) then
            vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[1] = vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[1] + 0.01*vr.camera_zoom
            local xx,xy,xz = vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'][0],vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'][1],vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'][2]
            upd_pickup:run(vr.current_pickup[0]+1)
        elseif isKeyDown(vkeys.VK_RIGHT) then
            vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[1] = vr.list_pickups[vr.current_pickup[0]+1]['data'].pos[1] - 0.01*vr.camera_zoom
            local xx,xy,xz = vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'][0],vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'][1],vr.list_pickups[vr.current_pickup[0]+1]['data']['pos'][2]
            upd_pickup:run(vr.current_pickup[0]+1)
        end
    end

end)

--explosion
imgui.OnFrame(function() return vr.explosions[0] end,
function()
    lockPlayerControl(true)
    local isWindow = false

    imgui.SetNextWindowSize(imgui.ImVec2(270,410),imgui.Cond.Appearing)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x-270,0),imgui.Cond.Appearing)
    imgui.Begin(faicons.ICON_MALE..' '..langt['explosions'],nil, imgui.WindowFlags.AlwaysAutoResize)

    --List
    imgui.SetNextItemWidth(255)
    imgui.ListBoxStr_arr('', vr.current_explosion,new('const char* const [?]', #vr.temp_var.list_name_explosions, vr.temp_var.list_name_explosions),#vr.temp_var.list_name_explosions,15)

    if imgui.Button(langt['add']) then
        vr.list_explosions[#vr.list_explosions+1] = {
            ['name'] = new.char[65](langt['explosion'].." #"..#vr.list_explosions),
        }
        local px,py,pz = getCharCoordinates(PLAYER_PED)
        pz = getGroundZFor3dCoord(px,py,pz)
        local angle = getCharHeading(PLAYER_PED)
        vr.list_explosions[#vr.list_explosions].data = {
            ['pos'] = new.float[3](px,py,pz),
            ['type'] = new.int(0),
            ['typeExplosion'] = new.ImU8(0),
            ['sizeFire'] = new.ImU8(0),
            ['propagationFire'] = new.ImU8(0),
            ['startC'] = new.int(0),
            ['endC'] = new.int(0),
        }
        local explosions_name = {}
        for i = 1,#vr.list_explosions do
            explosions_name[#explosions_name+1] = vr.list_explosions[i].name
        end
        vr.temp_var.list_name_explosions = explosions_name
        vr.current_explosion[0] = #vr.list_explosions-1
        upd_explosion:run(vr.current_explosion[0]+1)
    end
    if #vr.list_explosions > 0 then
        imgui.SameLine()
        if imgui.Button(langt['rename']) then
            imgui.OpenPopup("rename")
        end
        imgui.SameLine()
        if imgui.Button(langt['delete']) then
            imgui.OpenPopup(faicons.ICON_TRASH_ALT.." "..langt['delete'])
        end
    end

    isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()


    if imgui.BeginPopupModal(faicons.ICON_TRASH_ALT.." "..langt['delete'], nil,imgui.WindowFlags.AlwaysAutoResize) then
        imgui.Text(langt['deleteQues'])

        local size_b = imgui.ImVec2(100,0)

        if imgui.Button(langt['yes'],size_b) then
            if vr.list_explosions[vr.current_explosion[0]+1]['data']['fire'] then
                removeScriptFire(vr.list_explosions[vr.current_explosion[0]+1]['data']['fire'])
            end
            if vr.list_explosions[vr.current_explosion[0]+1]['data']['explosion'] then
                deleteObject(vr.list_explosions[vr.current_explosion[0]+1]['data']['explosion'])
            end
            table.remove(vr.list_explosions,vr.current_explosion[0]+1)
            table.remove(vr.temp_var.list_name_explosions,vr.current_explosion[0]+1)
            if vr.current_explosion[0] > 0 then
                vr.current_explosion[0] = vr.current_explosion[0] - 1
            end
            imgui.CloseCurrentPopup()
        end
        imgui.SameLine()
        if imgui.Button(langt['no'],size_b) then
            imgui.CloseCurrentPopup()
        end
    end

    --Rename popup
    if imgui.BeginPopup("rename") then

        imgui.InputText('',vr.list_explosions[vr.current_explosion[0]+1].name,ffi.sizeof(vr.list_explosions[vr.current_explosion[0]+1].name))

        if imgui.Button(langt['close']) then imgui.CloseCurrentPopup() end

        imgui.EndPopup()
    end

    imgui.End()

    -- explosion render
    if #vr.list_explosions > 0 then
        imgui.SetNextWindowSize(imgui.ImVec2(400,360),imgui.Cond.Always)
        imgui.SetNextWindowPos(imgui.ImVec2(res.x-670,0),imgui.Cond.Appearing)
        imgui.Begin(faicons.ICON_ARCHIVE..' '..langt['explosion'],nil, imgui.WindowFlags.AlwaysAutoResize)
    
        imgui.PushIDStr("position")
        if imgui.Button(faicons.ICON_STREET_VIEW) then
            local px,py,pz = getCharCoordinates(PLAYER_PED)
            pz = getGroundZFor3dCoord(px,py,pz)
            vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[0] = px
            vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[1] = py
            vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[2] = pz
            upd_explosion:run(vr.current_explosion[0]+1)
        end
        imgui.PopID()
        if imgui.IsItemHovered() then
            imgui.SetTooltip(langt['playerCoordinates'])
        end

        imgui.SameLine()
        imgui.PushItemWidth(270)
        if imgui.InputFloat3(langt['position'],vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'],"%.6f") then
            local xx,xy,xz = vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][0],vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][1],vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][2]
            upd_explosion:run(vr.current_explosion[0]+1)
        end
        if imgui.SliderInt(langt['type'], vr.list_explosions[vr.current_explosion[0]+1]['data'].type, 0, 1, vr.temp_var.type_expl[vr.list_explosions[vr.current_explosion[0]+1]['data'].type[0]+1]) then
            upd_explosion:run(vr.current_explosion[0]+1)
        end
        if vr.list_explosions[vr.current_explosion[0]+1]['data'].type[0] == 0 then
            if imgui.DragScalar(langt['size_fire'], imgui.DataType.U8, vr.list_explosions[vr.current_explosion[0]+1]['data'].sizeFire, 0.1) then
                upd_explosion:run(vr.current_explosion[0]+1)
            end
            imgui.DragScalar(langt['spread_fire'], imgui.DataType.U8, vr.list_explosions[vr.current_explosion[0]+1]['data'].propagationFire, 0.1)
        elseif vr.list_explosions[vr.current_explosion[0]+1]['data'].type[0] == 1 then
            imgui.DragScalar(langt['type_explosion'], imgui.DataType.U8, vr.list_explosions[vr.current_explosion[0]+1]['data'].typeExplosion, 0.1)
        end
        local list_tg_m = {langt['toend']}
        for ltg = 1,#vr.temp_var.list_name_targets do
            list_tg_m[#list_tg_m+1] = vr.temp_var.list_name_targets[ltg]
        end
        imgui.Combo(langt['app_on'],vr.list_explosions[vr.current_explosion[0]+1]['data']['startC'],new('const char* const [?]',#vr.temp_var.list_name_targets,vr.temp_var.list_name_targets),#vr.temp_var.list_name_targets)
        imgui.Combo(langt['dis_after'],vr.list_explosions[vr.current_explosion[0]+1]['data']['endC'],new('const char* const [?]',#list_tg_m,list_tg_m),#list_tg_m)
        

        --edit
        local cx,cy,cz = vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[0],vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[1],vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[2]
        cx = cx + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
        cy = cy + (vr.camera_zoom*math.cos(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
        cz = cz + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[2])))
        setFixedCameraPosition(cx,cy,cz)
        pointCameraAtPoint(vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[0],vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[1],vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[2],2)
        if not isWindow then
            if imgui.IsMouseDragging(2) then
                local dt = imgui.GetIO().MouseDelta
                vr.camera_angle[1],vr.camera_angle[2] = vr.camera_angle[1] + dt.x, vr.camera_angle[2] + dt.y
            end
            if imgui.GetIO().MouseWheel ~= 0 then
                vr.camera_zoom = vr.camera_zoom + (vr.camera_zoom * imgui.GetIO().MouseWheel)/4
                if vr.camera_zoom < 1 then vr.camera_zoom = 1 end
            end
        end
        if isKeyDown(vkeys.VK_UP) then
            vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[0] = vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[0] + 0.1
            local xx,xy,xz = vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][0],vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][1],vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][2]
            upd_explosion:run(vr.current_explosion[0]+1)
        elseif isKeyDown(vkeys.VK_DOWN) then
            vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[0] = vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[0] - 0.1
            local xx,xy,xz = vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][0],vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][1],vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][2]
            upd_explosion:run(vr.current_explosion[0]+1)
        end
        if isKeyDown(vkeys.VK_LEFT) then
            vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[1] = vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[1] + 0.1
            local xx,xy,xz = vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][0],vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][1],vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][2]
            upd_explosion:run(vr.current_explosion[0]+1)
        elseif isKeyDown(vkeys.VK_RIGHT) then
            vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[1] = vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[1] - 0.1
            local xx,xy,xz = vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][0],vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][1],vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][2]
            upd_explosion:run(vr.current_explosion[0]+1)
        end
        if isKeyDown(vkeys.VK_Q) then
            vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[2] = vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[2] + 0.1
            local xx,xy,xz = vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][0],vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][1],vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][2]
            upd_explosion:run(vr.current_explosion[0]+1)
        elseif isKeyDown(vkeys.VK_E) then
            vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[2] = vr.list_explosions[vr.current_explosion[0]+1]['data'].pos[2] - 0.1
            local xx,xy,xz = vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][0],vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][1],vr.list_explosions[vr.current_explosion[0]+1]['data']['pos'][2]
            upd_explosion:run(vr.current_explosion[0]+1)
        end
    end

end)

--audio
imgui.OnFrame(function() return vr.audios[0] end,
function()
    lockPlayerControl(true)
    local isWindow = false

    imgui.SetNextWindowSize(imgui.ImVec2(270,410),imgui.Cond.Appearing)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x-270,0),imgui.Cond.Appearing)
    imgui.Begin(faicons.ICON_MALE..' '..langt['audio'],nil, imgui.WindowFlags.AlwaysAutoResize)

    --List
    imgui.SetNextItemWidth(255)
    imgui.ListBoxStr_arr('', vr.current_audio,new('const char* const [?]', #vr.temp_var.list_name_audios, vr.temp_var.list_name_audios),#vr.temp_var.list_name_audios,15)

    if imgui.Button(langt['add']) then
        vr.list_audios[#vr.list_audios+1] = {
            ['name'] = new.char[65](langt['audio'].." #"..#vr.list_audios),
        }
        local px,py,pz = getCharCoordinates(PLAYER_PED)
        pz = getGroundZFor3dCoord(px,py,pz)
        local angle = getCharHeading(PLAYER_PED)
        vr.list_audios[#vr.list_audios].data = {
            ['pos'] = new.float[3](px,py,pz),
            ['sound'] = new.int(0),
            ['audio3d'] = new.bool(false),
            ['audio3dType'] = new.ImU8(0),
            ['audio3dAttach'] = new.int(0),
            ['repeat'] = new.bool(false),
            ['useTarget'] = new.bool(false),
            ['startC'] = new.int(0),
            ['endC'] = new.int(0),
        }
        local audios_name = {}
        for i = 1,#vr.list_audios do
            audios_name[#audios_name+1] = vr.list_audios[i].name
        end
        vr.temp_var.list_name_audios = audios_name
        vr.current_audio[0] = #vr.list_audios-1
        upd_audio:run(vr.current_audio[0]+1)
    end
    if #vr.list_audios > 0 then
        imgui.SameLine()
        if imgui.Button(langt['rename']) then
            imgui.OpenPopup("rename")
        end
        imgui.SameLine()
        if imgui.Button(langt['delete']) then
            imgui.OpenPopup(faicons.ICON_TRASH_ALT.." "..langt['delete'])
        end
    end

    isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()


    if imgui.BeginPopupModal(faicons.ICON_TRASH_ALT.." "..langt['delete'], nil,imgui.WindowFlags.AlwaysAutoResize) then
        imgui.Text(langt['deleteQues'])

        local size_b = imgui.ImVec2(100,0)

        if imgui.Button(langt['yes'],size_b) then
            if vr.list_audios[vr.current_audio[0]+1]['data'].audio3d[0] and vr.list_audios[vr.current_audio[0]+1]['data'].audio3dAttach[0] == 0 then 
                deleteObject(vr.list_audios[vr.current_audio[0]+1]['data']['obj'])
            end
            table.remove(vr.list_audios,vr.current_audio[0]+1)
            table.remove(vr.temp_var.list_name_audios,vr.current_audio[0]+1)
            if vr.current_audio[0] > 0 then
                vr.current_audio[0] = vr.current_audio[0] - 1
            end
            imgui.CloseCurrentPopup()
        end
        imgui.SameLine()
        if imgui.Button(langt['no'],size_b) then
            imgui.CloseCurrentPopup()
        end
    end

    --Rename popup
    if imgui.BeginPopup("rename") then

        imgui.InputText('',vr.list_audios[vr.current_audio[0]+1].name,ffi.sizeof(vr.list_audios[vr.current_audio[0]+1].name))

        if imgui.Button(langt['close']) then imgui.CloseCurrentPopup() end

        imgui.EndPopup()
    end

    imgui.End()

    -- audio render
    if #vr.list_audios > 0 then
        imgui.SetNextWindowSize(imgui.ImVec2(400,360),imgui.Cond.Always)
        imgui.SetNextWindowPos(imgui.ImVec2(res.x-670,0),imgui.Cond.Appearing)
        imgui.Begin(faicons.ICON_ARCHIVE..' '..langt['audio'],nil, imgui.WindowFlags.AlwaysAutoResize)
    
        imgui.Combo(langt['sound'],vr.list_audios[vr.current_audio[0]+1]['data'].sound,new('const char* const [?]',#vr.temp_var.list_audios_name,vr.temp_var.list_audios_name),#vr.temp_var.list_audios_name)
        
        if imgui.Button(langt['update']) then
            vr.temp_var.list_audios_name = {}
            for file in lfs.dir(getWorkingDirectory() .. '\\Missions_pack\\Pack missions ya\\audio') do
                if lfs.attributes(getWorkingDirectory() .. '\\Missions_pack\\Pack missions ya\\audio\\'..file,"mode") == "file" then
                    if string.sub(file, -3) == 'mp3' then
                        vr.temp_var.list_audios_name[#vr.temp_var.list_audios_name+1] = string.sub(file, 0, -5)
                    end
                end
            end
        end

        if mimgui_addons.ToggleButton(langt['audio3d'],vr.list_audios[vr.current_audio[0]+1]['data'].audio3d) then
            upd_audio:run(vr.current_audio[0]+1)
            vr.list_audios[vr.current_audio[0]+1]['data'].audio3dAttach[0] = 0
        end
        if vr.list_audios[vr.current_audio[0]+1]['data'].audio3d[0] then
            if imgui.SliderScalar(langt['place'],imgui.DataType.U8, vr.list_audios[vr.current_audio[0]+1]['data'].audio3dType, new.ImU8(0), new.ImU8(3), vr.temp_var.place_type[vr.list_audios[vr.current_audio[0]+1]['data'].audio3dType[0]+1]) then
                upd_audio:run(vr.current_audio[0]+1)
            end
            
            if vr.list_audios[vr.current_audio[0]+1]['data'].audio3dType[0] == 0 then 
                imgui.PushIDStr("position")
                if imgui.Button(faicons.ICON_STREET_VIEW) then
                    local px,py,pz = getCharCoordinates(PLAYER_PED)
                    pz = getGroundZFor3dCoord(px,py,pz)
                    vr.list_audios[vr.current_audio[0]+1]['data'].pos[0] = px
                    vr.list_audios[vr.current_audio[0]+1]['data'].pos[1] = py
                    vr.list_audios[vr.current_audio[0]+1]['data'].pos[2] = pz
                    upd_audio:run(vr.current_audio[0]+1)
                    setObjectCoordinates(vr.list_audios[vr.current_audio[0]+1]['data'].obj,px,py,pz)
                end
                imgui.PopID()
                if imgui.IsItemHovered() then
                    imgui.SetTooltip(langt['playerCoordinates'])
                end
                imgui.SameLine()
                imgui.PushItemWidth(270)
                if imgui.InputFloat3(langt['position'],vr.list_audios[vr.current_audio[0]+1]['data']['pos'],"%.6f") then
                    local xx,xy,xz = vr.list_audios[vr.current_audio[0]+1]['data']['pos'][0],vr.list_audios[vr.current_audio[0]+1]['data']['pos'][1],vr.list_audios[vr.current_audio[0]+1]['data']['pos'][2]
                    setObjectCoordinates(vr.list_audios[vr.current_audio[0]+1]['data'].obj,xx,xy,xz)
                end

                --edit
                local cx,cy,cz = vr.list_audios[vr.current_audio[0]+1]['data'].pos[0],vr.list_audios[vr.current_audio[0]+1]['data'].pos[1],vr.list_audios[vr.current_audio[0]+1]['data'].pos[2]
                cx = cx + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
                cy = cy + (vr.camera_zoom*math.cos(math.rad(vr.camera_angle[1]))*math.cos(math.rad(vr.camera_angle[2])))
                cz = cz + (vr.camera_zoom*math.sin(math.rad(vr.camera_angle[2])))
                setFixedCameraPosition(cx,cy,cz)
                pointCameraAtPoint(vr.list_audios[vr.current_audio[0]+1]['data'].pos[0],vr.list_audios[vr.current_audio[0]+1]['data'].pos[1],vr.list_audios[vr.current_audio[0]+1]['data'].pos[2],2)
                if not isWindow then
                    if imgui.IsMouseDragging(2) then
                        local dt = imgui.GetIO().MouseDelta
                        vr.camera_angle[1],vr.camera_angle[2] = vr.camera_angle[1] + dt.x, vr.camera_angle[2] + dt.y
                    end
                    if imgui.GetIO().MouseWheel ~= 0 then
                        vr.camera_zoom = vr.camera_zoom + (vr.camera_zoom * imgui.GetIO().MouseWheel)/4
                        if vr.camera_zoom < 1 then vr.camera_zoom = 1 end
                    end
                end
                if isKeyDown(vkeys.VK_UP) then
                    vr.list_audios[vr.current_audio[0]+1]['data'].pos[0] = vr.list_audios[vr.current_audio[0]+1]['data'].pos[0] + 0.1
                    local xx,xy,xz = vr.list_audios[vr.current_audio[0]+1]['data']['pos'][0],vr.list_audios[vr.current_audio[0]+1]['data']['pos'][1],vr.list_audios[vr.current_audio[0]+1]['data']['pos'][2]
                    setObjectCoordinates(vr.list_audios[vr.current_audio[0]+1]['data'].obj,xx,xy,xz)
                elseif isKeyDown(vkeys.VK_DOWN) then
                    vr.list_audios[vr.current_audio[0]+1]['data'].pos[0] = vr.list_audios[vr.current_audio[0]+1]['data'].pos[0] - 0.1
                    local xx,xy,xz = vr.list_audios[vr.current_audio[0]+1]['data']['pos'][0],vr.list_audios[vr.current_audio[0]+1]['data']['pos'][1],vr.list_audios[vr.current_audio[0]+1]['data']['pos'][2]
                    setObjectCoordinates(vr.list_audios[vr.current_audio[0]+1]['data'].obj,xx,xy,xz)
                end
                if isKeyDown(vkeys.VK_LEFT) then
                    vr.list_audios[vr.current_audio[0]+1]['data'].pos[1] = vr.list_audios[vr.current_audio[0]+1]['data'].pos[1] + 0.1
                    local xx,xy,xz = vr.list_audios[vr.current_audio[0]+1]['data']['pos'][0],vr.list_audios[vr.current_audio[0]+1]['data']['pos'][1],vr.list_audios[vr.current_audio[0]+1]['data']['pos'][2]
                    setObjectCoordinates(vr.list_audios[vr.current_audio[0]+1]['data'].obj,xx,xy,xz)
                elseif isKeyDown(vkeys.VK_RIGHT) then
                    vr.list_audios[vr.current_audio[0]+1]['data'].pos[1] = vr.list_audios[vr.current_audio[0]+1]['data'].pos[1] - 0.1
                    local xx,xy,xz = vr.list_audios[vr.current_audio[0]+1]['data']['pos'][0],vr.list_audios[vr.current_audio[0]+1]['data']['pos'][1],vr.list_audios[vr.current_audio[0]+1]['data']['pos'][2]
                    setObjectCoordinates(vr.list_audios[vr.current_audio[0]+1]['data'].obj,xx,xy,xz)
                end
                if isKeyDown(vkeys.VK_Q) then
                    vr.list_audios[vr.current_audio[0]+1]['data'].pos[2] = vr.list_audios[vr.current_audio[0]+1]['data'].pos[2] + 0.1
                    local xx,xy,xz = vr.list_audios[vr.current_audio[0]+1]['data']['pos'][0],vr.list_audios[vr.current_audio[0]+1]['data']['pos'][1],vr.list_audios[vr.current_audio[0]+1]['data']['pos'][2]
                    setObjectCoordinates(vr.list_audios[vr.current_audio[0]+1]['data'].obj,xx,xy,xz)
                elseif isKeyDown(vkeys.VK_E) then
                    vr.list_audios[vr.current_audio[0]+1]['data'].pos[2] = vr.list_audios[vr.current_audio[0]+1]['data'].pos[2] - 0.1
                    local xx,xy,xz = vr.list_audios[vr.current_audio[0]+1]['data']['pos'][0],vr.list_audios[vr.current_audio[0]+1]['data']['pos'][1],vr.list_audios[vr.current_audio[0]+1]['data']['pos'][2]
                    setObjectCoordinates(vr.list_audios[vr.current_audio[0]+1]['data'].obj,xx,xy,xz)
                end
            elseif vr.list_audios[vr.current_audio[0]+1]['data'].audio3dType[0] == 1 then
                imgui.Combo(langt['actor'],vr.list_audios[vr.current_audio[0]+1]['data'].audio3dAttach,new('const char* const [?]', #vr.temp_var.list_name_actors, vr.temp_var.list_name_actors),#vr.temp_var.list_name_actors)
            elseif vr.list_audios[vr.current_audio[0]+1]['data'].audio3dType[0] == 2 then
                imgui.Combo(langt['car'],vr.list_audios[vr.current_audio[0]+1]['data'].audio3dAttach,new('const char* const [?]', #vr.temp_var.list_name_cars, vr.temp_var.list_name_cars),#vr.temp_var.list_name_cars)
            elseif vr.list_audios[vr.current_audio[0]+1]['data'].audio3dType[0] == 3 then
                imgui.Combo(langt['object'],vr.list_audios[vr.current_audio[0]+1]['data'].audio3dAttach,new('const char* const [?]', #vr.temp_var.list_name_objects, vr.temp_var.list_name_objects),#vr.temp_var.list_name_objects)
            end
        end

        mimgui_addons.ToggleButton(langt['repeat'],vr.list_audios[vr.current_audio[0]+1]['data']['repeat'])
            
            
        mimgui_addons.ToggleButton(langt['useTarget'],vr.list_audios[vr.current_audio[0]+1]['data']['useTarget'])
        if vr.list_audios[vr.current_audio[0]+1]['data'].useTarget[0] then
            local list_tg_m = {langt['toend']}
            for ltg = 1,#vr.temp_var.list_name_targets do
                list_tg_m[#list_tg_m+1] = vr.temp_var.list_name_targets[ltg]
            end
            imgui.Combo(langt['app_on'],vr.list_audios[vr.current_audio[0]+1]['data']['startC'],new('const char* const [?]',#vr.temp_var.list_name_targets,vr.temp_var.list_name_targets),#vr.temp_var.list_name_targets)
            imgui.Combo(langt['dis_after'],vr.list_audios[vr.current_audio[0]+1]['data']['endC'],new('const char* const [?]',#list_tg_m,list_tg_m),#list_tg_m)
        end
    end

end)

--player
imgui.OnFrame(function() return vr.player[0] end,
function()
    -- player render
    imgui.SetNextWindowSize(imgui.ImVec2(400,360),imgui.Cond.Always)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x-400,0),imgui.Cond.Appearing)
    imgui.Begin(faicons.ICON_MALE..' '..langt['player'],nil, imgui.WindowFlags.AlwaysAutoResize)

    imgui.PushIDStr("position")
    if imgui.Button(faicons.ICON_STREET_VIEW) then
        local px,py,pz = getCharCoordinates(PLAYER_PED)
        pz = getGroundZFor3dCoord(px,py,pz)
        vr.missData['player'].pos[0] = px
        vr.missData['player'].pos[1] = py
        vr.missData['player'].pos[2] = pz
    end
    imgui.PopID()
    if imgui.IsItemHovered() then
        imgui.SetTooltip(langt['playerCoordinates'])
    end

    imgui.SameLine()
    imgui.PushItemWidth(270)
    if imgui.InputFloat3(langt['position'],vr.missData['player']['pos'],"%.6f") then
        local xx,xy,xz = vr.missData['player']['pos'][0],vr.missData['player']['pos'][1],vr.missData['player']['pos'][2]
    end

    imgui.PushIDStr("angle")
    if imgui.Button(faicons.ICON_STREET_VIEW) then
        local angle = getCharHeading(PLAYER_PED)
        vr.missData['player'].angle[0] = angle
    end
    imgui.PopID()
    if imgui.IsItemHovered() then
        imgui.SetTooltip(langt['playerAngle'])
    end

    imgui.SameLine()
    imgui.DragInt(langt['angle'], vr.missData['player'].angle,1,0,360)

    imgui.Separator()

    if imgui.SliderScalar('',imgui.DataType.U8,vr.missData['player'].modelType,new.ImU8(0),new.ImU8(1),tostring(fif(vr.missData['player'].modelType[0] == 0,langt['ped'],langt['specActor']))) then
        vr.missData['player']['modelId'][0] = 0
    end

    
    --skin popup open
    if vr.missData['player'].modelType[0] == 0 then
        if imgui.Button(faicons.ICON_TSHIRT) then
            imgui.OpenPopup('skins')
        end
        

        imgui.SameLine()

        if imgui.InputInt(langt['model'],vr.missData['player'].modelId) then
            local id_a = 0
            for v = 1,#ID_Actors do
                if vr.missData['player']['modelId'][0] <= ID_Actors[v] then
                    id_a = ID_Actors[v]
                    break
                end
            end
            vr.missData['player']['modelId'][0] = id_a
        end
    else
        imgui.Combo(langt['model'], vr.missData['player']['modelId'], new('const char* const [?]', #ID_Spec_Actors, ID_Spec_Actors),#ID_Spec_Actors)
    end
    imgui.Separator()

    if imgui.TreeNodeStr(langt['сhartics']) then
        imgui.InputInt(langt['countlive'],vr.missData['player']['health'])
        
        if imgui.ImageButton(weaponsAtlas, imgui.ImVec2(52,52),imgui.ImVec2((vr.missData['player'].weapon[0]-1)*0.02272727272,0), imgui.ImVec2(vr.missData['player'].weapon[0]*0.02272727272,1)) then
            imgui.OpenPopup('weapon')
        end

        imgui.SameLine()
        imgui.Text(langt["weapon"])

        if imgui.InputInt(langt['ammo'], vr.missData['player'].ammo) then
            if vr.missData['player'].ammo[0] < 0 then vr.missData['player'].ammo[0] = 0 end 
        end

            --weapon popup
        if imgui.BeginPopup('weapon') then
            imgui.BeginChild('weapon',imgui.ImVec2(200,450))
    
            for i = 1,#ID_Weapons do
                imgui.PushIDStr(tostring(i))
                if imgui.ImageButton(weaponsAtlas,imgui.ImVec2(52,52),imgui.ImVec2((i-1)*0.02272727272,0),imgui.ImVec2(i*0.02272727272,1)) then
                    vr.missData['player'].weapon[0] = i
                end
                if imgui.IsItemHovered() then
                    imgui.SetTooltip(vr.temp_var.weap_names[i])
                end
                imgui.PopID()
                if i % 3 ~= 0 then
                    imgui.SameLine()
                end
            end
    
            imgui.EndChild()
            imgui.EndPopup()
        end

        imgui.TreePop()
    end

    imgui.Separator()

    if imgui.Button(langt['edithand']) then
        vr.temp_var.editmodePlayer = true
        vr.player[0] = false
        lockPlayerControl(false)
    end

    --skin popup
    if imgui.BeginPopup('skins') then
        imgui.BeginChild('skins',imgui.ImVec2(210,450))


        for i = 1,#ID_Actors do
            imgui.PushIDStr(tostring(i))
            if imgui.ImageButton(pedsSkinAtlas,imgui.ImVec2(55,100),imgui.ImVec2(((i-1)*55)/14630,0),imgui.ImVec2((i*55)/14630,1)) then
                vr.missData['player']['modelId'][0] = ID_Actors[i]
            end
            imgui.PopID()
            if i % 3 ~= 0 then
                imgui.SameLine()
            end
        end

        imgui.EndChild()
        imgui.EndPopup()
    end
end)

--mission_pack
imgui.OnFrame(function() return vr.mission_packs[0] end,
function()
    imgui.SetNextWindowSize(imgui.ImVec2(270,410),imgui.Cond.Appearing)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x-270,0),imgui.Cond.Appearing)
    imgui.Begin(faicons.ICON_MALE..' '..langt['missionPacks'],nil, imgui.WindowFlags.AlwaysAutoResize)

    --List
    imgui.SetNextItemWidth(255)
    imgui.ListBoxStr_arr('', vr.current_mission_pack,new('const char* const [?]', #vr.temp_var.list_name_mission_packs, vr.temp_var.list_name_mission_packs),#vr.temp_var.list_name_mission_packs,15)

    if imgui.Button(langt['add']) then
        local name = langt['missionPack'].." #"..#vr.temp_var.list_name_mission_packs
        vr.temp_var.list_name_mission_packs[#vr.temp_var.list_name_mission_packs+1] = new.char[65](name)
        lfs.mkdir(getWorkingDirectory().."\\Missions_pack\\"..name)
        lfs.mkdir(getWorkingDirectory().."\\Missions_pack\\"..name..'\\audio')
        vr.current_mission_pack[0] = #vr.temp_var.list_name_mission_packs-1
    end
    if #vr.temp_var.list_name_mission_packs > 0 then
        imgui.SameLine()
        if imgui.Button(langt['rename']) then
            imgui.OpenPopup("rename")
        end
        imgui.SameLine()
        if imgui.Button(langt['delete']) then
            imgui.OpenPopup(faicons.ICON_TRASH_ALT.." "..langt['delete'])
        end
    end

    isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()


    if imgui.BeginPopupModal(faicons.ICON_TRASH_ALT.." "..langt['delete'], nil,imgui.WindowFlags.AlwaysAutoResize) then
        imgui.Text(langt['deleteQues'])

        local size_b = imgui.ImVec2(100,0)

        if imgui.Button(langt['yes'],size_b) then
            deletedir(getWorkingDirectory()..'\\Missions_pack\\'..ffi.string(vr.temp_var.list_name_mission_packs[vr.current_mission_pack[0]+1]))
            table.remove(vr.temp_var.list_name_mission_packs,vr.current_mission_pack[0]+1)
            if vr.current_mission_pack[0] > 0 then
                vr.current_mission_pack[0] = vr.current_mission_pack[0] - 1
            end
            imgui.CloseCurrentPopup()
        end
        imgui.SameLine()
        if imgui.Button(langt['no'],size_b) then
            imgui.CloseCurrentPopup()
        end
    end

    --Rename popup
    if imgui.BeginPopup("rename") then
        local old_name = ffi.string(vr.temp_var.list_name_mission_packs[vr.current_mission_pack[0]+1])
        if imgui.InputText('',vr.temp_var.list_name_mission_packs[vr.current_mission_pack[0]+1],ffi.sizeof(vr.temp_var.list_name_mission_packs[vr.current_mission_pack[0]+1])) then
            os.rename(getWorkingDirectory()..'\\Missions_pack\\'..old_name,getWorkingDirectory()..'\\Missions_pack\\'..ffi.string(vr.temp_var.list_name_mission_packs[vr.current_mission_pack[0]+1]))
        end

        if imgui.Button(langt['close']) then imgui.CloseCurrentPopup() end

        imgui.EndPopup()
    end

    imgui.End()

    -- mission render
    if #vr.temp_var.list_name_mission_packs > 0 then
        imgui.SetNextWindowSize(imgui.ImVec2(400,360),imgui.Cond.Always)
        imgui.SetNextWindowPos(imgui.ImVec2(res.x-670,0),imgui.Cond.Appearing)
        imgui.Begin(faicons.ICON_ARCHIVE..' '..langt['missions'],nil, imgui.WindowFlags.AlwaysAutoResize)
    
        --List
        imgui.SetNextItemWidth(255)
        imgui.ListBoxStr_arr('', vr.current_mission,new('const char* const [?]', #vr.temp_var.list_name_missions, vr.temp_var.list_name_missions),#vr.temp_var.list_name_missions,15)

        if imgui.Button(langt['add']) then
            local name = langt['mission'].." #"..#vr.temp_var.list_name_mission_packs
            vr.temp_var.list_name_missions[#vr.temp_var.list_name_missions+1] = name
            lfs.mkdir(getWorkingDirectory().."\\Missions_pack\\"..name)
            vr.current_mission[0] = #vr.temp_var.list_name_missions-1
        end
        if #vr.temp_var.list_name_missions > 0 then
            imgui.SameLine()
            if imgui.Button(langt['rename']) then
                imgui.OpenPopup("rename")
            end
            imgui.SameLine()
            if imgui.Button(langt['delete']) then
                imgui.OpenPopup(faicons.ICON_TRASH_ALT.." "..langt['delete'])
            end
        end

        isWindow = isWindow or imgui.IsWindowHovered() or imgui.IsAnyItemHovered()


        if imgui.BeginPopupModal(faicons.ICON_TRASH_ALT.." "..langt['delete'], nil,imgui.WindowFlags.AlwaysAutoResize) then
            imgui.Text(langt['deleteQues'])

            local size_b = imgui.ImVec2(100,0)

            if imgui.Button(langt['yes'],size_b) then
                os.remove(getWorkingDirectory()..'\\Missions_pack\\'..vr.temp_var.list_name_mission_packs[vr.current_mission_pack[0]+1].."\\"..vr.temp_var.list_name_missions[vr.current_mission[0]+1]..'.bin')
                table.remove(vr.temp_var.list_name_missions,vr.current_mission[0]+1)
                if vr.current_mission[0] > 0 then
                    vr.current_mission[0] = vr.current_mission[0] - 1
                end
                imgui.CloseCurrentPopup()
            end
            imgui.SameLine()
            if imgui.Button(langt['no'],size_b) then
                imgui.CloseCurrentPopup()
            end
        end

        --Rename popup
        if imgui.BeginPopup("rename") then
            local old_name = vr.temp_var.list_name_missions[vr.current_mission[0]+1]
            if imgui.InputText('',vr.temp_var.list_name_missions[vr.current_mission[0]+1],ffi.sizeof(vr.temp_var.list_name_missions[vr.current_mission[0]+1])) then
                os.rename(getWorkingDirectory()..'\\Missions_pack\\'..vr.temp_var.list_name_mission_packs[vr.current_mission_pack[0]+1].."\\"..old_name..'.bin',getWorkingDirectory()..'\\Missions_pack\\'..vr.temp_var.list_name_mission_packs[vr.current_mission_pack[0]+1].."\\"..vr.temp_var.list_name_missions[vr.current_mission[0]+1]..'.bin')
            end

            if imgui.Button(langt['close']) then imgui.CloseCurrentPopup() end

            imgui.EndPopup()
        end

        imgui.End()
    end

end)

--settings
imgui.OnFrame(function() return vr.settings[0] end,
function ()
    imgui.SetNextWindowSize(imgui.ImVec2(200,400),imgui.Cond.Always)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x/2-100,res.y/2-200),imgui.Cond.Always)
    imgui.Begin(faicons.ICON_CONGS..' '..langt['settings'],vr.mainMenu, imgui.WindowFlags.AlwaysAutoResize)

    imgui.PushItemWidth(125)
    if imgui.Combo(langt['style'],vr.temp_var.theme_curr,new('const char* const [?]', #vr.temp_var.theme_arr, vr.temp_var.theme_arr),#vr.temp_var.theme_arr) then
        vr.Data.Settings.curr_theme = ffi.string(vr.temp_var.theme_arr[vr.temp_var.theme_curr[0]+1])
        inicfg.save(vr.Data,getWorkingDirectory()..'\\LDYOM_data.ini')

        imgui.OpenPopup(faicons.ICON_EXCLAMATION_TRIANGLE.." "..langt['reload'])
    end

    if imgui.Combo(langt['lang'],vr.temp_var.curr_lang,new('const char* const [?]', #vr.temp_var.lang_list, vr.temp_var.lang_list),#vr.temp_var.lang_list) then
        vr.Data.Settings.curr_lang = LanguageList[vr.temp_var.curr_lang[0]+1]
        inicfg.save(vr.Data,getWorkingDirectory()..'\\LDYOM_data.ini')
        langt = inicfg.load(nil,getWorkingDirectory() .. '\\Language\\'..vr.Data.Settings.curr_lang..'.ini')['Keys']
        updLang()
    end

    if imgui.BeginPopupModal(faicons.ICON_EXCLAMATION_TRIANGLE.." "..langt['reload'], nil,imgui.WindowFlags.AlwaysAutoResize) then

        imgui.Text(vr.temp_var.reloadQues[1])
        imgui.Text(vr.temp_var.reloadQues[2])
        imgui.Text(vr.temp_var.reloadQues[3])

        local size_b = imgui.ImVec2(160,0)

        if imgui.Button(langt['yes'],size_b) then
            thisScript():reload()
        end
        imgui.SameLine()
        if imgui.Button(langt['no'],size_b) then
            imgui.CloseCurrentPopup()
        end

        imgui.EndPopup()
    end

    imgui.End()
end)

--tools
imgui.OnFrame(function() return vr.tools[0] end,
function ()
    imgui.SetNextWindowSize(imgui.ImVec2(420,200),imgui.Cond.Always)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x/2-210,res.y/2-100),imgui.Cond.Always)
    imgui.Begin(faicons.ICON_TOOLS..' '..langt['tools'],vr.mainMenu, imgui.WindowFlags.AlwaysAutoResize)

    if imgui.Button(langt['tool_tp_marker']) then
        local _,xx,xy,xz = getTargetBlipCoordinates()
        xz = getGroundZFor3dCoord(xx,xy,xz)
		setCharCoordinates(PLAYER_PED,xx,xy,xz)
    end
    if imgui.Button(langt['jetpack']) then
        taskJetpack(PLAYER_PED)
    end
    if imgui.Button(langt['enexMarker']) then
        disableAllEntryExits(vr.temp_var['enexMarker'][0])
    end
    imgui.Combo(langt['tool_tp_actor'],vr.temp_var.tools_var['tp_actor'],new('const char* const [?]', #vr.temp_var.list_name_actors, vr.temp_var.list_name_actors),#vr.temp_var.list_name_actors)
    imgui.SameLine()
    if imgui.Button(langt['teleport']) then
        local xx,yy,zz = vr.list_actors[vr.temp_var.tools_var['tp_actor'][0]+1]['data'].pos[0],vr.list_actors[vr.temp_var.tools_var['tp_actor'][0]+1]['data'].pos[1],vr.list_actors[vr.temp_var.tools_var['tp_actor'][0]+1]['data'].pos[2]
        setCharCoordinates(PLAYER_PED,xx,yy,zz)
    end
    imgui.Combo(langt['tool_tp_car'],vr.temp_var.tools_var['tp_car'],new('const char* const [?]', #vr.temp_var.list_name_cars, vr.temp_var.list_name_cars),#vr.temp_var.list_name_cars)
    imgui.SameLine()
    if imgui.Button(langt['teleport']) then
        local xx,yy,zz = vr.list_cars[vr.temp_var.tools_var['tp_car'][0]+1]['data'].pos[0],vr.list_cars[vr.temp_var.tools_var['tp_car'][0]+1]['data'].pos[1],vr.list_cars[vr.temp_var.tools_var['tp_car'][0]+1]['data'].pos[2]
        setCharCoordinates(PLAYER_PED,xx,yy,zz)
    end
    imgui.Combo(langt['tool_tp_object'],vr.temp_var.tools_var['tp_object'],new('const char* const [?]', #vr.temp_var.list_name_objects, vr.temp_var.list_name_objects),#vr.temp_var.list_name_objects)
    imgui.SameLine()
    if imgui.Button(langt['teleport']) then
        local xx,yy,zz = vr.list_objects[vr.temp_var.tools_var['tp_object'][0]+1]['data'].pos[0],vr.list_objects[vr.temp_var.tools_var['tp_object'][0]+1]['data'].pos[1],vr.list_objects[vr.temp_var.tools_var['tp_object'][0]+1]['data'].pos[2]
        setCharCoordinates(PLAYER_PED,xx,yy,zz)
    end

    imgui.End()
end)

--info
imgui.OnFrame(function() return vr.info[0] end,
function ()
    imgui.SetNextWindowSize(imgui.ImVec2(200,400),imgui.Cond.Always)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x/2-100,res.y/2-200),imgui.Cond.Always)
    imgui.Begin(faicons.ICON_TOOLS..' '..langt['info'],vr.mainMenu, imgui.WindowFlags.AlwaysAutoResize)

    local scr = thisScript()

    imgui.Text(vr.temp_var.info_t[3]..' '..scr.version)
    imgui.Spacing()
    imgui.Text(vr.temp_var.info_t[1]..' '..scr.authors[1])
    imgui.Text(vr.temp_var.info_t[2]..' '..scr.authors[2])
    imgui.Text(vr.temp_var.info_t[6]..' '..langt['nameLoc'])
    imgui.Spacing()
    imgui.Text(vr.temp_var.info_t[7])
    local namesHelpers = "Alexey Generalov, Ivan Kogotko"
    imgui.TextWrapped(namesHelpers)
    imgui.Text('')
    imgui.Button(faicons.ICON_BOOK..' '..vr.temp_var.info_t[8])

    imgui.End()
end)

--group relations
imgui.OnFrame(function() return vr.groupRelations[0] end,
function ()
    imgui.SetNextWindowSize(imgui.ImVec2(250,300),imgui.Cond.Always)
    imgui.SetNextWindowPos(imgui.ImVec2(res.x/2-125,res.y/2-150),imgui.Cond.Always)
    imgui.Begin(faicons.ICON_HANDSHAKE..' '..langt['groupRelations'],vr.mainMenu, imgui.WindowFlags.AlwaysAutoResize)

    imgui.PushItemWidth(125)
    for i = 2,9 do
        if imgui.TreeNodeStr(langt['group']..tostring(i-1)) then
            for y = 1,9 do
                if vr.missData.groupRelations[i][y] ~= 'NULL' then
                    imgui.SliderScalar(vr.temp_var.name_groups[y], imgui.DataType.U8, vr.missData.groupRelations[i][y], new.ImU8(0), new.ImU8(4),vr.temp_var.Relationship_types[vr.missData.groupRelations[i][y][0]+1])
                end
            end
            imgui.TreePop()
        end
    end

    imgui.End()
end)

function onStartNewGame(missionPackNumber)
	vr.mpack = missionPackNumber
	if missionPackNumber ~= 7 then
		thisScript():pause()
	else
		thisScript():resume()
	end
end

function updLang()
    setGxtEntry("HOBJ",koder(cyr(langt['HOBJ'])))
    setGxtEntry("HMTIM",koder(cyr(langt['HMTIM'])))
    setGxtEntry("HVIEW",koder(cyr(langt['HVIEW'])))

    vr.temp_var.type_targets_name = decodeJson(langt['targets_list_arr'])

    vr.temp_var.targets_marker_color = decodeJson(langt['targets_marker_color'])

    vr.temp_var.weap_names = decodeJson(langt['weap_names'])
    
    vr.temp_var.infoOverlay = decodeJson(langt['infoOverlay'])
    
    vr.temp_var.reloadQues = decodeJson(langt['reloadQues'])
    
    vr.temp_var.weap_names = decodeJson(langt['weap_names'])
    
    vr.temp_var.car_unbreak = decodeJson(langt['car_unbreak'])
    
    vr.temp_var.name_groups = decodeJson(langt['name_groups'])
    
    vr.temp_var.CutscenePos = decodeJson(langt['CutscenePos'])
    
    vr.temp_var.CutsceneLook = decodeJson(langt['CutsceneLook'])
    
    vr.temp_var.Weather_arr = decodeJson(langt['Weather_arr'])

    vr.temp_var.countTraffic = decodeJson(langt['countTraffic'])
    
    vr.temp_var.targetTypeObj = decodeJson(langt['targetTypeObj'])
    
    vr.temp_var.place_car = decodeJson(langt['place_car'])
    
    vr.temp_var.type_pickup = decodeJson(langt['type_pickup'])
    
    vr.temp_var.spawn_type_pickup = decodeJson(langt['spawn_type_pickup'])
    
    vr.temp_var.type_expl = decodeJson(langt['type_expl'])
    
    vr.temp_var.place_type = decodeJson(langt['place_type'])
    
    vr.temp_var.Relationship_types = decodeJson(langt['Relationship_types'])
    
    vr.temp_var.info_t = decodeJson(langt['info_t'])

end

function update_actor(actorr)
	local modell
	local xx,xy,xz = vr.list_actors[actorr]['data']['pos'][0], vr.list_actors[actorr]['data']['pos'][1], vr.list_actors[actorr]['data']['pos'][2]
	local angle = vr.list_actors[actorr]['data']['angle'][0]

	if vr.list_actors[actorr]['data']['modelType'][0] == 0 then
		modell = vr.list_actors[actorr]['data']['modelId'][0]
		requestModel(modell)
		while not hasModelLoaded(modell) do
			wait(0)
		end
	elseif vr.list_actors[actorr]['data']['modelType'][0] == 1 then
		local modell_n = ID_Spec_Actors[vr.list_actors[actorr]['data']['modelId'][0]+1]
		loadSpecialCharacter(modell_n,vr.list_actors[actorr]['data']['slotSkin'][0])
		while not hasSpecialCharacterLoaded(vr.list_actors[actorr]['data']['slotSkin'][0]) do
			wait(0)
		end
		modell = 290 + vr.list_actors[actorr]['data']['slotSkin'][0]-1
	end

	deleteChar(vr.list_actors[actorr]['data']['char'])
	print(modell)
	vr.list_actors[actorr]['data']['char'] = createChar(4,modell,xx,xy,xz)
	if vr.list_actors[actorr]['data']['modelType'][0] == 0 then
		markModelAsNoLongerNeeded(modell)
	else
		unloadSpecialCharacter(vr.list_actors[actorr]['data']['slotSkin'][0])
	end
	setCharHeading(vr.list_actors[actorr]['data']['char'], angle)
	setCharCollision(vr.list_actors[actorr]['data']['char'],false)
	requestModel(getWeapontypeModel(ID_Weapons[vr.list_actors[actorr]['data']['weapon'][0]]))
	while not hasModelLoaded(getWeapontypeModel(ID_Weapons[vr.list_actors[actorr]['data']['weapon'][0]])) do
		wait(0)
	end
	giveWeaponToChar(vr.list_actors[actorr]['data']['char'], ID_Weapons[vr.list_actors[actorr]['data']['weapon'][0]], 1)
	setCurrentCharWeapon(vr.list_actors[actorr]['data']['char'],1)
	markModelAsNoLongerNeeded(getWeapontypeModel(ID_Weapons[vr.list_actors[actorr]['data']['weapon'][0]]))
end

function update_car(carr,recolor)
	local modell = vr.list_cars[carr]['data']['modelId'][0]
	for v = 1,#ID_Cars do
		if vr.list_cars[carr]['data']['modelId'][0] <= ID_Cars[v] then
			modell = ID_Cars[v]
			break
		end
	end
	local xx,xy,xz = vr.list_cars[carr]['data']['pos'][0], vr.list_cars[carr]['data']['pos'][1], vr.list_cars[carr]['data']['pos'][2]
	local angle = vr.list_cars[carr]['data']['angle'][0]

	requestModel(modell)
	while not hasModelLoaded(modell) do
		wait(0)
	end

	deleteCar(vr.list_cars[carr]['data']['car'])
	vr.list_cars[carr]['data']['car'] = createCar(modell, xx, xy, xz)
	setCarHeading(vr.list_cars[carr]['data']['car'], angle)
	setCarCollision(vr.list_cars[carr]['data']['car'],false)
	freezeCarPosition(vr.list_cars[carr]['data']['car'], true)
	setCarCanBeDamaged(vr.list_cars[carr]['data']['car'], false)
	--changeCarColour(vr.list_cars[carr]['data']['car'], vr.list_cars[carr]['data']['Color_primary'],vr.list_cars[carr]['Car_Data']['Color_secondary'])
    markModelAsNoLongerNeeded(modell)
    if recolor then
        vr.list_cars[carr]['data'].colors = {}
        local comps = mad.get_all_vehicle_components(tonumber(vr.list_cars[carr]['data']['car']))
        for_each_vehicle_material(vr.list_cars[carr]['data'].car,function(i,mat, comp, obj)
            local r,g,b,a = mat:get_color()
            local colorr
            local typee
            if r == 0x3C and g == 0xFF and b == 0x00 then
                local prim = getCarColours(vr.list_cars[carr]['data'].car)
                colorr = imgui.ImColor.ImColorU32(get_car_color_rgba(prim))
                vr.list_cars[carr]['data'].primaryColor[0] = colorr.Value.x
                vr.list_cars[carr]['data'].primaryColor[1] = colorr.Value.y
                vr.list_cars[carr]['data'].primaryColor[2] = colorr.Value.z
                vr.list_cars[carr]['data'].primaryColor[3] = colorr.Value.w
                typee = 0
            elseif r == 0xFF and g == 0x00 and b == 0xAF then
                local _, sec = getCarColours(vr.list_cars[carr]['data'].car)
                colorr = imgui.ImColor.ImColorU32(get_car_color_rgba(sec))
                vr.list_cars[carr]['data'].secondaryColor[0] = colorr.Value.x
                vr.list_cars[carr]['data'].secondaryColor[1] = colorr.Value.y
                vr.list_cars[carr]['data'].secondaryColor[2] = colorr.Value.z
                vr.list_cars[carr]['data'].secondaryColor[3] = colorr.Value.w
                typee = 1
            else
                colorr = imgui.ImColor.ImColorInt(r, g, b, a)
                typee = 2
            end
            vr.list_cars[carr]['data'].colors[#vr.list_cars[carr]['data'].colors+1] = {typee,new.float[4](colorr.Value.x,colorr.Value.y,colorr.Value.z,colorr.Value.w)}

        end)
    else
        for_each_vehicle_material(vr.list_cars[carr]['data']['car'],function(i,mat, comp, obj)
            local new_r, new_g, new_b, a = vr.list_cars[carr]['data'].colors[i][2][0],vr.list_cars[carr]['data'].colors[i][2][1],vr.list_cars[carr]['data'].colors[i][2][2],vr.list_cars[carr]['data'].colors[i][2][3]
            mat:set_color(new_r*255, new_g*255, new_b*255, a*255)
        end)
    end
end

function update_object(objj)
	local modell = vr.list_objects[objj]['data']['modelId'][0]
	local xx,xy,xz = vr.list_objects[objj]['data']['pos'][0], vr.list_objects[objj]['data']['pos'][1], vr.list_objects[objj]['data']['pos'][2]
	local rxx,rxy,rxz = vr.list_objects[objj]['data']['rotate'][0], vr.list_objects[objj]['data']['rotate'][1], vr.list_objects[objj]['data']['rotate'][2]

	requestModel(modell)
	while not hasModelLoaded(modell) do
		wait(0)
	end

	deleteObject(vr.list_objects[objj]['data']['obj'])
    vr.list_objects[objj]['data']['obj'] = createObject(modell, xx, xy, xz)
    
    markModelAsNoLongerNeeded(modell)

	setObjectCoordinates(vr.list_objects[objj]['data']['obj'], xx, xy, xz)
	setObjectRotation(vr.list_objects[objj]['data']['obj'], rxx, rxy, rxz)
	setObjectCollision(vr.list_objects[objj]['data']['obj'],false)
end

function update_particle(prtcl)
	local modell = Particle_name[vr.list_particles[prtcl]['data']['modelId'][0]+1]
	local xx,xy,xz = vr.list_particles[prtcl]['data']['pos'][0], vr.list_particles[prtcl]['data']['pos'][1], vr.list_particles[prtcl]['data']['pos'][2]
	local rxx,rxy,rxz = vr.list_particles[prtcl]['data']['rotate'][0], vr.list_particles[prtcl]['data']['rotate'][1], vr.list_particles[prtcl]['data']['rotate'][2]

    killFxSystem(vr.list_particles[prtcl]['data']['prtcl'][1])
    if vr.list_particles[prtcl]['data']['prtcl'][2] then deleteObject(vr.list_particles[prtcl]['data']['prtcl'][2]) end
    wait(0)
    if not hasModelLoaded(327) then
        requestModel(327)
        while not hasModelLoaded(327) do
            wait(0)
        end
    end
    vr.list_particles[prtcl]['data']['prtcl'][2] = createObject(327,xx, xy, xz)
    vr.list_particles[prtcl]['data']['prtcl'][1] = createFxSystemOnObjectWithDirection(modell,vr.list_particles[prtcl]['data']['prtcl'][2],0,0,0,rxx,rxy,rxz, 1)
    playFxSystem(vr.list_particles[prtcl]['data']['prtcl'][1])
    wait(0)
    setObjectVisible(vr.list_particles[prtcl]['data']['prtcl'][2],false)
    setObjectCoordinates(vr.list_particles[prtcl]['data']['prtcl'][2], xx, xy, xz)
end

function update_pickup(pickk)
	removePickup(vr.list_pickups[pickk]['data']['pick'])
	wait(1)
	local xx,xy,xz = vr.list_pickups[pickk]['data']['pos'][0],vr.list_pickups[pickk]['data']['pos'][1],vr.list_pickups[pickk]['data']['pos'][2]
    vr.list_pickups[pickk]['data']['pos'][2] = getGroundZFor3dCoord(xx, xy, xz+1) + 1
	xz = vr.list_pickups[pickk]['data']['pos'][2]

	if vr.list_pickups[pickk]['data']['type'][0] == 0 then
		if not hasModelLoaded(getWeapontypeModel(ID_Weapons[vr.list_pickups[pickk]['data']['weapon'][0]])) then
			requestModel(getWeapontypeModel(ID_Weapons[vr.list_pickups[pickk]['data']['weapon'][0]]))
            while not hasModelLoaded(getWeapontypeModel(ID_Weapons[vr.list_pickups[pickk]['data']['weapon'][0]])) do
                wait(0)
            end
        end
		vr.list_pickups[pickk]['data']['pick'] = createPickupWithAmmo(getWeapontypeModel(ID_Weapons[vr.list_pickups[pickk]['data']['weapon'][0]]),9,1,xx,xy,xz)
	elseif vr.list_pickups[pickk]['data']['type'][0] == 1 then
		if not hasModelLoaded(1240) then
			requestModel(1240)
			while not hasModelLoaded(1240) do
				wait(0)
			end
		end
		vr.list_pickups[pickk]['data']['pick'] = select(2,createPickup(1240,9,xx,xy,xz))
	elseif vr.list_pickups[pickk]['data']['type'][0] == 2 then
		if not hasModelLoaded(1242) then
			requestModel(1242)
			while not hasModelLoaded(1242) do
				wait(0)
			end
		end
		vr.list_pickups[pickk]['data']['pick'] = select(2,createPickup(1242,9,xx,xy,xz))
	elseif vr.list_pickups[pickk]['data']['type'][0] == 3 then
		if not hasModelLoaded(1247) then
			requestModel(1247)
			while not hasModelLoaded(1247) do
				wait(0)
			end
		end
		vr.list_pickups[pickk]['data']['pick'] = select(2,createPickup(1247,9,xx,xy,xz))
	elseif vr.list_pickups[pickk]['data']['type'][0] == 4 then
		if not hasModelLoaded(1241) then
			requestModel(1241)
			while not hasModelLoaded(1241) do
				wait(0)
			end
		end
		vr.list_pickups[pickk]['data']['pick'] = select(2,createPickup(1241,9,xx,xy,xz))
	elseif vr.list_pickups[pickk]['data']['type'][0] == 5 then
		if not hasModelLoaded(vr.list_pickups[pickk]['data']['modelId'][0]) then
			requestModel(vr.list_pickups[pickk]['data']['modelId'][0])
			while not hasModelLoaded(vr.list_pickups[pickk]['data']['modelId'][0]) do
				wait(0)
			end
		end
		vr.list_pickups[pickk]['data']['pick'] = select(2,createPickup(vr.list_pickups[pickk]['data']['modelId'][0],9,xx,xy,xz))
	end
end

function update_explosion(expl)
	local xx,xy,xz = vr.list_explosions[expl]['data']['pos'][0],vr.list_explosions[expl]['data']['pos'][1],vr.list_explosions[expl]['data']['pos'][2]
    if vr.list_explosions[expl]['data']['type'][0] == 1 then
        if not doesObjectExist(vr.list_explosions[expl]['data']['explosion']) then
            if vr.list_explosions[expl]['data']['fire'] then
                removeScriptFire(vr.list_explosions[expl]['data']['fire'])
            end
            wait(0)
			if not hasModelLoaded(1654) then
				requestModel(1654)
				while not hasModelLoaded(1654) do
					wait(0)
				end
			end
			vr.list_explosions[expl]['data']['explosion'] = createObject(1654,xx,xy,xz)
			setObjectCollision(vr.list_explosions[expl]['data']['explosion'],false)
		end
		setObjectCoordinates(vr.list_explosions[expl]['data']['explosion'],xx,xy,xz)
    elseif vr.list_explosions[expl]['data']['type'][0] == 0 then
        if vr.list_explosions[expl]['data']['fire'] then
            removeScriptFire(vr.list_explosions[expl]['data']['fire'])
        end
        if vr.list_explosions[expl]['data']['explosion'] then
            deleteObject(vr.list_explosions[expl]['data']['explosion'])
        end
		wait(0)
		vr.list_explosions[expl]['data']['fire'] = startScriptFire(xx,xy,xz,0,vr.list_explosions[expl]['data']['sizeFire'][0])
	end
end

function update_audio(aud)
    if vr.list_audios[aud]['data']['audio3d'][0] and vr.list_audios[aud]['data']['audio3dType'][0] == 0 then
        local xx,yy,zz = getCharCoordinates(PLAYER_PED)
        vr.list_audios[aud]['data']['pos'][0] = xx
        vr.list_audios[aud]['data']['pos'][1] = yy
        vr.list_audios[aud]['data']['pos'][2] = zz
        requestModel(2231)
        while not hasModelLoaded(2231) do
            wait(0)
        end
        vr.list_audios[aud]['data']['obj'] = createObject(2231,xx,yy,zz)
        setObjectCollision(vr.list_audios[aud]['data']['obj'],false)
        setObjectCoordinates(vr.list_audios[aud]['data']['obj'],xx,yy,zz)
    elseif vr.list_audios[aud]['data']['obj'] then
        deleteObject(vr.list_audios[aud]['data']['obj'])
        vr.list_audios[aud]['data']['obj'] = nil
    end
end

function main()

    bitser.register('my_ctype', ffi.typeof(ffi.new("int",1)))
    bitser.register('my_ctype1', ffi.typeof(ffi.new("int",67)))
    
    local var = {new.int(23),5,new.bool(true)}
    local str = bitser.dumps(var)
    print(str)
    local var1 = bitser.loads(str)
    print(var1[1],var1[3][0])
    print(var1[1],var1[3][0])

    

    vr.Data = inicfg.load(nil,getWorkingDirectory()..'\\LDYOM_data.ini')

    --Load lang
    local lang_list = {}
    for file in lfs.dir(getWorkingDirectory() .. '\\Language') do
        if lfs.attributes(getWorkingDirectory() .. '\\Language\\'..file,"mode") == "file" then
            if string.sub(file, -3) == 'ini' then
                local lang = inicfg.load(nil,getWorkingDirectory() .. '\\Language\\'..file)
                lang_list[#lang_list+1] = lang['Info']['name']
                LanguageList[#LanguageList+1] = string.sub(file, 0, -5)--lang['Info']['name']
            end
        end
    end
    vr.temp_var.lang_list = lang_list
    for i,v in ipairs(LanguageList) do
        if v == vr.Data.Settings.curr_lang then
            vr.temp_var.curr_lang[0] = i-1
        end
    end
    langt = inicfg.load(nil,getWorkingDirectory() .. '\\Language\\'..vr.Data.Settings.curr_lang..'.ini')['Keys']
    wait(1)
    updLang()


    --Load Theme
    local theme_list = {}
    for file in lfs.dir(getWorkingDirectory() .. '\\lib\\Theme') do
        if lfs.attributes(getWorkingDirectory() .. '\\lib\\Theme\\'..file,"mode") == "file" then
            if string.sub(file, -3) == 'lua' then
                print(string.sub(file, 0,-5))
                theme_list[#theme_list+1] = string.sub(file, 0,-5)
            end
        end
    end
    vr.temp_var.theme_arr = theme_list
    for i,v in ipairs(theme_list) do
        if v == vr.Data.Settings.curr_theme then
            vr.temp_var.theme_curr[0] = i-1
        end
    end

    vr.missData.groupRelations = {}
	vr.missData.groupRelations[1] = 'NULL'
	for i = 2,9 do
		vr.missData.groupRelations[i] = {}
		for y = 1,9 do
			if i ~= y then
				vr.missData.groupRelations[i][y] = new.ImU8(2)
			else
				vr.missData.groupRelations[i][y] = 'NULL'
			end
		end
	end
    
    upd_actor = lua_thread.create_suspended(update_actor)
    upd_car = lua_thread.create_suspended(update_car)
    upd_object = lua_thread.create_suspended(update_object)
    upd_particle = lua_thread.create_suspended(update_particle)
    upd_pickup = lua_thread.create_suspended(update_pickup)
    upd_explosion = lua_thread.create_suspended(update_explosion)
    upd_audio = lua_thread.create_suspended(update_audio)

    nodes_s.show[0] = true
    while true do
        wait(0)

        if testCheat('top2009') then
			printHelpString('~r~I LOVE TWENTY ONE PILOTS')
        end


        if vr.temp_var.editActor then
            lockPlayerControl(false)
            restoreCamera()
            showCursor(false)
            if isKeyJustPressed(vkeys.VK_F) then
                vr.temp_var.editActor = false

                setPlayerModel(PLAYER_HANDLE, 0)
                local xx,xy,xz = getCharCoordinates(PLAYER_PED)
                local angle = getCharHeading(PLAYER_PED)
                xz = getGroundZFor3dCoord(xx,xy,xz)
                vr.list_actors[vr.current_actor[0]+1]['data']['pos'][0] = xx
                vr.list_actors[vr.current_actor[0]+1]['data']['pos'][1] = xy
                vr.list_actors[vr.current_actor[0]+1]['data']['pos'][2] = xz
                vr.list_actors[vr.current_actor[0]+1]['data']['angle'][0] = angle
                upd_actor:run(vr.current_actor[0]+1)
                lockPlayerControl(true)
            end
        end

        if vr.temp_var.editmodeCamera then
			displayRadar(false)
			displayHud(false)

			local lines = false

			while vr.temp_var.editmodeCamera do
				wait(0)
				--Переменная замедления
				local multy = 1
				if isKeyDown(vkeys.VK_CONTROL) then
					multy = 0.5
				else
					multy = 1
				end

				if isKeyDown(vkeys.VK_A) and not isKeyDown(vkeys.VK_SHIFT) then
					local sinn = math.sin(math.rad(vr.list_targets[vr.current_target[0]+1]['data']['rotate'][1]+90))
					local coss = math.cos(math.rad(vr.list_targets[vr.current_target[0]+1]['data']['rotate'][1]+90))
					vr.list_targets[vr.current_target[0]+1]['data']['pos'][0] = vr.list_targets[vr.current_target[0]+1]['data']['pos'][0] - 0.2 *sinn * multy
					vr.list_targets[vr.current_target[0]+1]['data']['pos'][1] = vr.list_targets[vr.current_target[0]+1]['data']['pos'][1] - 0.2 *coss * multy
				end
				if isKeyDown(vkeys.VK_D) and not isKeyDown(vkeys.VK_SHIFT) then
					local sinn = math.sin(math.rad(vr.list_targets[vr.current_target[0]+1]['data']['rotate'][1]-90))
					local coss = math.cos(math.rad(vr.list_targets[vr.current_target[0]+1]['data']['rotate'][1]-90))
					vr.list_targets[vr.current_target[0]+1]['data']['pos'][0] = vr.list_targets[vr.current_target[0]+1]['data']['pos'][0] - 0.2 *sinn * multy
					vr.list_targets[vr.current_target[0]+1]['data']['pos'][1] = vr.list_targets[vr.current_target[0]+1]['data']['pos'][1] - 0.2 *coss * multy
				end
				if isKeyDown(vkeys.VK_W) and not isKeyDown(vkeys.VK_SHIFT) then
					local sinn = math.sin(math.rad(vr.list_targets[vr.current_target[0]+1]['data']['rotate'][1]))
					local coss = math.cos(math.rad(vr.list_targets[vr.current_target[0]+1]['data']['rotate'][1]))
					vr.list_targets[vr.current_target[0]+1]['data']['pos'][0] = vr.list_targets[vr.current_target[0]+1]['data']['pos'][0] + 0.2 *sinn * multy
					vr.list_targets[vr.current_target[0]+1]['data']['pos'][1] = vr.list_targets[vr.current_target[0]+1]['data']['pos'][1] + 0.2 *coss * multy
				end
				if isKeyDown(vkeys.VK_S) and not isKeyDown(vkeys.VK_SHIFT)  then
					local sinn = math.sin(math.rad(vr.list_targets[vr.current_target[0]+1]['data']['rotate'][1]))
					local coss = math.cos(math.rad(vr.list_targets[vr.current_target[0]+1]['data']['rotate'][1]))
					vr.list_targets[vr.current_target[0]+1]['data']['pos'][0] = vr.list_targets[vr.current_target[0]+1]['data']['pos'][0] - 0.2 *sinn * multy
					vr.list_targets[vr.current_target[0]+1]['data']['pos'][1] = vr.list_targets[vr.current_target[0]+1]['data']['pos'][1] - 0.2 *coss * multy
				end
				if isKeyDown(vkeys.VK_Q) and not isKeyDown(vkeys.VK_SHIFT)  then
					vr.list_targets[vr.current_target[0]+1]['data']['pos'][2] = vr.list_targets[vr.current_target[0]+1]['data']['pos'][2] + 0.2 * multy
				end
				if isKeyDown(vkeys.VK_E) and not isKeyDown(vkeys.VK_SHIFT)  then
					vr.list_targets[vr.current_target[0]+1]['data']['pos'][2] = vr.list_targets[vr.current_target[0]+1]['data']['pos'][2] - 0.2 * multy
				end
				if wasKeyReleased(vkeys.VK_R) then
					if lines then

						lines = false
					else
						lines = true
					end
				end

				if lines then
					local wieght_line,height_line = getScreenResolution()
					for i = 1, 2 do
						renderDrawLine(0,(height_line/3)*i,wieght_line,(height_line/3)*i,1,rgba_to_int(255,255,255,255))
						renderDrawLine((wieght_line/3)*i,0,(wieght_line/3)*i,height_line,1,rgba_to_int(255,255,255,255))
					end
				end

				if isKeyDown(vkeys.VK_SHIFT) then
					if isKeyDown(vkeys.VK_A) then
						vr.list_targets[vr.current_target[0]+1]['data']['rotate'][1] = vr.list_targets[vr.current_target[0]+1]['data']['rotate'][1] - 2 * multy
					end
					if isKeyDown(vkeys.VK_D) then
						vr.list_targets[vr.current_target[0]+1]['data']['rotate'][1] = vr.list_targets[vr.current_target[0]+1]['data']['rotate'][1] + 2 * multy
					end
					if isKeyDown(vkeys.VK_W) then
						vr.list_targets[vr.current_target[0]+1]['data']['rotate'][0] = vr.list_targets[vr.current_target[0]+1]['data']['rotate'][0] - 2 * multy
					end
					if isKeyDown(vkeys.VK_S) then
						vr.list_targets[vr.current_target[0]+1]['data']['rotate'][0] = vr.list_targets[vr.current_target[0]+1]['data']['rotate'][0] + 2 * multy
					end
					if isKeyDown(vkeys.VK_E) then
						vr.list_targets[vr.current_target[0]+1]['data']['rotate'][2] = vr.list_targets[vr.current_target[0]+1]['data']['rotate'][2] - 2 * multy
					end
					if isKeyDown(vkeys.VK_Q) then
						vr.list_targets[vr.current_target[0]+1]['data']['rotate'][2] = vr.list_targets[vr.current_target[0]+1]['data']['rotate'][2] + 2 * multy
					end
				end

				local xx,xy,xz = vr.list_targets[vr.current_target[0]+1]['data']['pos'][0],vr.list_targets[vr.current_target[0]+1]['data']['pos'][1],vr.list_targets[vr.current_target[0]+1]['data']['pos'][2]
				local rxx,rxy,rxz = vr.list_targets[vr.current_target[0]+1]['data']['rotate'][0],vr.list_targets[vr.current_target[0]+1]['data']['rotate'][1],vr.list_targets[vr.current_target[0]+1]['data']['rotate'][2]
				local x1,y1,z1 = xx,xy,xz
				x1 = x1 + 2*math.sin(math.rad(rxy)) * math.sin(math.rad(rxx))
				y1 = y1 + 2*math.cos(math.rad(rxy)) * math.sin(math.rad(rxx))
				z1 = z1 + 2*math.cos(math.rad(rxx))

				--setObjectCoordinates(chr, x1, y1, z1)
				setFixedCameraPosition(xx, xy, xz,0,rxz,0)
				pointCameraAtPoint(x1, y1, z1, 2)
				setCinemaCamera(false)

				--Закрытие редактора
				if wasKeyReleased(vkeys.VK_F) then
					lockPlayerControl(true)
					clearHelp()
					restoreCamera()
                    vr.temp_var.editmodeCamera = false

                    vr.list_targets[vr.current_target[0]+1]['data']['rotate'][0] = optimAngle(vr.list_targets[vr.current_target[0]+1]['data']['rotate'][0])
                    vr.list_targets[vr.current_target[0]+1]['data']['rotate'][1] = optimAngle(vr.list_targets[vr.current_target[0]+1]['data']['rotate'][1])
                    vr.list_targets[vr.current_target[0]+1]['data']['rotate'][2] = optimAngle(vr.list_targets[vr.current_target[0]+1]['data']['rotate'][2])

                    vr.targets[0] = true
					displayRadar(true)
					displayHud(true)
				end
			end
        end

        while vr.temp_var.editmodeTimetarg do
			wait(0)
			if isKeyDown(vkeys.VK_I) then
				wait(100)
				vr.list_targets[vr.current_target[0]+1]['data']['time'][0] = vr.list_targets[vr.current_target[0]+1]['data']['time'][0] + 1
			end
			if isKeyDown(vkeys.VK_O) then
				wait(100)
				vr.list_targets[vr.current_target[0]+1]['data']['time'][0] = vr.list_targets[vr.current_target[0]+1]['data']['time'][0] - 1
			end
			if isKeyDown(vkeys.VK_K) then
				wait(100)
				vr.list_targets[vr.current_target[0]+1]['data']['time'][1] = vr.list_targets[vr.current_target[0]+1]['data']['time'][1] + 1
			end
			if isKeyDown(vkeys.VK_L) then
				wait(100)
				vr.list_targets[vr.current_target[0]+1]['data']['time'][1] = vr.list_targets[vr.current_target[0]+1]['data']['time'][1] - 1
			end
			if vr.list_targets[vr.current_target[0]+1]['data']['time'][1] == 60 then
				vr.list_targets[vr.current_target[0]+1]['data']['time'][0] = vr.list_targets[vr.current_target[0]+1]['data']['time'][0] + 1
				vr.list_targets[vr.current_target[0]+1]['data']['time'][1] = 0
			elseif vr.list_targets[vr.current_target[0]+1]['data']['time'][1] == -1 then
				vr.list_targets[vr.current_target[0]+1]['data']['time'][0] = vr.list_targets[vr.current_target[0]+1]['data']['time'][0] - 1
				vr.list_targets[vr.current_target[0]+1]['data']['time'][1] = 59
			end
			if vr.list_targets[vr.current_target[0]+1]['data']['time'][0] == 24 then
				vr.list_targets[vr.current_target[0]+1]['data']['time'][0] = 0
			elseif vr.list_targets[vr.current_target[0]+1]['data']['time'][0] == -1 then
				vr.list_targets[vr.current_target[0]+1]['data']['time'][0] = 23
			end
			setTimeOfDay(vr.list_targets[vr.current_target[0]+1]['data']['time'][0], vr.list_targets[vr.current_target[0]+1]['data']['time'][1])
            --Закрытие редактора
			if wasKeyReleased(vkeys.VK_F) then
				clearHelp()
                vr.temp_var.editmodeTimetarg = false
                setTimeOfDay(8,0)
                vr.targets[0] = true
				openMenu = true
			end
        end
        
        if vr.temp_var.editmodeTeleportPlayer then
            printHelpForever("HVIEW")
			setCharCoordinates(PLAYER_PED, vr.list_targets[vr.current_target[0]+1]['data']['pos'][0], vr.list_targets[vr.current_target[0]+1]['data']['pos'][1], vr.list_targets[vr.current_target[0]+1]['data']['pos'][2])
			local modell
			if vr.list_targets[vr.current_target[0]+1]['data']['modelType'][0] == 0 then
				modell = vr.list_targets[vr.current_target[0]+1]['data'].modelId[0]
				requestModel(modell)
				while not hasModelLoaded(modell) do
					wait(0)
				end
			else
				local modell_n = ID_Spec_Actors[vr.list_targets[vr.current_target[0]+1]['data'].modelId[0]+1]
				loadSpecialCharacter(modell_n,10)
				while not hasSpecialCharacterLoaded(10) do
					wait(0)
				end
				modell = 290 + 10-1
			end
			setPlayerModel(PLAYER_HANDLE, modell)
			setCharHeading(PLAYER_PED, vr.list_targets[vr.current_target[0]+1]['data']['angle'][0])
			setInteriorVisible(vr.list_targets[vr.current_target[0]+1]['data']['interiorId'][0])
			setCharInterior(PLAYER_PED, vr.list_targets[vr.current_target[0]+1]['data']['interiorId'][0])
			while vr.temp_var.editmodeTeleportPlayer do
				--Закрытие редактора
				wait(0)
				if wasKeyReleased(vkeys.VK_F) then
					clearHelp()
                    vr.temp_var.editmodeTeleportPlayer = false
                    vr.targets[0] = true
                    setPlayerModel(PLAYER_HANDLE, model.NULL)
                    local xx,xy,xz = getCharCoordinates(PLAYER_PED)
                    local angle = getCharHeading(PLAYER_PED)
                    xz = getGroundZFor3dCoord(xx,xy,xz)
                    vr.list_targets[vr.current_target[0]+1]['data']['pos'][0] = xx
                    vr.list_targets[vr.current_target[0]+1]['data']['pos'][1] = xy
                    vr.list_targets[vr.current_target[0]+1]['data']['pos'][2] = xz
                    vr.list_targets[vr.current_target[0]+1]['data']['angle'][0] = angle
                    vr.list_targets[vr.current_target[0]+1]['data']['interiorId'][0] = getActiveInterior()
				end
			end
        end
        
        if vr.temp_var.editmodePlayer then
            printHelpForever("HVIEW")
            lockPlayerControl(false)
			setCharCoordinates(PLAYER_PED, vr.missData['player']['pos'][0], vr.missData['player']['pos'][1], vr.missData['player']['pos'][2])
			local modell
			if vr.missData['player']['modelType'][0] == 0 then
				modell = vr.missData['player'].modelId[0]
				requestModel(modell)
				while not hasModelLoaded(modell) do
					wait(0)
				end
			else
				local modell_n = ID_Spec_Actors[vr.missData['player'].modelId[0]+1]
				loadSpecialCharacter(modell_n,10)
				while not hasSpecialCharacterLoaded(10) do
					wait(0)
				end
				modell = 290 + 10-1
			end
			setPlayerModel(PLAYER_HANDLE, modell)
			setCharHeading(PLAYER_PED, vr.missData['player']['angle'][0])
			setInteriorVisible(vr.missData['player']['interiorId'][0])
			setCharInterior(PLAYER_PED, vr.missData['player']['interiorId'][0])
			while vr.temp_var.editmodePlayer do
				--Закрытие редактора
				wait(0)
				if wasKeyReleased(vkeys.VK_F) then
					clearHelp()
                    vr.temp_var.editmodePlayer = false
                    vr.player[0] = true
                    setPlayerModel(PLAYER_HANDLE, model.NULL)
                    lockPlayerControl(true)
                    local xx,xy,xz = getCharCoordinates(PLAYER_PED)
                    local angle = getCharHeading(PLAYER_PED)
                    xz = getGroundZFor3dCoord(xx,xy,xz)
                    vr.missData['player']['pos'][0] = xx
                    vr.missData['player']['pos'][1] = xy
                    vr.missData['player']['pos'][2] = xz
                    vr.missData['player']['angle'][0] = angle
                    vr.missData['player']['interiorId'][0] = getActiveInterior()
				end
			end
		end

        if vr.temp_var.editmodeCar then
            lockPlayerControl(false)
            restoreCamera()
			local carr = createCar(vr.list_cars[vr.current_car[0]+1]['data']['modelId'][0], vr.list_cars[vr.current_car[0]+1]['data']['pos'][0], vr.list_cars[vr.current_car[0]+1]['data']['pos'][1], vr.list_cars[vr.current_car[0]+1]['data']['pos'][2])
            for_each_vehicle_material(carr,function(i,mat, comp, obj)
                local new_r, new_g, new_b, a = vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][0],vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][1],vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][2],vr.list_cars[vr.current_car[0]+1]['data'].colors[i][2][3]
                mat:set_color(new_r*255, new_g*255, new_b*255, a*255)
            end)
            setCarHeading(carr, vr.list_cars[vr.current_car[0]+1]['data']['angle'][0])
			taskWarpCharIntoCarAsDriver(PLAYER_PED, carr)
			while vr.temp_var.editmodeCar do
				wait(0)
				if not isCharInCar(PLAYER_PED, carr) then
					local xx,xy,xz = getCarCoordinates(carr)
					vr.list_cars[vr.current_car[0]+1]['data']['pos'][0] = xx
					vr.list_cars[vr.current_car[0]+1]['data']['pos'][1] = xy
					vr.list_cars[vr.current_car[0]+1]['data']['pos'][2] = xz
					vr.list_cars[vr.current_car[0]+1]['data']['angle'][0] = getCarHeading(carr)
					deleteCar(carr)
					upd_car:run(vr.current_car[0]+1)
                    vr.temp_var.editmodeCar = false
                    vr.cars[0] = true
				end
			end
		end

        if vr.miss_start then
			for i = 1,#vr.list_actors do
				deleteChar(vr.list_actors[i]['data']['char'])
			end
			for c = 1,#vr.list_cars do
				deleteCar(vr.list_cars[c]['data']['car'])
			end
			for o = 1,#vr.list_objects do
				deleteObject(vr.list_objects[o]['data']['obj'])
			end
			for p = 1,#vr.list_pickups do
				removePickup(vr.list_pickups[p]['data']['pick'])
			end
			for p = 1,#vr.list_particles do
				killFxSystem(vr.list_particles[p]['data']['prtcl'][1])
				deleteObject(vr.list_particles[p]['data']['prtcl'][2])
			end
            for p = 1,#vr.list_explosions do
                if vr.list_explosions[p]['data']['fire'] then
                    removeScriptFire(vr.list_explosions[p]['data']['fire'])
                end
                if vr.list_explosions[p]['data']['explosion'] then
                    deleteObject(vr.list_explosions[p]['data']['explosion'])
                end
			end
            for a = 1,#vr.list_audios do
                if vr.list_audios[a]['data']['obj'] then
                    deleteObject(vr.list_audios[a]['data']['obj'])
                end
			end
			wait(0)
			mp.start_mission(vr.list_targets,vr.list_actors,vr.list_cars,vr.list_objects,vr.list_pickups,vr.list_particles,vr.list_explosions,vr.list_audios)
            vr.miss_start = false
		end
        
        if not mp.mission_work then
            if vr.updateSphere then
                wait(0)
                vr.updateSphere = false
            else
                for i = 1, #vr.list_targets do
                    if vr.list_targets[i]['type'][0] == 0 then
                        drawSphere(vr.list_targets[i]['data']['pos'][0], vr.list_targets[i]['data']['pos'][1], vr.list_targets[i]['data']['pos'][2],vr.list_targets[i]['data']['radius'][0])
                    end
                end
            end
            if isKeyJustPressed(vkeys.VK_R) then
                if vr.mainMenu[0] or vr.settings[0] or vr.targets[0] or vr.actors[0] or vr.cars[0] or carSelector.show or vr.objects[0] or vr.particles[0] or vr.pickups[0] or vr.explosions[0] or vr.audios[0] or vr.player[0] or vr.groupRelations[0] or vr.mission_packs[0] or vr.tools[0] or vr.info[0] then
                    vr.mainMenu[0] = false
                    vr.settings[0] = false
                    vr.targets[0] = false
                    vr.actors[0] = false
                    vr.cars[0] = false
                    carSelector.show = false
                    vr.objects[0] = false
                    vr.particles[0] = false
                    vr.pickups[0] = false
                    vr.explosions[0] = false
                    vr.audios[0] = false
                    vr.player[0] = false
                    vr.groupRelations[0] = false
                    vr.mission_packs[0] = false
                    vr.tools[0] = false
                    vr.info[0] = false
                    wait(0)
                    lockPlayerControl(false)
                    restoreCamera()
                else
                    vr.mainMenu[0] = true
                end
            end
        end
    end

end
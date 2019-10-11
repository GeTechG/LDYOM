script_authors('SKIC','SIZZZ')
script_version('Beta 0.6.1')

require 'libstd.deps' {
	'fyp:mimgui'
 }

vkeys = require 'vkeys'
local inicfg = require 'inicfg'
imgui = require 'mimgui'
ffi = require 'ffi'
new = imgui.new
koder = require 'TextToGTX'
local manager_miss = require 'MissManager'
local mp = require 'mission_player'
model = require 'lib.game.models'
resource = require 'resource'
weapons = require 'lib.game.weapons'
local encoding = require 'encoding'
imgui.Process = false
encoding.default = 'CP1251'
u8 = encoding.UTF8

ID_Actors = {0, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288}

ID_Spec_Actors = {'andre','bbthin','bb','cat','cesar','copgrl1', 'copgrl2','claude','crogrl1', 'crogrl2','dwayne','emmet','forelli','janitor','jethro','jizzy','hern','gangrl2','gungrl1','gungrl2','kendl','maccer','maddogg','mecgrl2','nurgrl2','ogloc','paul','pulaski','rose','ryder','ryder2','ryder3','sindaco','smoke','smokev','suzie','sweet','tbone','tenpen','torino','wuzimu','zero'}

ID_Cars = {400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611}

ID_Weapons = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46}

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

Weather_str = {}

Particle_name = {"blood_heli","boat_prop","camflash","carwashspray","cement","cloudfast","coke_puff","coke_trail","cigarette_smoke","explosion_barrel","explosion_crate","explosion_door","exhale","explosion_fuel_car","explosion_large","explosion_medium","explosion_molotov","explosion_small","explosion_tiny","extinguisher","flame","fire","fire_med","fire_large","flamethrower","fire_bike","fire_car","gunflash","gunsmoke","insects","heli_dust","jetpack","jetthrust","nitro","molotov_flame","overheat_car","overheat_car_electric","prt_boatsplash","prt_cardebris","prt_collisionsmoke","prt_glass","prt_gunshell","prt_sand","prt_sand2","prt_smokeII_3_expand","prt_smoke_huge","prt_spark","prt_spark_2","prt_splash","prt_wake","prt_watersplash","prt_wheeldirt","petrolcan","puke","riot_smoke","spraycan","smoke30lit","smoke30m","smoke50lit","shootlight","smoke_flare","tank_fire","teargas","teargasAD","tree_hit_fir","tree_hit_palm","vent","vent2","water_hydrant","water_ripples","water_speed","water_splash","water_splash_big","water_splsh_sml","water_swim","waterfall_end","water_fnt_tme","water_fountain","wallbust","WS_factorysmoke"}

langt = {}

vr = {
	['mission_data'] = {
		['Name'] = '',
		['Time'] = {0,0},
		['Weather'] = 1,
		['Riot'] = false,
		['Player'] = {
			['Pos'] = {884,-1221,16},
			['Angle'] = 0,
			['ModelId'] = 0,
			['Weapon'] = 1,
			['Weap_ammo'] = 0,
			['Interior_id'] = 0,
			['ped_type'] = true
		}
	},
	['Data'] = {
		['Settings'] = {
			['curr_lang'] = 1
		},
		['Data'] = {
			ID_Spec_Actors = {}
		}
	},
	['Group_relationships'] = {},
 	['timer_time'] = 0,
	['Chalet_font'] = renderCreateFont('Chalet-LondonNineteenSixty',14),
	['SignPainter_font'] = renderCreateFont('SignPainter HouseScript',32),
	['Pricedown_font'] = renderCreateFont('Pricedown Rus',32,0),
	['awsome'] = renderCreateFont('FontAwesome',32),
	['main_menu_window'] = false,
	['actor_change_window'] = false,
	['actor_window'] = false,
	['actor_list_window'] = false,
	['actor_anims_change_window'] = false,
	['actor_anims_list_window'] = false,
	['actor_anim_window'] = false,
	['target_window'] = false,
	['targets_change_window'] = false,
	['targets_list_window'] = false,
	['target_list_dialog_window'] = false,
	['target_dialog_window'] = false,
	['cars_change_window'] = false,
	['cars_window'] = false,
	['cars_list_window'] = false,
	['car_anims_change_window'] = false,
	['car_anims_list_window'] = false,
	['car_anim_window'] = false,
	['objects_window'] = false,
	['objects_change_window'] = false,
	['objects_list_window'] = false,
	['object_anims_change_window'] = false,
	['object_anims_list_window'] = false,
	['object_anim_window'] = false,
	['particles_window'] = false,
	['particles_change_window'] = false,
	['particles_list_window'] = false,
	['pickup_window'] = false,
	['pickup_change_window'] = false,
	['pickup_list_window'] = false,
	['explosion_window'] = false,
	['explosion_change_window'] = false,
	['explosion_list_window'] = false,
	['pack_mission_change_window'] = false,
	['pack_mission_list_window'] = false,
	['missions_change_window'] = false,
	['missions_list_window'] = false,
	['missions_settings_window'] = false,
	['tools_window'] = false,
	['info_window'] = false,
	['settings_window'] = false,
	['Group_relationships_list_window'] = false,
	['Group_relationships_window'] = false,
	['actor_list_toggle'] = 1,
	['actor_anims_list_toggle'] = 1,
	['car_anims_list_toggle'] = 1,
	['targets_list_toggle'] = 1,
	['targets_dialog_toggle'] = 1,
	['car_list_toggle'] = 1,
	['object_list_toggle'] = 1,
	['object_anim_list_toggle'] = 1,
	['targets_list_toggle'] = 1,
	['mission_pack_list_toggle'] = 1,
	['missions_list_toggle'] = 1,
	['list_actors'] = {},
	['list_targets'] = {},
	['list_cars'] = {},
	['list_objects'] = {},
	['list_particle'] = {},
	['list_pickup'] = {},
	['list_explosion'] = {},
	['list_mission_pack'] = {},
	['list_missions'] = {},
	['list_name_actors'] = {},
	['list_name_targets'] = {},
	['list_name_cars'] = {},
	['list_name_objects'] = {},
	['list_name_pickup'] = {},
	['list_name_particle'] = {},
	['list_name_explosion'] = {},
	['list_name_mission_pack'] = {},
	['list_name_missions'] = {},
	['vector3_window'] = false,
	['previous_window'] = '',
	['editmode_target'] = false,
	['editmode_camera'] = false,
	['editmode_actor'] = false,
	['editmode_actor_add_point'] = false,
	['editmode_objects'] = false,
	['editmode_objects_anim'] = false,
	['editmode_pickup'] = false,
	['editmode_particle'] = false,
	['editmode_explosion'] = false,
	['editmode_timemiss'] = false,
	['editmode_timetarg'] = false,
	['editmode_player'] = false,
	['editmode_teleport_player'] = false,
	['editmode_preview_player_anim'] = false,
	['editmode_car'] = false,
	['editmode_input'] = false,
	['current_actor'] = 1,
	['current_target'] = 1,
	['current_car'] = 1,
	['current_object'] = 1,
	['current_particle'] = 1,
	['current_pickup'] = 1,
	['current_explosion'] = 1,
	['current_mission_pack'] = 1,
	['current_missions'] = 1,
	['current_dialog_target'] = 1,
	['current_group_relationships'] = 1,
	['miss_start'] = false,
	['current_item'] = 1,
	['selected_item'] = -1,
	['buffer_target'] = -1,
	['buffer_input'] = new.char[256+1](),
	['Fast_data'] = {
		['CurMiss'] = 1,
		['CurPack'] = 1
	}
}

local LanguageArr = {}
local LanguageList = {}

function apply_custom_style()
	local style = imgui.GetStyle()
	local colors = style.Colors
	local clr = imgui.Col
	local ImVec4 = imgui.ImVec4
	style.WindowRounding = 0
	colors[imgui.Col.Border]               = ImVec4(0,0,0,0)
	colors[clr.WindowBg]               = ImVec4(0.26, 0.14, 0.60,0.6)
	colors[clr.FrameBg]                = ImVec4(0,0,0, 0.7)
end

imgui.OnInitialize(function()
	apply_custom_style()
	imgui.GetIO().Fonts:Clear()
	local builder = imgui.ImFontGlyphRangesBuilder()
	builder:AddRanges(imgui.GetIO().Fonts:GetGlyphRangesCyrillic())
    builder:AddText([[‚„…†‡€‰‹‘’“”•–—™›№]])
    local range = imgui.ImVector_ImWchar()
    builder:BuildRanges(range)
    imgui.GetIO().Fonts:AddFontFromMemoryCompressedBase85TTF(resource.font.Chalet, 18.0, nil, range[0].Data)
end)

imgui.OnFrame(function() return vr.editmode_input end, 
function()
	
	imgui.SetNextWindowSize(imgui.ImVec2(640,60),imgui.Cond.Always)
	imgui.SetNextWindowPos(imgui.ImVec2(10,10),imgui.Cond.Always)
	imgui.Begin('test',nil,imgui.WindowFlags.NoDecoration)
	imgui.PushItemWidth(-1)
	imgui.InputText('',vr.buffer_input,ffi.sizeof(vr.buffer_input))
	imgui.Text(langt['pressFtoExit'])
	if not openMenu then
		vr.editmode_input = false
	end
end)

function drawMenu1(title_name,arr_menu)
	local columns = {}
	setGxtEntry('TITLEM',koder(u8:decode(title_name)))
	local curr = 1;
	for i = 1,math.floor(#arr_menu/12)+1 do
		local list = {}
		for r = 1,10 do
			if arr_menu[curr] ~= nil then
				list[r] = koder(u8:decode(arr_menu[curr]))
			end
			curr = curr + 1
		end
		columns[i] = list
	end
	local var = createMenu('TITLEM',320-(50*#columns),240-10*12,120,#columns,true,true,0)
	local curr_column = 1;
	setGxtEntry('MENU12',koder('Далее...'))
	setGxtEntry('MENU1',koder('Назад...'))
	activateMenuItem(var,0,false)
	activateMenuItem(var,11,false)
	if #columns[curr_column] > 1 then
		activateMenuItem(var,11,true)
	end
	for g = 1,10 do
		if columns[curr_column][g] == nil then
			setGxtEntry('MENU'..tostring(g+1),getGxtText('DUMMY'))
		else
			setGxtEntry('MENU'..tostring(g+1),columns[curr_column][g])
		end
	end
	setMenuColumn(var,0,'DUMMY','MENU1','MENU2','MENU3','MENU4','MENU5','MENU6','MENU7','MENU8','MENU9','MENU10','MENU11','MENU12')
	open_menu = true
	while open_menu do
		wait(0)
		if getMenuItemAccepted(var) == 11 then
			curr_column = curr_column + 1
			for g = 1,10 do
				if columns[curr_column][g] == nil then
					setGxtEntry('MENU'..tostring(g+1),getGxtText('DUMMY'))
					activateMenuItem(var,g,false)
				else
					setGxtEntry('MENU'..tostring(g+1),columns[curr_column][g])
					activateMenuItem(var,g,true)
				end
			end
			if curr_column ~= #columns then
				activateMenuItem(var,11,true)
			else
				activateMenuItem(var,11,false)
			end
			if curr_column ~= 1 then
				activateMenuItem(var,0,true)
			else
				activateMenuItem(var,0,false)
			end
		end
		if getMenuItemAccepted(var) == 0 then
			curr_column = curr_column - 1
			for g = 1,10 do
				if columns[curr_column][g] == nil then
					setGxtEntry('MENU'..tostring(g+1),getGxtText('DUMMY'))
					activateMenuItem(var,g,false)
				else
					setGxtEntry('MENU'..tostring(g+1),columns[curr_column][g])
					activateMenuItem(var,g,true)
				end
			end
			if curr_column ~= #columns then
				activateMenuItem(var,11,true)
			else
				activateMenuItem(var,11,false)
			end
			if curr_column ~= 1 then
				activateMenuItem(var,0,true)
			else
				activateMenuItem(var,0,false)
			end
		end
		if wasKeyReleased(0x55) then
			print('f')
			open_menu = false
		end
	end
	deleteMenu(var)
end

function timer()
	while true do
		wait(0)
		if vr.timer_time > 0 then
			wait(vr.timer_time)
		end
		vr.timer_time = 0
	end
end

function add_items(arr,arr_id,pos0)
	for n,i in ipairs(arr_id) do
		arr[pos0+n-1] = i
	end
end

function drawMenu(title_name,arr_menu,width,height)
	renderDrawBox(10,10,width,50,rgba_to_int(67, 35, 153, 150))
	renderFontDrawText(vr.SignPainter_font,u8:decode(title_name),(10+width/2)-renderGetFontDrawTextLength(vr.SignPainter_font,u8:decode(title_name))/2, 15,rgba_to_int(255,255,255,255))
	renderDrawBox(10,60,width,height,rgba_to_int(0, 0, 0, 150))
	local render_size = #arr_menu
	if render_size > 10 then
		render_size = 10
	end
	for i = 1,render_size do
		local g = i
		if vr.current_item > 10 then
			g = vr.current_item-10+i
			if arr_menu[g][1] == 'button' then
				if i == 10 then
					renderDrawBox(10,25+(renderGetFontDrawHeight(vr.Chalet_font)+10)*(i),width,renderGetFontDrawHeight(vr.Chalet_font)+(i),rgba_to_int(67, 35, 153, 255))
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(0,0,0,255))
					if wasKeyPressed(0x20) or wasKeyPressed(0x0D) then
						setAudioStreamVolume(resource.sounds.enter,10)
						setAudioStreamState(resource.sounds.enter,1)
						vr.selected_item = arr_menu[g][3]
						return true
					end
				else
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(255,255,255,255))
				end
			elseif arr_menu[g][1] == 'int_slider' then
				if i == 10 then
					renderDrawBox(10,25+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,width,renderGetFontDrawHeight(vr.Chalet_font)+10,rgba_to_int(67, 35, 153, 255))
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(0,0,0,255))
					renderDrawTexture(resource.icons.arrow_right,30+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,180,rgba_to_int(0,0,0,255))
					renderFontDrawText(vr.Chalet_font,arr_menu[g][3][arr_menu[g][4]],40+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(0,0,0,255))
					renderDrawTexture(resource.icons.arrow_right,45+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2]))+renderGetFontDrawTextLength(vr.Chalet_font,arr_menu[g][3][arr_menu[g][4]]),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,0,rgba_to_int(0,0,0,255))
					if isKeyDown(0x41) and vr.timer_time == 0 then
						arr_menu[g][3][arr_menu[g][4]] = arr_menu[g][3][arr_menu[g][4]] - (arr_menu[g][7] or 1)
						if arr_menu[g][5] and arr_menu[g][3][arr_menu[g][4]] < arr_menu[g][5] then
							arr_menu[g][3][arr_menu[g][4]] = arr_menu[g][5]
						end
						vr.timer_time = 60
						vr.selected_item = arr_menu[g][8]
						return true
					end
					if isKeyDown(0x44) and vr.timer_time == 0 then
						arr_menu[g][3][arr_menu[g][4]] = arr_menu[g][3][arr_menu[g][4]] + (arr_menu[g][7] or 1)
						if arr_menu[g][6] and arr_menu[g][3][arr_menu[g][4]] > arr_menu[g][6] then
							arr_menu[g][3][arr_menu[g][4]] = arr_menu[g][6]
						end
						vr.timer_time = 60
						vr.selected_item = arr_menu[g][8]
						return true
					end
				else
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(255,255,255,255))
					renderDrawTexture(resource.icons.arrow_right,30+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,180,rgba_to_int(255,255,255,255))
					renderFontDrawText(vr.Chalet_font,arr_menu[g][3][arr_menu[g][4]],40+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(255,255,255,255))
					renderDrawTexture(resource.icons.arrow_right,45+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2]))+renderGetFontDrawTextLength(vr.Chalet_font,arr_menu[g][3][arr_menu[g][4]]),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,0,rgba_to_int(255,255,255,255))
				end
			elseif arr_menu[g][1] == 'checkbox' then
				if i == 10 then
					renderDrawBox(10,25+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,width,renderGetFontDrawHeight(vr.Chalet_font)+10,rgba_to_int(67, 35, 153, 255))
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(0,0,0,255))
					if arr_menu[g][3][arr_menu[g][4]] then
						renderDrawTexture(resource.icons.check_true,width-23,25+8+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,21,16,0,rgba_to_int(0,255,0,255))
					else
						renderDrawTexture(resource.icons.check_false,width-18,25+8+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,16,16,0,rgba_to_int(255,0,0,255))
					end
					if wasKeyPressed(0x20) or wasKeyPressed(0x0D) then
						setAudioStreamVolume(resource.sounds.enter,10)
						setAudioStreamState(resource.sounds.enter,1)
						arr_menu[g][3][arr_menu[g][4]] = not arr_menu[g][3][arr_menu[g][4]]
						vr.selected_item = arr_menu[g][5]
						return true
					end
				else
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(255,255,255,255))
					if arr_menu[g][3][arr_menu[g][4]] then
						renderDrawTexture(resource.icons.check_true,width-23,25+8+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,21,16,0,rgba_to_int(0,255,0,255))
					else
						renderDrawTexture(resource.icons.check_false,width-18,25+8+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,16,16,0,rgba_to_int(255,0,0,255))
					end
				end
			elseif arr_menu[g][1] == 'arr_slider' then
				if i == 10 then
					renderDrawBox(10,25+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,width,renderGetFontDrawHeight(vr.Chalet_font)+10,rgba_to_int(67, 35, 153, 255))
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(0,0,0,255))
					if #arr_menu[g][5] > 0 then
						renderDrawTexture(resource.icons.arrow_right,30+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,180,rgba_to_int(0,0,0,255))
						renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][5][arr_menu[g][3][arr_menu[g][4]]]),40+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(0,0,0,255))
						renderDrawTexture(resource.icons.arrow_right,45+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2]))+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][5][arr_menu[g][3][arr_menu[g][4]]])),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,0,rgba_to_int(0,0,0,255))
					end
					if isKeyDown(0x41) and vr.timer_time == 0 then
						arr_menu[g][3][arr_menu[g][4]] = arr_menu[g][3][arr_menu[g][4]] - 1
						if arr_menu[g][3][arr_menu[g][4]] < 1 then
							arr_menu[g][3][arr_menu[g][4]] = 1
						end
						vr.timer_time = 60
						vr.selected_item = arr_menu[g][6]
						return true
					end
					if isKeyDown(0x44) and vr.timer_time == 0 then
						arr_menu[g][3][arr_menu[g][4]] = arr_menu[g][3][arr_menu[g][4]] + 1
						if arr_menu[g][3][arr_menu[g][4]] > #arr_menu[g][5] then
							arr_menu[g][3][arr_menu[g][4]] = #arr_menu[g][5]
						end
						vr.timer_time = 100
						vr.selected_item = arr_menu[g][6]
						return true
					end
				else
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(255,255,255,255))
					if #arr_menu[g][5] > 0 then
						renderDrawTexture(resource.icons.arrow_right,30+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,180,rgba_to_int(255,255,255,255))
						renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][5][arr_menu[g][3][arr_menu[g][4]]]),40+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(255,255,255,255))
						renderDrawTexture(resource.icons.arrow_right,45+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2]))+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][5][arr_menu[g][3][arr_menu[g][4]]])),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,0,rgba_to_int(255,255,255,255))
					end
				end
			end
		else
			if arr_menu[g][1] == 'button' then
				if i == vr.current_item then
					renderDrawBox(10,25+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,width,renderGetFontDrawHeight(vr.Chalet_font)+10,rgba_to_int(67, 35, 153, 255))
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(0,0,0,255))
					if wasKeyPressed(0x20) or wasKeyPressed(0x0D) then
						setAudioStreamVolume(resource.sounds.enter,10)
						setAudioStreamState(resource.sounds.enter,1)
						vr.selected_item = arr_menu[g][3]
						return true
					end
				else
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(255,255,255,255))
				end
			elseif arr_menu[g][1] == 'int_slider' then
				if i == vr.current_item then
					renderDrawBox(10,25+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,width,renderGetFontDrawHeight(vr.Chalet_font)+10,rgba_to_int(67, 35, 153, 255))
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(0,0,0,255))
					renderDrawTexture(resource.icons.arrow_right,30+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,180,rgba_to_int(0,0,0,255))
					renderFontDrawText(vr.Chalet_font,arr_menu[g][3][arr_menu[g][4]],40+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(0,0,0,255))
					renderDrawTexture(resource.icons.arrow_right,45+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2]))+renderGetFontDrawTextLength(vr.Chalet_font,arr_menu[g][3][arr_menu[g][4]]),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,0,rgba_to_int(0,0,0,255))
					if isKeyDown(0x41) and vr.timer_time == 0 then
						arr_menu[g][3][arr_menu[g][4]] = arr_menu[g][3][arr_menu[g][4]] - (arr_menu[g][7] or 1)
						if arr_menu[g][5] and arr_menu[g][3][arr_menu[g][4]] < arr_menu[g][5] then
							arr_menu[g][3][arr_menu[g][4]] = arr_menu[g][5]
						end
						vr.timer_time = 60
						vr.selected_item = arr_menu[g][8]
						return true
					end
					if isKeyDown(0x44) and vr.timer_time == 0 then
						arr_menu[g][3][arr_menu[g][4]] = arr_menu[g][3][arr_menu[g][4]] + (arr_menu[g][7] or 1)
						if arr_menu[g][6] and arr_menu[g][3][arr_menu[g][4]] > arr_menu[g][6] then
							arr_menu[g][3][arr_menu[g][4]] = arr_menu[g][6]
						end
						vr.timer_time = 60
						vr.selected_item = arr_menu[g][8]
						return true
					end
				else
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(255,255,255,255))
					renderDrawTexture(resource.icons.arrow_right,30+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,180,rgba_to_int(255,255,255,255))
					renderFontDrawText(vr.Chalet_font,arr_menu[g][3][arr_menu[g][4]],40+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(255,255,255,255))
					renderDrawTexture(resource.icons.arrow_right,45+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2]))+renderGetFontDrawTextLength(vr.Chalet_font,arr_menu[g][3][arr_menu[g][4]]),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,0,rgba_to_int(255,255,255,255))
				end
			elseif arr_menu[g][1] == 'checkbox' then
				if i == vr.current_item then
					renderDrawBox(10,25+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,width,renderGetFontDrawHeight(vr.Chalet_font)+10,rgba_to_int(67, 35, 153, 255))
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(0,0,0,255))
					if arr_menu[g][3][arr_menu[g][4]] then
						renderDrawTexture(resource.icons.check_true,width-23,25+8+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,21,16,0,rgba_to_int(0,255,0,255))
					else
						renderDrawTexture(resource.icons.check_false,width-18,25+8+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,16,16,0,rgba_to_int(255,0,0,255))
					end
					if wasKeyPressed(0x20) or wasKeyPressed(0x0D) then
						setAudioStreamVolume(resource.sounds.enter,10)
						setAudioStreamState(resource.sounds.enter,1)
						arr_menu[g][3][arr_menu[g][4]] = not arr_menu[g][3][arr_menu[g][4]]
						vr.selected_item = arr_menu[g][5]
						return true
					end
				else
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(255,255,255,255))
					if arr_menu[g][3][arr_menu[g][4]] then
						renderDrawTexture(resource.icons.check_true,width-23,25+8+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,21,16,0,rgba_to_int(0,255,0,255))
					else
						renderDrawTexture(resource.icons.check_false,width-18,25+8+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,16,16,0,rgba_to_int(255,0,0,255))
					end
				end
			elseif arr_menu[g][1] == 'arr_slider' then
				if i == vr.current_item then
					renderDrawBox(10,25+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,width,renderGetFontDrawHeight(vr.Chalet_font)+10,rgba_to_int(67, 35, 153, 255))
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(0,0,0,255))
					if #arr_menu[g][5] > 0 then
						renderDrawTexture(resource.icons.arrow_right,30+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,180,rgba_to_int(0,0,0,255))
						renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][5][arr_menu[g][3][arr_menu[g][4]]]),40+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(0,0,0,255))
						renderDrawTexture(resource.icons.arrow_right,45+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2]))+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][5][arr_menu[g][3][arr_menu[g][4]]])),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,0,rgba_to_int(0,0,0,255))
					end
					if isKeyDown(0x41) and vr.timer_time == 0 then
						arr_menu[g][3][arr_menu[g][4]] = arr_menu[g][3][arr_menu[g][4]] - 1
						if arr_menu[g][3][arr_menu[g][4]] < 1 then
							arr_menu[g][3][arr_menu[g][4]] = 1
						end
						vr.timer_time = 100
						vr.selected_item = arr_menu[g][6]
						return true
					end
					if isKeyDown(0x44) and vr.timer_time == 0 then
						arr_menu[g][3][arr_menu[g][4]] = arr_menu[g][3][arr_menu[g][4]] + 1
						if arr_menu[g][3][arr_menu[g][4]] > #arr_menu[g][5] then
							arr_menu[g][3][arr_menu[g][4]] = #arr_menu[g][5]
						end
						vr.timer_time = 100
						vr.selected_item = arr_menu[g][6]
						return true
					end
				else
					renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][2]),20,25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(255,255,255,255))
					if #arr_menu[g][5] > 0 then
						renderDrawTexture(resource.icons.arrow_right,30+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,180,rgba_to_int(255,255,255,255))
						renderFontDrawText(vr.Chalet_font,u8:decode(arr_menu[g][5][arr_menu[g][3][arr_menu[g][4]]]),40+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2])),25+5+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,rgba_to_int(255,255,255,255))
						renderDrawTexture(resource.icons.arrow_right,45+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][2]))+renderGetFontDrawTextLength(vr.Chalet_font,u8:decode(arr_menu[g][5][arr_menu[g][3][arr_menu[g][4]]])),25+12+(renderGetFontDrawHeight(vr.Chalet_font)+10)*i,4,8,0,rgba_to_int(255,255,255,255))
					end
				end
			end
		end
	end

	if #arr_menu > 10 then
		renderDrawBox(10,10+50+height,width,35,rgba_to_int(0, 0, 0, 200))
		renderDrawTexture(resource.icons.scroll,width/2 - 10,17+50+height,20,20,0,rgba_to_int(255,255,255,255))
	end
	if wasKeyPressed(0x57) then
		setAudioStreamState(resource.sounds.scroll,1)
		if vr.current_item ~= 1 then
			vr.current_item = vr.current_item - 1
		else
			vr.current_item = #arr_menu
		end
	end
	if wasKeyPressed(0x53) then
		setAudioStreamState(resource.sounds.scroll,1)
		if vr.current_item ~= #arr_menu then
			vr.current_item = vr.current_item + 1
		else
			vr.current_item = 1
		end
	end
end

function rgba_to_int(r,g,b,a)
	local argb = b  -- b
	argb = bit.bor(argb, bit.lshift(g, 8))  -- g
	argb = bit.bor(argb, bit.lshift(r, 16)) -- r
	argb = bit.bor(argb, bit.lshift(a, 24)) -- a
	return argb
end


function main_menu()
	if vr.selected_item == 1 then
		vr.current_item = 1
		vr.main_menu_window = false
		vr.targets_change_window = true
	elseif vr.selected_item == 2 then
		vr.current_item = 1
		vr.main_menu_window = false
		vr.actor_change_window = true
	elseif vr.selected_item == 3 then
		vr.current_item = 1
		vr.main_menu_window = false
		vr.Group_relationships_list_window = true
	elseif vr.selected_item == 4 then
		vr.current_item = 1
		vr.main_menu_window = false
		vr.cars_change_window = true
	elseif vr.selected_item == 5 then
		vr.current_item = 1
		vr.main_menu_window = false
		vr.objects_change_window = true
	elseif vr.selected_item == 6 then
		vr.current_item = 1
		vr.main_menu_window = false
		vr.particles_change_window = true
	elseif vr.selected_item == 7 then
		vr.current_item = 1
		vr.main_menu_window = false
		vr.pickup_change_window = true
	elseif vr.selected_item == 8 then
		vr.current_item = 1
		vr.main_menu_window = false
		vr.explosion_change_window = true
	elseif vr.selected_item == 9 then
		vr.editmode_player = true
		printHelpForever('HPLA')
		lockPlayerControl(false)
	elseif vr.selected_item == 10 then
		vr.current_item = 1
		vr.main_menu_window = false
		vr.pack_mission_change_window = true
	elseif vr.selected_item == 11 then
		vr.current_item = 1
		vr.main_menu_window = false
		vr.missions_change_window = true
	elseif vr.selected_item == 12 then
		vr.miss_start = true
	elseif vr.selected_item == 13 then
		vr.current_item = 1
		vr.main_menu_window = false
		vr.tools_window = true
	elseif vr.selected_item == 14 then
		vr.current_item = 1
		vr.main_menu_window = false
		vr.info_window = true
	elseif vr.selected_item == 15 then
		vr.current_item = 1
		vr.main_menu_window = false
		vr.settings_window = true
	end
end

function actor_change()
	
	if vr.selected_item == 1 then
		local xx,xy,xz = getCharCoordinates(PLAYER_PED)
		xz = getGroundZFor3dCoord(xx,xy,xz)
		local angle = getCharHeading(PLAYER_PED)
		vr.list_actors[#vr.list_actors+1] = {
			['Name'] = langt['actor']..' #' .. tostring(#vr.list_actors+1),
			['Type'] = -1,
			['Actor_Data'] = {
				['Pos'] = {xx,xy,xz},
				['Angle'] = math.floor(angle),
				['ModelId'] = 1,
				['Weapon'] = 1,
				['Ammo'] = 0,
				['StartC'] = 1,
				['EndC'] = 1,
				['Anims'] = {},
				['Anim_id'] = 0,
				['Should_not_die'] = false,
				['Health'] = 100,
				['Group'] = 2,
				['Model_type'] = 1,
				['Slot_model'] = 1
			}
		}
		upd_actor:run(#vr.list_actors)
		vr.list_name_actors[#vr.list_actors] = vr.list_actors[#vr.list_actors]['Name']
		vr.current_actor = #vr.list_actors
		vr.current_item = 1
		vr.actor_change_window = false
		vr.actor_window = true
	elseif vr.selected_item == 2 then
		vr.actor_list_toggle = 1
		vr.current_item = 1
		vr.actor_change_window = false
		vr.actor_list_window = true
	elseif vr.selected_item == 3 then
		vr.actor_list_toggle = 2
		vr.current_item = 1
		vr.actor_change_window = false
		vr.actor_list_window = true
	end
end

function actor_list()
	if vr.actor_list_toggle == 1 then
		vr.current_actor = vr.selected_item
		vr.current_item = 1
		vr.actor_list_window = false
		vr.actor_window = true
	elseif vr.actor_list_toggle == 2 then
		deleteChar(vr.list_actors[vr.selected_item]['Actor_Data']['Char'])
		vr.list_actors = DelCellArr(vr.list_actors,vr.selected_item)
		vr.list_name_actors = DelCellArr(vr.list_name_actors,vr.selected_item)
		vr.current_item = 1
		vr.actor_list_window = false
		vr.actor_change_window = true
	end
end

function actor()
	if vr.selected_item == 1 then
		local names = decodeJson(langt['vector3_arr'])
		vr.vector3_items = {
			{'int_slider',names[1],vr.list_actors[vr.current_actor]['Actor_Data']['Pos'],1,_,_,0.01,1},
			{'int_slider',names[2],vr.list_actors[vr.current_actor]['Actor_Data']['Pos'],2,_,_,0.01,2},
			{'int_slider',names[3],vr.list_actors[vr.current_actor]['Actor_Data']['Pos'],3,_,_,0.01,3},
			{'button',names[4],4}
		}
		vr.vector3_items[0] = decodeJson(langt['actor_arr'])[1]
		vr.previous_window = 'actor_window'
		vr.current_item = 1
		vr.vector3_window = true
		vr.actor_window = false
	elseif vr.selected_item == 2 then
		setCharHeading(vr.list_actors[vr.current_actor]['Actor_Data']['Char'], vr.list_actors[vr.current_actor]['Actor_Data']['Angle'])
	elseif vr.selected_item == 3 then
		vr.list_actors[vr.current_actor]['Actor_Data']['ModelId'] = 1
		upd_actor:run(vr.current_actor)
	elseif vr.selected_item == 4 or vr.selected_item == 5 then
		upd_actor:run(vr.current_actor)
	elseif vr.selected_item == 6 then
		requestModel(getWeapontypeModel(ID_Weapons[vr.list_actors[vr.current_actor]['Actor_Data']['Weapon']]))
		while not hasModelLoaded(getWeapontypeModel(ID_Weapons[vr.list_actors[vr.current_actor]['Actor_Data']['Weapon']])) do
			wait(0)
		end
		removeAllCharWeapons(vr.list_actors[vr.current_actor]['Actor_Data']['Char'])
		giveWeaponToChar(vr.list_actors[vr.current_actor]['Actor_Data']['Char'], ID_Weapons[vr.list_actors[vr.current_actor]['Actor_Data']['Weapon']], 1)
		setCurrentCharWeapon(vr.list_actors[vr.current_actor]['Actor_Data']['Char'],1)
		markModelAsNoLongerNeeded(getWeapontypeModel(ID_Weapons[vr.list_actors[vr.current_actor]['Actor_Data']['Weapon']]))
	elseif vr.selected_item == 11 then
		vr.editmode_actor = true
		deleteChar(vr.list_actors[vr.current_actor]['Actor_Data']['Char'])
	elseif vr.selected_item == 12 then
		vr.current_item = 1
		vr.actor_anims_change_window = true
		vr.actor_window = false 
	end
end

function actor_anims_change()
	if vr.selected_item == 1 then
		vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][#vr.list_actors[vr.current_actor]['Actor_Data']['Anims']+1] = {
			['Type'] = 1,
		  }
		vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id'] = #vr.list_actors[vr.current_actor]['Actor_Data']['Anims']
		vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Anim'] = 1
		vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Pack_anim'] = 1
		vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Loop'] = false
		vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Time'] = 1.0
		vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Condition'] = 1
		vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Unbreakable'] = false
		vr.current_item = 1
		vr.actor_anims_change_window = false
		vr.actor_anim_window = true
	elseif vr.selected_item == 2 then
		vr.actor_anims_list_toggle = 1
		vr.current_item = 1
		vr.actor_anims_change_window = false
		vr.actor_anims_list_window = true
	elseif vr.selected_item == 3 then
		vr.actor_anims_list_toggle = 2
		vr.current_item = 1
		vr.actor_anims_change_window = false
		vr.actor_anims_list_window = true
	end
end

function actor_anims_list()
	if vr.actor_anims_list_toggle == 1 then
		vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id'] = vr.selected_item
		vr.current_item = 1
		vr.actor_anims_list_window = false
		vr.actor_anim_window = true
	elseif vr.actor_anims_list_toggle == 2 then
		vr.list_actors[vr.current_actor]['Actor_Data']['Anims'] = DelCellArr(vr.list_actors[vr.current_actor]['Actor_Data']['Anims'],vr.selected_item)
		vr.current_item = 1
		vr.actor_anims_list_window = false
		vr.actor_anims_change_window = true
	end
end

function actor_anim()
	if vr.selected_item == 1 then
		if vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Type'] == 1 then
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Anim'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Pack_anim'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Loop'] = false
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Time'] = 1.0
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Unbreakable'] = false
		elseif vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Type'] == 2 then
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Path'] = {}
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Type_move'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Type_route'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Cur_point'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Vis_point'] = false
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Condition'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['wait_end'] = false
		elseif vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Type'] == 3 then
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Path'] = {}
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Car'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Speed'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Cur_point'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Vis_point'] = false
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Condition'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['wait_end'] = false
		elseif vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Type'] == 4 then
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Condition'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['wait_end'] = false
		elseif vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Type'] == 5 then
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Condition'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Car_a'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Car_t'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Speed'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['trafficFlag'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Vehicle_mission'] = 1
		elseif vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Type'] == 6 then
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Condition'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['place_in_car'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Car'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['speed_walk'] = 1
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['wait_end'] = false
		end
	elseif vr.selected_item == 2 then
		vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Target'] = 1
	elseif vr.selected_item == 5 then
		vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Anim'] = 1
	elseif vr.selected_item == 10 then
		upd_anim_actor:run(vr.current_actor,vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id'])
	elseif vr.selected_item == 4 then
		upd_actor:run(vr.current_actor)
	elseif vr.selected_item == 13 then
		if vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Vis_point'] then
				lua_thread.create(function(anim,spos)
					local font = renderCreateFont('Verdana',12)
					while anim['Vis_point'] do
						wait(0)
						anim['Path'][0] = {spos[1],spos[2],spos[3]}
						print(#anim['Path'])
						for i = 1, #anim['Path'] do
							local x1,y1,z1 = anim['Path'][i][1],anim['Path'][i][2],anim['Path'][i][3]
							local x2,y2,z2 = anim['Path'][i-1][1],anim['Path'][i-1][2],anim['Path'][i-1][3]
							local font_pos_x1,font_pos_y1 = convert3DCoordsToScreen(x1,y1,z1+0.1)
							local font_pos_x2,font_pos_y2 = convert3DCoordsToScreen(x2,y2,z2+0.1)
							DrawLineBy3dCoords(x1,y1,z1,x2,y2,z2,2,0xFFFFFFFF)
							if isPointOnScreen(x1, y1, z1,1) then
								renderFontDrawText(font,tostring(x1)..' '..tostring(y1) ..' '..tostring(z1),font_pos_x1,font_pos_y1,0xFFFFFFFF)
							end
							if isPointOnScreen(x2, y2, z2,1) then
								renderFontDrawText(font,tostring(x2)..' '..tostring(y2) ..' '..tostring(z2),font_pos_x2,font_pos_y2,0xFFFFFFFF)
							end
						end
					end
					renderReleaseFont(font)
				end,
				vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],vr.list_actors[vr.current_actor]['Actor_Data']['Pos'])
		end
	elseif vr.selected_item == 14 then
		printHelpForever('AAAP')
		lockPlayerControl(false)
		vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Path'] = {}
		vr.actor_anim_window = false
		vr.id_actor_anim = vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id'];
		vr.editmode_actor_add_point = true;
	elseif vr.selected_item == 15 then
		local xx,xy,xz = vr.list_actors[vr.current_actor]['Actor_Data']['Pos'][1],vr.list_actors[vr.current_actor]['Actor_Data']['Pos'][2],vr.list_actors[vr.current_actor]['Actor_Data']['Pos'][3]
		local dec = loadCharDecisionMaker(0)
		setCharProofs(vr.list_actors[vr.current_actor]['Actor_Data']['Char'],true,true,true,true,true)
		setCharDecisionMaker(vr.list_actors[vr.current_actor]['Actor_Data']['Char'],dec)
		setCharCoordinates(vr.list_actors[vr.current_actor]['Actor_Data']['Char'],xx,xy,xz)
		setCharCollision(vr.list_actors[vr.current_actor]['Actor_Data']['Char'],true)
		lua_thread.create(function(path)
				local type_walk = 4;
				if path['Type_move'] == 3 then type_walk = 6
				elseif path['Type_move'] == 4 then type_walk = 7 end
				taskToggleDuck(vr.list_actors[vr.current_actor]['Actor_Data']['Char'], ternar(path['Type_move'] == 1,true,false))
				repeat
					for i = 1,#path['Path'] do
						local x1,y1,z1 = path['Path'][i][1],path['Path'][i][2],path['Path'][i][3]
						taskGoStraightToCoord(vr.list_actors[vr.current_actor]['Actor_Data']['Char'],x1,y1,z1,type_walk,-1)
						px,py,pz = getCharCoordinates(vr.list_actors[vr.current_actor]['Actor_Data']['Char'])
						while getDistanceBetweenCoords3d(px,py,pz,x1,y1,z1) > 1 do
							wait(0)
							px,py,pz = getCharCoordinates(vr.list_actors[vr.current_actor]['Actor_Data']['Char'])
						end
					end
					wait(0)
				until path['Type_route'] == 1
				taskToggleDuck(vr.list_actors[vr.current_actor]['Actor_Data']['Char'],false)
		end,vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']])
	elseif vr.selected_item == 17 then
		if vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Vis_point'] then
			lua_thread.create(function(anim,spos)
				local font = renderCreateFont('Verdana',12)
				while anim['Vis_point'] do
					wait(0)
					anim['Path'][0] = {spos[1],spos[2],spos[3]}
					print(#anim['Path'])
					for i = 1, #anim['Path'] do
						local x1,y1,z1 = anim['Path'][i][1],anim['Path'][i][2],anim['Path'][i][3]
						local x2,y2,z2 = anim['Path'][i-1][1],anim['Path'][i-1][2],anim['Path'][i-1][3]
						local font_pos_x1,font_pos_y1 = convert3DCoordsToScreen(x1,y1,z1+0.1)
						local font_pos_x2,font_pos_y2 = convert3DCoordsToScreen(x2,y2,z2+0.1)
						DrawLineBy3dCoords(x1,y1,z1,x2,y2,z2,2,0xFFFFFFFF)
						if isPointOnScreen(x1, y1, z1,1) then
							renderFontDrawText(font,tostring(x1)..' '..tostring(y1) ..' '..tostring(z1),font_pos_x1,font_pos_y1,0xFFFFFFFF)
						end
						if isPointOnScreen(x2, y2, z2,1) then
							renderFontDrawText(font,tostring(x2)..' '..tostring(y2) ..' '..tostring(z2),font_pos_x2,font_pos_y2,0xFFFFFFFF)
						end
					end
				end
				renderReleaseFont(font)
			end,
			vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],vr.list_cars[vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Car']]['Car_Data']['Pos'])
		end
	elseif vr.selected_item == 20 then
		printHelpForever('AAAP')
		lockPlayerControl(false)
		vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Path'] = {}
		vr.actor_anim_window = false
		vr.id_actor_anim = vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id'];
		vr.editmode_actor_add_point = true;
	end
end

function targets_change()
	
	local xx,xy,xz = getCharCoordinates(PLAYER_PED)
	if vr.selected_item == 1 then
		vr.list_targets[#vr.list_targets+1] = {
			['Name'] = langt['target']..' #' .. tostring(#vr.list_targets+1),
			['Type'] = 1,
			['Target_Data'] = {}
		}
		vr.list_name_targets[#vr.list_targets] = vr.list_targets[#vr.list_targets]['Name']
		vr.current_target = #vr.list_targets
		vr.list_targets[vr.current_target]['Target_Data']['Pos'] = {xx,xy,xz}
		vr.list_targets[vr.current_target]['Target_Data']['Radius'] = 2
		vr.list_targets[vr.current_target]['Target_Data']['Text'] = ''
		vr.list_targets[vr.current_target]['Target_Data']['Text_time'] = 2
		vr.list_targets[vr.current_target]['Target_Data']['Color_blip'] = 1
		vr.current_item = 1
		vr.targets_change_window = false
		vr.target_window = true
	elseif vr.selected_item == 2 then
		vr.targets_list_toggle = 1
		vr.current_item = 1
		vr.targets_change_window = false
		vr.targets_list_window = true
	elseif vr.selected_item == 3 then
		vr.targets_list_toggle = 3
		vr.current_item = 1
		vr.targets_change_window = false
		vr.targets_list_window = true
	elseif vr.selected_item == 4 then
		vr.targets_list_toggle = 2
		vr.current_item = 1
		vr.targets_change_window = false
		vr.targets_list_window = true
	end
end

function targets_list()
	if vr.targets_list_toggle == 1 then
		vr.current_target = vr.selected_item
		vr.current_item = 1
		vr.targets_list_window = false
		vr.target_window = true
	elseif vr.targets_list_toggle == 2 then
		vr.list_targets = DelCellArr(vr.list_targets,vr.current_target)
      	vr.list_name_targets = DelCellArr(vr.list_name_targets,vr.current_target)
		vr.current_item = 1
		vr.targets_list_window = false
		vr.targets_change_window = true
	elseif vr.targets_list_toggle == 3 then
		vr.buffer_target = vr.current_item
		vr.targets_list_toggle = 4
	elseif vr.targets_list_toggle == 4 then
		vr.list_targets = MoveCellArr(vr.list_targets,vr.buffer_target,vr.current_item)
     	vr.list_name_targets = MoveCellArr(vr.list_name_targets,vr.buffer_target,vr.current_item)
		vr.buffer_target = -1
		vr.current_item = 1
		vr.targets_list_window = false
		vr.targets_change_window = true
	end
end

function target()
	
	local xx,xy,xz = getCharCoordinates(PLAYER_PED)
	if vr.selected_item == 1 then
		if vr.list_targets[vr.current_target]['Type'] == 1 then
			vr.list_targets[vr.current_target]['Target_Data']['Pos'] = {xx,xy,xz}
			vr.list_targets[vr.current_target]['Target_Data']['Radius'] = 2
			vr.list_targets[vr.current_target]['Target_Data']['Text'] = ''
			vr.list_targets[vr.current_target]['Target_Data']['Text_time'] = 2
			vr.list_targets[vr.current_target]['Target_Data']['Color_blip'] = 1

		elseif vr.list_targets[vr.current_target]['Type'] == 2 then
			vr.list_targets[vr.current_target]['Target_Data']['Target_car_id'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Color_blip'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Text'] = ''
		elseif vr.list_targets[vr.current_target]['Type'] == 3 then
			vr.list_targets[vr.current_target]['Target_Data']['Target_actor_id'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Color_blip'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Text'] = ''
		elseif vr.list_targets[vr.current_target]['Type'] == 4 then
			vr.list_targets[vr.current_target]['Target_Data']['Target_type'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Pos'] = {xx,xy,xz}
			vr.list_targets[vr.current_target]['Target_Data']['Rotates'] = {0,0,0}
			vr.list_targets[vr.current_target]['Target_Data']['Text'] = ''
			vr.list_targets[vr.current_target]['Target_Data']['Text_time'] = 2
			vr.list_targets[vr.current_target]['Target_Data']['Smooth'] = false
			vr.list_targets[vr.current_target]['Target_Data']['Time'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Weather'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Clock_time'] = {0,0}
			vr.list_targets[vr.current_target]['Target_Data']['Traffic'] = {1,1}
		elseif vr.list_targets[vr.current_target]['Type'] == 5 then
			vr.list_targets[vr.current_target]['Target_Data']['Target_object_id'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Color_blip'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Target_type'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Weap'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Text'] = ''
		elseif vr.list_targets[vr.current_target]['Type'] == 6 then
			vr.list_targets[vr.current_target]['Target_Data']['Target_pickup_id'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Color_blip'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Text'] = ''
		elseif vr.list_targets[vr.current_target]['Type'] == 7 then
			vr.list_targets[vr.current_target]['Target_Data']['Target_type'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Pos'] = {xx,xy,xz}
			vr.list_targets[vr.current_target]['Target_Data']['ModelId'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Model_type'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Angle'] = 0
			vr.list_targets[vr.current_target]['Target_Data']['Weapon'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Weap_ammo'] = 0
			vr.list_targets[vr.current_target]['Target_Data']['Anim'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Pack_anim'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Loop'] = false
			vr.list_targets[vr.current_target]['Target_Data']['Car_id'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Car_place'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Level_battue'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Dialog'] = {}
			vr.list_targets[vr.current_target]['Target_Data']['Dialog_id'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Add_money'] = 1
			vr.list_targets[vr.current_target]['Target_Data']['Interior_id'] = getActiveInterior()
		end
	elseif vr.selected_item == 2 then
		local names = decodeJson(langt['vector3_arr'])
		vr.vector3_items = {
			{'int_slider',names[1],vr.list_targets[vr.current_target]['Target_Data']['Pos'],1,_,_,0.01,1},
			{'int_slider',names[2],vr.list_targets[vr.current_target]['Target_Data']['Pos'],2,_,_,0.01,2},
			{'int_slider',names[3],vr.list_targets[vr.current_target]['Target_Data']['Pos'],3,_,_,0.01,3},
			{'button',names[4],4}
		}
		vr.vector3_items[0] = langt['position']
		vr.previous_window = 'target_window'
		vr.current_item = 1
		vr.vector3_window = true
		vr.target_window = false
	elseif vr.selected_item == 3 then
		vr.editmode_target = true
		printHelpForever('HTARG')
		lockPlayerControl(true)
		vr.id_target = vr.current_target
	elseif vr.selected_item == 4 then
		wait(0)
	elseif vr.selected_item == 6 then
		imgui.StrCopy(vr.buffer_input,vr.list_targets[vr.current_target]['Target_Data']['Text'])
		vr.editmode_input = true
		while vr.editmode_input do
			wait(0)
			if wasKeyReleased(0x46) then
				vr.editmode_input = false
			end
		end
		vr.list_targets[vr.current_target]['Target_Data']['Text'] = ffi.string(vr.buffer_input)
	elseif vr.selected_item == 10 then
		imgui.StrCopy(vr.buffer_input,vr.list_targets[vr.current_target]['Target_Data']['Text'])
		vr.editmode_input = true
		while vr.editmode_input do
			wait(0)
			if wasKeyReleased(0x46) then
				vr.editmode_input = false
			end
		end
		vr.list_targets[vr.current_target]['Target_Data']['Text'] = ffi.string(vr.buffer_input)
	elseif vr.selected_item == 13 then
		imgui.StrCopy(vr.buffer_input,vr.list_targets[vr.current_target]['Target_Data']['Text'])
		vr.editmode_input = true
		while vr.editmode_input do
			wait(0)
			if wasKeyReleased(0x46) then
				vr.editmode_input = false
			end
		end
		vr.list_targets[vr.current_target]['Target_Data']['Text'] = ffi.string(vr.buffer_input)
	elseif vr.selected_item == 15 then
		local names = decodeJson(langt['vector3_arr'])
		vr.vector3_items = {
			{'int_slider',names[1],vr.list_targets[vr.current_target]['Target_Data']['Pos'],1,_,_,0.01,1},
			{'int_slider',names[2],vr.list_targets[vr.current_target]['Target_Data']['Pos'],2,_,_,0.01,2},
			{'int_slider',names[3],vr.list_targets[vr.current_target]['Target_Data']['Pos'],3,_,_,0.01,3},
			{'button',names[4],4}
		}
		vr.vector3_items[0] = langt['position']
		vr.previous_window = 'target_window'
		vr.current_item = 1
		vr.vector3_window = true
		vr.target_window = false
	elseif vr.selected_item == 16 then
		local names = decodeJson(langt['vector3_arr'])
		vr.vector3_items = {
			{'int_slider',names[1],vr.list_targets[vr.current_target]['Target_Data']['Rotates'],1,0,360,_,1},
			{'int_slider',names[2],vr.list_targets[vr.current_target]['Target_Data']['Rotates'],2,0,360,_,2},
			{'int_slider',names[3],vr.list_targets[vr.current_target]['Target_Data']['Rotates'],3,0,360,_,3},
			{'button',names[4],4}
		}
		vr.vector3_items[0] = langt['rotate']
		vr.previous_window = 'target_window'
		vr.current_item = 1
		vr.vector3_window = true
		vr.target_window = false
	elseif vr.selected_item == 17 then
		vr.editmode_camera = true
		printHelpForever('HOBJ')
		vr.id_target = vr.current_target
	elseif vr.selected_item == 19 then
		imgui.StrCopy(vr.buffer_input,vr.list_targets[vr.current_target]['Target_Data']['Text'])
		vr.editmode_input = true
		while vr.editmode_input do
			wait(0)
			if wasKeyReleased(0x46) then
				vr.editmode_input = false
			end
		end
		vr.list_targets[vr.current_target]['Target_Data']['Text'] = ffi.string(vr.buffer_input)
	elseif vr.selected_item == 22 then
		imgui.StrCopy(vr.buffer_input,vr.list_targets[vr.current_target]['Target_Data']['Text'])
		vr.editmode_input = true
		while vr.editmode_input do
			wait(0)
			if wasKeyReleased(0x46) then
			vr.editmode_input = false
			end
		end
		vr.list_targets[vr.current_target]['Target_Data']['Text'] = ffi.string(vr.buffer_input)
	elseif vr.selected_item == 24 then
		forceWeatherNow(vr.list_targets[vr.current_target]['Target_Data']['Weather']-1)
	elseif vr.selected_item == 25 then
		vr.editmode_timetarg = true
		printHelpForever('HMTIM')
		vr.id_timetarg = vr.current_target
	elseif vr.selected_item == 32 then
		imgui.StrCopy(vr.buffer_input,vr.list_targets[vr.current_target]['Target_Data']['Text'])
		vr.editmode_input = true
		while vr.editmode_input do
			wait(0)
			if wasKeyReleased(0x46) then
				vr.editmode_input = false
			end
		end
		vr.list_targets[vr.current_target]['Target_Data']['Text'] = ffi.string(vr.buffer_input)
	elseif vr.selected_item == 35 then
		imgui.StrCopy(vr.buffer_input,vr.list_targets[vr.current_target]['Target_Data']['Text'])
		vr.editmode_input = true
		while vr.editmode_input do
			wait(0)
			if wasKeyReleased(0x46) then
				vr.editmode_input = false
			end
		end
		vr.list_targets[vr.current_target]['Target_Data']['Text'] = ffi.string(vr.buffer_input)
	elseif vr.selected_item == 37 then
		local names = decodeJson(langt['vector3_arr'])
		vr.vector3_items = {
			{'int_slider',names[1],vr.list_targets[vr.current_target]['Target_Data']['Pos'],1,_,_,0.01,1},
			{'int_slider',names[2],vr.list_targets[vr.current_target]['Target_Data']['Pos'],2,_,_,0.01,2},
			{'int_slider',names[3],vr.list_targets[vr.current_target]['Target_Data']['Pos'],3,_,_,0.01,3},
			{'button',names[4],4}
		}
		vr.vector3_items[0] = langt['position']
		vr.previous_window = 'target_window'
		vr.current_item = 1
		vr.vector3_window = true
		vr.target_window = false
	elseif vr.selected_item == 39 then
		vr.list_targets[vr.current_target]['Target_Data']['ModelId'] = 1
	elseif vr.selected_item == 43 then
		vr.id_teleport_player = vr.current_target
		printHelpForever('HPLA')
		vr.editmode_teleport_player = true
		lockPlayerControl(false)
	elseif vr.selected_item == 44 then
		vr.list_targets[vr.current_target]['Target_Data']['Anim'] = 1
	elseif vr.selected_item == 48 then
		vr.id_preview_player_anim = vr.current_target
		vr.editmode_preview_player_anim = true
		lockPlayerControl(true)
		printHelpForever('HVIEW')
	elseif vr.selected_item == 52 then
		vr.list_targets[vr.current_target]['Target_Data']['Dialog'][#vr.list_targets[vr.current_target]['Target_Data']['Dialog']+1] = {
			['Text'] = '',
			['Text_time'] = 1
		}
		vr.current_dialog_target = #vr.list_targets[vr.current_target]['Target_Data']['Dialog']
		vr.current_item = 1
		vr.target_dialog_window = true
		vr.target_window = false
	elseif vr.selected_item == 53 then
		vr.current_item = 1
		vr.targets_dialog_toggle = 1
		vr.target_list_dialog_window = true
		vr.target_window = false
	elseif vr.selected_item == 54 then
		vr.current_item = 1
		vr.targets_dialog_toggle = 2
		vr.target_list_dialog_window = true
		vr.target_window = false
	end
end

function target_list_dialog()
	if vr.targets_dialog_toggle == 1 then
		vr.current_dialog_target = vr.selected_item
		vr.current_item = 1
		vr.target_list_dialog_window = false
		vr.target_dialog_window = true
	elseif vr.targets_dialog_toggle == 2 then
		vr.list_targets[vr.current_target]['Target_Data']['Dialog'] = DelCellArr(vr.list_targets[vr.current_target]['Target_Data']['Dialog'],vr.selected_item)
		vr.current_item = 1
		vr.target_list_dialog_window = false
		vr.target_window = true
	end
end

function target_dialog()
	if vr.selected_item == 1 then
		imgui.StrCopy(vr.buffer_input,vr.list_targets[vr.current_target]['Target_Data']['Dialog'][vr.current_dialog_target]['Text'])
		vr.editmode_input = true
		while vr.editmode_input do
			wait(0)
			if wasKeyReleased(0x46) then
				vr.editmode_input = false
			end
		end
		vr.list_targets[vr.current_target]['Target_Data']['Dialog'][vr.current_dialog_target]['Text'] = ffi.string(vr.buffer_input)
	elseif vr.selected_item == 3 then
		vr.target_dialog_window = false
		vr.target_window = true
	end
end

function cars_change()
	
	if vr.selected_item == 1 then
		local xx,xy,xz = getCharCoordinates(PLAYER_PED)
		xz = getGroundZFor3dCoord(xx, xy, xz+10.0)
		vr.list_cars[#vr.list_cars+1] = {
			['Name'] = langt['car']..' #' .. tostring(#vr.list_cars+1),
			['Type'] = 1,
			['Car_Data'] = {
				['Pos'] = {xx,xy,xz},
				['Angle'] = 0,
				['ModelId'] = 400,
				['StartC'] = 1,
				['EndC'] = 1,
				['Color_primary'] = 0,
				['Color_secondary'] = 0,
				['Should_not_die'] = false,
				['Health'] = 1000,
				['Bulletproof'] = false,
				['Fireproof'] = false,
				['Explosionproof'] = false,
				['Collisionproof'] = false,
				['Meleeproof'] = false,
				['Tires_vulnerability'] = false,
				['Anims'] = {},
				['Anim_id'] = 1
			}

		}
		vr.list_name_cars[#vr.list_cars] = vr.list_cars[#vr.list_cars]['Name']
		vr.current_car = #vr.list_cars
		vr.current_item = 1
		vr.cars_change_window = false
		vr.car_window = true
		upd_car:run(#vr.list_cars)
	elseif vr.selected_item == 2 then
		vr.cars_list_toggle = 1
		vr.current_item = 1
		vr.cars_change_window = false
		vr.cars_list_window = true
	elseif vr.selected_item == 3 then
		vr.cars_list_toggle = 2
		vr.current_item = 1
		vr.cars_change_window = false
		vr.cars_list_window = true
	end
end

function cars_list()
	if vr.cars_list_toggle == 1 then
		vr.current_car = vr.selected_item
		vr.current_item = 1
		vr.cars_list_window = false
		vr.car_window = true
	elseif vr.cars_list_toggle == 2 then
		deleteCar(vr.list_cars[vr.selected_item]['Car_Data']['Car'])
		vr.list_cars = DelCellArr(vr.list_cars,vr.selected_item)
		vr.list_name_cars = DelCellArr(vr.list_name_cars,vr.selected_item)
		vr.current_item = 1
		vr.cars_list_window = false
		vr.cars_change_window = true
	end
end

function car()
	
	if vr.selected_item == 1 then
		local names = decodeJson(langt['vector3_arr'])
		vr.vector3_items = {
			{'int_slider',names[1],vr.list_cars[vr.current_car]['Car_Data']['Pos'],1,_,_,0.01,1},
			{'int_slider',names[2],vr.list_cars[vr.current_car]['Car_Data']['Pos'],2,_,_,0.01,2},
			{'int_slider',names[3],vr.list_cars[vr.current_car]['Car_Data']['Pos'],3,_,_,0.01,3},
			{'button',names[4],4}
		}
		vr.vector3_items[0] = langt['position']
		vr.previous_window = 'car_window'
		vr.current_item = 1
		vr.vector3_window = true
		vr.car_window = false
	elseif vr.selected_item == 2 then
		upd_car:run(vr.current_car)
	elseif vr.selected_item == 3 or vr.selected_item == 4 then
		changeCarColour(vr.list_cars[vr.current_car]['Car_Data']['Car'],vr.list_cars[vr.current_car]['Car_Data']['Color_primary'],vr.list_cars[vr.current_car]['Car_Data']['Color_secondary'])
	elseif vr.selected_item == 13 then
		vr.editmode_car = true
		vr.id_car = vr.current_car
		deleteCar(vr.list_cars[vr.current_car]['Car_Data']['Car'])
	elseif vr.selected_item == 14 then
		vr.current_item = 1
		vr.car_anims_change_window = true
		vr.car_window = false
	end
end

function car_anims_change()
	if vr.selected_item == 1 then
		vr.list_cars[vr.current_car]['Car_Data']['Anims'][#vr.list_cars[vr.current_car]['Car_Data']['Anims']+1] = {
			['Type'] = 1,
		}
		vr.list_cars[vr.current_car]['Car_Data']['Anim_id'] = #vr.list_cars[vr.current_car]['Car_Data']['Anims']
		vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Doors'] = {false,false,false,false,false,false}
		vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Condition'] = 1 
		vr.current_item = 1
		vr.car_anims_change_window = false
		vr.car_anim_window = true
	elseif vr.selected_item == 2 then
		vr.car_anims_list_toggle = 1
		vr.current_item = 1
		vr.car_anims_change_window = false
		vr.car_anims_list_window = true
	elseif vr.selected_item == 3 then
		vr.car_anims_list_toggle = 2
		vr.current_item = 1
		vr.car_anims_change_window = false
		vr.car_anims_list_window = true
	end
end

function car_anims_list()
	if vr.car_anims_list_toggle == 1 then
		vr.list_cars[vr.current_car]['Car_Data']['Anim_id'] = vr.selected_item
		vr.current_item = 1
		vr.car_anims_list_window = false
		vr.car_anim_window = true
	elseif vr.actor_anims_list_toggle == 2 then
		vr.list_cars[vr.current_car]['Car_Data']['Anims'] = DelCellArr(vr.list_cars[vr.current_car]['Car_Data']['Anims'],vr.selected_item)
		vr.current_item = 1
		vr.car_anims_list_window = false
		vr.car_anims_change_window = true
	end
end

function car_anim()
	if vr.selected_item == 1 then
		if vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Type'] == 1 then
			vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Doors'] = {false,false,false,false,false,false}
			vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Condition'] = 1 
		elseif vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Type'] == 2 then
			vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Door_lock'] = 1
			vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Condition'] = 1
		end
	end
end

function objects_change()
	
	if vr.selected_item == 1 then
		local xx,xy,xz = getCharCoordinates(PLAYER_PED)
		xz = getGroundZFor3dCoord(xx, xy, xz)
		vr.list_objects[#vr.list_objects+1] = {
			['Name'] = langt['object']..' #' .. tostring(#vr.list_objects+1),
			['Type'] = 1,
			['Object_Data'] = {
				['Pos'] = {xx,xy,xz},
				['Rotates'] = {0,0,0},
				['ModelId'] = 400,
				['StartC'] = 1,
				['EndC'] = 1,
				['Anim_id'] = 1,
				['Anims'] = {}
			}

		}
		vr.list_name_objects[#vr.list_objects] = vr.list_objects[#vr.list_objects]['Name']
		vr.current_object = #vr.list_objects
		vr.current_item = 1
		vr.objects_change_window = false
		vr.object_window = true
		upd_object:run(#vr.list_objects)
	elseif vr.selected_item == 2 then
		vr.objects_list_toggle = 1
		vr.current_item = 1
		vr.objects_change_window = false
		vr.objects_list_window = true
	elseif vr.selected_item == 3 then
		vr.objects_list_toggle = 2
		vr.current_item = 1
		vr.objects_change_window = false
		vr.objects_list_window = true
	end
end

function objects_list()
	if vr.objects_list_toggle == 1 then
		vr.current_object = vr.selected_item
		vr.current_item = 1
		vr.objects_list_window = false
		vr.object_window = true
	elseif vr.objects_list_toggle == 2 then
		deleteObject(vr.list_objects[vr.selected_item]['Object_Data']['Obj'])
		vr.list_objects = DelCellArr(vr.list_objects,vr.selected_item)
		vr.list_name_objects = DelCellArr(vr.list_name_objects,vr.selected_item)
		vr.current_item = 1
		vr.objects_list_window = false
		vr.objects_change_window = true
	end
end

function object()
	
	if vr.selected_item == 1 then
		local names = decodeJson(langt['vector3_arr'])
		vr.vector3_items = {
			{'int_slider',names[1],vr.list_objects[vr.current_object]['Object_Data']['Pos'],1,_,_,0.01,1},
			{'int_slider',names[2],vr.list_objects[vr.current_object]['Object_Data']['Pos'],2,_,_,0.01,2},
			{'int_slider',names[3],vr.list_objects[vr.current_object]['Object_Data']['Pos'],3,_,_,0.01,3},
			{'button',names[4],4}
		}
		vr.vector3_items[0] = decodeJson(langt['actor_arr'])[1]
		vr.previous_window = 'object_window'
		vr.current_item = 1
		vr.vector3_window = true
		vr.object_window = false
	elseif vr.selected_item == 2 then
		local names = decodeJson(langt['vector3_arr'])
		vr.vector3_items = {
			{'int_slider',names[3],vr.list_objects[vr.current_object]['Object_Data']['Rotates'],3,-360,360,1},
			{'int_slider',names[2],vr.list_objects[vr.current_object]['Object_Data']['Rotates'],2,-360,360,2},
			{'int_slider',names[1],vr.list_objects[vr.current_object]['Object_Data']['Rotates'],1,-360,360,3},
			{'button',names[4],4}
		}
		vr.vector3_items[0] = langt['rotate']
		vr.previous_window = 'object_window'
		vr.current_item = 1
		vr.vector3_window = true
		vr.object_window = false
	elseif vr.selected_item == 3 then
		if vr.list_objects[vr.current_object]['Object_Data']['Obj'] ~= nil then
			vr.editmode_objects = true
			printHelpForever('HOBJ')
			lockPlayerControl(true)
			vr.id_obj = vr.current_object
		end
	elseif vr.selected_item == 4 then
		imgui.StrCopy(vr.buffer_input,tostring(vr.list_objects[vr.current_object]['Object_Data']['ModelId']))
		vr.editmode_input = true
		while vr.editmode_input do
			wait(0)
			if wasKeyReleased(0x46) then
				vr.editmode_input = false
			end
		end
		vr.list_objects[vr.current_object]['Object_Data']['ModelId'] = tonumber(ffi.string(vr.buffer_input)) or 0
		upd_object:run(vr.current_object)
	elseif vr.selected_item == 5 then
		vr.current_item = 1
		vr.object_window = false
		vr.object_anims_change_window = true
	end
end

function object_anims_change()
	if vr.selected_item == 1 then
		local xx,xy,xz = getCharCoordinates(PLAYER_PED)
			vr.list_objects[vr.current_object]['Object_Data']['Anims'][#vr.list_objects[vr.current_object]['Object_Data']['Anims']+1] = {
				['Pos'] = {xx,xy,xz},
				['Rotates'] = {0,0,0},
				['Time'] = 1,
				['Condition'] = 1,
			}
		vr.current_item = 1
		vr.object_anims_change_window = false
		vr.object_anim_window = true
	elseif vr.selected_item == 2 then
		vr.object_anims_list_toggle = 1
		vr.current_item = 1
		vr.object_anims_change_window = false
		vr.object_anims_list_window = true
	elseif vr.selected_item == 3 then
		vr.object_anims_list_toggle = 2
		vr.current_item = 1
		vr.object_anims_change_window = false
		vr.object_anims_list_window = true
	end
end

function object_anims_list()
	if vr.object_anims_list_toggle == 1 then
		vr.list_objects[vr.current_object]['Object_Data']['Anim_id'] = vr.selected_item
		vr.current_item = 1
		vr.object_anims_list_window = false
		vr.object_anim_window = true
	elseif vr.object_anims_list_toggle == 2 then
		vr.list_objects[vr.current_object]['Object_Data']['Anim_id'] = DelCellArr(vr.list_objects[vr.current_object]['bject_Data']['Anims'],vr.selected_item)
		vr.current_item = 1
		vr.object_anims_list_window = false
		vr.object_anims_change_window = true
	end
end

function object_anim()
	
	if vr.selected_item == 3 then
		local names = decodeJson(langt['vector3_arr'])
		vr.vector3_items = {
			{'int_slider',names[1],vr.list_objects[vr.current_object]['Object_Data']['Anims'][vr.list_objects[vr.current_object]['Object_Data']['Anim_id']]['Pos'],1,_,_,0.01,1},
			{'int_slider',names[2],vr.list_objects[vr.current_object]['Object_Data']['Anims'][vr.list_objects[vr.current_object]['Object_Data']['Anim_id']]['Pos'],2,_,_,0.01,2},
			{'int_slider',names[3],vr.list_objects[vr.current_object]['Object_Data']['Anims'][vr.list_objects[vr.current_object]['Object_Data']['Anim_id']]['Pos'],3,_,_,0.01,3},
			{'button',names[4],4}
		}
		vr.vector3_items[0] = decodeJson(langt['actor_arr'])[1]
		vr.previous_window = 'object_anim_window'
		vr.current_item = 1
		vr.vector3_window = true
		vr.object_anim_window = false
	elseif vr.selected_item == 4 then
		local names = decodeJson(langt['vector3_arr'])
		vr.vector3_items = {
			{'int_slider',names[1],vr.list_objects[vr.current_object]['Object_Data']['Anims'][vr.list_objects[vr.current_object]['Object_Data']['Anim_id']]['Rotates'],1,-360,360,1},
			{'int_slider',names[2],vr.list_objects[vr.current_object]['Object_Data']['Anims'][vr.list_objects[vr.current_object]['Object_Data']['Anim_id']]['Rotates'],2,-360,360,2},
			{'int_slider',names[3],vr.list_objects[vr.current_object]['Object_Data']['Anims'][vr.list_objects[vr.current_object]['Object_Data']['Anim_id']]['Rotates'],3,-360,360,3},
			{'button',names[4],4}
		}
		vr.vector3_items[0] = langt['rotate']
		vr.previous_window = 'object_anim_window'
		vr.current_item = 1
		vr.vector3_window = true
		vr.object_anim_window = false
	elseif vr.selected_item == 6 then
		if vr.list_objects[vr.current_object]['Object_Data']['Obj'] ~= nil then
			vr.editmode_objects_anim = true
			printHelpForever('HOBJ')
			lockPlayerControl(true)
			vr.id_obj = vr.current_object
			vr.id_obj_anim = vr.list_objects[vr.current_object]['Object_Data']['Anim_id']
		end
	end
end

function particles_change()
	
	if vr.selected_item == 1 then
		local xx,xy,xz = getCharCoordinates(PLAYER_PED)
		xz = getGroundZFor3dCoord(xx, xy, xz)
		vr.list_particle[#vr.list_particle+1] = {
			['Name'] = langt['particle']..' #' .. tostring(#vr.list_particle+1),
			['Type'] = 1,
			['Particle_Data'] = {
				['Pos'] = {xx,xy,xz},
				['Rotates'] = {0,0,0},
				['ModelId'] = 1,
				['StartC'] = 1,
				['EndC'] = 1
			}
		}
		vr.list_name_particle[#vr.list_particle] = vr.list_particle[#vr.list_particle]['Name']
		vr.current_particle = #vr.list_particle
		vr.current_item = 1
		vr.particles_change_window = false
		vr.particle_window = true
		upd_particle:run(#vr.list_particle)
	elseif vr.selected_item == 2 then
		vr.particles_list_toggle = 1
		vr.current_item = 1
		vr.particles_change_window = false
		vr.particles_list_window = true
	elseif vr.selected_item == 3 then
		vr.particles_list_toggle = 2
		vr.current_item = 1
		vr.particles_change_window = false
		vr.particles_list_window = true
	end
end

function particles_list()
	if vr.particles_list_toggle == 1 then
		vr.current_particle = vr.selected_item
		vr.current_item = 1
		vr.particles_list_window = false
		vr.particle_window = true
	elseif vr.particles_list_toggle == 2 then
		killFxSystem(vr.list_particle[vr.selected_item]['Particle_Data']['Prtcl'])
		vr.list_particle = DelCellArr(vr.list_particle,vr.selected_item)
		vr.list_name_particle = DelCellArr(vr.list_name_particle,vr.selected_item)
		vr.current_item = 1
		vr.particles_list_window = false
		vr.particles_change_window = true
	end
end

function particle()
	
	if vr.selected_item == 1 then
		local names = decodeJson(langt['vector3_arr'])
		vr.vector3_items = {
			{'int_slider',names[1],vr.list_particle[vr.current_particle]['Particle_Data']['Pos'],1,_,_,0.01,1},
			{'int_slider',names[2],vr.list_particle[vr.current_particle]['Particle_Data']['Pos'],2,_,_,0.01,2},
			{'int_slider',names[3],vr.list_particle[vr.current_particle]['Particle_Data']['Pos'],3,_,_,0.01,3},
			{'button',names[4],4}
		}
		vr.vector3_items[0] = decodeJson(langt['actor_arr'])[1]
		vr.previous_window = 'particle_window'
		vr.current_item = 1
		vr.vector3_window = true
		vr.particle_window = false
	elseif vr.selected_item == 2 then
		local names = decodeJson(langt['vector3_arr'])
		vr.vector3_items = {
			{'int_slider',names[1],vr.list_particle[vr.current_particle]['Particle_Data']['Rotates'],1,-360,360,1},
			{'int_slider',names[2],vr.list_particle[vr.current_particle]['Particle_Data']['Rotates'],2,-360,360,2},
			{'int_slider',names[3],vr.list_particle[vr.current_particle]['Particle_Data']['Rotates'],3,-360,360,3},
			{'button',names[4],4}
		}
		vr.vector3_items[0] = langt['rotate']
		vr.previous_window = 'particle_window'
		vr.current_item = 1
		vr.vector3_window = true
		vr.particle_window = false
	elseif vr.selected_item == 3 then
		vr.editmode_particle = true
		printHelpForever('HOBJ')
		lockPlayerControl(true)
		vr.id_prtcl = vr.current_particle
	elseif vr.selected_item == 4 then
		upd_particle:run(vr.current_particle)
	end
end

function pickup_change()
	
	if vr.selected_item == 1 then
		local xx,xy,xz = getCharCoordinates(PLAYER_PED)
		xz = getGroundZFor3dCoord(xx, xy, xz)
		vr.list_pickup[#vr.list_pickup+1] = {
			['Name'] = langt['pickup']..' #' .. tostring(#vr.list_pickup+1),
			['Type'] = 1,
			['Pickup_Data'] = {
				['Type_pickup'] = 1,
				['Pos'] = {xx,xy,xz},
				['StartC'] = 1,
				['EndC'] = 1,
				['spawn_type'] = 1
			}

		}
		vr.list_name_pickup[#vr.list_pickup] = vr.list_pickup[#vr.list_pickup]['Name']
		vr.current_pickup = #vr.list_pickup
		vr.list_pickup[vr.current_pickup]['Pickup_Data']['Ammo'] = 0
		vr.list_pickup[vr.current_pickup]['Pickup_Data']['Weapon'] = 1
		vr.current_item = 1
		vr.pickup_change_window = false
		vr.pickup_window = true
		upd_pickup:run(vr.current_pickup)
	elseif vr.selected_item == 2 then
		vr.pickup_list_toggle = 1
		vr.current_item = 1
		vr.pickup_change_window = false
		vr.pickup_list_window = true
	elseif vr.selected_item == 3 then
		vr.pickup_list_toggle = 2
		vr.current_item = 1
		vr.pickup_change_window = false
		vr.pickup_list_window = true
	end
end

function pickup_list()
	if vr.pickup_list_toggle == 1 then
		vr.current_pickup = vr.selected_item
		vr.current_item = 1
		vr.pickup_list_window = false
		vr.pickup_window = true
	elseif vr.pickup_list_toggle == 2 then
		removePickup(vr.list_pickup[vr.selected_item]['Pickup_Data']['Pick'])
		vr.list_pickup = DelCellArr(vr.list_pickup,vr.selected_item)
		vr.list_name_pickup = DelCellArr(vr.list_name_pickup,vr.selected_item)
		vr.current_item = 1
		vr.pickup_list_window = false
		vr.pickup_change_window = true
	end
end

function pickup()
	
	if vr.selected_item == 2 then
		if vr.list_pickup[vr.current_pickup]['Pickup_Data']['Type_pickup'] == 1 then
			vr.list_pickup[vr.current_pickup]['Pickup_Data']['Ammo'] = 0
			vr.list_pickup[vr.current_pickup]['Pickup_Data']['Weapon'] = 1
		elseif vr.list_pickup[vr.current_pickup]['Pickup_Data']['Type_pickup'] == 6 then
			vr.list_pickup[vr.current_pickup]['Pickup_Data']['ModelId'] = 0
		end
		if vr.list_pickup[vr.current_pickup]['Pickup_Data']['Type_pickup'] ~= 6 then upd_pickup:run(vr.current_pickup) end
	elseif vr.selected_item == 3 then
		vr.editmode_pickup = true
		vr.id_pick = vr.current_pickup
		lockPlayerControl(true)
		printHelpForever('HPICW')
	elseif vr.selected_item == 6 then
		imgui.StrCopy(vr.buffer_input,tostring(vr.list_pickup[vr.current_pickup]['Pickup_Data']['ModelId']))
		vr.editmode_input = true
		while vr.editmode_input do
			wait(0)
			if wasKeyReleased(0x46) then
				vr.editmode_input = false
			end
		end
		vr.list_pickup[vr.current_pickup]['Pickup_Data']['ModelId'] = tonumber(ffi.string(vr.buffer_input)) or 0
		upd_pickup:run(vr.current_pickup)
	end
end

function explosion_change()
	if vr.selected_item == 1 then
		local xx,xy,xz = getCharCoordinates(PLAYER_PED)
		xz = getGroundZFor3dCoord(xx, xy, xz)
		vr.list_explosion[#vr.list_explosion+1] = {
			['Name'] = langt['explosion']..' #' .. tostring(#vr.list_explosion+1),
			['Type'] = 1,
			['Explosion_Data'] = {
				['Type'] = 1,
				['Pos'] = {xx,xy,xz},
				['StartC'] = 1,
				['EndC'] = 1
			}

		}
		vr.list_name_explosion[#vr.list_explosion] = vr.list_explosion[#vr.list_explosion]['Name']
		vr.current_explosion = #vr.list_explosion
		vr.list_explosion[vr.current_explosion]['Explosion_Data']['Size_fire'] = 0
		vr.list_explosion[vr.current_explosion]['Explosion_Data']['Propagation_fire'] = 0
		deleteObject(vr.list_explosion[vr.current_explosion]['Explosion_Data']['Explosion'])
		vr.current_item = 1
		vr.explosion_change_window = false
		vr.explosion_window = true
		upd_explosion:run(vr.current_explosion)
	elseif vr.selected_item == 2 then
		vr.explosion_list_toggle = 1
		vr.current_item = 1
		vr.explosion_change_window = false
		vr.explosion_list_window = true
	elseif vr.selected_item == 3 then
		vr.explosion_list_toggle = 2
		vr.current_item = 1
		vr.explosion_change_window = false
		vr.explosion_list_window = true
	end
end

function explosion_list()
	if vr.explosion_list_toggle == 1 then
		vr.current_explosion = vr.selected_item
		vr.current_item = 1
		vr.explosion_list_window = false
		vr.explosion_window = true
	elseif vr.explosion_list_toggle == 2 then
		removeExplosion(vr.list_explosion[vr.selected_item]['Explosion_Data']['Pick'])
		vr.list_explosion = DelCellArr(vr.list_explosion,vr.selected_item)
		vr.list_name_explosion = DelCellArr(vr.list_name_explosion,vr.selected_item)
		vr.current_item = 1
		vr.explosion_list_window = false
		vr.explosion_change_window = true
	end
end

function explosion()
	if vr.selected_item == 1 then
		if vr.list_explosion[vr.current_explosion]['Explosion_Data']['Type'] == 1 then
			vr.list_explosion[vr.current_explosion]['Explosion_Data']['Size_fire'] = 0
			vr.list_explosion[vr.current_explosion]['Explosion_Data']['Propagation_fire'] = 0
			deleteObject(vr.list_explosion[vr.current_explosion]['Explosion_Data']['Explosion'])
			upd_explosion:run(vr.current_explosion)
		elseif vr.list_explosion[vr.current_explosion]['Explosion_Data']['Type'] == 2 then
			vr.list_explosion[vr.current_explosion]['Explosion_Data']['Type_explosion'] = 1
			removeScriptFire(vr.list_explosion[vr.current_explosion]['Explosion_Data']['Fire'])
			upd_explosion:run(vr.current_explosion)
		end
	elseif vr.selected_item == 2 then
		local names = decodeJson(langt['vector3_arr'])
		vr.vector3_items = {
			{'int_slider',names[1],vr.list_explosion[vr.current_explosion]['Explosion_Data']['Pos'],1,_,_,0.01},
			{'int_slider',names[2],vr.list_explosion[vr.current_explosion]['Explosion_Data']['Pos'],2,_,_,0.01},
			{'int_slider',names[3],vr.list_explosion[vr.current_explosion]['Explosion_Data']['Pos'],3,_,_,0.01},
			{'button',names[4]}
		}
		vr.vector3_items[0] = decodeJson(langt['actor_arr'])[1]
		vr.previous_window = 'explosion_window'
		vr.current_item = 1
		vr.vector3_window = true
		vr.explosion_window = false
	elseif (vr.selected_item == 4 or vr.selected_item == 5) then
		upd_explosion:run(vr.current_explosion)
	elseif vr.selected_item == 6 then
		vr.editmode_explosion = true
		printHelpForever('HTARG')
		lockPlayerControl(true)
		vr.id_explosion = vr.current_explosion
	end
end

function pack_mission_change()
	if vr.selected_item == 1 then
		local m = #vr.list_mission_pack+1
    	vr.list_mission_pack[m] = {
    	  ['Name'] = langt['packMissions']..' #' .. tostring(#vr.list_mission_pack+1),
    	  ['Missions'] = vr.list_missions
    	}
    	vr.list_name_mission_pack[m] = vr.list_mission_pack[m]['Name']
    	manager.save(vr.list_mission_pack[m],m-1)
		vr.Fast_data['CurPack'] = m-1
		printString(koder(u8:decode(langt['new_miss_pack'])),1000)
	elseif vr.selected_item == 2 then
		vr.pack_mission_list_toggle = 1
		vr.current_item = 1
		vr.pack_mission_change_window = false
		vr.pack_mission_list_window = true
	elseif vr.selected_item == 3 then
		vr.pack_mission_list_toggle = 2
		vr.current_item = 1
		vr.pack_mission_change_window = false
		vr.pack_mission_list_window = true
	elseif vr.selected_item == 4 then
		vr.pack_mission_list_toggle = 3
		vr.current_item = 1
		vr.pack_mission_change_window = false
		vr.pack_mission_list_window = true
	elseif vr.selected_item == 5 then
		vr.pack_mission_list_toggle = 4
		vr.current_item = 1
		vr.pack_mission_change_window = false
		vr.pack_mission_list_window = true
	end
end

function pack_mission_list()
	if vr.pack_mission_list_toggle == 1 then
		for i = 1,#vr.list_actors do
			deleteChar(vr.list_actors[i]['Actor_Data']['Char'])
		end
		for c = 1,#vr.list_cars do
			deleteCar(vr.list_cars[c]['Car_Data']['Car'])
		end
		vr.list_name_missions = {}
		vr.list_missions = vr.list_mission_pack[vr.selected_item]['Missions']
		vr.Fast_data['CurPack'] = vr.selected_item
		for m = 1,#vr.list_mission_pack[vr.selected_item]['Missions'] do
			vr.list_name_missions[m] = vr.list_mission_pack[vr.selected_item]['Missions'][m]['Name']
		end
		vr.current_item = 1
		vr.pack_mission_list_window = false
		vr.pack_mission_change_window = true
	elseif vr.pack_mission_list_toggle == 2 then
		vr.list_mission_pack[vr.selected_item]['Missions'] = vr.list_missions
    	manager.save(vr.list_mission_pack[vr.selected_item],vr.selected_item-1)
		vr.Fast_data['CurPack'] = vr.selected_item
		vr.current_item = 1
		vr.pack_mission_list_window = false
		vr.pack_mission_change_window = true
	elseif vr.pack_mission_list_toggle == 3 then
		imgui.StrCopy(vr.buffer_input,vr.list_mission_pack[vr.selected_item]['Name'])
		vr.editmode_input = true
		wait(1)
		while vr.editmode_input do
			wait(0)
			if wasKeyReleased(0x46) then
				vr.editmode_input = false
			end
		end
		vr.list_mission_pack[vr.selected_item]['Name'] = ffi.string(vr.buffer_input)
		vr.list_name_mission_pack[vr.selected_item] = ffi.string(vr.buffer_input)
		vr.current_item = 1
		vr.pack_mission_list_window = false
		vr.pack_mission_change_window = true
	elseif vr.pack_mission_list_toggle == 4 then
		vr.list_mission_pack = DelCellArr(vr.list_mission_pack,vr.selected_item)
    	vr.list_name_mission_pack = DelCellArr(vr.list_name_mission_pack,vr.selected_item)
		manager.delete(vr.selected_item)
		vr.current_item = 1
		vr.pack_mission_list_window = false
		vr.pack_mission_change_window = true
	end
end

function missions_change()
	if vr.selected_item == 1 then
		local m = #vr.list_missions+1
		vr.mission_data['Name'] = langt['mission']..' #' .. tostring(m)
		vr.list_missions[m] = {
			['Name'] = vr.mission_data['Name'],
			['Prename'] = '',
			['Mission_Data'] = {
				['Targets'] = vr.list_targets,
				['Actors'] = vr.list_actors,
				['Cars'] = vr.list_cars,
				['Objects'] = vr.list_objects,
				['Pickup'] = vr.list_pickup,
				['Particle'] = vr.list_particle,
				['Explosion'] = vr.list_explosion,
				['Miss_data'] = vr.mission_data,
				['Group_relationships'] = vr.Group_relationships
			}
		}
		vr.list_name_missions[m] = vr.list_missions[m]['Name']
		vr.Fast_data['CurMiss'] = m-1
		printString(koder(u8:decode(langt['new_miss'])),1000)
	elseif vr.selected_item == 2 then
		vr.missions_list_toggle = 1
		vr.current_item = 1
		vr.missions_change_window = false
		vr.missions_list_window = true
	elseif vr.selected_item == 3 then
		vr.missions_list_toggle = 2
		vr.current_item = 1
		vr.missions_change_window = false
		vr.missions_list_window = true
	elseif vr.selected_item == 4 then
		vr.missions_list_toggle = 3
		vr.current_item = 1
		vr.missions_change_window = false
		vr.missions_list_window = true
	elseif vr.selected_item == 5 then
		vr.missions_list_toggle = 4
		vr.current_item = 1
		vr.missions_change_window = false
		vr.missions_list_window = true
	end
end

function missions_list()
	if vr.missions_list_toggle == 1 then
		vr.current_missions = vr.selected_item
		lua_thread.create(load_mission)
		vr.Fast_data['CurMiss'] = vr.selected_item
		vr.missions_change_window = true
		vr.missions_list_window = false
	elseif vr.missions_list_toggle == 2 then
		vr.list_missions[vr.selected_item]['Mission_Data'] = {
			['Targets'] = vr.list_targets,
			['Actors'] = vr.list_actors,
			['Cars'] = vr.list_cars,
			['Objects'] = vr.list_objects,
			['Pickup'] = vr.list_pickup,
			['Particle'] = vr.list_particle,
			['Explosion'] = vr.list_explosion,
			['Miss_data'] = vr.mission_data,
			['Group_relationships'] = vr.Group_relationships
		}
		vr.Fast_data['CurMiss'] = vr.selected_item
		vr.missions_change_window = true
		vr.missions_list_window = false
		vr.current_item = 1
	elseif vr.missions_list_toggle == 3 then
		vr.missions_settings_window = true
		vr.missions_list_window = false
		vr.current_item = 1
		vr.Fast_data['CurMiss'] = vr.selected_item
	elseif vr.missions_list_toggle == 4 then
		vr.list_missions = DelCellArr(vr.list_missions,vr.selected_item)
		vr.list_name_missions = DelCellArr(vr.list_name_missions,vr.selected_item)
		vr.missions_change_window = true
		vr.missions_list_window = false
		vr.current_item = 1
	end
end

function missions_settings()
	if vr.selected_item == 1 then
		imgui.StrCopy(vr.buffer_input,vr.list_missions[vr.Fast_data['CurMiss']]['Name'])
		vr.editmode_input = true
		wait(1)
		while vr.editmode_input do
			wait(0)
			if wasKeyReleased(0x46) then
				vr.editmode_input = false
			end
		end
		vr.list_missions[vr.Fast_data['CurMiss']]['Name'] = ffi.string(vr.buffer_input)
		vr.list_name_missions[vr.Fast_data['CurMiss']] = ffi.string(vr.buffer_input)
		vr.current_item = 1
		vr.missions_settings_window = false
		vr.missions_change_window = true
	elseif vr.selected_item == 2 then
		vr.editmode_timemiss = true
		printHelpForever('HMTIM')
	elseif vr.selected_item == 3 then
		forceWeatherNow(vr.mission_data['Weather']-1)
	end
end

function tools()
	if vr.selected_item == 1 then
		local _,xx,xy,xz = getTargetBlipCoordinates()
		setCharCoordinates(PLAYER_PED,xx,xy,xz)
	elseif vr.selected_item == 2 then
		taskJetpack(PLAYER_PED)
	end
end

function settings()
	if vr.selected_item == 1 then
		inicfg.save(vr.Data,getWorkingDirectory()..'\\LDYOM_data.ini')
		langt = LanguageArr[vr.Data.Settings.curr_lang]['Keys']

		setGxtEntry('HTARG', koder(u8:decode(langt['HTARG'])))
		setGxtEntry('HACT', koder(u8:decode(langt['HACT'])))
		setGxtEntry('HOBJ', koder(u8:decode(langt['HOBJ'])))
		setGxtEntry('HMTIM', koder(u8:decode(langt['HMTIM'])))
		setGxtEntry('HPLA', koder(u8:decode(langt['HPLA'])))
		setGxtEntry('HPLA2', koder(u8:decode(langt['HPLA2'])))
		setGxtEntry('HVIEW',koder(u8:decode(langt['HVIEW'])))
		setGxtEntry('HPICW', koder(u8:decode(langt['HPICW'])))
		setGxtEntry('AAAP',koder(u8:decode(langt['AAAP'])))
		local info_t = decodeJson(u8:decode(langt['info_t']))
		info_t = info_t[1]..'~r~ '..thisScript().authors[1]..'~w~~n~'..info_t[2]..'~r~ '..thisScript().authors[2]..'~w~~n~'..info_t[3]..' '..thisScript().version..'~n~~n~'..info_t[4]..'~n~'..info_t[5]..'~n~~n~'..info_t[6]
		setGxtEntry('INFOLD',koder(info_t))

		Weather_str = decodeJson(langt['Weather_arr'])

		local names = decodeJson(langt['main_menu_arr'])
		vr.main_menu_items = {
			{'button',names[1]},
			{'button',names[2]},
			{'button',names[3]},
			{'button',names[4]},
			{'button',names[5]},
			{'button',names[6]},
			{'button',names[7]},
			{'button',names[8]},
			{'button',names[9]},
			{'button',names[10]},
			{'button',names[11]},
			{'button',names[12]},
			{'button',names[13]},
			{'button',names[14]},
		}
		names = decodeJson(langt['actor_change_arr'])
		vr.actor_change_items = {
			{'button',names[1]},
			{'button',names[2]},
			{'button',names[3]}
		}
		names = decodeJson(langt['actor_anims_change_arr'])
		vr.actor_anims_change_items = {
			{'button',names[1]},
			{'button',names[2]},
			{'button',names[3]}
		}
		names = decodeJson(langt['targets_change_arr'])
		vr.targets_change_items = {
			{'button',names[1]},
			{'button',names[2]},
			{'button',names[3]},
			{'button',names[4]}
		}
		names = decodeJson(langt['cars_change_arr'])
		vr.cars_change_items = {
			{'button',names[1]},
			{'button',names[2]},
			{'button',names[3]}
		}
		names = decodeJson(langt['objects_change_arr'])
		vr.objects_change_items = {
			{'button',names[1]},
			{'button',names[2]},
			{'button',names[3]}
		}
		names = decodeJson(langt['particles_change_arr'])
		vr.particles_change_items = {
			{'button',names[1]},
			{'button',names[2]},
			{'button',names[3]}
		}
		names = decodeJson(langt['pickup_change_arr'])
		vr.pickup_change_items = {
			{'button',names[1]},
			{'button',names[2]},
			{'button',names[3]}
		}
		names = decodeJson(langt['explosion_change_arr'])
		vr.explosion_change_items = {
			{'button',names[1]},
			{'button',names[2]},
			{'button',names[3]}
		}
		names = decodeJson(langt['mission_pack_change_arr'])
		vr.mission_pack_change_items = {
			{'button',names[1]},
			{'button',names[2]},
			{'button',names[3]},
			{'button',names[4]},
			{'button',names[5]}
		}
		names = decodeJson(langt['missions_change_arr'])
		vr.missions_change_items = {
			{'button',names[1]},
			{'button',names[2]},
			{'button',names[3]},
			{'button',names[4]},
			{'button',names[5]}
		}
	end
end

function Group_relationships_list()
	vr.Group_relationships_items = {}
	local groups = decodeJson(langt['name_groups'])
	for i = 1,9 do
		if i ~= vr.selected_item then
			print(vr.current_group_relationships)
			print(encodeJson(vr['Group_relationships'][vr.current_group_relationships]))
			vr.Group_relationships_items[#vr.Group_relationships_items+1] = {'arr_slider',groups[i],vr['Group_relationships'][vr.selected_item],i,decodeJson(langt['Relationship_types']),i}
		end
	end
	vr.current_item = 1
	vr.Group_relationships_list_window = false
	vr.current_group_relationships = vr.selected_item
	vr.Group_relationships_window = true
end

function Group_relationships()
end

function vector3(arr_menu)
	if vr.selected_item ~= #arr_menu then
		if vr.previous_window == 'actor_window' then
			local xx,xy,xz = vr.list_actors[vr.current_actor]['Actor_Data']['Pos'][1],vr.list_actors[vr.current_actor]['Actor_Data']['Pos'][2],vr.list_actors[vr.current_actor]['Actor_Data']['Pos'][3]
			setCharCoordinates(vr.list_actors[vr.current_actor]['Actor_Data']['Char'],xx,xy,xz)
		elseif vr.previous_window == 'car_window' then
			local xx,xy,xz = vr.list_cars[vr.current_car]['Car_Data']['Pos'][1],vr.list_cars[vr.current_car]['Car_Data']['Pos'][2],vr.list_cars[vr.current_car]['Car_Data']['Pos'][3]
			setCarCoordinates(vr.list_cars[vr.current_car]['Car_Data']['Car'],xx,xy,xz)
			setCarHeading(vr.list_cars[vr.current_car]['Car_Data']['Car'], vr.list_cars[vr.current_car]['Car_Data']['Angle'])
		elseif vr.previous_window == 'object_window' then
			local xx,xy,xz = vr.list_objects[vr.current_object]['Object_Data']['Pos'][1],vr.list_objects[vr.current_object]['Object_Data']['Pos'][2],vr.list_objects[vr.current_object]['Object_Data']['Pos'][3]
			local rxx,rxy,rxz = vr.list_objects[vr.current_object]['Object_Data']['Rotates'][1],vr.list_objects[vr.current_object]['Object_Data']['Rotates'][2],vr.list_objects[vr.current_object]['Object_Data']['Rotates'][3]
			setObjectCoordinates(vr.list_objects[vr.current_object]['Object_Data']['Obj'],xx,xy,xz)
			setObjectRotation(vr.list_objects[vr.current_object]['Object_Data']['Obj'], rxx, rxy, rxz)
		elseif vr.previous_window == 'particle_window' then
			upd_particle:run(#vr.list_particle)
		elseif vr.previous_window == 'explosion_window' then
			upd_explosion:run(vr.current_explosion)
		end
	else
		vr.current_item = 1
		vr[vr.previous_window] = true
		vr.vector3_window = false
	end
end

-- Удаление ячейки
function DelCellArr(arr,i)
	local array = {}
	for v,k in ipairs(arr) do
		if v ~= i then
			array[#array+1] = k
		end
	end
	return array
end

function range(r)
	local arr = {}
	for i = 1,r do
		arr[#arr+1] = tostring(i)
	end
	return arr
end

function ternar(condition,truee,falses)
	if condition then
		return truee
	else
		return falses
	end
end

function DrawLineBy3dCoords(posX, posY, posZ, posX2, posY2, posZ2, width, color)
	if isPointOnScreen(posX, posY, posZ,1) and isPointOnScreen(posX2, posY2, posZ2,1) then
		local SposX, SposY = convert3DCoordsToScreen(posX, posY, posZ)
		local SposX2, SposY2 = convert3DCoordsToScreen(posX2, posY2, posZ2)
		renderDrawLine(SposX, SposY, SposX2, SposY2, width, color)
	end
end

-- Перемещение ячейки
function MoveCellArr(arr,i,j)
	local array = {}
	for v,k in ipairs(arr) do
		if v ~= i then
			array[#array+1] = k
		end
		if v == j then
			array[#array+1] = arr[i]
		end
	end
	return array
end

function deepcopy(object)
  local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, orig, nil do
            copy[manager.sorterTable2(orig_key)] = manager.sorterTable2(orig_value)
        end
		setmetatable(copy, manager.sorterTable2(getmetatable(orig)))
	elseif orig_type == 'userdata' then
		if type(orig) == 'userdata' then
			copy = {orig[1],orig[2],orig[3]}
		else
			copy = orig
		end

    else
        copy = orig
    end
    return copy
end

function update_actor(actorr)
	local modell
	local xx,xy,xz = vr.list_actors[actorr]['Actor_Data']['Pos'][1], vr.list_actors[actorr]['Actor_Data']['Pos'][2], vr.list_actors[actorr]['Actor_Data']['Pos'][3]
	local angle = vr.list_actors[actorr]['Actor_Data']['Angle']

	if vr.list_actors[actorr]['Actor_Data']['Model_type'] == 1 then
		modell = ID_Actors[vr.list_actors[actorr]['Actor_Data']['ModelId']]
		requestModel(modell)
		while not hasModelLoaded(modell) do
			wait(0)
		end
	elseif vr.list_actors[actorr]['Actor_Data']['Model_type'] == 2 then
		local modell_n = ID_Spec_Actors[vr.list_actors[actorr]['Actor_Data']['ModelId']]
		loadSpecialCharacter(modell_n,vr.list_actors[actorr]['Actor_Data']['Slot_model'])
		while not hasSpecialCharacterLoaded(vr.list_actors[actorr]['Actor_Data']['Slot_model']) do
			wait(0)
		end
		modell = 290 + vr.list_actors[actorr]['Actor_Data']['Slot_model']-1
	end

	deleteChar(vr.list_actors[actorr]['Actor_Data']['Char'])
	print(modell)
	vr.list_actors[actorr]['Actor_Data']['Char'] = createChar(4,modell,xx,xy,xz)
	if vr.list_actors[actorr]['Actor_Data']['Model_type'] == 1 then
		markModelAsNoLongerNeeded(modell)
	else
		unloadSpecialCharacter(vr.list_actors[actorr]['Actor_Data']['Slot_model'])
	end
	setCharHeading(vr.list_actors[actorr]['Actor_Data']['Char'], angle)
	setCharCollision(vr.list_actors[actorr]['Actor_Data']['Char'],false)
	requestModel(getWeapontypeModel(ID_Weapons[vr.list_actors[actorr]['Actor_Data']['Weapon']]))
	while not hasModelLoaded(getWeapontypeModel(ID_Weapons[vr.list_actors[actorr]['Actor_Data']['Weapon']])) do
		wait(0)
	end
	giveWeaponToChar(vr.list_actors[actorr]['Actor_Data']['Char'], ID_Weapons[vr.list_actors[actorr]['Actor_Data']['Weapon']], 1)
	setCurrentCharWeapon(vr.list_actors[actorr]['Actor_Data']['Char'],1)
	markModelAsNoLongerNeeded(getWeapontypeModel(ID_Weapons[vr.list_actors[actorr]['Actor_Data']['Weapon']]))
end

function update_actor_animation(actorr,anim)
	if not hasAnimationLoaded(Anims['Anim_name'][vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim']]) then
		requestAnimation(Anims['Anim_name'][vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim']])
		while not hasAnimationLoaded(Anims['Anim_name'][vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim']]) do
			wait(0)
		end
	end
	local animm = Anims['Anim_list'][vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim']]
	animm = animm[vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Anim']]
	taskPlayAnimNonInterruptable(vr.list_actors[actorr]['Actor_Data']['Char'], animm, Anims['Anim_name'][vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim']], 1, vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Loop'], false, false, false, -1)
	while not isCharPlayingAnim(vr.list_actors[actorr]['Actor_Data']['Char'],animm) do
		wait(0)
	end
	vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Time'] = getCharAnimTotalTime(vr.list_actors[actorr]['Actor_Data']['Char'],animm) / 1000
end

function update_car(carr)
	local modell
	local modell = 400
		for v = 1,#ID_Cars do
			if vr.list_cars[vr.current_car]['Car_Data']['ModelId'] <= ID_Cars[v] then
				modell = ID_Cars[v]
				break
			end
		end
	local xx,xy,xz = vr.list_cars[carr]['Car_Data']['Pos'][1], vr.list_cars[carr]['Car_Data']['Pos'][2], vr.list_cars[carr]['Car_Data']['Pos'][3]
	local angle = vr.list_cars[carr]['Car_Data']['Angle']

	requestModel(modell)
	while not hasModelLoaded(modell) do
		wait(0)
	end
	local last_id = 611
	for v = 1,#ID_Cars do
		if modell == ID_Cars[v] then
			last_id = ID_Cars[v-1]
			break
		end
	end
	markModelAsNoLongerNeeded(last_id)

	deleteCar(vr.list_cars[carr]['Car_Data']['Car'])
	vr.list_cars[carr]['Car_Data']['Car'] = createCar(modell, xx, xy, xz)
	setCarHeading(vr.list_cars[carr]['Car_Data']['Car'], angle)
	setCarCollision(vr.list_cars[carr]['Car_Data']['Car'],false)
	freezeCarPosition(vr.list_cars[carr]['Car_Data']['Car'], true)
	setCarCanBeDamaged(vr.list_cars[carr]['Car_Data']['Car'], false)
	changeCarColour(vr.list_cars[carr]['Car_Data']['Car'], vr.list_cars[carr]['Car_Data']['Color_primary'],vr.list_cars[carr]['Car_Data']['Color_secondary'])
end

function update_object(objj)
	local modell = vr.list_objects[objj]['Object_Data']['ModelId']
	local xx,xy,xz = vr.list_objects[objj]['Object_Data']['Pos'][1], vr.list_objects[objj]['Object_Data']['Pos'][2], vr.list_objects[objj]['Object_Data']['Pos'][3]
	local rxx,rxy,rxz = vr.list_objects[objj]['Object_Data']['Rotates'][1], vr.list_objects[objj]['Object_Data']['Rotates'][2], vr.list_objects[objj]['Object_Data']['Rotates'][3]

	requestModel(modell)
	while not hasModelLoaded(modell) do
		wait(0)
	end

	deleteObject(vr.list_objects[objj]['Object_Data']['Obj'])
	vr.list_objects[objj]['Object_Data']['Obj'] = createObject(modell, xx, xy, xz)
	setObjectCoordinates(vr.list_objects[objj]['Object_Data']['Obj'], xx, xy, xz)
	setObjectRotation(vr.list_objects[objj]['Object_Data']['Obj'], rxx, rxy, rxz)
end

function update_pickup(pickk)
	removePickup(vr.list_pickup[pickk]['Pickup_Data']['Pick'])
	wait(1)
	local xx,xy,xz = vr.list_pickup[pickk]['Pickup_Data']['Pos'][1],vr.list_pickup[pickk]['Pickup_Data']['Pos'][2],vr.list_pickup[pickk]['Pickup_Data']['Pos'][3]
	vr.list_pickup[pickk]['Pickup_Data']['Pos'][3] = getGroundZFor3dCoord(xx, xy, xz+1) + 1
	xz = vr.list_pickup[pickk]['Pickup_Data']['Pos'][3]

	if vr.list_pickup[pickk]['Pickup_Data']['Type_pickup'] == 1 then
		if not hasModelLoaded(getWeapontypeModel(vr.list_pickup[pickk]['Pickup_Data']['Weapon'])) then
			requestModel(getWeapontypeModel(vr.list_pickup[pickk]['Pickup_Data']['Weapon']))
			while not hasModelLoaded(getWeapontypeModel(vr.list_pickup[pickk]['Pickup_Data']['Weapon'])) do
				wait(0)
			end
		end
		vr.list_pickup[pickk]['Pickup_Data']['Pick'] = createPickupWithAmmo(getWeapontypeModel(vr.list_pickup[pickk]['Pickup_Data']['Weapon']),9,1,xx,xy,xz)
	elseif vr.list_pickup[pickk]['Pickup_Data']['Type_pickup'] == 2 then
		if not hasModelLoaded(1240) then
			requestModel(1240)
			while not hasModelLoaded(1240) do
				wait(0)
			end
		end
		vr.list_pickup[pickk]['Pickup_Data']['Pick'] = select(2,createPickup(1240,9,xx,xy,xz))
	elseif vr.list_pickup[pickk]['Pickup_Data']['Type_pickup'] == 3 then
		if not hasModelLoaded(1242) then
			requestModel(1242)
			while not hasModelLoaded(1242) do
				wait(0)
			end
		end
		vr.list_pickup[pickk]['Pickup_Data']['Pick'] = select(2,createPickup(1242,9,xx,xy,xz))
	elseif vr.list_pickup[pickk]['Pickup_Data']['Type_pickup'] == 4 then
		if not hasModelLoaded(1247) then
			requestModel(1247)
			while not hasModelLoaded(1247) do
				wait(0)
			end
		end
		vr.list_pickup[pickk]['Pickup_Data']['Pick'] = select(2,createPickup(1247,9,xx,xy,xz))
	elseif vr.list_pickup[pickk]['Pickup_Data']['Type_pickup'] == 5 then
		if not hasModelLoaded(1241) then
			requestModel(1241)
			while not hasModelLoaded(1241) do
				wait(0)
			end
		end
		vr.list_pickup[pickk]['Pickup_Data']['Pick'] = select(2,createPickup(1241,9,xx,xy,xz))
	elseif vr.list_pickup[pickk]['Pickup_Data']['Type_pickup'] == 6 then
		if not hasModelLoaded() then
			requestModel(vr.list_pickup[pickk]['Pickup_Data']['ModelId'])
			while not hasModelLoaded(vr.list_pickup[pickk]['Pickup_Data']['ModelId']) do
				wait(0)
			end
		end
		vr.list_pickup[pickk]['Pickup_Data']['Pick'] = select(2,createPickup(vr.list_pickup[pickk]['Pickup_Data']['ModelId'],9,xx,xy,xz))
	end
end

function update_explosion(expl)
	local xx,xy,xz = vr.list_explosion[expl]['Explosion_Data']['Pos'][1],vr.list_explosion[expl]['Explosion_Data']['Pos'][2],vr.list_explosion[expl]['Explosion_Data']['Pos'][3]

	if vr.list_explosion[expl]['Explosion_Data']['Type'] == 2 then
		if not doesObjectExist(vr.list_explosion[expl]['Explosion_Data']['Explosion']) then
			if not hasModelLoaded(1654) then
				requestModel(1654)
				while not hasModelLoaded(1654) do
					wait(0)
				end
			end
			vr.list_explosion[expl]['Explosion_Data']['Explosion'] = createObject(1654,xx,xy,xz)
			setObjectCollision(vr.list_explosion[expl]['Explosion_Data']['Explosion'],false)
		end
		setObjectCoordinates(vr.list_explosion[expl]['Explosion_Data']['Explosion'],xx,xy,xz)
	elseif vr.list_explosion[expl]['Explosion_Data']['Type'] == 1 then
		removeScriptFire(vr.list_explosion[expl]['Explosion_Data']['Fire'])
		wait(0)
		vr.list_explosion[expl]['Explosion_Data']['Fire'] = startScriptFire(xx,xy,xz,0,vr.list_explosion[expl]['Explosion_Data']['Size_fire'])
	end
end

function update_particle(prtcl)
	local modell = Particle_name[vr.list_particle[prtcl]['Particle_Data']['ModelId']]
	local xx,xy,xz = vr.list_particle[prtcl]['Particle_Data']['Pos'][1], vr.list_particle[prtcl]['Particle_Data']['Pos'][2], vr.list_particle[prtcl]['Particle_Data']['Pos'][3]
	local rxx,rxy,rxz = vr.list_particle[prtcl]['Particle_Data']['Rotates'][1], vr.list_particle[prtcl]['Particle_Data']['Rotates'][2], vr.list_particle[prtcl]['Particle_Data']['Rotates'][3]

	killFxSystem(vr.list_particle[prtcl]['Particle_Data']['Prtcl'])
	wait(0)
	vr.list_particle[prtcl]['Particle_Data']['Prtcl'] = createFxSystem(modell, xx, xy, xz, 1)
	playFxSystem(vr.list_particle[prtcl]['Particle_Data']['Prtcl'])
end

function load_mission()
	for i = 1,#vr.list_actors do
		deleteChar(vr.list_actors[i]['Actor_Data']['Char'])
	end
	for c = 1,#vr.list_cars do
		deleteCar(vr.list_cars[c]['Car_Data']['Car'])
	end
	for o = 1,#vr.list_objects do
		deleteObject(vr.list_objects[o]['Object_Data']['Obj'])
	end
	for p = 1,#vr.list_pickup do
		removePickup(vr.list_pickup[p]['Pickup_Data']['Pick'])
	end
	for e = 1,#vr.list_explosion do
		removeScriptFire(vr.list_explosion[e]['Explosion_Data']['Fire'])
		deleteObject(vr.list_explosion[e]['Explosion_Data']['Explosion'])
	end
	for p = 1,#vr.list_particle do
		removePickup(vr.list_pickup[p]['Pickup_Data']['Prtcl'])
	end
	vr.list_targets = vr.list_missions[vr.current_missions]['Mission_Data']['Targets']
	vr.list_actors = vr.list_missions[vr.current_missions]['Mission_Data']['Actors']
	vr.list_cars = vr.list_missions[vr.current_missions]['Mission_Data']['Cars']
	vr.list_objects = vr.list_missions[vr.current_missions]['Mission_Data']['Objects']
	vr.list_pickup = vr.list_missions[vr.current_missions]['Mission_Data']['Pickup']
	vr.list_particle = vr.list_missions[vr.current_missions]['Mission_Data']['Particle']
	vr.list_explosion = vr.list_missions[vr.current_missions]['Mission_Data']['Explosion']
	vr.mission_data = vr.list_missions[vr.current_missions]['Mission_Data']['Miss_data']
	vr.Group_relationships = vr.list_missions[vr.current_missions]['Mission_Data']['Group_relationships']
	vr.Fast_data['CurMiss'] = vr.current_missions
	vr.list_name_actors,vr.list_name_objects,vr.list_name_cars,vr.list_name_pickup,vr.list_name_particle,vr.list_name_explosion = {},{},{},{},{},{}
	for a = 1, #vr.list_actors do
		wait(0)
		vr.list_name_actors[a] = vr.list_actors[a]['Name']
		update_actor(a)
	end
	for o = 1, #vr.list_objects do
		vr.list_name_objects[o] = vr.list_objects[o]['Name']
		update_object(o)
	end
	for c = 1, #vr.list_cars do
		wait(0)
		vr.list_name_cars[c] = vr.list_cars[c]['Name']
		update_car(c)
	end
	for p = 1, #vr.list_pickup do
		wait(0)
		vr.list_name_pickup[p] = vr.list_pickup[p]['Name']
		update_pickup(p)
	end
	for p = 1, #vr.list_particle do
		wait(0)
		vr.list_name_particle[p] = vr.list_particle[p]['Name']
		update_particle(p)
	end
	for e = 1, #vr.list_explosion do
		wait(0)
		vr.list_name_explosion[e] = vr.list_explosion[e]['Name']
		update_explosion(e)
	end
	for t = 1, #vr.list_targets do
		wait(0)
		vr.list_name_targets[t] = vr.list_targets[t]['Name']
	end
	setCharCoordinates(PLAYER_PED, vr.mission_data['Player']['Pos'][1], vr.mission_data['Player']['Pos'][2], vr.mission_data['Player']['Pos'][3])
	setCharInterior(PLAYER_PED, vr.mission_data['Player']['Interior_id'])
	setInteriorVisible(vr.mission_data['Player']['Interior_id'])
end


function table.merge(t1,t2)
	for _,k in ipairs(t2) do
		t1[#t1+1] = k
	end
end

function onStartNewGame(missionPackNumber)
	vr.mpack = missionPackNumber
	if missionPackNumber ~= 7 then
		thisScript():pause()
	else
		thisScript():resume()
	end
end

function main()
	local tl = {}
	for line in io.popen("dir \""..getWorkingDirectory() .. '\\Language'.."\" /a:-d /b", "r"):lines() do
		if string.sub(line, -3) == 'ini' then
			LanguageArr[#LanguageArr+1] = inicfg.load(nil,getWorkingDirectory() .. '\\Language\\'..line)
			print(getWorkingDirectory() .. '\\Language\\'..line)
			LanguageList[#LanguageList+1] = LanguageArr[#LanguageArr]['Info']['name']
		end
	end

	vr.Data = inicfg.load(nil,getWorkingDirectory()..'\\LDYOM_data.ini')
	for k,v in pairs(decodeJson(vr.Data.Data.ID_Spec_Actors)) do table.insert(ID_Spec_Actors, v) end

	lua_thread.create(timer)

	langt = LanguageArr[vr.Data.Settings.curr_lang]['Keys']

	setGxtEntry('HTARG', koder(u8:decode(langt['HTARG'])))
	setGxtEntry('HACT', koder(u8:decode(langt['HACT'])))
	setGxtEntry('HOBJ', koder(u8:decode(langt['HOBJ'])))
	setGxtEntry('HMTIM', koder(u8:decode(langt['HMTIM'])))
	setGxtEntry('HPLA', koder(u8:decode(langt['HPLA'])))
	setGxtEntry('HPLA2', koder(u8:decode(langt['HPLA2'])))
	setGxtEntry('HVIEW',koder(u8:decode(langt['HVIEW'])))
	setGxtEntry('HPICW', koder(u8:decode(langt['HPICW'])))
	setGxtEntry('AAAP',koder(u8:decode(langt['AAAP'])))
	local info_t = decodeJson(u8:decode(langt['info_t']))
	info_t = info_t[1]..'~r~ '..thisScript().authors[1]..'~w~~n~'..info_t[2]..'~r~ '..thisScript().authors[2]..'~w~~n~'..info_t[3]..' '..thisScript().version..'~n~~n~'..info_t[4]..'~n~'..info_t[5]..'~n~~n~'..info_t[6]
	setGxtEntry('INFOLD',koder(info_t))

	Weather_str = decodeJson(langt['Weather_arr'])

	vr.Group_relationships_list_items = {}
	vr['Group_relationships'][1] = 'NULL'
	for i = 2,9 do
		vr['Group_relationships'][i] = {}
		for y = 1,9 do
			if i ~= y then
				vr['Group_relationships'][i][y] = 3
			else
				vr['Group_relationships'][i][y] = 'NULL'
			end
		end
		vr.Group_relationships_list_items[#vr.Group_relationships_list_items+1] = {'button',langt['group']..' '..i-1,i}
	end

	vr.list_mission_pack = manager.loadMPack()
	for mp = 1,#vr.list_mission_pack do
		vr.list_name_mission_pack[mp] = vr.list_mission_pack[mp]['Name']
	end
	print(vr.list_name_mission_pack[1])

	upd_actor = lua_thread.create_suspended(update_actor)
	upd_car = lua_thread.create_suspended(update_car)
	upd_anim_actor = lua_thread.create_suspended(update_actor_animation)
	upd_object = lua_thread.create_suspended(update_object)
	upd_pickup = lua_thread.create_suspended(update_pickup)
	upd_particle = lua_thread.create_suspended(update_particle)
	upd_explosion = lua_thread.create_suspended(update_explosion)

	local names = decodeJson(langt['main_menu_arr'])
	vr.main_menu_items = {
		{'button',names[1],1},
		{'button',names[2],2},
		{'button',names[3],3},
		{'button',names[4],4},
		{'button',names[5],5},
		{'button',names[6],6},
		{'button',names[7],7},
		{'button',names[8],8},
		{'button',names[9],9},
		{'button',names[10],10},
		{'button',names[11],11},
		{'button',names[12],12},
		{'button',names[13],13},
		{'button',names[14],14},
		{'button',names[14],15},
	}
	names = decodeJson(langt['actor_change_arr'])
	vr.actor_change_items = {
		{'button',names[1],1},
		{'button',names[2],2},
		{'button',names[3],3}
	}
	names = decodeJson(langt['actor_anims_change_arr'])
	vr.actor_anims_change_items = {
		{'button',names[1],1},
		{'button',names[2],2},
		{'button',names[3],3}
	}
	names = decodeJson(langt['targets_change_arr'])
	vr.targets_change_items = {
		{'button',names[1],1},
		{'button',names[2],2},
		{'button',names[3],3},
		{'button',names[4],4}
	}
	names = decodeJson(langt['cars_change_arr'])
	vr.cars_change_items = {
		{'button',names[1],1},
		{'button',names[2],2},
		{'button',names[3],3}
	}
	names = decodeJson(langt['objects_change_arr'])
	vr.objects_change_items = {
		{'button',names[1],1},
		{'button',names[2],2},
		{'button',names[3],3}
	}
	names = decodeJson(langt['particles_change_arr'])
	vr.particles_change_items = {
		{'button',names[1],1},
		{'button',names[2],2},
		{'button',names[3],3}
	}
	names = decodeJson(langt['pickup_change_arr'])
	vr.pickup_change_items = {
		{'button',names[1],1},
		{'button',names[2],2},
		{'button',names[3],3}
	}
	names = decodeJson(langt['explosion_change_arr'])
	vr.explosion_change_items = {
		{'button',names[1],1},
		{'button',names[2],2},
		{'button',names[3],3}
	}
	names = decodeJson(langt['mission_pack_change_arr'])
	vr.mission_pack_change_items = {
		{'button',names[1],1},
		{'button',names[2],2},
		{'button',names[3],3},
		{'button',names[4],4},
		{'button',names[5],5}
	}
	names = decodeJson(langt['missions_change_arr'])
	vr.missions_change_items = {
		{'button',names[1],1},
		{'button',names[2],2},
		{'button',names[3],3},
		{'button',names[4],4},
		{'button',names[5],5}
	}
	while true do
		wait(0)
		if vr.mpack == 7 then
			wait(500)
			local welcome_t = decodeJson((langt['welcome_t']))
			printHelpString(koder(u8:decode(welcome_t[1]) .. tostring(thisScript().version) .. u8:decode(welcome_t[2])))
			vr.mpack = nil
		end
		if vr.mpack ~= nil then
			break
		end

		if testCheat('top2009') then
			printHelpString('~r~I LOVE TWENTY ONE PILOTS')
		end

		openMenu = vr.main_menu_window or vr.actor_change_window or vr.actor_window or vr.actor_anim_window or vr.actor_anims_change_window or vr.actor_anims_list_window or vr.targets_change_window or vr.targets_list_window or vr.target_list_dialog_window or vr.target_dialog_window or vr.target_window or vr.objects_change_window or vr.objects_list_window or vr.object_window or vr.object_anims_change_window or vr.object_anims_list_window or vr.object_anim_window or vr.pickup_window or vr.pack_mission_list_window or vr.missions_settings_window

		for i = 1, #vr.list_targets do
			if vr.list_targets[i]['Type'] == 1 then
				drawSphere(vr.list_targets[i]['Target_Data']['Pos'][1], vr.list_targets[i]['Target_Data']['Pos'][2], vr.list_targets[i]['Target_Data']['Pos'][3],vr.list_targets[i]['Target_Data']['Radius'])
				end
		end

		--Редактор позиции объекта
		while vr.editmode_target do
			wait(0)
			--Закрытие редактора
			if wasKeyReleased(vkeys.VK_F) then
				clearHelp()
				vr.editmode_target = false
				vr.target_window = true
				openMenu = true
			end
			--Переменная замедления
			local multy = 1
			if isKeyDown(vkeys.VK_CONTROL) then
				multy = 0.5
			else
				multy = 1
			end
			--Управление
			if isKeyDown(vkeys.VK_A) then
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_D) then
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_W) then
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_S) then
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_SPACE) then
				wait(0)
				vr.list_targets[vr.id_target]['Target_Data']['Radius'] = vr.list_targets[vr.id_target]['Target_Data']['Radius'] + 0.1 * multy
			end
			if isKeyDown(vkeys.VK_SHIFT) then
				wait(0)
				vr.list_targets[vr.id_target]['Target_Data']['Radius'] = vr.list_targets[vr.id_target]['Target_Data']['Radius'] - 0.1 * multy
			end
			--Отрисовка в редакторе
			local xx,xy,xz = vr.list_targets[vr.id_target]['Target_Data']['Pos'][1],vr.list_targets[vr.id_target]['Target_Data']['Pos'][2],vr.list_targets[vr.id_target]['Target_Data']['Pos'][3]
			vr.list_targets[vr.id_target]['Target_Data']['Pos'][3] = getGroundZFor3dCoord(xx, xy, xz+2)
			drawSphere(xx, xy, xz,vr.list_targets[vr.id_target]['Target_Data']['Radius'])
		end

		if vr.editmode_actor then
			vr.actor_window = false
			openMenu = true
			lockPlayerControl(false)
			local stage = 0
			local modelId = vr.list_actors[vr.current_actor]['Actor_Data']['ModelId']
			local weapon = vr.list_actors[vr.current_actor]['Actor_Data']['Weapon']
			local ammo = vr.list_actors[vr.current_actor]['Actor_Data']['Ammo']
			if vr.list_actors[vr.current_actor]['Actor_Data']['Model_type'] == 1 then
				modell = ID_Actors[vr.list_actors[vr.current_actor]['Actor_Data']['ModelId']]
				requestModel(modell)
				while not hasModelLoaded(modell) do
					wait(0)
				end
			else
				local modell_n = ID_Spec_Actors[vr.list_actors[vr.current_actor]['Actor_Data']['ModelId']]
				loadSpecialCharacter(modell_n,vr.list_actors[vr.current_actor]['Actor_Data']['Slot_model'])
				while not hasSpecialCharacterLoaded(vr.list_actors[vr.current_actor]['Actor_Data']['Slot_model']) do
					wait(0)
				end
				modell = 290 + vr.list_actors[vr.current_actor]['Actor_Data']['Slot_model']-1
			end
			setPlayerModel(PLAYER_HANDLE, modell)
			local xx,xy,xz = vr.list_actors[vr.current_actor]['Actor_Data']['Pos'][1],vr.list_actors[vr.current_actor]['Actor_Data']['Pos'][2],vr.list_actors[vr.current_actor]['Actor_Data']['Pos'][3]
			setCharCoordinates(PLAYER_PED, xx, xy, xz)
			setCharHeading(PLAYER_PED, vr.list_actors[vr.current_actor]['Actor_Data']['Angle'])
			giveWeaponToChar(PLAYER_PED,ID_Weapons[weapon], 99999)
			setCurrentCharWeapon(PLAYER_PED,1)
			printHelpForever('HPLA')
			while vr.editmode_actor do
				--Закрытие редактора
				wait(0)
				if wasKeyReleased(vkeys.VK_F) then
					clearHelp()
					vr.editmode_actor = false
					setPlayerModel(PLAYER_HANDLE, model.NULL)
					local xx,xy,xz = getCharCoordinates(PLAYER_PED)
					local angle = getCharHeading(PLAYER_PED)
					xz = getGroundZFor3dCoord(xx,xy,xz)
					removeAllCharWeapons(PLAYER_PED)
					upd_actor:run(vr.current_actor)
					clearPrints()
					vr.actor_window = true
					lockPlayerControl(true)
				end
				if wasKeyPressed(vkeys.VK_G) then
					if stage == 0 then
						lockPlayerControl(true)
						printHelpForever('HPLA2')
						stage = stage + 1
					elseif stage == 1 then
						clearHelp()
						vr.editmode_actor = false
						setPlayerModel(PLAYER_HANDLE, model.NULL)
						local xx,xy,xz = getCharCoordinates(PLAYER_PED)
						local angle = getCharHeading(PLAYER_PED)
						xz = getGroundZFor3dCoord(xx,xy,xz)
						removeAllCharWeapons(PLAYER_PED)
						vr.list_actors[vr.current_actor]['Actor_Data']['Pos'] = {xx,xy,xz}
						vr.list_actors[vr.current_actor]['Actor_Data']['Angle'] = angle
						vr.list_actors[vr.current_actor]['Actor_Data']['ModelId'] = modelId
						vr.list_actors[vr.current_actor]['Actor_Data']['Weapon'] = weapon
						vr.list_actors[vr.current_actor]['Actor_Data']['Ammo'] = ammo
						upd_actor:run(vr.current_actor)
						clearPrints()
						vr.actor_window = true
						lockPlayerControl(true)
					end
				end
				if stage == 0 then
					if isKeyDown(vkeys.VK_I) then
						wait(100)
						print("df")
						local modell
						modelId = modelId + 1
						if vr.list_actors[vr.current_actor]['Actor_Data']['Model_type'] == 1 then
							if modelId > #ID_Actors then
								modelId = 1+modelId-#ID_Actors
							end
							modell = ID_Actors[modelId]
							requestModel(modell)
							while not hasModelLoaded(modell) do
								wait(0)
							end
						else
							if modelId > #ID_Spec_Actors then
								modelId = 1+modelId-#ID_Spec_Actors
							end

							local modell_n = ID_Spec_Actors[modelId]
							loadSpecialCharacter(modell_n,vr.list_actors[vr.current_actor]['Actor_Data']['Slot_model'])
							while not hasSpecialCharacterLoaded(vr.list_actors[vr.current_actor]['Actor_Data']['Slot_model']) do
								wait(0)
							end
							modell = 290 + vr.list_actors[vr.current_actor]['Actor_Data']['Slot_model']-1
						end
						setPlayerModel(PLAYER_HANDLE, modell)
					end
					if isKeyDown(vkeys.VK_O) then
						wait(100)
						local modell
						modelId = modelId - 1
						if vr.list_actors[vr.current_actor]['Actor_Data']['Model_type'] == 1 then
							if modelId < 1 then
								modelId = #ID_Actors+modelId
							end

							modell = ID_Actors[modelId]
							requestModel(modell)
							while not hasModelLoaded(modell) do
								wait(0)
							end
						else
							if modelId < 1 then
								modelId = #ID_Spec_Actors+modelId
							end
							local modell_n = ID_Spec_Actors[modelId]
							loadSpecialCharacter(modell_n,vr.list_actors[vr.current_actor]['Actor_Data']['Slot_model'])
							while not hasSpecialCharacterLoaded(vr.list_actors[vr.current_actor]['Actor_Data']['Slot_model']) do
								wait(0)
							end
							modell = 290 + vr.list_actors[vr.current_actor]['Actor_Data']['Slot_model']-1
						end
						setPlayerModel(PLAYER_HANDLE, modell)
					end
					if isKeyDown(vkeys.VK_K) then
						wait(100)
						local modell
						modelId = modelId + 10
						if vr.list_actors[vr.current_actor]['Actor_Data']['Model_type'] == 1 then
							if modelId > #ID_Actors then
								modelId = 1+modelId-#ID_Actors
							end
							modell = ID_Actors[modelId]
							requestModel(modell)
							while not hasModelLoaded(modell) do
								wait(0)
							end
						else
							if modelId > #ID_Spec_Actors then
								modelId = 1+modelId-#ID_Spec_Actors
							end

							local modell_n = ID_Spec_Actors[modelId]
							loadSpecialCharacter(modell_n,vr.list_actors[vr.current_actor]['Actor_Data']['Slot_model'])
							while not hasSpecialCharacterLoaded(vr.list_actors[vr.current_actor]['Actor_Data']['Slot_model']) do
								wait(0)
							end
							modell = 290 + vr.list_actors[vr.current_actor]['Actor_Data']['Slot_model']-1
						end
						setPlayerModel(PLAYER_HANDLE, modell)
					end
					if isKeyDown(vkeys.VK_L) then
						wait(100)
						local modell
						modelId = modelId - 10
						if vr.list_actors[vr.current_actor]['Actor_Data']['Model_type'] == 1 then
							if modelId < 1 then
								modelId = #ID_Actors+modelId
							end

							modell = ID_Actors[modelId]
							requestModel(modell)
							while not hasModelLoaded(modell) do
								wait(0)
							end
						else
							if modelId < 1 then
								modelId = #ID_Spec_Actors+modelId
							end
							local modell_n = ID_Spec_Actors[modelId]
							loadSpecialCharacter(modell_n,vr.list_actors[vr.current_actor]['Actor_Data']['Slot_model'])
							while not hasSpecialCharacterLoaded(vr.list_actors[vr.current_actor]['Actor_Data']['Slot_model']) do
								wait(0)
							end
							modell = 290 + vr.list_actors[vr.current_actor]['Actor_Data']['Slot_model']-1
						end
						setPlayerModel(PLAYER_HANDLE, modell)
					end
					if isKeyDown(vkeys.VK_P) then
						wait(200)
						weapon = weapon + 1
						if weapon > #ID_Weapons then
							weapon = 1
						end
						local id_w = ID_Weapons[weapon]
						requestModel(getWeapontypeModel(id_w))
						while not hasModelLoaded(getWeapontypeModel(id_w)) do
							wait(1)
						end
						removeAllCharWeapons(PLAYER_PED)
						giveWeaponToChar(PLAYER_PED, id_w, 9999)
						setCurrentCharWeapon(PLAYER_PED,1)
					end
					if isKeyDown(vkeys.VK_OEM_4) then
						wait(200)
						weapon = weapon - 1
						if weapon < 1 then
							weapon = #ID_Weapons
						end
						local id_w = ID_Weapons[weapon]
						requestModel(getWeapontypeModel(id_w))
						while not hasModelLoaded(getWeapontypeModel(id_w)) do
							wait(1)
						end
						removeAllCharWeapons(PLAYER_PED)
						giveWeaponToChar(PLAYER_PED, id_w, 9999)
						setCurrentCharWeapon(PLAYER_PED,1)
					end
					if wasKeyReleased(vkeys.VK_U) then
						if vr.list_actors[vr.current_actor]['Actor_Data']['Model_type'] == 1 then
							vr.list_actors[vr.current_actor]['Actor_Data']['Model_type'] = 2
						else
							vr.list_actors[vr.current_actor]['Actor_Data']['Model_type'] = 1
						end
						modelId = 1
						local modell
						if vr.list_actors[vr.current_actor]['Actor_Data']['Model_type'] == 1 then
							modell = ID_Actors[modelId]
							requestModel(modell)
							while not hasModelLoaded(modell) do
								wait(0)
							end
						else
							local modell_n = ID_Spec_Actors[modelId]
							loadSpecialCharacter(modell_n,10)
							while not hasSpecialCharacterLoaded(10) do
								wait(0)
							end
							modell = 290 + 10-1
						end
						setPlayerModel(PLAYER_HANDLE, modell)
					end
				elseif stage == 1 then
					if isKeyDown(vkeys.VK_I) then
						wait(50)
						ammo = ammo + 10
					end
					if isKeyDown(vkeys.VK_O) and ammo > 0 then
						wait(50)
						ammo = ammo - 10
					end
					printString(koder(u8:decode(langt['ammo'])..': ' .. '~r~' .. ammo), 0)
				end
			end
		end

		while vr.editmode_actor_add_point do
			wait(0)
			if wasKeyPressed(vkeys.VK_U) then
				local xx,xy,xz = getCharCoordinates(PLAYER_PED)
				table.insert(vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.id_actor_anim]['Path'],{xx,xy,xz})
				printString(koder(u8:decode(langt['added_point']))..#vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.id_actor_anim]['Path'],1000)
			end
			if wasKeyReleased(vkeys.VK_F) then
				clearHelp()
				vr.editmode_actor_add_point = false
				vr.actor_anim_window = true
				lockPlayerControl(true)
				openMenu = true
			end
		end

		while vr.editmode_objects do
			wait(0)
			--Закрытие редактора
			if wasKeyReleased(vkeys.VK_F) then
				lockPlayerControl(true)
				clearHelp()
				vr.editmode_objects = false
				openMenu = true
			end
			--Переменная замедления
			local multy = 1
			if isKeyDown(vkeys.VK_CONTROL) then
				multy = 0.2
			else
				multy = 1
			end
			--Управление
			if isKeyDown(vkeys.VK_A) and not isKeyDown(vkeys.VK_SHIFT) then
				vr.list_objects[vr.id_obj]['Object_Data']['Pos'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][2] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_D) and not isKeyDown(vkeys.VK_SHIFT) then
				vr.list_objects[vr.id_obj]['Object_Data']['Pos'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][2] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_W) and not isKeyDown(vkeys.VK_SHIFT) then
				vr.list_objects[vr.id_obj]['Object_Data']['Pos'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][1] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_S) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_objects[vr.id_obj]['Object_Data']['Pos'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][1] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_Q) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_objects[vr.id_obj]['Object_Data']['Pos'][3] = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][3] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_E) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_objects[vr.id_obj]['Object_Data']['Pos'][3] = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][3] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_SHIFT) then
				if isKeyDown(vkeys.VK_A) then
					vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][2] - 0.2 * multy
				end
				if isKeyDown(vkeys.VK_D) then
					vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][2] + 0.2 * multy
				end
				if isKeyDown(vkeys.VK_W) then
					vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][1] - 0.2 * multy
				end
				if isKeyDown(vkeys.VK_S) then
					vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][1] + 0.2 * multy
				end
				if isKeyDown(vkeys.VK_E) then
					vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][3] = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][3] - 0.2 * multy
				end
				if isKeyDown(vkeys.VK_Q) then
					vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][3] = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][3] + 0.2 * multy
				end
			end
			--Отрисовка в редакторе
			local xx,xy,xz = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][1],vr.list_objects[vr.id_obj]['Object_Data']['Pos'][2],vr.list_objects[vr.id_obj]['Object_Data']['Pos'][3]
			local rxx,rxy,rxz = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][1],vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][2],vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][3]
			setObjectCoordinates(vr.list_objects[vr.id_obj]['Object_Data']['Obj'], xx, xy, xz)
			setObjectRotation(vr.list_objects[vr.id_obj]['Object_Data']['Obj'], rxx, rxy, rxz)
		end

		while vr.editmode_objects_anim do
			wait(0)
			--Переменная замедления
			local multy = 1
			if isKeyDown(vkeys.VK_CONTROL) then
				multy = 0.2
			else
				multy = 1
			end
			--Управление
			if isKeyDown(vkeys.VK_A) and not isKeyDown(vkeys.VK_SHIFT) then
				vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][2] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_D) and not isKeyDown(vkeys.VK_SHIFT) then
				vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][2] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_W) and not isKeyDown(vkeys.VK_SHIFT) then
				vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][1] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_S) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][1] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_Q) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][3] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][3] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_E) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][3] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][3] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_SHIFT) then
				if isKeyDown(vkeys.VK_A) then
					vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][2] - 0.2 * multy
				end
				if isKeyDown(vkeys.VK_D) then
					vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][2] + 0.2 * multy
				end
				if isKeyDown(vkeys.VK_W) then
					vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][1] - 0.2 * multy
				end
				if isKeyDown(vkeys.VK_S) then
					vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][1] + 0.2 * multy
				end
				if isKeyDown(vkeys.VK_E) then
					vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][3] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][3] - 0.5 * multy
				end
				if isKeyDown(vkeys.VK_Q) then
					vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][3] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][3] + 0.5 * multy
				end
			end
			--Отрисовка в редакторе
			local xx,xy,xz = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][1],vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][2],vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][3]

			local rxx,rxy,rxz = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][1],vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][2],vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][3]

			setObjectCoordinates(vr.list_objects[vr.id_obj]['Object_Data']['Obj'], xx, xy, xz)
			setObjectRotation(vr.list_objects[vr.id_obj]['Object_Data']['Obj'], rxx, rxy, rxz)

			--Закрытие редактора
			if wasKeyReleased(vkeys.VK_F) then
				lockPlayerControl(true)
				clearHelp()
				local xx,xy,xz = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][1],vr.list_objects[vr.id_obj]['Object_Data']['Pos'][2],vr.list_objects[vr.id_obj]['Object_Data']['Pos'][3]
				local rxx,rxy,rxz = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][1],vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][2],vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][3]
				setObjectCoordinates(vr.list_objects[vr.id_obj]['Object_Data']['Obj'], xx, xy, xz)
				setObjectRotation(vr.list_objects[vr.id_obj]['Object_Data']['Obj'], rxx, rxy, rxz)
				vr.editmode_objects_anim = false
				openMenu = true
			end
		end

		while vr.editmode_particle do
			wait(0)
			--Закрытие редактора
			if wasKeyReleased(vkeys.VK_F) then
				lockPlayerControl(true)
				clearHelp()
				vr.editmode_particle = false
			end
			--Переменная замедления
			local multy = 1
			if isKeyDown(vkeys.VK_CONTROL) then
				multy = 0.2
			else
				multy = 1
			end
			--Управление
			if isKeyDown(vkeys.VK_A) and not isKeyDown(vkeys.VK_SHIFT) then
				wait(100)
				vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][2] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][2] + 0.2 * multy
				upd_particle:run(vr.id_prtcl)
			end
			if isKeyDown(vkeys.VK_D) and not isKeyDown(vkeys.VK_SHIFT) then
				wait(100)
				vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][2] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][2] - 0.2 * multy
				upd_particle:run(vr.id_prtcl)
			end
			if isKeyDown(vkeys.VK_W) and not isKeyDown(vkeys.VK_SHIFT) then
				wait(100)
				vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][1] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][1] + 0.2 * multy
				upd_particle:run(vr.id_prtcl)
			end
			if isKeyDown(vkeys.VK_S) and not isKeyDown(vkeys.VK_SHIFT)  then
				wait(100)
				vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][1] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][1] - 0.2 * multy
				upd_particle:run(vr.id_prtcl)
			end
			if isKeyDown(vkeys.VK_Q) and not isKeyDown(vkeys.VK_SHIFT)  then
				wait(100)
				vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][3] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][3] + 0.2 * multy
				upd_particle:run(vr.id_prtcl)
			end
			if isKeyDown(vkeys.VK_E) and not isKeyDown(vkeys.VK_SHIFT)  then
				wait(100)
				vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][3] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][3] - 0.2 * multy
				upd_particle:run(vr.id_prtcl)
			end
			if isKeyDown(vkeys.VK_SHIFT) then
				if isKeyDown(vkeys.VK_A) then
					wait(100)
					vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][2] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][2] + 0.2 * multy
					upd_particle:run(vr.id_prtcl)
				end
				if isKeyDown(vkeys.VK_D) then
					wait(100)
					vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][2] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][2] - 0.2 * multy
					upd_particle:run(vr.id_prtcl)
				end
				if isKeyDown(vkeys.VK_W) then
					wait(100)
					vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][1] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][1] + 0.2 * multy
					upd_particle:run(vr.id_prtcl)
				end
				if isKeyDown(vkeys.VK_S) then
					vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][1] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][1] - 0.2 * multy
					upd_particle:run(vr.id_prtcl)
				end
				if isKeyDown(vkeys.VK_E) then
					wait(100)
					vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][3] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][3] + 0.2 * multy
					upd_particle:run(vr.id_prtcl)
				end
				if isKeyDown(vkeys.VK_Q) then
					wait(100)
					vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][3] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][3] - 0.2 * multy
					upd_particle:run(vr.id_prtcl)
				end
			end
		end

		while vr.editmode_pickup do
			wait(0)
			--Закрытие редактора
			if wasKeyReleased(vkeys.VK_F) then
				clearHelp()
				vr.editmode_pickup = false
			end
			--Переменная замедления
			local multy = 1
			if isKeyDown(vkeys.VK_CONTROL) then
				multy = 0.5
			else
				multy = 1
			end
			--Управление
			if isKeyDown(vkeys.VK_A) then
				wait(50)
				vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][2] = vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][2] - 0.2 * multy
				upd_pickup:run(vr.id_pick)
			end
			if isKeyDown(vkeys.VK_D) then
				wait(50)
				vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][2] = vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][2] + 0.2 * multy
				upd_pickup:run(vr.id_pick)
			end
			if isKeyDown(vkeys.VK_W) then
				wait(50)
				vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][1] = vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][1] - 0.2 * multy
				upd_pickup:run(vr.id_pick)
			end
			if isKeyDown(vkeys.VK_S) then
				wait(50)
				vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][1] = vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][1] + 0.2 * multy
				upd_pickup:run(vr.id_pick)
			end
			if isKeyDown(vkeys.VK_I) then
				wait(100)
				if vr.list_pickup[vr.id_pick]['Pickup_Data']['Type_pickup'] == 1 then
					vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'] = vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'] + 1
					local id_w = 1
					for v = 1,#Number_Weapons do
						if vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'] <= Number_Weapons[v] then
							id_w = Number_Weapons[v]
							markModelAsNoLongerNeeded(getWeapontypeModel(Number_Weapons[v-1]))
							break
						end
					end
					vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'] = id_w
					upd_pickup:run(vr.id_pick)
				end
			end
			if isKeyDown(vkeys.VK_O) then
				wait(100)
				if 	vr.list_pickup[vr.id_pick]['Pickup_Data']['Type_pickup'] == 1 then
					vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'] = vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'] - 1
					if vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'] < 1 then
						vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'] = 46
					end
					local id_w = 46
					for v = 1,#Number_Weapons do
						if vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'] < Number_Weapons[v] then
							id_w = Number_Weapons[v-1]
							markModelAsNoLongerNeeded(getWeapontypeModel(Number_Weapons[v]))
							break
						end
					end
					vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'] = id_w
				end
				upd_pickup:run(vr.id_pick)
			end
		end

		while vr.editmode_explosion do
			wait(0)
			--Закрытие редактора
			if wasKeyReleased(vkeys.VK_F) then
				clearHelp()
				vr.editmode_explosion = false
				openMenu = true
			end
			--Переменная замедления
			local multy = 1
			if isKeyDown(vkeys.VK_CONTROL) then
				multy = 0.5
			else
				multy = 1
			end
			--Управление
			if isKeyDown(vkeys.VK_A) then
				vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][2] = vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][2] + 0.2 * multy
				upd_explosion:run(vr.id_explosion)
			end
			if isKeyDown(vkeys.VK_D) then
				vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][2] = vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][2] - 0.2 * multy
				upd_explosion:run(vr.id_explosion)
			end
			if isKeyDown(vkeys.VK_W) then
				vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][1] = vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][1] + 0.2 * multy
				upd_explosion:run(vr.id_explosion)
			end
			if isKeyDown(vkeys.VK_S) then
				vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][1] = vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][1] - 0.2 * multy
				upd_explosion:run(vr.id_explosion)
			end
			if isKeyDown(vkeys.VK_Q) then
				vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][3] = vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][3] + 0.2 * multy
				upd_explosion:run(vr.id_explosion)
			end
			if isKeyDown(vkeys.VK_E) then
				vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][3] = vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][3] - 0.2 * multy
				upd_explosion:run(vr.id_explosion)
			end
		end

		if vr.editmode_camera then
			displayRadar(false)
			displayHud(false)
		end
		while vr.editmode_camera do
			wait(0)
			--Переменная замедления
			local multy = 1
			if isKeyDown(vkeys.VK_CONTROL) then
				multy = 0.5
			else
				multy = 1
			end

			if isKeyDown(vkeys.VK_A) and not isKeyDown(vkeys.VK_SHIFT) then
				local sinn = math.sin(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2]+90))
				local coss = math.cos(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2]+90))
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] - 0.2 *sinn * multy
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] - 0.2 *coss * multy
			end
			if isKeyDown(vkeys.VK_D) and not isKeyDown(vkeys.VK_SHIFT) then
				local sinn = math.sin(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2]-90))
				local coss = math.cos(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2]-90))
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] - 0.2 *sinn * multy
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] - 0.2 *coss * multy
			end
			if isKeyDown(vkeys.VK_W) and not isKeyDown(vkeys.VK_SHIFT) then
				local sinn = math.sin(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2]))
				local coss = math.cos(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2]))
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] + 0.2 *sinn * multy
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] + 0.2 *coss * multy
			end
			if isKeyDown(vkeys.VK_S) and not isKeyDown(vkeys.VK_SHIFT)  then
				local sinn = math.sin(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2]))
				local coss = math.cos(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2]))
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] - 0.2 *sinn * multy
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] - 0.2 *coss * multy
			end
			if isKeyDown(vkeys.VK_Q) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][3] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][3] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_E) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][3] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][3] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_SHIFT) then
				if isKeyDown(vkeys.VK_A) then
					vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2] = vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2] - 2 * multy
				end
				if isKeyDown(vkeys.VK_D) then
					vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2] = vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2] + 2 * multy
				end
				if isKeyDown(vkeys.VK_W) then
					vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1] = vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1] - 2 * multy
				end
				if isKeyDown(vkeys.VK_S) then
					vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1] = vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1] + 2 * multy
				end
				if isKeyDown(vkeys.VK_E) then
					vr.list_targets[vr.id_target]['Target_Data']['Rotates'][3] = vr.list_targets[vr.id_target]['Target_Data']['Rotates'][3] - 2 * multy
				end
				if isKeyDown(vkeys.VK_Q) then
					vr.list_targets[vr.id_target]['Target_Data']['Rotates'][3] = vr.list_targets[vr.id_target]['Target_Data']['Rotates'][3] + 2 * multy
				end
			end

			local xx,xy,xz = vr.list_targets[vr.id_target]['Target_Data']['Pos'][1],vr.list_targets[vr.id_target]['Target_Data']['Pos'][2],vr.list_targets[vr.id_target]['Target_Data']['Pos'][3]
			local rxx,rxy,rxz = vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1],vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2],vr.list_targets[vr.id_target]['Target_Data']['Rotates'][3]
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
				vr.editmode_camera = false
				displayRadar(true)
				displayHud(true)
				openMenu = true
			end
		end

		while vr.editmode_timemiss do
			wait(0)
			--Закрытие редактора
			if wasKeyReleased(vkeys.VK_F) then
				lockPlayerControl(true)
				clearHelp()
				vr.editmode_timemiss = false
			end
			if isKeyDown(vkeys.VK_I) then
				wait(100)
				vr.mission_data['Time'][1] = vr.mission_data['Time'][1] + 1
			end
			if isKeyDown(vkeys.VK_O) then
				wait(100)
				vr.mission_data['Time'][1] = vr.mission_data['Time'][1] - 1
			end
			if isKeyDown(vkeys.VK_K) then
				wait(100)
				vr.mission_data['Time'][2] = vr.mission_data['Time'][2] + 1
			end
			if isKeyDown(vkeys.VK_L) then
				wait(100)
				vr.mission_data['Time'][2] = vr.mission_data['Time'][2] - 1
			end
			if vr.mission_data['Time'][2] == 60 then
				vr.mission_data['Time'][1] = vr.mission_data['Time'][1] + 1
				vr.mission_data['Time'][2] = 0
			elseif vr.mission_data['Time'][2] == -1 then
				vr.mission_data['Time'][1] = vr.mission_data['Time'][1] - 1
				vr.mission_data['Time'][2] = 59
			end
			if vr.mission_data['Time'][1] == 24 then
				vr.mission_data['Time'][1] = 0
			elseif vr.mission_data['Time'][1] == -1 then
				vr.mission_data['Time'][1] = 23
			end
			setTimeOfDay(vr.mission_data['Time'][1], vr.mission_data['Time'][2])
		end

		while vr.editmode_timetarg do
			wait(0)
			--Закрытие редактора
			if wasKeyReleased(vkeys.VK_F) then
				clearHelp()
				vr.editmode_timetarg = false
				openMenu = true
			end
			if isKeyDown(vkeys.VK_I) then
				wait(100)
				vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][1] = vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][1] + 1
			end
			if isKeyDown(vkeys.VK_O) then
				wait(100)
				vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][1] = vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][1] - 1
			end
			if isKeyDown(vkeys.VK_K) then
				wait(100)
				vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][2] = vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][2] + 1
			end
			if isKeyDown(vkeys.VK_L) then
				wait(100)
				vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][2] = vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][2] - 1
			end
			if vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][2] == 60 then
				vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][1] = vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][1] + 1
				vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][2] = 0
			elseif vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][2] == -1 then
				vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][1] = vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][1] - 1
				vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][2] = 59
			end
			if vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][1] == 24 then
				vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][1] = 0
			elseif vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][1] == -1 then
				vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][1] = 23
			end
			setTimeOfDay(vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][1], vr.list_targets[vr.id_timetarg]['Target_Data']['Clock_time'][2])
		end
		if vr.editmode_player then
			local stage = 0
			local modelId = 1
			local weapon = 1
			local ammo = 0
			local ped_type = true
			while vr.editmode_player do
				--Закрытие редактора
				wait(0)
				if wasKeyReleased(vkeys.VK_F) then
					clearHelp()
					vr.editmode_player = false
					setPlayerModel(PLAYER_HANDLE, model.NULL)
					removeAllCharWeapons(PLAYER_PED)
					lockPlayerControl(true)
				end
				if wasKeyPressed(vkeys.VK_G) then
					if stage == 0 then
						lockPlayerControl(true)
						printHelpForever('HPLA2')
						stage = stage + 1
					elseif stage == 1 then
						clearHelp()
						vr.editmode_player = false
						setPlayerModel(PLAYER_HANDLE, model.NULL)
						local xx,xy,xz = getCharCoordinates(PLAYER_PED)
						local angle = getCharHeading(PLAYER_PED)
						xz = getGroundZFor3dCoord(xx,xy,xz)
						vr.mission_data['Player']['Pos'] = {xx,xy,xz}
						vr.mission_data['Player']['Angle'] = angle
						vr.mission_data['Player']['ModelId'] = modelId
						vr.mission_data['Player']['Weapon'] = weapon
						vr.mission_data['Player']['Weap_ammo'] = ammo
						vr.mission_data['Player']['Interior_id'] = getActiveInterior()
						vr.mission_data['Player']['ped_type'] = ped_type
						removeAllCharWeapons(PLAYER_PED)
						lockPlayerControl(true)
					end
				end
				if stage == 0 then
					if isKeyDown(vkeys.VK_I) then
						wait(100)
						local modell
						modelId = modelId + 1
						if ped_type then
							if modelId > #ID_Actors then
								modelId = 1+modelId-#ID_Actors
							end
							modell = ID_Actors[modelId]
							requestModel(modell)
							while not hasModelLoaded(modell) do
								wait(0)
							end
						else
							if modelId > #ID_Spec_Actors then
								modelId = 1+modelId-#ID_Spec_Actors
							end
						
							local modell_n = ID_Spec_Actors[modelId]
							loadSpecialCharacter(modell_n,10)
							while not hasSpecialCharacterLoaded(10) do
								wait(0)
							end
							modell = 290 + 10-1
						end
						setPlayerModel(PLAYER_HANDLE, modell)
					end
					if isKeyDown(vkeys.VK_O) then
						wait(100)
						local modell
						modelId = modelId - 1
						if ped_type then
							if modelId < 1 then
								modelId = #ID_Actors-modelId
							end
							modell = ID_Actors[modelId]
							requestModel(modell)
							while not hasModelLoaded(modell) do
								wait(0)
							end
						else
							if modelId < 1 then
								modelId = #ID_Spec_Actors-modelId
							end
						
							local modell_n = ID_Spec_Actors[modelId]
							loadSpecialCharacter(modell_n,10)
							while not hasSpecialCharacterLoaded(10) do
								wait(0)
							end
							modell = 290 + 10-1
						end
						setPlayerModel(PLAYER_HANDLE, modell)
					end
					if isKeyDown(vkeys.VK_K) then
						wait(100)
						local modell
						modelId = modelId + 10
						if ped_type then
							if modelId > #ID_Actors then
								modelId = 1+modelId-#ID_Actors
							end
							modell = ID_Actors[modelId]
							requestModel(modell)
							while not hasModelLoaded(modell) do
								wait(0)
							end
						else
							if modelId > #ID_Spec_Actors then
								modelId = 1+modelId-#ID_Spec_Actors
							end
						
							local modell_n = ID_Spec_Actors[modelId]
							loadSpecialCharacter(modell_n,10)
							while not hasSpecialCharacterLoaded(10) do
								wait(0)
							end
							modell = 290 + 10-1
						end
						setPlayerModel(PLAYER_HANDLE, modell)
					end
					if isKeyDown(vkeys.VK_L) then
						wait(100)
						local modell
						modelId = modelId - 10
						if ped_type then
							if modelId < 1 then
								modelId = #ID_Actors-modelId
							end
							modell = ID_Actors[modelId]
							requestModel(modell)
							while not hasModelLoaded(modell) do
								wait(0)
							end
						else
							if modelId < 1 then
								modelId = #ID_Spec_Actors-modelId
							end
						
							local modell_n = ID_Spec_Actors[modelId]
							loadSpecialCharacter(modell_n,10)
							while not hasSpecialCharacterLoaded(10) do
								wait(0)
							end
							modell = 290 + 10-1
						end
						setPlayerModel(PLAYER_HANDLE, modell)
					end
					if isKeyDown(vkeys.VK_P) then
						wait(200)
						weapon = weapon + 1
						if weapon > #ID_Weapons then
							weapon = 1
						end
						local id_w = ID_Weapons[weapon]
						requestModel(getWeapontypeModel(id_w))
						while not hasModelLoaded(getWeapontypeModel(id_w)) do
							wait(1)
						end
						removeAllCharWeapons(PLAYER_PED)
						giveWeaponToChar(PLAYER_PED, id_w, 9999)
						setCurrentCharWeapon(PLAYER_PED,1)
					end
					if isKeyDown(vkeys.VK_OEM_4) then
						wait(200)
						weapon = weapon - 1
						if weapon < 1 then
							weapon = #ID_Weapons
						end
						local id_w = ID_Weapons[weapon]
						requestModel(getWeapontypeModel(id_w))
						while not hasModelLoaded(getWeapontypeModel(id_w)) do
							wait(1)
						end
						removeAllCharWeapons(PLAYER_PED)
						giveWeaponToChar(PLAYER_PED, id_w, 9999)
						setCurrentCharWeapon(PLAYER_PED,1)
					end
					if wasKeyPressed(vkeys.VK_U) then
						ped_type = not ped_type
						modelId = 1
						local modell
						if ped_type then
							modell = ID_Actors[modelId]
							requestModel(modell)
							while not hasModelLoaded(modell) do
								wait(0)
							end
						else
							local modell_n = ID_Spec_Actors[modelId]
							loadSpecialCharacter(modell_n,10)
							while not hasSpecialCharacterLoaded(10) do
								wait(0)
							end
							modell = 290 + 10-1
						end
						setPlayerModel(PLAYER_HANDLE, modell)
					end
				elseif stage == 1 then
					if isKeyDown(vkeys.VK_I) then
						wait(50)
						ammo = ammo + 10
					end
					if isKeyDown(vkeys.VK_O) and ammo > 0 then
						wait(50)
						ammo = ammo - 10
					end
					printString(koder(u8:decode(langt['ammo'])..': ' .. '~r~' .. ammo), 0)
				end
			end
		end

		if vr.editmode_teleport_player then
			local stage = 0
			local modelId = vr.list_targets[vr.id_teleport_player]['Target_Data']['ModelId']
			local weapon = vr.list_targets[vr.id_teleport_player]['Target_Data']['Weapon']
			local ammo = vr.list_targets[vr.id_teleport_player]['Target_Data']['Weap_ammo']
			setCharCoordinates(PLAYER_PED, vr.list_targets[vr.id_teleport_player]['Target_Data']['Pos'][1], vr.list_targets[vr.id_teleport_player]['Target_Data']['Pos'][2], vr.list_targets[vr.id_teleport_player]['Target_Data']['Pos'][3])
			local modell
			if vr.list_targets[vr.current_target]['Target_Data']['Model_type'] == 1 then
				if modelId > #ID_Actors then
					modelId = 1+modelId-#ID_Actors
				end
				modell = ID_Actors[modelId]
				requestModel(modell)
				while not hasModelLoaded(modell) do
					wait(0)
				end
			else
				if modelId > #ID_Spec_Actors then
					modelId = 1+modelId-#ID_Spec_Actors
				end
			
				local modell_n = ID_Spec_Actors[modelId]
				loadSpecialCharacter(modell_n,10)
				while not hasSpecialCharacterLoaded(10) do
					wait(0)
				end
				modell = 290 + 10-1
			end
			setPlayerModel(PLAYER_HANDLE, modell)
			setCharHeading(PLAYER_PED, vr.list_targets[vr.id_teleport_player]['Target_Data']['Angle'])
			giveWeaponToChar(PLAYER_PED,ID_Weapons[weapon], 99999)
			setInteriorVisible(vr.list_targets[vr.id_teleport_player]['Target_Data']['Interior_id'])
			setCharInterior(PLAYER_PED, vr.list_targets[vr.id_teleport_player]['Target_Data']['Interior_id'])
			while vr.editmode_teleport_player do
				--Закрытие редактора
				wait(0)
				if wasKeyReleased(vkeys.VK_F) then
					clearHelp()
					vr.editmode_teleport_player = false
					setPlayerModel(PLAYER_HANDLE, model.NULL)
					removeAllCharWeapons(PLAYER_PED)
					lockPlayerControl(true)
				end
				if wasKeyReleased(vkeys.VK_G) then
					if stage == 0 then
						lockPlayerControl(true)
						printHelpForever('HPLA2')
						stage = stage + 1
					elseif stage == 1 then
						clearHelp()
						vr.editmode_teleport_player = false
						setPlayerModel(PLAYER_HANDLE, model.NULL)
						local xx,xy,xz = getCharCoordinates(PLAYER_PED)
						local angle = getCharHeading(PLAYER_PED)
						xz = getGroundZFor3dCoord(xx,xy,xz)
						vr.list_targets[vr.id_teleport_player]['Target_Data']['Pos'] = {xx,xy,xz}
						vr.list_targets[vr.id_teleport_player]['Target_Data']['Angle'] = angle
						vr.list_targets[vr.id_teleport_player]['Target_Data']['ModelId'] = modelId
						vr.list_targets[vr.id_teleport_player]['Target_Data']['Weapon'] = weapon
						vr.list_targets[vr.id_teleport_player]['Target_Data']['Weap_ammo'] = ammo
						vr.list_targets[vr.id_teleport_player]['Target_Data']['Interior_id'] = getActiveInterior()
						removeAllCharWeapons(PLAYER_PED)
						openMenu = true
					end
				end
				if stage == 0 then
					if isKeyDown(vkeys.VK_I) then
						wait(100)
						local modell
						modelId = modelId + 1
						if vr.list_targets[vr.current_target]['Target_Data']['Model_type'] == 1 then
							if modelId > #ID_Actors then
								modelId = 1+modelId-#ID_Actors
							end
							modell = ID_Actors[modelId]
							requestModel(modell)
							while not hasModelLoaded(modell) do
								wait(0)
							end
						else
							if modelId > #ID_Spec_Actors then
								modelId = 1+modelId-#ID_Spec_Actors
							end
						
							local modell_n = ID_Spec_Actors[modelId]
							loadSpecialCharacter(modell_n,10)
							while not hasSpecialCharacterLoaded(10) do
								wait(0)
							end
							modell = 290 + 10-1
						end
						setPlayerModel(PLAYER_HANDLE, modell)
					end
					if isKeyDown(vkeys.VK_O) then
						wait(100)
						local modell
						modelId = modelId - 1
						if vr.list_targets[vr.current_target]['Target_Data']['Model_type'] == 1 then
							if modelId < 1 then
								modelId = #ID_Actors-modelId
							end
							modell = ID_Actors[modelId]
							requestModel(modell)
							while not hasModelLoaded(modell) do
								wait(0)
							end
						else
							if modelId < 1 then
								modelId = #ID_Spec_Actors-modelId
							end
						
							local modell_n = ID_Spec_Actors[modelId]
							loadSpecialCharacter(modell_n,10)
							while not hasSpecialCharacterLoaded(10) do
								wait(0)
							end
							modell = 290 + 10-1
						end
						setPlayerModel(PLAYER_HANDLE, modell)
					end
					if isKeyDown(vkeys.VK_K) then
						wait(100)
						local modell
						modelId = modelId + 10
						if vr.list_targets[vr.current_target]['Target_Data']['Model_type'] == 1 then
							if modelId > #ID_Actors then
								modelId = 1+modelId-#ID_Actors
							end
							modell = ID_Actors[modelId]
							requestModel(modell)
							while not hasModelLoaded(modell) do
								wait(0)
							end
						else
							if modelId > #ID_Spec_Actors then
								modelId = 1+modelId-#ID_Spec_Actors
							end
						
							local modell_n = ID_Spec_Actors[modelId]
							loadSpecialCharacter(modell_n,10)
							while not hasSpecialCharacterLoaded(10) do
								wait(0)
							end
							modell = 290 + 10-1
						end
						setPlayerModel(PLAYER_HANDLE, modell)
					end
					if isKeyDown(vkeys.VK_L) then
						wait(100)
						local modell
						modelId = modelId - 10
						if vr.list_targets[vr.current_target]['Target_Data']['Model_type'] == 1 then
							if modelId < 1 then
								modelId = #ID_Actors-modelId
							end
							modell = ID_Actors[modelId]
							requestModel(modell)
							while not hasModelLoaded(modell) do
								wait(0)
							end
						else
							if modelId < 1 then
								modelId = #ID_Spec_Actors-modelId
							end
						
							local modell_n = ID_Spec_Actors[modelId]
							loadSpecialCharacter(modell_n,10)
							while not hasSpecialCharacterLoaded(10) do
								wait(0)
							end
							modell = 290 + 10-1
						end
						setPlayerModel(PLAYER_HANDLE, modell)
					end
					if isKeyDown(vkeys.VK_P) then
						wait(200)
						weapon = weapon + 1
						if weapon > #ID_Weapons then
							weapon = 1
						end
						local id_w = ID_Weapons[weapon]
						requestModel(getWeapontypeModel(id_w))
						while not hasModelLoaded(getWeapontypeModel(id_w)) do
							wait(1)
						end
						removeAllCharWeapons(PLAYER_PED)
						giveWeaponToChar(PLAYER_PED, id_w, 9999)
						setCurrentCharWeapon(PLAYER_PED,1)
					end
					if isKeyDown(vkeys.VK_OEM_4) then
						wait(200)
						weapon = weapon - 1
						if weapon < 1 then
							weapon = #ID_Weapons
						end
						local id_w = ID_Weapons[weapon]
						requestModel(getWeapontypeModel(id_w))
						while not hasModelLoaded(getWeapontypeModel(id_w)) do
							wait(1)
						end
						removeAllCharWeapons(PLAYER_PED)
						giveWeaponToChar(PLAYER_PED, id_w, 9999)
						setCurrentCharWeapon(PLAYER_PED,1)
					end
					if wasKeyReleased(vkeys.VK_U) then
						if vr.list_targets[vr.current_target]['Target_Data']['Model_type'] == 1 then
							vr.list_targets[vr.current_target]['Target_Data']['Model_type'] = 2
						else
							vr.list_targets[vr.current_target]['Target_Data']['Model_type'] = 1
						end
						modelId = 1
						local modell
						if vr.list_targets[vr.current_target]['Target_Data']['Model_type'] == 1 then
							modell = ID_Actors[modelId]
							requestModel(modell)
							while not hasModelLoaded(modell) do
								wait(0)
							end
						else
							local modell_n = ID_Spec_Actors[modelId]
							loadSpecialCharacter(modell_n,10)
							while not hasSpecialCharacterLoaded(10) do
								wait(0)
							end
							modell = 290 + 10-1
						end
						setPlayerModel(PLAYER_HANDLE, modell)
					end
				elseif stage == 1 then
					if isKeyDown(vkeys.VK_I) then
						wait(50)
						ammo = ammo + 10
					end
					if isKeyDown(vkeys.VK_O) and ammo > 0 then
						wait(50)
						ammo = ammo - 10
					end
					printString(koder(u8:decode(langt['ammo'])..': ' .. '~r~' .. ammo), 0)
				end
			end
		end

		if vr.editmode_preview_player_anim then
			if not hasAnimationLoaded(Anims['Anim_name'][vr.list_targets[vr.id_preview_player_anim]['Target_Data']['Pack_anim']]) then
				requestAnimation(Anims['Anim_name'][vr.list_targets[vr.id_preview_player_anim]['Target_Data']['Pack_anim']])
				while not hasAnimationLoaded(Anims['Anim_name'][vr.list_targets[vr.id_preview_player_anim]['Target_Data']['Pack_anim']]) do
					wait(0)
				end
			end
			local animm = Anims['Anim_list'][vr.list_targets[vr.id_preview_player_anim]['Target_Data']['Pack_anim']]
			animm = animm[vr.list_targets[vr.id_preview_player_anim]['Target_Data']['Anim']]
			taskPlayAnim(PLAYER_PED, animm, Anims['Anim_name'][vr.list_targets[vr.id_preview_player_anim]['Target_Data']['Pack_anim']], 1.0, vr.list_targets[vr.id_preview_player_anim]['Target_Data']['Loop'], false, false, false, -1)
			while vr.editmode_preview_player_anim do
				wait(0)
				if wasKeyReleased(vkeys.VK_F) then
					clearHelp()
					vr.editmode_preview_player_anim = false
					taskPlayAnim(PLAYER_PED, "WALK_START", 'PED', 1.0, false, false, false, false, -1)
					lockPlayerControl(true)
				end
			end
		end

		if vr.editmode_car then
			lockPlayerControl(false)
			local carr = createCar(vr.list_cars[vr.id_car]['Car_Data']['ModelId'], vr.list_cars[vr.id_car]['Car_Data']['Pos'][1], vr.list_cars[vr.id_car]['Car_Data']['Pos'][2], vr.list_cars[vr.id_car]['Car_Data']['Pos'][3])
			changeCarColour(carr, vr.list_cars[vr.id_car]['Car_Data']['Color_primary'],vr.list_cars[vr.id_car]['Car_Data']['Color_secondary'])
			setCarHeading(carr, vr.list_cars[vr.id_car]['Car_Data']['Angle'])
			taskWarpCharIntoCarAsDriver(PLAYER_PED, carr)
			while vr.editmode_car do
				wait(0)
				if not isCharInCar(PLAYER_PED, carr) then
					local xx,xy,xz = getCarCoordinates(carr)
					vr.list_cars[vr.id_car]['Car_Data']['Pos'] = {xx,xy,xz}
					vr.list_cars[vr.id_car]['Car_Data']['Angle'] = getCarHeading(carr)
					deleteCar(carr)
					upd_car:run(vr.id_car)
					vr.editmode_car = false
					openMenu = true
					lockPlayerControl(true)
				end
			end
		end

		if vr.miss_start then
			for i = 1,#vr.list_actors do
				deleteChar(vr.list_actors[i]['Actor_Data']['Char'])
			end
			for c = 1,#vr.list_cars do
				deleteCar(vr.list_cars[c]['Car_Data']['Car'])
			end
			for o = 1,#vr.list_objects do
				deleteObject(vr.list_objects[o]['Object_Data']['Obj'])
			end
			for p = 1,#vr.list_pickup do
				removePickup(vr.list_pickup[p]['Pickup_Data']['Pick'])
			end
			for p = 1,#vr.list_particle do
				killFxSystem(vr.list_particle[p]['Particle_Data']['Prtcl'])
			end
			for p = 1,#vr.list_explosion do
				deleteObject(vr.list_explosion[p]['Explosion_Data']['Explosion'])
				removeScriptFire(vr.list_explosion[p]['Explosion_Data']['Fire'])
			end
			wait(0)
			mp.start_mission(vr.list_targets,vr.list_actors,vr.list_cars,vr.list_objects,vr.list_pickup,vr.list_particle,vr.list_explosion,vr.mission_data)
			vr.miss_start = false
		end

		while mp.miss ~= 0 do
			wait(0)
		end

        if wasKeyReleased(0x55) and not openMenu then
			-- drawMenu(langt['main_menu'],decodeJson(langt['main_menu_arr']))
			if not vr.main_menu_window then
				vr.current_item = 1
				vr.main_menu_window = true
				lockPlayerControl(true)
			end
		end
		if wasKeyReleased(0x45) then
			vr.editmode_input = true
		end
		if isKeyDown(vkeys.VK_CONTROL) and isKeyJustPressed(vkeys.VK_S) then
			if vr.list_missions[vr.Fast_data['CurMiss']] == nil then
				vr.mission_data['Name'] = langt['mission']..' #' .. tostring(vr.Fast_data['CurMiss'])
				vr.list_missions[vr.Fast_data['CurMiss']] = {
					['Name'] = vr.mission_data['Name'],
					['Prename'] = '',
					['Mission_Data'] = {
						['Targets'] = vr.list_targets,
						['Actors'] = vr.list_actors,
						['Cars'] = vr.list_cars,
						['Objects'] = vr.list_objects,
						['Pickup'] = vr.list_pickup,
						['Particle'] = vr.list_particle,
						['Explosion'] = vr.list_explosion,
						['Miss_data'] = vr.mission_data,
						['Group_relationships'] = vr.Group_relationships
					}
				}
				vr.list_name_missions[vr.Fast_data['CurMiss']] = vr.list_missions[vr.Fast_data['CurMiss']]['Name']
			else
				vr.list_missions[vr.Fast_data['CurMiss']]['Mission_Data'] = {
					['Targets'] = vr.list_targets,
					['Actors'] = vr.list_actors,
					['Cars'] = vr.list_cars,
					['Objects'] = vr.list_objects,
					['Pickup'] = vr.list_pickup,
					['Particle'] = vr.list_particle,
					['Explosion'] = vr.list_explosion,
					['Miss_data'] = vr.mission_data,
					['Group_relationships'] = vr.Group_relationships
				}
			end
			if vr.list_mission_pack[vr.Fast_data['CurPack']] == nil then
				vr.list_mission_pack[vr.Fast_data['CurPack']] = {
					['Name'] = langt['packMissions']..' #' .. tostring(vr.Fast_data['CurPack']),
					['Missions'] = vr.list_missions
				}
				vr.list_name_mission_pack[vr.Fast_data['CurPack']] = vr.list_mission_pack[vr.Fast_data['CurPack']]['Name']
				manager.save(vr.list_mission_pack[vr.Fast_data['CurPack']],vr.Fast_data['CurPack']-1)
			else
				vr.list_mission_pack[vr.Fast_data['CurPack']]['Missions'] = vr.list_missions
				manager.save(vr.list_mission_pack[vr.Fast_data['CurPack']],vr.Fast_data['CurPack']-1)
			end
			printHelpString(koder('Сохранено!'))
		end

		if vr.main_menu_window then
			if drawMenu(langt['main_menu'],vr.main_menu_items,300,320) then
				main_menu()
			end
			if isKeyJustPressed(0x46) then
				vr.main_menu_window = false
				lockPlayerControl(false)
			end
		elseif vr.actor_change_window then
			if drawMenu(langt['actors'],vr.actor_change_items,300,320) then
				actor_change()
			end
			if isKeyJustPressed(0x46) then
				vr.actor_change_window = false
				lockPlayerControl(false)
			end
		elseif vr.actor_list_window then
			vr.actor_list_items =  {}
			for i = 1,#vr.list_name_actors do
				vr.actor_list_items[#vr.actor_list_items+1] = {'button',vr.list_name_actors[i],i}
			end
			if drawMenu(langt['actors'],vr.actor_list_items,300,320) then
				actor_list()
			end
			if isKeyJustPressed(0x46) then
				vr.actor_list_window = false
				lockPlayerControl(false)
			end
		elseif vr.actor_window then
			local names = decodeJson(langt['actor_arr'])
			local list_tg_m = {langt['toend']}
			for ltg = 1,#vr.list_name_targets do
			  list_tg_m[#list_tg_m+1] = vr.list_name_targets[ltg]
			end
			vr.actor_items = {
				{'button',names[1],1},
				{'int_slider',names[2],vr.list_actors[vr.current_actor]['Actor_Data'],"Angle",0,360,_,2},
				{'arr_slider',langt['type_model'],vr.list_actors[vr.current_actor]['Actor_Data'],"Model_type",decodeJson(langt['type_model_arr']),3}
			}
			if vr.list_actors[vr.current_actor]['Actor_Data']['Model_type'] == 1 then
				table.merge(vr.actor_items,{
					{'arr_slider',names[3],vr.list_actors[vr.current_actor]['Actor_Data'],"ModelId",ID_Actors,4}
				})
			elseif vr.list_actors[vr.current_actor]['Actor_Data']['Model_type'] == 2 then
				table.merge(vr.actor_items,{
					{'arr_slider',names[3],vr.list_actors[vr.current_actor]['Actor_Data'],"ModelId",ID_Spec_Actors,4},
					{'int_slider',langt['slot'],vr.list_actors[vr.current_actor]['Actor_Data'],"Slot_model",1,10,_,5},
				})
			end
			table.merge(vr.actor_items,{
				{'arr_slider',names[4],vr.list_actors[vr.current_actor]['Actor_Data'],"Weapon",decodeJson(langt['weap_names']),6},
				{'int_slider',names[5],vr.list_actors[vr.current_actor]['Actor_Data'],"Ammo",0,_,_,7},
				{'int_slider',names[6],vr.list_actors[vr.current_actor]['Actor_Data'],"Health",0,_,_,8},
				{'arr_slider',names[7],vr.list_actors[vr.current_actor]['Actor_Data'],"Group",decodeJson(langt['name_groups']),9},
				{'checkbox',names[8],vr.list_actors[vr.current_actor]['Actor_Data'],"Should_not_die",10},
				{'button',names[9],11},
				{'button',names[10],12},
				{'arr_slider',names[11],vr.list_actors[vr.current_actor]['Actor_Data'],"StartC",vr.list_name_targets,13},
				{'arr_slider',names[12],vr.list_actors[vr.current_actor]['Actor_Data'],"EndC",list_tg_m,14}
			})
			if drawMenu(langt['actors'],vr.actor_items,360,320) then
				actor()
			end
			if isKeyJustPressed(0x46) then
				vr.actor_window = false
				lockPlayerControl(false)
			end
		elseif vr.vector3_window then
			if drawMenu(vr.vector3_items[0],vr.vector3_items,300,320) then
				vector3(vr.vector3_items)
			end
			if isKeyJustPressed(0x46) then
				vr.vector3_window = false
				lockPlayerControl(false)
			end
		elseif vr.actor_anims_change_window then
			if drawMenu(decodeJson(langt['actor_arr'])[7],vr.actor_anims_change_items,300,320) then
				actor_anims_change()
			end
			if isKeyJustPressed(0x46) then
				vr.actor_anims_change_window = false
				lockPlayerControl(false)
			end
		elseif vr.actor_anims_list_window then
			vr.actor_anims_list_items =  {}
			for i = 1,#vr.list_actors[vr.current_actor]['Actor_Data']['Anims'] do
				vr.actor_anims_list_items[#vr.actor_anims_list_items+1] = {'button',i,i}
			end
			if drawMenu(langt['actors'],vr.actor_anims_list_items,300,320) then
				actor_anims_list()
			end
			if isKeyJustPressed(0x46) then
				vr.actor_anims_list_window = false
				lockPlayerControl(false)
			end
		elseif vr.actor_anim_window then
			local names = decodeJson(langt['actor_arr'])
			local type = decodeJson(langt['type_anim_actr'])
			local cond_type = decodeJson(langt['cond_type'])
			vr.actor_anim_items = {
				{'arr_slider',langt['type'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],'Type',type,1},
				{'arr_slider',langt['cond'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],'Condition',cond_type,2}
			}
			if vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Condition']  == 2 then
				table.merge(vr.actor_anim_items,{
					{'arr_slider',langt['target'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],'Target',vr.list_name_targets,3},
				})
			end
			table.merge(vr.actor_anim_items,{
				{'button',langt['del_preview'],4}
			})
			if vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Type'] == 1 then
				table.merge(vr.actor_anim_items,{
					{'arr_slider',langt['pack'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Pack_anim",Anims['Anim_name'],5},
					{'arr_slider',langt['anim'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Anim",Anims['Anim_list'][vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Pack_anim']],6},	
					{'int_slider',langt['time_anim'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Time",0,_,0.05,7},
					{'checkbox',langt['looped'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Loop",8},
					{'checkbox',langt['unbreak'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Unbreakable",9},
					{'button',langt['preview'],10}
				})
			elseif vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Type'] == 2 then
				local move_type = decodeJson(langt['move_type_ped'])
				local move_route = decodeJson(langt['move_route_ped'])
				table.merge(vr.actor_anim_items,{
					{'arr_slider',langt['type_move_ped'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Type_move",move_type,11},
					{'arr_slider',langt['type_move_ped'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Type_route",move_route,12},
					{'checkbox',langt['view_path'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Vis_point",13},
					{'button',langt['add_enter'],14},
					{'button',langt['perform'],15},
					{'checkbox',langt['wait_end'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"wait_end",16},
				})
			elseif vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Type'] == 3 then
				table.merge(vr.actor_anim_items,{
					{'checkbox',langt['view_path'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Vis_point",17},
					{'arr_slider',langt['car'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Car",vr.list_name_cars,18},
					{'int_slider',langt['speed'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Speed",0,_,_,19},
					{'button',langt['add_enter'],20},
					{'checkbox',langt['wait_end'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"wait_end",21},
				})
			elseif vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Type'] == 4 then
				table.merge(vr.actor_anim_items,{
					{'checkbox',langt['wait_end'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"wait_end",22},
				})
			elseif vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Type'] == 5 then
				local drive_beh = decodeJson(langt['driver_beh'])
				table.merge(vr.actor_anim_items,{
					{'arr_slider',langt['car_actr'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Car_a",vr.list_name_cars,23},
					{'int_slider',langt['max_speed'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Speed",0,_,_,24},
					{'int_slider',langt['beh_transp'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"trafficFlag",0,_,_,25},
					{'arr_slider',langt['beh_driver'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Vehicle_mission",drive_beh,26},
					{'arr_slider',langt['car_target'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Car_t",vr.list_name_cars,27},
				})
			elseif vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']]['Type'] == 6 then
				local place_in_car = decodeJson(langt['place_in_car'])
				local speed_walk_to_car = decodeJson(langt['speed_walk_to_car'])
				table.merge(vr.actor_anim_items,{
					{'arr_slider',langt['speed_walk'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"speed_walk",speed_walk_to_car,28},
					{'arr_slider',langt['car'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"Car",vr.list_name_cars,29},
					{'arr_slider',langt['seat'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"place_in_car",place_in_car,30},
					{'checkbox',langt['wait_end'],vr.list_actors[vr.current_actor]['Actor_Data']['Anims'][vr.list_actors[vr.current_actor]['Actor_Data']['Anim_id']],"wait_end",31},
				})
			end
			if drawMenu(langt['actors'],vr.actor_anim_items,540,320) then
				actor_anim()
			end
			if isKeyJustPressed(0x46) then
				vr.actor_anim_window = false
				lockPlayerControl(false)
			end
		elseif vr.targets_change_window then
			if drawMenu(decodeJson(langt['main_menu_arr'])[1],vr.targets_change_items,300,320) then
				targets_change()
			end
			if isKeyJustPressed(0x46) then
				vr.targets_change_window = false
				lockPlayerControl(false)
			end
		elseif vr.targets_list_window then
			vr.targets_list_items =  {}
			for i = 1,#vr.list_name_targets do
				vr.targets_list_items[#vr.targets_list_items+1] = {'button',vr.list_name_targets[i],i}
			end
			local namet = ''
			if vr.targets_list_toggle == 1 then
				namet = decodeJson(langt['main_menu_arr'])[1]
			elseif vr.targets_list_toggle == 2 then
				namet = decodeJson(langt['main_menu_arr'])[1]
			elseif vr.targets_list_toggle == 3 then
				namet = langt['cut']
			elseif vr.targets_list_toggle == 4 then
				namet = langt['paste']
			end
			if drawMenu(namet,vr.targets_list_items,300,320) then
				targets_list()
			end
			if isKeyJustPressed(0x46) then
				vr.targets_list_window = false
				lockPlayerControl(false)
			end
		elseif vr.target_window then
			local targets_list_arr = decodeJson(langt['targets_list_arr'])
			local targets_marker_color = decodeJson(langt['targets_marker_color'])
			local weap_names = decodeJson(langt['weap_names'])
			vr.target_items = {
				{'arr_slider',langt['typeTarget'],vr.list_targets[vr.current_target],"Type",targets_list_arr,1}
			}
			if vr.list_targets[vr.current_target]['Type'] == 1 then
				table.merge(vr.target_items,{
					{'button',langt['position'],2},
					{'button',langt['modeMove'],3},
					{'int_slider',langt['radiusCheckpoint'],vr.list_targets[vr.current_target]['Target_Data'],"Radius",0,100,0.1,4},
					{'arr_slider',langt['colorMarker'],vr.list_targets[vr.current_target]['Target_Data'],"Color_blip",targets_marker_color,5},
					{'button',langt['textTarget'],6},
					{'int_slider',langt['timeText'],vr.list_targets[vr.current_target]['Target_Data'],"Text_time",0,_,0.1,7},
				})
			elseif vr.list_targets[vr.current_target]['Type'] == 2 then
				table.merge(vr.target_items,{
					{'arr_slider',langt['car'],vr.list_targets[vr.current_target]['Target_Data'],"Target_car_id",vr.list_name_cars,8},
					{'arr_slider',langt['colorMarker'],vr.list_targets[vr.current_target]['Target_Data'],"Color_blip",targets_marker_color,9},
					{'button',langt['text'],10},	
				})
			elseif vr.list_targets[vr.current_target]['Type'] == 3 then
				table.merge(vr.target_items,{
					{'arr_slider',langt['actor'],vr.list_targets[vr.current_target]['Target_Data'],"Target_actor_id",vr.list_name_actors,11},
					{'arr_slider',langt['colorMarker'],vr.list_targets[vr.current_target]['Target_Data'],"Color_blip",targets_marker_color,12},
					{'button',langt['text'],13},
				})
			elseif vr.list_targets[vr.current_target]['Type'] == 4 then
				local target_type = decodeJson(langt['target_type'])
				table.merge(vr.target_items,{
					{'arr_slider',langt['type'],vr.list_targets[vr.current_target]['Target_Data'],"Target_type",target_type,14},
				})
				if vr.list_targets[vr.current_target]['Target_Data']['Target_type'] == 1 then
					table.merge(vr.target_items,{
						{'button',langt['position'],15},
						{'button',langt['rotate'],16},
						{'button',langt['modeMove'],17},
						{'checkbox',langt['movecam'],vr.list_targets[vr.current_target]['Target_Data'],"Smooth",18},
						{'button',langt['text'],19},
						{'int_slider',langt['timecutscene'],vr.list_targets[vr.current_target]['Target_Data'],"Text_time",0,_,0.1,20},
					})
				elseif vr.list_targets[vr.current_target]['Target_Data']['Target_type'] == 2 then
					table.merge(vr.target_items,{
					{'int_slider',langt['time'],vr.list_targets[vr.current_target]['Target_Data'],"Time",0,_,0.1,21},
					})
				elseif vr.list_targets[vr.current_target]['Target_Data']['Target_type'] == 3 then
					table.merge(vr.target_items,{
					{'button',langt['text'],22},
					{'int_slider',langt['timeout'],vr.list_targets[vr.current_target]['Target_Data'],"Time",0,_,0.1,23},
				})
				elseif vr.list_targets[vr.current_target]['Target_Data']['Target_type'] == 4 then
					table.merge(vr.target_items,{
					{'arr_slider',langt['weather'],vr.list_targets[vr.current_target]['Target_Data'],"Weather",Weather_str,24},
				})
				elseif vr.list_targets[vr.current_target]['Target_Data']['Target_type'] == 5 then
					table.merge(vr.target_items,{
					{'button',langt['timeGameMiss'],25},
				})
				elseif vr.list_targets[vr.current_target]['Target_Data']['Target_type'] == 6 then
					local count = decodeJson(langt['count_traffic'])
					table.merge(vr.target_items,{
					{'arr_slider',langt['countPed'],vr.list_targets[vr.current_target]['Target_Data']['Traffic'],1,count,26},
					{'arr_slider',langt['countCar'],vr.list_targets[vr.current_target]['Target_Data']['Traffic'],2,count,27},
					})
				end
			elseif vr.list_targets[vr.current_target]['Type'] == 5 then
				local target_type = decodeJson(langt['target_type_obj'])
				table.merge(vr.target_items,{
					{'arr_slider',langt['object'],vr.list_targets[vr.current_target]['Target_Data'],"Target_object_id",vr.list_name_objects,28},
					{'arr_slider',langt['type'],vr.list_targets[vr.current_target]['Target_Data'],"Target_type",target_type,29},	
				})
				if vr.list_targets[vr.current_target]['Target_Data']['Target_type'] == 4 then
					table.merge(vr.target_items,{
						{'arr_slider',langt['weapon'],vr.list_targets[vr.current_target]['Target_Data'],"Weap",weap_names,30},
					})
				end
				table.merge(vr.target_items,{
					{'arr_slider',langt['colorMarker'],vr.list_targets[vr.current_target]['Target_Data'],"Color_blip",targets_marker_color,31},
					{'button',langt['text'],32},
				})
			elseif vr.list_targets[vr.current_target]['Type'] == 6 then
				table.merge(vr.target_items,{
					{'arr_slider',langt['pickup'],vr.list_targets[vr.current_target]['Target_Data'],"Target_pickup_id",vr.list_name_pickup,33},
					{'arr_slider',langt['colorMarker'],vr.list_targets[vr.current_target]['Target_Data'],"Color_blip",targets_marker_color,34},
					{'button',langt['text'],35},
				})
			elseif vr.list_targets[vr.current_target]['Type'] == 7 then
				local target_type = decodeJson(langt['target_type_ev'])
				table.merge(vr.target_items,{
					{'arr_slider',langt['type'],vr.list_targets[vr.current_target]['Target_Data'],"Target_type",target_type,36},
				})
				if vr.list_targets[vr.current_target]['Target_Data']['Target_type'] == 1 then
					table.merge(vr.target_items,{
						{'button',langt['position'],37},
						{'int_slider',langt['angle'],vr.list_targets[vr.current_target]['Target_Data'],"Angle",0,360,1,38},
						{'arr_slider',langt['type_model'],vr.list_targets[vr.current_target]['Target_Data'],"Model_type",decodeJson(langt['type_model_arr']),39},
					})
					if vr.list_targets[vr.current_target]['Target_Data']['Model_type'] == 1 then
						table.merge(vr.target_items,{
							{'int_slider',langt['model'],vr.list_targets[vr.current_target]['Target_Data'],"ModelId",0,ID_Actors[#ID_Actors],40},
						})
					else
						table.merge(vr.target_items,{
							{'arr_slider',langt['model'],vr.list_targets[vr.current_target]['Target_Data'],"ModelId",ID_Spec_Actors,41},
						})
					end
					table.merge(vr.target_items,{
						{'int_slider',langt['countAmmo'],vr.list_targets[vr.current_target]['Target_Data'],"Weap_ammo",0,_,1,42},
						{'button',langt['edithand'],43},
					})
				elseif vr.list_targets[vr.current_target]['Target_Data']['Target_type'] == 2 then
					table.merge(vr.target_items,{
						{'arr_slider',langt['pack'],vr.list_targets[vr.current_target]['Target_Data'],"Pack_anim",Anims['Anim_name'],44},
						{'arr_slider',langt['anim'],vr.list_targets[vr.current_target]['Target_Data'],"Anim",Anims['Anim_list'][vr.list_targets[vr.current_target]['Target_Data']['Pack_anim']],46},
						{'checkbox',langt['looped'],vr.list_targets[vr.current_target]['Target_Data'],"Loop",47},
						{'button',langt['preview'],48},
					})
				elseif vr.list_targets[vr.current_target]['Target_Data']['Target_type'] == 3 then
					local place_car = decodeJson(langt['place_car'])
					table.merge(vr.target_items,{
						{'arr_slider',langt['car'],vr.list_targets[vr.current_target]['Target_Data'],"Car_id",vr.list_name_cars,49},
						{'arr_slider',langt['place'],vr.list_targets[vr.current_target]['Target_Data'],"Car_place",place_car,50},
					})
				elseif vr.list_targets[vr.current_target]['Target_Data']['Target_type'] == 4 then
					table.merge(vr.target_items,{
						{'int_slider',langt['levelp'],vr.list_targets[vr.current_target]['Target_Data'],"Level_battue",0,6,1,51},
					})
				elseif vr.list_targets[vr.current_target]['Target_Data']['Target_type'] == 6 then
					local names = decodeJson(langt['target_dialog_change_arr'])
					table.merge(vr.target_items,{
						{'button',names[1],52},
						{'button',names[2],53},
						{'button',names[3]},54,
					})
				elseif vr.list_targets[vr.current_target]['Target_Data']['Target_type'] == 7 then
					table.merge(vr.target_items,{
						{'int_slider',langt['add'],vr.list_targets[vr.current_target]['Target_Data'],"Add_money",55},
					})
				end
			end
			if drawMenu(langt['target'],vr.target_items,360,320) then
				target()
			end
			if isKeyJustPressed(0x46) then
				vr.target_window = false
				lockPlayerControl(false)
			end
		elseif vr.target_dialog_window then
			vr.target_dialog_items = {
				{'button',langt['text'],1},
				{'int_slider',langt['time'],vr.list_targets[vr.current_target]['Target_Data']['Dialog'][vr.current_dialog_target],"Text_time",0,_,0.1,2},
				{'button',langt['back'],3},
			}
			if drawMenu(langt['replica'],vr.target_dialog_items,360,320) then
				target_dialog()
			end
			if isKeyJustPressed(0x46) then
				vr.target_dialog_window = false
				lockPlayerControl(false)
			end
		elseif vr.target_list_dialog_window then
			vr.target_list_dialog_items =  {}
			for i = 1,#vr.list_targets[vr.current_target]['Target_Data']['Dialog'] do
				vr.target_list_dialog_items[#vr.target_list_dialog_items+1] = {'button',i,i}
			end
			if drawMenu(langt['replicas'],vr.target_list_dialog_items,300,320) then
				target_list_dialog()
			end
			if isKeyJustPressed(0x46) then
				vr.target_list_dialog_window = false
				lockPlayerControl(false)
			end
		elseif vr.cars_change_window then
			if drawMenu(langt['cars'],vr.cars_change_items,300,320) then
				cars_change()
			end
			if isKeyJustPressed(0x46) then
				vr.cars_change_window = false
				lockPlayerControl(false)
			end
		elseif vr.cars_list_window then
			vr.cars_list_items =  {}
			for i = 1,#vr.list_name_cars do
				vr.cars_list_items[#vr.cars_list_items+1] = {'button',vr.list_name_cars[i],i}
			end
			if drawMenu(langt['cars'],vr.cars_list_items,300,320) then
				cars_list()
			end
			if isKeyJustPressed(0x46) then
				vr.cars_list_window = false
				lockPlayerControl(false)
			end
		elseif vr.car_window then
			local car_unbreak = decodeJson(langt['car_unbreak'])
			local list_tg_m = {langt['toend']}
			for ltg = 1,#vr.list_name_targets do
				list_tg_m[#list_tg_m+1] = vr.list_name_targets[ltg]
			end
			vr.cars_items = {
				{'button',langt['position'],1},
				{'int_slider',langt['model'], vr.list_cars[vr.current_car]['Car_Data'],"ModelId",400,ID_Cars[#ID_Cars],_,2},
				{'int_slider',langt['color_car']..' 1', vr.list_cars[vr.current_car]['Car_Data'],"Color_primary",0,126,_,3},
				{'int_slider',langt['color_car']..' 2', vr.list_cars[vr.current_car]['Car_Data'],"Color_secondary",0,126,_,4},
				{'int_slider',langt['countlive'], vr.list_cars[vr.current_car]['Car_Data'],"Health",0,5},
				{'checkbox',car_unbreak[1],vr.list_cars[vr.current_car]['Car_Data'],"Bulletproof",6},
				{'checkbox',car_unbreak[2],vr.list_cars[vr.current_car]['Car_Data'],"Fireproof",7},
				{'checkbox',car_unbreak[3],vr.list_cars[vr.current_car]['Car_Data'],"Explosionproof",8},
				{'checkbox',car_unbreak[4],vr.list_cars[vr.current_car]['Car_Data'],"Collisionproof",9},
				{'checkbox',car_unbreak[5],vr.list_cars[vr.current_car]['Car_Data'],"Meleeproof",10},
				{'checkbox',car_unbreak[6],vr.list_cars[vr.current_car]['Car_Data'],"Tires_vulnerability",11},
				{'checkbox',langt['should_live'],vr.list_cars[vr.current_car]['Car_Data'],"Should_not_die",12},
				{'button',langt['edithand'],13},
				{'button',langt['actions'],14},
				{'arr_slider',langt['app_on'],vr.list_cars[vr.current_car]['Car_Data'],"StartC",vr.list_name_targets,15},
				{'arr_slider',langt['dis_after'],vr.list_cars[vr.current_car]['Car_Data'],"EndC",list_tg_m,16}
			}
			if drawMenu(langt['car'],vr.cars_items,320,320) then
				car()
			end
			if isKeyJustPressed(0x46) then
				vr.car_window = false
				lockPlayerControl(false)
			end
		elseif vr.car_anims_change_window then
			if drawMenu(langt['actions'],vr.actor_anims_change_items,300,320) then
				car_anims_change()
			end
			if isKeyJustPressed(0x46) then
				vr.car_anims_change_window = false
				lockPlayerControl(false)
			end
		elseif vr.car_anims_list_window then
			vr.car_anims_list_items =  {}
			for i = 1,#vr.list_cars[vr.current_car]['Car_Data']['Anims'] do
				vr.car_anims_list_items[#vr.car_anims_list_items+1] = {'button',i,i}
			end
			if drawMenu(langt['cars'],vr.car_anims_list_items,300,320) then
				car_anims_list()
			end
			if isKeyJustPressed(0x46) then
				vr.car_anims_list_window = false
				lockPlayerControl(false)
			end
		elseif vr.car_anim_window then
			local type = decodeJson(langt['type_car_anim'])
			vr.car_anim_items = {
				{'arr_slider',langt['type'],vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']],"Type",type,1},
			}
			if vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Type'] == 1 then
				local cond_type = decodeJson(langt['cond_type'])
				table.merge(vr.car_anim_items,{
					{'arr_slider',langt['cond'],vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']],"Condition",cond_type,2},
				})
				if vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Condition'] == 2 then
					table.merge(vr.car_anim_items,{
					{'arr_slider',langt['target'],vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']],"Target",vr.list_name_targets,3},
				})
				end
				local door_car = decodeJson(langt['door_car'])
				table.merge(vr.car_anim_items,{
					{'checkbox',door_car[1],vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Doors'],1,4},
					{'checkbox',door_car[2],vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Doors'],2,5},
					{'checkbox',door_car[3],vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Doors'],3,6},
					{'checkbox',door_car[4],vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Doors'],4,7},
					{'checkbox',door_car[5],vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Doors'],5,8},
					{'checkbox',door_car[6],vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Doors'],6,9},
				})
			elseif vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Type'] == 2 then
				local cond_type = decodeJson(langt['cond_type'])
				table.merge(vr.car_anim_items,{
					{'arr_slider',langt['cond'],vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']],"Condition",cond_type,10},
				})
				if vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']]['Condition'] == 2 then
					table.merge(vr.car_anim_items,{
					{'arr_slider',langt['target'],vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']],"Target",vr.list_name_targets,11},
				})
				end
				local open_close = decodeJson(langt['open_close'])
				table.merge(vr.car_anim_items,{
					{'arr_slider',langt['doors'],vr.list_cars[vr.current_car]['Car_Data']['Anims'][vr.list_cars[vr.current_car]['Car_Data']['Anim_id']],"Door_lock",open_close,12},	
				})
			end

			if drawMenu(langt['action'],vr.car_anim_items,320,320) then
				car_anim()
			end
			if isKeyJustPressed(0x46) then
				vr.car_anim_window = false
				lockPlayerControl(false)
			end
		elseif vr.objects_change_window then
			if drawMenu(langt['objects'],vr.objects_change_items,300,320) then
				objects_change()
			end
			if isKeyJustPressed(0x46) then
				vr.objects_change_window = false
				lockPlayerControl(false)
			end
		elseif vr.objects_list_window then
			vr.objects_list_items =  {}
			for i = 1,#vr.list_name_objects do
				vr.objects_list_items[#vr.objects_list_items+1] = {'button',vr.list_name_objects[i],i}
			end
			if drawMenu(langt['objects'],vr.objects_list_items,300,320) then
				objects_list()
			end
			if isKeyJustPressed(0x46) then
				vr.objects_list_window = false
				lockPlayerControl(false)
			end
		elseif vr.object_window then
			local list_tg_m = {langt['toend']}
			for ltg = 1,#vr.list_name_targets do
				list_tg_m[#list_tg_m+1] = vr.list_name_targets[ltg]
			end
			vr.object_items =  {
				{'button',langt['position'],1},
				{'button',langt['rotate'],2},
				{'button',langt['edithand'],3},
				{'button',langt['model'],4},
				{'button',langt['actions'],5},
				{'arr_slider',langt['app_on'],vr.list_objects[vr.current_object]['Object_Data'],"StartC",vr.list_name_targets,6},
				{'arr_slider',langt['dis_after'],vr.list_objects[vr.current_object]['Object_Data'],"EndC",list_tg_m,7}
			}
			if drawMenu(langt['object'],vr.object_items,320,320) then
				object()
			end
			if isKeyJustPressed(0x46) then
				vr.object_window = false
				lockPlayerControl(false)
			end
		elseif vr.object_anims_change_window then
			if drawMenu(langt['actions'],vr.actor_anims_change_items,300,320) then
				object_anims_change()
			end
			if isKeyJustPressed(0x46) then
				vr.object_anims_change_window = false
				lockPlayerControl(false)
			end
		elseif vr.object_anims_list_window then
			vr.object_anims_list_items =  {}
			for i = 1,#vr.list_objects[vr.current_object]['Object_Data']['Anims'] do
				vr.object_anims_list_items[#vr.object_anims_list_items+1] = {'button',i,i}
			end
			if drawMenu(langt['objects'],vr.object_anims_list_items,300,320) then
				object_anims_list()
			end
			if isKeyJustPressed(0x46) then
				vr.object_anims_list_window = false
				lockPlayerControl(false)
			end
		elseif vr.object_anim_window then
			vr.object_anim_items = {}
			local cond_type = decodeJson(langt['cond_type'])
			table.merge(vr.object_anim_items,{
				{'arr_slider',langt['cond'],vr.list_objects[vr.current_object]['Object_Data']['Anims'][vr.list_objects[vr.current_object]['Object_Data']['Anim_id']],"Condition",cond_type,1},
			})
			if vr.list_objects[vr.current_object]['Object_Data']['Anims'][vr.list_objects[vr.current_object]['Object_Data']['Anim_id']]['Condition'] == 2 then
				table.merge(vr.object_anim_items,{
					{'arr_slider',langt['target'],vr.list_objects[vr.current_object]['Object_Data']['Anims'][vr.list_objects[vr.current_object]['Object_Data']['Anim_id']],"Target",vr.list_name_targets,2},
				})
			end
			table.merge(vr.object_anim_items,{
				{'button',langt['position'],3},
				{'button',langt['rotate'],4},
				{'int_slider',langt['time_anim'],vr.list_objects[vr.current_object]['Object_Data']['Anims'][vr.list_objects[vr.current_object]['Object_Data']['Anim_id']],"Time",0,_,0.1,5},
				{'button',langt['edithand'],6},
			})
			if drawMenu(langt['object'],vr.object_anim_items,300,320) then
				object_anim()
			end
			if isKeyJustPressed(0x46) then
				vr.object_anim_window = false
				lockPlayerControl(false)
			end
		elseif vr.particles_change_window then
			if drawMenu(langt['particles'],vr.particles_change_items,300,320) then
				particles_change()
			end
			if isKeyJustPressed(0x46) then
				vr.particles_change_window = false
				lockPlayerControl(false)
			end
		elseif vr.particles_list_window then
			vr.particles_list_items =  {}
			for i = 1,#vr.list_name_particle do
				vr.particles_list_items[#vr.particles_list_items+1] = {'button',vr.list_name_particle[i],i}
			end
			if drawMenu(langt['particles'],vr.particles_list_items,300,320) then
				particles_list()
			end
			if isKeyJustPressed(0x46) then
				vr.particles_list_window = false
				lockPlayerControl(false)
			end
		elseif vr.particle_window then
			local list_tg_m = {langt['toend']}
			for ltg = 1,#vr.list_name_targets do
				list_tg_m[#list_tg_m+1] = vr.list_name_targets[ltg]
			end
			vr.particle_items =  {
				{'button',langt['position'],1},
				{'button',langt['rotate'],2},
				{'button',langt['edithand'],3},
				{'arr_slider',langt['particle'],vr.list_particle[vr.current_particle]['Particle_Data'],"ModelId",Particle_name,4},
				{'arr_slider',langt['app_on'],vr.list_particle[vr.current_particle]['Particle_Data'],"StartC",vr.list_name_targets,5},
				{'arr_slider',langt['dis_after'],vr.list_particle[vr.current_particle]['Particle_Data'],"EndC",list_tg_m,6}
			}
			if drawMenu(langt['particle'],vr.particle_items,320,320) then
				particle()
			end
			if isKeyJustPressed(0x46) then
				vr.particle_window = false
				lockPlayerControl(false)
			end
		elseif vr.pickup_change_window then
			if drawMenu(langt['pickup'],vr.pickup_change_items,300,320) then
				pickup_change()
			end
			if isKeyJustPressed(0x46) then
				vr.pickup_change_window = false
				lockPlayerControl(false)
			end
		elseif vr.pickup_list_window then
			vr.pickup_list_items =  {}
			for i = 1,#vr.list_name_pickup do
				vr.pickup_list_items[#vr.pickup_list_items+1] = {'button',vr.list_name_pickup[i],i}
			end
			if drawMenu(langt['pickup'],vr.pickup_list_items,300,320) then
				pickup_list()
			end
			if isKeyJustPressed(0x46) then
				vr.pickup_list_window = false
				lockPlayerControl(false)
			end
		elseif vr.pickup_window then
			local list_tg_m = {langt['toend']}
			for ltg = 1,#vr.list_name_targets do
				list_tg_m[#list_tg_m+1] = vr.list_name_targets[ltg]
			end
			local type_pick = decodeJson(langt['type_pickup'])
			local spawn_type = decodeJson(langt['spawn_type_pickup'])
			vr.pickup_items =  {
				{'arr_slider',langt['spawn'],vr.list_pickup[vr.current_pickup]['Pickup_Data'],"spawn_type",spawn_type,1},
				{'arr_slider',langt['type'],vr.list_pickup[vr.current_pickup]['Pickup_Data'],"Type_pickup",type_pick,2},
			}
			if vr.list_pickup[vr.current_pickup]['Pickup_Data']['Type_pickup'] == 1 then
				table.merge(vr.pickup_items,{
					{'button',langt['edithand'],3},
					{'int_slider',langt['ammo'], vr.list_pickup[vr.current_pickup]['Pickup_Data'],"Ammo",0,_,_,4},
				})
			elseif vr.list_pickup[vr.current_pickup]['Pickup_Data']['Type_pickup'] >= 2 then
				table.merge(vr.pickup_items,{
					{'button',langt['edithand'],5},
				})
				if vr.list_pickup[vr.current_pickup]['Pickup_Data']['Type_pickup'] == 6 then
					table.merge(vr.pickup_items,{
						{'button',langt['model'],6},
					})
				end
			end
			table.merge(vr.pickup_items,{
				{'arr_slider',langt['app_on'],vr.list_pickup[vr.current_pickup]['Pickup_Data'],"StartC",vr.list_name_targets,7},
				{'arr_slider',langt['dis_after'],vr.list_pickup[vr.current_pickup]['Pickup_Data'],"EndC",list_tg_m,8}
			})
			if drawMenu(langt['pickup'],vr.pickup_items,340,320) then
				pickup()
			end
			if isKeyJustPressed(0x46) then
				vr.pickup_window = false
				lockPlayerControl(false)
			end
		elseif vr.explosion_change_window then
			if drawMenu(langt['explosions'],vr.explosion_change_items,300,320) then
				explosion_change()
			end
			if isKeyJustPressed(0x46) then
				vr.explosion_change_window = false
				lockPlayerControl(false)
			end
		elseif vr.explosion_list_window then
			vr.explosion_list_items =  {}
			for i = 1,#vr.list_name_explosion do
				vr.explosion_list_items[#vr.explosion_list_items+1] = {'button',vr.list_name_explosion[i],i}
			end
			if drawMenu(langt['explosions'],vr.explosion_list_items,300,320) then
				explosion_list()
			end
			if isKeyJustPressed(0x46) then
				vr.explosion_list_window = false
				lockPlayerControl(false)
			end
		elseif vr.explosion_window then
			vr.explosion_items =  {}

			local list_tg_m = {langt['toend']}
			for ltg = 1,#vr.list_name_targets do
				list_tg_m[#list_tg_m+1] = vr.list_name_targets[ltg]
			end

			local type_expl = decodeJson(langt['type_expl'])
			table.merge(vr.explosion_items,{
				{'arr_slider',langt['type'],vr.list_explosion[vr.current_explosion]['Explosion_Data'],"Type",type_expl,1},
				{'button',langt['position'],2},
			})

			if vr.list_explosion[vr.current_explosion]['Explosion_Data']['Type'] == 2 then
				table.merge(vr.explosion_items,{
					{'int_slider',langt['type_explosion'],vr.list_explosion[vr.current_explosion]['Explosion_Data'],"Type_explosion",0,_,_,3},
				})
			elseif vr.list_explosion[vr.current_explosion]['Explosion_Data']['Type'] == 1 then
				table.merge(vr.explosion_items,{
					{'int_slider',langt['size_fire'],vr.list_explosion[vr.current_explosion]['Explosion_Data'],"Size_fire",0,_,_,4},
					{'int_slider',langt['spread_fire'],vr.list_explosion[vr.current_explosion]['Explosion_Data'],"Propagation_fire",0,_,_,5},
				})
			end

			table.merge(vr.explosion_items,{
				{'button',langt['edithand'],6},
				{'arr_slider',langt['app_on'],vr.list_explosion[vr.current_explosion]['Explosion_Data'],"StartC",vr.list_name_targets,7},
				{'arr_slider',langt['dis_after'],vr.list_explosion[vr.current_explosion]['Explosion_Data'],"EndC",list_tg_m,8},
			})

			if drawMenu(langt['explosion'],vr.explosion_items,320,320) then
				explosion()
			end
			if isKeyJustPressed(0x46) then
				vr.explosion_window = false
				lockPlayerControl(false)
			end
		elseif vr.pack_mission_change_window then
			if drawMenu(langt['packsMissions'],vr.mission_pack_change_items,300,320) then
				pack_mission_change()
			end
			if isKeyJustPressed(0x46) then
				vr.pack_mission_change_window = false
				lockPlayerControl(false)
			end
		elseif vr.pack_mission_list_window then
			vr.pack_mission_list_items =  {}
			for i = 1,#vr.list_name_mission_pack do
				vr.pack_mission_list_items[#vr.pack_mission_list_items+1] = {'button',vr.list_name_mission_pack[i],i}
			end
			if drawMenu(langt['packsMissions'],vr.pack_mission_list_items,460,320) then
				pack_mission_list()
			end
			if isKeyJustPressed(0x46) then
				vr.pack_mission_list_window = false
				lockPlayerControl(false)
			end
		elseif vr.missions_change_window then
			if drawMenu(langt['missions'],vr.missions_change_items,300,320) then
				missions_change()
			end
			if isKeyJustPressed(0x46) then
				vr.missions_change_window = false
				lockPlayerControl(false)
			end
		elseif vr.missions_list_window then
			vr.missions_list_items =  {}
			for i = 1,#vr.list_name_missions do
				vr.missions_list_items[#vr.missions_list_items+1] = {'button',vr.list_name_missions[i],i}
			end
			if drawMenu(langt['missions'],vr.missions_list_items,460,320) then
				missions_list()
			end
			if isKeyJustPressed(0x46) then
				vr.missions_list_window = false
				lockPlayerControl(false)
			end
		elseif vr.missions_settings_window then
			vr.missions_settings_items =  {
				{'button',langt['nameMission'],1},
				{'button',langt['timeGameMiss'],2},
				{'arr_slider',langt['weather'],vr.mission_data,"Weather",Weather_str,3},
				{'checkbox',langt['modeRiot'],vr.mission_data,"Riot",4},

			}
			if drawMenu(langt['missions'],vr.missions_settings_items,460,320) then
				missions_settings()
			end
			if isKeyJustPressed(0x46) then
				vr.missions_settings_window = false
				lockPlayerControl(false)
			end
		elseif vr.tools_window then
			vr.tools_items =  {
				{'button',langt['tool_tp_marker'],1},
				{'button',langt['jetpack'],2},
			}
			if drawMenu(langt['tools'],vr.tools_items,300,320) then
				tools()
			end
			if isKeyJustPressed(0x46) then
				vr.tools_window = false
				lockPlayerControl(false)
			end
		elseif vr.info_window then
			printHelpForever('INFOLD')

			while not isKeyJustPressed(0x46) do
				wait(0)
			end
			clearHelp()
			vr.info_window = false
			lockPlayerControl(false)
		elseif vr.settings_window then
			vr.settings_items =  {
				{'arr_slider',langt['lang'],vr.Data.Settings,'curr_lang',LanguageList,1},
			}
			if drawMenu(langt['settings'],vr.settings_items,300,320) then
				settings()
			end
			if isKeyJustPressed(0x46) then
				vr.settings_window = false
				lockPlayerControl(false)
			end
		elseif vr.Group_relationships_list_window then
			if drawMenu(langt['groups'],vr.Group_relationships_list_items,300,320) then
				Group_relationships_list()
			end
			if isKeyJustPressed(0x46) then
				vr.Group_relationships_list_window = false
				lockPlayerControl(false)
			end
		elseif vr.Group_relationships_window then
			if drawMenu(langt['Relationship_to_groups'],vr.Group_relationships_items,340,320) then
				Group_relationships()
			end
			if isKeyJustPressed(0x46) then
				vr.Group_relationships_window = false
				lockPlayerControl(false)
			end
		end
	end
end
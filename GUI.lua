script_authors('SKIC','SIZZZ')
script_version('Beta 0.5.7.1')

require 'libstd.deps' {
    'fyp:mimgui',
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
weapons = require 'lib.game.weapons'
local encoding = require 'encoding'
imgui.Process = false
encoding.default = 'CP1251'
u8 = encoding.UTF8

ID_Actors = {0, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68, 69, 70, 71, 72, 73, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288}

ID_Cars = {400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611}

Number_Weapons = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46}

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

local Weather_str = {u8'Очень солнечно (LS)',u8'Солнечно (LS)',u8'Очень солнечно, смог (LS)',u8'Солнечно, смог (LS)',u8'Облака (LS)',u8'Солнечно (SF)',u8'Очень солнечно (SF)',u8'Облака (SF)',u8'Дождливая (SF)',u8'Туманная (SF)',u8'Солнечно (LV)',u8'Очень солнечно (LV)',u8'Облака (LV)',u8'Очень солнечно (CS)',u8'Солнечно (CS)',u8'Облака (CS)',u8'Дождливая (CS)',u8'Очень солнечно (Des)',u8'Солнечно (Des)',u8'Песчаная буря (Des)',u8'Подводная', u8'Дополнительно 1',u8'Дополнительно 2'}

Particle_name = {"blood_heli","boat_prop","camflash","carwashspray","cement","cloudfast","coke_puff","coke_trail","cigarette_smoke","explosion_barrel","explosion_crate","explosion_door","exhale","explosion_fuel_car","explosion_large","explosion_medium","explosion_molotov","explosion_small","explosion_tiny","extinguisher","flame","fire","fire_med","fire_large","flamethrower","fire_bike","fire_car","gunflash","gunsmoke","insects","heli_dust","jetpack","jetthrust","nitro","molotov_flame","overheat_car","overheat_car_electric","prt_boatsplash","prt_cardebris","prt_collisionsmoke","prt_glass","prt_gunshell","prt_sand","prt_sand2","prt_smokeII_3_expand","prt_smoke_huge","prt_spark","prt_spark_2","prt_splash","prt_wake","prt_watersplash","prt_wheeldirt","petrolcan","puke","riot_smoke","spraycan","smoke30lit","smoke30m","smoke50lit","shootlight","smoke_flare","tank_fire","teargas","teargasAD","tree_hit_fir","tree_hit_palm","vent","vent2","water_hydrant","water_ripples","water_speed","water_splash","water_splash_big","water_splsh_sml","water_swim","waterfall_end","water_fnt_tme","water_fountain","wallbust","WS_factorysmoke"}

vr = {
	['mission_data'] = {
		['Name'] = '',
		['Time'] = {0,0},
		['Weather'] = new.int(0),
		['Riot'] = new.bool(false),
		['Player'] = {
			['Pos'] = {884,-1221,16},
			['Angle'] = 0,
			['ModelId'] = 0,
			['Weapon'] = 0,
			['Weap_ammo'] = 0,
			['Interior_id'] = 0
		}
	},
	['buf_edit_targets_name'] = new.char[64](),
	['buf_edit_actors_name'] = new.char[64](),
	['buf_edit_cars_name'] = new.char[64](),
	['buf_edit_obj_name'] = new.char[64](),
	['buf_edit_pick_name'] = new.char[64](),
	['buf_edit_prtcl_name'] = new.char[64](),
	['buf_edit_explosion_name'] = new.char[64](),
	['buf_edit_mission_pack_name'] = new.char[64](),
	['buf_edit'] = new.float(0),
	['main_window'] = new.bool(false),
	['pack_mission_window'] = new.bool(false),
	['missions_window'] = new.bool(false),
	['targets_window'] = new.bool(false),
	['actors_window'] = new.bool(false),
	['cars_window'] = new.bool(false),
	['objects_window'] = new.bool(false),
	['pickup_window'] = new.bool(false),
	['particle_window'] = new.bool(false),
	['explosion_window'] = new.bool(false),
	['targets_window_s1'] = new.bool(false),
	['targets_window_s2'] = new.bool(false),
	['missions_n_window'] = new.bool(false),
	['info_window'] = new.bool(false),
	['settings_window'] = new.bool(false),
	['tool_window'] = new.bool(false),
	['lb_cur_targets'] = new.int(0),
	['lb_cur_actors'] = new.int(0),
	['lb_cur_cars'] = new.int(0),
	['lb_cur_objects'] = new.int(0),
	['lb_cur_pickup'] = new.int(0),
	['lb_cur_particle'] = new.int(0),
	['lb_cur_explosion'] = new.int(0),
	['lb_cur_pack_mission'] = new.int(0),
	['lb_cur_missions'] = new.int(0),
	['buffer_target'] = -1,
	['list_targets'] = {},
	['list_actors'] = {},
	['list_cars'] = {},
	['list_objects'] = {},
	['list_pickup'] = {},
	['list_particle'] = {},
	['list_explosion'] = {},
	['list_mission_pack'] = {},
	['list_missions'] = {},
	['list_name_targets'] = {},
	['list_name_actors'] = {},
	['list_name_cars'] = {},
	['list_name_objects'] = {},
	['list_name_pickup'] = {},
	['list_name_particle'] = {},
	['list_name_explosion'] = {},
	['list_name_mission_pack'] = {},
	['list_name_missions'] = {},
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
	['id_target'] = 0,
	['id_actor'] = 0,
	['id_actor_anim'] = 0,
	['id_car'] = 0,
	['id_obj'] = 0,
	['id_obj_anim'] = 0,
	['id_pick'] = 0,
	['id_prtcl'] = 0,
	['id_explosion'] = 0,
	['id_timetarg'] = 0,
	['id_teleport_player'] = 0,
	['id_preview_player_anim'] = 0,
	['miss_start'] = false,
	['update_actor'] = -1,
	['curr_lang'] = new.int(1),
	['Fast_data'] = {
		['CurMiss'] = 0,
		['CurPack'] = 0
	}
}

local LanguageArr = {}
local LanguageList = {}

-- Главное меню
imgui.OnFrame(function() return vr.main_window[0] and imgui.Process end, 
function()
    local langt = LanguageArr[vr.curr_lang[0]+1]['Keys']
    
    local resX,resY = getScreenResolution()
    local sizeX,sizeY = 200, 400
    imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
    imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2,(resY-sizeY)/2),imgui.Cond.FirstUseEver)
    imgui.Begin(langt['mainMenu'], vr.main_window)
    if imgui.Button(langt['targets']) then
      vr.targets_window[0] = not vr.targets_window[0]
      vr.main_window[0] = false
    end
    if imgui.Button(langt['actors']) then
      vr.actors_window[0] = not vr.actors_window[0]
      vr.main_window[0] = false
    end
    if imgui.Button(langt['cars']) then
      vr.cars_window[0] = not vr.cars_window[0]
      vr.main_window[0] = false
    end
    if imgui.Button(langt['objects']) then
      vr.objects_window[0] = not vr.objects_window[0]
      vr.main_window[0] = false
    end
    if imgui.Button(langt['particles']) then
      vr.particle_window[0] = not vr.particle_window[0]
      vr.main_window[0] = false
    end
    if imgui.Button(langt['pickups']) then
      vr.pickup_window[0] = true
      vr.main_window[0] = false
    end
    if imgui.Button(langt['explosions']) then
      vr.explosion_window[0] = true
      vr.main_window[0] = false
    end
    if imgui.Button(langt['player']) then
      vr.editmode_player = true
      printHelpForever('HPLA')
      imgui.Process = false
    end
    imgui.Separator()
    if imgui.Button(langt['packsMissions']) then
      vr.pack_mission_window[0] = not vr.pack_mission_window[0]
      --manager_miss.save(vr.list_targets,vr.list_actors,vr.list_cars,vr.list_objects)
      vr.main_window[0] = false
    end
    if imgui.Button(langt['missions']) then
      vr.missions_window[0] = not vr.missions_window[0]
      vr.main_window[0] = false
    end
    imgui.Separator()
    if imgui.Button(langt['missionStart']) then
      vr.miss_start = true
    end
    if imgui.Button(langt['tools']) then
      vr.main_window[0] = false
      vr.tool_window[0] = true
    end
    imgui.Separator()
    if imgui.Button(langt['info']) then
      vr.main_window[0] = false
      vr.info_window[0] = true
    end
    if imgui.Button(langt['settings']) then
      vr.main_window[0] = false
      vr.settings_window[0] = true
    end
    --mymod.main_menu()
    imgui.End()
end)

imgui.OnFrame(function() return vr.missions_window[0] and imgui.Process end, 
function()
	local langt = LanguageArr[vr.curr_lang[0]+1]['Keys']

	local resX,resY = getScreenResolution()
	local sizeX,sizeY = 350, 340
	imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
	imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 175,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
	imgui.Begin(langt['missions'], vr.missions_window)

	imgui.PushItemWidth(-10)


	--Список Объектов
	imgui.ListBoxStr_arr('', vr.lb_cur_missions,new('const char* const [?]', #vr.list_name_missions, vr.list_name_missions),#vr.list_name_missions,15)

	--Кнопки редактирования
	if imgui.Button(langt['add']) then
		local m = #vr.list_missions+1
		vr.mission_data['Name'] = langt['mission']..' #' .. tostring(m)
		vr.list_missions[m] = {
			['Name'] = vr.mission_data['Name'],
			['Prename'] = new.char[128](),
			['Mission_Data'] = {
				['Targets'] = vr.list_targets,
				['Actors'] = vr.list_actors,
				['Cars'] = vr.list_cars,
				['Objects'] = vr.list_objects,
				['Pickup'] = vr.list_pickup,
				['Particle'] = vr.list_particle,
				['Explosion'] = vr.list_explosion,
				['Miss_data'] = vr.mission_data
			}
		}
		vr.list_name_missions[m] = vr.list_missions[m]['Name']
		vr.Fast_data['CurMiss'] = m-1
	end

	imgui.SameLine()
	if imgui.Button(langt['load']) and #vr.list_missions > 0 then
		lua_thread.create(load_mission)
	end

	imgui.SameLine()
	if imgui.Button(langt['save']) and #vr.list_missions > 0 then
		vr.list_missions[vr.lb_cur_missions[0]+1]['Mission_Data'] = {
			['Targets'] = vr.list_targets,
			['Actors'] = vr.list_actors,
			['Cars'] = vr.list_cars,
			['Objects'] = vr.list_objects,
			['Pickup'] = vr.list_pickup,
			['Particle'] = vr.list_particle,
			['Explosion'] = vr.list_explosion,
			['Miss_data'] = vr.mission_data
		}
		vr.Fast_data['CurMiss'] = vr.lb_cur_missions[0]
	end

	imgui.SameLine()
	if imgui.Button(langt['delete']) then
		vr.list_missions = DelCellArr(vr.list_missions,vr.lb_cur_missions[0]+1)
		vr.list_name_missions = DelCellArr(vr.list_name_missions,vr.lb_cur_missions[0]+1)
	end

	imgui.End()
	-- Настройки
	if  #vr.list_missions > 0 and vr.list_missions[vr.lb_cur_missions[0]+1] then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 300, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 150,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
		imgui.Begin(langt['settings']..': ' .. vr.list_missions[vr.lb_cur_missions[0]+1]['Name'])
		imgui.PushItemWidth(-120)
		imgui.InputText(langt['nameMission'],vr.list_missions[vr.lb_cur_missions[0]+1]['Prename'],ffi.sizeof(vr.list_missions[vr.lb_cur_missions[0]+1]['Prename']))
		if imgui.Button(langt['apply']) then
			vr.list_missions[vr.lb_cur_missions[0]+1]['Name'] = vr.list_missions[vr.lb_cur_missions[0]+1]['Prename'][0]
			vr.list_name_missions[vr.lb_cur_missions[0]+1] = vr.list_missions[vr.lb_cur_missions[0]+1]['Name']
			vr.mission_data['Name'] = vr.list_missions[vr.lb_cur_missions[0]+1]['Name']
		end
		imgui.Separator()
		if imgui.Button(langt['timeGameMiss']) then
			vr.editmode_timemiss = true
			printHelpForever('HMTIM')
			imgui.Process = false
		end
		
		if imgui.Combo(langt['weather'],vr.mission_data['Weather'],new('const char* const [?]', #Weather_str, Weather_str),#Weather_str) then
			forceWeatherNow(vr.mission_data['Weather'][0])
		end
		imgui.Checkbox(langt['modeRiot'],vr.mission_data['Riot'])
		imgui.End()
	end
end)

-- Паки миссий
imgui.OnFrame(function() return vr.pack_mission_window[0] and imgui.Process end, 
function()
  local langt = LanguageArr[vr.curr_lang[0]+1]['Keys']

  local resX,resY = getScreenResolution()
  local sizeX,sizeY = 300, 340
  imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
  imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2,(resY-sizeY)/2),imgui.Cond.FirstUseEver)
  imgui.Begin(langt['saveLoadMissPack'], vr.pack_mission_window)

  imgui.PushItemWidth(-10)


  --Список Объектов
  imgui.ListBoxStr_arr('',vr.lb_cur_pack_mission,new('const char* const [?]', #vr.list_name_mission_pack, vr.list_name_mission_pack),#vr.list_name_mission_pack,15)
  if imgui.BeginPopupContextItem('hee') then
    imgui.InputText('',vr.buf_edit_mission_pack_name)
    if imgui.Button(langt['apply']) and vr.buf_edit_mission_pack_name[0] ~= '' then
      vr.list_mission_pack[vr.lb_cur_pack_mission[0]+1]['Name'] = vr.buf_edit_mission_pack_name[0]
      vr.list_name_mission_pack[vr.lb_cur_pack_mission[0]+1] = vr.buf_edit_mission_pack_name[0]
      vr.buf_edit_mission_pack_name[0] = ''
      imgui.CloseCurrentPopup()
    end
    imgui.PushItemWidth(-1);
    imgui.PopItemWidth();
    imgui.EndPopup();
	imgui.End()
  end

  --Кнопки редактирования
  if imgui.Button(langt['add']) then
    local m = #vr.list_mission_pack+1
    vr.list_mission_pack[m] = {
      ['Name'] = langt['add']..' #' .. tostring(#vr.list_mission_pack+1),
      ['Enable'] = new.bool(false),
      ['Missions'] = vr.list_missions
    }
    vr.list_name_mission_pack[m] = vr.list_mission_pack[m]['Name']
    manager.save(vr.list_mission_pack[m],m-1)
    vr.Fast_data['CurPack'] = m-1
  end

  imgui.SameLine()
  if imgui.Button(langt['load']) and #vr.list_mission_pack > 0 then
    for i = 1,#vr.list_actors do
      deleteChar(vr.list_actors[i]['Actor_Data']['Char'])
    end
    for c = 1,#vr.list_cars do
      deleteCar(vr.list_cars[c]['Car_Data']['Car'])
    end
    vr.list_name_missions = {}
    vr.list_missions = vr.list_mission_pack[vr.lb_cur_pack_mission[0]+1]['Missions']
    vr.Fast_data['CurPack'] = vr.lb_cur_pack_mission[0]
    for m = 1,#vr.list_mission_pack[vr.lb_cur_pack_mission[0]+1]['Missions'] do
      vr.list_name_missions[m] = vr.list_mission_pack[vr.lb_cur_pack_mission[0]+1]['Missions'][m]['Name']
    end
  end
  imgui.SameLine()
  if imgui.Button(langt['save']) and #vr.list_mission_pack > 0 then
    vr.list_mission_pack[vr.lb_cur_pack_mission[0]+1]['Missions'] = vr.list_missions
    manager.save(vr.list_mission_pack[vr.lb_cur_pack_mission[0]+1],vr.lb_cur_pack_mission[0])
    vr.Fast_data['CurPack'] = vr.lb_cur_pack_mission[0]
  end

  imgui.SameLine()
  if imgui.Button(langt['delete']) then
    vr.list_mission_pack = DelCellArr(vr.list_mission_pack,vr.lb_cur_pack_mission[0]+1)
    vr.list_name_mission_pack = DelCellArr(vr.list_name_mission_pack,vr.lb_cur_pack_mission[0]+1)
    manager.delete(vr.lb_cur_pack_mission[0])
  end
end)

-- Цели
imgui.OnFrame(function() return vr.targets_window[0] and imgui.Process end, 
function()
	local langt = LanguageArr[vr.curr_lang[0]+1]['Keys']

  local resX,resY = getScreenResolution()
  local sizeX,sizeY = 300, 360
  imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
  imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 150,(resY-sizeY)/2),imgui.Cond.FirstUseEver)
  imgui.Begin(langt['targets'], vr.targets_window)

  imgui.PushItemWidth(-10)


  --Список Объектов
  imgui.ListBoxStr_arr('', vr.lb_cur_targets,new('const char* const [?]',#vr.list_name_targets,vr.list_name_targets),#vr.list_name_targets,15)
  if imgui.BeginPopupContextItem('hee') then
    imgui.InputText('',vr.buf_edit_targets_name,ffi.sizeof(vr.buf_edit_targets_name))
    if imgui.Button(langt['apply']) and vr.buf_edit_targets_name[0] ~= '' then
      vr.list_targets[vr.lb_cur_targets[0]+1]['Name'] =  ffi.string(vr.buf_edit_targets_name)
      vr.list_name_targets[vr.lb_cur_targets[0]+1] = ffi.string(vr.buf_edit_targets_name)
      imgui.StrCopy(vr.buf_edit_targets_name,'')
	  imgui.CloseCurrentPopup()
    end
    imgui.PushItemWidth(-1);
    imgui.PopItemWidth();
    imgui.EndPopup();
  end

  --Кнопки редактирования
  if imgui.Button(langt['add']) then
    vr.list_targets[#vr.list_targets+1] = {
      ['Name'] = langt['target']..' #' .. tostring(#vr.list_targets+1),
      ['Type'] = new.int(-1),
      ['Target_Data'] = {}
	}
    vr.list_name_targets[#vr.list_targets] = vr.list_targets[#vr.list_targets]['Name']
  end

  imgui.SameLine()
  if imgui.Button(langt['delete']) then
    if vr.list_targets[vr.lb_cur_targets[0]+1] then
      vr.list_targets = DelCellArr(vr.list_targets,vr.lb_cur_targets[0]+1)
      vr.list_name_targets = DelCellArr(vr.list_name_targets,vr.lb_cur_targets[0]+1)
    end
  end
  imgui.SameLine()
  if imgui.Button(langt['cut']) and #vr.list_targets > 0 then
    vr.buffer_target = vr.lb_cur_targets[0] + 1
  end
  imgui.SameLine()
  if vr.buffer_target ~= -1 then
    if imgui.Button(langt['paste']) then
      vr.list_targets = MoveCellArr(vr.list_targets,vr.buffer_target,vr.lb_cur_targets[0]+1)
      vr.list_name_targets = MoveCellArr(vr.list_name_targets,vr.buffer_target,vr.lb_cur_targets[0]+1)
      vr.buffer_target = -1
    end
  end

  imgui.End()

  --Редактор цели
	if #vr.list_targets > 0 and vr.list_targets[vr.lb_cur_targets[0]+1] then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 300, 340
		local targets_list_arr = decodeJson(langt['targets_list_arr'])
		local targets_marker_color = decodeJson(langt['targets_marker_color'])
		local weap_names = decodeJson(langt['weap_names'])
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 150,(resY-sizeY)/2),imgui.Cond.FirstUseEver)
		imgui.Begin(vr.list_targets[vr.lb_cur_targets[0]+1]['Name'])
		imgui.PushItemWidth(-65)
		if imgui.Combo(langt['typeTarget'],vr.list_targets[vr.lb_cur_targets[0]+1]['Type'],new('const char* const [?]',#targets_list_arr,targets_list_arr),#targets_list_arr) then
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data'] = {}
			local xx,xy,xz = getCharCoordinates(PLAYER_PED)
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Type'][0] == 0 then
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Pos'] = new.float[3](xx,xy,xz)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Radius'] = new.int(2)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text'] = new.char[128]()
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text_time'] = new.float(2)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Color_blip'] = new.int(0)
			end
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Type'][0] == 1 then
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_car_id'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Color_blip'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text'] = new.char[128]()
			end
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Type'][0] == 2 then
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_actor_id'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Color_blip'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text'] = new.char[128]()
			end
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Type'][0] == 3 then
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'] = new.int(-1)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Pos'] = new.float[3](xx,xy,xz)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Rotates'] = new.float[3](0,0,0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text'] = new.char[128]()
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text_time'] = new.float(2)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Smooth'] = new.bool(false)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Time'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Weather'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Clock_time'] = {0,0}
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Traffic'] = {new.int(1),new.int(1)}
			end
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Type'][0] == 4 then
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_object_id'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Color_blip'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'] = new.int(-1)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Weap'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text'] = new.char[128]()
			end
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Type'][0] == 5 then
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_pickup_id'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Color_blip'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text'] = new.char[128]()
			end
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Type'][0] == 6 then
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'] = new.int(-1)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Pos'] = new.float[3](xx,xy,xz)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['ModelID'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Angle'] = new.float(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Weapon'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Weap_ammo'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Anim'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Pack_anim'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Loop'] = new.bool(false)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Car_id'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Car_place'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Level_battue'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Dialog'] = {}
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Dialog_id'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Add_money'] = new.int(0)
			vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Interior_id'] = getActiveInterior()
			end
		
		end

		if vr.list_targets[vr.lb_cur_targets[0]+1]['Type'][0] == 0 then
			imgui.InputFloat3(langt['position'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Pos'])
			if imgui.Button(langt['modeMove']) then
			vr.editmode_target = true
			printHelpForever('HTARG')
			lockPlayerControl(true)
			vr.id_target = vr.lb_cur_targets[0]+1
			imgui.Process = false
			end
			imgui.PushItemWidth(-130)
			imgui.DragInt(langt['radiusCheckpoint'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Radius'],0.1,-100,100)
			imgui.Combo(langt['colorMarker'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Color_blip'],new('const char* const [?]',#targets_marker_color,targets_marker_color),#targets_marker_color)
			imgui.InputText(langt['textTarget'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text'],ffi.sizeof(vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text']))
			imgui.InputFloat(langt['timeText'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text_time'])
		end
		if vr.list_targets[vr.lb_cur_targets[0]+1]['Type'][0] == 1 then
			imgui.Combo(langt['car'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_car_id'],new('const char* const [?]',#vr.list_name_cars,vr.list_name_cars),#vr.list_name_cars)
			imgui.Combo(langt['colorMarker'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Color_blip'],new('const char* const [?]',#targets_marker_color,targets_marker_color),#targets_marker_color)
			imgui.InputText(langt['text'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text'],ffi.sizeof(vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text']))
		end
		if vr.list_targets[vr.lb_cur_targets[0]+1]['Type'][0] == 2 then
			imgui.Combo(langt['actor'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_actor_id'],new('const char* const [?]',#vr.list_name_actors,vr.list_name_actors),#vr.list_name_actors)
			imgui.Combo(langt['colorMarker'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Color_blip'],new('const char* const [?]',#targets_marker_color,targets_marker_color),#targets_marker_color)
			imgui.InputText(langt['text'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text'],ffi.sizeof(vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text']))
		end
		if vr.list_targets[vr.lb_cur_targets[0]+1]['Type'][0] == 3 then
			local target_type = decodeJson(langt['target_type'])
			imgui.Combo(langt['type'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'],new('const char* const [?]',#target_type,target_type),#target_type)
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'][0] == 0 then
				imgui.InputFloat3(langt['position'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Pos'])
				imgui.InputFloat3(langt['rotate'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Rotates'])
				if imgui.Button(langt['modeMove']) then
					vr.editmode_camera = true
					printHelpForever('HOBJ')
					lockPlayerControl(true)
					vr.id_target = vr.lb_cur_targets[0]+1
					imgui.Process = false
				end
				imgui.Checkbox(langt['movecam'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Smooth'])
				imgui.InputText(langt['text'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text'],ffi.sizeof(vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text']))
				imgui.InputFloat(langt['timecutscene'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text_time'])
			end

			if vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'][0] == 1 then
				imgui.InputInt(langt['time'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Time'])
			end
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'][0] == 2 then
				imgui.InputText(langt['text'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text'],ffi.sizeof(vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text']))
				imgui.InputInt(langt['timeout'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Time'])
			end
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'][0] == 3 then
				if imgui.Combo(langt['weather'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Weather'],new('const char* const [?]',#Weather_str,Weather_str),#Weather_str) then
					forceWeatherNow(vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Weather'][0])
				end
			end
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'][0] == 4 then
				if imgui.Button(langt['timeGameMiss']) then
					vr.editmode_timetarg = true
					printHelpForever('HMTIM')
					imgui.Process = false
					vr.id_timetarg = vr.lb_cur_targets[0]+1
				end
			end
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'][0] == 5 then
				local count = decodeJson(langt['count_traffic'])
				imgui.PushItemWidth(-100)
				imgui.Combo(langt['countPed'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Traffic'][1],new('const char* const [?]',#count,count),#count)
				imgui.Combo(langt['countCar'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Traffic'][2],new('const char* const [?]',#count,count),#count)
			end
		end
		if vr.list_targets[vr.lb_cur_targets[0]+1]['Type'][0] == 4 then
			local target_type = decodeJson(langt['target_type_obj'])
			imgui.Combo(langt['object'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_object_id'],new('const char* const [?]',#vr.list_name_objects,vr.list_name_objects),#vr.list_name_objects)
			imgui.Combo(langt['type'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'],new('const char* const [?]',#target_type,target_type),#target_type)
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'][0] == 3 then
				imgui.Combo(langt['weapon'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Weap'],new('const char* const [?]',#weap_names,weap_names),#weap_names)
			end
			imgui.Combo(langt['colorMarker'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Color_blip'],new('const char* const [?]',#targets_marker_color,targets_marker_color),#targets_marker_color)
			imgui.InputText(langt['text'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text'],ffi.sizeof(vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text']))
		end
		if vr.list_targets[vr.lb_cur_targets[0]+1]['Type'][0] == 5 then
			imgui.Combo(langt['pickup'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_pickup_id'],new('const char* const [?]',#vr.list_name_pickup,vr.list_name_pickup),#vr.list_name_pickup)
			imgui.Combo(langt['colorMarker'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Color_blip'],new('const char* const [?]',#targets_marker_color,targets_marker_color),#targets_marker_color)
			imgui.InputText(langt['text'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text'],ffi.sizeof(vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text']))
		end
		if vr.list_targets[vr.lb_cur_targets[0]+1]['Type'][0] == 6 then
			local target_type = decodeJson(langt['target_type_ev'])
			imgui.Combo(langt['type'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'],new('const char* const [?]',#target_type,target_type),#target_type)
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'][0] == 0 then
				imgui.InputFloat3(langt['position'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Pos'])
				imgui.DragFloat(langt['angle'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Angle'],0.5,-360,360)
				imgui.InputInt(langt['model'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['ModelID'])
				imgui.PushItemWidth(-100)
				imgui.InputInt(langt['countAmmo'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Weap_ammo'],10)
				if imgui.Button(langt['edithand']) then
					vr.id_teleport_player = vr.lb_cur_targets[0]+1
					imgui.Process = false
					printHelpForever('HPLA')
					vr.editmode_teleport_player = true
				end
			end
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'][0] == 1 then
				imgui.Combo(langt['pack'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Pack_anim'],new('const char* const [?]',#Anims['Anim_name'],Anims['Anim_name']),#Anims['Anim_name'])
				imgui.Combo(langt['anim'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Anim'],new('const char* const [?]',#Anims['Anim_list'][vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Pack_anim'][0]+1],Anims['Anim_list'][vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Pack_anim'][0]+1]),#Anims['Anim_list'][vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Pack_anim'][0]+1])
				imgui.Checkbox(langt['looped'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Loop'])
				if imgui.Button(langt['preview']) then
					vr.id_preview_player_anim = vr.lb_cur_targets[0]+1
					vr.editmode_preview_player_anim = true
					lockPlayerControl(true)
					imgui.Process = false
					printHelpForever('HVIEW')
				end
			end
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'][0] == 2 then
				local place_car = decodeJson(langt['place_car'])
				imgui.Combo(langt['car'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Car_id'],new('const char* const [?]',#vr.list_name_cars,vr.list_name_cars),#vr.list_name_cars)
				imgui.Combo(langt['place'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Car_place'],new('const char* const [?]',#place_car,place_car),#place_car)
			end
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'][0] == 3 then
				imgui.PushItemWidth(-100)
				imgui.SliderInt(langt['levelp'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Level_battue'],0,6)
			end
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'][0] == 5 then
				imgui.Text(langt['dialog'])
				if imgui.Button(langt['add']) then
					vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Dialog'][#vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Dialog']+1] = {
					['Text'] = new.char[128](),
					['Text_time'] = new.float(0)
					}
				end
				imgui.PushItemWidth(-150)
				imgui.SameLine()
				local rang = range(#vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Dialog'])
				imgui.Combo(langt['number'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Dialog_id'],new('const char* const [?]',#rang,rang),#rang)
				imgui.SameLine()
				if imgui.Button(langt['delete']) then
				vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Dialog'] = DelCellArr(vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Dialog'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Dialog_id'][0]+1)
				end
				imgui.PushItemWidth(-70)
				for d = 1,#vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Dialog'] do
					if imgui.TreeNodeStr(u8:encode(d)) then
					imgui.InputText(langt['text'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Dialog'][d]['Text'],ffi.sizeof(vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Dialog'][d]['Text']))
					imgui.InputFloat(langt['time'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Dialog'][d]['Text_time'])
					imgui.TreePop()
					end
				end
				--imgui.InputText(langt['text'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Text'])
			end
			if vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Target_type'][0] == 6 then
				imgui.InputInt(langt['add'],vr.list_targets[vr.lb_cur_targets[0]+1]['Target_Data']['Add_money'])
			end
		end
		imgui.End()
	end

end)

--Актёры
imgui.OnFrame(function() return vr.actors_window[0] and imgui.Process end, 
function()
	local langt = LanguageArr[vr.curr_lang[0]+1]['Keys']

	local resX,resY = getScreenResolution()
	local sizeX,sizeY = 300, 340
	imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
	imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 175,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
	imgui.Begin(langt['actors'])
	imgui.PushItemWidth(-10)

	--Список Актёров
	imgui.ListBoxStr_arr('', vr.lb_cur_actors,new('const char* const [?]',#vr.list_name_actors,vr.list_name_actors),#vr.list_name_actors,15)
	if imgui.BeginPopupContextItem('hee') then
		imgui.InputText('',vr.buf_edit_actors_name,ffi.sizeof(vr.buf_edit_actors_name))
		if imgui.Button(langt['apply']) and vr.buf_edit_actors_name[0] ~= '' then
			vr.list_actors[vr.lb_cur_actors[0]+1]['Name'] = ffi.string(vr.buf_edit_actors_name)
			vr.list_name_actors[vr.lb_cur_actors[0]+1] = ffi.string(vr.buf_edit_actors_name)
			imgui.StrCopy(vr.buf_edit_actors_name,'')
			imgui.CloseCurrentPopup()
		end
		imgui.PushItemWidth(-1);
		imgui.PopItemWidth();
		imgui.EndPopup();
	end
	--Кнопки редактирования
	if imgui.Button(langt['add']) then
		local xx,xy,xz = getCharCoordinates(PLAYER_PED)
		xz = getGroundZFor3dCoord(xx,xy,xz)
		local angle = getCharHeading(PLAYER_PED)
		vr.list_actors[#vr.list_actors+1] = {
			['Name'] = langt['actor']..' #' .. tostring(#vr.list_actors+1),
			['Type'] = new.int(-1),
			['Actor_Data'] = {
				['Pos'] = new.float[3](xx,xy,xz),
				['Angle'] = new.float(angle),
				['ModelId'] = new.int(0),
				['StartC'] = new.int(0),
				['EndC'] = new.int(0),
				['Anims'] = {},
				['Anim_id'] = new.int(0),
				['Should_not_die'] = new.bool(false),
				['Health'] = new.int(100)
			}
		}
		upd_actor:run(#vr.list_actors)
		vr.list_name_actors[#vr.list_actors] = vr.list_actors[#vr.list_actors]['Name']
	end
	imgui.SameLine()
	if imgui.Button(langt['delete']) then
		deleteChar(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Char'])
		vr.list_actors = DelCellArr(vr.list_actors,vr.lb_cur_actors[0]+1)
		vr.list_name_actors = DelCellArr(vr.list_name_actors,vr.lb_cur_actors[0]+1)
	end
	-- if imgui.Button(u8'Дублировать') then
	-- 	vr.list_actors[#vr.list_actors+1] = {
	-- 		['Name'] = vr.list_actors[vr.lb_cur_actors[0]+1]['Name'] .. u8'д',
	-- 		['Type'] = new.int(vr.list_actors[vr.lb_cur_actors[0]+1]['Type'][0]),
	-- 		['Enable'] = new.bool(vr.list_actors[vr.lb_cur_actors[0]+1]['Enable'][0]),
	-- 		['Actor_Data'] = {
	-- 			['Pos'] = new.float[3](vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Pos'][0],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Pos'][1],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Pos'][2]),
	-- 			['Angle'] = new.float(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Angle'][0]),
	-- 			['ModelId'] = new.int(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['ModelId'][0]),
	-- 			['StartC'] = new.int(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['StartC'][0]),
	-- 			['EndC'] = new.int(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['EndC'][0]),
	-- 			['Anims'] = {}
	-- 		}
	-- 	}
	-- 	vr.list_name_actors[#vr.list_actors] = vr.list_actors[vr.lb_cur_actors[0]+1]['Name']
	-- end
	imgui.End()

	--Редактор актёра
	if #vr.list_actors > 0 and vr.list_actors[vr.lb_cur_actors[0]+1] then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 400, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 175,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
		imgui.Begin(vr.list_actors[vr.lb_cur_actors[0]+1]['Name'])
		imgui.PushItemWidth(-90)
		
		if imgui.InputFloat3(langt['position'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Pos']) or imgui.SliderFloat(langt['rotate'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Angle'],-360,360) then
		  local xx,xy,xz = vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Pos'][0],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Pos'][1],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Pos'][2]
		  setCharCoordinates(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Char'],xx,xy,xz)
		  setCharHeading(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Char'], vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Angle'][0])
		end
		if imgui.InputInt(langt['model'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['ModelId']) then
		  local id_a = 0
		  for v = 1,#ID_Actors do
			if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['ModelId'][0] <= ID_Actors[v] then
			  id_a = ID_Actors[v]
			  break
			end
		  end
		  vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['ModelId'][0] = id_a
		  upd_actor:run(vr.lb_cur_actors[0]+1)
		end
		imgui.InputInt(langt['countlive'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Health'])
		imgui.Checkbox(langt['should_live'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Should_not_die'])
		if imgui.Button(langt['edithand']) then
		  vr.editmode_actor = true
		  vr.id_actor = vr.lb_cur_actors[0]+1
		  deleteChar(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Char'])
		  printHelpForever('HACT')
		end
		
		imgui.PushItemWidth(-110)
		
		imgui.Separator()
		local list_tg_m = {langt['toend']}
		for ltg = 1,#vr.list_name_targets do
		  list_tg_m[#list_tg_m+1] = vr.list_name_targets[ltg]
		end
		imgui.Combo(langt['app_on'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['StartC'],new('const char* const [?]',#vr.list_name_targets,vr.list_name_targets),#vr.list_name_targets)
		imgui.Combo(langt['dis_after'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['EndC'],new('const char* const [?]',#list_tg_m,list_tg_m),#list_tg_m)
		imgui.Separator()
		imgui.Text(langt['actions'])
		
		if imgui.Button(langt['add']) then
		  vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][#vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims']+1] = {
			['Type'] = new.int(-1),
		  }
		end
		imgui.SameLine()
		imgui.PushItemWidth(-250)
		local rang = range(#vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'])
		imgui.Combo(langt['number'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anim_id'],new('const char* const [?]',#rang,rang),#rang)
		imgui.SameLine()
		if imgui.Button(langt['delete']) then
		  vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'] = DelCellArr(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anim_id'][0]+1)
		end
		if imgui.Button(langt['del_preview']) then
		  upd_actor:run(vr.lb_cur_actors[0]+1)
		end
		imgui.PushItemWidth(-150)
		for a = 1,#vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'] do
			if imgui.TreeNodeStr(u8:encode(a)) then
				local type = decodeJson(langt['type_anim_actr'])
				if imgui.Combo(langt['type'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type'],new('const char* const [?]',#type,type),#type) then
					if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type'][0] == 0 then
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Anim'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Pack_anim'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Loop'] = new.bool(false)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Time'] = new.float(1.0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Unbreakable'] = new.bool(false)
					end
					if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type'][0] == 1 then
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Path'] = {}
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type_move'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type_route'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Cur_point'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Vis_point'] = new.bool(false)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['wait_end'] = new.bool(false)
					end
					if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type'][0] == 2 then
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Path'] = {}
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Car'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Speed'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Cur_point'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Vis_point'] = new.bool(false)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['wait_end'] = new.bool(false)
					end
					if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type'][0] == 3 then
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['wait_end'] = new.bool(false)
					end
					if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type'][0] == 4 then
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Car_a'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Car_t'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Speed'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['trafficFlag'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Vehicle_mission'] = new.int(0)
					end
					if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type'][0] == 5 then
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['place_in_car'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Car'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['speed_walk'] = new.int(0)
						vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['wait_end'] = new.bool(false)
					end
				end
				imgui.Separator()
				if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type'][0] == 0 then
					local cond_type = decodeJson(langt['cond_type'])
					if imgui.Combo(langt['cond'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'],new('const char* const [?]',#cond_type,cond_type),#cond_type) then
						if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'][0] == 1 then
							vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Target'] = new.int(0)
						end
					end
					if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'][0] == 1 then
						imgui.Combo(langt['target'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Target'],new('const char* const [?]',#vr.list_name_targets,vr.list_name_targets),#vr.list_name_targets)
					end
					imgui.Separator()
					imgui.Combo(langt['pack'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Pack_anim'],new('const char* const [?]',#Anims['Anim_name'],Anims['Anim_name']),#Anims['Anim_name'])
					imgui.Combo(langt['anim'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Anim'],new('const char* const [?]',#Anims['Anim_list'][vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Pack_anim'][0]+1],Anims['Anim_list'][vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Pack_anim'][0]+1]),#Anims['Anim_list'][vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Pack_anim'][0]+1])
					imgui.InputFloat(langt['time_anim'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Time'],0)
					imgui.Checkbox(langt['looped'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Loop'])
					imgui.Checkbox(langt['unbreak'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Unbreakable'])
					if imgui.Button(langt['preview']) then
						upd_anim_actor:run(vr.lb_cur_actors[0]+1,a)
					end
					imgui.Button(langt['delete'])
				end
				if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type'][0] == 1 then
					local cond_type = decodeJson(langt['cond_type'])
					if imgui.Combo(langt['cond'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'],new('const char* const [?]',#cond_type,cond_type),#cond_type) then
						if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'][0] == 1 then
							vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Target'] = new.int(0)
						end
					end
					if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'][0] == 1 then
						imgui.Combo(langt['target'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Target'],new('const char* const [?]',#vr.list_name_targets,vr.list_name_targets),#vr.list_name_targets)
					end
					imgui.Separator()
					local move_type = decodeJson(langt['move_type_ped'])
					local move_route = decodeJson(langt['move_route_ped'])
					imgui.Combo(langt['type_move_ped'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type_move'],new('const char* const [?]',#move_type,move_type),#move_type)
					local list_name_point = {}
					imgui.Combo(langt['type_route_ped'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type_route'],new('const char* const [?]',#move_route,move_route),#move_route)
					for i = 1, #vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Path'] do
						local point = vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Path'][i]
						list_name_point[i] = tostring(point[1])..' '..tostring(point[2]) ..' '..tostring(point[3])
					end
					if imgui.Checkbox(langt['view_path'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Vis_point']) then
						if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Vis_point'][0] then
							lua_thread.create(function(anim)
								local font = renderCreateFont('Verdana',12)
								while anim['Vis_point'][0] do
									wait(0)
									vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Path'][0] = {vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Pos'][0],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Pos'][1],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Pos'][2]}
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
							vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a])
						end
						
					end
					imgui.ListBoxStr_arr(langt['point_path'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Cur_point'],new('const char* const [?]',#list_name_point,list_name_point),#list_name_point,4)
					if imgui.Button(langt['add']) then
						local xx,xy,xz = getCharCoordinates(PLAYER_PED)
						table.insert(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Path'],{xx,xy,xz})
					end
					imgui.SameLine()
					if imgui.Button(langt['add_enter']) then
						imgui.Process = false;
						vr.id_actor_anim = a;
						vr.editmode_actor_add_point = true;
					end
					imgui.SameLine()
					if imgui.Button(langt['delete']) then
						table.remove(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Path'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Cur_point'][0]+1)
					end
					imgui.Separator()
					if imgui.Button(langt['perform']) then
						local xx,xy,xz = vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Pos'][0],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Pos'][1],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Pos'][2]
						local dec = loadCharDecisionMaker(0)
						setCharProofs(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Char'],true,true,true,true,true)
						setCharDecisionMaker(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Char'],dec)
						setCharCoordinates(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Char'],xx,xy,xz)
						setCharCollision(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Char'],true)
						lua_thread.create(function(path)
							flushPatrolRoute()
							local type_walk = 4;
							if path['Type_move'][0] == 2 then type_walk = 6
							elseif path['Type_move'][0] == 3 then type_walk = 7 end
							taskToggleDuck(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Char'], ternar(path['Type_move'][0] == 0,true,false))
							for i = 1,#path['Path'] do
								local x1,y1,z1 = path['Path'][i][1],path['Path'][i][2],path['Path'][i][3]
								if i % 7 == 0 then
									wait(0)
									local px,py,pz = getCharCoordinates(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Char'])
									taskFollowPatrolRoute(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Char'],type_walk,path['Type_route'][0])
									while getDistanceBetweenCoords3d(path['Path'][i-1][1],path['Path'][i-1][2],path['Path'][i-1][3],px,py,pz) > 0.1 do
										wait(0)
										px,py,pz = getCharCoordinates(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Char'])
										print(getDistanceBetweenCoords3d(path['Path'][i][1],path['Path'][i][2],path['Path'][i][3],px,py,pz))
									end
									flushPatrolRoute()
								end
								extendPatrolRoute(x1,y1,z1,'NONE','NONE')
							end
							taskFollowPatrolRoute(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Char'],type_walk,path['Type_route'][0])
						end,vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a])
					end
					imgui.Checkbox(langt['wait_end'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['wait_end'])
				end
				if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type'][0] == 2 then
					local cond_type = decodeJson(langt['cond_type'])
					if imgui.Combo(langt['cond'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'],new('const char* const [?]',#cond_type,cond_type),#cond_type) then
						if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'][0] == 1 then
							vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Target'] = new.int(0)
						end
					end
					if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'][0] == 1 then
						imgui.Combo(langt['target'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Target'],new('const char* const [?]',#vr.list_name_targets,vr.list_name_targets),#vr.list_name_targets)
					end
					imgui.Separator()
					local list_name_point = {}
					for i = 1,#vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Path'] do
						local point = vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Path'][i]
						list_name_point[i] = tostring(point[1])..' '..tostring(point[2]) ..' '..tostring(point[3])
					end
					if imgui.Checkbox(langt['view_path'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Vis_point']) then
						if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Vis_point'][0] then
							lua_thread.create(function(anim)
								local font = renderCreateFont('Verdana',12)
								while anim['Vis_point'][0] do
									wait(0)
									if #vr.list_cars > 0 then
										vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Path'][0] = {vr.list_cars[vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Car'][0]+1]['Car_Data']['Pos'][0] or 0 ,vr.list_cars[vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Car'][0]+1]['Car_Data']['Pos'][1] or 0 ,vr.list_cars[vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Car'][0]+1]['Car_Data']['Pos'][2] or 0 }
									else
										vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Path'][0] = {0,0,0}
									end
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
							vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a])
						end
						
					end
					imgui.Combo(langt['car'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Car'],new('const char* const [?]',#vr.list_name_cars,vr.list_name_cars),#vr.list_name_cars)
					imgui.InputInt(langt['speed'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Speed'])
					imgui.ListBoxStr_arr(langt['point_path'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Cur_point'],list_name_point,4)
					if imgui.Button(langt['add']) then
						local xx,xy,xz = getCharCoordinates(PLAYER_PED)
						table.insert(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Path'],{xx,xy,xz})
					end
					imgui.SameLine()
					if imgui.Button(langt['add_enter']) then
						imgui.Process = false;
						vr.id_actor_anim = a;
						vr.editmode_actor_add_point = true;
					end
					imgui.SameLine()
					if imgui.Button(langt['delete']) then
						table.remove(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Path'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Cur_point'][0]+1)
					end
					imgui.Checkbox(langt['wait_end'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['wait_end'])
				end
				if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type'][0] == 3 then
					local cond_type = decodeJson(langt['cond_type'])
					if imgui.Combo(langt['cond'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'],new('const char* const [?]',#cond_type,cond_type),#cond_type) then
						if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'][0] == 1 then
							vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Target'] = new.int(0)
						end
					end
					if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'][0] == 1 then
						imgui.Combo(langt['target'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Target'],new('const char* const [?]',#vr.list_name_targets,vr.list_name_targets),#vr.list_name_targets)
					end
					imgui.Checkbox(langt['wait_end'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['wait_end'])
				end
				if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type'][0] == 4 then
					local drive_beh = decodeJson(langt['driver_beh'])
					local cond_type = decodeJson(langt['cond_type'])
					if imgui.Combo(langt['cond'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'],new('const char* const [?]',#cond_type,cond_type),#cond_type) then
						if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'][0] == 1 then
							vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Target'] = new.int(0)
						end
					end
					if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'][0] == 1 then
						imgui.Combo(langt['target'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Target'],new('const char* const [?]',#vr.list_name_targets,vr.list_name_targets),#vr.list_name_targets)
					end
					imgui.Separator()
					imgui.Combo(langt['car_actr'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Car_a'],new('const char* const [?]',#vr.list_name_cars,vr.list_name_cars),#vr.list_name_cars)
					imgui.InputInt(langt['max_speeed'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Speed'])
					imgui.InputInt(langt['beh_transp'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['trafficFlag'])
					imgui.Combo(langt['beh_driver'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Vehicle_mission'],new('const char* const [?]',#drive_beh,drive_beh),#drive_beh)
					imgui.Separator()
					imgui.Combo(langt,vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Car_t'],new('const char* const [?]',#vr.list_name_cars,vr.list_name_cars),#vr.list_name_cars)
				end
				if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Type'][0] == 5 then
					local place_in_car = decodeJson(langt['place_in_car'])
					local speed_walk_to_car = decodeJson(langt['speed_walk_to_car'])
					local cond_type = decodeJson(langt['cond_type'])
					if imgui.Combo(langt['cond'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'],new('const char* const [?]',#cond_type,cond_type),#cond_type) then
						if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'][0] == 1 then
							vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Target'] = new.int(0)
						end
					end
					if vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Condition'][0] == 1 then
						imgui.Combo(langt['target'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Target'],new('const char* const [?]',#vr.list_name_targets,vr.list_name_targets),#vr.list_name_targets)
					end
					imgui.Separator()
					imgui.Combo(langt['speed_walk'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['speed_walk'],new('const char* const [?]',#speed_walk_to_car,speed_walk_to_car),#speed_walk_to_car)
					imgui.Combo(langt['car'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['Car'],new('const char* const [?]',#vr.list_name_cars,vr.list_name_cars),#vr.list_name_cars)
					imgui.Combo(langt['seat'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['place_in_car'],new('const char* const [?]',#place_in_car,place_in_car),#place_in_car)
					imgui.Checkbox(langt['wait_end'],vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][a]['wait_end'])
				end
				imgui.TreePop()
			end
		end
		imgui.End()
	end
end)


-- Окно списка машин
imgui.OnFrame(function() return vr.cars_window[0] and imgui.Process end, 
function()
	local langt = LanguageArr[vr.curr_lang[0]+1]['Keys']

	local resX,resY = getScreenResolution()
	local sizeX,sizeY = 300, 340
	imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
	imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 150,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
	imgui.Begin(langt['cars'])

	imgui.PushItemWidth(-10)


	--Список Машин
	imgui.ListBoxStr_arr('', vr.lb_cur_cars,new('const char* const [?]',#vr.list_name_cars,vr.list_name_cars),#vr.list_name_cars,15)
	if imgui.BeginPopupContextItem('hee') then
		imgui.InputText('',vr.buf_edit_cars_name,ffi.sizeof(vr.buf_edit_cars_name))
		if imgui.Button(langt['apply']) and vr.buf_edit_cars_name[0] ~= '' then
			vr.list_cars[vr.lb_cur_cars[0]+1]['Name'] = ffi.string(vr.buf_edit_cars_name)
			vr.list_name_cars[vr.lb_cur_cars[0]+1] = ffi.string(vr.buf_edit_cars_name)
			imgui.StrCopy(vr.buf_edit_cars_name,'')
			imgui.CloseCurrentPopup()
		end
		imgui.PushItemWidth(-1);
		imgui.PopItemWidth();
		imgui.EndPopup();
	end

	--Кнопки редактирования
	if imgui.Button(langt['add']) then
		local xx,xy,xz = getCharCoordinates(PLAYER_PED)
		xz = getGroundZFor3dCoord(xx, xy, xz+10.0)
		vr.list_cars[#vr.list_cars+1] = {
			['Name'] = langt['car']..' #' .. tostring(#vr.list_cars+1),
			['Type'] = new.int(-1),
			['Car_Data'] = {
				['Pos'] = new.float[3](xx,xy,xz),
				['Angle'] = new.float(0),
				['ModelId'] = new.int(400),
				['StartC'] = new.int(0),
				['EndC'] = new.int(0),
				['Color_primary'] = new.int(0),
				['Color_secondary'] = new.int(0),
				['Should_not_die'] = new.bool(false),
				['Health'] = new.int(1000),
				['Bulletproof'] = new.bool(false),
				['Fireproof'] = new.bool(false),
				['Explosionproof'] = new.bool(false),
				['Collisionproof'] = new.bool(false),
				['Meleeproof'] = new.bool(false),
				['Tires_vulnerability'] = new.bool(false),
				['Anims'] = {},
				['Anim_id'] = new.int(0)
			}

		}
		upd_car:run(#vr.list_cars)
		vr.list_name_cars[#vr.list_cars] = vr.list_cars[#vr.list_cars]['Name']
	end

	imgui.SameLine()
	if imgui.Button(langt['delete']) then
		if vr.list_cars[vr.lb_cur_cars[0]+1] then
			deleteCar(vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Car'])
			vr.list_cars = DelCellArr(vr.list_cars,vr.lb_cur_cars[0]+1)
			vr.list_name_cars = DelCellArr(vr.list_name_cars,vr.lb_cur_cars[0]+1)
		end
	end
	imgui.End()
	--Редактор машин
	if #vr.list_cars > 0 and vr.list_cars[vr.lb_cur_cars[0]+1] then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 340, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 170,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
		imgui.Begin(vr.list_cars[vr.lb_cur_cars[0]+1]['Name'])
		imgui.PushItemWidth(-90)
		
		if imgui.InputFloat3(langt['position'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Pos']) or imgui.SliderFloat(langt['rotate'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Angle'],-360,360) then
			local xx,xy,xz = vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Pos'][0],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Pos'][1],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Pos'][2]
			setCarCoordinates(vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Car'],xx,xy,xz)
			setCarHeading(vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Car'], vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Angle'][0])
		end
		if imgui.InputInt(langt['model'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['ModelId']) then
			local id_c = 400
			for v = 1,#ID_Cars do
			if vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['ModelId'][0] <= ID_Cars[v] then
				id_c = ID_Cars[v]
				break
			end
			end
			vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['ModelId'][0] = id_c
			upd_car:run(vr.lb_cur_cars[0]+1)
		end
		if imgui.DragInt(langt['color_car']..' 1',vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Color_primary'],0.1,0,126) then
			changeCarColour(vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Car'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Color_primary'][0],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Color_secondary'][0])
		end
		if imgui.DragInt(langt['color_car']..' 2',vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Color_secondary'],0.1,0,126) then
			changeCarColour(vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Car'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Color_primary'][0],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Color_secondary'][0])
		end
		imgui.InputInt(langt['countlive'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Health'])
		local car_unbreak = decodeJson(langt['car_unbreak'])
		imgui.Checkbox(car_unbreak[1],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Bulletproof'])
		imgui.Checkbox(car_unbreak[2],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Fireproof'])
		imgui.Checkbox(car_unbreak[3],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Explosionproof'])
		imgui.Checkbox(car_unbreak[4],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Collisionproof'])
		imgui.Checkbox(car_unbreak[5],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Meleeproof'])
		imgui.Checkbox(car_unbreak[6],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Tires_vulnerability'])
		imgui.Checkbox(langt['should_live'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Should_not_die'])
		if imgui.Button(langt['edithand']) then
			vr.editmode_car = true
			vr.id_car = vr.lb_cur_cars[0]+1
			deleteCar(vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Car'])
			imgui.Process = false
		end
		
		imgui.PushItemWidth(-110)
		
		imgui.Separator()
		local list_tg_m = {langt['toend']}
		for ltg = 1,#vr.list_name_targets do
			list_tg_m[#list_tg_m+1] = vr.list_name_targets[ltg]
		end
		imgui.Combo(langt['app_on'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['StartC'],new('const char* const [?]',#vr.list_name_targets,vr.list_name_targets),#vr.list_name_targets)
		imgui.Combo(langt['dis_after'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['EndC'],new('const char* const [?]',#list_tg_m,list_tg_m),#list_tg_m)

		imgui.Text(langt['actions'])

		if imgui.Button(langt['add']) then
			vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][#vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims']+1] = {
			['Type'] = new.int(-1),
			}
		end
		imgui.SameLine()
		imgui.PushItemWidth(-150)
		local rang = range(#vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'])
		imgui.Combo(langt['number'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anim_id'],new('const char* const [?]',#rang,rang),#rang)
		imgui.SameLine()
		if imgui.Button(langt['delete']) then
			vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'] = DelCellArr(vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anim_id'][0]+1)
		end
		-- if imgui.Button(u8'Очистить предпросмотр/движение') then
		--   upd_actor:run(vr.lb_cur_actors[0]+1)
		-- end
		imgui.PushItemWidth(-150)
		for a = 1,#vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'] do
			if imgui.TreeNodeStr(u8:encode(a)) then
				local type = decodeJson(langt['type_car_anim'])
				if imgui.Combo(langt['type'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Type'],new('const char* const [?]',#type,type),#type) then
					if vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Type'][0] == 0 then
						vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Doors'] = {new.bool(false),new.bool(false),new.bool(false),new.bool(false),new.bool(false),new.bool(false)}
						vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Condition'] = new.int(0) 
					end
					if vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Type'][0] == 1 then
						vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Door_lock'] = new.int(0)
						vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Condition'] = new.int(0)
					end
				end
				if vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Type'][0] == 0 then
					local cond_type = decodeJson(langt['cond_type'])
					if imgui.Combo(langt['cond'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Condition'],new('const char* const [?]',#cond_type,cond_type),#cond_type) then
						if vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Condition'][0] == 1 then
							vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Target'] = new.int(0)
						end
					end
					if vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Condition'][0] == 1 then
						imgui.Combo(langt['target'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Target'],new('const char* const [?]',#vr.list_name_targets,vr.list_name_targets),#vr.list_name_targets)
					end
					imgui.Separator()
					imgui.Text(langt['open_door_to'])
					local door_car = decodeJson(langt['door_car'])
					imgui.Checkbox(door_car[1],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Doors'][1])
					imgui.Checkbox(door_car[2],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Doors'][2])
					imgui.Checkbox(door_car[3],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Doors'][3])
					imgui.Checkbox(door_car[4],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Doors'][4])
					imgui.Checkbox(door_car[5],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Doors'][5])
					imgui.Checkbox(door_car[6],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Doors'][6])
				end
				if vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Type'][0] == 1 then
					local cond_type = decodeJson(langt['cond_type'])
					if imgui.Combo(langt['cond'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Condition'],new('const char* const [?]',#cond_type,cond_type),#cond_type) then
						if vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Condition'][0] == 1 then
							vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Target'] = new.int(0)
						end
					end
					if vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Condition'][0] == 1 then
						imgui.Combo(langt['target'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Target'],new('const char* const [?]',#vr.list_name_targets,vr.list_name_targets),#vr.list_name_targets)
					end
					imgui.Separator()
					local open_close = decodeJson(langt['open_close'])
					imgui.Combo(langt['doors'],vr.list_cars[vr.lb_cur_cars[0]+1]['Car_Data']['Anims'][a]['Door_lock'],new('const char* const [?]',#open_close,open_close),#open_close)
				end
				imgui.TreePop()
			end
		end
		imgui.End()
	end
end)

-- Окно списка объектов
imgui.OnFrame(function() return vr.objects_window[0] and imgui.Process end, 
function()
	local langt = LanguageArr[vr.curr_lang[0]+1]['Keys']

	local resX,resY = getScreenResolution()
	local sizeX,sizeY = 300, 340
	imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
	imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 150,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
	imgui.Begin(langt['objects'])

	imgui.PushItemWidth(-10)


	--Список Объектов
	imgui.ListBoxStr_arr('', vr.lb_cur_objects,new('const char* const [?]',#vr.list_name_objects,vr.list_name_objects),#vr.list_name_objects,15)
	if imgui.BeginPopupContextItem('hee') then
		imgui.InputText('',vr.buf_edit_obj_name,ffi.sizeof(vr.buf_edit_obj_name))
		if imgui.Button(langt['apply']) and vr.buf_edit_obj_name[0] ~= '' then
			vr.list_objects[vr.lb_cur_objects[0]+1]['Name'] = ffi.string(vr.buf_edit_obj_name)
			vr.list_name_objects[vr.lb_cur_objects[0]+1] = ffi.string(vr.buf_edit_obj_name)
			imgui.StrCopy(vr.buf_edit_obj_name,'')
			imgui.CloseCurrentPopup()
		end
		imgui.PushItemWidth(-1);
		imgui.PopItemWidth();
		imgui.EndPopup();
	end

	--Кнопки редактирования
	if imgui.Button(langt['add']) then
		local xx,xy,xz = getCharCoordinates(PLAYER_PED)
		xz = getGroundZFor3dCoord(xx, xy, xz)
		vr.list_objects[#vr.list_objects+1] = {
			['Name'] = u8'Объект #' .. tostring(#vr.list_objects+1),
			['Type'] = new.int(-1),
			['Object_Data'] = {
				['Pos'] = new.float[3](xx,xy,xz),
				['Rotates'] = new.float[3](0,0,0),
				['ModelId'] = new.int(400),
				['StartC'] = new.int(0),
				['EndC'] = new.int(0),
				['Anim_id'] = new.int(0),
				['Anims'] = {}
			}

		}
		vr.list_name_objects[#vr.list_objects] = vr.list_objects[#vr.list_objects]['Name']
	end

	imgui.SameLine()
	if imgui.Button(langt['delete']) then
		if vr.list_objects[vr.lb_cur_objects[0]+1] then
			deleteObject(vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Obj'])
			vr.list_objects = DelCellArr(vr.list_objects,vr.lb_cur_objects[0]+1)
			vr.list_name_objects = DelCellArr(vr.list_name_objects,vr.lb_cur_objects[0]+1)
		end
	end
	imgui.End()
	--Редактор объектов
	if #vr.list_objects > 0 and vr.list_objects[vr.lb_cur_objects[0]+1] then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 360, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 180,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
		imgui.Begin(vr.list_objects[vr.lb_cur_objects[0]+1]['Name'], vr.list_objects[vr.lb_cur_objects[0]+1]['Enable'])
		imgui.PushItemWidth(-90)
	
		if imgui.InputFloat3(langt['position'],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Pos']) or imgui.InputFloat3(langt['rotate'],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Rotates']) then
			local xx,xy,xz = vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Pos'][0],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Pos'][1],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Pos'][2]
			local rxx,rxy,rxz = vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Rotates'][0],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Rotates'][1],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Rotates'][2]
			setObjectCoordinates(vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Obj'],xx,xy,xz)
			setObjectRotation(vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Obj'], rxx, rxy, rxz)
		end
		if imgui.Button(langt['edithand']) then
			if vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Obj'] ~= nil then
				vr.editmode_objects = true
				printHelpForever('HOBJ')
				lockPlayerControl(true)
				vr.id_obj = vr.lb_cur_objects[0]+1
				imgui.Process = false
			end
		end
		imgui.InputInt(langt['model'],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['ModelId'])
		if imgui.Button(langt['apply']) then
			upd_object:run(vr.lb_cur_objects[0]+1)
		end
	
		imgui.PushItemWidth(-110)
	
		imgui.Separator()
		local list_tg_m = {langt['toend']}
		for ltg = 1,#vr.list_name_targets do
			list_tg_m[#list_tg_m+1] = vr.list_name_targets[ltg]
		end
		imgui.Combo(langt['app_on'],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['StartC'],new('const char* const [?]',#vr.list_name_targets,vr.list_name_targets),#vr.list_name_targets)
		imgui.Combo(langt['dis_after'],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['EndC'],new('const char* const [?]',#list_tg_m,list_tg_m),#list_tg_m)
		imgui.Text(langt['anim'])
	
		if imgui.Button(langt['add']) then
			local xx,xy,xz = getCharCoordinates(PLAYER_PED)
			vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anims'][#vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anims']+1] = {
				['Pos'] = new.float[3](xx,xy,xz),
				['Rotates'] = new.float[3](0,0,0),
				['Time'] = new.float(1.0),
				['Condition'] = new.int(0),
			}
		end
		imgui.SameLine()
		imgui.PushItemWidth(-200)
		local rang = range(#vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anims'])
		imgui.Combo(langt['number'],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anim_id'],new('const char* const [?]',#rang,rang),#rang)
		imgui.SameLine()
		if imgui.Button(langt['delete']) then
			vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anims'] = DelCellArr(vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anims'],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anim_id'][0]+1)
		end
	
		imgui.PushItemWidth(-100)
		for a = 1,#vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anims'] do
			if imgui.TreeNodeStr(u8:encode(a)) then
				local cond_type = decodeJson(langt['cond_type'])
				if imgui.Combo(langt['cond'],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anims'][a]['Condition'],new('const char* const [?]',#cond_type,cond_type),#cond_type) then
					if vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anims'][a]['Condition'][0] == 1 then
						vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anims'][a]['Target'] = new.int(0)
					end
				end
				if vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anims'][a]['Condition'][0] == 1 then
					imgui.Combo(langt['target'],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anims'][a]['Target'],vr.list_name_targets)
				end
	
				imgui.InputFloat3(langt['position'],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anims'][a]['Pos'])
				imgui.InputFloat3(langt['rotate'],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anims'][a]['Rotates'])
				imgui.InputFloat(langt['time_anim'],vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Anims'][a]['Time'],0)
				if imgui.Button(langt['edithand']) then
					if vr.list_objects[vr.lb_cur_objects[0]+1]['Object_Data']['Obj'] ~= nil then
						vr.editmode_objects_anim = true
						printHelpForever('HOBJ')
						lockPlayerControl(true)
						vr.id_obj = vr.lb_cur_objects[0]+1
						vr.id_obj_anim = a
						imgui.Process = false
					end
				end
				imgui.TreePop()
			end
		end
		imgui.End()
	end
end)

imgui.OnFrame(function() return vr.pickup_window[0] and imgui.Process end, 
function()
	local langt = LanguageArr[vr.curr_lang[0]+1]['Keys']

	local resX,resY = getScreenResolution()
	local sizeX,sizeY = 300, 340
	imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
	imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 150,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
	imgui.Begin(langt['pickups'])

	imgui.PushItemWidth(-10)


	--Список Объектов
	imgui.ListBoxStr_arr('', vr.lb_cur_pickup,new('const char* const [?]',#vr.list_name_pickup,vr.list_name_pickup),#vr.list_name_pickup,15)
	if imgui.BeginPopupContextItem('hee') then
		imgui.InputText('',vr.buf_edit_pick_name,ffi.sizeof(vr.buf_edit_pick_name))
		if imgui.Button(langt['apply']) and vr.buf_edit_pick_name[0] ~= '' then
			vr.list_pickup[vr.lb_cur_pickup[0]+1]['Name'] = ffi.string(vr.buf_edit_pick_name)
			vr.list_name_pickup[vr.lb_cur_pickup[0]+1] = ffi.string(vr.buf_edit_pick_name)
			imgui.StrCopy(vr.buf_edit_pick_name,'')
			imgui.CloseCurrentPopup()
		end
		imgui.PushItemWidth(-1);
		imgui.PopItemWidth();
		imgui.EndPopup();
	end

	--Кнопки редактирования
	if imgui.Button(langt['add']) then
		local xx,xy,xz = getCharCoordinates(PLAYER_PED)
		xz = getGroundZFor3dCoord(xx, xy, xz)
		vr.list_pickup[#vr.list_pickup+1] = {
			['Name'] = langt['pickup']..' #' .. tostring(#vr.list_pickup+1),
			['Type'] = new.int(-1),
			['Pickup_Data'] = {
				['Type_pickup'] = new.int(-1),
				['Pos'] = new.float[3](xx,xy,xz),
				['StartC'] = new.int(0),
				['EndC'] = new.int(0),
				['spawn_type'] = new.int(0)
			}

		}
		vr.list_name_pickup[#vr.list_pickup] = vr.list_pickup[#vr.list_pickup]['Name']
	end

	imgui.SameLine()
	if imgui.Button(langt['delete']) then
		if vr.list_pickup[vr.lb_cur_pickup[0]+1] then
			removePickup(vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['Pick'])
			vr.list_pickup = DelCellArr(vr.list_pickup,vr.lb_cur_pickup[0]+1)
			vr.list_name_pickup = DelCellArr(vr.list_name_pickup,vr.lb_cur_pickup[0]+1)
		end
	end
	imgui.End()
	--Окно пикапа
	if #vr.list_pickup > 0 and vr.list_pickup[vr.lb_cur_pickup[0]+1] then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 300, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 150,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
		imgui.Begin(vr.list_pickup[vr.lb_cur_pickup[0]+1]['Name'], vr.list_pickup[vr.lb_cur_pickup[0]+1]['Enable'])
		imgui.PushItemWidth(-90)
		local type_pick = decodeJson(langt['type_pickup'])
		local spawn_type = decodeJson(langt['spawn_type_pickup'])
		imgui.Combo(langt['spawn'],vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['spawn_type'],new('const char* const [?]',#spawn_type,spawn_type),#spawn_type)
		if imgui.Combo(langt['type'],vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['Type_pickup'],new('const char* const [?]',#type_pick,type_pick),#type_pick) then
			if vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['Type_pickup'][0] == 0 then
				vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['Ammo'] = new.int(0)
				vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['Weapon'] = new.int(1)
			end
			if vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['Type_pickup'][0] == 5 then
				vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['ModelId'] = new.int(0)
			end
			if vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['Type_pickup'][0] ~= 5 then upd_pickup:run(vr.lb_cur_pickup[0]+1) end
		end
	
		if vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['Type_pickup'][0] == 0 then
			imgui.Separator()
			if imgui.Button(langt['edithand']) then
				vr.editmode_pickup = true
				imgui.Process = false
				vr.id_pick = vr.lb_cur_pickup[0]+1
				lockPlayerControl(true)
				printHelpForever('HPICW')
			end
			imgui.InputInt(langt['ammo'],vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['Ammo'])
			imgui.Separator()
		end
		if vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['Type_pickup'][0] >= 1 then
			if imgui.Button(langt['edithand']) then
				vr.editmode_pickup = true
				imgui.Process = false
				vr.id_pick = vr.lb_cur_pickup[0]+1
				lockPlayerControl(true)
				printHelpForever('HTARG')
			end
			if vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['Type_pickup'][0] == 5 then
				imgui.InputInt(langt['model'],vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['ModelId'])
				if imgui.Button(langt['apply']) then
					upd_pickup:run(vr.lb_cur_pickup[0]+1)
				end
			end
		end
	
		local list_tg_m = {langt['toend']}
		for ltg = 1,#vr.list_name_targets do
			list_tg_m[#list_tg_m+1] = vr.list_name_targets[ltg]
		end
		imgui.Combo(langt['app_on'],vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['StartC'],new('const char* const [?]',#vr.list_name_targets,vr.list_name_targets),#vr.list_name_targets)
		imgui.Combo(langt['dis_after'],vr.list_pickup[vr.lb_cur_pickup[0]+1]['Pickup_Data']['EndC'],new('const char* const [?]',#list_tg_m,list_tg_m),#list_tg_m)
	
		imgui.End()
	end

end)

imgui.OnFrame(function() return vr.particle_window[0] and imgui.Process end, 
function()
	local langt = LanguageArr[vr.curr_lang[0]+1]['Keys']

	local resX,resY = getScreenResolution()
	local sizeX,sizeY = 300, 340
	imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
	imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 150,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
	imgui.Begin(langt['particles'])

	imgui.PushItemWidth(-10)


	--Список Объектов
	imgui.ListBoxStr_arr('', vr.lb_cur_particle,new('const char* const [?]',#vr.list_name_particle,vr.list_name_particle),#vr.list_name_particle,15)
	if imgui.BeginPopupContextItem('hee') then
		imgui.InputText('',vr.buf_edit_prtcl_name,ffi.sizeof(vr.buf_edit_prtcl_name))
		if imgui.Button(langt['apply']) and vr.buf_edit_prtcl_name[0] ~= '' then
			vr.list_particle[vr.lb_cur_particle[0]+1]['Name'] = ffi.string(vr.buf_edit_prtcl_name)
			vr.list_name_particle[vr.lb_cur_particle[0]+1] = ffi.string(vr.buf_edit_prtcl_name)
			imgui.StrCopy(vr.buf_edit_prtcl_name[0],'')
			imgui.CloseCurrentPopup()
		end
		imgui.PushItemWidth(-1);
		imgui.PopItemWidth();
		imgui.EndPopup();
	end

	--Кнопки редактирования
	if imgui.Button(langt['add']) then
		local xx,xy,xz = getCharCoordinates(PLAYER_PED)
		xz = getGroundZFor3dCoord(xx, xy, xz)
		vr.list_particle[#vr.list_particle+1] = {
			['Name'] = langt['particle']..' #' .. tostring(#vr.list_particle+1),
			['Type'] = new.int(-1),
			['Particle_Data'] = {
				['Pos'] = new.float[3](xx,xy,xz),
				['Rotates'] = new.float[3](0,0,0),
				['ModelId'] = new.int(0),
				['StartC'] = new.int(0),
				['EndC'] = new.int(0)
			}

		}
		vr.list_name_particle[#vr.list_particle] = vr.list_particle[#vr.list_particle]['Name']
		upd_particle:run(#vr.list_particle)
	end

	imgui.SameLine()
	if imgui.Button(langt['delete']) then
		if vr.list_particle[vr.lb_cur_particle[0]+1] then
			killFxSystemNow(vr.list_particle[vr.lb_cur_particle[0]+1]['Particle_Data']['Prtcl'])
			vr.list_particle = DelCellArr(vr.list_particle,vr.lb_cur_particle[0]+1)
			vr.list_name_particle = DelCellArr(vr.list_name_particle,vr.lb_cur_particle[0]+1)
		end
	end
	imgui.End()
	--Редактор объектов
	if #vr.list_particle > 0 and vr.list_particle[vr.lb_cur_particle[0]+1] then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 300, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 150,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
		imgui.Begin(vr.list_particle[vr.lb_cur_particle[0]+1]['Name'], vr.list_particle[vr.lb_cur_particle[0]+1]['Enable'])
		imgui.PushItemWidth(-90)
	
		--imgui.InputFloat3(u8'Угол поворота',vr.list_particle[vr.lb_cur_particle[0]+1]['Particle_Data']['Rotates'])
		if imgui.InputFloat3(langt['position'],vr.list_particle[vr.lb_cur_particle[0]+1]['Particle_Data']['Pos']) then
			upd_particle:run(vr.lb_cur_particle[0]+1)
		end
		if imgui.Button(langt['edithand']) then
			if vr.list_particle[vr.lb_cur_particle[0]+1]['Particle_Data']['Prtcl'] ~= nil then
				vr.editmode_particle = true
				printHelpForever('HOBJ')
				lockPlayerControl(true)
				vr.id_prtcl = vr.lb_cur_particle[0]+1
				imgui.Process = false
			end
		end
		if imgui.Combo(langt['particle'],vr.list_particle[vr.lb_cur_particle[0]+1]['Particle_Data']['ModelId'],new('const char* const [?]',#Particle_name,Particle_name),#Particle_name) then
			upd_particle:run(vr.lb_cur_particle[0]+1)
		end
	
		imgui.PushItemWidth(-110)
	
		imgui.Separator()
		local list_tg_m = {langt['toend']}
		for ltg = 1,#vr.list_name_targets do
			list_tg_m[#list_tg_m+1] = vr.list_name_targets[ltg]
		end
		imgui.Combo(langt['app_on'],vr.list_particle[vr.lb_cur_particle[0]+1]['Particle_Data']['StartC'],new('const char* const [?]',#vr.list_name_targets,vr.list_name_targets),#vr.list_name_targets)
		imgui.Combo(langt['dis_after'],vr.list_particle[vr.lb_cur_particle[0]+1]['Particle_Data']['EndC'],new('const char* const [?]',#list_tg_m,list_tg_m),#list_tg_m)
	
		imgui.End()
	end

end)

imgui.OnFrame(function() return vr.explosion_window[0] and imgui.Process end, 
function()
	local langt = LanguageArr[vr.curr_lang[0]+1]['Keys']

	local resX,resY = getScreenResolution()
	local sizeX,sizeY = 300, 340
	imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
	imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 + 150,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
	imgui.Begin(langt['explosions'])

	imgui.PushItemWidth(-10)


	--Список Объектов
	imgui.ListBoxStr_arr('', vr.lb_cur_explosion,new('const char* const [?]',#vr.list_name_explosion,vr.list_name_explosion),#vr.list_name_explosion,15)
	if imgui.BeginPopupContextItem('hee') then
		imgui.InputText('',vr.buf_edit_explosion_name,ffi.sizeof(vr.buf_edit_explosion_name))
		if imgui.Button(langt['apply']) and vr.buf_edit_explosion_name[0] ~= '' then
			vr.list_explosion[vr.lb_cur_explosion[0]+1]['Name'] = ffi.string(vr.buf_edit_explosion_name)
			vr.list_name_explosion[vr.lb_cur_explosion[0]+1] = ffi.string(vr.buf_edit_explosion_name)
			imgui.StrCopy(vr.buf_edit_explosion_name,'')
			imgui.CloseCurrentPopup()
		end
		imgui.PushItemWidth(-1);
		imgui.PopItemWidth();
		imgui.EndPopup();
	end

	--Кнопки редактирования
	if imgui.Button(langt['add']) then
		local xx,xy,xz = getCharCoordinates(PLAYER_PED)
		xz = getGroundZFor3dCoord(xx, xy, xz)
		vr.list_explosion[#vr.list_explosion+1] = {
			['Name'] = langt['explosion']..' #' .. tostring(#vr.list_explosion+1),
			['Type'] = new.int(-1),
			['Explosion_Data'] = {
				['Type'] = new.int(-1),
				['Pos'] = new.float[3](xx,xy,xz),
				['StartC'] = new.int(0),
				['EndC'] = new.int(0)
			}

		}
		vr.list_name_explosion[#vr.list_explosion] = vr.list_explosion[#vr.list_explosion]['Name']
	end

	imgui.SameLine()
	if imgui.Button(langt['delete']) then
		if vr.list_explosion[vr.lb_cur_explosion[0]+1] then
			removeScriptFire(vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Fire'])
			deleteObject(vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Explosion'])
			vr.list_explosion = DelCellArr(vr.list_explosion,vr.lb_cur_explosion[0]+1)
			vr.list_name_explosion = DelCellArr(vr.list_name_explosion,vr.lb_cur_explosion[0]+1)
		end
	end
	imgui.End()
	--Окно пикапа
	if #vr.list_explosion > 0 and vr.list_explosion[vr.lb_cur_explosion[0]+1] then
		local resX,resY = getScreenResolution()
		local sizeX,sizeY = 300, 340
		imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
		imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2 - 150,(resY-sizeY)/2 - 30),imgui.Cond.FirstUseEver)
		imgui.Begin(vr.list_explosion[vr.lb_cur_explosion[0]+1]['Name'], vr.list_explosion[vr.lb_cur_explosion[0]+1]['Enable'])
		imgui.PushItemWidth(-90)
		local type_expl = decodeJson(langt['type_expl'])
		if imgui.Combo(langt['type'],vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Type'],new('const char* const [?]',#type_expl,type_expl),#type_expl) then
			if vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Type'][0] == 1 then
				vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Type_explosion'] = new.int(0)
				removeScriptFire(vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Fire'])
				upd_explosion:run(vr.lb_cur_explosion[0]+1)
			end
			if vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Type'][0] == 0 then
				vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Size_fire'] = new.int(0)
				vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Propagation_fire'] = new.int(0)
				deleteObject(vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Explosion'])
				upd_explosion:run(vr.lb_cur_explosion[0]+1)
			end
		end
	
		if imgui.InputFloat3(langt['position'],vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Pos']) then
			upd_explosion:run(vr.lb_cur_explosion[0]+1)
		end
	
		if vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Type'][0] == 1 then
			imgui.InputInt(langt['type_explosions'],vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Type_explosion'])
		end
		if vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Type'][0] == 0 then
			if imgui.InputInt(langt['size_fire'],vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Size_fire']) then
				upd_explosion:run(vr.lb_cur_explosion[0]+1)
			end
			if imgui.InputInt(langt['spread_fire'],vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Propagation_fire']) then
				upd_explosion:run(vr.lb_cur_explosion[0]+1)
			end
		end
	
		if imgui.Button(langt['edithand']) then
			if vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['Type'][0] ~= -1 then
				vr.editmode_explosion = true
				printHelpForever('HTARG')
				lockPlayerControl(true)
				vr.id_explosion = vr.lb_cur_explosion[0]+1
				imgui.Process = false
			end
		end
	
		imgui.Separator()
		local list_tg_m = {langt['toend']}
		for ltg = 1,#vr.list_name_targets do
			list_tg_m[#list_tg_m+1] = vr.list_name_targets[ltg]
		end
		imgui.Combo(langt['app_on'],vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['StartC'],new('const char* const [?]',#vr.list_name_targets,vr.list_name_targets),#vr.list_name_targets)
		imgui.Combo(langt['dis_after'],vr.list_explosion[vr.lb_cur_explosion[0]+1]['Explosion_Data']['EndC'],new('const char* const [?]',#list_tg_m,list_tg_m),#list_tg_m)
	
	
		imgui.End()
	end
end)

imgui.OnFrame(function() return vr.info_window[0] and imgui.Process end, 
function()
	local langt = LanguageArr[vr.curr_lang[0]+1]['Keys']

	local resX,resY = getScreenResolution()
	local sizeX,sizeY = 200, 400
	imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
	imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2,(resY-sizeY)/2),imgui.Cond.FirstUseEver)
	imgui.Begin(langt['info'])
	local info_t = decodeJson(langt['info_t'])
	imgui.Text(info_t[1])
	imgui.SameLine()
	imgui.TextColored(imgui.ImVec4(1.0,0.0,0.0,1.0),thisScript().authors[1])
	imgui.Text(info_t[2])
	imgui.SameLine()
	imgui.TextColored(imgui.ImVec4(1.0,0.0,0.0,1.0),thisScript().authors[2])
	imgui.Text(info_t[3])
	imgui.SameLine()
	imgui.Text(thisScript().version)
	imgui.NewLine()
	imgui.Text(info_t[4])
	imgui.Text(info_t[5])
	imgui.End()

end)

-- Настройки
imgui.OnFrame(function() return vr.settings_window[0] and imgui.Process end, 
function()
	local langt = LanguageArr[vr.curr_lang[0]+1]['Keys']

	local resX,resY = getScreenResolution()
	local sizeX,sizeY = 200, 400
	imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
	imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2,(resY-sizeY)/2),imgui.Cond.FirstUseEver)
	imgui.Begin(langt['settings'], vr.settings_window)
	imgui.Combo(langt['lang'],vr.curr_lang,new('const char* const [?]',#LanguageList,LanguageList),#LanguageList)
	imgui.End()
end)

imgui.OnFrame(function() return vr.tool_window[0] and imgui.Process end, 
function()
	local langt = LanguageArr[vr.curr_lang[0]+1]['Keys']

	local resX,resY = getScreenResolution()
	local sizeX,sizeY = 200, 400
	imgui.SetNextWindowSize(imgui.ImVec2(sizeX,sizeY), imgui.Cond.FirstUseEver)
	imgui.SetNextWindowPos(imgui.ImVec2((resX-sizeX)/2,(resY-sizeY)/2),imgui.Cond.FirstUseEver)
	imgui.Begin(langt['setting'], vr.tool_window)

	if imgui.Button(langt['tool_tp_marker']) then
		local xx,xy,xz = getTargetBlipCoordinates()
		setCharCoordinates(PLAYER_PED,select(2,getTargetBlipCoordinates()),select(3,getTargetBlipCoordinates()),select(4,getTargetBlipCoordinates()))
	end
	if imgui.Button(langt['jetpack']) then
		taskJetpack(PLAYER_PED)
	end
	-- if imgui.Button(u8'Потушить пожар') then
	-- 	switchEmergencyServices(true)
	-- end

	imgui.End()

end)

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
		if type(orig[0]) == 'userdata' then
			copy = {orig[0][1],orig[0][2],orig[0][3]}
		else
			copy = orig[0]
		end

    else
        copy = orig
    end
    return copy
end

function update_actor(actorr)
	local modell = vr.list_actors[actorr]['Actor_Data']['ModelId'][0]
	local xx,xy,xz = vr.list_actors[actorr]['Actor_Data']['Pos'][0], vr.list_actors[actorr]['Actor_Data']['Pos'][1], vr.list_actors[actorr]['Actor_Data']['Pos'][2]
	local angle = vr.list_actors[actorr]['Actor_Data']['Angle'][0]

	requestModel(modell)
	while not hasModelLoaded(modell) do
		wait(0)
	end
	local last_id = 288
	for v = 1,#ID_Actors do
		if modell == ID_Actors[v] then
			last_id = ID_Cars[v-1]
			break
		end
	end
	markModelAsNoLongerNeeded(last_id)

	deleteChar(vr.list_actors[actorr]['Actor_Data']['Char'])
	vr.list_actors[actorr]['Actor_Data']['Char'] = createChar(4,modell,xx,xy,xz)
	setCharHeading(vr.list_actors[actorr]['Actor_Data']['Char'], angle)
	setCharCollision(vr.list_actors[actorr]['Actor_Data']['Char'],false)
end

function update_actor_animation(actorr,anim)
	if not hasAnimationLoaded(Anims['Anim_name'][vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim'][0]+1]) then
		requestAnimation(Anims['Anim_name'][vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim'][0]+1])
		while not hasAnimationLoaded(Anims['Anim_name'][vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim'][0]+1]) do
			wait(0)
		end
	end
	local animm = Anims['Anim_list'][vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim'][0]+1]
	animm = animm[vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Anim'][0]+1]
	taskPlayAnimNonInterruptable(vr.list_actors[actorr]['Actor_Data']['Char'], animm, Anims['Anim_name'][vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Pack_anim'][0]+1], 1, vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Loop'][0], false, false, false, -1)
	while not isCharPlayingAnim(vr.list_actors[actorr]['Actor_Data']['Char'],animm) do
		wait(0)
	end
	vr.list_actors[actorr]['Actor_Data']['Anims'][anim]['Time'][0] = getCharAnimTotalTime(vr.list_actors[actorr]['Actor_Data']['Char'],animm) / 1000
end

function update_car(carr)
	local modell = vr.list_cars[carr]['Car_Data']['ModelId'][0]
	local xx,xy,xz = vr.list_cars[carr]['Car_Data']['Pos'][0], vr.list_cars[carr]['Car_Data']['Pos'][1], vr.list_cars[carr]['Car_Data']['Pos'][2]
	local angle = vr.list_cars[carr]['Car_Data']['Angle'][0]

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
	changeCarColour(vr.list_cars[carr]['Car_Data']['Car'], vr.list_cars[carr]['Car_Data']['Color_primary'][0],vr.list_cars[carr]['Car_Data']['Color_secondary'][0])
end

function update_object(objj)
	local modell = vr.list_objects[objj]['Object_Data']['ModelId'][0]
	local xx,xy,xz = vr.list_objects[objj]['Object_Data']['Pos'][0], vr.list_objects[objj]['Object_Data']['Pos'][1], vr.list_objects[objj]['Object_Data']['Pos'][2]
	local rxx,rxy,rxz = vr.list_objects[objj]['Object_Data']['Rotates'][0], vr.list_objects[objj]['Object_Data']['Rotates'][1], vr.list_objects[objj]['Object_Data']['Rotates'][2]

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
	local xx,xy,xz = vr.list_pickup[pickk]['Pickup_Data']['Pos'][0],vr.list_pickup[pickk]['Pickup_Data']['Pos'][1],vr.list_pickup[pickk]['Pickup_Data']['Pos'][2]
	vr.list_pickup[pickk]['Pickup_Data']['Pos'][2] = getGroundZFor3dCoord(xx, xy, xz+1) + 1
	xz = vr.list_pickup[pickk]['Pickup_Data']['Pos'][2]

	if vr.list_pickup[pickk]['Pickup_Data']['Type_pickup'][0] == 0 then
		if not hasModelLoaded(getWeapontypeModel(vr.list_pickup[pickk]['Pickup_Data']['Weapon'][0])) then
			requestModel(getWeapontypeModel(vr.list_pickup[pickk]['Pickup_Data']['Weapon'][0]))
			while not hasModelLoaded(getWeapontypeModel(vr.list_pickup[pickk]['Pickup_Data']['Weapon'][0])) do
				wait(0)
			end
		end
		vr.list_pickup[pickk]['Pickup_Data']['Pick'] = createPickupWithAmmo(getWeapontypeModel(vr.list_pickup[pickk]['Pickup_Data']['Weapon'][0]),9,1,xx,xy,xz)
	elseif vr.list_pickup[pickk]['Pickup_Data']['Type_pickup'][0] == 1 then
		if not hasModelLoaded(1240) then
			requestModel(1240)
			while not hasModelLoaded(1240) do
				wait(0)
			end
		end
		vr.list_pickup[pickk]['Pickup_Data']['Pick'] = select(2,createPickup(1240,9,xx,xy,xz))
	elseif vr.list_pickup[pickk]['Pickup_Data']['Type_pickup'][0] == 2 then
		if not hasModelLoaded(1242) then
			requestModel(1242)
			while not hasModelLoaded(1242) do
				wait(0)
			end
		end
		vr.list_pickup[pickk]['Pickup_Data']['Pick'] = select(2,createPickup(1242,9,xx,xy,xz))
	elseif vr.list_pickup[pickk]['Pickup_Data']['Type_pickup'][0] == 3 then
		if not hasModelLoaded(1247) then
			requestModel(1247)
			while not hasModelLoaded(1247) do
				wait(0)
			end
		end
		vr.list_pickup[pickk]['Pickup_Data']['Pick'] = select(2,createPickup(1247,9,xx,xy,xz))
	elseif vr.list_pickup[pickk]['Pickup_Data']['Type_pickup'][0] == 4 then
		if not hasModelLoaded(1241) then
			requestModel(1241)
			while not hasModelLoaded(1241) do
				wait(0)
			end
		end
		vr.list_pickup[pickk]['Pickup_Data']['Pick'] = select(2,createPickup(1241,9,xx,xy,xz))
	elseif vr.list_pickup[pickk]['Pickup_Data']['Type_pickup'][0] == 5 then
		if not hasModelLoaded() then
			requestModel(vr.list_pickup[pickk]['Pickup_Data']['ModelId'][0])
			while not hasModelLoaded(vr.list_pickup[pickk]['Pickup_Data']['ModelId'][0]) do
				wait(0)
			end
		end
		vr.list_pickup[pickk]['Pickup_Data']['Pick'] = select(2,createPickup(vr.list_pickup[pickk]['Pickup_Data']['ModelId'][0],9,xx,xy,xz))
	end
end

function update_explosion(expl)
	local xx,xy,xz = vr.list_explosion[expl]['Explosion_Data']['Pos'][0],vr.list_explosion[expl]['Explosion_Data']['Pos'][1],vr.list_explosion[expl]['Explosion_Data']['Pos'][2]

	if vr.list_explosion[expl]['Explosion_Data']['Type'][0] == 1 then
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
	end
	if vr.list_explosion[expl]['Explosion_Data']['Type'][0] == 0 then
		removeScriptFire(vr.list_explosion[expl]['Explosion_Data']['Fire'])
		wait(0)
		vr.list_explosion[expl]['Explosion_Data']['Fire'] = startScriptFire(xx,xy,xz,0,vr.list_explosion[expl]['Explosion_Data']['Size_fire'][0])
	end
end

function update_particle(prtcl)
	local modell = Particle_name[vr.list_particle[prtcl]['Particle_Data']['ModelId'][0]+1]
	local xx,xy,xz = vr.list_particle[prtcl]['Particle_Data']['Pos'][0], vr.list_particle[prtcl]['Particle_Data']['Pos'][1], vr.list_particle[prtcl]['Particle_Data']['Pos'][2]
	local rxx,rxy,rxz = vr.list_particle[prtcl]['Particle_Data']['Rotates'][0], vr.list_particle[prtcl]['Particle_Data']['Rotates'][1], vr.list_particle[prtcl]['Particle_Data']['Rotates'][2]

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
		removePickup(vr.list_pickup[p]['Particle_Data']['Prtcl'])
	end
	vr.list_targets = vr.list_missions[vr.lb_cur_missions[0]+1]['Mission_Data']['Targets']
	vr.list_actors = vr.list_missions[vr.lb_cur_missions[0]+1]['Mission_Data']['Actors']
	vr.list_cars = vr.list_missions[vr.lb_cur_missions[0]+1]['Mission_Data']['Cars']
	vr.list_objects = vr.list_missions[vr.lb_cur_missions[0]+1]['Mission_Data']['Objects']
	vr.list_pickup = vr.list_missions[vr.lb_cur_missions[0]+1]['Mission_Data']['Pickup']
	vr.list_particle = vr.list_missions[vr.lb_cur_missions[0]+1]['Mission_Data']['Particle']
	vr.list_explosion = vr.list_missions[vr.lb_cur_missions[0]+1]['Mission_Data']['Explosion']
	vr.mission_data = vr.list_missions[vr.lb_cur_missions[0]+1]['Mission_Data']['Miss_data']
	vr.Fast_data['CurMiss'] = vr.lb_cur_missions[0]
	vr.list_name_actors,vr.list_name_objects,vr.list_name_cars,vr.list_name_pickup,vr.list_name_particle,vr.list_name_explosion = {},{},{},{},{},{}
	for a = 1, #vr.list_actors do
		wait(0)
		vr.list_name_actors[a] = vr.list_actors[a]['Name']
		lua_thread.create(update_actor,a)
	end
	for o = 1, #vr.list_objects do
		vr.list_name_objects[o] = vr.list_objects[o]['Name']
		lua_thread.create(update_object,o)
	end
	for c = 1, #vr.list_cars do
		wait(0)
		vr.list_name_cars[c] = vr.list_cars[c]['Name']
		lua_thread.create(update_car,c)
	end
	for p = 1, #vr.list_pickup do
		wait(0)
		vr.list_name_pickup[p] = vr.list_pickup[p]['Name']
		lua_thread.create(update_pickup,p)
	end
	for p = 1, #vr.list_particle do
		wait(0)
		vr.list_name_particle[p] = vr.list_particle[p]['Name']
		lua_thread.create(update_particle,p)
	end
	for e = 1, #vr.list_explosion do
		wait(0)
		vr.list_name_explosion[e] = vr.list_explosion[e]['Name']
		lua_thread.create(update_explosion,e)
	end
	for t = 1, #vr.list_targets do
		wait(0)
		vr.list_name_targets[t] = vr.list_targets[t]['Name']
	end
	setCharCoordinates(PLAYER_PED, vr.mission_data['Player']['Pos'][1], vr.mission_data['Player']['Pos'][2], vr.mission_data['Player']['Pos'][3])
	setCharInterior(PLAYER_PED, vr.mission_data['Player']['Interior_id'])
	setInteriorVisible(vr.mission_data['Player']['Interior_id'])
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

	local langt = LanguageArr[vr.curr_lang[0]+1]['Keys']

	setGxtEntry('HTARG', koder(u8:decode(langt['HTARG'])))
	setGxtEntry('HACT', koder(u8:decode(langt['HACT'])))
	setGxtEntry('HOBJ', koder(u8:decode(langt['HOBJ'])))
	setGxtEntry('HMTIM', koder(u8:decode(langt['HMTIM'])))
	setGxtEntry('HPLA', koder(u8:decode(langt['HPLA'])))
	setGxtEntry('HPLA2', koder(u8:decode(langt['HPLA2'])))
	setGxtEntry('HVIEW',koder(u8:decode(langt['HVIEW'])))
	setGxtEntry('HPICW', koder(u8:decode(langt['HPICW'])))

	local top
	local rand

	vr.list_mission_pack = manager.loadMPack()
	for mp = 1,#vr.list_mission_pack do
		vr.list_name_mission_pack[mp] = vr.list_mission_pack[mp]['Name']
	end

	upd_actor = lua_thread.create_suspended(update_actor)
	upd_car = lua_thread.create_suspended(update_car)
	upd_anim_actor = lua_thread.create_suspended(update_actor_animation)
	upd_object = lua_thread.create_suspended(update_object)
	upd_pickup = lua_thread.create_suspended(update_pickup)
	upd_particle = lua_thread.create_suspended(update_particle)
	upd_explosion = lua_thread.create_suspended(update_explosion)


	local langt = LanguageArr[vr.curr_lang[0]+1]['Keys']

	while true do
		wait(0)
		if vr.mpack == 7 then
			wait(500)
			local welcome_t = decodeJson((langt['welcome_t']))
			printHelpString(koder(u8:decode(welcome_t[1]) .. tostring(thisScript().version) .. u8:decode(welcome_t[2])))
			vr.mpack = nil
		end

		if testCheat('top2009') then
			printHelpString('~r~I LOVE TWENTY ONE PILOTS')
		end
		removeSphere(r)

		--Редактор позиции объекта
		while vr.editmode_target do
			wait(0)
			--Закрытие редактора
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
				clearHelp()
				vr.editmode_target = false
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
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][0] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][0] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_S) then
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][0] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][0] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_SPACE) then
				vr.list_targets[vr.id_target]['Target_Data']['Radius'][0] = vr.list_targets[vr.id_target]['Target_Data']['Radius'][0] + 1 * multy
			end
			--Отрисовка в редакторе
			local xx,xy,xz = vr.list_targets[vr.id_target]['Target_Data']['Pos'][0],vr.list_targets[vr.id_target]['Target_Data']['Pos'][1],vr.list_targets[vr.id_target]['Target_Data']['Pos'][2]
			vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] = getGroundZFor3dCoord(xx, xy, xz+2)
			drawSphere(xx, xy, xz,vr.list_targets[vr.id_target]['Target_Data']['Radius'][0])
		end

		if vr.editmode_actor then
			imgui.Process = false
			requestModel(model.BMORI)
			while not hasModelLoaded(vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0]) do
				wait(1)
			end
			setPlayerModel(PLAYER_HANDLE, vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0])
			local xx,xy,xz = vr.list_actors[vr.id_actor]['Actor_Data']['Pos'][0],vr.list_actors[vr.id_actor]['Actor_Data']['Pos'][1],vr.list_actors[vr.id_actor]['Actor_Data']['Pos'][2]
			setCharCoordinates(PLAYER_PED, xx, xy, xz)
			setCharHeading(PLAYER_PED, vr.list_actors[vr.id_actor]['Actor_Data']['Angle'][0])
			printHelpForever('HACT')
		end
		while vr.editmode_actor do
			--Закрытие редактора
			wait(0)
			if wasKeyPressed(vkeys.VK_F) then
				clearHelp()
				vr.editmode_actor = false
				setPlayerModel(PLAYER_HANDLE, model.NULL)
				local xx,xy,xz = getCharCoordinates(PLAYER_PED)
				local angle = getCharHeading(PLAYER_PED)
				xz = getGroundZFor3dCoord(xx,xy,xz)
				vr.list_actors[vr.id_actor]['Actor_Data']['Pos'] = new.float[3](xx,xy,xz)
				vr.list_actors[vr.id_actor]['Actor_Data']['Angle'][0] = angle

				upd_actor:run(vr.id_actor)
				clearPrints()
			end
			if isKeyDown(vkeys.VK_I) then
				wait(100)
				vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] = vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] + 1
				local id_a = 0
				for v = 1,#ID_Actors do
					if vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] <= ID_Actors[v] then
						id_a = ID_Actors[v]
						break
					end
				end
				vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] = id_a
				requestModel(vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0])
				while not hasModelLoaded(vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0]) do
					wait(1)
				end
				setPlayerModel(PLAYER_HANDLE, vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0])
			end
			if isKeyDown(vkeys.VK_O) then
				wait(100)
				vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] = vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] - 1
				if vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] < 0 then
					vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] = 288
				end
				local id_a = 288
				for v = 1,#ID_Actors do
					if vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] < ID_Actors[v] then
						id_a = ID_Actors[v-1]
						break
					end
				end
				vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] = id_a
				requestModel(vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0])
				while not hasModelLoaded(vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0]) do
					wait(1)
				end
				setPlayerModel(PLAYER_HANDLE, vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0])
			end
			if isKeyDown(vkeys.VK_K) then
				wait(100)
				vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] = vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] + 10
				local id_a = 0
				for v = 1,#ID_Actors do
					if vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] <= ID_Actors[v] then
						id_a = ID_Actors[v]
						break
					end
				end
				vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] = id_a
				requestModel(vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0])
				while not hasModelLoaded(vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0]) do
					wait(1)
				end
				setPlayerModel(PLAYER_HANDLE, vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0])
			end
			if isKeyDown(vkeys.VK_L) then
				wait(100)
				vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] = vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] - 10
				if vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] < 0 then
					vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] = 289 + vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0]
				end
				local id_a = 288
				for v = 1,#ID_Actors do
					if vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] < ID_Actors[v] then
						id_a = ID_Actors[v-1]
						break
					end
				end
				vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0] = id_a
				requestModel(vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0])
				while not hasModelLoaded(vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0]) do
					wait(1)
				end
				setPlayerModel(PLAYER_HANDLE, vr.list_actors[vr.id_actor]['Actor_Data']['ModelId'][0])
			end
		end

		while vr.editmode_actor_add_point do
			wait(0)
			if wasKeyPressed(vkeys.VK_U) then
				print('ch')
				local xx,xy,xz = getCharCoordinates(PLAYER_PED)
				table.insert(vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][vr.id_actor_anim]['Path'],{xx,xy,xz})
			end
			local mx,my = getCursorPos()
			mx,my = convertWindowScreenCoordsToGameScreenCoords(mx,my)
			local xx,xy,xz = vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][vr.id_actor_anim]['Path'][#vr.list_actors[vr.lb_cur_actors[0]+1]['Actor_Data']['Anims'][vr.id_actor_anim]['Path']]
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
				clearHelp()
				vr.editmode_actor_add_point = false
			end
		end

		while vr.editmode_objects do
			wait(0)
			--Закрытие редактора
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
				clearHelp()
				vr.editmode_objects = false
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
				vr.list_objects[vr.id_obj]['Object_Data']['Pos'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][1] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_D) and not isKeyDown(vkeys.VK_SHIFT) then
				vr.list_objects[vr.id_obj]['Object_Data']['Pos'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][1] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_W) and not isKeyDown(vkeys.VK_SHIFT) then
				vr.list_objects[vr.id_obj]['Object_Data']['Pos'][0] = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][0] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_S) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_objects[vr.id_obj]['Object_Data']['Pos'][0] = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][0] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_Q) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_objects[vr.id_obj]['Object_Data']['Pos'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][2] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_E) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_objects[vr.id_obj]['Object_Data']['Pos'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][2] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_SHIFT) then
				if isKeyDown(vkeys.VK_A) then
					vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][1] - 0.2 * multy
				end
				if isKeyDown(vkeys.VK_D) then
					vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][1] + 0.2 * multy
				end
				if isKeyDown(vkeys.VK_W) then
					vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][0] = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][0] - 0.2 * multy
				end
				if isKeyDown(vkeys.VK_S) then
					vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][0] = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][0] + 0.2 * multy
				end
				if isKeyDown(vkeys.VK_E) then
					vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][2] - 0.2 * multy
				end
				if isKeyDown(vkeys.VK_Q) then
					vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][2] + 0.2 * multy
				end
			end
			--Отрисовка в редакторе
			local xx,xy,xz = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][0],vr.list_objects[vr.id_obj]['Object_Data']['Pos'][1],vr.list_objects[vr.id_obj]['Object_Data']['Pos'][2]
			local rxx,rxy,rxz = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][0],vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][1],vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][2]
			print(vr.list_objects[vr.id_obj]['Object_Data']['Pos'][2])
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
			vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][1] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_D) and not isKeyDown(vkeys.VK_SHIFT) then
				vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][1] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_W) and not isKeyDown(vkeys.VK_SHIFT) then
				vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][0] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][0] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_S) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][0] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][0] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_Q) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][2] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_E) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][2] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_SHIFT) then
				if isKeyDown(vkeys.VK_A) then
					vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][1] - 0.2 * multy
				end
				if isKeyDown(vkeys.VK_D) then
					vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][1] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][1] + 0.2 * multy
				end
				if isKeyDown(vkeys.VK_W) then
					vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][0] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][0] - 0.2 * multy
				end
				if isKeyDown(vkeys.VK_S) then
					vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][0] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][0] + 0.2 * multy
				end
				if isKeyDown(vkeys.VK_E) then
					vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][2] - 0.5 * multy
				end
				if isKeyDown(vkeys.VK_Q) then
					vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][2] = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][2] + 0.5 * multy
				end
			end
			--Отрисовка в редакторе
			local xx,xy,xz = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][0],vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][1],vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Pos'][2]

			local rxx,rxy,rxz = vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][0],vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][1],vr.list_objects[vr.id_obj]['Object_Data']['Anims'][vr.id_obj_anim]['Rotates'][2]

			setObjectCoordinates(vr.list_objects[vr.id_obj]['Object_Data']['Obj'], xx, xy, xz)
			setObjectRotation(vr.list_objects[vr.id_obj]['Object_Data']['Obj'], rxx, rxy, rxz)

			--Закрытие редактора
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
				clearHelp()
				local xx,xy,xz = vr.list_objects[vr.id_obj]['Object_Data']['Pos'][0],vr.list_objects[vr.id_obj]['Object_Data']['Pos'][1],vr.list_objects[vr.id_obj]['Object_Data']['Pos'][2]
				local rxx,rxy,rxz = vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][0],vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][1],vr.list_objects[vr.id_obj]['Object_Data']['Rotates'][2]
				setObjectCoordinates(vr.list_objects[vr.id_obj]['Object_Data']['Obj'], xx, xy, xz)
				setObjectRotation(vr.list_objects[vr.id_obj]['Object_Data']['Obj'], rxx, rxy, rxz)
				vr.editmode_objects_anim = false
			end
		end

		while vr.editmode_particle do
			wait(0)
			--Закрытие редактора
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
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
				vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][1] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][1] + 0.2 * multy
				upd_particle:run(vr.id_prtcl)
			end
			if isKeyDown(vkeys.VK_D) and not isKeyDown(vkeys.VK_SHIFT) then
				wait(100)
				vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][1] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][1] - 0.2 * multy
				upd_particle:run(vr.id_prtcl)
			end
			if isKeyDown(vkeys.VK_W) and not isKeyDown(vkeys.VK_SHIFT) then
				wait(100)
				vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][0] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][0] + 0.2 * multy
				upd_particle:run(vr.id_prtcl)
			end
			if isKeyDown(vkeys.VK_S) and not isKeyDown(vkeys.VK_SHIFT)  then
				wait(100)
				vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][0] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][0] - 0.2 * multy
				upd_particle:run(vr.id_prtcl)
			end
			if isKeyDown(vkeys.VK_Q) and not isKeyDown(vkeys.VK_SHIFT)  then
				wait(100)
				vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][2] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][2] + 0.2 * multy
				upd_particle:run(vr.id_prtcl)
			end
			if isKeyDown(vkeys.VK_E) and not isKeyDown(vkeys.VK_SHIFT)  then
				wait(100)
				vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][2] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Pos'][2] - 0.2 * multy
				upd_particle:run(vr.id_prtcl)
			end
			if isKeyDown(vkeys.VK_SHIFT) then
				if isKeyDown(vkeys.VK_A) then
					wait(100)
					vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][1] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][1] + 0.2 * multy
					upd_particle:run(vr.id_prtcl)
				end
				if isKeyDown(vkeys.VK_D) then
					wait(100)
					vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][1] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][1] - 0.2 * multy
					upd_particle:run(vr.id_prtcl)
				end
				if isKeyDown(vkeys.VK_W) then
					wait(100)
					vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][0] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][0] + 0.2 * multy
					upd_particle:run(vr.id_prtcl)
				end
				if isKeyDown(vkeys.VK_S) then
					vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][0] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][0] - 0.2 * multy
					upd_particle:run(vr.id_prtcl)
				end
				if isKeyDown(vkeys.VK_E) then
					wait(100)
					vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][2] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][2] + 0.2 * multy
					upd_particle:run(vr.id_prtcl)
				end
				if isKeyDown(vkeys.VK_Q) then
					wait(100)
					vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][2] = vr.list_particle[vr.id_prtcl]['Particle_Data']['Rotates'][2] - 0.2 * multy
					upd_particle:run(vr.id_prtcl)
				end
			end
		end

		while vr.editmode_pickup do
			wait(0)
			--Закрытие редактора
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
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
				vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][1] = vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][1] - 0.2 * multy
				upd_pickup:run(vr.id_pick)
			end
			if isKeyDown(vkeys.VK_D) then
				wait(50)
				vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][1] = vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][1] + 0.2 * multy
				upd_pickup:run(vr.id_pick)
			end
			if isKeyDown(vkeys.VK_W) then
				wait(50)
				vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][0] = vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][0] - 0.2 * multy
				upd_pickup:run(vr.id_pick)
			end
			if isKeyDown(vkeys.VK_S) then
				wait(50)
				vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][0] = vr.list_pickup[vr.id_pick]['Pickup_Data']['Pos'][0] + 0.2 * multy
				upd_pickup:run(vr.id_pick)
			end
			if isKeyDown(vkeys.VK_I) then
				wait(100)
				if vr.list_pickup[vr.id_pick]['Pickup_Data']['Type_pickup'][0] == 0 then
					vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'][0] = vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'][0] + 1
					local id_w = 1
					for v = 1,#Number_Weapons do
						if vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'][0] <= Number_Weapons[v] then
							id_w = Number_Weapons[v]
							markModelAsNoLongerNeeded(getWeapontypeModel(Number_Weapons[v-1]))
							break
						end
					end
					vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'][0] = id_w
					upd_pickup:run(vr.id_pick)
				end
			end
			if isKeyDown(vkeys.VK_O) then
				wait(100)
				if 	vr.list_pickup[vr.id_pick]['Pickup_Data']['Type_pickup'][0] == 0 then
					vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'][0] = vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'][0] - 1
					if vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'][0] < 1 then
						vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'][0] = 46
					end
					local id_w = 46
					for v = 1,#Number_Weapons do
						if vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'][0] < Number_Weapons[v] then
							id_w = Number_Weapons[v-1]
							markModelAsNoLongerNeeded(getWeapontypeModel(Number_Weapons[v]))
							break
						end
					end
					vr.list_pickup[vr.id_pick]['Pickup_Data']['Weapon'][0] = id_w
				end
				upd_pickup:run(vr.id_pick)
			end
		end

		while vr.editmode_explosion do
			wait(0)
			--Закрытие редактора
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
				clearHelp()
				vr.editmode_explosion = false
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
				vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][1] = vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][1] + 0.2 * multy
				upd_explosion:run(vr.id_explosion)
			end
			if isKeyDown(vkeys.VK_D) then
				vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][1] = vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][1] - 0.2 * multy
				upd_explosion:run(vr.id_explosion)
			end
			if isKeyDown(vkeys.VK_W) then
				vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][0] = vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][0] + 0.2 * multy
				upd_explosion:run(vr.id_explosion)
			end
			if isKeyDown(vkeys.VK_S) then
				vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][0] = vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][0] - 0.2 * multy
				upd_explosion:run(vr.id_explosion)
			end
			if isKeyDown(vkeys.VK_Q) then
				vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][2] = vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][2] + 0.2 * multy
				upd_explosion:run(vr.id_explosion)
			end
			if isKeyDown(vkeys.VK_E) then
				vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][2] = vr.list_explosion[vr.id_explosion]['Explosion_Data']['Pos'][2] - 0.2 * multy
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
				local sinn = math.sin(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1]+90))
				local coss = math.cos(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1]+90))
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][0] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][0] - 0.2 *sinn * multy
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] - 0.2 *coss * multy
			end
			if isKeyDown(vkeys.VK_D) and not isKeyDown(vkeys.VK_SHIFT) then
				local sinn = math.sin(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1]-90))
				local coss = math.cos(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1]-90))
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][0] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][0] - 0.2 *sinn * multy
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] - 0.2 *coss * multy
			end
			if isKeyDown(vkeys.VK_W) and not isKeyDown(vkeys.VK_SHIFT) then
				local sinn = math.sin(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1]))
				local coss = math.cos(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1]))
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][0] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][0] + 0.2 *sinn * multy
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] + 0.2 *coss * multy
			end
			if isKeyDown(vkeys.VK_S) and not isKeyDown(vkeys.VK_SHIFT)  then
				local sinn = math.sin(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1]))
				local coss = math.cos(math.rad(vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1]))
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][0] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][0] - 0.2 *sinn * multy
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][1] - 0.2 *coss * multy
			end
			if isKeyDown(vkeys.VK_Q) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] + 0.2 * multy
			end
			if isKeyDown(vkeys.VK_E) and not isKeyDown(vkeys.VK_SHIFT)  then
				vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] = vr.list_targets[vr.id_target]['Target_Data']['Pos'][2] - 0.2 * multy
			end
			if isKeyDown(vkeys.VK_SHIFT) then
				if isKeyDown(vkeys.VK_A) then
					vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1] = vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1] - 2 * multy
				end
				if isKeyDown(vkeys.VK_D) then
					vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1] = vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1] + 2 * multy
				end
				if isKeyDown(vkeys.VK_W) then
					vr.list_targets[vr.id_target]['Target_Data']['Rotates'][0] = vr.list_targets[vr.id_target]['Target_Data']['Rotates'][0] - 2 * multy
				end
				if isKeyDown(vkeys.VK_S) then
					vr.list_targets[vr.id_target]['Target_Data']['Rotates'][0] = vr.list_targets[vr.id_target]['Target_Data']['Rotates'][0] + 2 * multy
				end
				if isKeyDown(vkeys.VK_E) then
					vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2] = vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2] - 2 * multy
				end
				if isKeyDown(vkeys.VK_Q) then
					vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2] = vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2] + 2 * multy
				end
			end

			local xx,xy,xz = vr.list_targets[vr.id_target]['Target_Data']['Pos'][0],vr.list_targets[vr.id_target]['Target_Data']['Pos'][1],vr.list_targets[vr.id_target]['Target_Data']['Pos'][2]
			local rxx,rxy,rxz = vr.list_targets[vr.id_target]['Target_Data']['Rotates'][0],vr.list_targets[vr.id_target]['Target_Data']['Rotates'][1],vr.list_targets[vr.id_target]['Target_Data']['Rotates'][2]
			local x1,y1,z1 = xx,xy,xz
			x1 = x1 + 2*math.sin(math.rad(rxy)) * math.sin(math.rad(rxx))
			y1 = y1 + 2*math.cos(math.rad(rxy)) * math.sin(math.rad(rxx))
			z1 = z1 + 2*math.cos(math.rad(rxx))

			--setObjectCoordinates(chr, x1, y1, z1)
			setFixedCameraPosition(xx, xy, xz,0,rxz,0)
			pointCameraAtPoint(x1, y1, z1, 2)
			setCinemaCamera(false)

			--Закрытие редактора
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
				clearHelp()
				restoreCamera()
				vr.editmode_camera = false
				displayRadar(true)
				displayHud(true)
			end
		end

		while vr.editmode_timemiss do
			wait(0)
			--Закрытие редактора
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
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
			if wasKeyPressed(vkeys.VK_F) then
				lockPlayerControl(false)
				clearHelp()
				vr.editmode_timetarg = false
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
			local modelID = 0
			local weapon = 0
			local ammo = 0
			while vr.editmode_player do
				--Закрытие редактора
				wait(0)
				if wasKeyPressed(vkeys.VK_F) then
					clearHelp()
					vr.editmode_player = false
					setPlayerModel(PLAYER_HANDLE, model.NULL)
					removeAllCharWeapons(PLAYER_PED)
					lockPlayerControl(false)
				end
				if wasKeyPressed(vkeys.VK_SPACE) then
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
						vr.mission_data['Player']['ModelId'] = modelID
						vr.mission_data['Player']['Weapon'] = weapon
						vr.mission_data['Player']['Weap_ammo'] = ammo
						vr.mission_data['Player']['Interior_id'] = getActiveInterior()
						removeAllCharWeapons(PLAYER_PED)
						lockPlayerControl(false)
					end
				end
				if stage == 0 then
					if isKeyDown(vkeys.VK_I) then
						wait(100)
						modelID = modelID + 1
						local id_a = 0
						for v = 1,#ID_Actors do
							if modelID <= ID_Actors[v] then
								id_a = ID_Actors[v]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_O) then
						wait(100)
						modelID = modelID - 1
						if modelID < 0 then
							modelID = 288
						end
						local id_a = 288
						for v = 1,#ID_Actors do
							if modelID < ID_Actors[v] then
								id_a = ID_Actors[v-1]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_K) then
						wait(100)
						modelID = modelID + 10
						local id_a = 0
						for v = 1,#ID_Actors do
							if modelID <= ID_Actors[v] then
								id_a = ID_Actors[v]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_L) then
						wait(100)
						modelID = modelID - 10
						if modelID < 0 then
							modelID = 289 + modelID
						end
						local id_a = 288
						for v = 1,#ID_Actors do
							if modelID < ID_Actors[v] then
								id_a = ID_Actors[v-1]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_P) then
						wait(200)
						weapon = weapon + 1
						local id_w = 0
						for v = 1,#Number_Weapons do
							if weapon <= Number_Weapons[v] then
								id_w = Number_Weapons[v]
								markModelAsNoLongerNeeded(getWeapontypeModel(Number_Weapons[v-1]))
								break
							end
						end
						weapon = id_w
						requestModel(getWeapontypeModel(weapon))
						while not hasModelLoaded(getWeapontypeModel(weapon)) do
							wait(1)
						end
						removeAllCharWeapons(PLAYER_PED)
						print(weapon)
						giveWeaponToChar(PLAYER_PED, weapon, 9999)
						setCurrentCharWeapon(PLAYER_PED,1)
					end
					if isKeyDown(vkeys.VK_OEM_4) then
						wait(200)
						weapon = weapon - 1
						if weapon < 0 then
							weapon = 46
						end
						local id_w = 46
						for v = 1,#Number_Weapons do
							if weapon < Number_Weapons[v] then
								id_w = Number_Weapons[v-1]
								markModelAsNoLongerNeeded(getWeapontypeModel(Number_Weapons[v]))
								break
							end
						end
						weapon = id_w
						requestModel(getWeapontypeModel(weapon))
						while not hasModelLoaded(getWeapontypeModel(weapon)) do
							wait(1)
						end
						removeAllCharWeapons(PLAYER_PED)
						giveWeaponToChar(PLAYER_PED, weapon, 9999)
						setCurrentCharWeapon(PLAYER_PED,1)
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
					printString(koder('Патронов: ' .. '~r~' .. ammo), 0)
				end
			end
		end

		if vr.editmode_teleport_player then
			local stage = 0
			local modelID = vr.list_targets[vr.id_teleport_player]['Target_Data']['ModelID'][0]
			local weapon = vr.list_targets[vr.id_teleport_player]['Target_Data']['Weapon'][0]
			local ammo = vr.list_targets[vr.id_teleport_player]['Target_Data']['Weap_ammo'][0]
			setCharCoordinates(PLAYER_PED, vr.list_targets[vr.id_teleport_player]['Target_Data']['Pos'][0], vr.list_targets[vr.id_teleport_player]['Target_Data']['Pos'][1], vr.list_targets[vr.id_teleport_player]['Target_Data']['Pos'][2])
			if not hasModelLoaded(vr.list_targets[vr.id_teleport_player]['Target_Data']['ModelID'][0]) then
				requestModel(vr.list_targets[vr.id_teleport_player]['Target_Data']['ModelID'][0])
				while not hasModelLoaded(vr.list_targets[vr.id_teleport_player]['Target_Data']['ModelID'][0]) do
					wait(1)
				end
			end
			setPlayerModel(PLAYER_HANDLE, vr.list_targets[vr.id_teleport_player]['Target_Data']['ModelID'][0])
			setCharHeading(PLAYER_PED, vr.list_targets[vr.id_teleport_player]['Target_Data']['Angle'][0])
			giveWeaponToChar(PLAYER_PED, vr.list_targets[vr.id_teleport_player]['Target_Data']['Weapon'][0], 99999)
			setInteriorVisible(vr.list_targets[vr.id_teleport_player]['Target_Data']['Interior_id'])
			setCharInterior(PLAYER_PED, vr.list_targets[vr.id_teleport_player]['Target_Data']['Interior_id'])
			while vr.editmode_teleport_player do
				--Закрытие редактора
				wait(0)
				if wasKeyPressed(vkeys.VK_F) then
					clearHelp()
					vr.editmode_teleport_player = false
					setPlayerModel(PLAYER_HANDLE, model.NULL)
					removeAllCharWeapons(PLAYER_PED)
					lockPlayerControl(false)
				end
				if wasKeyPressed(vkeys.VK_SPACE) then
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
						vr.list_targets[vr.id_teleport_player]['Target_Data']['Pos'] = new.float[3](xx,xy,xz)
						vr.list_targets[vr.id_teleport_player]['Target_Data']['Angle'][0] = angle
						vr.list_targets[vr.id_teleport_player]['Target_Data']['ModelID'][0] = modelID
						vr.list_targets[vr.id_teleport_player]['Target_Data']['Weapon'][0] = weapon
						vr.list_targets[vr.id_teleport_player]['Target_Data']['Weap_ammo'][0] = ammo
						vr.list_targets[vr.id_teleport_player]['Target_Data']['Interior_id'] = getActiveInterior()
						removeAllCharWeapons(PLAYER_PED)
						lockPlayerControl(false)
					end
				end
				if stage == 0 then
					if isKeyDown(vkeys.VK_I) then
						wait(100)
						modelID = modelID + 1
						local id_a = 0
						for v = 1,#ID_Actors do
							if modelID <= ID_Actors[v] then
								id_a = ID_Actors[v]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_O) then
						wait(100)
						modelID = modelID - 1
						if modelID < 0 then
							modelID = 288
						end
						local id_a = 288
						for v = 1,#ID_Actors do
							if modelID < ID_Actors[v] then
								id_a = ID_Actors[v-1]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_K) then
						wait(100)
						modelID = modelID + 10
						local id_a = 0
						for v = 1,#ID_Actors do
							if modelID <= ID_Actors[v] then
								id_a = ID_Actors[v]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_L) then
						wait(100)
						modelID = modelID - 10
						if modelID < 0 then
							modelID = 289 + modelID
						end
						local id_a = 288
						for v = 1,#ID_Actors do
							if modelID < ID_Actors[v] then
								id_a = ID_Actors[v-1]
								break
							end
						end
						modelID = id_a
						requestModel(modelID)
						while not hasModelLoaded(modelID) do
							wait(1)
						end
						setPlayerModel(PLAYER_HANDLE, modelID)
					end
					if isKeyDown(vkeys.VK_P) then
						wait(200)
						weapon = weapon + 1
						local id_w = 0
						for v = 1,#Number_Weapons do
							if weapon <= Number_Weapons[v] then
								id_w = Number_Weapons[v]
								markModelAsNoLongerNeeded(getWeapontypeModel(Number_Weapons[v-1]))
								break
							end
						end
						weapon = id_w
						requestModel(getWeapontypeModel(weapon))
						while not hasModelLoaded(getWeapontypeModel(weapon)) do
							wait(1)
						end
						removeAllCharWeapons(PLAYER_PED)
						print(weapon)
						giveWeaponToChar(PLAYER_PED, weapon, 9999)
						setCurrentCharWeapon(PLAYER_PED,1)
					end
					if isKeyDown(vkeys.VK_OEM_4) then
						wait(200)
						weapon = weapon - 1
						if weapon < 0 then
							weapon = 46
						end
						local id_w = 46
						for v = 1,#Number_Weapons do
							if weapon < Number_Weapons[v] then
								id_w = Number_Weapons[v-1]
								markModelAsNoLongerNeeded(getWeapontypeModel(Number_Weapons[v]))
								break
							end
						end
						weapon = id_w
						requestModel(getWeapontypeModel(weapon))
						while not hasModelLoaded(getWeapontypeModel(weapon)) do
							wait(1)
						end
						removeAllCharWeapons(PLAYER_PED)
						giveWeaponToChar(PLAYER_PED, weapon, 9999)
						setCurrentCharWeapon(PLAYER_PED,1)
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
					printString(koder('Патронов: ' .. '~r~' .. ammo), 0)
				end
			end
		end

		if vr.editmode_preview_player_anim then
			if not hasAnimationLoaded(Anims['Anim_name'][vr.list_targets[vr.id_preview_player_anim]['Target_Data']['Pack_anim'][0]+1]) then
				requestAnimation(Anims['Anim_name'][vr.list_targets[vr.id_preview_player_anim]['Target_Data']['Pack_anim'][0]+1])
				while not hasAnimationLoaded(Anims['Anim_name'][vr.list_targets[vr.id_preview_player_anim]['Target_Data']['Pack_anim'][0]+1]) do
					wait(0)
				end
			end
			local animm = Anims['Anim_list'][vr.list_targets[vr.id_preview_player_anim]['Target_Data']['Pack_anim'][0]+1]
			animm = animm[vr.list_targets[vr.id_preview_player_anim]['Target_Data']['Anim'][0]+1]
			taskPlayAnim(PLAYER_PED, animm, Anims['Anim_name'][vr.list_targets[vr.id_preview_player_anim]['Target_Data']['Pack_anim'][0]+1], 1.0, vr.list_targets[vr.id_preview_player_anim]['Target_Data']['Loop'][0], false, false, false, -1)
			while vr.editmode_preview_player_anim do
				wait(0)
				if wasKeyPressed(vkeys.VK_F) then
					clearHelp()
					vr.editmode_preview_player_anim = false
					taskPlayAnim(PLAYER_PED, "WALK_START", 'PED', 1.0, false, false, false, false, -1)
					lockPlayerControl(false)
				end
			end
		end

		if vr.editmode_car then
			local carr = createCar(vr.list_cars[vr.id_car]['Car_Data']['ModelId'][0], vr.list_cars[vr.id_car]['Car_Data']['Pos'][0], vr.list_cars[vr.id_car]['Car_Data']['Pos'][1], vr.list_cars[vr.id_car]['Car_Data']['Pos'][2])
			changeCarColour(carr, vr.list_cars[vr.id_car]['Car_Data']['Color_primary'][0],vr.list_cars[vr.id_car]['Car_Data']['Color_secondary'][0])
			setCarHeading(carr, vr.list_cars[vr.id_car]['Car_Data']['Angle'][0])
			taskWarpCharIntoCarAsDriver(PLAYER_PED, carr)
			while vr.editmode_car do
				wait(0)
				if not isCharInCar(PLAYER_PED, carr) then
					local xx,xy,xz = getCarCoordinates(carr)
					vr.list_cars[vr.id_car]['Car_Data']['Pos'] = new.float[3](xx,xy,xz)
					vr.list_cars[vr.id_car]['Car_Data']['Angle'][0] = getCarHeading(carr)
					deleteCar(carr)
					upd_car:run(vr.id_car)
					vr.editmode_car = false
				end
			end
		end

		for i = 1, #vr.list_targets do
			if vr.list_targets[i]['Type'][0] == 0 then
				drawSphere(vr.list_targets[i]['Target_Data']['Pos'][0], vr.list_targets[i]['Target_Data']['Pos'][1], vr.list_targets[i]['Target_Data']['Pos'][2],vr.list_targets[i]['Target_Data']['Radius'][0])
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

		--Откр/Закр. Меню
		if wasKeyPressed(vkeys.VK_U) then
			if vr.main_window[0] or vr.targets_window[0] or vr.actors_window[0] or vr.cars_window[0] or vr.objects_window[0] or vr.missions_window[0] or vr.pack_mission_window[0] or vr.info_window[0] or vr.pickup_window[0] or vr.tool_window[0] or vr.particle_window[0] or vr.explosion_window[0] or vr.settings_window[0] then
				vr.main_window[0] = false
				vr.targets_window[0] = false
				vr.actors_window[0] = false
				vr.cars_window[0] = false
				vr.objects_window[0] = false
				vr.missions_window[0] = false
				vr.pack_mission_window[0] = false
				vr.info_window[0] = false
				vr.pickup_window[0] = false
				vr.tool_window[0] = false
				vr.particle_window[0] = false
				vr.explosion_window[0] = false
				vr.settings_window[0] = false
			else
				vr.main_window[0] = not vr.main_window[0]
			end
		end

		if isKeyDown(vkeys.VK_CONTROL) and isKeyJustPressed(vkeys.VK_S) then
			if vr.list_missions[vr.Fast_data['CurMiss']+1] == nil then
				vr.mission_data['Name'] = u8'Миссия #' .. tostring(vr.Fast_data['CurMiss']+1)
				vr.list_missions[vr.Fast_data['CurMiss']+1] = {
					['Name'] = vr.mission_data['Name'],
					['Prename'] = new.char[128](),
					['Enable'] = new.bool(false),
					['Mission_Data'] = {
						['Targets'] = vr.list_targets,
						['Actors'] = vr.list_actors,
						['Cars'] = vr.list_cars,
						['Objects'] = vr.list_objects,
						['Pickup'] = vr.list_pickup,
						['Particle'] = vr.list_particle,
						['Explosion'] = vr.list_explosion,
						['Miss_data'] = vr.mission_data
					}
				}
				vr.list_name_missions[vr.Fast_data['CurMiss']+1] = vr.list_missions[vr.Fast_data['CurMiss']+1]['Name']
			else
				vr.list_missions[vr.Fast_data['CurMiss']+1]['Mission_Data'] = {
					['Targets'] = vr.list_targets,
					['Actors'] = vr.list_actors,
					['Cars'] = vr.list_cars,
					['Objects'] = vr.list_objects,
					['Pickup'] = vr.list_pickup,
					['Particle'] = vr.list_particle,
					['Explosion'] = vr.list_explosion,
					['Miss_data'] = vr.mission_data
				}
			end
			if vr.list_mission_pack[vr.Fast_data['CurPack']+1] == nil then
				vr.list_mission_pack[vr.Fast_data['CurPack']+1] = {
					['Name'] = u8'Пак миссий #' .. tostring(vr.Fast_data['CurPack']+1),
					['Enable'] = new.bool(false),
					['Missions'] = vr.list_missions
				}
				vr.list_name_mission_pack[vr.Fast_data['CurPack']+1] = vr.list_mission_pack[vr.Fast_data['CurPack']+1]['Name']
				manager.save(vr.list_mission_pack[vr.Fast_data['CurPack']+1],vr.Fast_data['CurPack'])
			else
				vr.list_mission_pack[vr.Fast_data['CurPack']+1]['Missions'] = vr.list_missions
				manager.save(vr.list_mission_pack[vr.Fast_data['CurPack']+1],vr.Fast_data['CurPack'])
			end
			printHelpString(koder('Сохранено!'))
		end
		imgui.Process = vr.main_window[0] or vr.targets_window[0] or vr.actors_window[0] or vr.cars_window[0] or vr.objects_window[0] or vr.missions_window[0] or vr.pack_mission_window[0] or vr.info_window[0] or vr.pickup_window[0] or vr.tool_window[0] or vr.particle_window[0] or vr.explosion_window[0] or vr.settings_window[0]

		-- Дебаг: позиция
		if wasKeyPressed(vkeys.VK_Q) then
			px,py,pz = getCharCoordinates(PLAYER_PED)
			ang = getCharHeading(PLAYER_PED)
			print(px .. ' ' .. py .. ' ' .. pz .. ' ' .. ang)
			print(getCharActiveInterior(PLAYER_PED))
		end
	end
end
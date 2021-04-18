#pragma once

#include <D3dx9tex.h>
#pragma comment(lib, "D3dx9")
#include <array>
#include <CCivilianPed.h>
#include <CTheScripts.h>
#include <CWorld.h>
#include <string>
#include <vector>
#include <ctime>
#include <boost/serialization/base_object.hpp>
#include <CTrain.h>


#include "CBmx.h"
#include "CBoat.h"
#include "CExplosion.h"
#include "CHeli.h"
#include "CModelInfo.h"
#include "CMonsterTruck.h"
#include "components.h"
#include "CPickup.h"
#include "CPickups.h"
#include "CPlane.h"
#include "CQuadBike.h"
#include "CStreaming.h"
#include "CTrailer.h"
#include "plugin.h"
#include "extensions/ScriptCommands.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_dx9.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_internal.h"

using namespace plugin;
using namespace std;

extern float camera_zoom;
extern float camera_angle[2];
extern bool storylineMode;
extern bool editmodeCamera;
extern bool editmodeTimetarg;
extern bool editmodeTeleportPlayer;
extern bool editmodeNodeAnimActor;
extern bool editmodeNodePathActor;
extern bool editmodeNodePathActorCar;
extern bool editActor;
extern bool editmodeCar;
extern bool editPlayer;
extern bool editmodeTimemiss;
extern PDIRECT3DTEXTURE9 weaponsAtlas;
extern PDIRECT3DTEXTURE9 pedsSkinAtlas;
extern PDIRECT3DTEXTURE9 blipsAtlas;
extern const int ID_Cars[212];
extern const int ID_Weapons[44];
extern vector<std::string> Particle_name;
extern vector <int> ID_Actors;
extern vector <std::string> ID_Spec_Actors;
extern vector <std::string> Anim_name;
extern vector<vector <std::string>> Anim_list;
extern const char* langt(const std::string& key);

class Actor {
public:
	CPed* editorPed = nullptr;
	CPed* missionPed = nullptr;
	char name[65];
	float pos[3];
	float angle = 0;
	int modelID = 0;
	ImU8 modelType = 0;
	int slotSkin = 1;
	int startC;
	int endC = 0;
	bool shouldNotDie = false;
	int health = 100;
	int weapon = 0;
	int ammo = 0;
	int group = 1;
	bool randomSpawn = false;
	int accuracy = 0;
	bool useTarget = true;
	bool headshot = true;
	int interiorID = 0;
	bool stayInSamePlace = false;
	bool kindaStayInSamePlace = false;

	Actor() = default;
	Actor(const char* name, float x, float y, float z, float angle, int lastTarget);
	Actor(const Actor& actor);

	void updateEditorPed();
	void updateMissionPed();

	void removeEditorPed();
	void removeMissionPed();

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & name;
		ar & pos;
		ar & angle;
		ar & modelID;
		ar & modelType;
		ar & slotSkin;
		ar & startC;
		ar & endC;
		ar & shouldNotDie;
		ar & health;
		ar & weapon;
		ar & ammo;
		ar & group;
		ar & randomSpawn;
		ar & accuracy;
		ar & useTarget;
		ar & headshot;
		ar & interiorID;
		ar & stayInSamePlace;
		ar & kindaStayInSamePlace;
	}
};

class Target {
public:
	char name[65];
	unsigned char type;
	unsigned char targetType;
	virtual ~Target() = default;

	template <typename Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & name;
		ar & type;
		ar & targetType;
	}
};

class TargetCheckpoint : public Target {
public:
	float pos[3];
	float radius = 2.0f;
	char text[129] = {};
	float textTime = 2.0f;
	int colorBlip = 0;
	unsigned int indexSphere;

	TargetCheckpoint() = default;
	TargetCheckpoint(const char* name, float x, float y, float z);

	TargetCheckpoint(const TargetCheckpoint& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & pos;
		ar & radius;
		ar & text;
		ar & textTime;
		ar & colorBlip;
		ar & indexSphere;
	}
};

class TargetCar : public Target {
public:
	int car = 0;
	char text[129] = {};
	float textTime = 2.0f;
	int colorBlip = 0;

	TargetCar(const char* name);
	TargetCar() = default;
	TargetCar(const TargetCar& target);

	
	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & car;
		ar & text;
		ar & textTime;
		ar & colorBlip;
	}
};

class TargetActor : public Target {
	friend class boost::serialization::access;
public:
	int actor = 0;
	char text[129] = "";
	float textTime = 2.0f;
	int colorBlip = 0;
	bool killGroup = false;
	bool hit = false;

	TargetActor(const char* name);
	TargetActor() = default;
	TargetActor(const TargetActor& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & actor;
		ar & text;
		ar & textTime;
		ar & colorBlip;
		ar & killGroup;
		ar & hit;
	}
};


class TargetCutscene : public Target {
public:
	int tied = 0;
	int tiedID = 0;
	int follow = 0;
	int followID = 0;
	float pos[3];
	int rotate[3] = {0,0,0};
	char text[129] = {};
	float time = 2;
	bool moveCam = false;


	TargetCutscene(const char* name, float x, float y, float z);
	TargetCutscene() = default;
	TargetCutscene(const TargetCutscene& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & tied;
		ar & tiedID;
		ar & follow;
		ar & followID;
		ar & pos;
		ar & rotate;
		ar & text;
		ar & time;
		ar & moveCam;
	}
};

class TargetCountdown : public Target {
public:
	int time = 3;

	TargetCountdown(const char* name);
	TargetCountdown() = default;
	TargetCountdown(const TargetCountdown& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & time;
	}
};

class TargetTimeout : public Target {
public:
	char text[129] = "";
	float time = 2;

	TargetTimeout(const char* name);
	TargetTimeout() = default;
	TargetTimeout(const TargetTimeout& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & time;
		ar & text;
	}
};

class TargetWeather : public Target {
public:
	int weather = 0;

	TargetWeather(const char* name);
	TargetWeather() = default;
	TargetWeather(const TargetWeather& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & weather;
	}
};

class TargetTime : public Target {
public:
	signed char time[2] = {8,0};

	TargetTime(const char* name);
	TargetTime() = default;
	TargetTime(const TargetTime& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & time;
	}
};

class TargetTraffic : public Target {
public:
	unsigned char peds = 0;
	unsigned char cars = 0;

	TargetTraffic(const char* name);
	TargetTraffic() = default;
	TargetTraffic(const TargetTraffic& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & peds;
		ar & cars;
	}
};

class TargetObject : public Target {
public:
	int object = 0;
	char text[129] = {};
	float textTime = 2.0f;
	int colorBlip = 0;
	int typeUse = 0;
	int weapon = 0;

	TargetObject(const char* name);
	TargetObject() = default;
	TargetObject(const TargetObject& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & object;
		ar & text;
		ar & textTime;
		ar & colorBlip;
		ar & typeUse;
		ar & weapon;
	}
};

class TargetPickup : public Target {
public:
	int pickup = 0;
	char text[129] = {};
	float textTime = 2.0f;
	int colorBlip = 0;

	TargetPickup(const char* name);
	TargetPickup() = default;
	TargetPickup(const TargetPickup& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & pickup;
		ar & text;
		ar & textTime;
		ar & colorBlip;
	}
};

class TargetTeleport : public Target {
public:
	float pos[3];
	float angle;
	int modelID = 0;
	unsigned char modelType = 0;
	int health = 100;
	int weapon = 0;
	int ammo = 0;
	int interiorID;

	TargetTeleport(const char* name, float x, float y, float z, float angle, int interiorID);
	TargetTeleport() = default;
	TargetTeleport(const TargetTeleport& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & pos;
		ar & angle;
		ar & modelID;
		ar & modelType;
		ar & health;
		ar & weapon;
		ar & ammo;
		ar & interiorID;
	}
};

class TargetAnimation : public Target {
public:
	int pack = 0;
	int anim = 0;
	bool looped = false;
	float blend = 1.0f;

	TargetAnimation(const char* name);
	TargetAnimation() = default;
	TargetAnimation(const TargetAnimation& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & pack;
		ar & anim;
		ar & looped;
	}
};

class TargetTeleportToVehicle : public Target {
public:
	int car = 0;
	int carPlace = 0;

	TargetTeleportToVehicle(const char* name);
	TargetTeleportToVehicle() = default;
	TargetTeleportToVehicle(const TargetTeleportToVehicle& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & car;
		ar & carPlace;
	}
};

class TargetLevelWanted : public Target {
public:
	int levelWanted = 0;

	TargetLevelWanted(const char* name);
	TargetLevelWanted() = default;
	TargetLevelWanted(const TargetLevelWanted& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & levelWanted;
	}
};

class TargetRemoveWeapons : public Target {
public:

	TargetRemoveWeapons(const char* name);
	TargetRemoveWeapons() = default;
	TargetRemoveWeapons(const TargetRemoveWeapons& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
	}
};

struct Dialog {
	char text[129] = {};
	float textTime = 2.0f;

	Dialog() {}
	Dialog(const Dialog& dialog);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & text;
		ar & textTime;
	}
};

class TargetDialog : public Target {
public:
	int dialog = 0;
	vector<Dialog> dialogs;

	TargetDialog(const char* name);
	TargetDialog() = default;
	TargetDialog(const TargetDialog& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & dialog;
		ar & dialogs;
	}
};

class TargetMoney : public Target {
public:
	int money = 0;

	TargetMoney(const char* name);
	TargetMoney() = default;
	TargetMoney(const TargetMoney& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
		ar & money;
	}
};

class TargetWaitSignal : public Target {
public:

	TargetWaitSignal(const char* name);
	TargetWaitSignal() = default;
	TargetWaitSignal(const TargetWaitSignal& target);

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Target>(*this);
	}
};

class Car {
public:
	CVehicle* editorCar = nullptr;
	CVehicle* missionCar = nullptr;
	char name[65];
	float pos[3];
	float angle = 0;
	int modelID = 400;
	int startC;
	int endC = 0;
	bool shouldNotDie = false;
	int health = 1000;
	bool bulletproof = false;
	bool fireproof = false;
	bool explosionproof = false;
	bool collisionproof = false;
	bool meleeproof = false;
	bool tiresVulnerability = false;
	bool extendedColor = false;
	float primaryColor[4];
	float secondaryColor[4];
	bool useTarget = true;
	bool locked = false;
	vector<pair<unsigned char, std::array<float,4>>> colors;
	int component_type_A = 0;
	int component_type_B = 0;
	int numberplate_city = -1;
	char numberplate[9] = "";

	Car(const char* name, float x, float y, float z, float angle, int lastTarget);
	Car(const Car& car);
	Car() = default;
	
	void updateEditorCar(bool recolor = false);
	void removeEditorCar();

	void updateMissionCar();
	void removeMissionCar();

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & name;
		ar & pos;
		ar & angle;
		ar & modelID;
		ar & startC;
		ar & endC;
		ar & shouldNotDie;
		ar & health;
		ar & bulletproof;
		ar & fireproof;
		ar & explosionproof;
		ar & collisionproof;
		ar & meleeproof;
		ar & tiresVulnerability;
		ar & extendedColor;
		ar & primaryColor;
		ar & secondaryColor;
		ar & useTarget;
		ar & locked;
		ar & colors;
		ar & component_type_A;
		ar & component_type_B;
		ar & numberplate;
		ar & numberplate_city;
	}
};

class Train {
public:
	CTrain* editorTrain = nullptr;
	CTrain* missionTrain = nullptr;
	char name[65];
	float pos[3];
	bool rotation = false;
	int modelID = 0;
	int startC;
	int endC = 0;
	bool useTarget = true;
	float speed = 0.0f;
	float cruise_speed = 0.0f;

	Train(const char* name, float x, float y, float z, int lastTarget);
	Train(const Train& train);
	Train() = default;
	void updateEditorTrain();

	void removeEditorTrain();

	void updateMissionTrain();

	void removeMissionTrain();

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & name;
		ar & pos;
		ar & rotation;
		ar & modelID;
		ar & startC;
		ar & endC;
		ar & useTarget;
		ar & speed;
		ar & cruise_speed;
	}
};

class Object {
public:
	CObject* editorObject = nullptr;
	CObject* missionObject = nullptr;
	char name[65];
	float pos[3];
	int rotation[3] = { 0,0,0 };
	int modelID = 0;
	int startC;
	int endC = 0;
	bool useTarget = true;

	Object(const char* name, float x, float y, float z, int lastTarget);
	Object(const Object& object);
	Object() = default;
	void updateEditorObject();

	void removeEditorObject();

	void updateMissionObject();

	void removeMissionObject();

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & name;
		ar & pos;
		ar & rotation;
		ar & modelID;
		ar & startC;
		ar & endC;
		ar & useTarget;
	}
};

class Particle {
public:
	pair<unsigned int, CObject*> editorParticle = make_pair(NULL, nullptr);
	pair<unsigned int,CObject*> missionParticle = make_pair(NULL, nullptr);
	char name[65];
	float pos[3];
	int rotation[3] = { 0,0,0 };
	int modelID = 0;
	int startC;
	int endC = 0;
	bool useTarget = true;
	int tied = 0;
	int tiedID = 0;

	Particle(const char* name, float x, float y, float z, int lastTarget);
	Particle(const Particle& particle);
	Particle() = default;
	void updateEditorParticle();
	void updateMissionParticle(void* void_mission);

	void removeEditorParticle();
	void removeMissionParticle();

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & name;
		ar & pos;
		ar & rotation;
		ar & modelID;
		ar & startC;
		ar & endC;
		ar & useTarget;
		ar & tied;
		ar & tiedID;
	}
};

class Pickup {
public:
	int editorPickup = NULL;
	int missionPickup = NULL;
	char name[65];
	float pos[3];
	int type = 0;
	int spawnType = 0;
	int weapon = 1;
	int ammo = 0;
	int modelID = 0;
	int startC;
	int endC = 0;
	bool useTarget = true;

	Pickup(const char* name, float x, float y, float z, int lastTarget);
	Pickup(const Pickup& particle);
	Pickup() = default;
	void updateEditorPickup();
	void updateMissionPickup();

	void removeEditorPickup();
	void removeMissionPickup();

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & name;
		ar & pos;
		ar & type;
		ar & spawnType;
		ar & weapon;
		ar & ammo;
		ar & modelID;
		ar & startC;
		ar & endC;
		ar & useTarget;
	}
};

class Explosion {
public:
	CObject* editorExplosion = nullptr;
	int editorFire = NULL;
	int missionFire = NULL;
	char name[65];
	float pos[3];
	int type = 0;
	ImU8 typeExplosion = 0;
	ImU8 sizeFire = 0;
	ImU8 propagationFire = 0;
	int startC;
	int endC = 0;
	bool useTarget = true;

	Explosion(const char* name, float x, float y, float z, int lastTarget);
	Explosion(const Explosion& particle);
	Explosion() = default;
	void updateEditorExplosion();
	void updateMissionExplosion();

	void removeEditorExplosion();
	void removeMissionExplosion();

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & name;
		ar & pos;
		ar & type;
		ar & typeExplosion;
		ar & sizeFire;
		ar & propagationFire;
		ar & startC;
		ar & endC;
		ar & useTarget;
	}
};

class Audio {
public:
	CObject* object = nullptr;
	int missionAudio = NULL;
	char name[65];
	float pos[3];
	int sound = 0;
	bool audio3D = false;
	ImU8 audio3DType = 0;
	int audio3DAttach = 0;
	bool repeat = false;
	int startC;
	int endC = 0;
	bool useTarget = true;


	Audio(const char* name, float x, float y, float z, int lastTarget);
	Audio(const Audio& particle);
	Audio() = default;
	void updateEditorAudio(bool _new = false);
	static void loadAudiosList();
	void loadAudio();
	void unloadAudio();
	void play(void* void_mission);
	void stop();

	void removeEditorAudio();

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & name;
		ar & pos;
		ar & sound;
		ar & audio3D;
		ar & audio3DType;
		ar & audio3DAttach;
		ar & repeat;
		ar & startC;
		ar & endC;
		ar & useTarget;
	}
};

class Player {
public:
	CPed* editorPed = nullptr;
	float pos[3];
	float angle = 0;
	int modelID = 0;
	ImU8 modelType = 0;
	int health = 100;
	int weapon = 0;
	int ammo = 0;
	int interiorID = 0;

	Player(float x, float y, float z, float angle);
	Player(const Player& player);
	Player() = default;
	void updateEditorPed();

	void removeEditorPed();

	template <typename Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & pos;
		ar & angle;
		ar & modelID;
		ar & modelType;
		ar & health;
		ar & weapon;
		ar & ammo;
		ar & interiorID;
	}
};

struct Mission {
	friend class boost::serialization::access;
	std::string name;
	vector <Target*> list_targets;
	vector <Actor*> list_actors;
	vector <Car*> list_cars;
	vector <Train*> list_trains;
	vector <Object*> list_objects;
	vector <Particle*> list_particles;
	vector <Pickup*> list_pickups;
	vector <Explosion*> list_explosions;
	vector <Audio*> list_audios;
	signed char groupRelations[8][9];
	signed char time[2] = { 0, 0 };
	ImU8 traffic_ped = 1;
	ImU8 traffic_car = 1;
	ImU8 wanted_min = 0;
	ImU8 wanted_max = 6;
	int weather = 0;
	bool riot = false;
	Player player = Player(884.6011f, -1221.845f, 16.9766f, 0.0f);

	Mission();
	~Mission();
	void removeEditorEntity();
	void updateEditorEntity();

	void removeMissionEntity();

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		
		ar & name;
		
		ar.template register_type<TargetActor>();
		ar.template register_type <TargetCheckpoint>();
		ar.template register_type <TargetCar>();
		ar.template register_type <TargetActor>();
		ar.template register_type <TargetCutscene>();
		ar.template register_type <TargetCountdown>();
		ar.template register_type <TargetTimeout>();
		ar.template register_type <TargetWeather>();
		ar.template register_type <TargetTime>();
		ar.template register_type <TargetTraffic>();
		ar.template register_type <TargetObject>();
		ar.template register_type <TargetPickup>();
		ar.template register_type <TargetTeleport>();
		ar.template register_type <TargetAnimation>();
		ar.template register_type <TargetTeleportToVehicle>();
		ar.template register_type <TargetLevelWanted>();
		ar.template register_type <TargetRemoveWeapons>();
		ar.template register_type <TargetDialog>();
		ar.template register_type <TargetMoney>();
		ar.template register_type <TargetWaitSignal>();
		
		ar & list_targets;
		ar & list_actors;
		ar & list_cars;
		ar & list_trains;
		ar & list_objects;
		ar & list_particles;
		ar & list_pickups;
		ar & list_explosions;
		ar & list_audios;
		ar & groupRelations;
		ar & time;
		ar & traffic_ped;
		ar & traffic_car;
		ar & wanted_min;
		ar & wanted_max;
		ar & weather;
		ar & riot;
		ar & player;
	}
};

class CheckpointStoryline
{
	friend class boost::serialization::access;
public:
	char name[65];
	float pos[3];
	int colorBlip = 1;
	ImU8 iconMarker = 0;
	unsigned int indexSphere;
	bool useMission = true;
	int startC = 0;
	int gotoMission = 0;
	int timeStart = 0;
	int marker;
	int markerPlay = NULL;
	bool activate = false;
	
	CheckpointStoryline(const char* name, float x, float y, float z);
	CheckpointStoryline() = default;
	CheckpointStoryline(const CheckpointStoryline& checkpoint);

	void updateEditorCheckpoint();
	void removeEditorCheckpoint();

	void checkpoint();
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & name;
		ar & pos;
		ar & colorBlip;
		ar & iconMarker;
		ar & useMission;
		ar & startC;
		ar & gotoMission;
		ar & timeStart;
	}
};

struct Storyline
{
	friend class boost::serialization::access;
	std::string missPack;
	int startMission = 0;
	int endMission = 0;
	vector<CheckpointStoryline*> list_checkpoints;
	
	Storyline(std::string& missPack);
	Storyline() = default;
	~Storyline();
	void updateEditorEntity();
	void removeEditorEntity();

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & missPack;
		ar & startMission;
		ar & endMission;
		ar & list_checkpoints;
	}
};
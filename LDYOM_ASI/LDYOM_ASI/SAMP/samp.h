#pragma once

#include "d3d9.h"
#include <d3dx9.h>
#include "iostream"
#include "Windows.h"

#define SAMP_INFO_OFFSET							0x21A0F8
#define SAMP_CHAT_INFO_OFFSET						0x21A0E4
#define SAMP_CHAT_INPUT_INFO_OFFSET					0x21A0E8
#define SAMP_FUNC_ADDTOCHATWND						0x064010
#define SAMP_FUNC_ADDCLIENTCMD						0x065AD0
#define SAMP_FUNC_RESTARTGAME						0x00A060

enum
{
	DIALOG_STYLE_MSGBOX,
	DIALOG_STYLE_INPUT,
	DIALOG_STYLE_LIST,
	DIALOG_STYLE_PASSWORD,
};

enum
{
	SAMP_MAX_ACTORS = 1000,
	SAMP_MAX_PLAYERS = 1004,
	SAMP_MAX_VEHICLES = 2000,
	SAMP_MAX_PICKUPS = 4096,
	SAMP_MAX_OBJECTS = 1000,
	SAMP_MAX_GANGZONES = 1024,
	SAMP_MAX_3DTEXTS = 2048,
	SAMP_MAX_TEXTDRAWS = 2048,
	SAMP_MAX_PLAYERTEXTDRAWS = 256,
	SAMP_MAX_CLIENTCMDS = 144,
	SAMP_MAX_MENUS = 128,
	SAMP_MAX_PLAYER_NAME = 24,
	SAMP_ALLOWED_PLAYER_NAME_LENGTH = 20,
	SAMP_TOGGLECURSOR = 0x09BD30,
	SAMP_CURSORUNLOCKACTORCAM = 0x09BC10,
};

enum
{
	CHAT_TYPE_NONE = 0,
	CHAT_TYPE_CHAT = 2,
	CHAT_TYPE_INFO = 4,
	CHAT_TYPE_DEBUG = 8
};

enum
{
	CHAT_WINDOW_MODE_OFF = 0,
	CHAT_WINDOW_MODE_LIGHT = 1,
	CHAT_WINDOW_MODE_FULL = 2
};

enum
{
	GAMESTATE_WAIT_CONNECT = 9,
	GAMESTATE_CONNECTING = 13,
	GAMESTATE_AWAIT_JOIN = 15,
	GAMESTATE_CONNECTED = 14,
	GAMESTATE_RESTARTING = 18
};

enum
{
	PLAYER_STATE_NONE = 0,
	PLAYER_STATE_ONFOOT = 17,
	PLAYER_STATE_PASSENGER,
	PLAYER_STATE_DRIVER,
	PLAYER_STATE_WASTED = 32,
	PLAYER_STATE_SPAWNED
};

enum
{
	PLAYER_MARKERS_MODE_OFF,
	PLAYER_MARKERS_MODE_GLOBAL,
	PLAYER_MARKERS_MODE_STREAMED,
};

enum
{
	SPECIAL_ACTION_NONE,
	SPECIAL_ACTION_DUCK,
	SPECIAL_ACTION_USEJETPACK,
	SPECIAL_ACTION_ENTER_VEHICLE,
	SPECIAL_ACTION_EXIT_VEHICLE,
	SPECIAL_ACTION_DANCE1,
	SPECIAL_ACTION_DANCE2,
	SPECIAL_ACTION_DANCE3,
	SPECIAL_ACTION_DANCE4,
	SPECIAL_ACTION_HANDSUP,
	SPECIAL_ACTION_USECELLPHONE,
	SPECIAL_ACTION_SITTING,
	SPECIAL_ACTION_STOPUSECELLPHONE,
	SPECIAL_ACTION_DRINK_BEER = 20,
	SPECIAL_ACTION_SMOKE_CIGGY,
	SPECIAL_ACTION_DRINK_WINE,
	SPECIAL_ACTION_DRINK_SPRUNK,
	SPECIAL_ACTION_CUFFED,
	SPECIAL_ACTION_CARRY,
};

#pragma pack(push, 1)

struct stSAMPPools
{
	struct stActorPool		*pActor;
	struct stObjectPool		*pObject;
	struct stGangzonePool	*pGangzone;
	struct stTextLabelPool	*pText3D;
	struct stTextdrawPool	*pTextdraw;
	void					*pPlayerLabels;
	struct stPlayerPool		*pPlayer;
	struct stVehiclePool	*pVehicle;
	struct stPickupPool		*pPickup;
};

struct stSAMP
{
	void					*pUnk0[2];
	uint8_t					byteSpace[24];
	char					szIP[257];
	char					szHostname[259];
	uint8_t					byteUnk1;
	uint32_t				ulPort;
	uint32_t				ulMapIcons[100];
	int						iLanMode;
	int						iGameState;
	uint32_t				ulConnectTick;
	struct stServerPresets	*pSettings;
	void					*pRakClientInterface;
	struct stSAMPPools		*pPools;
};

struct stServerPresets
{
	uint8_t byteCJWalk;
	uint8_t byteUnk0[4];
	float	fWorldBoundaries[4];
	uint8_t byteUnk1;
	float	fGravity;
	uint8_t byteDisableInteriorEnterExits;
	uint32_t ulVehicleFriendlyFire;
	uint8_t byteUnk2[4];
	int		iClassesAvailable;
	float	fNameTagsDistance;
	uint8_t byteUnk3;
	uint8_t byteWorldTime_Hour;
	uint8_t byteWorldTime_Minute;
	uint8_t byteWeather;
	uint8_t byteNoNametagsBehindWalls;
	uint8_t bytePlayerMarkersMode;
	uint8_t byteUnk4[3];
	float	fGlobalChatRadiusLimit;
	uint8_t byteShowNameTags;
};

struct stTextDrawTransmit
{
	union
	{
		BYTE byteFlags;
		struct
		{
			BYTE byteBox : 1;
			BYTE byteLeft : 1;
			BYTE byteRight : 1;
			BYTE byteCenter : 1;
			BYTE byteProportional : 1;
			BYTE bytePadding : 3;
		};
	};
	float fLetterWidth;
	float fLetterHeight;
	DWORD dwLetterColor;
	float fBoxWidth;
	float fBoxHeight;
	DWORD dwBoxColor;
	BYTE byteShadow;
	BYTE byteOutline;
	DWORD dwBackgroundColor;
	BYTE byteStyle;
	BYTE byteUNK;
	float fX;
	float fY;
	uint16_t sModel;
	float fRot[3];
	float fZoom;
	WORD sColor[2];
};

struct stTextdraw
{
	char		szText[800 + 1];
	char		szString[1600 + 2];
	float		fLetterWidth;
	float		fLetterHeight;
	DWORD		dwLetterColor;
	uint8_t		byte_unk;	
	BYTE		byteCenter;
	BYTE		byteBox;
	float		fBoxSizeX;
	float		fBoxSizeY;
	DWORD		dwBoxColor;
	BYTE		byteProportional;
	DWORD		dwShadowColor;
	BYTE		byteShadowSize;
	BYTE		byteOutline;
	BYTE		byteLeft;
	BYTE		byteRight;
	int			iStyle;		
	float		fX;
	float		fY;
	byte		unk[8];
	DWORD		dword99B;	
	DWORD		dword99F;	
	DWORD		index;	
	BYTE		byte9A7;	
	uint16_t	sModel;
	float		fRot[3];
	float		fZoom;
	WORD		sColor[2];
	BYTE		f9BE;
	BYTE		byte9BF;
	BYTE		byte9C0;
	DWORD		dword9C1;
	DWORD		dword9C5;
	DWORD		dword9C9;
	DWORD		dword9CD;
	BYTE		byte9D1;
	DWORD		dword9D2;
};

struct stTextdrawPool
{
	int					iIsListed[SAMP_MAX_TEXTDRAWS];
	int					iPlayerTextDraw[SAMP_MAX_PLAYERTEXTDRAWS];
	struct stTextdraw	*textdraw[SAMP_MAX_TEXTDRAWS];
	struct stTextdraw	*playerTextdraw[SAMP_MAX_PLAYERTEXTDRAWS];
};

struct stPickup
{
	int		iModelID;
	int		iType;
	float	fPosition[3];
};

struct stPickupPool
{
	int				iPickupsCount;
	uint32_t		ul_GTA_PickupID[SAMP_MAX_PICKUPS];
	int				iPickupID[SAMP_MAX_PICKUPS];
	int				iTimePickup[SAMP_MAX_PICKUPS];
	uint8_t			unk[SAMP_MAX_PICKUPS * 3];
	struct stPickup pickup[SAMP_MAX_PICKUPS];
};

struct stPlayerPool
{
	uint32_t				ulUnk0;
	uint16_t				sLocalPlayerID;
	void					*pVTBL_txtHandler;
	union
	{
		char	szLocalPlayerName[16];
		char	*pszLocalPlayerName;
	};
	int						iLocalPlayerNameLen;
	int						iLocalPlayerNameAllocated;
	struct stLocalPlayer	*pLocalPlayer;
	int						iLocalPlayerPing;
	int						iLocalPlayerScore;
	struct stRemotePlayer	*pRemotePlayer[SAMP_MAX_PLAYERS];
	int						iIsListed[SAMP_MAX_PLAYERS];
	uint32_t				ulUnk1[SAMP_MAX_PLAYERS];
};

struct stSAMPKeys
{
	uint8_t keys_primaryFire : 1;
	uint8_t keys_horn__crouch : 1;
	uint8_t keys_secondaryFire__shoot : 1;
	uint8_t keys_accel__zoomOut : 1;
	uint8_t keys_enterExitCar : 1;
	uint8_t keys_decel__jump : 1;		
	uint8_t keys_circleRight : 1;
	uint8_t keys_aim : 1;				
	uint8_t keys_circleLeft : 1;
	uint8_t keys_landingGear__lookback : 1;
	uint8_t keys_unknown__walkSlow : 1;
	uint8_t keys_specialCtrlUp : 1;
	uint8_t keys_specialCtrlDown : 1;
	uint8_t keys_specialCtrlLeft : 1;
	uint8_t keys_specialCtrlRight : 1;
	uint8_t keys__unused : 1;
};

struct stOnFootData
{
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float		fPosition[3];
	float		fQuaternion[4];
	uint8_t		byteHealth;
	uint8_t		byteArmor;
	uint8_t		byteCurrentWeapon;
	uint8_t		byteSpecialAction;
	float		fMoveSpeed[3];
	float		fSurfingOffsets[3];
	uint16_t	sSurfingVehicleID;
	short		sCurrentAnimationID;
	short		sAnimFlags;
};

struct stInCarData
{
	uint16_t	sVehicleID;
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float		fQuaternion[4];
	float		fPosition[3];
	float		fMoveSpeed[3];
	float		fVehicleHealth;
	uint8_t		bytePlayerHealth;
	uint8_t		byteArmor;
	uint8_t		byteCurrentWeapon;
	uint8_t		byteSiren;
	uint8_t		byteLandingGearState;
	uint16_t	sTrailerID;
	union
	{
		uint16_t	HydraThrustAngle[2];	
		float		fTrainSpeed;
	};
};

struct stAimData
{
	BYTE	byteCamMode;
	float	vecAimf1[3];
	float	vecAimPos[3];
	float	fAimZ;
	BYTE	byteCamExtZoom : 6;	
	BYTE	byteWeaponState : 2;
	BYTE	bUnk;
};

struct stTrailerData
{
	uint16_t	sTrailerID;
	float		fPosition[3];
	float		fQuaternion[4]; 
	float		fSpeed[3];
	float		fUnk[2];
	uint32_t	pad;
};

struct stPassengerData
{
	uint16_t	sVehicleID;
	uint8_t		byteSeatID;
	uint8_t		byteCurrentWeapon;
	uint8_t		byteHealth;
	uint8_t		byteArmor;
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float	fPosition[3];
};

struct stDamageData
{
	uint16_t	sVehicleID_lastDamageProcessed;
	int			iBumperDamage;
	int			iDoorDamage;
	uint8_t		byteLightDamage;
	uint8_t		byteWheelDamage;
};

struct stSurfData
{
	int			iIsSurfing;
	float		fSurfPosition[3];
	int			iUnk0;
	uint16_t	sSurfingVehicleID;
	uint32_t	ulSurfTick;
	struct stSAMPVehicle *pSurfingVehicle;
	int			iUnk1;
	int			iSurfMode;
};

struct stUnoccupiedData
{
	int16_t sVehicleID;
	uint8_t byteSeatID;
	float	fRoll[3];
	float	fDirection[3];
	float	fPosition[3];
	float	fMoveSpeed[3];
	float	fTurnSpeed[3];
	float	fHealth;
};

struct stBulletData
{
	uint8_t		byteType;
	uint16_t	sTargetID;
	float		fOrigin[3];
	float		fTarget[3];
	float		fCenter[3];
};

struct stSpectatorData
{
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	union
	{
		uint16_t			sKeys;
		struct stSAMPKeys	stSampKeys;
	};
	float	fPosition[3];
};

struct stStatsData
{
	int iMoney;
	int iAmmo;
};

struct stHeadSync
{
	float	fHeadSync[3];
	int		iHeadSyncUpdateTick;
	int		iHeadSyncLookTick;
};

struct stLocalPlayer
{
	struct stSAMPPed		*pSAMP_Actor;
	uint16_t				sCurrentAnimID;
	uint16_t				sAnimFlags;
	uint32_t				ulUnk0;
	int						iIsActive;
	int						iIsWasted;
	uint16_t				sCurrentVehicleID;
	uint16_t				sLastVehicleID;
	struct stOnFootData		onFootData;
	struct stPassengerData	passengerData;
	struct stTrailerData	trailerData;
	struct stInCarData		inCarData;
	struct stAimData		aimData;
	uint8_t					byteTeamID;
	int						iSpawnSkin;
	uint8_t					byteUnk1;
	float					fSpawnPos[3];
	float					fSpawnRot;
	int						iSpawnWeapon[3];
	int						iSpawnAmmo[3];
	int						iIsActorAlive;
	int						iSpawnClassLoaded;
	uint32_t				ulSpawnSelectionTick;
	uint32_t				ulSpawnSelectionStart;
	int						iIsSpectating;
	uint8_t					byteTeamID2;
	uint16_t				usUnk2;
	uint32_t				ulSendTick;
	uint32_t				ulSpectateTick;
	uint32_t				ulAimTick;
	uint32_t				ulStatsUpdateTick;
	uint32_t				ulWeapUpdateTick;
	uint16_t				sAimingAtPid;
	uint16_t				usUnk3;
	uint8_t					byteCurrentWeapon;
	uint8_t					byteWeaponInventory[13];
	int						iWeaponAmmo[13];
	int						iPassengerDriveBy;
	uint8_t					byteCurrentInterior;
	int						iIsInRCVehicle;
	uint16_t				sTargetObjectID;
	uint16_t				sTargetVehicleID;
	uint16_t				sTargetPlayerID;
	struct stHeadSync		headSyncData;
	uint32_t				ulHeadSyncTick;
	BYTE					byteSpace3[260];
	struct stSurfData		surfData;
	int						iClassSelectionOnDeath;
	int						iSpawnClassID;
	int						iRequestToSpawn;
	int						iIsInSpawnScreen;
	uint32_t				ulUnk4;
	uint8_t					byteSpectateMode;	
	uint8_t					byteSpectateType;	
	int						iSpectateID;
	int						iInitiatedSpectating;
	struct stDamageData		vehicleDamageData;
};

struct stRemotePlayerData
{
	struct stSAMPPed		*pSAMP_Actor;
	struct stSAMPVehicle	*pSAMP_Vehicle;
	uint8_t					byteTeamID;
	uint8_t					bytePlayerState;
	uint8_t					byteSeatID;
	uint32_t				ulUnk3;
	int						iPassengerDriveBy;
	void					*pUnk0;
	uint8_t					byteUnk1[60];
	float					fSomething[3];
	float					fVehicleRoll[4];
	uint32_t				ulUnk2[3];
	float					fOnFootPos[3];
	float					fOnFootMoveSpeed[3];
	float					fVehiclePosition[3];
	float					fVehicleMoveSpeed[3];
	uint16_t				sPlayerID;
	uint16_t				sVehicleID;
	uint32_t				ulUnk5;
	uint8_t					byteUnk6[2];
	short					sShowNameTag;
	int						iHasJetPack;
	uint8_t					byteSpecialAction;
	struct stAimData		aimData;
	struct stInCarData		inCarData;
	struct stOnFootData		onFootData;
	struct stTrailerData	trailerData;
	struct stPassengerData	passengerData;
	uint32_t				ulUnk4[3];
	float					fActorArmor;
	float					fActorHealth;
	uint32_t				ulUnk10;
	uint8_t					byteUnk9;
	uint32_t				dwTick;
	uint32_t				dwLastStreamedInTick;
	uint32_t				ulUnk7;
	int						iAFKState;
	struct stHeadSync		headSyncData;
	int						iGlobalMarkerLoaded;
	int						iGlobalMarkerLocation[3];
	uint32_t				ulGlobalMarker_GTAID;
};

struct stRemotePlayer
{
	stRemotePlayerData	*pPlayerData;
	int					iIsNPC;
	void				*pVTBL_txtHandler;
	union
	{
		char			szPlayerName[16];
		char			*pszPlayerName;
	};
	int					iNameLen;
	int					iNameAllocated;
	int					iScore;
	int					iPing;
};

template <typename T>
struct stSAMPEntity
{
	void		*pVTBL;
	uint8_t		byteUnk0[60]; 
	T			*pGTAEntity;
	uint32_t	ulGTAEntityHandle;
};

struct stSAMPPed
{
	int					usingCellPhone;
	uint8_t				byteUnk0[600];
	struct actor_info	*pGTA_Ped;
	uint8_t				byteUnk1[22];
	uint8_t				byteKeysId;
	uint16_t			ulGTA_UrinateParticle_ID;
	int					DrinkingOrSmoking;
	int					object_in_hand;
	int					drunkLevel;
	uint8_t				byteUnk2[5];
	int					isDancing;
	int					danceStyle;
	int					danceMove;
	uint8_t				byteUnk3[20];
	int					isUrinating;
};

struct stVehiclePool
{
	int						iVehicleCount;
	void					*pUnk0;
	uint8_t					byteSpace1[0x112C];
	struct stSAMPVehicle	*pSAMP_Vehicle[SAMP_MAX_VEHICLES];
	int						iIsListed[SAMP_MAX_VEHICLES];
	struct vehicle_info		*pGTA_Vehicle[SAMP_MAX_VEHICLES];
	uint8_t					byteSpace2[SAMP_MAX_VEHICLES * 6];
	uint32_t				ulShit[SAMP_MAX_VEHICLES];
	int						iIsListed2[SAMP_MAX_VEHICLES];
	uint32_t				byteSpace3[SAMP_MAX_VEHICLES * 2];
	float					fSpawnPos[SAMP_MAX_VEHICLES][3];
	int						iInitiated;
};

struct stSAMPVehicle
{
	uint32_t			bUnk0;
	struct vehicle_info *pGTA_Vehicle;
	uint8_t				byteUnk1[8];
	int					bIsMotorOn;
	int					iIsLightsOn;
	int					iIsLocked;
	uint8_t				byteIsObjective;
	int					iObjectiveBlipCreated;
	uint8_t				byteUnk2[20];
	uint8_t				byteColor[2];
	int					iColorSync;
	int					iColor_something;
};

struct stObject
{
	uint8_t				byteUnk0[2];
	uint32_t			ulUnk1;
	int					iModel;
	uint8_t				byteUnk2;
	float				fDrawDistance;
	float				fUnk;
	float				fPos[3];
};

struct stObjectPool
{
	int				iObjectCount;
	int				iIsListed[SAMP_MAX_OBJECTS];
	struct stObject *object[SAMP_MAX_OBJECTS];
};

struct stGangzone
{
	float	fPosition[4];
	DWORD	dwColor;
	DWORD	dwAltColor;
};

struct stGangzonePool
{
	struct stGangzone	*pGangzone[SAMP_MAX_GANGZONES];
	int					iIsListed[SAMP_MAX_GANGZONES];
};

struct stTextLabel
{
	char		*pText;
	DWORD		color;
	float		fPosition[3];
	float		fMaxViewDistance;
	uint8_t		byteShowBehindWalls;
	uint16_t	sAttachedToPlayerID;
	uint16_t	sAttachedToVehicleID;
};

struct stTextLabelPool
{
	struct stTextLabel	textLabel[SAMP_MAX_3DTEXTS];
	int					iIsListed[SAMP_MAX_3DTEXTS];
};

struct stChatEntry
{
	uint32_t	SystemTime;
	char		szPrefix[28];
	char		szText[144];
	uint8_t		unknown[64];
	int			iType;	
	D3DCOLOR	clTextColor;
	D3DCOLOR	clPrefixColor;
};

struct stFontRenderer
{
	ID3DXFont	*m_pChatFont;
	ID3DXFont	*m_pLittleFont;
	ID3DXFont	*m_pChatShadowFont;
	ID3DXFont	*m_pLittleShadowFont;
	ID3DXFont	*m_pCarNumberFont;
	ID3DXSprite *m_pTempSprite;
	int			field_18;
	char		*m_pszTextBuffer;
};

struct stChatInfo
{
	int					pagesize;
	char				*pLastMsgText;
	int					iChatWindowMode;
	uint8_t				bTimestamps;
	uint32_t			ulUnk0;
	char				logFilePathChatLog[MAX_PATH + 1];
	void				*pChatbox_unknown[3];
	D3DCOLOR			clTextColor;
	D3DCOLOR			clInfoColor;
	D3DCOLOR			clDebugColor;
	DWORD				dwChatboxOffset;
	struct stChatEntry	chatEntry[100];
	stFontRenderer		*m_pFontRenderer;
	ID3DXSprite			*m_pChatTextSprite;
	ID3DXSprite			*m_pSprite;
	IDirect3DDevice9	*m_pD3DDevice;
	uint32_t			ulUnk1[2];
	IDirect3DTexture9	*m_pTexture;
	uint32_t			ulUnk2[7];
	int					m_iRedraw;
	int					m_nPrevScrollBarPosition;
	int					m_iDistanceAfterTimestamp;
	uint32_t			ulUnk3;
};

struct stInputBox
{
	void	*pUnknown;
	uint8_t	bIsChatboxOpen;
	uint8_t	bIsMouseInChatbox;
	uint8_t	bMouseClick_related;
	uint8_t	unk;
	DWORD	dwPosChatInput[2];
	uint8_t	unk2[263];
	int		iCursorPosition;
	uint8_t	unk3;
	int		iMarkedText_startPos;
	uint8_t	unk4[20];
	int		iMouseLeftButton;
};

typedef void(__cdecl *CMDPROC) (PCHAR);
struct stInputInfo
{
	void				*pD3DDevice;
	void				*pDXUTDialog;
	stInputBox			*pDXUTEditBox;
	CMDPROC				pCMDs[SAMP_MAX_CLIENTCMDS];
	char				szCMDNames[SAMP_MAX_CLIENTCMDS][33];
	int					iCMDCount;
	int					iInputEnabled;
	char				szInputBuffer[129];
	char				szRecallBufffer[10][129];
	char				szCurrentBuffer[129];
	int					iCurrentRecall;
	int					iTotalRecalls;
	CMDPROC				pszDefaultCMD;
};

struct stKillEntry
{
	char		szKiller[25];
	char		szVictim[25];
	D3DCOLOR	clKillerColor;
	D3DCOLOR	clVictimColor;
	uint8_t		byteType;
};

struct stKillInfo
{
	int					iEnabled;
	struct stKillEntry	killEntry[5];
	int					iXOffset[2];
	int					iYSpacing;
	ID3DXFont		    *pD3DFont;
	ID3DXFont			*pWeaponFont;
	ID3DXSprite			*pSprite;
	IDirect3DDevice9	*pD3DDevice;
};

struct stChatPlayer
{
	int		iCreated;
	char	probablyTheText[256];
	DWORD	dwTickCreated;
	DWORD	dwLiveLength;
	DWORD	dwColor;
	float	fDrawDistance;
	DWORD	dwUnknown;
};

struct stMiscInfo
{
	void	*pUnk0[3];
	uint8_t	byteSpace1[73];
	int		iCursorMode;
	int		iSomething[5];
};

struct stScoreboardInfo
{
	int					iIsEnabled;
	int					iPlayersCount;
	float				fTextOffset[2];
	float				fScalar;
	float				fSize[2];
	float				fUnk0[5];
	IDirect3DDevice9	*pDirectDevice;
	class _CDXUTDialog	*pDialog;
	class _CDXUTListBox *pList;
	int					iOffset;		
	int					iIsSorted;		
};

struct stActorPool
{
	int					iLastActorID;
	stSAMPEntity<void>	*pActor[SAMP_MAX_ACTORS];
	int					iIsListed[SAMP_MAX_ACTORS];
	struct actor_info	*pGTAPed[SAMP_MAX_ACTORS];
	uint32_t			ulUnk0[SAMP_MAX_ACTORS];
	uint32_t			ulUnk1[SAMP_MAX_ACTORS];
};

struct stChatBubbleInfo
{
	struct stChatPlayer	chatBubble[SAMP_MAX_PLAYERS];
};

struct stStreamedOutPlayerInfo
{
	int					iPlayerID[SAMP_MAX_PLAYERS];
	float				fPlayerPos[SAMP_MAX_PLAYERS][3];
};

#pragma pack(pop)

class SAMP
{
public:
	bool isInited;

	SAMP(HMODULE hModule)
	{
		isInited = false;
		if (hModule) dwSAMPAddr = (DWORD)hModule;
	};

	~SAMP()
	{
		delete g_SAMP;
		delete g_Chat;
		delete g_Input;
	};

	bool IsInitialized();

	void AddChatMessage(D3DCOLOR, char*);
	void registerChatCommand(char*, CMDPROC);
	void ToggleCursor(bool);

	struct stSAMP *getInfo() { return g_SAMP; };
	struct stPlayerPool *getPlayers() { return g_Players; };
	struct stVehiclePool *getVehicles() { return g_Vehicles; };
	struct stChatInfo *getChat() { return g_Chat; };
	struct stInputInfo *getInput() { return g_Input; };

private:

	DWORD dwSAMPAddr;
	struct stSAMP *g_SAMP;
	struct stPlayerPool *g_Players;
	struct stVehiclePool *g_Vehicles;
	struct stChatInfo *g_Chat;
	struct stInputInfo *g_Input;
};
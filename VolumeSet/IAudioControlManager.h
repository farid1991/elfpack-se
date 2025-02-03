#ifndef _CLASSES_H_
#define _CLASSES_H_

UUID CID_CAudioControlManager = {0xE2, 0, 0x10, 0x71, 0x81, 0xD3, 0x43, 0xC3, 0xAB, 0xEA, 0xD4, 0xF4, 0xEB, 0x67, 0xC8, 0x93};
UUID IID_IAudioControlManager = {0x78, 0xE, 0x15, 0xD9, 0x40, 0xF5, 0x4F, 0x99, 0xAF, 0x93, 0x38, 0x3B, 0x38, 0x7F, 0xA7, 0xED};

const unsigned short AUDIOCONTROL_DEBUGMENU_STRING_MAXLENGTH = 79;
typedef wchar_t TAudioControl_DebugMenuString[AUDIOCONTROL_DEBUGMENU_STRING_MAXLENGTH+1];

typedef enum
{
  AUDIOCONTROL_VOLUMETYPE_CURRENT,    /**< The current volumetype */
  AUDIOCONTROL_VOLUMETYPE_CALLVOLUME, /**< The call volume */
  AUDIOCONTROL_VOLUMETYPE_RINGVOLUME, /**< The ring signal volume */
  AUDIOCONTROL_VOLUMETYPE_MEDIAVOLUME, /**< The media volume */
  AUDIOCONTROL_VOLUMETYPE_INPUTVOLUME /**< FOR AUDIOCONTROL INTERNAL USE ONLY */
} TAudioControl_VolumeType;

typedef enum
{
  AUDIOCONTROL_VIBRATORSTATE_OFF,           /**< The vibrator is always off */
  AUDIOCONTROL_VIBRATORSTATE_ON,            /**< The vibrator is used when appropriate */
  AUDIOCONTROL_VIBRATORSTATE_ON_IF_SILENT   /**< The vibrator is used when appropriate, if silentmode is on */
} TAudioControl_VibratorState;

typedef enum
{
  AUDIOCONTROL_TTYMODE_OFF,  /**< TTY mode set to off*/
  AUDIOCONTROL_TTYMODE_STANDARD,  /**< TTY mode set to standard*/
  AUDIOCONTROL_TTYMODE_HCO,  /**< TTY mode set to hearing carry over*/
  AUDIOCONTROL_TTYMODE_VCO /**< TTY mode set to voice carry over*/
} TAudioControl_TTYMode;

typedef enum
{
  AUDIOCONTROL_HACMODE_OFF,
  AUDIOCONTROL_HACMODE_MIC,
  AUDIOCONTROL_HACMODE_TCOIL
} TAudioControl_HACMode;

typedef enum
{
  AUDIOCONTROL_CALLTYPE_VOICE,  /**< VoiceCall */
  AUDIOCONTROL_CALLTYPE_VIDEO  /**< VideoCall */
} TAudioControl_CallType;

typedef enum
{
  AUDIOCONTROL_CALLSTATE_IDLE,     /**< Should be used when no call is active */
  AUDIOCONTROL_CALLSTATE_ANSWERING,/**< Should be used before ICall_Answer to indicate that a call is about to be answered*/
  AUDIOCONTROL_CALLSTATE_ALERTING, /**< Obsolete, replaced with AUDIOCONTROL_CALLSTATE_ANSWERING*/
  AUDIOCONTROL_CALLSTATE_SETUP,    /**< Should be used before initiating an outgoing call*/
  AUDIOCONTROL_CALLSTATE_ONGOING,  /**< Should be used when a call is active*/
  AUDIOCONTROL_CALLSTATE_HOLDING  /**< Should be used when all calls are on hold*/
} TAudioControl_CallState;

typedef enum
{
  AUDIOCONTROL_SPEAKERTYPE_EARPIECE, /**< The earpiece speaker */
  AUDIOCONTROL_SPEAKERTYPE_SPEAKER,  /**< The builtin speaker */
  AUDIOCONTROL_SPEAKERTYPE_AUTO      /**< Automatically choose speaker */
} TAudioControl_SpeakerType;

class IUnknown
{
public:
  virtual void* pguid();
  virtual void* QueryInterface();
  virtual void* AddRef();
  virtual void* Release();
};

class IAudioControl: public IUnknown
{
public:
  virtual int unk_0x10();
  virtual int unk_0x14();
  virtual int unk_0x18();
  virtual int unk_0x1C();
  virtual int unk_0x20();
  virtual int unk_0x24();
  virtual int unk_0x28();
  virtual int unk_0x2C();
  virtual int unk_0x30();
  virtual int unk_0x34();
  virtual int unk_0x38();
  virtual int unk_0x3C();
  virtual int unk_0x40();
  virtual int unk_0x44();
  virtual int unk_0x48();
  virtual int unk_0x4C();
  virtual int unk_0x50();
  virtual int unk_0x54();
  virtual int unk_0x58();
  virtual int unk_0x5C();
  virtual int unk_0x60();
  virtual int unk_0x64();
  virtual int unk_0x68();
  virtual int unk_0x6C();
  virtual int unk_0x70();
  virtual int unk_0x74();
  virtual int unk_0x78();
  virtual int unk_0x7C();
  virtual int unk_0x80();
  virtual int unk_0x84();
  virtual int unk_0x88();
  virtual int unk_0x8C();
  virtual int unk_0x90();
  virtual int IsAudioPlaying(bool* pAudioPlaying, TAudioControl_VolumeType* pVolumeType);
  virtual int IsVibratorRunning(bool* pVibratorRunning);
  virtual int unk_0x9C();
  virtual int unk_0xA0();
  virtual int unk_0xA4();
  virtual int unk_0xA8();
  virtual int unk_0xAC();
  virtual int unk_0xB0();
  virtual int unk_0xB4();
  virtual int unk_0xB8();
  virtual int Volume_Set(TAudioControl_VolumeType volumeType, char pVolume);
  virtual int Volume_Get(TAudioControl_VolumeType volumeType, char* pVolume);
  virtual int unk_0xC4();
  virtual int unk_0xC8();
  virtual int unk_0xCC();
  virtual int unk_0xD0();
  virtual int IncreaseVolume(TAudioControl_VolumeType volumeType, char* pNewVolume);
  virtual int DecreaseVolume(TAudioControl_VolumeType volumeType, char* pNewVolume);
  virtual int unk_0xDC();
  virtual int unk_0xE0();
  virtual int unk_0xE4();
  virtual int unk_0xE8();
  virtual int unk_0xEC();
  virtual int unk_0xF0();
  virtual int unk_0xF4();
  virtual int unk_0xF8();
  virtual int unk_0xFC();
  virtual int unk_0x100();
  virtual int unk_0x104();
  virtual int unk_0x108();
  virtual int unk_0x10C();
  virtual int unk_0x110();
  virtual int unk_0x114();
  virtual int unk_0x118();
  virtual int unk_0x11C();
  virtual int unk_0x120();
  virtual int unk_0x124();
  virtual int unk_0x128();
  virtual int unk_0x12C();
  virtual int unk_0x130();
  virtual int unk_0x134();
  virtual int unk_0x138();
  virtual int unk_0x13C();
  virtual int unk_0x140();
  virtual int unk_0x144();
  virtual int unk_0x148();
  virtual int unk_0x14C();
  virtual int unk_0x150();
  virtual int unk_0x154();
  virtual int DebugMenu_GetString(TAudioControl_DebugMenuString* pBuffer);
  virtual int unk_0x15C();
  virtual int unk_0x160();
  virtual int unk_0x164();
  virtual int unk_0x168();
  virtual int unk_0x16C();
  virtual int SilentMode_Set(bool silentModeEnabled);
  virtual int SilentMode_Get(bool* silentModeEnabled);
  virtual int TTYMode_Set(TAudioControl_TTYMode TTYMode);
  virtual int TTYMode_Get(TAudioControl_TTYMode* pTTYMode);
  virtual int HACMode_Set(TAudioControl_HACMode HACMode);
  virtual int HACMode_Get(TAudioControl_HACMode* pHACMode);
  virtual int VibratorState_Set(TAudioControl_VibratorState vibratorState);
  virtual int VibratorState_Get(TAudioControl_VibratorState* pVibratorState);
  virtual int Vibrator_Start(int period, int dutyCycle, int duration);
  virtual int Vibrator_Stop(void);
  virtual int Callstate_Set(TAudioControl_CallType callType, TAudioControl_CallState callState);
  virtual int unk_0x19C();
  virtual int Camera_FollowSilentMode_Set(bool followSilentMode);
  virtual int Camera_FollowSilentMode_Get(bool* pFollowSilentMode);
  virtual int unk_0x1A8();
  virtual int unk_0x1AC();
  virtual int unk_0x1B0();
  virtual int unk_0x1B4();
  virtual int unk_0x1B8();
  virtual int unk_0x1BC();
};

class IAudioControlManager: public IUnknown
{
public:
  virtual int AudioControl(IAudioControl** ppIAudioControl);
};

#endif

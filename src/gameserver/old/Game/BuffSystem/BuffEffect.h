// BuffEffect.h: interface for the CBuffEffect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUFFEFFECT_H__F0F1694E_8F69_4585_A186_C80F6AB2BE61__INCLUDED_)
#define AFX_BUFFEFFECT_H__F0F1694E_8F69_4585_A186_C80F6AB2BE61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BuffScriptLoader.h"

enum EFFECTTYPE_DEFINE
{
	EFFECTTYPE_NONE = 0x0,
	EFFECTTYPE_ATTACKSPEED = 0x1,
	EFFECTTYPE_IMPROVE_DAMAGE = 0x2,
	EFFECTTYPE_IMPROVE_DEFENSE = 0x3,
	EFFECTTYPE_HP = 0x4,
	EFFECTTYPE_MANA = 0x5,
	EFFECTTYPE_EXPERIENCE = 0x6,
	EFFECTTYPE_ITEMDROPRATE = 0x7,
	EFFECTTYPE_EXPERIENCE_ZERO = 0x8,
	EFFECTTYPE_STRENGTH = 0x9,
	EFFECTTYPE_DEXTERITY = 0xa,
	EFFECTTYPE_VITALITY = 0xb,
	EFFECTTYPE_ENERGY = 0xc,
	EFFECTTYPE_LEADERSHIP = 0xd,
	EFFECTTYPE_MELEEDAMAGE = 0xe,
	EFFECTTYPE_MAGICDAMAGE = 0xf,
	EFFECTTYPE_MAPMOVEFREE = 0x10,
	EFFECTTYPE_CRITICAL_DMG = 0x11,
	EFFECTTYPE_GIVE_DMG_TICK = 0x12,
	EFFECTTYPE_POISON_DMG_TICK = 0x13,
	EFFECTTYPE_REDUCE_MOVE_SPEED = 0x14,
	EFFECTTYPE_IMPROVE_MELEE_DEFENSE = 0x15,
	EFFECTTYPE_IMPROVE_MAGIC_DEFENSE = 0x16,
	EFFECTTYPE_DECREASE_DEFENSE = 0x17,
	EFFECTTYPE_DAMAGEREFLECT = 0x18,
	EFFECTTYPE_SLEEP = 0x19,
	EFFECTTYPE_REDUCE_ATTACK_RATE = 0x1a,
	EFFECTTYPE_CURSED_DAMAGE = 0x1b,
	EFFECTTYPE_MELEE_DEFENSE_DOWN_MANA = 0x1c,
	EFFECTTYPE_CURSED_ATTACKDOWN = 0x1d,
	EFFECTTYPE_CURSED_DEFENSEDOWN = 0x1e,
	EFFECTTYPE_BERSERKER_UP = 0x1f,
	EFFECTTYPE_BERSERKER_DOWN = 0x20,
	EFFECTTYPE_MAGICPOWER_INC = 0x21,
	EFFECTTYPE_MAGICPOWERMAX_INC = 0x22,
	EFFECTTYPE_IMPROVE_DAMAGE_PER = 0x33,
	EFFECTTYPE_ABSORB_DAMAGE = 0x34,
	EFFECTTYPE_RECOVERY_HP = 0x35,
	EFFECTTYPE_RECOVERY_MANA = 0x36,
	EFFECTTYPE_CRITICALDAMAGE = 0x37,
	EFFECTTYPE_EXCELLENTDAMAGE = 0x38,
	EFFECTTYPE_POWER_UP = 0x39,
	EFFECTTYPE_GUARD_UP = 0x3a,
	EFFECTTYPE_AG_RECOVERY_UP = 0x3b,
	EFFECTTYPE_VULCANUS_EXPERIENCE = 0x3c,
	EFFECTTYPE_VULCANUS_ITEMDROPRATE = 0x3d,
	EFFECTTYPE_GUARD_EXPNITEM = 0x3e,
	EFFECTTYPE_PROTECT_ITEM = 0x3f,
	EFFECTTYPE_ZEN_UP = 0x40,
	EFFECTTYPE_AG_UP = 0x42,
	EFFECTTYPE_SD_UP = 0x43,
	EFFECTTYPE_AG_UP_VALUE = 0x47,
	EFFECTTYPE_SD_UP_VALUE = 0x46,
	EFFECTTYPE_IGNORE_OPPONENT_DEFENSE = 0x48,
	EFFECTTYPE_MONK_VITALITY = 0x49,
	EFFECTTYPE_IMPROVE_DEFENSE_RATE = 0x4A,
	EFFECTTYPE_DECREASE_DEFENSE_RATE = 0x4B,
	EFFECTTYPE_MONK_DECREASE_DEFENSE = 0x4C,
	EFFECTTYPE_ELF_BLESS = 0x4D,
	EFFECTTYPE_BLIND = 0x4E,
	EFFECTTYPE_REDUCE_ATTACK_SPEED = 0x4F,
	EFFECTTYPE_FILLHP = 0x50,
	EFFECTTYPE_REDUCE_HP_HALF = 0x51,
	EFFECTTYPE_REDUCE_MP_HALF = 0x52,
	EFFECTTYPE_REDUCE_SD_HALF = 0x53,
	EFFECTTYPE_ASCENSION_MARK_EXP = 0x57,
	EFFECTTYPE_LIGHT_BLESS_EXP = 0x58,
	EFFECTTYPE_AG_OPPONENT_DECREASE = 0x59,
	EFFECTTYPE_WRATH_INC_DAMAGE = 0x5A
};

struct PMSG_ANS_PERIODBUFF_SELECT
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	char szCharacterName[MAX_ACCOUNT_LEN + 1];
	BYTE btResultCode;
	WORD wBuffIndex;
	BYTE btEffectType1;
	BYTE btEffectType2;
	time_t lExpireDate;
};

class CBuffEffect  
{
public:
	CBuffEffect();
	virtual ~CBuffEffect();

	void SetBuffEffect(CGameObject &Obj, BYTE EffectType, int EffectValue);
	void ClearBuffEffect(CGameObject &Obj, BYTE EffectType, int EffectValue);
	void SetActiveBuffEffect(CGameObject &Obj, BYTE EffectType, int EffectValue);
	void SetPrevEffect(CGameObject &Obj);
	void SetNextEffect(CGameObject &Obj);
	void ClearPrevEffect(CGameObject &Obj);
	void GiveDamageEffect(CGameObject &Obj, int Damage);
	void PoisonEffect(CGameObject &Obj, BYTE PoisonRate);
	void GiveDamageFillHPEffect(CGameObject &Obj, int Damage);
	void RequestGuildPeriodBuffInsert(char *szGuildName,PeriodBuffInfo *lpBuffInfo);
	void RequestGuildPeriodBuffDelete(WORD *wBuffIndex, char btGuildCnt);
	void RequestPeriodBuffDelete(CGameObject &Obj, WORD wBuffIndex);
	void RequestPeriodBuffDelete(char *szName, WORD wBuffIndex);
	void RequestPeriodBuffInsert(CGameObject &Obj, PeriodBuffInfo *lpBuffInfo);
	void RequestPeriodBuffSelect(CGameObject &Obj);
	void DGPeriodItemExSelect(PMSG_ANS_PERIODBUFF_SELECT *lpMsg);
};

extern CBuffEffect	g_BuffEffect;

#endif // !defined(AFX_BUFFEFFECT_H__F0F1694E_8F69_4585_A186_C80F6AB2BE61__INCLUDED_)


////////////////////////////////////////////////////////////////////////////////
//  vnDev.Games - MuServer S12EP2 IGC v12.0.1.0 - Trong.LIVE - DAO VAN TRONG  //
////////////////////////////////////////////////////////////////////////////////


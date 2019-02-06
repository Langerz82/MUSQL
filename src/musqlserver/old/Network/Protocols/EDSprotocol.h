// EDSprotocol.h: interface for the EDSprotocol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDSPROTOCOL_H__675D1E8D_B9AC_45E3_8077_17EC82A9A5A5__INCLUDED_)
#define AFX_EDSPROTOCOL_H__675D1E8D_B9AC_45E3_8077_17EC82A9A5A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;


#include "StdAfx.h"
#include "ProtocolStructs.h"
#include "GensSystemProtocol.h"
#include "Fprotocol.h"

#define MAX_MEMBER_GUILD	80

class CPartyMatchingData
{
public:
	CPartyMatchingData();
	~CPartyMatchingData();

	void Init();

	void CreateParty(int Server, int ServerIndex, int nLeaderIndex, char *szLeaderName);
	void DeleteParty(char *szLeaderName);

	PARTYMATCHING_PARTYDATA * GetPartyDataByMaster(char *szLeaderName);
	PARTYMATCHING_PARTYDATA * GetPartyDataByMember(char *szMemberName);
	PARTYMATCHING_PARTYDATA * GetPartyDataByNumber(int nPartyMatchingIndex);

	PARTYMATCHING_PARTYUSERDATA * GetPartyMemberData(char *szMemberName);

	int AddPartyMember(char *szLeaderName, char *szMemberName, int nMemberIndex, int nMemberServer, int nMemberServerIndex);
	int DelPartyMember(char *szLeaderName, char *szMemberName);

	int ConnectMember(char *szMemberName, int nMemberIndex, int nChannelNumber, int nChannelIndex);
	int DisconnectMember(char *szMemberName);

private:
	CRITICAL_SECTION							m_criti;
	std::vector<PARTYMATCHING_PARTYDATA>		m_vtPartyData;

};

#define MAX_GENS_CLASS				14
#define GENS_REWARD_UNAVAILABLE		0
#define GENS_REWARD_WAITING			1
#define GENS_REWARD_ALREADY_GIVEN	2

class GensSystem_EDS
{
public:

	GensSystem_EDS();
	~GensSystem_EDS();

	BOOL InitGensSystem();
	void GensRankingProcess();
	void ManualRefreshRanking(BYTE Type);
	BOOL LoadGensData(LPSTR lpszFile);
	int GetGensInfluence(char *szUserName);

	void GDReqRegGensMember(int aIndex, _tagPMSG_REQ_REG_GENS_MEMBER_EXDB *aRecv);
	void GDReqSecedeGensMember(int aIndex, _tagPMSG_REQ_SECEDE_GENS_MEMBER_EXDB *aRecv);
	void GDReqAbusingInfo(int aIndex, _tagPMSG_REQ_ABUSING_INFO *aRecv);
	void GDReqGensInfo(int aIndex, _tagPMSG_REQ_ABUSING_INFO *aRecv);
	void GDReqSaveContributePoint(int aIndex, _tagPMSG_REQ_SAVE_CONTRIBUTE_POINT_EXDB *aRecv);
	void GDReqSaveAbusingKillUserName(int aIndex, _tagPMSG_REQ_SAVE_ABUSING_KILLUSER_EXDB *aRecv);
	void GDReqGensRewardCheck(int aIndex, _tagPMSG_REQ_GENS_REWARD_CHECK_EXDB *aRecv);
	void GDReqGensRewardComplete(int aIndex, _tagPMSG_REQ_GENS_REWARD_COMPLETE_EXDB *aRecv);
	void GDReqGensMemberCount(int aIndex, _tagPMSG_REQ_GENS_MEMBER_COUNT *aRecv);
	void GDReqSetGensRewardDay(int aIndex, _tagPMSG_SET_GENS_REWARD_DAY_EXDB *aRecv);
	void GDReqGensRewardDay(int aIndex, _tagPMSG_REQ_GENS_REWARD_DAY *aRecv);

private:

	int CalcGensClass(int ContributePoint, int Ranking);
	void MakeRanking();
	void ReloadCurrentMonth();

	std::map<string, GENS_USER, strCmp> m_mapGensRankDuprian;
	std::map<string, GENS_USER, strCmp> m_mapGensRankVanert;

	_tagGENS_RANKING m_GensRankingData[14];

	CRITICAL_SECTION m_GensCriticalSection;
	DWORD m_GensRankingUpdateTimeTick;
	DWORD m_RewardMonth;



	//CLogToFile m_RankingLog;
	int m_DuprianUserCount;
	int m_VanertUserCount;
	int m_GensRankingMonth;
	int m_LeaveDateDayDiff;
	int m_GensRewardStartDay;
	int m_GensRewardEndDay;
	int m_MinRankToGainReward;
	TCHAR g_GensRankingPath[96];
};

class CExDataServerProtocol
{
public:

	CExDataServerProtocol();
	~CExDataServerProtocol();
	
	friend class CFriendSystem;
	static void ProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
	BOOL Init();
	BOOL DBConnect();
	
	void GensManualRefreshRanking(BYTE Type);
	void GensRankingProcess();

	void GuildMatchingSendAcceptAndRejectInfo(int aIndex, char* szUserName, int nUserIndex, int nServerIndex);

private:

	void ExDataServerLogin(int aIndex, SDHP_SERVERINFO * lpMsg);	
	void GDCharCloseRecv(int aIndex, SDHP_USERCLOSE * aRecv);
	void GDGuildCreateSend(int aIndex, SDHP_GUILDCREATE * aRecv);
	void GDGuildDestroyRecv(int aIndex, SDHP_GUILDDESTROY * aRecv);
	void GDGuildMemberAdd(int aIndex, SDHP_GUILDMEMBERADD * aRecv);
	void GDGuildMemberAddWithoutUserIndex(int aIndex, SDHP_GUILDMEMBERADD_WITHOUT_USERINDEX * aRecv);
	void GDGuildMemberDel(int aIndex, SDHP_GUILDMEMBERDEL * aRecv);
	void DGGuildMemberInfoRequest(int aIndex, SDHP_GUILDMEMBER_INFO_REQUEST * aRecv);
	void DGGuildScoreUpdate(int aIndex, SDHP_GUILDSCOREUPDATE * aRecv);
	void GDGuildNoticeSave(int aIndex, SDHP_GUILDNOTICE * aRecv);
	void GDGuildReqAssignStatus(int aIndex, EXSDHP_GUILD_ASSIGN_STATUS_REQ * aRecv);
	void GDGuildReqAssignType(int aIndex, EXSDHP_GUILD_ASSIGN_TYPE_REQ * aRecv);
	void GDGuildServerGroupChattingSend(int aIndex,EXSDHP_SERVERGROUP_GUILD_CHATTING_RECV * aRecv);
	void GDUnionServerGroupChattingSend(int aIndex,EXSDHP_SERVERGROUP_UNION_CHATTING_RECV * aRecv);
	void GDGensServerGroupChattingSend(int aIndex,EXSDHP_SERVERGROUP_GENS_CHATTING_RECV * aRecv);
	void DGRelationShipAnsJoin(int aIndex, EXSDHP_RELATIONSHIP_JOIN_REQ * aRecv);
	void DGRelationShipAnsBreakOff(int aIndex, EXSDHP_RELATIONSHIP_BREAKOFF_REQ * aRecv);
	void DGUnionListRecv(int aIndex, EXSDHP_UNION_LIST_REQ * aRecv);
	void DGRelationShipAnsKickOutUnionMember(int aIndex, EXSDHP_KICKOUT_UNIONMEMBER_REQ *aRecv);
	void DGGuildMasterListSend(char *szGuild);
	void DGGuildInfoRequest(int aIndex, SDHP_GUILDMEMBER_INFO_GUILDNAME_REQUEST *aRecv);
	void DGRelationShipListSend(int aIndex, int iGuild, int relation_type, BOOL snd_all);
	void DGRelationShipNotificationSend(int iGuild, int iUpdateFlag);
	void FCHRoomCreateReq(int aIndex, char *szName, char *szFriendName, short Number, short ServerId, short FriendNumber, short FriendServerId);
	void FriendChatRoomCreateReq(int aIndex, FHP_FRIEND_CHATROOM_CREATE_REQ* lpMsg);
	void FCHChatRoomCreateReq(int aIndex, char *szName, char *szFriendName, short Number, short ServerId, short FriendNumber, short FriendServerId);
	void FriendChatRoomCreateAns(int aIndex, FCHP_CHATROOM_CREATE_RESULT* lpMsg);
	void FriendChatRoomInvitationReq(int aIndex, FHP_FRIEND_INVITATION_REQ * lpMsg);
	void FCHChatRoomInvitationReq(int aIndex, short RoomNumber,	char *szName, short Number,	short ServerId,	BYTE Type);
	void GDReqGuildPeriodBuffInsert(int aIndex, PMSG_REQ_GUILD_PERIODBUFF_INSERT *aRecv);
	void GDReqGuildPeriodBuffDelete(int aIndex, PMSG_REQ_GUILD_PERIODBUFF_DELETE *aRecv);

	void CloseExDataServer();

	LPGUILD_INFO_STRUCT GetGuild(char *szGuild);
	LPGUILD_INFO_STRUCT GetGuild(int iNumber);
	LPGUILD_MEMBER GetGuildMember(char *szGuild, char *szName);
	LPGUILD_MEMBER GetGuildMember(int nGuildNumber, char *szName);
	LPUNION_DATA GetUnionData(int iGuild);

	int GetGuildMemberCount(char *szGuild);
	int GetGuildMemberCount(int nGuildNumber);

	int CreateDBGuild(char* szGuild, char* szMaster, LPBYTE pMark);
	int GetDBGuildNumber(char *szGuild);

	BOOL GuildExists(char *szGuild);
	BOOL AddGuild(char *szGuild, char *szMaster, int iNumber, char *lpNotice, int iScore, LPBYTE pMark, int iType);
	BOOL DelGuild(char *szGuild);
	BOOL MasterDBExists(char *szMaster);
	BOOL DelDBGuild(char *szGuild);

	BOOL GuildMemberExists(char *szGuild, char *szName);
	BOOL AddGuildMember(char *szGuild, char *szName, BYTE btStatus = 0, BYTE btConnected = -1);
	BOOL DelAllDBGuildMember(char *szGuild);
	BOOL DBGuildMemberExists(char *szName);
	BOOL AddDBGuildMember(char *szGuild, char *szName);
	BOOL DelDBGuildMember(char *szName);
	BOOL DelGuildMember(char *szGuild, char *szName);

	BOOL UpdateGuildMemberStatus(char *szGuild, char *szName, BYTE btStatus);
	BOOL UpdateGuildType(char *szGuild, BYTE btType);

	void SendGuildMemberInfo(int aIndex, char *szName, int iUserIndex);
	BOOL GetGuildMemberInfo(char *szName, OUT char *szGuild, OUT int& riLevel, OUT int& riStatus);

	BOOL IsCSGuild(char *szGuild);

	BOOL UpdateScore(char *szGuild, int iScore);
	BOOL SaveNotice(char *szGuild, char *szNotice);

	BOOL CreateRelationShipData(int iGuild);
	BOOL DestroyRelationShipData(int iGuild);

	int RelationShipOperation(int iReqGuild, int iTargGuild, int relation_type, int operation);

	int RelationShipJoin(LPGUILD_INFO_STRUCT lpReqGuild, LPGUILD_INFO_STRUCT lpTargGuild, int type);
	int UnionJoin(LPGUILD_INFO_STRUCT lpReqGuild, LPGUILD_INFO_STRUCT lpTargGuild);
	int RivalJoin(LPGUILD_INFO_STRUCT lpReqGuild, LPGUILD_INFO_STRUCT lpTargGuild);

	int RelationShipBreak(LPGUILD_INFO_STRUCT lpMasterGuild, LPGUILD_INFO_STRUCT lpKickGuild, int type);
	int UnionBreak(LPGUILD_INFO_STRUCT lpMasterGuild, LPGUILD_INFO_STRUCT lpKickGuild);
	int RivalBreak(LPGUILD_INFO_STRUCT lpMasterGuild, LPGUILD_INFO_STRUCT lpKickGuild);

	BOOL AddUnion(LPGUILD_INFO_STRUCT lpReqGuild, LPGUILD_INFO_STRUCT lpTargGuild);
	BOOL AddUnion(int iReqGuild, int iTargGuild);
	BOOL KickUnion(LPGUILD_INFO_STRUCT lpMasterGuild, LPGUILD_INFO_STRUCT lpKickGuild);

	BOOL UpdateDBUnion(int iGuild, int iUnion);
	BOOL UpdateDBRival(int iGuild, int iRival);

	void RelationShipBreakSend(int iGuild, int iType);
	void SendListToAllRivals(LPGUILD_INFO_STRUCT lpGuild);

	int GetChatServer();

	void GDReqGuildMatchingList(int aIndex, _stReqGuildMatchingList * lpMsg);
	void GDReqGuildMatchingListSearchWord(int aIndex, _stReqGuildMatchingListSearchWord * lpMsg);
	void GDReqRegGuildMatchingList(int aIndex, _stReqGuildMatchingData * lpMsg);
	void GDReqDelMatchingList(int aIndex, _stReqDelGuildMatchingList * lpMsg);
	void GDReqJoinGuildMatchingList(int aIndex, _stRegWaitGuildMatching * lpMsg);
	void GDReqCancelJoinGuildMatching(int aIndex, _stReqDelWaitGuildMatchingList * lpMsg);
	void GDReqAllowJoinGuildMatching(int aIndex, _stReqAllowJoinGuildMatching * lpMsg);
	void GDReqWaitGuildMatchingList(int aIndex, _stReqWaitGuildMatchingList * lpMsg);
	void GDReqGetWaitStateListGuildMatching(int aIndex, _stReqWaitStateListGuildMatching * lpMsg);

	void SendNotiGuildMatchingForGuildMaster(int nGuildNumber);
	void SendUseGuildMatchingGuild(char *szName, int nGuildNumber);
	void UpdateGuildMatchingMemberCount(char *szGuildName, int nGuildMemberCnt);

	void GDReqRegWantedPartyMember(int aIndex, _stReqRegWantedPartyMember * lpMsg);
	void GDReqGetPartyMatchingList(int aIndex, _stReqGetPartyMatchingList * lpMsg);
	void GDReqJoinMemberPartyMatching(int aIndex, _stReqJoinMemberPartyMatching * lpMsg);
	void GDReqJoinMemberStateList(int aIndex, _stReqJoinMemberStateListPartyMatching *lpMsg);
	void GDReqJoinMemberStateListLeader(int aIndex, _stReqWaitListPartyMatching *lpMsg);
	void GDReqAcceptMemberJoin(int aIndex, _stReqAddPartyMember *lpMsg);
	void GDReqCancelPartyMatching(int aIndex, _stReqCancelPartyMatching *lpMsg);
	void GDReqDeletePartyUser(int aIndex, _stReqDelPartyUserPartyMatching *lpMsg);
	void GDReqSendPartyMemberList(int aIndex, _stReqSendPartyMemberList * lpMsg);
	void GDSendChatMsgPartyMatching(int aIndex, _stReqChattingPartyMatching * lpMsg);

	void FixAddPartyMember(int nLeaderIndex, int nMemberIndex, int nServerIndex);
	void SendPartyMatchingMemberList(char *szLeaderName, BYTE btType);
	void SendPartyMatchingOneList(char *szMemberName);
	void AutoAddPartyMember(char *szLeaderName, char *szMemberName, int nUserIndex, int nServerIndex);

	std::map<std::string, tagGUILD_INFO_STRUCT,strCmp> m_MapGuildManager;
	std::map<int, tagUNION_DATA> m_MapUnionManager;

	GensSystem_EDS				m_GensSystemEDS;
	CFriendSystem				m_FriendSystemEDS;
	CPartyMatchingData			m_PartyMatchingEDS;

};

#endif // !defined(AFX_EDSPROTOCOL_H__675D1E8D_B9AC_45E3_8077_17EC82A9A5A5__INCLUDED_)
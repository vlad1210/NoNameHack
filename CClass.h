#include <Windows.h>
#include "Addresses.h"
class SSystemGlobalEnvironment;
class IGameFramework;
class CGameRules;
class IActorSystem;
class IActor;
class CPlayer;
class IEntitySystem;
class IEntityIt;
class IEntity;
class IEntityRenderProxy;
class IRenderNode;

class ClientDevice;
class IDirectDevice;

typedef unsigned __int32 EntityId;

template <typename T> 
T cCall(void* pBase, int iIndex) { return (*(T**)pBase)[iIndex / 8]; }

typedef bool(__thiscall* bIsSameTeam)(CGameRules * pGameRules, EntityId iEntityId1, EntityId iEntityId2);


class IGameFramework
{
public:

	IActorSystem*  pGetIActorSystem(bool bIsRuClient = true){ return cCall<IActorSystem*(__fastcall*)(PVOID)>(this, bIsRuClient ? F_GET_IACTORSYSTEM : F_GET_IACTORSYSTEM_EU)(this); }

	CGameRules* pGetGameRules(bool bIsRuClient = true){ return cCall<CGameRules*(__fastcall*)(PVOID)>(this, bIsRuClient ? F_GET_GAMERULES : F_GET_GAMERULES_EU)(this); }

	bool bGetClientActor(IActor** pAct, bool bIsRuClient = true){ return cCall<bool(__fastcall*)(PVOID, IActor**)>(this, bIsRuClient ? F_GET_CLIENTACTOR : F_GET_CLIENTACTOR_EU)(this, pAct) && *pAct != NULL; }

};//class IGameFramework

class CGameRules
{
public:

	bool bIsSameTeam(EntityId iId1, EntityId iId2, bool bIsRuClient = true){ return ((::bIsSameTeam)(bIsRuClient ? I_IS_SAME_TEAM_FA : I_IS_SAME_TEAM_FA_EU))(this, iId1, iId2); }

};//class CGameRules

class IActorSystem
{
public:

	IActor *pGetActor(EntityId entityId, bool bIsRuClient = true){ return cCall<IActor*(__fastcall*)(PVOID, EntityId)>(this, bIsRuClient ? AS_GET_ACTOR : AS_GET_ACTOR_EU)(this, entityId); }

};//class IActorSystem

class IActor
{
public:

	bool bIsDead(bool bIsRuClient = true){ return cCall<bool(__fastcall*)(PVOID)>(this, bIsRuClient ? A_ISDEAD : A_ISDEAD_EU)(this); }

	CPlayer* pGetCPlayer(bool bIsRuClient = true){ return *(CPlayer**)((DWORD)this + (bIsRuClient ? A_GET_PLAYER : A_GET_PLAYER_EU)); }

	EntityId iGetId(bool bIsRuClient = true){ return *(EntityId*)((DWORD)this + (bIsRuClient ? A_GET_ID : A_GET_ID_EU)); }

};//class IActor

class CPlayer
{
public:

	void vIgnoreRecoil(bool bStatus = true, bool bIsRuClient = true){ *(bool*)((DWORD)this + (bIsRuClient ? P_IGNORE_RECOIL : P_IGNORE_RECOIL_EU)) = bStatus; }

};//class CPlayer

class IEntitySystem
{
public:
	
	IEntityIt* pGetEntityIterator(bool bIsRuClient = true){ return cCall<IEntityIt*(__fastcall*)(PVOID)>(this, bIsRuClient ? IES_GET_ENTITY_ITERATOR : IES_GET_ENTITY_ITERATOR_EU)(this); }

};//class IEntitySystem

class IEntityIt
{
public:

	IEntity* pGetNext(bool bIsRuClient = true){ return cCall<IEntity*(__fastcall*)(PVOID)>(this, bIsRuClient ? IEI_GET_NEXT : IEI_GET_NEXT_EU)(this); }

};//class IEntityIt

class IEntity
{
public:

	IEntityRenderProxy* pGetRenderProxy(bool bIsRuClient = true){ return cCall<IEntityRenderProxy*(__fastcall*)(PVOID, int)>(this, bIsRuClient ? IE_GET_PROXY : IE_GET_PROXY_EU)(this, 0); }

	EntityId iGetId(bool bIsRuClient = true){ return cCall<EntityId(__fastcall*)(PVOID)>(this, bIsRuClient ? IE_GET_ID : IE_GET_ID_EU)(this); }

};//class IEntity

class IEntityRenderProxy
{
public:

	IRenderNode*  pGetRenderNode(bool bIsRuClient = true) { return cCall<IRenderNode*(__fastcall*)(PVOID)>(this, bIsRuClient ? IRP_GET_RENDERNODE : IRP_GET_RENDERNODE_EU)(this); }

	void vSetHUDSilhouettesParams(float a, float r, float g, float b, bool bIsRuClient = true){ return cCall<void(__fastcall*)(PVOID, float, float, float, float)>(this, bIsRuClient ? IRP_RENDER_PROXY_SETHUD : IRP_RENDER_PROXY_SETHUD_EU)(this, a, r, g, b); }

};//class IEntityRenderProxy

class IDirectDevice
{
public:

	ClientDevice* pGetDevice(bool bIsRuClient = true){ return *(ClientDevice**)((DWORD)this + (bIsRuClient ? IDD_GET_CLIENT_DEVICE : IDD_GET_CLIENT_DEVICE_EU)); }

};//class IDirectDevice

class ClientDevice
{
public:

	DWORD* pGetDevice(){ return *(DWORD**)(DWORD)this; }

};//class ClientDevice
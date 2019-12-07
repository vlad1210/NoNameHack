#include "CClass.h"
#include "MinHook.h"
#pragma comment (lib, "zxcawq.lib" )

class Hook;
class CSystem;
typedef HRESULT(APIENTRY* EndScene) (DWORD);

class CSystem
{
public:

	CSystem();
	void vHack();

private:
	
	bool is_ru_client;

	bool v_init(bool bIsRuClient = true);

	IGameFramework* p_game_framework;

	IEntitySystem*  p_entity_system;

	IEntityIt *p_ent_it;

	IEntity* p_entity;

	IActor* p_actor;

	IActor* p_player;

	IEntityRenderProxy* p_end_render;

	IActorSystem* p_actor_sys;

};//class CSystem

class Hook
{
public:

	void vHook(LPVOID pDetour, EndScene &hr_end_scene_orig);

private:

	bool v_hook(LPVOID pTarget, LPVOID pDetour, LPVOID *ppOriginal);

};//class Hook

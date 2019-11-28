#include "CSystem.h"

CSystem::CSystem() : is_ru_client(true), p_actor(NULL), p_player(NULL), p_game_framework(NULL), p_entity_system(NULL), p_end_render(NULL), p_ent_it(NULL), p_actor_sys(NULL){}//CSystem::CSystem()

bool CSystem::v_init(bool bIsRuClient)
{
	if (*(DWORD64*)(bIsRuClient ? I_IGAMEF_RAMEWORK_ADR : I_IGAMEF_RAMEWORK_ADR_EU) == NULL)
		return false;
	if (*(DWORD64*)(bIsRuClient ? I_SSYSTEMGLOBALENVIRONMENT_ADR : I_SSYSTEMGLOBALENVIRONMENT_ADR_EU) == NULL)
		return false;
	if (*(DWORD64*)(*(DWORD64*)(bIsRuClient ? I_SSYSTEMGLOBALENVIRONMENT_ADR : I_SSYSTEMGLOBALENVIRONMENT_ADR_EU) + (bIsRuClient ? SSG_ENTITY_SYSTEM : SSG_ENTITY_SYSTEM_EU)) == NULL)
		return false;

	p_game_framework = *(IGameFramework**)(bIsRuClient ? I_IGAMEF_RAMEWORK_ADR : I_IGAMEF_RAMEWORK_ADR_EU);

	p_entity_system = *(IEntitySystem**)(*(DWORD64*)(bIsRuClient ? I_SSYSTEMGLOBALENVIRONMENT_ADR : I_SSYSTEMGLOBALENVIRONMENT_ADR_EU) + (bIsRuClient ? SSG_ENTITY_SYSTEM : SSG_ENTITY_SYSTEM_EU));

	p_actor_sys = p_game_framework->pGetIActorSystem();

	if (p_actor_sys == NULL)
		return false;

	p_ent_it = p_entity_system->pGetEntityIterator();

	if (p_ent_it == NULL)
		return false;

	return true;
}

void CSystem::vHack()
{
	if (*(DWORD64*)(I_SSYSTEMGLOBALENVIRONMENT_ADR_EU + 0x10) != NULL)
		is_ru_client = false;

	if (!v_init(is_ru_client))
		return;
	
	if (!p_game_framework->bGetClientActor(&p_actor))
		return;

	if (p_actor->bIsDead())
		return;

	p_actor->pGetCPlayer()->vIgnoreRecoil();

	for (; p_entity = p_ent_it->pGetNext();)
	{
		if (p_player = p_actor_sys->pGetActor(p_entity->iGetId()))
		{
			if (!(p_game_framework->pGetGameRules()->bIsSameTeam(p_actor->iGetId(), p_player->iGetId(), is_ru_client)))
			{
				if (p_end_render = p_entity->pGetRenderProxy())
				{
					if (!p_player->bIsDead())
					{
						p_end_render->vSetHUDSilhouettesParams(255.f, 255.f, 0, 0);
						*(DWORD*)((DWORD)(p_end_render->pGetRenderNode()) + 0x1C) |= 0x10u;
					}//if (!p_player->bIsDead())
					else
						p_end_render->vSetHUDSilhouettesParams(0, 0, 0, 0);
				}//if(p_end_render = p_entity->pGetRenderProxy())
			}//if (!(p_game_framework->pGetGameRules()->bIsSameTeam(p_actor->iGetId(), p_player->iGetId())))
		}//if (p_player = p_actor_sys->pGetActor(p_entity->iGetId()))
	}//for (; p_entity = p_ent_it->pGetNext();)
}//void CSystem::vHack()


void Hook::vHook(LPVOID pDetour, EndScene &hr_end_scene_orig)
{
	bool is_ru_client = true;

	if (*(DWORD64*)(I_SSYSTEMGLOBALENVIRONMENT_ADR_EU + 0x10) != NULL)
		is_ru_client = false;

	while (*(DWORD64*)(is_ru_client ? I_DIRECT_DEVICE_ADR : I_DIRECT_DEVICE_ADR_EU) == NULL){}

	IDirectDevice* cTable = NULL;

	void* pInterface = NULL;

	DWORD64  dVtable[105];

	cTable = *(IDirectDevice**)(is_ru_client ? I_DIRECT_DEVICE_ADR : I_DIRECT_DEVICE_ADR_EU);


	pInterface = (PDWORD)cTable->pGetDevice()->pGetDevice();

	memcpy(&dVtable, (void*)pInterface, I_ENDSCENE_OFFSET * 10);

	hr_end_scene_orig = (EndScene)dVtable[I_ENDSCENE_OFFSET];

	v_hook((DWORD_PTR*)dVtable[I_ENDSCENE_OFFSET], pDetour, reinterpret_cast<void**>(&hr_end_scene_orig));

}//void Hook::vHook(LPVOID pDetour, EndScene &hr_end_scene_orig)

bool Hook::v_hook(LPVOID pTarget, LPVOID pDetour, LPVOID *ppOriginal)//use MinHook https://github.com/TsudaKageyu/minhook
{
	if (InitHook() != MH_OK) return false;

	if (CreateHook(pTarget, pDetour, ppOriginal) != MH_OK) return false;

	if (EnableHook(pTarget) != MH_OK) return false;

	return true;

}//bool Hook::v_hook(LPVOID pTarget, LPVOID pDetour, LPVOID *ppOriginal)
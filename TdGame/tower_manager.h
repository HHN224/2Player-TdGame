#ifndef _TOWER_MANAGER_H_
#define _TOWER_MANAGER_H_

#include"tower.h"
#include"tower_type.h"
#include"manager.h"
#include"archer_tower.h"
#include"axeman_tower.h"
#include"gunner_tower.h"
#include"config_manager.h"
#include"resources_manager.h"
#include"coin_manager.h"

#include<vector>

class TowerManager : public Manager<TowerManager>
{
	friend class Manager<TowerManager>;

public:
	void on_update(double dela)
	{
		for(Tower* tower : tower_list)
			tower->on_update(dela);
	}

	void on_render(SDL_Renderer* renderer)
	{
		for (Tower* tower : tower_list)
			tower->on_render(renderer);
	}

	double get_place_cost(TowerType type)
	{
		static ConfigManager* instance = ConfigManager:: GetInstance();

		switch (type)
		{
		case Archer:
			return instance->archer_template.cost[instance->level_archer];
			break;
		case Axeman:
			return instance->axeman_template.cost[instance->level_axeman];
			break;
		case Gunner:
			return instance->gunner_template.cost[instance->level_gunner];
			break;
		}

		return 0;
	}

	double on_upgrade_cost(TowerType type)
	{
		static ConfigManager* instance = ConfigManager::GetInstance();

		switch (type)
		{
		case Archer:
			return instance->level_archer == 9 ? -1 : instance->archer_template.upgrade_cost[instance->level_archer];
			break;
		case Axeman:
			return instance->level_axeman == 9 ? -1 : instance->axeman_template.upgrade_cost[instance->level_axeman];
			break;
		case Gunner:
			return instance->level_gunner == 9 ? -1 : instance->gunner_template.upgrade_cost[instance->level_gunner];
			break;
		}

		return 0;
	}

	double get_damage_range(TowerType type)
	{
		static ConfigManager* instance = ConfigManager::GetInstance();

		switch (type)
		{
		case Archer:
			return instance->archer_template.view_range[instance->level_archer];
			break;
		case Axeman:
			return instance->axeman_template.view_range[instance->level_axeman];
			break;
		case Gunner:
			return instance->gunner_template.view_range[instance->level_gunner];
			break;
		}

		return 0;
	}

	void place_tower(TowerType type, const SDL_Point& idx)
	{
		Tower* tower = nullptr;

		ConfigManager:: GetInstance()->map.place_tower(idx);

		switch (type)
		{
		case Archer:
			tower = new ArcherTower();
			break;
		case Axeman:
			tower = new AxemanTower();
			break;
		case Gunner:
			tower = new GunnerTower();
			break;
		default:
			tower = new ArcherTower();
			break;
		}

		static Vector2 position;
		static const SDL_Rect& rect = ConfigManager:: GetInstance()->rect_tile_map;

		position.x = rect.x + idx.x * SIZE_TILE + SIZE_TILE / 2;
		position.y = rect.y + idx.y * SIZE_TILE + SIZE_TILE / 2;
		tower->set_position(position);
		tower->set_idx_tile(idx);
		tower_list.push_back(tower);

		static const ResourcesManager:: SoundPool& sound_pool = ResourcesManager:: GetInstance()->get_sound_pool();

		Mix_PlayChannel(-1, sound_pool.find(ResID:: Sound_PlaceTower)->second, 0);
	}

	void upgrade_tower(TowerType type)
	{
		static ConfigManager* instance = ConfigManager:: GetInstance();

		switch (type)
		{
		case Archer:
			instance->level_archer = instance->level_archer > 9 ? 9 : instance->level_archer + 1;
			break;
		case Axeman:
			instance->level_axeman = instance->level_axeman > 9 ? 9 : instance->level_axeman + 1;
			break;
		case Gunner:
			instance->level_gunner = instance->level_gunner > 9 ? 9 : instance->level_gunner + 1;
			break;
		}

		static const ResourcesManager::SoundPool& sound_pool = ResourcesManager::GetInstance()->get_sound_pool();

		Mix_PlayChannel(-1, sound_pool.find(ResID:: Sound_TowerLevelUp)->second, 0);
	}

	bool sell_tower(const SDL_Point& idx)
	{
		for (auto it = tower_list.begin(); it != tower_list.end(); ++it)
		{
			Tower* tower = *it;
			const SDL_Point& tile = tower->get_idx_tile();
			if (tile.x != idx.x || tile.y != idx.y)
				continue;

			double refund = get_place_cost(tower->get_tower_type()) * ConfigManager::GetInstance()->tower_sell_ratio;
			CoinManager::GetInstance()->increase_coin(refund);
			ConfigManager::GetInstance()->map.remove_tower(idx);
			delete tower;
			tower_list.erase(it);

			static const ResourcesManager::SoundPool& sound_pool = ResourcesManager::GetInstance()->get_sound_pool();
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_Coin)->second, 0);
			return true;
		}

		return false;
	}

protected:
	TowerManager() = default;
	~TowerManager() = default;

private:
	std::vector<Tower*> tower_list;

};


#endif// !_TOWER_MANAGER_H_

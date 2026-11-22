#ifndef _HOME_MANAGER_H_
#define _HOME_MANAGER_H_

#include"manager.h"
#include"config_manager.h"
#include"resources_manager.h"

class HomeManager : public Manager<HomeManager>
{
	friend class Manager<HomeManager>;

public:
	double get_current_hp_num()
	{
		return num_hp;
	}

	void increase_hp(double val)
	{
		num_hp += val;

		double max_hp = ConfigManager::GetInstance()->num_initial_hp;
		if (num_hp > max_hp)
			num_hp = max_hp;
	}

	void decrease_hp(double val)
	{
		num_hp -= val;

		if(num_hp < 0 )
			num_hp = 0;

		if (num_hp <= 0) {
			ConfigManager::GetInstance()->is_game_win = false;
			ConfigManager::GetInstance()->is_game_over = true;
		}

		static const ResourcesManager:: SoundPool& sound_pool
			= ResourcesManager:: GetInstance()->get_sound_pool();

		Mix_PlayChannel(-1, sound_pool.find(ResID:: Sound_HomeHurt)->second, 0);
	}

protected:
	HomeManager()
	{
		num_hp = ConfigManager:: GetInstance()->num_initial_hp;
	}

	~HomeManager() = default;

private:
	double num_hp = 0;

};

#endif// !_HOME_MANAGER_H_
#ifndef _RUSH_SLIME_ENEMY_H_
#define _RUSH_SLIME_ENEMY_H_

#include"enemy.h"
#include"config_manager.h"
#include"resources_manager.h"

class RushSlimeEnemy : public Enemy
{
public:
	RushSlimeEnemy()
	{
		static const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager:: GetInstance()->get_texture_pool();
		static SDL_Texture* tex_rush_slime = texture_pool.find(ResID::Tex_RushSlime)->second;
		static SDL_Texture* tex_rush_slime_sketch = texture_pool.find(ResID::Tex_RushSlimeSketch)->second;
		static ConfigManager:: EnemyTemplate& rush_slime_template = ConfigManager:: GetInstance()->rush_slime_template;

		static const std::vector<int> idx_list_up = { 6, 7, 8, 9, 10, 11 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4, 5 };
		static const std::vector<int> idx_list_left = { 18, 19, 20, 21, 22, 23 };
		static const std::vector<int> idx_list_right = { 12, 13, 14, 15, 16, 17 };

		anim_up.set_loop(true); anim_up.set_interval(0.06);
		anim_up.set_frame_data(tex_rush_slime, 6, 4, idx_list_up);
		anim_down.set_loop(true); anim_down.set_interval(0.06);
		anim_down.set_frame_data(tex_rush_slime, 6, 4, idx_list_down);
		anim_left.set_loop(true); anim_left.set_interval(0.06);
		anim_left.set_frame_data(tex_rush_slime, 6, 4, idx_list_left);
		anim_right.set_loop(true); anim_right.set_interval(0.06);
		anim_right.set_frame_data(tex_rush_slime, 6, 4, idx_list_right);

		anim_up_sketch.set_loop(true); anim_up_sketch.set_interval(0.06);
		anim_up_sketch.set_frame_data(tex_rush_slime_sketch, 6, 4, idx_list_up);
		anim_down_sketch.set_loop(true); anim_down_sketch.set_interval(0.06);
		anim_down_sketch.set_frame_data(tex_rush_slime_sketch, 6, 4, idx_list_down);
		anim_left_sketch.set_loop(true); anim_left_sketch.set_interval(0.06);
		anim_left_sketch.set_frame_data(tex_rush_slime_sketch, 6, 4, idx_list_left);
		anim_right_sketch.set_loop(true); anim_right_sketch.set_interval(0.06);
		anim_right_sketch.set_frame_data(tex_rush_slime_sketch, 6, 4, idx_list_right);

		max_hp = rush_slime_template.hp;
		max_speed = rush_slime_template.speed;
		damage = rush_slime_template.damage;
		reward_ratio = rush_slime_template.reward_ratio;
		recover_interval = rush_slime_template.recover_interval;
		recover_range = rush_slime_template.recover_range;
		recover_intensity = rush_slime_template.recover_intensity;

		color_hp_fill = { 255, 168, 72, 255 };
		color_hp_border = { 168, 64, 28, 255 };

		size.x = 48, size.y = 48;
		hp = max_hp, speed = max_speed;
	}

	~RushSlimeEnemy() = default;

private:

};

#endif // !_RUSH_SLIME_ENEMY_H_

#ifndef _DARK_PRIEST_ENEMY_H_
#define _DARK_PRIEST_ENEMY_H_

#include "enemy.h"
#include "config_manager.h"
#include "resources_manager.h"

class DarkPriestEnemy : public Enemy
{
public:
	DarkPriestEnemy()
	{
		static const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager:: GetInstance()->get_texture_pool();
		static SDL_Texture* tex_priest = texture_pool.find(ResID::Tex_GoblinPriest)->second;
		static SDL_Texture* tex_priest_sketch = texture_pool.find(ResID::Tex_GoblinPriestSketch)->second;
		static ConfigManager::EnemyTemplate& dark_priest_template = ConfigManager:: GetInstance()->dark_priest_template;

		static const std::vector<int> idx_list_up = { 5, 6, 7, 8, 9 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4 };
		static const std::vector<int> idx_list_left = { 15, 16, 17, 18, 19 };
		static const std::vector<int> idx_list_right = { 10, 11, 12, 13, 14 };
		static const Uint8 tint_r = 196;
		static const Uint8 tint_g = 72;
		static const Uint8 tint_b = 214;

		anim_up.set_loop(true); anim_up.set_interval(0.12);
		anim_up.set_frame_data(tex_priest, 5, 4, idx_list_up);
		anim_up.set_tint(tint_r, tint_g, tint_b);
		anim_down.set_loop(true); anim_down.set_interval(0.12);
		anim_down.set_frame_data(tex_priest, 5, 4, idx_list_down);
		anim_down.set_tint(tint_r, tint_g, tint_b);
		anim_left.set_loop(true); anim_left.set_interval(0.12);
		anim_left.set_frame_data(tex_priest, 5, 4, idx_list_left);
		anim_left.set_tint(tint_r, tint_g, tint_b);
		anim_right.set_loop(true); anim_right.set_interval(0.12);
		anim_right.set_frame_data(tex_priest, 5, 4, idx_list_right);
		anim_right.set_tint(tint_r, tint_g, tint_b);

		anim_up_sketch.set_loop(true); anim_up_sketch.set_interval(0.12);
		anim_up_sketch.set_frame_data(tex_priest_sketch, 5, 4, idx_list_up);
		anim_up_sketch.set_tint(tint_r, tint_g, tint_b);
		anim_down_sketch.set_loop(true); anim_down_sketch.set_interval(0.12);
		anim_down_sketch.set_frame_data(tex_priest_sketch, 5, 4, idx_list_down);
		anim_down_sketch.set_tint(tint_r, tint_g, tint_b);
		anim_left_sketch.set_loop(true); anim_left_sketch.set_interval(0.12);
		anim_left_sketch.set_frame_data(tex_priest_sketch, 5, 4, idx_list_left);
		anim_left_sketch.set_tint(tint_r, tint_g, tint_b);
		anim_right_sketch.set_loop(true); anim_right_sketch.set_interval(0.12);
		anim_right_sketch.set_frame_data(tex_priest_sketch, 5, 4, idx_list_right);
		anim_right_sketch.set_tint(tint_r, tint_g, tint_b);

		max_hp = dark_priest_template.hp;
		max_speed = dark_priest_template.speed;
		damage = dark_priest_template.damage;
		reward_ratio = dark_priest_template.reward_ratio;
		recover_interval = dark_priest_template.recover_interval;
		recover_range = dark_priest_template.recover_range;
		recover_intensity = dark_priest_template.recover_intensity;
		armor = dark_priest_template.armor;

		color_hp_fill = { 214, 120, 196, 255 };
		color_hp_border = { 110, 36, 120, 255 };

		size.x = 48, size.y = 48;
		hp = max_hp, speed = max_speed;

		timer_skill.set_wait_time(recover_interval);
	}

	~DarkPriestEnemy() = default;

};

#endif // !_DARK_PRIEST_ENEMY_H_

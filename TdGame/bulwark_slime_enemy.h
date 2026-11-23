#ifndef _BULWARK_SLIME_ENEMY_H_
#define _BULWARK_SLIME_ENEMY_H_

#include "enemy.h"
#include "config_manager.h"
#include "resources_manager.h"

class BulwarkSlimeEnemy : public Enemy
{
public:
	BulwarkSlimeEnemy()
	{
		static const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager:: GetInstance()->get_texture_pool();
		static SDL_Texture* tex_bulwark_slime = texture_pool.find(ResID::Tex_BulwarkSlime)->second;
		static SDL_Texture* tex_bulwark_slime_sketch = texture_pool.find(ResID::Tex_BulwarkSlimeSketch)->second;
		static ConfigManager::EnemyTemplate& bulwark_slime_template = ConfigManager:: GetInstance()->bulwark_slime_template;

		static const std::vector<int> idx_list_up = { 18, 19, 20, 21, 22, 23 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4, 5 };
		static const std::vector<int> idx_list_left = { 6, 7, 8, 9, 10, 11 };
		static const std::vector<int> idx_list_right = { 12, 13, 14, 15, 16, 17 };

		anim_up.set_loop(true); anim_up.set_interval(0.16);
		anim_up.set_frame_data(tex_bulwark_slime, 6, 4, idx_list_up);
		anim_down.set_loop(true); anim_down.set_interval(0.16);
		anim_down.set_frame_data(tex_bulwark_slime, 6, 4, idx_list_down);
		anim_left.set_loop(true); anim_left.set_interval(0.16);
		anim_left.set_frame_data(tex_bulwark_slime, 6, 4, idx_list_left);
		anim_right.set_loop(true); anim_right.set_interval(0.16);
		anim_right.set_frame_data(tex_bulwark_slime, 6, 4, idx_list_right);

		anim_up_sketch.set_loop(true); anim_up_sketch.set_interval(0.16);
		anim_up_sketch.set_frame_data(tex_bulwark_slime_sketch, 6, 4, idx_list_up);
		anim_down_sketch.set_loop(true); anim_down_sketch.set_interval(0.16);
		anim_down_sketch.set_frame_data(tex_bulwark_slime_sketch, 6, 4, idx_list_down);
		anim_left_sketch.set_loop(true); anim_left_sketch.set_interval(0.16);
		anim_left_sketch.set_frame_data(tex_bulwark_slime_sketch, 6, 4, idx_list_left);
		anim_right_sketch.set_loop(true); anim_right_sketch.set_interval(0.16);
		anim_right_sketch.set_frame_data(tex_bulwark_slime_sketch, 6, 4, idx_list_right);

		max_hp = bulwark_slime_template.hp;
		max_speed = bulwark_slime_template.speed;
		damage = bulwark_slime_template.damage;
		reward_ratio = bulwark_slime_template.reward_ratio;
		recover_interval = bulwark_slime_template.recover_interval;
		recover_range = bulwark_slime_template.recover_range;
		recover_intensity = bulwark_slime_template.recover_intensity;
		armor = bulwark_slime_template.armor;

		color_hp_fill = { 214, 96, 84, 255 };
		color_hp_border = { 120, 36, 32, 255 };

		size.x = 64, size.y = 64;
		hp = max_hp, speed = max_speed;
	}

	~BulwarkSlimeEnemy() = default;

};

#endif // !_BULWARK_SLIME_ENEMY_H_

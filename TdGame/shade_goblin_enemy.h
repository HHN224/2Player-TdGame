#ifndef _SHADE_GOBLIN_ENEMY_H_
#define _SHADE_GOBLIN_ENEMY_H_

#include "enemy.h"
#include "config_manager.h"
#include "resources_manager.h"

class ShadeGoblinEnemy : public Enemy
{
public:
	ShadeGoblinEnemy()
	{
		static const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager:: GetInstance()->get_texture_pool();
		static SDL_Texture* tex_goblin = texture_pool.find(ResID::Tex_Goblin)->second;
		static SDL_Texture* tex_goblin_sketch = texture_pool.find(ResID::Tex_GoblinSketch)->second;
		static ConfigManager::EnemyTemplate& shade_goblin_template = ConfigManager:: GetInstance()->shade_goblin_template;

		static const std::vector<int> idx_list_up = { 5, 6, 7, 8, 9 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4 };
		static const std::vector<int> idx_list_left = { 15, 16, 17, 18, 19 };
		static const std::vector<int> idx_list_right = { 10, 11, 12, 13, 14 };
		static const Uint8 tint_r = 96;
		static const Uint8 tint_g = 52;
		static const Uint8 tint_b = 148;

		anim_up.set_loop(true); anim_up.set_interval(0.08);
		anim_up.set_frame_data(tex_goblin, 5, 4, idx_list_up);
		anim_up.set_tint(tint_r, tint_g, tint_b);
		anim_down.set_loop(true); anim_down.set_interval(0.08);
		anim_down.set_frame_data(tex_goblin, 5, 4, idx_list_down);
		anim_down.set_tint(tint_r, tint_g, tint_b);
		anim_left.set_loop(true); anim_left.set_interval(0.08);
		anim_left.set_frame_data(tex_goblin, 5, 4, idx_list_left);
		anim_left.set_tint(tint_r, tint_g, tint_b);
		anim_right.set_loop(true); anim_right.set_interval(0.08);
		anim_right.set_frame_data(tex_goblin, 5, 4, idx_list_right);
		anim_right.set_tint(tint_r, tint_g, tint_b);

		anim_up_sketch.set_loop(true); anim_up_sketch.set_interval(0.08);
		anim_up_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_up);
		anim_up_sketch.set_tint(tint_r, tint_g, tint_b);
		anim_down_sketch.set_loop(true); anim_down_sketch.set_interval(0.08);
		anim_down_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_down);
		anim_down_sketch.set_tint(tint_r, tint_g, tint_b);
		anim_left_sketch.set_loop(true); anim_left_sketch.set_interval(0.08);
		anim_left_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_left);
		anim_left_sketch.set_tint(tint_r, tint_g, tint_b);
		anim_right_sketch.set_loop(true); anim_right_sketch.set_interval(0.08);
		anim_right_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_right);
		anim_right_sketch.set_tint(tint_r, tint_g, tint_b);

		max_hp = shade_goblin_template.hp;
		max_speed = shade_goblin_template.speed;
		damage = shade_goblin_template.damage;
		reward_ratio = shade_goblin_template.reward_ratio;
		recover_interval = shade_goblin_template.recover_interval;
		recover_range = shade_goblin_template.recover_range;
		recover_intensity = shade_goblin_template.recover_intensity;
		armor = shade_goblin_template.armor;

		color_hp_fill = { 176, 112, 214, 255 };
		color_hp_border = { 72, 36, 112, 255 };

		size.x = 48, size.y = 48;
		hp = max_hp, speed = max_speed;
	}

	~ShadeGoblinEnemy() = default;

};

#endif // !_SHADE_GOBLIN_ENEMY_H_

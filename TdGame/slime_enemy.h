#ifndef _SLIME_ENEMY_H_
#define	_SLIME_ENEMY_H_

#include"enemy.h"
#include"config_manager.h"
#include"resources_manager.h"

class SlimeEnemy : public Enemy
{
public:
	SlimeEnemy()
	{
		static const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager:: GetInstance()->get_texture_pool();
		static SDL_Texture* tex_slime = texture_pool.find(ResID:: Tex_Slime)->second;
		static SDL_Texture* tex_slime_sketch = texture_pool.find(ResID:: Tex_SlimeSketch)->second;
		static ConfigManager:: EnemyTemplate& slime_template = ConfigManager:: GetInstance()->slime_template;

		static const std::vector<int> idx_list_up = { 6, 7, 8, 9, 10, 11 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4, 5 };
		static const std::vector<int> idx_list_left = { 18, 19, 20, 21, 22, 23 };
		static const std::vector<int> idx_list_right = { 12, 13, 14, 15, 16, 17 };

		anim_up.set_loop(true); anim_up.set_interval(0.1);
		anim_up.set_frame_data(tex_slime, 6, 4, idx_list_up);
		anim_down.set_loop(true); anim_down.set_interval(0.1);
		anim_down.set_frame_data(tex_slime, 6, 4, idx_list_down);
		anim_left.set_loop(true); anim_left.set_interval(0.1);
		anim_left.set_frame_data(tex_slime, 6, 4, idx_list_left);
		anim_right.set_loop(true); anim_right.set_interval(0.1);
		anim_right.set_frame_data(tex_slime, 6, 4, idx_list_right);

		anim_up_sketch.set_loop(true); anim_up_sketch.set_interval(0.1);
		anim_up_sketch.set_frame_data(tex_slime_sketch, 6, 4, idx_list_up);
		anim_down_sketch.set_loop(true); anim_down_sketch.set_interval(0.1);
		anim_down_sketch.set_frame_data(tex_slime_sketch, 6, 4, idx_list_down);
		anim_left_sketch.set_loop(true); anim_left_sketch.set_interval(0.1);
		anim_left_sketch.set_frame_data(tex_slime_sketch, 6, 4, idx_list_left);
		anim_right_sketch.set_loop(true); anim_right_sketch.set_interval(0.1);
		anim_right_sketch.set_frame_data(tex_slime_sketch, 6, 4, idx_list_right);

		max_hp = slime_template.hp;
		max_speed = slime_template.speed;
		damage = slime_template.damage;
		reward_ratio = slime_template.reward_ratio;
		recover_interval = slime_template.recover_interval;
		recover_range = slime_template.recover_range;
		recover_intensity = slime_template.recover_intensity;

		size.x = 48, size.y = 48;
		hp = max_hp, speed = max_speed;
	}

	~SlimeEnemy() = default;

private:

};

#endif // !_SLIME_ENEMY_H_

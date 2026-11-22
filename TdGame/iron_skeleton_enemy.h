#ifndef _IRON_SKELETON_ENEMY_H_
#define _IRON_SKELETON_ENEMY_H_

#include "enemy.h"
#include "config_manager.h"
#include "resources_manager.h"

class IronSkeletonEnemy : public Enemy
{
public:
	IronSkeletonEnemy()
	{
		static const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager:: GetInstance()->get_texture_pool();
		static SDL_Texture* tex_skeleton = texture_pool.find(ResID::Tex_Skeleton)->second;
		static SDL_Texture* tex_skeleton_sketch = texture_pool.find(ResID::Tex_SkeletonSketch)->second;
		static ConfigManager::EnemyTemplate& iron_skeleton_template = ConfigManager:: GetInstance()->iron_skeleton_template;

		static const std::vector<int> idx_list_up = { 5, 6, 7, 8, 9 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4 };
		static const std::vector<int> idx_list_left = { 15, 16, 17, 18, 19 };
		static const std::vector<int> idx_list_right = { 10, 11, 12, 13, 14 };
		static const Uint8 tint_r = 150;
		static const Uint8 tint_g = 190;
		static const Uint8 tint_b = 230;

		anim_up.set_loop(true); anim_up.set_interval(0.18);
		anim_up.set_frame_data(tex_skeleton, 5, 4, idx_list_up);
		anim_up.set_tint(tint_r, tint_g, tint_b);
		anim_down.set_loop(true); anim_down.set_interval(0.18);
		anim_down.set_frame_data(tex_skeleton, 5, 4, idx_list_down);
		anim_down.set_tint(tint_r, tint_g, tint_b);
		anim_left.set_loop(true); anim_left.set_interval(0.18);
		anim_left.set_frame_data(tex_skeleton, 5, 4, idx_list_left);
		anim_left.set_tint(tint_r, tint_g, tint_b);
		anim_right.set_loop(true); anim_right.set_interval(0.18);
		anim_right.set_frame_data(tex_skeleton, 5, 4, idx_list_right);
		anim_right.set_tint(tint_r, tint_g, tint_b);

		anim_up_sketch.set_loop(true); anim_up_sketch.set_interval(0.18);
		anim_up_sketch.set_frame_data(tex_skeleton_sketch, 5, 4, idx_list_up);
		anim_up_sketch.set_tint(tint_r, tint_g, tint_b);
		anim_down_sketch.set_loop(true); anim_down_sketch.set_interval(0.18);
		anim_down_sketch.set_frame_data(tex_skeleton_sketch, 5, 4, idx_list_down);
		anim_down_sketch.set_tint(tint_r, tint_g, tint_b);
		anim_left_sketch.set_loop(true); anim_left_sketch.set_interval(0.18);
		anim_left_sketch.set_frame_data(tex_skeleton_sketch, 5, 4, idx_list_left);
		anim_left_sketch.set_tint(tint_r, tint_g, tint_b);
		anim_right_sketch.set_loop(true); anim_right_sketch.set_interval(0.18);
		anim_right_sketch.set_frame_data(tex_skeleton_sketch, 5, 4, idx_list_right);
		anim_right_sketch.set_tint(tint_r, tint_g, tint_b);

		max_hp = iron_skeleton_template.hp;
		max_speed = iron_skeleton_template.speed;
		damage = iron_skeleton_template.damage;
		reward_ratio = iron_skeleton_template.reward_ratio;
		recover_interval = iron_skeleton_template.recover_interval;
		recover_range = iron_skeleton_template.recover_range;
		recover_intensity = iron_skeleton_template.recover_intensity;
		armor = iron_skeleton_template.armor;

		color_hp_fill = { 150, 196, 230, 255 };
		color_hp_border = { 48, 78, 112, 255 };

		size.x = 48, size.y = 48;
		hp = max_hp, speed = max_speed;
	}

	~IronSkeletonEnemy() = default;

};

#endif // !_IRON_SKELETON_ENEMY_H_

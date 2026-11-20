#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include"manager.h"
#include"config_manager.h"	
#include"enemy_manager.h"
#include"wave_manager.h"
#include"resources_manager.h"
#include"tower_manager.h"
#include"bullet_manager.h"
#include"status_bar.h"
#include"panel.h"
#include"place_panel.h"
#include"upgrade_panle.h"
#include"player_manager.h"
#include"banner.h"

#include<iostream>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<SDL_image.h>

class GameManager : public Manager <GameManager>
{
	friend class Manager<GameManager>;

public:
	int run(int argc, char** argv)
	{
		Mix_FadeInMusic(ResourcesManager::GetInstance()->get_music_pool().find(ResID:: Music_BGM)->second, -1, 1500);

		Uint64 last_time = SDL_GetPerformanceCounter();
		const Uint64 count_freq = SDL_GetPerformanceFrequency();

		while (!is_quit)
		{	
			while (SDL_PollEvent(&event)) 
			on_input();

			Uint64 current_time = SDL_GetPerformanceCounter();
			double delta = (double)(current_time - last_time)/count_freq;
			if (delta*1000 < 1000.0/60)
				SDL_Delay((Uint32)(1000.0/60 - delta *1000));
			last_time = current_time;
			
			on_update(delta);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear (renderer);

			on_render();

			SDL_RenderPresent (renderer);

		}

		return 0;
	}

protected:
	GameManager()
	{
		init_assert(!SDL_Init(SDL_INIT_EVERYTHING), "SDL init failed!");
		init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), "SDL_image init failed!");
		init_assert(Mix_Init(MIX_INIT_MP3), "SDL_mixer init failed!");
		init_assert(!TTF_Init(), "SDL_ttf init failed!");

		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		SDL_SetHint(SDL_HINT_IME_SHOW_UI,"1");

		ConfigManager* config = ConfigManager::GetInstance();

		init_assert(config->map.load("map.csv"), "map load failed!");
		init_assert(config->load_level_config("level.json"), "load level config failed!");
		init_assert(config->load_game_config("config.json"), "load game config failed!");

		window = SDL_CreateWindow(config->basic_template.window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		config->basic_template.window_width, config->basic_template.window_height, SDL_WINDOW_SHOWN);
		init_assert(window, "window create failed!");

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
		init_assert(renderer, "renderer create failed!");

		init_assert(ResourcesManager::GetInstance()->load_from_file(renderer), "load game rescource failed!");

		init_assert(generate_tile_map_texture(), "generate tile map texture failed!");

		status_bar.set_position(15, 15);

		banner = new Banner();
		place_panel = new PlacePanel();
		upgrade_panel = new UpgradePanel();
	};
	~GameManager()
	{
		SDL_DestroyTexture(tex_pause_title);
		SDL_DestroyTexture(tex_pause_hint);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		TTF_Quit();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
	}; 

private:
	void init_assert(bool flag, const char* err_msg) 
	{
		if(flag) 
			return;
		
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "failed to run game", err_msg, window);
	}

	void on_input()
	{
		static SDL_Point pos_center;
		static SDL_Point idx_tile_selected;
		static ConfigManager* instance = ConfigManager::GetInstance();

		switch (event.type)
		{
		case SDL_QUIT:
			is_quit = true;
			break;
		case SDL_KEYDOWN:
			if (instance->is_game_over || event.key.repeat)
				break;
			switch (event.key.keysym.sym)
			{
			case SDLK_SPACE:
				instance->is_paused = !instance->is_paused;
				break;
			case SDLK_1:
			case SDLK_KP_1:
				instance->time_scale = 1;
				break;
			case SDLK_2:
			case SDLK_KP_2:
				instance->time_scale = 2;
				break;
			case SDLK_3:
			case SDLK_KP_3:
				instance->time_scale = 3;
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(instance->is_game_over)
				break;
			if (get_cursor_idx_tile(idx_tile_selected, event.motion.x, event.motion.y))
			{
				get_selected_tile_center_pos(pos_center, idx_tile_selected);

				if (check_home(idx_tile_selected))
				{
					upgrade_panel->set_idx_tile(idx_tile_selected);
					upgrade_panel->set_center_pos(pos_center);
					upgrade_panel->show();
				}
				else if (can_place_tower(idx_tile_selected))
				{
					place_panel->set_idx_tile(idx_tile_selected);
					place_panel->set_center_pos(pos_center);
					place_panel->show();
				}
			}
			break;
		default:
			break;
		}

		if (!instance->is_game_over)
		{
			place_panel->on_input(event);
			upgrade_panel->on_input(event);
			PlayerManager::GetInstance()->on_input(event);
		}
	}

	void on_update(double delta)
	{
		static bool is_game_over_last_tick = false;
		static ConfigManager* instance = ConfigManager:: GetInstance();

		if (!instance->is_game_over)
		{
			status_bar.on_update(renderer);
			place_panel->on_update(renderer);
			upgrade_panel->on_update(renderer);

			const double sim_delta = instance->is_paused ? 0.0 : delta * instance->time_scale;
			WaveManager::GetInstance()->on_update(sim_delta);
			EnemyManager::GetInstance()->on_update(sim_delta);
			BulletManager::GetInstance()->on_update(sim_delta);
			TowerManager::GetInstance()->on_update(sim_delta);
			CoinManager::GetInstance()->on_update(sim_delta);
			PlayerManager::GetInstance()->on_update(sim_delta);

			return;
		}

		if (!is_game_over_last_tick && instance->is_game_over)
		{
			static const ResourcesManager::SoundPool& sound_pool = ResourcesManager::GetInstance()->get_sound_pool();

			Mix_FadeOutMusic(1500);
			Mix_PlayChannel(-1, instance->is_game_win ? 
				sound_pool.find(ResID::Sound_Win)->second : sound_pool.find(ResID::Sound_Loss)->second, 0);
		}

		is_game_over_last_tick = instance->is_game_over;

		banner->on_update(delta);
		if (banner->check_end_display())
			is_quit = true;
	}

	void on_render()
	{
		static ConfigManager* instance = ConfigManager:: GetInstance();
		static SDL_Rect& rect_dst = instance->rect_tile_map;
		SDL_RenderCopy(renderer, tex_tile_map, nullptr,  &rect_dst);

		EnemyManager:: GetInstance()->on_render(renderer);
		BulletManager::GetInstance()->on_render(renderer);
		CoinManager::GetInstance()->on_render(renderer);
		TowerManager::GetInstance()->on_render(renderer);
		PlayerManager::GetInstance()->on_render(renderer);

		if (!instance->is_game_over)
		{
			if (instance->is_paused)
				render_pause_dim();

			place_panel->on_render(renderer);
			upgrade_panel->on_render(renderer);
			status_bar.on_render(renderer);

			if (instance->is_paused)
				render_pause_label();

			return;
		}

		int width_screen, height_screen;
		SDL_GetWindowSizeInPixels(window, &width_screen, &height_screen);
		banner->set_center_position( { (double)width_screen / 2, (double)height_screen / 2});
		banner->on_render(renderer);
	}

	bool generate_tile_map_texture()
	{
		const Map& map = ConfigManager:: GetInstance()->map;
		const TileMap& tile_map = map.get_tile_map();
		SDL_Rect& rect_tile_map = ConfigManager:: GetInstance()->rect_tile_map;
		SDL_Texture* tex_tile_set = ResourcesManager:: GetInstance()->get_texture_pool().find(ResID:: Tex_Tileset)->second;
	
		int width_tex_tile_set, height_tex_tile_set;
		SDL_QueryTexture(tex_tile_set, nullptr, nullptr, &width_tex_tile_set, &height_tex_tile_set);
		int num_tile_single_line = (int)std::ceil((double)width_tex_tile_set/SIZE_TILE);

		int width_tex_tile_map, height_tex_tile_map;
		width_tex_tile_map = (int)map.get_width() * SIZE_TILE;
		height_tex_tile_map = (int)map.get_height() * SIZE_TILE;
		tex_tile_map = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, 
			SDL_TEXTUREACCESS_TARGET, width_tex_tile_map, height_tex_tile_map);
		if(!tex_tile_map) 
			return false;

		ConfigManager* config = ConfigManager:: GetInstance();
		rect_tile_map.x = (config->basic_template.window_width - width_tex_tile_map) / 2;
		rect_tile_map.y = (config->basic_template.window_height - height_tex_tile_map) / 2;
		rect_tile_map.w = width_tex_tile_map;
		rect_tile_map.h = height_tex_tile_map;

		SDL_SetTextureBlendMode(tex_tile_map, SDL_BLENDMODE_BLEND);
		SDL_SetRenderTarget(renderer, tex_tile_map);

		for (int y = 0 ; y < map.get_height() ; y++ )
		{
			for (int x = 0 ; x < map.get_width() ; x++)
			{
				SDL_Rect rect_src;
				const Tile& tile = tile_map[y][x];

				const SDL_Rect& rect_dst =
				{
					x * SIZE_TILE, y * SIZE_TILE,
					SIZE_TILE, SIZE_TILE
				};

				rect_src =
				{
					(tile.terrain % num_tile_single_line) * SIZE_TILE,
					(tile.terrain / num_tile_single_line) * SIZE_TILE,
					SIZE_TILE, SIZE_TILE
				};
				SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst);

				if (tile.decoration >= 0)
				{
					rect_src =
					{
						(tile.decoration % num_tile_single_line) * SIZE_TILE,
						(tile.decoration / num_tile_single_line) * SIZE_TILE,
						SIZE_TILE, SIZE_TILE
					};
					SDL_RenderCopy(renderer, tex_tile_set, &rect_src, &rect_dst);
				}
			}
		}
		
		const SDL_Point& idx_home = map.get_idx_home();
		const SDL_Rect rect_dst = 
		{
			idx_home.x * SIZE_TILE, idx_home.y * SIZE_TILE,
			SIZE_TILE, SIZE_TILE
		};

		SDL_RenderCopy(renderer, ResourcesManager:: GetInstance()->get_texture_pool().find(ResID:: Tex_Home)->second, nullptr, &rect_dst);
		
		SDL_SetRenderTarget(renderer, nullptr);

		return true;
	}

	bool check_home(const SDL_Point& idx_tile_selected)
	{
		static const Map& map = ConfigManager::GetInstance()->map;
		static const SDL_Point& idx_home = map.get_idx_home();

		return (idx_tile_selected.x == idx_home.x && idx_tile_selected.y == idx_home.y);
	}

	bool get_cursor_idx_tile(SDL_Point& idx_tile_selected, int screen_x, int screen_y) const
	{
		static const Map& map = ConfigManager::GetInstance()->map;
		static const SDL_Rect& rect_tile_map = ConfigManager::GetInstance()->rect_tile_map;

		if (screen_x < rect_tile_map.x || screen_x > rect_tile_map.x + rect_tile_map.w
			|| screen_y < rect_tile_map.y || screen_y > rect_tile_map.y + rect_tile_map.h)
			return false;

		idx_tile_selected.x = std::min((screen_x - rect_tile_map.x) / SIZE_TILE, (int)map.get_width() - 1);
		idx_tile_selected.y = std::min((screen_y - rect_tile_map.y) / SIZE_TILE, (int)map.get_height() - 1);

		return true;
	}

	bool can_place_tower(const SDL_Point& idx_tile_selected) const
	{
		static const Map& map = ConfigManager::GetInstance()->map;
		const Tile& tile = map.get_tile_map()[idx_tile_selected.y][idx_tile_selected.x];

		return (tile.decoration < 0 && tile.direction == Tile:: Direction:: None && !tile.has_tower);
	}

	void render_pause_dim()
	{
		int width_screen = 0, height_screen = 0;
		SDL_GetWindowSizeInPixels(window, &width_screen, &height_screen);

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 12, 10, 16, 140);
		const SDL_Rect shade = { 0, 0, width_screen, height_screen };
		SDL_RenderFillRect(renderer, &shade);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	}

	void render_pause_label()
	{
		ensure_pause_text();
		if (!tex_pause_title || !tex_pause_hint)
			return;

		int width_screen = 0, height_screen = 0;
		SDL_GetWindowSizeInPixels(window, &width_screen, &height_screen);

		SDL_Rect box;
		box.w = pause_hint_w + 48;
		box.h = pause_title_h + pause_hint_h + 36;
		box.x = (width_screen - box.w) / 2;
		box.y = (height_screen - box.h) / 2;

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 36, 24, 28, 230);
		SDL_RenderFillRect(renderer, &box);
		SDL_SetRenderDrawColor(renderer, 255, 196, 120, 255);
		SDL_RenderDrawRect(renderer, &box);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

		SDL_Rect rect_dst;
		rect_dst.w = pause_title_w;
		rect_dst.h = pause_title_h;
		rect_dst.x = (width_screen - pause_title_w) / 2;
		rect_dst.y = box.y + 14;
		SDL_RenderCopy(renderer, tex_pause_title, nullptr, &rect_dst);

		rect_dst.w = pause_hint_w;
		rect_dst.h = pause_hint_h;
		rect_dst.x = (width_screen - pause_hint_w) / 2;
		rect_dst.y = rect_dst.y + pause_title_h + 8;
		SDL_RenderCopy(renderer, tex_pause_hint, nullptr, &rect_dst);
	}

	void ensure_pause_text()
	{
		if (pause_label_ready)
			return;

		TTF_Font* font = ResourcesManager::GetInstance()->get_font_pool().find(ResID::Font_Main)->second;
		const SDL_Color title_color = { 255, 228, 196, 255 };
		const SDL_Color hint_color = { 236, 220, 196, 255 };
		SDL_Surface* title = TTF_RenderUTF8_Blended(font, "暂停", title_color);
		SDL_Surface* hint = TTF_RenderUTF8_Blended(font, "空格继续     1 / 2 / 3 倍速", hint_color);
		if (title)
		{
			pause_title_w = title->w;
			pause_title_h = title->h;
			tex_pause_title = SDL_CreateTextureFromSurface(renderer, title);
			SDL_FreeSurface(title);
		}
		if (hint)
		{
			pause_hint_w = hint->w;
			pause_hint_h = hint->h;
			tex_pause_hint = SDL_CreateTextureFromSurface(renderer, hint);
			SDL_FreeSurface(hint);
		}

		pause_label_ready = tex_pause_title && tex_pause_hint;
	}

	void get_selected_tile_center_pos(SDL_Point& pos, const SDL_Point& idx_tile_selected) const
	{
		static const SDL_Rect& rect_tile_map = ConfigManager::GetInstance()->rect_tile_map;	

		pos.x = rect_tile_map.x + idx_tile_selected.x * SIZE_TILE + SIZE_TILE / 2;
		pos.y = rect_tile_map.y + idx_tile_selected.y * SIZE_TILE + SIZE_TILE / 2;
	}


private:
	SDL_Event event;
	bool is_quit = false;

	StatusBar status_bar;

	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Texture* tex_tile_map = nullptr;
	SDL_Texture* tex_pause_title = nullptr;
	SDL_Texture* tex_pause_hint = nullptr;
	int pause_title_w = 0;
	int pause_title_h = 0;
	int pause_hint_w = 0;
	int pause_hint_h = 0;
	bool pause_label_ready = false;

	Panel* place_panel = nullptr;
	Panel* upgrade_panel = nullptr;
	Banner* banner = nullptr;

};


#endif //!_GAME_MANAGER_H_
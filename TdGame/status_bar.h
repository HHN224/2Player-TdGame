#ifndef _STATUS_BAR_H_
#define _STATUS_BAR_H_

#include"resources_manager.h"
#include"coin_manager.h"
#include"home_manager.h"
#include"player_manager.h"
#include"wave_manager.h"
#include"config_manager.h"


#include <SDL2_gfxPrimitives.h>
#include<SDL.h>
#include<cmath>
#include<string>

class StatusBar
{
public:
	StatusBar() = default;
	~StatusBar() = default;

	void set_position(int x, int y)
	{
		position.x = x, position.y = y;
	}

	void on_update(SDL_Renderer* renderer)
	{
		static TTF_Font* font = ResourcesManager:: GetInstance()->get_font_pool().find(ResID:: Font_Main)->second;

		SDL_DestroyTexture(tex_text_background);
		tex_text_background = nullptr;
		SDL_DestroyTexture(tex_text_foreground);
		tex_text_foreground = nullptr;

		std::string str_val = std::to_string((int)CoinManager:: GetInstance()->get_current_coin_num());
		SDL_Surface* suf_text_background = TTF_RenderText_Blended(font, str_val.c_str(), color_text_background);
		SDL_Surface* suf_text_foreground = TTF_RenderText_Blended(font, str_val.c_str(), color_text_foreground);

		width_text = suf_text_background->w, height_text = suf_text_background->h;

		tex_text_background = SDL_CreateTextureFromSurface(renderer, suf_text_background);
		tex_text_foreground = SDL_CreateTextureFromSurface(renderer, suf_text_foreground);

		SDL_FreeSurface(suf_text_background);
		SDL_FreeSurface(suf_text_foreground);

		SDL_DestroyTexture(tex_wave_background);
		tex_wave_background = nullptr;
		SDL_DestroyTexture(tex_wave_foreground);
		tex_wave_foreground = nullptr;

		std::string wave_text = make_wave_text();
		SDL_Surface* suf_wave_background = TTF_RenderUTF8_Blended(font, wave_text.c_str(), color_wave_background);
		SDL_Surface* suf_wave_foreground = TTF_RenderUTF8_Blended(font, wave_text.c_str(), color_wave_foreground);
		if (suf_wave_background && suf_wave_foreground)
		{
			width_wave_text = suf_wave_background->w;
			height_wave_text = suf_wave_background->h;
			tex_wave_background = SDL_CreateTextureFromSurface(renderer, suf_wave_background);
			tex_wave_foreground = SDL_CreateTextureFromSurface(renderer, suf_wave_foreground);
		}
		if (suf_wave_background)
			SDL_FreeSurface(suf_wave_background);
		if (suf_wave_foreground)
			SDL_FreeSurface(suf_wave_foreground);
	}

	void on_render(SDL_Renderer* renderer)
	{
		static SDL_Rect rect_dst;
		static const ResourcesManager::TexturePool& tex_pool = ResourcesManager:: GetInstance()->get_texture_pool();
		static SDL_Texture* tex_coin = tex_pool.find(ResID:: Tex_UICoin)->second;
		static SDL_Texture* tex_heart = tex_pool.find(ResID:: Tex_UIHeart)->second;
		static SDL_Texture* tex_home_avatar = tex_pool.find(ResID:: Tex_UIHomeAvatar)->second;
		static SDL_Texture* tex_player_avatar = tex_pool.find(ResID:: Tex_UIPlayerAvatar)->second;

		rect_dst.x = position.x, rect_dst.y = position.y;
		rect_dst.w = 78, rect_dst.h = 78;
		SDL_RenderCopy(renderer, tex_home_avatar, nullptr, &rect_dst);

		for (int i = 1; i < (int)HomeManager:: GetInstance()->get_current_hp_num(); i++)
		{
			rect_dst.x = position.x + 78 + 15 + i * ( 32 + 2);
			rect_dst.y = position.y;
			rect_dst.w = 32, rect_dst.h = 32;
			SDL_RenderCopy(renderer, tex_heart, nullptr, &rect_dst);
		}

		rect_dst.x = position.x + 78 + 15;
		rect_dst.y = position.y + 78 - 32;
		rect_dst.w = 32, rect_dst.h = 32;
		SDL_RenderCopy(renderer, tex_coin, nullptr, &rect_dst);

		rect_dst.x += 32 + 10 + offset_shadow.x;
		rect_dst.y = rect_dst.y + (32 - height_text) / 2 + offset_shadow.y;
		rect_dst.w = width_text, rect_dst.h = height_text;
		SDL_RenderCopy(renderer, tex_text_background, nullptr, &rect_dst);

		rect_dst.x -= offset_shadow.x;
		rect_dst.y -= offset_shadow.y;
		SDL_RenderCopy(renderer, tex_text_foreground, nullptr, &rect_dst);

		if (tex_wave_background && tex_wave_foreground)
		{
			rect_dst.x = position.x + 470 + offset_shadow.x;
			rect_dst.y = position.y + 18 + offset_shadow.y;
			rect_dst.w = width_wave_text;
			rect_dst.h = height_wave_text;
			SDL_RenderCopy(renderer, tex_wave_background, nullptr, &rect_dst);

			rect_dst.x -= offset_shadow.x;
			rect_dst.y -= offset_shadow.y;
			SDL_RenderCopy(renderer, tex_wave_foreground, nullptr, &rect_dst);
		}

		rect_dst.x = position.x + (78 - 65) / 2;
		rect_dst.y = position.y + 78 + 5;
		rect_dst.w = 65, rect_dst.h = 65;
		SDL_RenderCopy(renderer, tex_player_avatar, nullptr, &rect_dst);

		rect_dst.x = position.x + 78 + 15;
		rect_dst.y += 10;
		roundedBoxRGBA(renderer, rect_dst.x, rect_dst.y, rect_dst.x + width_mp_bar, rect_dst.y + height_mp_bar, 4,
			color_mp_bar_background.r, color_mp_bar_background.g, color_mp_bar_background.b, color_mp_bar_background.a);

		rect_dst.x += width_border_mp_bar;
		rect_dst.y += width_border_mp_bar;
		rect_dst.w = width_mp_bar - 2 * width_border_mp_bar;
		rect_dst.h = height_mp_bar - 2 * width_border_mp_bar;
		double process = PlayerManager::GetInstance()->get_current_mp() / 100.0;
		roundedBoxRGBA(renderer, rect_dst.x, rect_dst.y, rect_dst.x + (int)(rect_dst.w * process), rect_dst.y + rect_dst.h, 2,
			color_mp_bar_foreground.r, color_mp_bar_foreground.g, color_mp_bar_foreground.b, color_mp_bar_foreground.a);
	}

private:
	const int size_heart = 32;
	const int width_mp_bar = 200;
	const int height_mp_bar = 20;
	const int width_border_mp_bar = 4;
	const SDL_Point offset_shadow = { 2, 2 };
	const SDL_Color color_text_background = { 175, 175, 175, 255 };
	const SDL_Color color_text_foreground = { 255, 255, 255, 255 };
	const SDL_Color color_wave_background = { 72, 42, 28, 255 };
	const SDL_Color color_wave_foreground = { 255, 214, 140, 255 };
	const SDL_Color color_mp_bar_background = { 48, 40, 51, 255 };
	const SDL_Color color_mp_bar_foreground = { 144, 121, 173, 255 };

private:
	SDL_Point position = { 0, 0 };
	int width_text = 0, height_text = 0;
	int width_wave_text = 0, height_wave_text = 0;
	SDL_Texture* tex_text_background = nullptr;
	SDL_Texture* tex_text_foreground = nullptr;
	SDL_Texture* tex_wave_background = nullptr;
	SDL_Texture* tex_wave_foreground = nullptr;

	std::string make_wave_text() const
	{
		const ConfigManager* config = ConfigManager::GetInstance();
		const WaveManager* waves = WaveManager::GetInstance();
		const int total = (int)config->wave_list.size();
		std::string text = "第 " + std::to_string(waves->get_wave_number()) + "/" + std::to_string(total) + " 波";

		if (config->is_paused)
			text += "   暂停";
		else if (waves->is_waiting_to_spawn())
		{
			int seconds = (int)std::ceil(waves->get_seconds_until_spawn());
			if (seconds < 1)
				seconds = 1;
			text += "   下波 " + std::to_string(seconds) + " 秒";
		}
		else
			text += "   x" + std::to_string((int)config->time_scale);

		return text;
	}

};

#endif // !_STATUS_BAR_H_

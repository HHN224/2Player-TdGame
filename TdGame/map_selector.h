#ifndef _MAP_SELECTOR_H_
#define _MAP_SELECTOR_H_

#include "resources_manager.h"

#include <SDL.h>
#include <string>

class MapSelector
{
public:
	bool pick(SDL_Renderer* renderer, SDL_Window* window, std::string& path)
	{
		struct Choice
		{
			const char* name;
			const char* detail;
			const char* file;
		};

		static const Choice choices[] =
		{
			{ "平原双径", "原来的两条弯路，在地图中部汇合", "map.csv" },
			{ "外环", "两条路绕着基地走一大圈，布防时间更长", "maps/ring.csv" },
			{ "窄谷", "两条很短的直道，敌人很快就会到家", "maps/valley.csv" },
			{ "蛇行", "左右各自来回折返，路口更多", "maps/snake.csv" },
		};
		const int count = (int)(sizeof(choices) / sizeof(choices[0]));
		int selected = 0;

		TTF_Font* font = ResourcesManager::GetInstance()->get_font_pool().find(ResID::Font_Main)->second;
		const SDL_Color title_color = { 255, 228, 196, 255 };
		const SDL_Color name_color = { 255, 244, 230, 255 };
		const SDL_Color detail_color = { 210, 196, 176, 255 };
		const SDL_Color hint_color = { 186, 170, 148, 255 };

		SDL_Texture* tex_title = make_text(renderer, font, "选择地图", title_color);
		SDL_Texture* tex_hint = make_text(renderer, font, "上下选择    回车开始    1-4 直接进入", hint_color);
		SDL_Texture* tex_name[4] = {};
		SDL_Texture* tex_detail[4] = {};
		for (int i = 0; i < count; i++)
		{
			tex_name[i] = make_text(renderer, font, choices[i].name, name_color);
			tex_detail[i] = make_text(renderer, font, choices[i].detail, detail_color);
		}

		bool finished = false;
		bool accepted = false;
		while (!finished)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					finished = true;
					accepted = false;
				}
				else if (event.type == SDL_KEYDOWN && !event.key.repeat)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_UP:
					case SDLK_w:
						selected = (selected + count - 1) % count;
						break;
					case SDLK_DOWN:
					case SDLK_s:
						selected = (selected + 1) % count;
						break;
					case SDLK_1:
					case SDLK_KP_1:
						selected = 0;
						finished = true;
						accepted = true;
						break;
					case SDLK_2:
					case SDLK_KP_2:
						selected = 1;
						finished = true;
						accepted = true;
						break;
					case SDLK_3:
					case SDLK_KP_3:
						selected = 2;
						finished = true;
						accepted = true;
						break;
					case SDLK_4:
					case SDLK_KP_4:
						selected = 3;
						finished = true;
						accepted = true;
						break;
					case SDLK_RETURN:
					case SDLK_SPACE:
						finished = true;
						accepted = true;
						break;
					case SDLK_ESCAPE:
						finished = true;
						accepted = false;
						break;
					default:
						break;
					}
				}
				else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
				{
					int hit = hit_row(window, event.button.x, event.button.y, count);
					if (hit >= 0)
					{
						if (hit == selected)
						{
							finished = true;
							accepted = true;
						}
						else
							selected = hit;
					}
				}
			}

			int width_screen = 0, height_screen = 0;
			SDL_GetWindowSize(window, &width_screen, &height_screen);

			SDL_SetRenderDrawColor(renderer, 28, 36, 24, 255);
			SDL_RenderClear(renderer);

			draw_texture(renderer, tex_title, width_screen / 2, 92, true);
			for (int i = 0; i < count; i++)
			{
				SDL_Rect row = row_rect(width_screen, height_screen, i);
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
				if (i == selected)
					SDL_SetRenderDrawColor(renderer, 92, 68, 36, 230);
				else
					SDL_SetRenderDrawColor(renderer, 24, 32, 22, 210);
				SDL_RenderFillRect(renderer, &row);
				SDL_SetRenderDrawColor(renderer, i == selected ? 255 : 120, i == selected ? 196 : 140, i == selected ? 120 : 90, 255);
				SDL_RenderDrawRect(renderer, &row);
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

				draw_texture(renderer, tex_name[i], row.x + 28, row.y + 16, false);
				draw_texture(renderer, tex_detail[i], row.x + 28, row.y + 52, false);
			}
			draw_texture(renderer, tex_hint, width_screen / 2, height_screen - 64, true);

			SDL_RenderPresent(renderer);
			SDL_Delay(16);
		}

		if (accepted)
			path = choices[selected].file;

		SDL_DestroyTexture(tex_title);
		SDL_DestroyTexture(tex_hint);
		for (int i = 0; i < count; i++)
		{
			SDL_DestroyTexture(tex_name[i]);
			SDL_DestroyTexture(tex_detail[i]);
		}
		return accepted;
	}

private:
	static SDL_Texture* make_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color)
	{
		SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
		if (!surface)
			return nullptr;
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		return texture;
	}

	static void draw_texture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, bool center_x)
	{
		if (!texture)
			return;
		int w = 0, h = 0;
		SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
		SDL_Rect dst = { center_x ? x - w / 2 : x, y, w, h };
		SDL_RenderCopy(renderer, texture, nullptr, &dst);
	}

	static SDL_Rect row_rect(int width_screen, int height_screen, int index)
	{
		SDL_Rect row;
		row.w = 760;
		row.h = 96;
		row.x = (width_screen - row.w) / 2;
		row.y = 150 + index * 112;
		(void)height_screen;
		return row;
	}

	static int hit_row(SDL_Window* window, int x, int y, int count)
	{
		int width_screen = 0, height_screen = 0;
		SDL_GetWindowSize(window, &width_screen, &height_screen);
		SDL_Point point = { x, y };
		for (int i = 0; i < count; i++)
		{
			SDL_Rect row = row_rect(width_screen, height_screen, i);
			if (SDL_PointInRect(&point, &row))
				return i;
		}
		return -1;
	}
};

#endif // !_MAP_SELECTOR_H_

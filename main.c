#include "rtv1.h"

void	ft_pixel_put(t_file *file, int x, int y)
{
	int i;

	i = (x * 4) + (y * file->sl);
	if ((x > 0 && x < 1920) && (y > 0 && y < 1080))
	{
		file->imgdata[i] = (char)file->color.blue;
		file->imgdata[i + 1] = (char)file->color.green;
		file->imgdata[i + 2] = (char)file->color.red;
	}
}

int main (int argc, char *argv[])
{
	t_param			*p;

	p = (t_param *)malloc(sizeof(t_param));

	SDL_Init(SDL_INIT_VIDEO);

	p->sdl.tex = SDL_CreateTexture(p->sdl.ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
	
	p->sdl.pitch = 0;
	p->cam.pos = (t_vec3df){0.0, 0.0, 0.0};
	p->cam.rot = (t_vec3df){1.0, 1.0, 1.0};
	p->sdl.win = SDL_CreateWindow("RTV1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	p->sdl.ren = SDL_CreateRenderer(p->sdl.win, -1, SDL_RENDERER_ACCELERATED);
	if (p->sdl.win == NULL)
		return (1);
	while (1)
	{
		SDL_Event		event;
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				break;
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_d)
					p->cam.pos = v_add(p->cam.pos, (t_vec3df){0.005, 0, 0});
				if (event.key.keysym.sym == SDLK_q)
					p->cam.pos = v_add(p->cam.pos, (t_vec3df){-0.005, 0, 0});
				if (event.key.keysym.sym == SDLK_s)
					p->cam.pos = v_add(p->cam.pos, (t_vec3df){0, 0, 0.005});
				if (event.key.keysym.sym == SDLK_z)
					p->cam.pos = v_add(p->cam.pos, (t_vec3df){0, 0, -0.005});
				if (event.key.keysym.sym == SDLK_LSHIFT)
					p->cam.pos = v_add(p->cam.pos, (t_vec3df){0, 0.005, 0});
				if (event.key.keysym.sym == SDLK_SPACE)
					p->cam.pos = v_add(p->cam.pos, (t_vec3df){0, -0.005, 0});
				if (event.key.keysym.sym == SDLK_a)
					p->cam.rot = v_add(p->cam.rot, (t_vec3df){0.05,0, 0});
				if (event.key.keysym.sym == SDLK_e)
					p->cam.rot = v_add(p->cam.rot, (t_vec3df){-0.05, 0, 0});
			}
		}
		SDL_LockTexture(p->sdl.tex, NULL, (void**)&p->sdl.pixels, &p->sdl.pitch);
		SDL_RenderCopy(p->sdl.ren, p->sdl.tex, NULL, NULL);
		ray_tracing(p);
		SDL_RenderPresent(p->sdl.ren);
		//SDL_RenderClear(p->sdl.ren);
	}
	SDL_DestroyRenderer(p->sdl.ren);
	SDL_DestroyWindow(p->sdl.win);
	SDL_Quit();
	return (0);
}
#include "rtv1.h"

void	ft_pixel_put(t_param *p, int x, int y)
{
	int i;

	//i = (x * 4) + (y * p->sdl.pitch);
	//i = (x * 4) + (y);
	//i = y + (p->sdl.pitch / 4) + x;
	i = (WIDTH * 4 * y) + x * 4;
	
	if ((x > 0 && x < WIDTH) && (y > 0 && y < HEIGHT))
	{
		p->sdl.pixels[i + 0] = 255;//p->rgb.b;
		p->sdl.pixels[i + 1] = 255;//(char)p->rgb.g;
		p->sdl.pixels[i + 2] = 0;//(char)p->rgb.r;
		p->sdl.pixels[i + 3] = SDL_ALPHA_OPAQUE;//(char)p->rgb.r;
	}
}

int main (int argc, char *argv[])
{
	t_param			*p;

	p = (t_param *)malloc(sizeof(t_param));

	SDL_Init(SDL_INIT_VIDEO);

	p->sdl.tex = SDL_CreateTexture(p->sdl.ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	
	//p->sdl.pitch = ;
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
	//	ray_tracing(p);
		SDL_LockTexture(p->sdl.tex, NULL, (void**)&p->sdl.pixels, &p->sdl.pitch);
		ft_pixel_put(p, 100, 100);
		SDL_UpdateTexture(p->sdl.tex, NULL, &p->sdl.pixels[0], WIDTH * 4);
		//SDL_UnlockTexture(p->sdl.tex);
		//SDL_SetRenderTarget(p->sdl.ren, p->sdl.tex);
		SDL_RenderCopy(p->sdl.ren, p->sdl.tex, NULL, NULL);
		SDL_RenderPresent(p->sdl.ren);
		//SDL_RenderClear(p->sdl.ren);
	}
	SDL_DestroyRenderer(p->sdl.ren);
	SDL_DestroyWindow(p->sdl.win);
	SDL_Quit();
	return (0);
}
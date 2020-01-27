#include "rtv1.h"

void	ft_pixel_put(t_param *p, int x, int y)
{
	int i;
	i = (WIDTH * y) + x;
	
	if ((x >= 0 && x < WIDTH) && (y >= 0 && y < HEIGHT))
		p->sdl.pixels[i] = p->color;
}

void		addsphere(t_param *p)
{
	t_sphere		sp1;
	t_sphere		sp2;
	t_sphere		sp3;

	sp1.pos = (t_vec3df){0., 0., -50};
	sp1.radius = 3;
	sp1.color = 0xFF0000;
	sp2.pos = (t_vec3df){0, 0., 50};
	sp2.radius = 3;
	sp2.color = 0x00FF00;
	sp3.pos = (t_vec3df){1.5, 0.8, 0};
	sp3.radius = 1.5;
	sp3.color = 0x0000FF;
	sphere_pushback(&p->obj.sp, sp1, p);
	sphere_pushback(&p->obj.sp, sp2, p);
	// sphere_pushback(&p->obj.sp, sp3, p);

}

int main (int argc, char *argv[])
{
	t_param			*p;

	p = (t_param *)malloc(sizeof(t_param));

	SDL_Init(SDL_INIT_VIDEO);

	p->cam.pos = (t_vec3df){0.0, 0.0, -100.0};
	p->cam.rot = (t_vec3df){1.0, 1.0, 1.0};
	p->sdl.win = SDL_CreateWindow("RTV1starfoulilaliloulala", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	p->sdl.ren = SDL_CreateRenderer(p->sdl.win, -1, 0);
	p->sdl.tex = SDL_CreateTexture(p->sdl.ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	p->sdl.pixels = malloc(sizeof(uint32_t) * WIDTH * HEIGHT);
	p->obj.sp = NULL;
	addsphere(p);
	if (p->sdl.win == NULL)
		return (1);
	while (1)
	{
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		SDL_Event		event;
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				break;
			if (event.type == SDL_KEYDOWN)
			{
				if (state[SDL_SCANCODE_D])
					p->cam.pos = v_add(p->cam.pos, (t_vec3df){5, 0, 0});
				if (state[SDL_SCANCODE_Q])
					p->cam.pos = v_add(p->cam.pos, (t_vec3df){-5, 0, 0});
				if (state[SDL_SCANCODE_S])
					p->cam.pos = v_add(p->cam.pos, (t_vec3df){0, 0, 5});
				if (state[SDL_SCANCODE_Z])
					p->cam.pos = v_add(p->cam.pos, (t_vec3df){0, 0, -5});
				if (event.key.keysym.sym == SDLK_LSHIFT)
					p->cam.pos = v_add(p->cam.pos, (t_vec3df){0, 5, 0});
				if (event.key.keysym.sym == SDLK_SPACE)
					p->cam.pos = v_add(p->cam.pos, (t_vec3df){0, -5, 0});
				if (event.key.keysym.sym == SDLK_a)
					p->cam.rot = v_add(p->cam.rot, (t_vec3df){0.05,0, 0});
				if (event.key.keysym.sym == SDLK_e)
					p->cam.rot = v_add(p->cam.rot, (t_vec3df){-0.05, 0, 0});
			}
		}
		ray_tracing(p);
		SDL_UpdateTexture(p->sdl.tex, NULL, p->sdl.pixels, WIDTH * 4);
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
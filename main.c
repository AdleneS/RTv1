 #include "rtv1.h"

void	ft_pixel_put(t_param *p, int x, int y)
{
	int			i;
	uint32_t	color;
	
	i = (WIDTH * y) + x;
	color = rgb_to_hex(p->color);
	
	if ((x >= 0 && x < WIDTH) && (y >= 0 && y < HEIGHT))
	{
		p->sdl.pixels[i] = color;

	}
}

void		addsphere(t_param *p)
{
	t_sphere		sp1;
	t_sphere		sp2;
	t_sphere		sp3;
	t_sphere		sp4;
	t_light			light;
	t_light			light2;
	t_light			light3;
	t_plane			pl1;

	sp1.pos = (t_vec3df){20, 0.8, 0};
	sp1.radius = 3;
	sp1.spe = 100;
	sp1.color = (t_rgb){255, 100, 100, 255};
	sp2.pos = (t_vec3df){50, 5.0, 50};
	sp2.radius = 3;
	sp2.spe = -1;
	sp2.color = (t_rgb){0, 255, 0, 255};
	sp3.pos = (t_vec3df){0.0, 10.0, 0.0};
	sp3.radius = 5;
	sp3.spe = -1;
	sp3.color = (t_rgb){1, 1, 255, 255};
	sp4.pos = (t_vec3df){50, -10.0, 0.0};
	sp4.radius = 25;
	sp4.spe = 500;
	sp4.color = (t_rgb){255, 255, 255, 255};
	light.pos = (t_vec3df){10, 5.0, 0};
	light.color = (t_rgb){255, 255, 255, 255};
	light.intensity = 0.1;
	light.type = 2;
	light2.pos = (t_vec3df){-100.0, 10.0, 20.0};
	light2.color = (t_rgb){255, 255, 255, 255};
	light2.intensity = 0.07;
	light2.type = 2;
	light3.pos = (t_vec3df){50.0, 50, 150.0};
	light3.color = (t_rgb){255, 255, 255, 255};
	light3.intensity = 0.1;
	light3.type = 2;
	pl1.pos = (t_vec3df){50, -10.0, 0.0};
	pl1.n = (t_vec3df){50.0, 10.0, 20.0};
	pl1.spe = 500;
	pl1.color = (t_rgb){255, 255, 255, 255};
	light_push(&p->light, light2, p);
	light_push(&p->light, light3, p);
	light_push(&p->light, light, p);
	plane_push(&p->obj.pl, pl1, p);
	sphere_push(&p->obj.sp, sp1, p);
	sphere_push(&p->obj.sp, sp2, p);
	sphere_push(&p->obj.sp, sp3, p);
	sphere_push(&p->obj.sp, sp4, p);
}

int main (int argc, char *argv[])
{
	t_param			*p;
	t_vec3df		move;
	p = (t_param *)malloc(sizeof(t_param));

	SDL_Init(SDL_INIT_VIDEO);

	p->cam.pos = (t_vec3df){0.0, 0.0, -100.0};
	p->cam.rot = (t_vec3df){0.0, 0.0, 0.0};
	
	p->sdl.win = SDL_CreateWindow("RTV1starfoulilaliloulala", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	p->sdl.ren = SDL_CreateRenderer(p->sdl.win, -1, 0);
	p->sdl.tex = SDL_CreateTexture(p->sdl.ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	p->sdl.pixels = malloc(sizeof(uint32_t) * WIDTH * HEIGHT);
	p->obj.sp = NULL;
	p->light = NULL;
	addsphere(p);
	if (p->sdl.win == NULL)
		return (1);
	while (1)
	{
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		SDL_Event		event;
		if (SDL_PollEvent(&event))
		{
			move = (t_vec3df){0.0, 0.0, 0.0};
			if (event.type == SDL_QUIT)
				break;
			if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
			{
				if (state[SDL_SCANCODE_D])
					move = v_add(move, (t_vec3df){1, 0, 0});
				if (state[SDL_SCANCODE_A])
					move = v_add(move, (t_vec3df){-1, 0, 0});
				if (state[SDL_SCANCODE_S])
					move = v_add(move, (t_vec3df){0, 0, -1});
				if (state[SDL_SCANCODE_W])
					move = v_add(move, (t_vec3df){0, 0, 1});
				if (state[SDL_SCANCODE_LSHIFT])
					move = v_add(move, (t_vec3df){0, 1, 0});
				if (state[SDL_SCANCODE_SPACE])
					move = v_add(move, (t_vec3df){0, -1, 0});
				if (state[SDL_SCANCODE_R])
					p->cam.rot = v_add(p->cam.rot, (t_vec3df){0.05,0, 0});
				if (state[SDL_SCANCODE_F])
					p->cam.rot = v_add(p->cam.rot, (t_vec3df){-0.05, 0, 0});
				if (state[SDL_SCANCODE_E])
					p->cam.rot = v_add(p->cam.rot, (t_vec3df){0, 0.05, 0});
				if (state[SDL_SCANCODE_Q])
					p->cam.rot = v_add(p->cam.rot, (t_vec3df){0, -0.05, 0});
			}
			rot_x(p, &move);
			rot_y(p, &move);
			p->cam.pos = v_add(p->cam.pos, move);
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
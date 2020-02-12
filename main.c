#include "rtv1.h"

int		multithreads(t_param *p)
{
	int			i;
	pthread_t	tid[THREADS];
	t_param		th_param[THREADS];

	i = 0;
	while (i < THREADS)
	{
		th_param[i] = *p;
		th_param[i].th_i = TWIDTH * i;
		th_param[i].th_imax = TWIDTH * (i + 1);
		pthread_create(&tid[i], NULL, ray_tracing, &th_param[i]);
		i++;
	}
	while (i--)
		pthread_join(tid[i], NULL);
	return (1);
}

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
	t_light			light;
	t_light			light2;
	t_light			light3;
	t_plane			pl1;
	t_tex			tex;
	t_cylinder		cyn;
	t_cone			cone;

	sp1.pos = (t_vec3df){0, -5, 0};
	sp1.radius = 5;
	tex.color = (t_rgb){1, 0, 0, 1};
	tex.spe = 800;
	obj_push(1, &p->obj, &sp1, tex);

	sp1.pos = (t_vec3df){-20, 0, 45};
	sp1.radius = 10;
	tex.color = (t_rgb){0, 1, 1, 1};
	tex.spe = -1;
	obj_push(1, &p->obj, &sp1, tex);

	sp1.pos = (t_vec3df){0, -5, -20};
	sp1.radius = 10;
	tex.color = (t_rgb){0.9, 1.0, 0.2, 1};
	tex.spe = -1;
	obj_push(1, &p->obj, &sp1, tex);

	cyn.pos = (t_vec3df){0, 0, -10};
	cyn.r = 10;
	cyn.n = (t_vec3df){0, 1, 0};
	tex.color = (t_rgb){0.9, 0.7, 0.9, 1};
	tex.spe = 800;
	obj_push(3, &p->obj, &cyn, tex);

	cyn.pos = (t_vec3df){0, 30, 30};
	cyn.r = 5;
	cyn.n = (t_vec3df){1, 0, 0};
	tex.color = (t_rgb){1.0, 0.0, 1.0, 1};
	tex.spe = 800;
	obj_push(3, &p->obj, &cyn, tex);

	cone.pos = (t_vec3df){-70, 0, -50};
	cone.angle = 5.0;
	cone.n = (t_vec3df){0, 0, 1};
	tex.color = (t_rgb){1.0, 1.0, 0.5, 1};
	tex.spe = 800;
	obj_push(4, &p->obj, &cone, tex);
	//sp2.pos = (t_vec3df){50, 5.0, 50};
	//sp2.radius = 3;
	//sp2.spe = -1;
	//sp3.pos = (t_vec3df){0.0, 10.0, 0.0};
	//sp3.radius = 5;
	//sp3.spe = -1;
	//sp4.pos = (t_vec3df){50, -10.0, 0.0};
	//sp4.radius = 25;
	//sp4.spe = -1;
	pl1.pos = (t_vec3df){0.0, -20.0, 0.0};
	pl1.n = (t_vec3df){0.0, 1.0, 0.0};
	tex.color = (t_rgb){1, 1, 0, 1};
	tex.spe = 800;
	obj_push(2, &p->obj, &pl1, tex);

	pl1.pos = (t_vec3df){50.0, 0.0, 0.0};
	pl1.n = (t_vec3df){-1.0, 0.0, 0.0};
	tex.color = (t_rgb){0.02, 0.8, 0.6, 255};
	tex.spe = 800;
	obj_push(2, &p->obj, &pl1, tex);

	pl1.pos = (t_vec3df){0.0, 50.0, 0.0};
	pl1.n = (t_vec3df){0.0, -1.0, 0.0};
	tex.color = (t_rgb){1, 0.4, 0.01, 255};
	tex.spe = -1;
	obj_push(2, &p->obj, &pl1, tex);

	pl1.pos = (t_vec3df){0.0, 0.0, 75.0};
	pl1.n = (t_vec3df){0.0, 0.0, -1.0};
	tex.color = (t_rgb){0.01, 0.2, 0.9, 255};
	tex.spe = 800;
	obj_push(2, &p->obj, &pl1, tex);
	//sphere_push(&p->obj.sp, sp1, p);
	//sphere_push(&p->obj.sp, sp2, p);
	//sphere_push(&p->obj.sp, sp3, p);

	//sphere_push(&p->obj.sp, sp4, p);
	//obj_push(2, &p->obj, &pl1, (t_rgb){1, 1, .5, 255});
	//obj_push(1, &p->obj, &sp2, (t_rgb){0, 255, 0, 255});
	//obj_push(1, &p->obj, &sp3, (t_rgb){0, 0, 255, 255});
	//obj_push(1, &p->obj, &sp4, (t_rgb){255, 255, 255, 255});
	light.pos = (t_vec3df){-30.0, 10.0, 30.0};
	light.color = (t_rgb){0, 0, 1, 1};
	light.intensity = .5;
	light.type = 2;
	light_push(&p->light, light, p);

	light2.pos = (t_vec3df){1.0, 0.0, 0.0};
	light2.color = (t_rgb){1, 1, 1, 1};
	light2.intensity = 0.1;
	light2.type = 2;
	light_push(&p->light, light2, p);

	light3.pos = (t_vec3df){-100.0, 20.0, 0.0};
	light3.color = (t_rgb){1, 1, 1, 1};
	light3.intensity = 0.8;
	light3.type = 2;
	light_push(&p->light, light3, p);
}

int main (int argc, char *argv[])
{
	t_param			*p;
	t_vec3df		move;
	p = (t_param *)malloc(sizeof(t_param));

	SDL_Init(SDL_INIT_VIDEO);

	p->cam.pos = (t_vec3df){-150.0, 0.0, -50.0};
	p->cam.rot = (t_vec3df){0.0, 1.5, 0.0};
	p->sdl.win = SDL_CreateWindow("RTV1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	p->sdl.ren = SDL_CreateRenderer(p->sdl.win, -1, 0);
	p->sdl.tex = SDL_CreateTexture(p->sdl.ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	p->sdl.pixels = malloc(sizeof(uint32_t) * WIDTH * HEIGHT);
	p->light = NULL;
	//p->obj.sp = NULL;
	p->obj = NULL;
	SDL_Event	event;
	addsphere(p);
	if (p->sdl.win == NULL)
		return (1);
	while (1)
	{
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (SDL_PollEvent(&event))
		{
			move = (t_vec3df){0.0, 0.0, 0.0};
			if (event.type == SDL_QUIT)
				break ;
			if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
			{
				if (state[SDL_SCANCODE_D])
					move = v_add(move, (t_vec3df){1, 0, 0});
				if (state[SDL_SCANCODE_ESCAPE])
					break ;
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
		//ray_tracing(p);
		multithreads(p);
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
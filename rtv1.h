#ifndef RTV1_H
# define RTV1_H

# include "include\SDL2\SDL.h"
# include <stdio.h>
# include <math.h>

# define WIDTH 800
# define HEIGHT 450
# define INF 10^8
# define RATIO WIDTH / HEIGHT
# define FOV 30
# define PI 3.14
# define ANGLE tan(PI * 0.5 * FOV / 180.0)

typedef struct			s_vec2d
{	
	int					x;
	int					y;
}						t_vec2d;

typedef struct			s_vec2df
{	
	double				x;
	double				y;
}						t_vec2df;

typedef struct			s_vec3df
{	
	double				x;
	double				y;
	double				z;
}						t_vec3df;

typedef struct			s_rgb
{	
	unsigned int		r;
	unsigned int		g;
	unsigned int		b;
}						t_rgb;

typedef struct			s_sphere
{	
	struct s_vec3df		pos;
	double				radius;
	struct s_rgb		color;
}						t_sphere;

typedef struct			s_cam
{	
	struct s_vec3df		pos;
	struct s_vec3df		rot;

}						t_cam;

typedef struct			s_vertex
{	
	double				x;
	double				y;
	double				z;
	double				w;
}						t_vertex;

typedef struct			s_matrix
{
	double				m[4][4];
}						t_matrix;

typedef struct			s_sdl
{
	SDL_Window			*win;
	SDL_Texture			*tex;
	Uint32				*pixels;
	int 				pitch;
	SDL_Renderer		*ren;
}						t_sdl;

typedef struct			s_param
{
	struct s_sdl		sdl;
	struct s_cam		cam;
}						t_param;


t_vec3df		v_sub(t_vec3df v1, t_vec3df v2);
double			v_dotproduct(t_vec3df v1, t_vec3df v2);
void			ray_tracing(t_param *p);
t_vec3df		v_mul(t_vec3df v1, t_vec3df v2);
t_vec3df		v_add(t_vec3df v1, t_vec3df v2);

#endif

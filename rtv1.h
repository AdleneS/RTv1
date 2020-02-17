#ifndef RTV1_H
# define RTV1_H

# include "SDL.h"
# include <stdio.h>
# include <math.h>
# include <pthread.h>

# define WIDTH 800
# define HEIGHT 450
# define THREADS 16
# define TWIDTH (WIDTH / THREADS)
# define INF 10^8
# define FOV 30
# define PI 3.14
# define ANGLE tan((FOV / 2) * M_PI / 180.0)

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

typedef struct			s_quadratic
{
	t_vec3df			oc;
	double				a;
	double				b;
	double				c;
	double				disc;
	double				t1;
	double				t2;
	double				dot1;
	double				dot2;
}						t_quadratic;

typedef struct			s_rgb
{
	float				r;
	float				g;
	float				b;
	float				a;
}						t_rgb;

typedef struct			s_sphere
{
	struct s_vec3df		pos;
	double				radius;
	double				r2;
}						t_sphere;

typedef struct			s_cylinder
{
	struct s_vec3df		pos;
	struct s_vec3df		n;
	double				r;
	double				r2;
}						t_cylinder;

typedef struct			s_plane
{
	struct s_vec3df		pos;
	struct s_vec3df		n;
	struct s_plane		*next;
}						t_plane;

typedef struct			s_cone
{
	struct s_vec3df		pos;
	struct s_vec3df		n;
	float				angle;
	struct s_plane		*next;
}						t_cone;

typedef struct			s_tex
{
	struct s_rgb		color;
	float				spe;
}						t_tex;

typedef struct			s_obj
{
	int					type;
	struct s_tex		tex;
	void				*data;
	struct s_obj		*next;
	t_vec3df			oc;
	//struct s_sphere		*sp;
	//struct s_plane		*pl;
}						t_obj;

typedef struct			s_light
{
	struct s_vec3df		pos;
	struct s_rgb		color;
	float				intensity;
	unsigned int		type;
	struct s_light		*next;
}						t_light;


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
	uint32_t			*pixels;
	int 				pitch;
	SDL_Renderer		*ren;
}						t_sdl;

typedef struct			s_param
{
	struct s_sdl		sdl;
	struct s_rgb		color;
	struct s_cam		cam;
	int					th_i;
	int					th_imax;
	double				dd;
	int					moving;
	struct s_obj		*obj;
	struct s_light		*light;
}						t_param;


t_vec3df		v_sub(t_vec3df v1, t_vec3df v2);
double			v_dotproduct(t_vec3df v1, t_vec3df v2);
void			*ray_tracing(void *pvoid);
t_vec3df		v_mul(t_vec3df v1, t_vec3df v2);
t_vec3df		v_add(t_vec3df v1, t_vec3df v2);
void			ft_pixel_put(t_param *p, int x, int y);
void			sphere_push(t_sphere **list, t_sphere sp, t_param *p);
t_vec3df		v_normalize(t_vec3df v);
void			rot_x(t_param *p, t_vec3df *d);
void			rot_y(t_param *p, t_vec3df *d);
void			light_push(t_light **list, t_light light, t_param *p);
t_vec3df		v_sub(t_vec3df v1, t_vec3df v2);
t_vec3df		v_mulk(t_vec3df v1, double k);
t_rgb			mult_color(t_rgb color, t_rgb k);
uint32_t		rgb_to_hex(t_rgb color);
double			v_length(t_vec3df v);
void			plane_push(t_plane **list, t_plane pl, t_param *p);
void			obj_push(int type, t_obj **list, void *data, t_tex tex);
t_rgb			add_color(t_rgb color, t_rgb k);
t_rgb			addk_color(t_rgb color, float k);
t_rgb			mulk_color(t_rgb color, float k);

#endif

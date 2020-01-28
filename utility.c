#include "rtv1.h"

double		min(double a, double b)
{
	return ((a < b) ? a : b);
}

double		max(double a, double b)
{
	return ((a > b) ? a : b);
}

double		clamp(double a, double mi, double ma)
{
	return (min(max(a, mi), ma));
}

t_vec3df		v_add(t_vec3df v1, t_vec3df v2)
{
	return ((t_vec3df){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z});
}

t_vec3df		v_sub(t_vec3df v1, t_vec3df v2)
{
	return ((t_vec3df){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z});
}

t_vec3df		v_mul(t_vec3df v1, t_vec3df v2)
{
	return ((t_vec3df){v1.x * v2.x, v1.y * v2.y, v1.z * v2.z});
}

t_vec3df		v_mulk(t_vec3df v1, double k)
{
	return ((t_vec3df){v1.x * k, v1.y * k, v1.z * k});
}
t_vec3df		v_div(t_vec3df v1, double k)
{
	return ((t_vec3df){v1.x / k, v1.y / k, v1.z / k});
}

uint32_t		rgb_to_hex(t_rgb color)
{
	return (color.a << 24 | color.r << 16 | color.g << 8 | color.b);
	//return (((color.r >> 24) & 0xff) * 0x01000000 + ((color.g >> 16) & 0xff) * 0x010000 + ((color.b >> 8) & 0xff) * 0x0100 + (color.a & 0xff));
}

void			mult_color(t_rgb *color, int k)
{
	//uint32_t uu;
//
	//uu = 0xa00001;
	//printf("%d %d %d %d\n", (uu >> 24 & 0xff), (uu >> 16 & 0xff) , (uu >> 8 & 0xff), (uu & 0xff));
	//printf("%#08x\n", ((uu >> 24 & 0xff) * 0x01000000) + ((uu >> 16 & 0xff) * 0x010000) + ((uu >> 8 & 0xff) * 0x0100) + ((uu & 0xff)));
	//printf("%#08x\n", 65738);
	//*hex = (((*hex >> 24) & 0xff) * 0x01000000) * k + (((*hex >> 16) & 0xff) * 0x010000) * k + (((*hex >> 8) & 0xff) * 0x0100) * k + ((*hex & 0xff)) * k;
	*color = (t_rgb){(color->r * k), (color->g * k), (color->b * k), (color->a * k)};
}

void			rot_x(t_param *p, t_vec3df *d)
{
	t_vec3df new;

	new.x = d->x;
	new.y = d->y * cosf(p->cam.rot.x) - d->z * sinf(p->cam.rot.x);
	new.z = d->y * sinf(p->cam.rot.x) + d->z * cosf(p->cam.rot.x);
	*d = new;
}

void			rot_y(t_param *p, t_vec3df *d)
{
	t_vec3df new;

	new.x = d->x * cosf(p->cam.rot.y) + d->z * sinf(p->cam.rot.y);
	new.y = d->y;
	new.z = d->x * -sinf(p->cam.rot.y) + d->z * cosf(p->cam.rot.y);
	*d = new;
}

double			v_dotproduct(t_vec3df v1, t_vec3df v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

double v_length(t_vec3df v)
{
	return (sqrtf(v_dotproduct(v, v)));
}

void		v_normalize(t_vec3df *v)
{
	double l = v_length(*v);
	*v = ((t_vec3df){v->x / l, v->y / l, v->z / l});
}

t_vec3df		v_crossprod(t_vec3df v1, t_vec3df v2)
{
	t_vec3df v;
	
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return (v);
}

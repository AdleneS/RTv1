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
	color.r = max(0.0, color.r);
	color.g = max(0.0, color.g);
	color.b = max(0.0, color.b);
	color.a = max(0.0, color.a);
	color.r = min(1.0, color.r);
	color.g = min(1.0, color.g);
	color.b = min(1.0, color.b);
	color.a = min(1.0, color.a);
	color.r *= 255;
	color.g *= 255;
	color.b *= 255;
	color.a *= 255;
	return ((int)color.a << 24 | (int)color.r << 16 | (int)color.g << 8 | (int)color.b);
}

t_rgb			mult_color(t_rgb color, double k)
{
	t_rgb		tmp;

	tmp = color;
	tmp.r /= 255;
	tmp.g /= 255;
	tmp.b /= 255;
	tmp.a /= 255;
	return ((t_rgb){(tmp.r * k), (tmp.g * k), (tmp.b * k), (tmp.a * k)});
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

t_vec3df		v_normalize(t_vec3df v)
{
	double l = v_length(v);
	return ((t_vec3df){v.x / l, v.y / l, v.z / l});
}

t_vec3df		v_crossprod(t_vec3df v1, t_vec3df v2)
{
	t_vec3df v;
	
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return (v);
}

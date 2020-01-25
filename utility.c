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

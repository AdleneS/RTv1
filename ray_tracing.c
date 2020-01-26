#include "rtv1.h"

t_vec2df	intersect_sphere(t_param *p, t_vec3df d, t_sphere sp)
{
	t_vec3df	oc;
	double		k1;
	double		k2;
	double		k3;
	double		t1;
	double		t2;
	double		discriminant;

	oc = v_sub(p->cam.pos, sp.pos);
	k1 = v_dotproduct(d, d);
	k2 = 2 * v_dotproduct(oc, d);
	k3 = v_dotproduct(oc,oc) - sp.radius * sp.radius;

	discriminant = k2 * k2 - 4.0 * k1 * k3;

	if (discriminant < 0)
		return ((t_vec2df){INF, INF});
	t1 = (-k2 + sqrt(discriminant)) / (2.0 * k1);
	t2 = (-k2 - sqrt(discriminant)) / (2.0 * k1);
	return ((t_vec2df){t1, t2});
}

void		trace_ray(t_param *p, t_vec3df d)
{
	double		closest_t;
	t_sphere	*closest_sphere;
	t_vec2df	t;
	t_sphere	*tmp;

	tmp = p->obj.sp;
	closest_t = INF;
	closest_sphere = NULL;
	while (tmp)
	{
		t = intersect_sphere(p, d, *tmp);
		if (t.x > 1 && t.x < INF && t.x < closest_t)
		{
			closest_t = t.x;
			closest_sphere = tmp;
		}
		if (t.y > 1 && t.y < INF && t.y < closest_t)
		{
			closest_t = t.y;
			closest_sphere = tmp;
		}
		tmp = tmp->next;
	}
	if (closest_sphere == NULL)
	{
		p->color = 0x00;
		return ;
	}
	p->color = closest_sphere->color;
}

void		ray_tracing(t_param *p)
{
	int			x;
	int			y;
	t_vec3df	d;

	x = 0;
	while (x < WIDTH )
	{
		y = 0;
		while (y < HEIGHT)
		{
			d = (t_vec3df){x * (double)RATIO / WIDTH, y * 1.0 / HEIGHT, 1.0};
			//d = (t_vec3df){x * 1.0 / WIDTH, y * 1.0 / HEIGHT, 1.0};
			//d = (t_vec3df){(2.0 * ((x + 0.5) * (1.0 / WIDTH))) * ANGLE * RATIO, (2.0 * ((y + 0.5) * 1.0 / HEIGHT)) * ANGLE, 1.0};
			trace_ray(p, d);
			ft_pixel_put(p, x, y);
			//SDL_RenderDrawPoint(p->sdl.ren, x, y);
			y++;
		}
		x++;
	}
}
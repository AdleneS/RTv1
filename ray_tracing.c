#include "rtv1.h"

float	intersect_sphere(t_param *p, t_vec3df d, t_sphere *sp)
{
	t_vec3df	oc;
	double		k1;
	double		k2;
	double		k3;
	double		t1;
	double		t2;
	double		discriminant;
	float		di;

	oc = v_sub(p->cam.pos, sp->pos);
	k1 = v_dotproduct(d, d);
	k2 = 2 * v_dotproduct(oc, d);
	k3 = v_dotproduct(oc, oc) - (sp->radius * sp->radius);

	discriminant = k2 * k2 - 4.0 * k1 * k3;

	if (discriminant < 0)
		return (-1);
	t1 = (-k2 + sqrt(discriminant)) * .5;
	t2 = (-k2 - sqrt(discriminant)) * .5;

	di = fminf(t1, t2);
	return (di);
}

void		trace_ray(t_param *p, t_vec3df d)
{
	t_sphere	*closest_sphere;
	t_sphere	*tmp;
	float		dis;
	float		t;

	dis = INFINITY;
	tmp = p->obj.sp;
	closest_sphere = NULL;
	while (tmp)
	{
		if ((t = intersect_sphere(p, d, tmp)) > 0 && t < dis)
		{
			dis = t;
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

void		lala2(t_vec3df *d, float ratio)
{
	d->x = (d->x + .5) / WIDTH;
	d->y = (d->y + .5) / HEIGHT;
	d->x = 2 * d->x - 1;
	d->y = 1 - 2 * d->y;
	d->x = d->x * ANGLE * ratio;
	d->y = d->y * ANGLE;
	d->z = 1;
	*d = v_normalize(*d);
	
}

void		ray_tracing(t_param *p)
{
	int			x;
	int			y;
	t_vec3df	d;
	float		ratio;

	ratio = WIDTH / (float)HEIGHT;
	x = 0;
	while (x < WIDTH )
	{
		y = 0;
		while (y < HEIGHT)
		{
			// d = (t_vec3df){x * (double)RATIO / WIDTH, y * 1.0 / HEIGHT, 1.0};

			// d = (t_vec3df){x * 1.0 / WIDTH, y * 1.0 / HEIGHT, 1.0};
			d.x = x;
			d.y = y;
			d.z = 1;
			lala2(&d, ratio);
			trace_ray(p, d);
			ft_pixel_put(p, x, y);
			//SDL_RenderDrawPoint(p->sdl.ren, x, y);
			y++;
		}
		x++;
	}
}
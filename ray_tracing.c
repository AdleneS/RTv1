#include "rtv1.h"

float	intersect_sphere(t_vec3df origin, t_vec3df d, t_sphere *sp, float min_t, float max_t)
{
	t_vec3df	oc;
	double		k1;
	double		k2;
	double		k3;
	double		t1;
	double		t2;
	double		discriminant;
	double		min;

	oc = v_sub(origin, sp->pos);
	k1 = v_dotproduct(d, d);
	k2 = 2 * v_dotproduct(oc, d);
	k3 = v_dotproduct(oc, oc) - (sp->radius * sp->radius);

	discriminant = k2 * k2 - 4.0 * k1 * k3;

	if (discriminant < 0)
		return (-1);
	t1 = (-k2 + sqrt(discriminant)) * .5;
	t2 = (-k2 - sqrt(discriminant)) * .5;
	min = fminf(t1, t2);
	if (min > min_t && min < max_t)
		return (min);
	else
		return (-1);
	
}

float	compute_light(t_param *p, t_vec3df phit, t_vec3df nhit, t_sphere *sp)
{
	t_light		*tmp;
	float		intensity;
	float		dot;
	t_vec3df	vec_l;
	t_vec3df	vec_r;
	double		r_dot_v;
	float		n_lenght;
	t_vec3df	shadow;
	float		t;
	t_sphere	*tmp1;

	tmp = p->light;
	n_lenght = v_length(nhit);
	intensity = 0;
	vec_l = (t_vec3df){0, 0, 0};
	while (tmp)
	{
		if (tmp->type == 1)
			intensity += tmp->intensity;
		else if (tmp->type == 2)
			vec_l = v_sub(tmp->pos, phit);
		else if (tmp->type == 3)
			vec_l = tmp->pos;
		tmp1 = p->obj.sp;
		v_normalize(&vec_l);
		if (sp->spe != -1)
		{
			vec_r = v_sub(v_mulk(nhit, (2.0 * v_dotproduct(nhit, vec_l))), vec_l);
			r_dot_v = v_dotproduct(vec_r, p->cam.pos);
			if (r_dot_v > 0)
				intensity += tmp->intensity * pow(r_dot_v / (v_length(vec_r) * v_length(p->cam.pos)), tmp1->spe);
		}
		while (tmp1)
		{
			if (((t = intersect_sphere(phit, vec_l, tmp1, 0.01, v_length(tmp->pos)))) < 0)
			{
				dot = v_dotproduct(nhit, vec_l);
				if (dot > 0)
					intensity += tmp->intensity * dot / (n_lenght * v_length(vec_l));

			}
			tmp1 = tmp1->next;
		}
		tmp = tmp->next;
	}
	return (intensity);
}

double		intersect_plane(t_plane *pl, t_vec3df d, t_param *p)
{
	double	denom;
	double	t;

	denom = v_dotproduct(pl->n, d);
	if (denom > 0)
	{
		t = v_dotproduct(v_sub(pl->pos, p->cam.pos), pl->n) / denom;
		return (t >= 0);
	}
	return (0);
}

void		trace_ray(t_param *p, t_vec3df d)
{
	t_sphere	*closest_sphere;
	t_sphere	*tmp;
	float		dis;
	float		t;
	t_vec3df	shadow;
	t_vec3df	phit;
	t_vec3df	nhit;

	dis = INFINITY;
	tmp = p->obj.sp;
	closest_sphere = NULL;
	while (tmp)
	{
		if (((t = intersect_sphere(p->cam.pos, d, tmp, 0, INFINITY)) > 0) && t < dis)
		{
			dis = t;
			closest_sphere = tmp;
			phit = v_add(p->cam.pos, v_mulk(d, t));
			nhit = v_sub(phit, closest_sphere->pos);
			nhit = v_mulk(nhit, (1.0 / v_length(nhit)));

		}
		tmp = tmp->next;
	}
	if (closest_sphere == NULL)
	{
		p->color = (t_rgb){0, 0, 0, 0};
		return ;
	}
	// printf("%f\n", closest_sphere->color.r);
	p->color = mult_color(closest_sphere->color, compute_light(p, phit, nhit, closest_sphere));
	//else
	//{
	//	p->color = mult_color(closest_sphere->color, compute_light(p, phit, nhit));
	//}
	//p->color = mult_color(closest_sphere->color, compute_light(p, phit, nhit, closest_sphere));

	//p->color = closest_sphere->color;
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
	v_normalize(d);
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
			rot_x(p, &d);
			rot_y(p, &d);
			trace_ray(p, d);
			ft_pixel_put(p, x, y);
			//SDL_RenderDrawPoint(p->sdl.ren, x, y);
			y += 1;
		}
		x += 1;
	}
}
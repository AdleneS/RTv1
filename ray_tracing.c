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
	t_obj		*tmp1;

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
		vec_l = v_normalize(vec_l);
		//if (sp->spe != -1)
		//{
		//	vec_r = v_sub(v_mulk(nhit, (2.0 * v_dotproduct(nhit, vec_l))), vec_l);
		//	r_dot_v = v_dotproduct(vec_r, p->cam.pos);
		//	if (r_dot_v > 0)
		//		intensity += tmp->intensity * pow(r_dot_v / (v_length(vec_r) * v_length(p->cam.pos)), sp->spe);
		//}
		//tmp1 = p->obj;
		//while (tmp1 && tmp1->type == 1)
		//{
			//if (((t = intersect_sphere(phit, vec_l, ((t_sphere*)tmp1->data), 0.01, v_length(tmp->pos)))) < 0)
			//{
				dot = v_dotproduct(nhit, vec_l);
				if (dot > 0)
					intensity += tmp->intensity * dot ;
			//}
			//tmp1 = tmp1->next;
		//}
		tmp = tmp->next;
	}
	return (intensity);
}

double		intersect_plane(t_plane *pl, t_vec3df d, t_param *p)
{
	/*double		denom;
	double		t;
	t_vec3df	diff;
	
	denom = v_dotproduct(pl->n, d);
	if ((denom) > 0.0001)
	{
		diff = v_sub(pl->pos, p->cam.pos);
		diff = v_normalize(diff);
		t = v_dotproduct(diff, pl->n) / denom;
		if (t > 0.0001)
			return (t);
	}
	return (-1);
*/
	double	t;
	double	a;
	double	b;

	a = v_dotproduct(v_sub(p->cam.pos, pl->pos), pl->n);
	b = v_dotproduct(d, pl->n);
	if (b < 0.0001 || (t = -a / b) < 0)
		return (-1);
	return (t > 0.001 ? t : -1);
}

void		trace_ray(t_param *p, t_vec3df d)
{
	t_obj		*closest;
	t_obj		*tmp;
	float		dis;
	float		t;
	t_vec3df	shadow;
	t_vec3df	phit;
	t_vec3df	nhit;

	dis = INFINITY;
	tmp = p->obj;
	closest = NULL;
	while (tmp)
	{

		if (tmp->type == 1)
			t = intersect_sphere(p->cam.pos, d, ((t_sphere*)tmp->data), 0, INFINITY);
		if (tmp->type == 2)
			t = intersect_plane(((t_plane*)tmp->data), d, p);
		if (t > 0.001 && t < dis)
		{
			dis = t;
			closest = tmp;
			phit = v_add(p->cam.pos, v_mulk(d, t));
			if (closest->type == 1)
			{
				nhit = v_sub(phit, ((t_sphere*)closest->data)->pos);
				nhit = v_mulk(nhit, (1.0 / v_length(nhit)));
			}
			if (closest->type == 2)
			{
				//nhit = v_sub(phit, ((t_plane*)closest->data)->pos);
				//nhit = v_mulk(nhit, (1.0 / v_length(nhit)));
				if (v_dotproduct(d, ((t_plane*)closest->data)->n) > 0)
					nhit = ((((t_plane*)closest->data)->n));
				nhit = (v_mulk(((t_plane*)closest->data)->n, -1));
				
			}
		}
		tmp = tmp->next;
	}
	if (closest == NULL)
	{
		p->color = (t_rgb){0, 0, 0, 0};
		return ;
	}

	//p->color = mult_color(closest->color, compute_light(p, phit, nhit, ((t_sphere*)closest->data)));
	//else
	//{
	////p->color = mult_color(closest->color, compute_light(p, phit, nhit));
	//}
	p->color = mult_color(closest->color, compute_light(p, phit, nhit, ((t_sphere*)closest->data)));
	// p->color = closest->color;
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
			rot_x(p, &d);
			rot_y(p, &d);
			trace_ray(p, d);
			ft_pixel_put(p, x, y);
			//SDL_RenderDrawPoint(p->sdl.ren, x, y);
			y++;
		}
		x++;
	}
}
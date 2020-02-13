#include "rtv1.h"

float				intersect_cone(t_vec3df origin, t_vec3df d, t_cone *cone)
{
	double			k1;
	double			k2;
	double			k3;
	t_vec3df		l;
	double			tmp[2];
	double			taN;
	double			t1;
	double			t2;
	double			discriminant;
	taN = cone->angle * (M_PI / 180);
	l = v_sub(origin, cone->pos);
	tmp[0] = v_dotproduct(d, v_normalize(cone->n));
	tmp[1] = v_dotproduct(l, v_normalize(cone->n));
	k1 = tmp[0] * tmp[0] - cos(taN) * cos(taN);
	k2 = 2.0 * ((tmp[0] * tmp[1]) - v_dotproduct(d, l) * pow(cos(taN), 2));
	k3 = pow(v_dotproduct(l, v_normalize(cone->n)), 2) - v_dotproduct(l, l) * pow(cos(taN), 2);
	discriminant = k2 * k2 - 4.0 * k1 * k3;
	if (discriminant < 0)
		return (-1);
	t1 = (-k2 + sqrtf(discriminant)) / (2 * k1);
	t2 = INFINITY;//(-k2 - sqrtf(discriminant)) / (2 * k1);
	return (fminf(t1, t2));
}

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
	t1 = (-k2 + sqrtf(discriminant)) / (2 * k1);
	t2 = (-k2 - sqrtf(discriminant)) / (2 * k1);
	min = fminf(t1, t2);
	if (min > min_t && min < max_t)
		return (min);
	else
		return (-1);
	
}

float	intersect_cylinder(t_vec3df raypos, t_vec3df d, t_cylinder *cyn)
{
	t_vec3df	oc;
	double		k1;
	double		k2;
	double		k3;
	double		t1;
	double		t2;
	double		discriminant;
	double		min;
	double		dot1;
	double		dot2;
	
	oc = v_sub(raypos, cyn->pos);
	dot1 = v_dotproduct(d, v_normalize(cyn->n));
	dot2 = v_dotproduct(oc, cyn->n);
	k1 = v_dotproduct(d, d) - pow(dot1, 2);
	k2 = 2 * (v_dotproduct(d, oc) - (dot1 * dot2));
	k3 = v_dotproduct(oc, oc) - pow(dot2, 2) - pow(cyn->r, 2);

	discriminant = k2 * k2 - 4.0 * k1 * k3;
	if (discriminant < 0)
		return (-1);
	t1 = (-k2 + sqrtf(discriminant)) / (2 * k1);
	t2 = (-k2 - sqrtf(discriminant)) / ( 2 * k1);
	min = fminf(t1, t2);
	return (min);
}

float		intersect_plane(t_vec3df raypos, t_vec3df d, t_plane *pl)
{
	double		denom;
	double		t;
	t_vec3df	diff;
	
	denom = v_dotproduct(pl->n, d);
	if ((fabs(denom)) > 0.001)
	{
		diff = v_sub(pl->pos, raypos);
		t = v_dotproduct(diff, pl->n) / denom;
		if (t > 0.001)
			return (t);
	}
	return (-1);
}

t_rgb	compute_light(t_param *p, t_vec3df phit, t_vec3df nhit, t_obj *closest)
{
	t_light		*tmp;
	t_rgb		intensity;
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
	intensity = (t_rgb){0, 0, 0, 0};
	vec_l = (t_vec3df){0, 0, 0};
	dot = 0;
	t = 0;
	while (tmp)
	{
		if (tmp->type == 1)
			intensity = add_color(intensity, mulk_color(tmp->color, tmp->intensity));
		else if (tmp->type == 2)
			vec_l = v_sub(tmp->pos, phit);
		else if (tmp->type == 3)
			vec_l = tmp->pos;
		double didis = v_length(vec_l);
		vec_l = v_normalize(vec_l);
		tmp1 = p->obj;
		int		hit;
		hit = 0;
		while (tmp1)
		{
			if (tmp1->type == 1)
				t = intersect_sphere(phit, vec_l, ((t_sphere*)tmp1->data), 0, INFINITY);
			if (tmp1->type == 2)
				t = intersect_plane(phit, vec_l, ((t_plane*)tmp1->data));
			if (tmp1->type == 3)
				t = intersect_cylinder(phit, vec_l, ((t_cylinder*)tmp1->data));
			if (tmp1->type == 4)
				t = intersect_cone(phit, vec_l, ((t_cone*)tmp1->data));
			if (t > 0.001 && t < didis)
			{
				hit = 1;
				break ;
			}
			tmp1 = tmp1->next;
		}
		if (hit == 0)
		{
			dot = v_dotproduct(nhit, vec_l);
			if (dot > 0)
				intensity = add_color(intensity, mulk_color(tmp->color, tmp->intensity * dot));
			if (closest->tex.spe != -1)
			{
				vec_r = v_sub(v_mulk(nhit, (2.0 * v_dotproduct(nhit, vec_l))), vec_l);
				r_dot_v = v_dotproduct(vec_r, p->cam.pos);
				if (r_dot_v > 0)
				{
					//intensity = addk_color(intensity, tmp->intensity * pow(r_dot_v / (v_length(vec_r) * v_length(p->cam.pos)), closest->tex.spe));
					intensity.r += tmp->color.r * pow(r_dot_v / (v_length(vec_r) * v_length(p->cam.pos)), closest->tex.spe);
					intensity.g += tmp->color.g * pow(r_dot_v / (v_length(vec_r) * v_length(p->cam.pos)), closest->tex.spe);
					intensity.b += tmp->color.b * pow(r_dot_v / (v_length(vec_r) * v_length(p->cam.pos)), closest->tex.spe);
				}
			}
		}
		tmp = tmp->next;
	}
	return (intensity);
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
			t = intersect_plane(p->cam.pos, d, ((t_plane*)tmp->data));
		if (tmp->type == 3)
			t = intersect_cylinder(p->cam.pos, d, ((t_cylinder*)tmp->data));
		if (tmp->type == 4)
			t = intersect_cone(p->cam.pos, d, ((t_cone*)tmp->data));
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
				nhit = ((((t_plane*)closest->data)->n));
				if (v_dotproduct(d, ((t_plane*)closest->data)->n) > 0)
					nhit = (v_mulk(nhit, -1));
			}
			if (closest->type == 3)
			{
				nhit = v_sub(phit, ((t_cylinder*)closest->data)->pos);
				nhit = v_add(((t_cylinder*)closest->data)->pos, v_mulk(((t_cylinder*)closest->data)->n, v_dotproduct(nhit, ((t_cylinder*)closest->data)->n)));
				nhit = v_normalize(v_sub(phit, nhit));
			}
			if (closest->type == 4)
			{
				nhit = v_sub(phit, ((t_cone*)closest->data)->pos);
				nhit = v_add(((t_cone*)closest->data)->pos, v_mulk(v_normalize(((t_cone*)closest->data)->n), v_dotproduct(nhit, v_normalize(((t_cone*)closest->data)->n))));
				nhit = v_normalize(v_sub(phit, nhit));
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
		p->color = mult_color(closest->tex.color, compute_light(p, phit, nhit, closest));
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

void		*ray_tracing(void *pvoid)
{
	int			x;
	int			y;
	t_vec3df	d;
	float		ratio;
	t_param		*p;

	p = (t_param *)pvoid;
	ratio = WIDTH / (float)HEIGHT;
	x = p->th_i;
	while (x < p->th_imax)
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
	return (p);
}
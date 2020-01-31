#include "rtv1.h"

void		sphere_push(t_sphere **list, t_sphere sp, t_param *p)
{
	t_sphere *new;

	if (!(new = malloc(sizeof(t_sphere))))
		SDL_Error(-1);
	new->pos = sp.pos;
	new->radius = sp.radius;
	new->color = sp.color;
	new->spe = sp.spe;
	new->next = !*list ? NULL : *list;
	*list = new;
}

void		light_push(t_light **list, t_light light, t_param *p)
{
	t_light *tmp;
	t_light *new;

	if (!(new = malloc(sizeof(t_light))))
		SDL_Error(-1);
	new->pos = light.pos;
	new->color = light.color;
	new->intensity = light.intensity;
	new->type = light.type;
	new->next = !*list ? NULL : *list;
	*list = new;
}
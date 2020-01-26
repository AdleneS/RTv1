#include "rtv1.h"

void		sphere_pushback(t_sphere **list, t_sphere sp, t_param *p)
{
	t_sphere *tmp;
	t_sphere *new;

	if (!(new = malloc(sizeof(t_sphere))))
		SDL_Error(-1);
	new->pos = sp.pos;
	new->radius = sp.radius;
	new->color = sp.color;
	new->next = NULL;
	if (!*list)
		*list = new;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}
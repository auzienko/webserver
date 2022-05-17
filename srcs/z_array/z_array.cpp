/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   z_array.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcris <zcris@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 10:20:40 by zcris             #+#    #+#             */
/*   Updated: 2022/05/17 17:19:04 by zcris            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "z_array.hpp"

int	z_array_init(t_z_array *a)
{
	(*a).size = 0;
	(*a).elem = NULL;
	(*a).elem = (char**)z_array_utls_ft_alloc(sizeof(char **));
	if (NULL == (*a).elem)
	{
		(*a).status = 1;
		return (1);
	}
	(*a).status = 0;
	return (0);
}

int	z_array_append(t_z_array *a, char *str)
{
	char		**temp;
	char		*tstr;
	int			i;

	if ((*a).status != 0)
		return (1);
	i = 0;
	tstr = z_array_utls_ft_strdup(str);
	if (NULL == tstr)
		return (1);
	temp = (char**)z_array_utls_ft_alloc(sizeof(char *) * ((*a).size + 1));
	if (NULL == temp)
	{
		free(tstr);
		return (1);
	}
	*(temp + (*a).size) = tstr;
	i = (*a).size;
	while (i-- != 0)
		temp[i] = (*a).elem[i];
	free((*a).elem);
	(*a).elem = temp;
	(*a).size += 1;
	return (0);
}

int	z_array_remove(t_z_array *a, size_t index)
{
	char	*backup;

	if ((*a).status != 0)
		return (1);
	if (index + 1 > (*a).size || (*a).size == 0)
		return (1);
	backup = (*a).elem[index];
	if (index + 1 != (*a).size)
	{
		size_t i = index;
		while (i < (*a).size - 1)
		{
			(*a).elem[i] = (*a).elem[i + 1];
			i++;
		}
	}
	(*a).size -= 1;
	free(backup);
	(*a).elem[(*a).size] = NULL;
	return (0);
}

int	z_array_null_terminate(t_z_array *a)
{
	if ((*a).status != 0)
		return (1);
	z_array_append(a, (char *)"N");
	z_array_remove(a, (*a).size - 1);
	return (0);
}

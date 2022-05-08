/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   z_array_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wgaunt <wgaunt@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 08:51:11 by zcris             #+#    #+#             */
/*   Updated: 2022/05/08 15:34:22 by wgaunt           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "z_array.hpp"

void	*z_array_utls_ft_alloc(size_t size)
{
	void	*res;

	res = NULL;
	res = (void *)malloc(size);
	if (!res)
	{
		if (Z_ARRAY_EXIT_IF_NULL)
		{
			write(2, "〰✖️: Heap memory error. Out of memory.\n", 40);
			exit(12);
		}
	}
	return ((void *)res);
}

char	*z_array_utls_ft_strdup(char *s1)
{
	char	*res;
	int		i;
	int		j;

	i = 0;
	while (s1[i++])
		;
	res = NULL;
	res = (char *)z_array_utls_ft_alloc(sizeof(char) * i + 1);
	if (!res)
		return (NULL);
	j = 0;
	while (s1[j])
	{
		res[j] = s1[j];
		j++;
	}
	res[j] = '\0';
	return (res);
}

int	z_array_sort(t_z_array *a, int (f)(const char *, const char *))
{
	char	*tmp;
	size_t	i;
	size_t	j;

	if ((*a).size < 2)
		return (0);
	i = 0;
	while (i < (*a).size - 1)
	{
		j = i + 1;
		while (j && j != (*a).size)
		{
			if (j - 1 == 0 && f((*a).elem[j], (*a).elem[0]) >= 0)
				break ;
			if (f((*a).elem[j], (*a).elem[j - 1]) >= 0)
				break ;
			tmp = (*a).elem[j - 1];
			(*a).elem[j - 1] = (*a).elem[j];
			(*a).elem[j] = tmp;
			j--;
		}
		i++;
	}
	return (0);
}

int	z_array_free(t_z_array *a)
{
	int	i;

	i = a->size;
	while (--i >= 0)
	{
		free((*a).elem[i]);
		(*a).elem[i] = NULL;
	}
	a->size = 0;
	free((*a).elem);
	(*a).elem = NULL;
	return (0);
}

void	z_array_re_init(t_z_array *a)
{
	z_array_free(a);
	z_array_init(a);
}

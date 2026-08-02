/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:10:13 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/04 14:25:13 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*allocated;
	size_t	total;

	total = count * size;
	allocated = malloc(total);
	if (!allocated)
	{
		return (NULL);
	}
	ft_memset(allocated, 0, total);
	return (allocated);
}
/* int main()
{
	int *test = ft_calloc(4, 5);
	int i = 0;
	while(i < 20)
	{
		printf("%i", test[i++]);
	}
}*/
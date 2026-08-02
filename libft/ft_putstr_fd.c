/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:58:49 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/05 20:09:54 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	buffer;

	if (!s)
		return ;
	buffer = ft_strlen(s);
	write(fd, s, buffer);
}
/* int	main(void)
{
	ft_putstr_fd("jacare", 1);
} */

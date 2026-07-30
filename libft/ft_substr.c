/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:08:08 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/04 16:48:01 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*result;
	size_t	slen;
	size_t	i;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (start >= slen)
		len = 0;
	else if (len > slen - start)
		len = slen - start;
	result = malloc((len + 1));
	if (!result)
	{
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		result[i] = s[start + i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

/* int	main(void)
{
	char	*s1;
	char	*s2;

	s1 = "fox mulder";
	s2 = ft_substr(s1, 4, 16);
	printf("string: %s", s2);
	free(s2);
} */

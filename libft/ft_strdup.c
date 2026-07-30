/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strdup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 14:26:07 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/04 15:06:29 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*s2;
	int		i;

	s2 = malloc((ft_strlen(s1) + 1) * sizeof(char));
	if (!s2)
	{
		return (NULL);
	}
	i = 0;
	while (s1[i])
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}
/* int main()
{
	char *s1 = "malokeyro";
	char *s2 = "";
	//char *s3 = NULL;
	char *s4 = "   ";

	char *ss1 = ft_strdup(s1);
	char *ss2 = ft_strdup(s2);
	//char *ss3 = strdup(s3);
	char *ss4 = ft_strdup(s4);

	printf("%s: %s\n", s1, ss1);
	printf("%s: %s\n", s2, ss2);
	//printf("%s: %s\n", s3, ss3);
	printf("%s: %s\n", s4, ss4);
} */

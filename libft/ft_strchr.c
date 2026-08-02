/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 20:27:45 by fpedroso          #+#    #+#             */
/*   Updated: 2024/10/30 21:04:06 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*ptr;
	char	ch;

	ptr = (char *)s;
	ch = (char)c;
	while (*ptr)
	{
		if (*ptr == ch)
		{
			return (ptr);
		}
		ptr++;
	}
	if (ch == '\0')
	{
		return (ptr);
	}
	return (NULL);
}

/* int main(void)
{
	const char	*str;
	char		ch;
	char		*result;

	str = "Hello, zworld!";
	ch = 'o';
	// Test case 1: Character is in the string
	result = ft_strchr(str, ch);
	if (result)
		printf("Character '%c' found at position: %ld\n", ch, result - str);
	else
		printf("Character '%c' not found\n", ch);
	// Test case 2: Character is not in the string
	ch = 'z';
	result = ft_strchr(str, ch);
	if (result)
		printf("Character '%c' found at position: %ld\n", ch, result - str);
	else
		printf("Character '%c' not found\n", ch);
	// Test case 3: Null character
	ch = '\0';
	result = ft_strchr(str, ch);
	if (result)
		printf("Null character found at position: %ld\n", result - str);
	else
		printf("Null character not found\n");
	return (0);
}
 */
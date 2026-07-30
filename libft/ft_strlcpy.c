/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:47:29 by fpedroso          #+#    #+#             */
/*   Updated: 2024/10/30 13:26:37 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	len;
	size_t	amount;

	amount = 0;
	len = ft_strlen(src);
	if (dstsize > 0)
	{
		if (len < dstsize - 1)
		{
			amount = len;
		}
		else
		{
			amount = dstsize - 1;
		}
		ft_memcpy(dst, src, amount);
		dst[amount] = '\0';
	}
	return (len);
}
/* int main(void)
{
    char src1[] = "Hello, World!";
    char dst1[20]; // Enough space
    char dst2[10]; // Not enough space
    char dst3[0];  // Zero size
    size_t result;

    // Test 1: Normal case with enough space
    result = ft_strlcpy(dst1, src1, sizeof(dst1));
    printf("Test 1 - Result: %zu, Destination: '%s'\n", result, dst1);

    // Test 2: Case where dstsize is less than the source length
    result = ft_strlcpy(dst2, src1, sizeof(dst2));
    printf("Test 2 - Result: %zu, Destination: '%s'\n", result, dst2);

    // Test 3: Case where dstsize is zero
    result = ft_strlcpy(dst3, src1, sizeof(dst3));
    printf("Test 3 - Result: %zu, Destination: '%s' (Should be empty)\n",
		result, dst3);

    // Test 4: Copying an empty string
    char emptySrc[] = "";
    char dst4[20];
    result = ft_strlcpy(dst4, emptySrc, sizeof(dst4));
    printf("Test 4 - Result: %zu, Destination: '%s'\n", result, dst4);

    // Test 5: Exact length match (dstsize == length of src)
    char exactSrc[] = "123456789";
    char dst5[10];
    result = ft_strlcpy(dst5, exactSrc, sizeof(dst5));
    printf("Test 5 - Result: %zu, Destination: '%s'\n", result, dst5);

    return (0);
} */
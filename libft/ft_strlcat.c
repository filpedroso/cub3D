/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:05:20 by fpedroso          #+#    #+#             */
/*   Updated: 2024/10/30 14:42:03 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	d_len;
	size_t	s_len;

	d_len = ft_strlen(dst);
	s_len = ft_strlen(src);
	if (d_len >= dstsize)
	{
		return (dstsize + s_len);
	}
	ft_strlcpy(&dst[d_len], src, dstsize - d_len);
	return (d_len + s_len);
}
/* int main()
{
	char dest[20] = "meu ";
	char *src = "dongo";
	size_t size = 10;
	ft_strlcat(dest, src, size);
	printf("%s", dest);
} */

/* int main(void)
{
    char dst1[20] = "Hello";
    const char *src1 = " World";
    size_t size1 = sizeof(dst1);
    
    size_t ret1 = ft_strlcat(dst1, src1, size1);
    printf("Test 1:\n");
    printf("Result: %zu, dst: %s\n\n", ret1, dst1);

    char dst2[10] = "Foo";
    const char *src2 = "BarBaz";
    size_t size2 = sizeof(dst2);
    
    size_t ret2 = ft_strlcat(dst2, src2, size2);
    printf("Test 2:\n");
    printf("Result: %zu, dst: %s\n\n", ret2, dst2);

    char dst3[15] = "";
    const char *src3 = "EmptyDst";
    size_t size3 = sizeof(dst3);
    
    size_t ret3 = ft_strlcat(dst3, src3, size3);
    printf("Test 3:\n");
    printf("Result: %zu, dst: %s\n\n", ret3, dst3);

    char dst4[5] = "1234";
    const char *src4 = "5678";
    size_t size4 = sizeof(dst4);
    
    size_t ret4 = ft_strlcat(dst4, src4, size4);
    printf("Test 4:\n");
    printf("Result: %zu, dst: %s\n\n", ret4, dst4);

    return (0);
} */
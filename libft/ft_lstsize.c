/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:36:02 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/07 15:30:44 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		len;
	t_list	*ptr;

	if (!lst)
		return (0);
	ptr = lst;
	len = 0;
	while (ptr)
	{
		len++;
		ptr = ptr->next;
	}
	return (len);
}

/* int main()
{
	int i = 0;
	t_list *lst = ft_lstnew(&i);
	t_list *new;

	while (i < 10)
	{
		new = ft_lstnew(&i);
		ft_lstadd_front(&lst, new);
		i++;
	}
	int len = ft_lstsize(lst);
	printf("%i", len);
} */
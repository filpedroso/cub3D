/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:02:49 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/07 13:20:01 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}
/* int main()
{
	int i = 42;
	int j = 43;
	t_list *lst = ft_lstnew(&i);
	t_list *new = ft_lstnew(&j);

	ft_lstadd_front(&lst, new);
printf("%i %i", *((int *)new->content), *((int *)new->next->content));
} */

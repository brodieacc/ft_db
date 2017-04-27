/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   query.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschroed <bschroed@student.42.us.org       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/26 20:39:47 by bschroed          #+#    #+#             */
/*   Updated: 2017/04/26 20:39:48 by bschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUERY_H
# define QUERY_H

typedef char*	(*t_command)(char*);
typedef int		(*t_cmp)(char*, char*);

typedef struct		s_list
{
	int				index;
	struct s_list	*next;
}					t_list;

void init_first();
void init_second();
void init_third();
void init_fourth();
void init_operators();
t_list	*ft_lstnew(int index);
void	ft_lstadd(t_list **alst, t_list *new);
void ft_listaddback(t_list **alst, t_list *new);
int cmp_equals(char *s1, char *s2);
int cmp_less_than(char *s1, char *s2);
int cmp_greater_than(char *s1, char *s2);
void init_cmp_funct(t_cmp *cmp);
t_cmp get_cmp_funct(int index);
int validate_command(char *first);
int validate_column(char *second);
int validate_from(char *third);
int validate_table(char *fourth);
int validate_operator(char *third);
int validate_equal(char *input);
int validate_into(char *input);
int validate_set(char *input);
int validate_values(char *input);
int validate_where(char *third);
char *return_data(void);
int search_table(void);
void parse_from_table(void);
void where(char *input);
void from(char *input);
void parse_values(char *input);
char *insert_data_to_table(void);
void set(char *input);
char *update_data_to_table(void);
char *update(char *input);
char *insert(char *input);
char *delete(char *input);
char *query(char *input);
void init_command_funct(t_command *command);
t_command get_command_funct(int index);
char *query_validation(char *argv);
void	error(char *msg);

#endif

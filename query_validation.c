#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "query.h"
// #include <conio.h>
# define FIRST_STRING_LIMIT 4
# define SECOND_STRING_LIMIT 5
# define THIRD_STRING_LIMIT 6
# define FOURTH_STRING_LIMIT 1
# define OPERATOR_STRING_LIMIT 3
# define eos(s) ((s)+strlen(s))

t_list *users;
int operator;
int MAX_USERS;
int MAX_COLUMNS;
int LAST_INDEX;
char **valid_first;
char **valid_second;
char **valid_third;
char **valid_fourth;
char **valid_operator;
char **valid_where;
int ret_value;
int table_name;
int column;
int update_column;
char *update_value;
int user_index;
char *value;
char **new_data;
char ***file_data;

void init_first()
{
	valid_first = malloc(sizeof(char*) * 4);
	valid_first[0] = strdup("SELECT");
	valid_first[1] = strdup("DELETE");
	valid_first[2] = strdup("INSERT");
	valid_first[3] = strdup("UPDATE");
}

void init_second()
{
	valid_second = malloc(sizeof(char*) * 5);
	valid_second[0] = strdup("*");
	valid_second[1] = strdup("ID");
	valid_second[2] = strdup("F_NAME");
	valid_second[3] = strdup("L_NAME");
	valid_second[4] = strdup("AGE");
}

void init_third()
{
	valid_third = malloc(sizeof(char*) * 6);
	valid_third[0] = strdup("FROM");
	valid_third[1] = strdup("WHERE");
	valid_third[2] = strdup("=");
	valid_third[3] = strdup("INTO");
	valid_third[4] = strdup("VALUES");
	valid_third[5] = strdup("SET");
}

void init_fourth()
{
	valid_fourth = malloc(sizeof(char*) * 1);
	valid_fourth[0] = strdup("USERS");
}

void init_operators()
{
	valid_operator = malloc(sizeof(char*) * 3);
	valid_operator[0] = strdup("=");
	valid_operator[1] = strdup("<");
	valid_operator[2] = strdup(">");
}
t_list	*ft_lstnew(int index)
{
	t_list	*link;

	if (!(link = (t_list *)malloc(sizeof(t_list))))
		return (NULL);
	link->index = index;
	link->next = NULL;
	return (link);
}

void	ft_lstadd(t_list **alst, t_list *new)
{
	new->next = *alst;
	*alst = new;
}

void ft_listaddback(t_list **alst, t_list *new)
{
	t_list *node;

	if (!*alst)
	{
		ft_lstadd(alst, new);
		return ;
	}
	node = *alst;
	while (node->next)
		node = node->next;
	node->next = new;
}

int cmp_equals(char *s1, char *s2)
{
	if (strcasecmp(s1, s2) == 0)
		return (1);
	return (0);
}

int cmp_less_than(char *s1, char *s2)
{
	if (strcasecmp(s1, s2) < 0)
		return (1);
	return (0);
}

int cmp_greater_than(char *s1, char *s2)
{
	if (strcasecmp(s1, s2) > 0)
		return (1);
	return (0);
}
void init_cmp_funct(t_cmp *cmp)
{
	cmp[0] = &cmp_equals;
	cmp[1] = &cmp_less_than;
	cmp[2] = &cmp_greater_than;
}

t_cmp get_cmp_funct(int index)
{
	static t_cmp *cmp = NULL;

	if (cmp == NULL)
	{
		cmp = malloc(sizeof(*cmp) * 4);
		if (cmp != NULL)
			init_cmp_funct(cmp);
	}
	return (cmp[index]);
}

int validate_command(char *first)
{
	int i;

	i = 0;
	while (i < FIRST_STRING_LIMIT)
	{
		if (strcasecmp(first, valid_first[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int validate_column(char *second)
{
	int i;

	i = 0;
	while (i < SECOND_STRING_LIMIT)
	{
		if (strcasecmp(second, valid_second[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int validate_from(char *third)
{
	int i;

	i = 0;
	while (i < THIRD_STRING_LIMIT)
	{
		if (strcasecmp(third, valid_third[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int validate_table(char *fourth)
{
	int i;

	i = 0;
	while (i < FOURTH_STRING_LIMIT)
	{
		if (strcasecmp(fourth, valid_fourth[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int validate_operator(char *third)
{
	int i;

	i = -1;
	while (++i < OPERATOR_STRING_LIMIT)
	{
		if (strcasecmp(third, valid_operator[i]) == 0)
		return (i);
	}
	return (-1);
}

int validate_equal(char *input)
{
	if (strcasecmp(input, valid_operator[0]) == 0)
		return (1);
	return (0);
}

int validate_into(char *input)
{
	if (strcasecmp(input, valid_third[3]) == 0)
		return (1);
	return (0);
}

int validate_set(char *input)
{
	if (strcasecmp(input, valid_third[5]) == 0)
		return (1);
	return (0);
}

int validate_values(char *input)
{
	if (strcasecmp(input, valid_third[4]) == 0)
		return (1);
	return (0);
}

int validate_where(char *third)
{
	int i;

	i = 1;
	while (i < THIRD_STRING_LIMIT + 1)
	{
		if (strcasecmp(third, valid_third[i]) == 0)
			return (i + 1);
		i++;
	}
	return (0);
}

char *return_data(void)
{
	int i;
	char buf[4096];

	bzero(buf, sizeof(char) * 4096);
	if (column == -1)
	{
		i = -1;
		if (ret_value == 0)
			while (++i < MAX_USERS)
				sprintf(eos(buf), "%s %s %s %s\n", file_data[i][0], file_data[i][1],
					file_data[i][2], file_data[i][3]);
		else
			while (++i < MAX_USERS)
				sprintf(eos(buf), "%s\n", file_data[i][ret_value - 1]);
	}
	else if (ret_value == 0)
	{
		while (users)
		{
			sprintf(eos(buf), "%s %s %s %s\n", file_data[users->index][0], file_data[users->index][1],
				file_data[users->index][2], file_data[users->index][3]);
			users = users->next;
		}
	}
	else
	{
		while (users)
		{
			sprintf(eos(buf), "%s\n", file_data[users->index][ret_value - 1]);
			users = users->next;
		}
	}
	return (strdup(buf));
}

int search_table(void)
{
	int i;
	int ret;
	t_cmp cmp;

	ret = 0;
	i = 0;
	cmp = get_cmp_funct(operator);
	while (i < MAX_USERS)
	{
		if (cmp(file_data[i][column], value))
		{
			ft_listaddback(&users, ft_lstnew(i));
			ret = 1;
		}
		i++;
	}
	return (ret);
}

void parse_from_table(void)
{
	FILE *fp;
	char str[256];
	char *tmp;
	int i;
	int j;

	j = -1;
	fp = fopen(valid_fourth[table_name], "r");
	if (fp == NULL)
	{
		perror("Failure to open input file.");
		exit(EXIT_FAILURE);
	}
	fscanf(fp, "%s\n", str);
	MAX_USERS = atoi(strtok(str, ","));
	LAST_INDEX = atoi(strtok(NULL, ","));
	MAX_COLUMNS = atoi(strtok(NULL, ","));
	file_data = (char***)malloc(sizeof(char**) * MAX_USERS);
	while (!feof(fp))
	{
		fscanf(fp, "%s\n", str);
		tmp = strtok(str, ",");
		i = -1;
		file_data[++j] = (char**)malloc(sizeof(char*) * MAX_COLUMNS);
		while(tmp != NULL)
		{
			file_data[j][++i] = strdup(tmp);
			tmp = strtok(NULL, ",");
		}
		bzero(str, sizeof(char) * 256);
	}
	fclose(fp);
}

void where(char *input)
{
	if (!validate_where(input))
		error("Invalid WHERE");
	else
	{
		input = strtok(NULL, " ");
		column = validate_column(input) - 1;
		if (column < 0)
			error("Invalid Column Value after WHERE");
		else
		{
			input = strtok(NULL, " ");
			operator = validate_operator(input);
			if (operator == -1)
				error("Invalid Equals");
			else
			{
				input = strtok(NULL, " ");
				value = strdup(input);
			}
		}
	}
}

void from(char *input)
{
	if (validate_from(input) == -1)
	{
		error("Invalid FROM");
		error(input);
	}
	else
	{
		input = strtok(NULL, " \n");
		table_name = validate_table(input);
		if (table_name == -1)
			error("Invalid TABLE");
		else
		{
			input = strtok(NULL, " ");
			if (input != NULL)
				where(input);
			else
				column = -1;
		}
	}
}

void parse_values(char *input)
{
	int i;

	new_data = (char**)malloc(sizeof(char*) * 16);
	i = -1;
	while(input != NULL)
	{
		new_data[++i] = strdup(input);
		input = strtok(NULL, " ");
	}
}

char *insert_data_to_table(void)
{
	FILE *fp;
	int i;
	char buf[4096];

	fp = fopen(valid_fourth[table_name], "w");
	if (fp == NULL)
	{
		perror("Failure to open input file.");
		exit(EXIT_FAILURE);
	}
	fprintf(fp, "%d,%d,%d\n", MAX_USERS + 1, LAST_INDEX + 1, MAX_COLUMNS);
	i = -1;
	while (++i < MAX_USERS)
		fprintf(fp, "%s,%s,%s,%s\n", file_data[i][0], file_data[i][1],
					file_data[i][2], file_data[i][3]);
	fprintf(fp, "%d,%s,%s,%s\n", LAST_INDEX + 1, new_data[0], new_data[1],
		new_data[2]);
	fclose(fp);
	sprintf (eos(buf), "INSERTED ID=%d F_NAME=%s L_NAME=%s AGE=%s\n",
		LAST_INDEX + 1, new_data[0], new_data[1], new_data[2]);
	return (strdup(buf));
}

void set(char *input)
{
	if (!validate_set(input))
		error("Invalid SET");
	else
	{
		input = strtok(NULL, " ");
		update_column = validate_column(input) - 1;
		if (column < 0)
			error("Invalid Column Value after SET");
		else
		{
			input = strtok(NULL, " ");
			if (!validate_equal(input))
				error("Invalid Equals");
			else
			{
				input = strtok(NULL, " ");
				update_value = strdup(input);
				input = strtok(NULL, " ");
				where(input);
			}
		}
	}
}

char *update_data_to_table(void)
{
	FILE *fp;
	int i;
	char buf[4096];

	while (users)
	{
		file_data[users->index][update_column] = strdup(update_value);
		users = users->next;
	}
	fp = fopen(valid_fourth[table_name], "w");
	fprintf(fp, "%d,%d,%d\n", MAX_USERS, LAST_INDEX, MAX_COLUMNS);
	i = -1;
	while (++i < MAX_USERS)
		fprintf(fp, "%s,%s,%s,%s\n", file_data[i][0], file_data[i][1],
					file_data[i][2], file_data[i][3]);
	fclose(fp);
	sprintf(eos(buf), "UPDATED %s = %s\n", valid_second[update_column + 1], update_value);
	return (strdup(buf));
}

char *update(char *input)
{
	table_name = validate_table(input);
	if (table_name == -1)
		return ("Invalid TABLE\n");
	else
	{
		input = strtok(NULL, " ");
		set(input);
		parse_from_table();
		if (!search_table())
			return ("User not found\n");
		else
			return (update_data_to_table());
	}
}

char *insert(char *input)
{
	if (!validate_into(input))
		return ("Invalid INSERT\n");
	else
	{
		input = strtok(NULL, " ");
		table_name = validate_table(input);
		if (table_name == -1)
			return ("Invalid TABLE\n");
		else
		{
			input = strtok(NULL, " ");
			if (!validate_values(input))
				return ("Invalid VALUES\n");
			else
			{
				input = strtok(NULL, " ");
				parse_values(input);
				parse_from_table();
				return (insert_data_to_table());
			}
		}
	}
}
char *delete(char *input)
{
	FILE *fp;
	int i;
	char buf[4096];

	from(input);
	parse_from_table();
	if (!search_table())
		return ("User not found\n");
	else
	{
		fp = fopen(valid_fourth[table_name], "w");
		fprintf(fp, "%d,%d,%d\n", MAX_USERS - 1, LAST_INDEX, MAX_COLUMNS);
		while (users)
		{
			file_data[users->index][0] = strdup("-1");
			users = users->next;
		}
		i = -1;
		while (++i < MAX_USERS)
		{
			if (strcasecmp(file_data[i][0], "-1"))
				fprintf(fp, "%s,%s,%s,%s\n", file_data[i][0], file_data[i][1],
					file_data[i][2], file_data[i][3]);
		}
		fclose(fp);
		sprintf (eos(buf), "DELETED %s = %s\n", valid_second[column + 1], value);
		return (strdup(buf));
	}
}

char *query(char *input)
{
	ret_value = validate_column(input);
	if (ret_value != -1)
	{
		input = strtok(NULL, " ");
		from(input);
		parse_from_table();
		if (column != -1 && !search_table())
			return ("User not found\n");
		else
			return (return_data());
	}
	else
		return ("Invalid Column Entry\n");
}

void init_command_funct(t_command *command)
{
	command[0] = &query;
	command[1] = &delete;
	command[2] = &insert;
	command[3] = &update;
}

t_command get_command_funct(int index)
{
	static t_command *command = NULL;

	if (command == NULL)
	{
		command = malloc(sizeof(*command) * 4);
		if (command != NULL)
			init_command_funct(command);
	}
	return (command[index]);
}

char *query_validation(char *argv)
{
	int i;
	int command_index;
	char *temp;
	t_command command;

	i = 0;
	temp = strtok(argv, " ");
	command_index = validate_command(temp);
	if (command_index != -1)
	{
		command = get_command_funct(command_index);
		temp = strtok(NULL, " ");
		return(command(temp));
	}
	return ("Error on query Validation\n");
}

// int main(int argc, char **argv)
// {
// 	if (argc ==  2)
// 	{
// 		init_first();
// 		init_second();
// 		init_third();
// 		init_fourth();
// 		init_operators();
// 		printf("%s", query_validation(argv[1]));
// 	}
// 	else
// 		printf("Invalid Nuumber of Arguements %s\n", argv[2]);
// 	return (1);
// }

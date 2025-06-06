/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 16:01:21 by odudniak          #+#    #+#             */
/*   Updated: 2025/05/24 16:01:44 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hotrace.h"
#include <time.h>
#include <stdio.h>

static t_entry	*new_entry(t_key key, char *value)
{
	t_entry	*entry;

	entry = malloc(sizeof(t_entry));
	entry->key = key.key;
	entry->key_str = key.str;
	entry->value = value;
	entry->value_len = ft_strlen(value);
	return (entry);
}

static void	insert(t_data *data, t_key key, char *value)
{
	int		i;
	size_t	j;

	i = 0;
	if (data->inserted > data->hashtable_size * 0.69)
		expand_hashtable(data);
	while (i < data->hashtable_size)
	{
		j = get_hash(data->hashtable_size, key.key, i);
		if (data->hashtable[j] == NULL)
		{
			data->hashtable[j] = new_entry(key, value);
			data->inserted++;
			return ;
		}
		i++;
	}
}

static int	search(t_data *data, char *key_str, size_t key)
{
	int		i;
	size_t	j;

	i = 0;
	j = get_hash(data->hashtable_size, key, i);
	while (i < data->hashtable_size && data->hashtable[j] != NULL)
	{
		if (data->hashtable[j]->key == key
			&& ft_strcmp(data->hashtable[j]->key_str, key_str) == 0)
		{
			write(1, data->hashtable[j]->value, data->hashtable[j]->value_len);
			return (write(1, "\n", 1));
		}
		i++;
		j = get_hash(data->hashtable_size, key, i);
	}
	write(1, key_str, ft_strlen(key_str));
	write(1, ": Not found.\n", 13);
	return (0);
}

static void	init(t_data *data, t_state *state)
{
	*state = INS_KEY;
	data->inserted = 0;
	data->hashtable_size = HASH_TABLE_SIZE;
	data->hashtable = ft_calloc(data->hashtable_size, sizeof(t_entry *));
}

/*
	// REMOVE-----------------
	struct timespec	start;
	struct timespec	end;
	double			elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);
	// REMOVE-----------------
*/
int	main(void)
{
	char			*line;
	t_state			state;
	t_key			key;
	t_data			data;

	init(&data, &state);
	line = get_next_line(0);
	while (line)
	{
		if (state == INS_KEY)
			key = (t_key){convert_to_int(line), line};
		else if (state == INS_VALUE)
			insert(&data, key, line);
		else if (state == SEARCH)
			search(&data, line, convert_to_int(line));
		if (state == SEARCH)
			free(line);
		if (state == INS_KEY && line[0] == '\0')
			state = SEARCH;
		if (state != SEARCH)
			state = (state + 1) % SEARCH;
		line = get_next_line(0);
	}
	free_all(&data);
}
/*
	// REMOVE-----------------
	clock_gettime(CLOCK_MONOTONIC, &end);
	elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
	printf("Elapsed time: %.9f seconds\n", elapsed);
	// REMOVE-----------------
*/

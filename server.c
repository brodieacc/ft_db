/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschroed <bschroed@student.42.us.org       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/23 00:22:58 by bschroed          #+#    #+#             */
/*   Updated: 2017/04/23 00:26:20 by bschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void	error(char *msg)
{
	perror(msg);
	exit(1);
}

void dostuff(int sock)
{
	int n;
	char buffer[256];

	bzero(buffer, 256);
	n = read(sock, buffer, 255);
	if (n < 0)
		error("ERROR reading from socket");
	printf("Here is the message: %s\n", buffer);
	n = write(sock, "I got your message", 18);
	if (n < 0)
		error("ERROR writing to socket");
}

int		main(int argc, char *argv[])
{
	int	sockfd;
	int	newsockfd;
	int	portno;
	unsigned int	clilen;
	int	pid;
	struct	sockaddr_in serv_addr;
	struct	sockaddr_in cli_addr;
	if (argc < 2)
	{
		fprintf(stderr, "Error, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0)
			error("ERROR on binding");
	listen(sockfd, 128);
	clilen = sizeof(cli_addr);
	while (1)
	{
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
			error("ERROR on accept");
		pid = fork();
		if (pid < 0)
			error("ERROR on fork");
		if (pid == 0)
		{
			close(sockfd);
			dostuff(newsockfd);
			exit(0);
		}
		else
			close(newsockfd);
	}
	return (0); /* Never gets here just for semantics */
}

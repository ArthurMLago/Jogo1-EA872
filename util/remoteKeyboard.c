#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ncurses.h>

int main(){

	int socket_fd;
	struct sockaddr_in target;

	initscr();			/* Start curses mode 		  */
  
	
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	printf("Socket criado\n");

	target.sin_family = AF_INET;
	target.sin_port = htons(18259);
	inet_aton("127.0.0.1", &(target.sin_addr));
	printf("Tentando conectar\n");
	while(connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
		printf("Problemas na conexao\n");
		sleep(1);
		/*endwin();*/
		/*return 0;*/
	}
	printf("Conectei ao servidor\n");


	printw("Teclado rodando");	/* Print Hello World		  */
	int shouldQuit = 0;
	while(!shouldQuit){
		

		char c = getch();			/* Wait for user input */
		send(socket_fd, &c, 1, 0);
		printw("Teclado rodando");	/* Print Hello World		  */
		refresh();			/* Print it on to the real screen */
		if (c == 'q')
			shouldQuit = 1;
	}

	endwin();

}

#include "clientController.hpp"

// Construtor: cria socket, cria thread.
ClientController::ClientController(const char *Endereco, int porta){
    struct sockaddr_in target;
     // Cria um socket e retorna o descritor de arquivo
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    target.sin_family = AF_INET;
  	target.sin_port = htons(porta);
    inet_aton(Endereco, &(target.sin_addr));
    
    // Tenta estabelecer uma conexão com um socket
    while(connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    	//Se nao conectar repete ate conectar
    	fprintf(stderr, "Erro ao conectar!\n");
    	sleep(1);
    }

	// recebe o ID 4 bytes pela variavel serve_index através do socket 
	int var_leitura_bit = recv(socket_fd, &server_index, 4, 0);
	// Condicional que ve se foi passado 4 bytes para o socket
	if(var_leitura_bit!=4){
		if (var_leitura_bit == -1){
			fprintf(stderr, "Erro ao usar recv: %s\n", strerror(errno));
		}else{
			fprintf(stderr, "Quantidade de bytes lidos diferente do requisitado para obter id(%d != 4)!1\n", var_leitura_bit);
		}
	}
	fprintf(stderr, "Server index: %d\n", server_index);
	shouldTerminate_Aux = 0;
	// Criar thread que escuta o estado da tela
	// Recebe do servidor o estado da tela
	recebe = new std::thread(&ClientController::receive_thread, this);




}

// Funcao que monta o buffer de atulizacao da cena de acordo com o servidor
void ClientController::receive_thread(){
	// O valor foi escolhido para alinhar com a paginas de memoria do linux
	unsigned char buffer[65536];
	int i, numero_bytes, numero_jogadores, numero_inimigos;
	//
	while(shouldTerminate_Aux == 0){
		
		numero_bytes = recv(socket_fd, buffer, 4, 0);
		// Se nao se recebeu os 4 bytes temos um tratamento de erro
		if(numero_bytes != 4 ){
			shouldTerminate_Aux = 1;
			if (numero_bytes == -1){
				fprintf(stderr, "Erro ao usar recv para obter numero_byts: %s\n", strerror(errno));
			}else{
				fprintf(stderr, "Quantidade de bytes lidos diferente do requisitado(%d != 4)!1\n", numero_bytes);
			}
			continue;
		}
		// Pega 4 bytes que e o tamanho do inteiro
		int pular_bytes = *((int *)buffer);
		// Incremento para percorre o buffer com os valores de ID
		i = i + 4;
		
		
		numero_bytes = recv(socket_fd, buffer+i, pular_bytes, 0);
		// Se nao se recebeu os pular_bytes bytes temos um tratamento de erro
		if(numero_bytes != pular_bytes ){
			shouldTerminate_Aux = 1;
			fprintf(stderr, "Quantidade de bytes lidos diferente do requisitado!2\n");
			continue;
		}
		i = i + pular_bytes;

		// Le o numero de jogadores
		numero_bytes = recv(socket_fd, buffer+i, 4, 0);
		// Se nao se recebeu os 4 bytes temos um tratamento de erro
		if(numero_bytes != 4 ){
			shouldTerminate_Aux = 1;
			fprintf(stderr, "Quantidade de bytes lidos diferente do requisitado!3\n");
			continue;
		}
		// Interpreta o numero de jogadores
		int numero_jogadores = *((int *)(buffer+i));
		i = i + 4;

		// Recebe os jogadores
		numero_bytes = recv(socket_fd, buffer+i, numero_jogadores*sizeof(Player), 0);
		// Se nao se recebeu os 4 bytes temos um tratamento de erro
		if(numero_bytes != numero_jogadores*sizeof(Player)){
			shouldTerminate_Aux = 1;
			fprintf(stderr, "Quantidade de bytes lidos diferente do requisitado!4\n");
			continue;
		}
		// Interpreta o numero de jogadores
		int numero_inimigos = *((int *)(buffer+i));
		i = i + numero_jogadores*sizeof(Player);

		// Recebe os inimigos
		numero_bytes = recv(socket_fd, buffer+i, numero_inimigos*sizeof(Enemy), 0);
		// Se nao se recebeu os 4 bytes temos um tratamento de erro
		if(numero_bytes != numero_inimigos*sizeof(Enemy)){
			shouldTerminate_Aux = 1;
			fprintf(stderr, "Quantidade de bytes lidos diferente do requisitado!5\n");
			continue;
		}
		i = i + numero_inimigos*sizeof(Enemy);
	}
}

// Funcao que e chamada pelo view controle e envia para o servidor que w foi pressionado
void ClientController::userPressedUp(){
	int enviado;
	enviado = send(socket_fd, "w", 1, 0);
	if(enviado != 1){
		shouldTerminate_Aux = 1;
	}
}

// Funcao que e chamada pelo view controle e envia para o servidor que s foi pressionado
void ClientController::userPressedDown(){
	int enviado;
	enviado = send(socket_fd, "s", 1, 0);
	if(enviado != 1){
		shouldTerminate_Aux = 1;
	}
}

// Funcao que e chamada pelo view controle e envia para o servidor que a foi pressionado
void ClientController::userPressedLeft(){
	int enviado;
	enviado = send(socket_fd, "a", 1, 0);
	if(enviado != 1){
		shouldTerminate_Aux = 1;
	}
}

// Funcao que e chamada pelo view controle e envia para o servidor que d foi pressionado
void ClientController::userPressedRight(){
	int enviado;
	enviado = send(socket_fd, "d", 1, 0);
	if(enviado != 1){
		shouldTerminate_Aux = 1;
	}
}

// Funcao que altera o valor do shouldTerminate_Aux
void ClientController::terminate(){
	fprintf(stderr, "terminate called\n");
	shouldTerminate_Aux = 1;
}

// Funcao que retorna o valor do shouldTerminate_Aux
int ClientController::shouldTerminate(){
	return shouldTerminate_Aux;	
}

/*
*Essa funcão atualiza a propriedade viewController para o ponteiro passado.
*/
void ClientController::setViewController(ViewController *pointer){
	viewController = pointer;
}

/*
*Essa funcão atualiza a propriedade Setscene para o ponteiro passado.
*/
void ClientController::setScene(Scene *pointer){
	currentScene = pointer;
}




# Jogo para EA872

## Como usar o modo multiplayer
Na raiz do projeto, usar `make` para compilar

Usar `./build/game 2>log` para executar o jogo, que ira ouvir a porta 7823 em todas as interfaces disponíveis.

O servidor espera 10 segundos esperando conexões de clientes.

Usar `./build/game client 127.0.0.1 7823 2>log2` para se conectar a um servidor.

## Membros
* Arthur Moraes do Lago (157702)
* Vitor Saldanha da Silveira (158508)

## TODO
Ao invés do gameController ter uma referencia para serverCOntroller apenas para enviar som para clientes especificos, fazer gameController chamar uma função no viewCOntroller, que vai fazer as contas da intensidade do som no lado esquerdo e direito, e enviar isso para o serverController. Faz sentido pois essa conta para o som stereo faz parte de logica de visualizacao, e o serverCOntroller e clientCOntroller sao apenas comunicadores, que no final so estarao repassando mensagens entre viewControllers.

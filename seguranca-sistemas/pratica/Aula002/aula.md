# Como nmap funciona

## Dois passos

1. Descoberta de hosts (host discovery)
    - Identifica quais hosts estão ativos em uma rede.
    - Utiliza pacotes ICMP, TCP e UDP para determinar a presença de dispositivos.

2. Varredura de portas (port scanning)
    - Identifica quais portas em um host estão abertas e aceitando conexões.
    - Utiliza diferentes técnicas de varredura, como TCP SYN scan, TCP connect scan, e UDP scan.


## TCP

### O que é um three-way handshake?

Um three-way handshake é um processo usado para estabelecer uma conexão TCP confiável entre um cliente e um servidor. Ele envolve três etapas principais:
1. **SYN (Synchronize)**: O cliente envia um segmento TCP com a flag SYN definida para o servidor, indicando que deseja iniciar uma conexão.
2. **SYN-ACK (Synchronize-Acknowledge)**: O servidor responde com um segmento TCP que tem as flags SYN e ACK definidas, reconhecendo o pedido do cliente e indicando que está pronto para estabelecer a conexão.
3. **ACK (Acknowledge)**: O cliente envia um segmento TCP com a flag ACK definida, confirmando que recebeu a resposta do servidor. Após essas três etapas, a conexão TCP é estabelecida e os dados podem ser trocados entre o cliente e o servidor.

Esse processo garante que ambos os lados estão prontos para a comunicação e ajuda a sincronizar os números de sequência usados para rastrear os dados enviados.

## UDP

1 - Envia um pacote UDP para a porta alvo.
2 - Se a porta estiver fechada, o host responde com um pacote ICMP "Port Unreachable".
3 - Se a porta estiver aberta, o host não responde (silêncio).
4 - Se o host estiver inativo, o nmap pode receber um pacote ICMP "Host Unreachable" ou "Network Unreachable".



As camadas de rede seguem o modelo **simplificado**:

1. **Física**
2. **Ligação / Link**
3. **Rede**
4. **Transporte**
5. **Aplicação** //TCP/IP
5.1 **Sessão** //OSI
5.2 **Apresentação** //OSI

O modelo OSI (Open Systems Interconnection) possui 7 camadas, mas o modelo TCP/IP (mais utilizado) é uma versão simplificada com 5 camadas principais.

## 5 aplicação

1. **HTTP (Hypertext Transfer Protocol)**: Protocolo utilizado para a transferência de páginas web. Ele define como os clientes (navegadores) e servidores se comunicam para solicitar e enviar páginas web.

2. **FTP (File Transfer Protocol)**: Protocolo utilizado para a transferência de arquivos entre um cliente e um servidor. Ele permite o upload e download de arquivos, além de oferecer funcionalidades como autenticação e controle de acesso.

## 4 transporte

1. **TCP (Transmission Control Protocol)**: Protocolo orientado à conexão que garante a entrega confiável dos dados. Ele estabelece uma conexão antes de transmitir dados e utiliza mecanismos como controle de fluxo, controle de congestionamento e verificação de erros para garantir que os dados cheguem corretamente ao destino.

2. **UDP (User Datagram Protocol)**: Protocolo sem conexão que não garante a entrega dos dados. Ele é mais rápido que o TCP, mas não oferece mecanismos de controle de fluxo ou verificação de erros. É usado em aplicações onde a velocidade é mais importante que a confiabilidade, como streaming de vídeo e jogos online.

## 3 rede

1. **IP (Internet Protocol)**: Protocolo responsável pelo endereçamento e roteamento dos pacotes de dados na rede. Ele define como os pacotes são encaminhados de um dispositivo para outro, utilizando endereços IP para identificar os dispositivos na rede.

2. **ICMP (Internet Control Message Protocol)**: Protocolo usado para enviar mensagens de controle e erro na rede. Ele é frequentemente utilizado para diagnosticar problemas de conectividade, como no comando "ping", que envia pacotes ICMP Echo Request para verificar se um host está acessível.

## 2 ligação

1. **Ethernet**: Protocolo de rede local (LAN) mais comum, que define como os dispositivos na mesma rede física se comunicam. Ele utiliza endereços MAC para identificar os dispositivos e controla o acesso ao meio físico.

2. **Wi-Fi (Wireless Fidelity)**: Protocolo de rede sem fio que permite a comunicação entre dispositivos em uma rede local sem a necessidade de cabos. Ele utiliza frequências de rádio para transmitir dados e é amplamente utilizado em redes domésticas e públicas.

## 1 física

1. **Cabo de par trançado (Twisted Pair Cable)**: Tipo de cabo amplamente utilizado em redes Ethernet, composto por pares de fios trançados para reduzir interferências eletromagnéticas.

2. **Fibra óptica (Optical Fiber)**: Meio de transmissão que utiliza luz para transmitir dados em alta velocidade e a longas distâncias, com baixa perda de sinal e imunidade a interferências eletromagnéticas.


-- Software nmap -- Exercícios

1) Utilize o software nmap, para obter todos os endereços IPs disponíveis na rede do laboratório;
Utilize apenas as opções que não fazem scanning de portas; Que comando utilizou?

```bash
sudo nmap -sn 192.168.88.0/24
```

```plaintext
192.168.88.1
192.168.88.137
192.168.88.139
192.168.88.140
192.168.88.143
192.168.88.144
192.168.88.145
192.168.88.149
192.168.88.150
192.168.88.151
192.168.88.152
192.168.88.155
192.168.88.156
```

2) Escolha o seu colega do lado para fazer scanning às portas do seu equipamento. Teste várias
abordagens (comandos) para ver qual destas devolve mais informações sobre as portas; Faça
testes com e sem a firewall ligada. Escreva abaixo os vários comandos que introduziu e quais os
resultados.

```bash
sudo nmap -sS 192.168.88.141 -- FAIL
sudo nmap -sT 192.168.88.141 -- OK
sudo nmap -sU 192.168.88.141 -- TAKE TOO LONG
sudo nmap -sA 192.168.88.141 -- FAIL
```

```
root@ubuntu:/home/kauan/Desktop/estg# nmap -sT 192.168.88.141

Starting Nmap 7.95 ( https://nmap.org ) at 2025-10-02 18:44 WEST
Nmap scan report for 192.168.88.141
Host is up (0.0046s latency).
Not shown: 977 closed tcp ports (conn-refused)
PORT     STATE SERVICE
21/tcp   open  ftp
22/tcp   open  ssh
23/tcp   open  telnet
25/tcp   open  smtp
53/tcp   open  domain
80/tcp   open  http
111/tcp  open  rpcbind
139/tcp  open  netbios-ssn
445/tcp  open  microsoft-ds
512/tcp  open  exec
513/tcp  open  login
514/tcp  open  shell
1099/tcp open  rmiregistry
1524/tcp open  ingreslock
2049/tcp open  nfs
2121/tcp open  ccproxy-ftp
3306/tcp open  mysql
5432/tcp open  postgresql
5900/tcp open  vnc
6000/tcp open  X11
6667/tcp open  irc
8009/tcp open  ajp13
8180/tcp open  unknown
MAC Address: 2C:6D:C1:62:CD:52 (Intel Corporate)
```

3) De acordo com os resultados anteriores, qual é o endereço MAC e o fabricante da placa de rede
do seu colega do lado?

```plaintext
Endereço MAC: 2C:6D:C1:62:CD:52
Fabricante: Intel Corporate
```

4) Pesquise se existe na rede do laboratório algum servidor web a funcionar na porta 80 ou 443? Se
sim, pesquise qual é a versão do serviço. Que comando(s) utilizou?

```bash
# -sT -> TCP connect scan
# -p 80,443 -> scan only ports 80 and 443
# -sV -> service/version detection
sudo nmap -sT -p 80,443 -sV 192.168.88.0/24
```

5) Pesquise qual é a versão do servidor web dos sites: www.ipleiria.pt, ead.ipleiria.pt, www.sapo.pt

```bash
sudo nmap -sT -p 80,443 -sV www.ipleiria.pt ead.ipleiria.pt www.sapo.pt
```

```
www.ipleiria.pt: Check Point SVN foundation httpd (port 80), nginx (port 443)

ead.ipleiria.pt: nginx (port 80 and 443)

www.sapo.pt: nginx (port 80 and 443)
```

6) Pesquise qual é a versão do sistema operativo dos sites: www.ipleiria.pt, ead.ipleiria.pt,
www.sapo.pt

```bash
# -A -> Enable OS detection, version detection, script scanning, and traceroute
# -T4 -> Set timing template (higher is faster)
sudo nmap -A -T4 www.ipleiria.pt ead.ipleiria.pt www.sapo.pt
```

```plaintext
www.ipleiria.pt: Linux 3.10 - 4.11 (87%)
ead.ipleiria.pt: Linux 4.X|3.X|5.X|2.6.X (92%)
www.sapo.pt: No OS matches for host
```


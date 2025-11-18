
### 1) Utilize o software nmap, para obter todos os endereços IPs disponíveis na rede do laboratório; Utilize apenas as opções que não fazem scanning de portas; Que comando utilizou?

```bash
sudo nmap -sn 192.168.88.0/24
```


### 2) Escolha o seu colega do lado para fazer scanning às portas do seu equipamento. Teste várias abordagens (comandos) para ver qual destas devolve mais informações sobre as portas; Faça testes com e sem a firewall ligada. Escreva abaixo os vários comandos que introduziu e quais os resultados.


```bash
nmap -sT -p- 192.168.88.141
sudo nmap -sS -p- 192.168.88.141
sudo nmap -sS -sV -p- 192.168.88.141
```

* `-sT`: *connect* (completa a ligação TCP). Mais lento e ruidoso, mas funciona sem `sudo`.
* `-sS`: *SYN* stealth scan. Normalmente devolve mais informação que `-sT`.
* `-sV`: tenta identificar o serviço e versão das portas abertas.
* `-p-`: varre todas as 65535 portas TCP (padrão é só as 1000 mais comuns).

### 3) De acordo com os resultados anteriores, qual é o endereço MAC e o fabricante da placa de rede do seu colega do lado?

```bash
sudo nmap -sn 192.168.88.141
```

### 4) Pesquise se existe na rede do laboratório algum servidor web a funcionar na porta 80 ou 443? Se sim, pesquise qual é a versão do serviço. Que comando(s) utilizou?

```bash
sudo nmap -p 80,443 --open -sV 192.168.88.0/24 -oN web_hosts.txt
```

* `--open` só mostra hosts com as portas abertas.
* `-sV` tenta identificar versão do serviço (ex.: Apache/2.4.41).
* `-oN` guarda a saída num ficheiro (opcional).

### 5) Pesquise qual é a versão do servidor web dos sites: www.ipleiria.pt, ead.ipleiria.pt, www.sapo.pt

```bash
sudo nmap -p 80,443 -sV www.ipleiria.pt ead.ipleiria.pt www.sapo.pt
```

### 6) Pesquise qual é a versão do sistema operativo dos sites: www.ipleiria.pt, ead.ipleiria.pt, www.sapo.pt

```bash
sudo nmap -p 80,443 -O www.ipleiria.pt ead.ipleiria.pt www.sapo.pt
```

### 7) Tente descobrir nas redes do laboratório, qual é o equipamento/endereço IP que tem um serviço à escuta no porto 3306. Qual é o endereço IP e qual é o serviço, a sua versão e o seu sistema operativo?

```bash
sudo nmap -p 3306 --open -sV 192.168.88.0/24
```

### 8) Faça um port scanning das portas 0 a 1000 do default gateway da sua rede. Quais as portas que estão abertas e qual é o fabricante do equipamento?

```bash
sudo nmap -p 0-1000 -sS -sV -O 192.168.88.1
```

### 9) Procure na rede local se existem equipamentos que tenham vulnerabilidades conhecidas. Utilize a script vulners.

```bash
sudo nmap -sV --script vulners 192.168.88.0/24
```

### 10) Procure na internet um endereço IP aleatório que tenha o serviço de POP3 Seguro aberto.

**Nota:** Não é recomendado fazer varreduras na Internet sem permissão. Em vez disso, utilize serviços como Shodan ou Censys para encontrar IPs com POP3S aberto.

```bash
# Exemplo com Shodan CLI
shodan search --fields ip_str,port "port:995"
```

### 11) Utilizando o endereço IP que encontrou, verifique através do script traceroute-geolocation, onde é que está esse servidor localizado?

```bash
sudo nmap --script traceroute-geolocation --script-args 'traceroute-geolocation.host=IP_DO_SERVIDOR' -Pn IP_DO_SERVIDOR
```

### 12) Procure na internet um endereço IP aleatório (max: 100) que tenha o serviço de POP3 Seguro aberto. Ative o modo de verbose.

```bash
shodan search --fields ip_str,port "port:995" --limit 100 --verbose
```

```bash
sudo nmap -sS -sV -p 995 -Pn -v IP_AUTORIZADO
```
### 13) Procure na rede local todos os equipamentos ativos e guarde o resultado num ficheiro com o nome nmaOut.txt. Execute o varrimento utilizando a maneira de análise pais rápida de portas abertas.

### 14) Utilizando o ficheiro de texto nmapOut.txt resultado do exercício anterior, verifique quais as portas que estão abertas para os equipamentos descobertos.


## 1) Obter todos os IPs disponíveis na rede do laboratório — **sem** fazer *port scanning*

Comando recomendado (descoberta de hosts / ping scan, não varre portas):


---

## 2) Scanning às portas do colega — várias abordagens (comandos) e diferenças

Executa estes comandos no teu computador apontando para o IP do colega (ex.: `192.168.88.141`). Pede ao colega para ligar/desligar firewall e compara.



* `-sV` tenta identificar serviço e versão das portas abertas.

### d) Aggressive (OS, scripts, banner)

```bash
sudo nmap -A -p 1-65535 192.168.88.141
```

* `-A` combina detecção de versão, OS, traceroute e scripts NSE — dá muita informação (também mais detectável).

### e) Scan rápido das portas mais comuns

```bash
sudo nmap -sS -F -sV 192.168.88.141
```

* `-F` faz *fast* scan (portas mais comuns).

### Como registar os resultados (com e sem firewall)

Executa cada comando e guarda a saída:

```bash
sudo nmap -sS -sV -p 1-65535 192.168.88.141 -oN scan_full.txt
```

Resultados típicos e o que observar:

* Portas `open` / `filtered` / `closed`.
* `SERVICE` e `VERSION` quando `-sV` detecta.
* Com firewall ligada: muitas portas aparecem `filtered` ou `closed`; sem firewall, mais portas aparecem `open`.
* Com `-A` podes também ver o `OS details` e banners HTTP.

---

## 3) Obter endereço MAC e fabricante da placa do colega

Do `-sn` (host discovery) ou de um scan directo ao IP:

```bash
sudo nmap -sn 192.168.88.141
# ou
sudo nmap -sS -sV 192.168.88.141
```

Na saída o Nmap mostra:

```
MAC Address: AA:BB:CC:DD:EE:FF (Intel Corporate)
```

Alternativas:

```bash
arp -n 192.168.88.141
# ou listar a ARP table:
arp -a
```

Interpreta: o prefixo do MAC (primeiros 3 bytes) identifica o fabricante (vendor). O Nmap já mostra o vendor entre parêntesis.

---

## 4) Há algum servidor web na rede (porta 80 ou 443)? Comando(s) usados

Varre a rede apenas para portas 80 e 443, detectando versões:

```bash
sudo nmap -p 80,443 --open -sV 192.168.88.0/24 -oN web_hosts.txt
```

* `--open` só mostra hosts com as portas abertas.
* `-sV` tenta identificar versão do serviço (ex.: Apache/2.4.41).

Se quiseres mostrar título e cabeçalhos HTTP:

```bash
sudo nmap -p 80,443 --script http-server-header,http-title -sV 192.168.88.0/24
```

Interpretação:

* Se existir servidor na rede vais ver IP, porta (80/443), `SERVICE` e possivelmente algo como:

```
80/tcp open  http    Apache httpd 2.4.41 ((Ubuntu))
443/tcp open  ssl/http nginx 1.18.0
```

Também podes confirmar com `curl`:

```bash
curl -I http://192.168.88.X
curl -I https://192.168.88.Y
```

(para HTTPS usa `-k` se certificate self-signed).

---

## 5) Versão do servidor web para: `www.ipleiria.pt`, `ead.ipleiria.pt`, `www.sapo.pt`

**Comandos a executar localmente (recomendado):**

```bash
# obter cabeçalhos HTTP com curl
curl -I https://www.ipleiria.pt
curl -I https://ead.ipleiria.pt
curl -I https://www.sapo.pt

# ou usar nmap para detecção de versão e script http-server-header
sudo nmap -p 80,443 -sV --script=http-server-header http[s]://www.ipleiria.pt
# exemplo
sudo nmap -p 80,443 -sV --script=http-server-header www.ipleiria.pt



sudo nmap -p 80,443 -O www.ipleiria.pt ead.ipleiria.pt www.sapo.pt
```

O `curl -I` retorna o cabeçalho `Server:` quando disponível. Exemplo:

```
HTTP/1.1 200 OK
Server: nginx/1.20.1
...
```

> Observação prática: muitos sites escondem/despersonalizam o `Server:` (ou usam CDN) — nalguns casos o cabeçalho não revela versão real.

---

## 6) Versão do sistema operativo (OS) dos sites: `www.ipleiria.pt`, `ead.ipleiria.pt`, `www.sapo.pt`

Para medir via *remote OS fingerprinting* usa:

```bash
sudo nmap -O -Pn www.ipleiria.pt
# ou combinando detecção de versão
sudo nmap -A -Pn www.ipleiria.pt
```

* `-O` tenta detectar o OS remoto (apenas se o host responder parcialmente).
* `-Pn` evita o ping (útil para hosts protegidos por firewall).
  **Nota:** detecção de OS sobre a Internet é frequentemente imprecisa (CDNs, load-balancers mascaram o real SO). Usa os resultados como indicativos, não definitivos.

---

## 7) Encontrar serviço à escuta na porta 3306 (MySQL) na rede do laboratório

Comando para procurar hosts que tenham 3306 aberto:

```bash
sudo nmap -p 3306 --open -sV 192.168.88.0/24 -oN mysql_hosts.txt
```

Interpretação da saída:

```
3306/tcp open  mysql  MySQL 5.7.33 (protocol 10)
```

Se detetar `mysql` e `VERSION`, a linha acima dará IP, serviço e versão. Para sistema operativo usa `-O` no IP descoberto:

```bash
sudo nmap -O -p 3306 192.168.88.123
```

---

## 8) Port scanning portas 0 a 1000 do **default gateway** — portas abertas e fabricante do equipamento

1. Obter default gateway:

```bash
ip route | grep default
# resultado ex.: default via 192.168.88.1 dev wlp0s20f3
```

2. Scannear portas 0–1000:

```bash
GATEWAY=192.168.88.1
sudo nmap -p 0-1000 -sS -sV -O $GATEWAY -oN gw_scan.txt
```

O Nmap mostrará portas `open` e possivelmente:

```
Device type: general purpose
Running: Linux 4.X
OS details: Linux 4.15 - 4.19
MAC Address: 00:11:22:33:44:55 (Cisco Systems)
```

O fabricante aparece no campo `MAC Address` (vendor) quando disponível.

---

## 9) Procurar equipamentos com vulnerabilidades conhecidas — usar script `vulners`

Exemplo de comando (alvo: rede local):

```bash
# varre a rede inteira com detecção de versão e aplica o script vulners
sudo nmap -sV --script vulners --script-args mincvss=5.0 192.168.88.0/24 -oN vulns.txt
```

* `--script vulners` consulta a base de dados de vulnerabilidades (requer internet para o Nmap baixar referências).
* `mincvss=5.0` limita a relatórios com CVSS >= 5.0 (opcional).
  Interpretação: o output contém CVE, descrição e referência (se detectado). Guarda `vulns.txt` para relatório.

---

## 10) Procurar na Internet um IP com POP3 Seguro (POP3S, porto 995) **(recomendação segura)**

**Não vou executar scans na Internet sem permissão.** Em vez disso usa índices públicos como **Shodan** ou **Censys** (são ferramentas apropriadas para encontrar serviços públicos) — assim não tens de varrer a Internet por conta própria.

Exemplo com **Shodan CLI**:

```bash
# instala shodan CLI e autentica com a tua API key
shodan init YOUR_API_KEY
# procura hosts com porta 995 aberta
shodan search --fields ip_str,port "port:995"
```

Isto devolve IPs públicos que o Shodan indexou com POP3S. **Não** conectes/forneças ataques a esses hosts sem autorização — apenas regista o IP para fins de estudo/consulta.

---

## 11) Com o IP encontrado: usar `traceroute-geolocation` (script Nmap)

Se tiveres um IP público (obviamente com permissão para investigar), podes usar:

```bash
sudo nmap --script traceroute-geolocation --script-args 'traceroute-geolocation.host=IP_DO_SERVIDOR' -Pn -oN geo.txt IP_DO_SERVIDOR
```

Ou combinar com `-sV`:

```bash
sudo nmap -sV -Pn --script traceroute-geolocation IP_DO_SERVIDOR
```

Interpretação: o script devolve um traceroute enriquecido com localização estimada dos hops (baseado em bases públicas). Resultados são estimativas geográficas (não 100% precisos).

---

## 12) Procurar na Internet até 100 IPs com POP3S aberto, modo verbose

Novamente, **usa Shodan** para evitar varreduras activas indevidas. Com Shodan CLI podes limitar resultados e pedir verbosidade:

```bash
# devolver até 100 resultados
shodan search --fields ip_str,port --limit 100 "port:995"
```

Se depois tiveres autorização para testar um conjunto de IPs, para fazer nmap em verbose:

```bash
# exemplo para um IP autorizado
sudo nmap -sS -sV -p 995 -Pn -v IP_AUTORIZADO
```

* `-v` ativa verbose. Para mais detalhe usa `-vv`.

---

## 13) Procurar na rede local todos os equipamentos activos e guardar em `nmapOut.txt` — análise rápida de portas abertas

Comando (varredura rápida de portas com `-F` e só mostrar portas abertas):

```bash
sudo nmap -sS -F --open 192.168.88.0/24 -oN nmapOut.txt
```

* `-F` = fast (portas mais comuns).
* `--open` = só listar hosts com portas abertas.
  Isto devolve uma listagem dos hosts activos com as portas mais comuns abertas e guarda em `nmapOut.txt`.

Se apenas queres descobrir hosts (ping) e depois um scan rápido:

```bash
sudo nmap -sn 192.168.88.0/24 -oG - | awk '/Up$/{print $2}' > hosts.txt
# depois varrer esses hosts rapidamente:
sudo nmap -sS -F --open -iL hosts.txt -oN nmapOut.txt
```

---

## 14) A partir do ficheiro `nmapOut.txt`, verificar quais portas estão abertas

Comandos úteis para extrair portas abertas do ficheiro:

```bash
# Método simples: procurar linhas com 'open'
grep -i "open" nmapOut.txt

# Mostrar IP associado à(s) porta(s) open:
awk '
/Nmap scan report for/ { host=$NF }
/[0-9]+\/tcp.*open/ { print host " " $0 }
' nmapOut.txt

# Uma versão compacta:
sed -n '/Nmap scan report for/ h; /open/p' nmapOut.txt
```

Exemplo de saída esperada:

```
192.168.88.141 22/tcp open  ssh
192.168.88.141 80/tcp open  http
192.168.88.101 3306/tcp open mysql
```

Podes também converter para CSV com um script awk para relatório.

---

### Resumo rápido — comandos-chave a copiar

```bash
# 1) host discovery (sem portas)
sudo nmap -sn 192.168.88.0/24

# 2) varios tipos de port scan (para um IP alvo)
sudo nmap -sS -sV -p 1-65535 192.168.88.141
sudo nmap -A 192.168.88.141

# 3) mac + vendor
sudo nmap -sn 192.168.88.141

# 4) procurar web servers (80/443)
sudo nmap -p 80,443 --open -sV 192.168.88.0/24
sudo nmap -p 80,443 --script=http-server-header,http-title -sV www.ipleiria.pt

# 7) procurar 3306
sudo nmap -p 3306 --open -sV 192.168.88.0/24

# 8) scan gateway portas 0-1000
G=$(ip route | awk '/default/ {print $3}')
sudo nmap -p 0-1000 -sS -sV -O $G

# 9) usar vulners
sudo nmap -sV --script vulners 192.168.88.0/24

# 13) scan rápido e guardar
sudo nmap -sS -F --open 192.168.88.0/24 -oN nmapOut.txt

# 14) extrair portas abertas
grep -i "open" nmapOut.txt
awk '/Nmap scan report for/ { host=$NF } /open/ {print host " " $0}' nmapOut.txt
```

---

Se queres, eu:

* **a.** preparo um ficheiro `bash` (script) com estes comandos adaptáveis (só tens de definir a rede/gateway/target) para correr no lab; ou
* **b.** faço exemplos de parsing/relatórios (awk) mais elaborados para gerar CSV/markdown a partir do `nmapOut.txt`.

Diz qual opção preferes — eu já tenho tudo pronto para gerar o script/relatório e envio directamente aqui.

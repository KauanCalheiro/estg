#! /bin/bash

# chmod +x firewall-on.sh
# sudo ./firewall-on.sh

# How to find port numbers by service name
# kauan@ubuntu:~/Desktop/estg/seguranca-sistemas/iptables$ grep -i https /etc/services 
# # Updated from https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml .
# https		443/tcp				# http protocol over TLS/SSL
# https		443/udp				# HTTP/3

info() {
    echo -e "\033[0;34m$1\033[0m"
}

success() {
    echo -e "\033[0;32m$1\033[0m"
}

IPT=/usr/sbin/iptables
DYN="1024:65535" # Dynamic ports range

info "Defining default policy (deny all)"
$IPT -P INPUT DROP
$IPT -P OUTPUT DROP
$IPT -P FORWARD DROP
success "Defined default policy\n" 

info "Flushing all rules"
$IPT -F
success "Flushed all rules\n\n"

info "Firewall rules"

info "\tAllowing DNS"
$IPT -A OUTPUT -p udp --sport $DYN --dport 53   -j ACCEPT
$IPT -A INPUT  -p udp --sport 53   --dport $DYN -j ACCEPT
success "\tAllowed DNS\n"

info "\tAllowing loopback interface"
$IPT -A INPUT  -i lo -j ACCEPT
$IPT -A OUTPUT -o lo -j ACCEPT
success "\tAllowed loopback interface\n"

info "\tBlocking ping to 192.168.88.144"
$IPT -A OUTPUT -p icmp --destination 192.168.88.144 --icmp-type echo-request -j DROP # block to send ping to IP
# # $IPT -A INPUT  -p icmp --source 192.168.88.144 --icmp-type echo-reply -j DROP # block to recive pong from IP
success "\tBlocked ping to 192.168.88.144\n"

info "\tAllowing ping (stateless)"
$IPT -A OUTPUT -p icmp -j ACCEPT
$IPT -A INPUT  -p icmp -j ACCEPT
success "\tAllowed ping (stateless)\n"

info "\tAllowing ping (as client)"
$IPT -A OUTPUT -p icmp --icmp-type echo-request -j ACCEPT
$IPT -A INPUT  -p icmp --icmp-type echo-reply   -j ACCEPT
success "\tAllowed ping (as client)\n"

info "\tAllowing ping (as server)"
$IPT -A INPUT -p icmp --icmp-type echo-request -j ACCEPT
$IPT -A OUTPUT -p icmp --icmp-type echo-reply   -j ACCEPT
success "\tAllowed ping (as server)\n"

info "\tAllowing http"
$IPT -A OUTPUT -p tcp --sport $DYN --dport 80   -j ACCEPT
$IPT -A INPUT  -p tcp --sport 80   --dport $DYN -j ACCEPT
success "\tAllowed http\n"

info "\tAllowing https"
# Can use name|port

# $IPT -A OUTPUT -p tcp --sport $DYN   --dport https -j ACCEPT
# $IPT -A INPUT  -p tcp --sport https  --dport $DYN  -j ACCEPT

$IPT -A OUTPUT -p tcp --sport $DYN --dport 443  -j ACCEPT
$IPT -A INPUT  -p tcp --sport 443  --dport $DYN -j ACCEPT
success "\tAllowed https\n"

info "\tAllowing SSH (as client)"
$IPT -A OUTPUT -p tcp --sport $DYN --dport ssh   -j ACCEPT
$IPT -A INPUT  -p tcp --sport ssh  --dport $DYN  -j ACCEPT
success "\tAllowed SSH (as client)\n"

info "\tAllowing SSH (as server)"
$IPT -A INPUT  -p tcp --sport $DYN --dport ssh   -j ACCEPT
$IPT -A OUTPUT -p tcp --sport ssh  --dport $DYN  -j ACCEPT
success "\tAllowed SSH (as server)\n"

info "\tAllowing DHCP"
$IPT -A OUTPUT -p udp --sport 68 --dport 67 -j ACCEPT
$IPT -A INPUT  -p udp --sport 67 --dport 68 -j ACCEPT
success "\tAllowed DHCP\n"

success "Firewall rules"
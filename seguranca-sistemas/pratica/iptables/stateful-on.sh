#! /bin/bash

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

info "\tAllowing loopback interface"
$IPT -A INPUT  -i lo -j ACCEPT
$IPT -A OUTPUT -o lo -j ACCEPT
success "\tAllowed loopback interface\n"

##################################################
# GENERIC STATEFUL RULES 
##################################################
$IPT -A INPUT  -m state --state ESTABLISHED,RELATED -j ACCEPT
$IPT -A OUTPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

info "\tAllowing DNS"
$IPT -A OUTPUT -p udp --sport $DYN --dport 53 -m state --state NEW -j ACCEPT
success "\tAllowed DNS\n"

info "\tAllowing http"
$IPT -A OUTPUT -p tcp --sport $DYN --dport 80 -m state --state NEW -j ACCEPT
success "\tAllowed http\n"

info "\tAllowing https"
$IPT -A OUTPUT -p tcp --sport $DYN --dport 443 -m state --state NEW -j ACCEPT
success "\tAllowed https\n"

info "\tAllowing SSH (as client)"
$IPT -A OUTPUT -p tcp --sport $DYN --dport ssh -m state --state NEW -j ACCEPT
success "\tAllowed SSH (as client)\n"

info "\tAllowing SSH (as server)"
$IPT -A INPUT -p tcp --sport $DYN --dport ssh -m state --state NEW -j DROP
success "\tAllowed SSH (as server)\n"

info "\tAllowing DHCP"
$IPT -A OUTPUT -p udp --sport 68 --dport 67 -m state --state NEW -j ACCEPT
success "\tAllowed DHCP\n"

info "\tBlocking ping to 192.168.88.144"
$IPT -A OUTPUT -p icmp --destination 192.168.88.144 --icmp-type echo-request -m state --state NEW -j DROP # block to send ping to IP
# # $IPT -A INPUT  -p icmp --source 192.168.88.144 --icmp-type echo-reply -j DROP # block to recive pong from IP
success "\tBlocked ping to 192.168.88.144\n"

info "\tAllowing ping (stateless)"
$IPT -A OUTPUT -p icmp -m state --state NEW -j ACCEPT
success "\tAllowed ping (stateless)\n"

info "\tAllowing ping (as client)"
$IPT -A OUTPUT -p icmp --icmp-type echo-request -m state --state NEW -j ACCEPT
success "\tAllowed ping (as client)\n"

info "\tAllowing ping (as server)"
$IPT -A INPUT -p icmp --icmp-type echo-request -m state --state NEW -j ACCEPT
success "\tAllowed ping (as server)\n"


success "Firewall rules"
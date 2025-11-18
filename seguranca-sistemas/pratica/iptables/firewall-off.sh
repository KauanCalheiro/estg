#! /bin/bash

# chmod +x firewall-on.sh
# sudo ./firewall-on.sh

IPT=/usr/sbin/iptables

echo "Define default policy: deny all"
$IPT -P INPUT ACCEPT
$IPT -P OUTPUT ACCEPT
$IPT -P FORWARD ACCEPT

echo "Flush all rules"
$IPT -F
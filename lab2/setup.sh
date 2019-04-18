IP=10.42.0.1

iptables -I FORWARD -d $IP/32 -i wlx74da38e6c42b -p tcp -m tcp --dport 8888 -j ACCEPT
iptables -I FORWARD -i wlx74da38e6c42b -j REJECT
iptables -t nat -A PREROUTING -i wlx74da38e6c42b -p tcp -m tcp --dport 80 -j DNAT --to-destination $IP:8888
iptables -t nat -A PREROUTING -i wlx74da38e6c42b -p tcp -m tcp --dport 443 -j DNAT --to-destination $IP:8888

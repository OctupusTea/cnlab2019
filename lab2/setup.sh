iptables -A FORWARD -d 192.168.12.1/32 -i wlx74da38e6c42b -p tcp -m tcp --dport 8888 -j ACCEPT
iptables -A FORWARD -i wlx74da38e6c42b -j REJECT
iptables -t nat -A PREROUTING -i wlx74da38e6c42b -p tcp -m tcp --dport 80 -j DNAT --to-destination 192.168.12.1:8888
iptables -t nat -A PREROUTING -i wlx74da38e6c42b -p tcp -m tcp --dport 443 -j DNAT --to-destination 192.168.12.1:8888
iptables -t nat -A POSTROUTING -s 192.168.12.0/24 ! -o wlx74da38e6c42b -j MASQUERADE

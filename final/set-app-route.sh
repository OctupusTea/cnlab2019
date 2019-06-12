FWMARK=10
TABLE=100
GATEWAY=$1

ip rule add fwmark $FWMARK table $TABLE 2> /dev/null
ip route add default via $GATEWAY table $TABLE 2> /dev/null

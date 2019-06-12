#! /usr/bin/env bash

FWMARK=10
TABLE=100

if [[ $1 == "--remove" ]]; then
	ip rule del fwmark $FWMARK table $TABLE
	ip route del table 100 default
else
	IFACE=$1

	GATEWAY=$( route -n | grep -e "^0.0.0.0" | grep $1 | cut -c17-32 | uniq )
	echo $GATEWAY

	ip rule add fwmark $FWMARK table $TABLE
	ip route add default via $GATEWAY table $TABLE
fi

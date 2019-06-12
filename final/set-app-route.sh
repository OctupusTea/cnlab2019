#! /usr/bin/env bash

FWMARK=10
TABLE=100

if [[ $1 == "remove" ]]; then
	ip rule del fwmark $FWMARK table $TABLE
	ip route del table 100 default
else
	GATEWAY=$1

	ip rule add fwmark $FWMARK table $TABLE
	ip route add default via $GATEWAY table $TABLE
fi

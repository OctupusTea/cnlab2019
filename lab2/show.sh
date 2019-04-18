#!/usr/bin/env bash
iptables -L --line-numbers && echo === && iptables -t nat -L --line-numbers

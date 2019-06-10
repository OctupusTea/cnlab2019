#! /usr/bin/env python3

import subprocess

def popen(command):
	process = subprocess.Popen(command, stdout=subprocess.PIPE)
	return process

def wifi_list():
	process = popen(["nmcli", "-f", "SIGNAL,SSID", "dev", "wifi"])
	process.wait()
	print(process.returncode)

	# map(,output[1:])

def wifi_connect(iface,ssid):
	process = popen(["nmcli", "c", "up", "id", ssid, "ifname", iface])
	process.wait()
	return process.returncode == 0

if __name__ == '__main__':
	if wifi_connect("wlx74da38e6c42f", "ntu_peap 1"):
		print("Done")
	else:
		print("Failed")


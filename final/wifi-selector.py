#! /usr/bin/env python3

import subprocess
import configparser
import json

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

def wifi_disconnect(iface,ssid):
	process = popen(["nmcli", "dev", "disconnect", "ifname", iface])
	process.wait()
	return process.returncode == 0

def speedtest():
	process = popen(["sudo", "./speedtest.sh"])
	stdout, errout = process.communicate()
	result = json.loads(stdout)
	print(result)
	return result['download']

if __name__ == '__main__':
	config = configparser.ConfigParser()
	config.read("config.ini")
	num_retry = 5
	wifi_interface = "wlx74da38e6c42f"

	best_ssid = ''
	best_speed = 0
	for ssid in config.sections():
		for _ in range(num_retry):
			print("Connecting to", ssid)
			if wifi_connect(wifi_interface, config[ssid]['file2']):
				print("Connected")
				popen(["sudo", "./set-app-route.sh", wifi_interface]).wait()
				print("Start testing speed...")
				speed = speedtest()
				print(ssid + " speed = " + str(speed / 1024 / 1024) + " Mbps")
				if speed > best_speed:
					best_speed = speed
					best_ssid = ssid
				break
			else:
				print("Failed")
	print("Best wifi is:", best_ssid)
	wifi_connect("wlx74da38e6c42f", config[best_ssid]['file1'])
	wifi_disconnect(wifi_interface)

CN Lab 2
===

## Setup

1. Clear iptables: `sudo iptables -F && sudo iptables -F -t nat`
2. Run authentication server: `sudo node app.js`
3. Create Wifi hotspot `sudo create_ap -n wlx74da38e6c42b cnlab`
4. Setup iptables `sudo bash setup.sh`


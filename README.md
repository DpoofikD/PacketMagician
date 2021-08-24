# PacketMagician
[ ![Relwarc17](https://www.hackthebox.eu/badge/image/278078)](https://www.hackthebox.eu/home/users/profile/278078)

Easy Linux tool for constructing and sending custom TCP, UDP or ICMP packets. May be used for scanning, monitoring and testing.

### Warning!

I am **_not_** responsible for any damage caused by this tool and do not support using it for illegal operations. Use it your own risk.

> *Now water can flow, or it can crash... Be water, my friend*

## Compile:

~~~
git clone https://github.com/DpoofikD/PacketMagician
cd PacketMagician/
g++ -std=c++17 -o PacketMagician src/PacketMagician.cpp
~~~

## Usage:

~~~
sudo ./PacketMagician --tip IP --sip IP --tp PORT --sp PORT [args]
~~~

(```sudo``` is required for creating **RAW socket**)

## Arguments list:
~~~
-h, --help - shows this message
--usage - shows the usage message
--protocol - which protocol would you like to use (tcp, udp, icmp)
--tip - Target IP
--sip - Source IP
--tp - Target Port
--sp - Source Port
--ttl - TTL of a packet
--syn - SYN flag
--fin - FIN flag
--rst - RST flag
--psh - PSH flag
--ack - ACK flag
--urg - URG flag
--w-size - packet window size
-t, --time - Time to send packets
--pck-amount - Amount of packets to send
--tout, --timeout - Time to wait for response
-d, --delay - Delay between packets sendings
--cksum - Whether to calculate the checksum or not
-p, --payload - Exact string payload to send
~~~


##To do:
- Reieve mode (for now, only in ICMP protocol)

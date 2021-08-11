# PacketMagician

Easy tool for constructing and sending custom TCP or ICMP packets.

### Warning!

I am **_not_** responsible for any damage caused by this tool and do not support using it for illegal operations. Use it your own risk.

> *Now water can flow, or it can crash... Be water, my friend*

# Compile:

~~~
git clone https://github.com/DpoofikD/PacketMagician
cd PacketMagician/
g++ -std=c++17 -o PacketMagician PacketMagician.cpp
~~~

# Usage:

~~~
sudo ./PacketMagician [args]
~~~

# Arguments list:
~~~
-h, --help - shows this message
--usage - shows the usage message
--protocol - which protocol would you like to use (currently icmp and tcp are avaliable)
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
--w-size - Window size
-t, --time - Time to send packets
--pck-amount - Amount of packets to send
--tout, --timeout - Time to wait for response
-d, --delay - Delay between packets
--cksum - Whether to calculate the checksum or not
-p, --payload - Exact payload to send
~~~

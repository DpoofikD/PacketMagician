# PacketMagician

Easy tool for constructing and sending custom packets.

# Compile:

~~~
git clone https://github.com/DpoofikD/PacketMagician
cd PacketMagician/
g++ -std=c++17 -o PacketMagician PacketMagician.cpp
~~~

# Usage:

~~~
./PacketMagician -h
        -h] [--protocol <icmp, tcp (icmp)>] [--tip <target ip (192.168.0.1)>] [--sip <source ip
        (192.168.0.2)>] [--tp <target port (80)>] [--sp <source port (15243)>] [--ttl <ttl of packet
        (255)>] [--syn <syn flag (0)>] [--fin <fin flag (0)>] [--rst <rst flag (0)>] [--psh <psh
        flag (0)>] [--ack <ack flag (0)>] [--urg <urg flag (0)>] [--w-size <window size (65536)>]
        [-t <time to repeat in seconds, -1 for timeless (-1)>] [--timeout <time to wait for respond
        in ms (1000)>] [-d <delay in ms (0)>] [--cksum <calculate the IP header checksum or not
        (0/1), 1 is default>] [-p <sets the exact payload for TCP>] [--pck-amount <amount of packets
        sent, if > 0, -t will be ignored. Use -1 for endless (-1)>]
~~~

# slirp4tinc
Attach SLiRP to a tinc UML socket, to go out from the VPN to some host net without root access.

This way the UML socket of the tinc daemon can become an VPN exit node.

The inspiration for this project came while playing around with the famous tinc daemon and some UM Linux VM, which should
function as a masquerading instance, but is really big overhead, compared to this solution.

Other possible setups could use the VDE framework, but a lot of those networking stuff requires root access, which
is not always necessary and available.

## TODO

* testing
* configuration of the slirp parameter from commandline:
   * tinc node IP
   * network mask
   * IPv4/IPv6 settings
   * DNS settings
   * loopback access
* testing
* documentation, how to start from tincd
* packet sniffing stuff with pcap file output
* port forwarding into UML socket
* installing
* testing
* error handling for failing UML connection
* logging
* daemonize
* testing

## Credits
The arg library I found somewhere in the 'net, the author is mentioned in the docs as Chun-Chung Chen <cjj@u.washington.edu>.

The umlplug stuff stems from the Linux kernel UM drivers and has it's own copyright and copyleft with the authors mentioned 
in the file headers.

Thanks to the [libvdeslirp](https://github.com/virtualsquare/libvdeslirp) project, making the access to 
[libslirp](https://gitlab.freedesktop.org/slirp/libslirp) so easy.

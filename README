# This is the buildsystem for the LEDE Linux distribution with Netgear WNR2000 V5 support.

Please use `make menuconfig` to choose your preferred
configuration for the toolchain and firmware.

You need to have installed gcc, binutils, bzip2, flex, python, perl, make,
find, grep, diff, unzip, gawk, getopt, subversion, libz-dev and libc headers.

Run `./scripts/feeds update -a` to get all the latest package definitions
defined in feeds.conf / feeds.conf.default respectively
and `./scripts/feeds install -a` to install symlinks of all of them into
package/feeds/.

Use `make menuconfig` to configure your image.

Simply running `make` will build your firmware.
It will download all sources, build the cross-compile toolchain, 
the kernel and all choosen applications.

To build your own firmware you need to have access to a Linux, BSD or MacOSX system
(case-sensitive filesystem required). Cygwin will not be supported because of
the lack of case sensitiveness in the file system.

## Push the firmware in rooter
~~~
tftp default.IPv4.rooter.route
binary
rexmt 1
timeout 60
trace
~~~
Prepare the line without enter tape: `put squashfs-image.img`.
Unplug all input internet connection.
Connect with ethernet to rooter.
Launch the rooter, led is orange.
Tape enter.

## If LuCi is in read-only mode
Add Utilities/Filesystem/e2fsprogs to your configuration.
~~~
chmod 755 /www/cgi-bin
chmod 755 /www/luci-static
find /www -type f -exec chmod 644 {} \;
chmod 755 /www/cgi-bin/luci
~~~

## Install ProtonVPN
Add Network/VPN/openvpn-easy-rsa and openvpn-openssl to yous configuration.
~~~
YourOpenVpnId > passfile.txt
YourOpenVpnPw >> passfile.txt
~~~
Add in your *.ovpn file the line `auth-user-pass passfile.txt`.
Connect with ssh and launch the line: `/usr/sbin/openvpn --cd /etc/openvpn --daemon --config /path/to/ovpn`.
Add the line to the startup (LuCi or by CLI).

Sunshine!
	Your LEDE Community
	http://www.lede-project.org



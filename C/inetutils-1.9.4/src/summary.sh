#!/bin/sh
#
# Copyright (C) 2013, 2014, 2015 Free Software Foundation, Inc.
#
# This file is part of GNU Inetutils.
#
# GNU Inetutils is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or (at
# your option) any later version.
#
# GNU Inetutils is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see `http://www.gnu.org/licenses/'.

# Template for printing a summary of program configurations.

enable_dnsdomainname=yes
dnsdomainname_BUILD=dnsdomainname
dnsdomainname_PROPS=""
enable_ftp=yes
ftp_BUILD=ftp
ftp_PROPS="-ledit"
enable_hostname=yes
hostname_BUILD=hostname
hostname_PROPS=""
enable_ifconfig=yes
ifconfig_BUILD=ifconfig
ifconfig_PROPS=""
enable_logger=yes
libls_BUILD=libls.a
libls_PROPS="@libls_PROPS@"
enable_libls=yes
logger_BUILD=logger
logger_PROPS=""
enable_ping=yes
ping_BUILD=ping
ping_PROPS=""
enable_ping6=yes
ping6_BUILD=ping6
ping6_PROPS=
enable_rcp=yes
rcp_BUILD=rcp
rcp_PROPS=""
enable_rexec=yes
rexec_BUILD=rexec
rexec_PROPS=""
enable_rlogin=yes
rlogin_BUILD=rlogin
rlogin_PROPS=""
enable_rsh=yes
rsh_BUILD=rsh
rsh_PROPS=""
enable_talk=yes
talk_BUILD=talk
talk_PROPS="-lcurses"
enable_telnet=yes
telnet_BUILD=telnet
telnet_PROPS="-ltermcap"
enable_tftp=yes
tftp_BUILD=tftp
tftp_PROPS=""
enable_traceroute=yes
traceroute_BUILD=traceroute
traceroute_PROPS=""
enable_whois=yes
whois_BUILD=whois
whois_PROPS=""
enable_ftpd=yes
ftpd_BUILD=ftpd
ftpd_PROPS=""
enable_inetd=yes
inetd_BUILD=inetd
inetd_PROPS=""
enable_rexecd=yes
rexecd_BUILD=rexecd
rexecd_PROPS=""
enable_rlogind=yes
rlogind_BUILD=rlogind
rlogind_PROPS=""
enable_rshd=yes
rshd_BUILD=rshd
rshd_PROPS=""
enable_syslogd=yes
syslogd_BUILD=syslogd
syslogd_PROPS=""
enable_talkd=yes
talkd_BUILD=talkd
talkd_PROPS=""
enable_telnetd=yes
telnetd_BUILD=telnetd
telnetd_PROPS="-ltermcap"
enable_tftpd=yes
tftpd_BUILD=tftpd
tftpd_PROPS=""
enable_uucpd=yes
uucpd_BUILD=uucpd
uucpd_PROPS=""

# Collect informational status.
cat <<EOT
Summary of build decisions:

  Clients:

    dnsdomainname  ${enable_dnsdomainname}
    ftp            ${enable_ftp}${ftp_BUILD:+  $ftp_PROPS}
    hostname       ${enable_hostname}
    ifconfig       ${enable_ifconfig}
    logger         ${enable_logger}
    ping           ${enable_ping}${ping_BUILD:+  $ping_PROPS}
    ping6          ${enable_ping6}${ping6_BUILD:+  $ping_PROPS}
    rcp            ${enable_rcp}${rcp_BUILD:+  $rcp_PROPS}
    rexec          ${enable_rexec}
    rlogin         ${enable_rlogin}${rlogin_BUILD:+  $rlogin_PROPS}
    rsh            ${enable_rsh}${rsh_BUILD:+  $rsh_PROPS}
    talk           ${enable_talk}${talk_BUILD:+  $talk_PROPS}
    telnet         ${enable_telnet}${telnet_BUILD:+  $telnet_PROPS}
    tftp           ${enable_tftp}
    traceroute     ${enable_traceroute}\
${traceroute_BUILD:+  $traceroute_PROPS}
    whois          ${enable_whois}

  Servers:

    ftpd           ${enable_ftpd}${ftpd_BUILD:+  $ftpd_PROPS}
    inetd          ${enable_inetd}
    rexecd         ${enable_rexecd}${rexecd_BUILD:+  $rexecd_PROPS}
    rlogind        ${enable_rlogind}${rlogind_BUILD:+  $rlogind_PROPS}
    rshd           ${enable_rshd}${rshd_BUILD:+  $rshd_PROPS}
    syslogd        ${enable_syslogd}
    talkd          ${enable_talkd}
    telnetd        ${enable_telnetd}${telnetd_BUILD:+  $telnetd_PROPS}
    tftpd          ${enable_tftpd}
    uucpd          ${enable_uucpd}

  Support:

    libls          ${enable_libls}
EOT

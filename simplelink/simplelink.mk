SIMPLELINKSRC := ${SIMPLELINK}/source/device.c \
                 ${SIMPLELINK}/source/driver.c \
                 ${SIMPLELINK}/source/flowcont.c \
                 ${SIMPLELINK}/source/fs.c \
                 ${SIMPLELINK}/source/netapp.c \
                 ${SIMPLELINK}/source/netcfg.c \
                 ${SIMPLELINK}/source/nonos.c \
                 ${SIMPLELINK}/source/socket.c \
                 ${SIMPLELINK}/source/spawn.c \
                 ${SIMPLELINK}/source/wlan.c \
                 ${SIMPLELINK}/platform/platform.c \
                 ${SIMPLELINK}/platform/platform_spi.c \
                 ${SIMPLELINK}/platform/wifi.c \
                 ${SIMPLELINK}/platform/wifi_scan.c \
                 ${SIMPLELINK}/platform/wifi_version.c \
                 ${SIMPLELINK}/platform/wifi_prog.c \
                 ${SIMPLELINK}/platform/wifi_ping.c \
                 ${SIMPLELINK}/platform/udp_server.c \
                 ${SIMPLELINK}/platform/udp_client.c \
                 ${SIMPLELINK}/platform/tcp_server.c \
                 ${SIMPLELINK}/platform/wifi_spawn.c \
                 ${SIMPLELINK}/platform/tcp_stream.c \
                 ${SIMPLELINK}/platform/wifi_gettime.c

SIMPLELINKINC := ${SIMPLELINK}/include \
                 ${SIMPLELINK}/source \
                 ${SIMPLELINK}/platform \
                 ${SIMPLELINK}/hostprog

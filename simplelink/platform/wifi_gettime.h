#ifndef TK_WIFI_GETTIME_H
#define TK_WIFI_GETTIME_H

extern msg_t slGetSNTPTime(uint32_t serverIP, char *hostName, RTCDateTime *timespec, int tz);

#endif

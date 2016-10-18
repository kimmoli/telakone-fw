#include <stdlib.h>
#include <string.h>
#include "wifi_scan.h"
#include "hal.h"
#include "wifi.h"
#include "helpers.h"

static const char *secNames[] = {SL_SEC_NAMES};

void slWifiScan(void)
{
    int numOfEntries;
    int res;
    uint8_t policyOpt;
    uint32_t policyVal;
    Sl_WlanNetworkEntry_t netEntries[SL_SCAN_TABLE_SIZE];

    PRINT("Scanning...");

    policyOpt = SL_CONNECTION_POLICY(0, 0, 0, 0, 0);

    res = sl_WlanPolicySet(SL_POLICY_CONNECTION , policyOpt, NULL, 0);

    if (res < 0)
    {
        PRINT(" failed\n\r");
        return;
    }

    policyOpt = SL_SCAN_POLICY(1);
    policyVal = SL_SCAN_INTERVAL;

    /* Start scanning */
    res = sl_WlanPolicySet(SL_POLICY_SCAN , policyOpt, (uint8_t *)&policyVal, sizeof(policyVal));

    if (res < 0)
    {
        PRINT(" failed\n\r");
        return;
    }

    chThdSleepSeconds(1);

    /* get scan results - all entries in one transaction */
    numOfEntries = sl_WlanGetNetworkList(0, SL_SCAN_TABLE_SIZE, &netEntries[0]);

    PRINT(" ok\n\r");

    for (int i=0; i<numOfEntries; i++)
    {
        PRINT("%2d %-20s %02x:%02x:%02x:%02x:%02x:%02x %-7s %4d\n\r", i, netEntries[i].ssid,
              netEntries[i].bssid[0], netEntries[i].bssid[1], netEntries[i].bssid[2],
              netEntries[i].bssid[3], netEntries[i].bssid[4], netEntries[i].bssid[5],
              secNames[netEntries[i].sec_type], netEntries[i].rssi);
    }

    /* disable scan */
    policyOpt = SL_SCAN_POLICY(0);
    res = sl_WlanPolicySet(SL_POLICY_SCAN , policyOpt, NULL, 0);

    if (res < 0)
    {
        PRINT("Failed to set the connection policy\n\r");
        return;
    }
}

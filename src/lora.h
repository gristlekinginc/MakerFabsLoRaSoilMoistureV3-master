#include <Arduino.h>
#include <lmic.h>
#include <helpers.h>
#include <LowPower.h>

extern void txCycle();

void onEvent(ev_t ev)
{
  Serial.print(os_getTime());
  Serial.print(F(": "));
  switch (ev)
  {
  case EV_SCAN_TIMEOUT:
    Serial.println(F("EV_SCAN_TIMEOUT"));
    break;
  case EV_BEACON_FOUND:
    Serial.println(F("EV_BEACON_FOUND"));
    break;
  case EV_BEACON_MISSED:
    Serial.println(F("EV_BEACON_MISSED"));
    break;
  case EV_BEACON_TRACKED:
    Serial.println(F("EV_BEACON_TRACKED"));
    break;
  case EV_JOINING:
    Serial.println(F("EV_JOINING"));
    break;
  case EV_JOINED:
    Serial.println(F("EV_JOINED"));
    {
      u4_t netid = 0;
      devaddr_t devaddr = 0;
      u1_t nwkKey[16];
      u1_t artKey[16];
      LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
      Serial.print(F("netid: "));
      Serial.println(netid, DEC);
      Serial.print(F("devaddr: "));
      Serial.println(devaddr, HEX);
      Serial.print(F("AppSKey: "));
      for (size_t i = 0; i < sizeof(artKey); ++i)
      {
        if (i != 0)
          Serial.print(F("-"));
        printHex2(artKey[i]);
      }
      Serial.println(F(""));
      Serial.print(F("NwkSKey: "));
      for (size_t i = 0; i < sizeof(nwkKey); ++i)
      {
        if (i != 0)
          Serial.print(F("-"));
        printHex2(nwkKey[i]);
      }
      Serial.println();
    }
    // Disable link check validation (automatically enabled
    // during join, but because slow data rates change max TX
    // size, we don't use it in this example.
    LMIC_setLinkCheckMode(0);
    break;
  /*
  || This event is defined but not used in the code. No
  || point in wasting codespace on it.
  ||
  || case EV_RFU1:
  ||     Serial.println(F("EV_RFU1"));
  ||     break;
  */
  case EV_JOIN_FAILED:
    Serial.println(F("EV_JOIN_FAILED"));
    break;
  case EV_REJOIN_FAILED:
    Serial.println(F("EV_REJOIN_FAILED"));
    break;
  case EV_TXCOMPLETE:
    Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
    if (LMIC.txrxFlags & TXRX_ACK)
      Serial.println(F("Received ack"));
    if (LMIC.dataLen)
    {
      Serial.print(F("Received "));
      Serial.print(LMIC.dataLen);
      Serial.println(F(" bytes of payload"));
    }
    // Schedule next transmission
    // os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);

    txCycle();
    break;
  case EV_LOST_TSYNC:
    Serial.println(F("EV_LOST_TSYNC"));
    break;
  case EV_RESET:
    Serial.println(F("EV_RESET"));
    break;
  case EV_RXCOMPLETE:
    // data received in ping slot
    Serial.println(F("EV_RXCOMPLETE"));
    break;
  case EV_LINK_DEAD:
    Serial.println(F("EV_LINK_DEAD"));
    break;
  case EV_LINK_ALIVE:
    Serial.println(F("EV_LINK_ALIVE"));
    break;
  /*
  || This event is defined but not used in the code. No
  || point in wasting codespace on it.
  ||
  || case EV_SCAN_FOUND:
  ||    Serial.println(F("EV_SCAN_FOUND"));
  ||    break;
  */
  case EV_TXSTART:
    Serial.println(F("EV_TXSTART"));
    break;
  case EV_TXCANCELED:
    Serial.println(F("EV_TXCANCELED"));
    break;
  case EV_RXSTART:
    /* do not print anything -- it wrecks timing */
    break;
  case EV_JOIN_TXCOMPLETE:
    Serial.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
    break;

  default:
    Serial.print(F("Unknown event: "));
    Serial.println((unsigned)ev);
    break;
  }
}

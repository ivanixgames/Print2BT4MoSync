/**
 * File: Print2BT.h
 *
 * Author: IvanU @ Ivanix Mobile LLC
 *
 * Description: Code for use with MoSync SDK (3.3) to provide Html5 Hybrid app with
 * javascript access to Bluetooth Printer with optional Magnetic Card Reader
 * Atm, only printers with SPP profile supported. Tested on Android platform only.
 *
 * Credit to Sam Pickard and Naveed Asif examples of MoSync BT api
 * http://www.mosync.com/docs/sdk/cpp/guides/bluetooth/discovering-devices-and-services-bluetooth/index.html
 */

#ifndef BluePrintPlus_H_
#define BluePrintPlus_H_

#include <MAUtil/BluetoothDiscovery.h>
#include <MAUtil/BluetoothConnection.h>

#include <MAUtil/MAUUID.h>
#include <conprint.h>
/*
#include <Wormhole/WebAppMoblet.h>>
*/
#include <Wormhole/HybridMoblet.h>
#include <Wormhole/Encoder.h>
using namespace MAUtil;
using namespace Wormhole;



class Print2BT :
	public BluetoothDeviceDiscoveryListener,
	public ConnectionListener
{
	public:
	Print2BT(HybridMoblet* webApp);
    virtual ~Print2BT();
	BluetoothConnection channel;

    // Callbacks Required By BluetoothDeviceDiscoveryListener
    void btNewDevice (const BtDevice &dev);
    void btDeviceDiscoveryFinished (int state);
    void connectFinished (Connection *conn, int result);
    void connRecvFinished (Connection *conn, int result);
    void connWriteFinished (Connection *conn, int result);
    void connReadFinished (Connection *conn, int result);

    void Discover();
    void Cancel();
    void Open(int idx, const MAUUID &uuidProfile);
    void Read();
    void ReadBase64();
    void Recv();
    void RecvBase64();


	private:
    HybridMoblet* webApp;

    BluetoothDiscoverer* dvcDiscover;
    bool btBusy;
    bool base64Conv;
    Vector<BtDevice> dvcVector;
	char readbuf[512];


    void jsStatus(const char *msg);
    void jsEvent(const char *event, const char *args);
    void dvcReset();

};


#endif /* BluePrintPlus_H_ */

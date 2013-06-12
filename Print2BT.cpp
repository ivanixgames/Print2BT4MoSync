/**
 * File: Print2BT.cpp
 * Author: IvanU @ Ivanix Mobile LLC
 *
 * Description: Code for use with MoSync SDK (3.3) to provide Html5 Hybrid app with
 * javascript access to Bluetooth Printer with optional Magnetic Card Reader
 *
 * Atm, only printers with SPP profile supported. Tested on Android platform only.
 *
 * Credit to Sam Pickard and Naveed Asif examples of MoSync BT api
 * http://www.mosync.com/docs/sdk/cpp/guides/bluetooth/discovering-devices-and-services-bluetooth/index.html
 */


#include "Print2BT.h"

//-------------------------------------------
Print2BT::Print2BT(HybridMoblet* webApp) :  channel(this), webApp(webApp)
{
    dvcDiscover = new BluetoothDiscoverer();
    btBusy = false;

}
void Print2BT::jsStatus(const char *msg)
{
	char buf[512];
	sprintf(
		buf,
		"Print2BT.EventSwitch('Status.Update','%s');",
		msg
		 );
	webApp->callJS(buf);
}
void Print2BT::jsEvent(const char *event, const char *args)
{
	char buf[512];
	sprintf(
		buf,
		"Print2BT.EventSwitch('%s'%s);",
		event,
		args
		 );
	webApp->callJS(buf);
}
void Print2BT::dvcReset()
{
    // Clear the Bluetooth device storage.
    Vector_each(BtDevice, itr, dvcVector)
    delete itr;
    dvcVector.clear();
}
// Callbacks Methods Required By BluetoothDeviceDiscoveryListener
void Print2BT::btNewDevice (const BtDevice &dev)
{

    dvcVector.add(dev);
	char buf[512];
	char addr[20];
	sprintf(addr,
			"%.2X:%.2X:%.2X:%.2X:%.2X:%.2X",
			dev.address.a[0],
			dev.address.a[1],
			dev.address.a[2],
			dev.address.a[3],
			dev.address.a[4],
			dev.address.a[5]
	);

	sprintf(
		buf,
		"Print2BT.EventSwitch('%s',%d,'%s','%s');",
		"Device.Found",
		dvcVector.size() - 1,
		dev.name.c_str(),
		addr
		 );
	webApp->callJS(buf);
	//dvcDiscover->cancel();
}
void Print2BT::btDeviceDiscoveryFinished (int state)
{
	//todo: check state for error (!=0)

    btBusy = false;
    jsEvent("Discover.Done","");
}

// Methods Called from JS
void Print2BT::Discover()
{
	// Search For New Devices
    if(!btBusy)
    {
        btBusy = true;
        dvcReset();
        dvcDiscover->startDeviceDiscovery(this, true);
    }
}
void Print2BT::Cancel()
{
	dvcDiscover->cancel();
}
void Print2BT::Open(int idx,  const MAUUID &uuidProfile)
{

	jsEvent("Open.Started","");
	base64Conv = false;
	int rc = channel.connect(dvcVector[idx].address, uuidProfile);
	if (rc < 0)
	{
		char ec[10];
		sprintf(	ec,", %i", rc);
		jsEvent("Open.FAIL", ec);
	}
	else
	{
		jsEvent("Open.Wait","");
	}
}
void Print2BT::Read()
{
	char buf[515];
	memset(&readbuf[0], 0, sizeof(readbuf));
	channel.read(&readbuf, 512);
	sprintf(buf,",'%s'",readbuf);
	jsEvent("Read.Now", buf);
}
void Print2BT::ReadBase64()
{
	base64Conv = true;
	Read();
}
void Print2BT::Recv()
{
	char buf[515];

	memset(&readbuf[0], 0, sizeof(readbuf));
	channel.recv(&readbuf, 512);
	sprintf(buf,",'%s'",readbuf);
	jsEvent("Recv.Now", buf);
}
void Print2BT::RecvBase64()
{
	base64Conv = true;
	Recv();
}
Print2BT::~Print2BT()
{
	// Destructor for the Print2BT class.
	dvcReset();
}
void Print2BT::connectFinished (Connection *conn, int result)
{
	char ec[10];
	sprintf(	ec,", %i", result);

	jsEvent("Open.Done", ec);
}
void Print2BT::connRecvFinished (Connection *conn, int result)
{

	char buf[515];
	Wormhole::Encoder  encoder;
	if (base64Conv)
	{
		sprintf(buf,",%d ,'%s'", result, encoder.base64Encode(readbuf, result).c_str());
		jsEvent("RecvBase64.Done", buf);
	} else {
		sprintf(buf,",%d ,'%s'", result, readbuf);
		jsEvent("Recv.Done", buf);
	}

	base64Conv = false;

}
void Print2BT::connWriteFinished (Connection *conn, int result)
{
	char ec[10];
	sprintf(	ec,", %i", result);
	jsEvent("Write.Done", ec);
}
void Print2BT::connReadFinished (Connection *conn, int result)
{
	char buf[515];
	Wormhole::Encoder  encoder;
	if (base64Conv)
	{
		sprintf(buf,",%d ,'%s'", result, encoder.base64Encode(readbuf, result).c_str());
		jsEvent("ReadBase64.Done", buf);
	} else {
		sprintf(buf,",%d ,'%s'", result, readbuf);
		jsEvent("Read.Done", buf);
	}
	base64Conv = false;

}

/*
 * File: Print2BT.js
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
var Print2BT = {
	_CallCPP: function() {
		var i, cmdlist;
		cmdlist = ['Custom','Print2BT'];
		for (i = 0; i < arguments.length; i++) {
			cmdlist.push(arguments[i]);
		}
		// alert('_CallCPP: ' + cmdlist);
		mosync.bridge.send(cmdlist);
		
	},
	Discover: function() {
		document.getElementById("status").innerHTML = 'Discover,JS';
		this._CallCPP('Discover');
	},
	Cancel: function() {
		document.getElementById("status").innerHTML = 'Cancel,JS';
		this._CallCPP('Cancel');
	},
	Open: function(idx, profile) {
		document.getElementById("status").innerHTML = 'Open,JS,' + profile;
		this._CallCPP('Open', idx.toString(), profile);
	},
	Write: function(str) {
		document.getElementById("status").innerHTML = 'Write,JS';
		this._CallCPP('Write', str);
	},
	Read: function() {
		document.getElementById("status").innerHTML = 'Read,JS';
		this._CallCPP('Read');
	},
	ReadBase64: function() {
		document.getElementById("status").innerHTML = 'Read,JS';
		this._CallCPP('ReadBase64');
	},
	Recv: function() {
		document.getElementById("status").innerHTML = 'Recv,JS';
		this._CallCPP('Recv');
	},
	RecvBase64: function() {
		document.getElementById("status").innerHTML = 'Recv,JS';
		this._CallCPP('RecvBase64');
	},
	Close: function() {
		document.getElementById("status").innerHTML = 'Close,JS';
		this._CallCPP('Close');
	},
	CCInit: function() {
		this.Write(String.fromCharCode(27,77,72));
	},
	CCRead: function() {
		this.Write(String.fromCharCode(27,77,73));
	},
	EventSwitch: function(event) {
		var args = Array.prototype.slice.call(arguments);
		
		document.getElementById("status").innerHTML = args.join('|');
		switch (event) 
		{
		case 'Discover.Begin':
			break;
		case 'Device.Found':
			var idx = arguments[1];	
			document.getElementById('bt' + idx).innerHTML = args.slice(1).join('|');			
			break;
		case 'Discover.Done':
			break;
		case 'ReadBase64.Done':
			var b64 = arguments[2];
			var bin = atob(b64);
			document.getElementById('status').innerHTML = "ReadBase64Done: (" + bin + ")" 
			break;
		case 'RecvBase64.Done':
			var b64 = arguments[2];
			var bin = atob(b64);
			document.getElementById('status').innerHTML = "RecvBase64Done: (" + bin + ")" 
			break;
		}
	}

	
};


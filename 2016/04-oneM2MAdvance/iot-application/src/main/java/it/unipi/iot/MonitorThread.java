package it.unipi.iot;

import java.net.SocketException;

public class MonitorThread extends Thread{
	public void run(){
		CoAPMonitor server;
		try {
			server = new CoAPMonitor();
			server.addEndpoints();
	    	server.start();
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}

package it.unipi.iot;

import java.io.IOException;
import java.net.InetSocketAddress;

import com.sun.net.httpserver.HttpServer;

public class MonitorThreadHTTP extends Thread{
	public void run(){
		HttpServer server;
		try {
			server = HttpServer.create(new InetSocketAddress(1440), 0);
			server.createContext("/monitor", new HTTPMonitor());
		    server.setExecutor(null); // creates a default executor
		    server.start();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	    
	}
}

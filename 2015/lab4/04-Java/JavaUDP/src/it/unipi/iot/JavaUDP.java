package it.unipi.iot;

import java.io.IOException;
import java.net.*;

public class JavaUDP {

	public static void main(String [] args){
		try {
			while(true){
				DatagramSocket sk = new DatagramSocket(1234);
				InetAddress ip = InetAddress.getByName("aaaa::c30c:0:0:0002");
				String message = "Hello";
				DatagramPacket packet = new DatagramPacket(message.getBytes(), message.length(), ip, 1234);
				sk.send(packet);
				sk.close();
				Thread.sleep(10000);
			}
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
}

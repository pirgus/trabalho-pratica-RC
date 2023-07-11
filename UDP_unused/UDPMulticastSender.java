import java.net.*;
public class UDPMulticastSender {


    public static void main(String args[]) {
       DatagramSocket socket;
       InetAddress group;
       byte[] buf;
	try {
		socket = new DatagramSocket();
		group = InetAddress.getByName("224.0.0.0");
		buf = "teste multicast".getBytes();

		DatagramPacket packet 
		  = new DatagramPacket(buf, buf.length, group, 4446);
		socket.send(packet);
		socket.close();
	} catch(Exception e) { 
	   e.printStackTrace();
	}
    }
 }

//Local: 224.0.0.0 a 224.0.0.255


import java.net.*;
public class UDPMulticastReceiver {

    public static void main(String args[]) {
    	MulticastSocket socket = null;
        byte[] buf = new byte[256];
    	try{
		socket = new MulticastSocket(4446);
		InetAddress group = InetAddress.getByName("224.0.0.0");
		socket.joinGroup(group);
		while (true) {
		    DatagramPacket packet = new DatagramPacket(buf, buf.length);
		    System.out.println("Aguardando...");
		    socket.receive(packet);
		    String received = new String(
		      packet.getData(), 0, packet.getLength());
		    System.out.println("from: "+packet.getAddress().toString()+": "+received);
		    if ("end".equals(received)) {
		        break;
		    }
		}
		socket.leaveGroup(group);
		socket.close();
        } catch(Exception e) {
            e.printStackTrace(); 
        }
    }
    
}

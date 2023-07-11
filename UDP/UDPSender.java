import java.io.*;
import java.net.*;

class UDPSender {
    public static void main(String args[]) throws Exception {
        InputStream fileInputStream = null;
        try {
            fileInputStream = new FileInputStream("../arquivo.txt");

            DatagramSocket clientSocket = new DatagramSocket();

            InetAddress IPAddress = InetAddress.getByName("localhost");

            byte[] sendData = new byte[100]; // Tamanho do segmento de dados
            byte[] receiveData = new byte[1024];

            int bytesRead;
            while ((bytesRead = fileInputStream.read(sendData)) != -1) {
                DatagramPacket sendPacket =
                        new DatagramPacket(sendData, bytesRead, IPAddress, 9876);
                clientSocket.send(sendPacket);
            }

            DatagramPacket receivePacket =
                    new DatagramPacket(receiveData, receiveData.length);

            clientSocket.receive(receivePacket);

            String modifiedSentence = new String(receivePacket.getData(), 0, receivePacket.getLength());

            System.out.println("FROM SERVER: " + modifiedSentence);

            clientSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (fileInputStream != null) {
                try {
                    fileInputStream.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}


import java.awt.Color;
import java.awt.BorderLayout;
import java.awt.event.*;
import javax.swing.*;

import java.io.*;
import java.net.*;

class ClientWorker implements Runnable {
  private Socket client;
  private JTextArea textArea;
  private SocketThrdServer server;
  ClientWorker(Socket client, JTextArea textArea, SocketThrdServer server) {
   this.client = client;
   this.textArea = textArea;
   this.server = server;
  }
  public void run(){
    String line;
    BufferedReader in = null;
    PrintWriter out = null;
    try{
      in = new BufferedReader(new InputStreamReader(client.getInputStream()));
      out = new PrintWriter(client.getOutputStream(), true);
    } catch (IOException e) {
      System.out.println("in or out failed");
      System.exit(-1);
    }

    while(true){
      try{
        line = in.readLine();
//Send data back to client
         out.println(line);
         textArea.append(line+"\n");
         server.broadCast(this, line);
       } catch (IOException e) {
         System.out.println("Read failed");
         System.exit(-1);
       }
    }
  }
  public void sendMessage(String message){
    PrintWriter out = null;
    try{
      out = new PrintWriter(client.getOutputStream(), true);
      System.out.println("out created");
      out.println(message);
    } catch (IOException e) {
      System.out.println("out failed");
    }
  }
}


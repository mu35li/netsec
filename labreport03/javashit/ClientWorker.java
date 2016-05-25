
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
  private boolean isLoggedIn = false;
  private int failedAttempts = 0;
  ClientWorker(Socket client, JTextArea textArea, SocketThrdServer server) {
   this.client = client;
   this.textArea = textArea;
   this.server = server;
 }
 public void run(){
  System.out.println(client.toString());
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
      System.out.println(line);
      if (line.equals("register")) {
        out.println("Please enter a username");
        String username = in.readLine();
        out.println("Please enter a password");
        String password = in.readLine();
        textArea.append("User wants to register with username: "+username+" and password "+password+"\n");
        boolean register = server.register(this, username, password);
        if (register == true) {
          out.println("registered successfully");
        }
      }else if(line.equals("login")){
        if (failedAttempts >=3) {
          out.println("Too many failed attempts");
        }else{
          out.println("Please enter a username");
          String username = in.readLine();
          out.println("Please enter a password");
          String password = in.readLine();
          textArea.append("User wants to login with username: "+username+" and password "+password+"\n");
          boolean login = server.login(this, username, password);
          if (login == true) {
            this.isLoggedIn = true;
            out.println("logged in successfully");
          }else{
            this.failedAttempts ++;
            if (failedAttempts >=3) {
              out.println("Login has been blocked");
            }
            out.println("login failed");
          }
        }
      }else{
//Send data back to client
      if (line != null && this.isLoggedIn) {
       out.println(line);
       textArea.append(line+"\n");
       server.broadCast(this, line);
     }else{
      out.println("Please login to chat");
     }
   }
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
protected void finalize(){
    //Objects created in run method are finalized when 
    //program terminates and thread exits
  server.unlistenWorker(this);
}

public String getIP(){
    return this.client.getInetAddress().toString();
}

}


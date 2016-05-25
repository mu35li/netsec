import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.io.IOException;
import java.net.ServerSocket;
import java.util.*;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.Timer;
import javax.swing.Timer;

class SocketThrdServer extends JFrame{

    JLabel label = new JLabel("Text received over socket:");
    JPanel panel;
    JTextArea textArea = new JTextArea();
    ServerSocket server = null;
    private List<ClientWorker> workers = new ArrayList<ClientWorker>();
    private HashMap connectedIPs = new HashMap();

       SocketThrdServer(){ //Begin Constructor
        panel = new JPanel();
        panel.setLayout(new BorderLayout());
        panel.setBackground(Color.white);
        getContentPane().add(panel);
        panel.add("North", label);
        panel.add("Center", textArea);
       } //End Constructor

       public void listenSocket(){
        try{
            server = new ServerSocket(4444); 
        } catch (IOException e) {
            System.out.println("Could not listen on port 4444");
            System.exit(-1);
        }
        while(true){
            ClientWorker w;
            try{
                w = new ClientWorker(server.accept(), textArea, this);
                System.out.println(w.toString());
                Thread t = new Thread(w);
                t.start();
                workers.add(w);
                String ip = w.getIP();
                connectedIPs.put(ip, true);
                System.out.println("ip is: "+ip);
                System.out.println("Added worker to list");
            } catch (IOException e) {
                System.out.println("Accept failed: 4444");
                System.exit(-1);
            }
        }
    }

    protected void finalize(){
    //Objects created in run method are finalized when 
    //program terminates and thread exits
        try{
            server.close();
        } catch (IOException e) {
            System.out.println("Could not close socket");
            System.exit(-1);
        }
    }

    public static void main(String[] args){
        SocketThrdServer frame = new SocketThrdServer();
        frame.setTitle("Server Program");
        WindowListener l = new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                System.exit(0);
            }
        };
        frame.addWindowListener(l);
        frame.pack();
        frame.setVisible(true);
        frame.listenSocket();
    }

    public void broadCast(ClientWorker client, String message) {
        if (message != null) {
            for (ClientWorker worker : workers) {
                if (!(worker.equals(client))) {
                    System.out.println("sending Message "+message+" to "+client.toString());
                    worker.sendMessage(message);
                }
            }
        }
    }

    public void unlistenWorker(ClientWorker client) {

        workers.remove(client);
    }

    public boolean register(ClientWorker worker, String username, String password){
        Useradmin useradminObj = new Useradmin();
        useradminObj.addUser(username, password);
        return true;
    }

    public boolean login(ClientWorker worker, String username, String password){
        Useradmin useradminObj = new Useradmin();
        boolean check = useradminObj.checkUser(username, password);
        
        final String ip = worker.getIP();
        if (connectedIPs.get(ip) == true) {
            connectedIPs.put(ip, false);
            System.out.println("IP blocked: "+ip);
            ActionListener unblockTimer = new ActionListener() {
              public void actionPerformed(ActionEvent evt) {
                unblockIP(ip);
              }
            };
            Timer timer = new Timer(10000, unblockTimer);
            timer.setRepeats(false);
            timer.start();
            if (check == true) {
                return true;
            }else{
                return false;
            }
        }else{
            System.out.println("blocked ip tried to login");
            return false;
        }
    }

    private void unblockIP(String ip) {
        System.out.println("IP unblocked: "+ip);
        connectedIPs.put(ip, true);
    }
}

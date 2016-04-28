import java.io.*;

public class Useradmin implements Useradministration
{
    String file = "passwords.txt";

    public Useradmin()
    {
        
    }
    
    public void addUser(String username, char[] password) {
        BCrypt bcryptObj = new BCrypt();
        String passwordString = new String(password);
        String pw_hash = BCrypt.hashpw(passwordString, BCrypt.gensalt());
        String line = username + ' ' + pw_hash;
        try {
            PrintWriter writer = new PrintWriter(
            new FileOutputStream (
            new File(file), 
            true /* append = true */));
            writer.println(line);
            writer.close();
        } catch (IOException e) {
            
        }
        
    }
    
    public boolean checkUser(String username, char[] password) {
        try(BufferedReader br = new BufferedReader(new FileReader(file))) {
            for(String line; (line = br.readLine()) != null; ) {
                String[] parts = line.split(" ");
                String passwordString = new String(password);
            
                if (parts[0].equals(username)){
                    return BCrypt.checkpw(passwordString, parts[1]);
                }
            }
        } catch (FileNotFoundException e) {
        
        } catch (IOException e) {
        
        }
        return false;
    }
}

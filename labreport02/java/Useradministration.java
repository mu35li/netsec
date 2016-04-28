
/**
 * Write a description of interface Useradministration here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public interface Useradministration
{
    public void addUser(String username, char[] password);
    public boolean checkUser(String username, char[] password);
}

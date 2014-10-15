package cs3041.project4;

import java.util.ArrayList;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlElementWrapper;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement(name = "user")
public class User {
    private String firstName;
    private String lastName;
    private String email;
    private String password;
    private ArrayList<CD> wishlist;
    
    public User() {
        this.firstName = null;
        this.lastName = null;
        this.email = null;
        this.password = null;
        this.wishlist = new ArrayList<CD>();
    }

    public String getFirstName() {
        return firstName;
    }

    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }

    public String getLastName() {
        return lastName;
    }

    public void setLastName(String lastName) {
        this.lastName = lastName;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    @XmlElementWrapper(name = "wishlist")
    @XmlElement(name = "cd")
    public ArrayList<CD> getWishlist() {
        return wishlist;
    }

    public void setWishlist(ArrayList<CD> wishlist) {
        this.wishlist = wishlist;
    }
    
    
}

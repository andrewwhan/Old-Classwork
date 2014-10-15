package cs3041.project4;

import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;

@XmlRootElement(name = "cd")
@XmlType(propOrder = {"genre", "artist", "title"})
public class CD {
    private String genre;
    private String artist;
    private String title;
    
    public CD(){
        this.genre = null;
        this.artist = null;
        this.title = null;
    }
    
    public CD(String genre, String artist, String title){
        this.genre = genre;
        this.artist = artist;
        this.title = title;
    }
    
    public String getGenre() {
        return genre;
    }
    
    public void setGenre(String genre) {
        this.genre = genre;
    }
    
    public String getArtist() {
        return artist;
    }
    
    public void setArtist(String artist) {
        this.artist = artist;
    }
    
    public String getTitle() {
        return title;
    }
    
    public void setTitle(String title) {
        this.title = title;
    }
    
}

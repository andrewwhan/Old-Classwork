package cs3041.project4;

import java.util.ArrayList;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement(name = "catalog")
public class Catalog {
    
    private ArrayList<CD> cdList;
    
    public Catalog(){
        this.cdList = new ArrayList<CD>();
    }

    @XmlElement(name = "cd")
    public ArrayList<CD> getCdList() {
        return cdList;
    }

    public void setCdList(ArrayList<CD> cdList) {
        this.cdList = cdList;
    }
    
}

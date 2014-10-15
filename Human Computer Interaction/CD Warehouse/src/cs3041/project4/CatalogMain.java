package cs3041.project4;

import java.io.File;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Unmarshaller;

public class CatalogMain {

    public static void main(String args[]) throws JAXBException{
        JAXBContext catalogContext  = JAXBContext.newInstance(Catalog.class);
        Unmarshaller catalogUm = catalogContext.createUnmarshaller();
        Catalog listofcds = (Catalog) catalogUm.unmarshal(new File("catalog.xml"));
        JAXBContext userContext = JAXBContext.newInstance(User.class);
        Unmarshaller userUm = userContext.createUnmarshaller();
        User andrew = (User) userUm.unmarshal(new File("exampleuser.xml"));
        CatalogFrame view = new CatalogFrame(listofcds, andrew);
    }
}

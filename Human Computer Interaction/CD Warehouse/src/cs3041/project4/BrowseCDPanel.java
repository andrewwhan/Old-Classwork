package cs3041.project4;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JPanel;

import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.ColumnSpec;
import com.jgoodies.forms.layout.RowSpec;
import com.jgoodies.forms.factories.FormFactory;

import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JButton;
import java.awt.Font;

public class BrowseCDPanel extends JPanel{
    /**
     * 
     */
    private static final long serialVersionUID = 1L;
    private JTextField quantityField;
    private JLabel lblAlbumTitle;
    private JButton btnAddToWishlist;
    private JLabel lblRegisterToCreate;
    public BrowseCDPanel() {
        setLayout(new FormLayout(new ColumnSpec[] {
                FormFactory.RELATED_GAP_COLSPEC,
                FormFactory.DEFAULT_COLSPEC,
                FormFactory.RELATED_GAP_COLSPEC,
                FormFactory.DEFAULT_COLSPEC,
                FormFactory.RELATED_GAP_COLSPEC,
                FormFactory.DEFAULT_COLSPEC,
                FormFactory.RELATED_GAP_COLSPEC,
                ColumnSpec.decode("default:grow"),},
            new RowSpec[] {
                FormFactory.RELATED_GAP_ROWSPEC,
                FormFactory.DEFAULT_ROWSPEC,
                FormFactory.RELATED_GAP_ROWSPEC,
                FormFactory.DEFAULT_ROWSPEC,
                FormFactory.RELATED_GAP_ROWSPEC,
                RowSpec.decode("max(30dlu;default)"),}));
        
        lblAlbumTitle = new JLabel("Album Title");
        lblAlbumTitle.setFont(new Font("Tahoma", Font.BOLD, 14));
        add(lblAlbumTitle, "2, 2, 7, 1");
        
        JLabel lblQuantity = new JLabel("Quantity");
        add(lblQuantity, "2, 4, right, default");
        
        quantityField = new JTextField();
        quantityField.setText("0");
        add(quantityField, "4, 4, center, default");
        quantityField.setColumns(2);
        
        JButton btnPurchase = new JButton("Purchase");
        add(btnPurchase, "6, 4");
        
        btnAddToWishlist = new JButton("Add to Wishlist");
        add(btnAddToWishlist, "6, 6");
        
        lblRegisterToCreate = new JLabel("*Register to create a wishlist");
        add(lblRegisterToCreate, "8, 6");
        lblRegisterToCreate.setVisible(false);
    }
    
    public BrowseCDPanel(final CD cd, boolean registered, boolean first) {
        this();
        lblAlbumTitle.setText(cd.getTitle());
        btnAddToWishlist.setEnabled(registered);
        btnAddToWishlist.addActionListener(new ActionListener(){

            @Override
            public void actionPerformed(ActionEvent e) {
                if(!CatalogFrame.loggedInUser.getWishlist().contains(cd)){
                    CatalogFrame.loggedInUser.getWishlist().add(cd);
                }
                CatalogFrame.populateWishlist();
            }
            
        });
        lblRegisterToCreate.setVisible(!registered && first);
    }
    
}

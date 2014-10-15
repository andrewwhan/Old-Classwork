package cs3041.project4;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import com.jgoodies.forms.factories.FormFactory;
import com.jgoodies.forms.layout.ColumnSpec;
import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.RowSpec;
import java.awt.Font;

public class WishlistCDPanel extends JPanel {
    /**
     * 
     */
    private static final long serialVersionUID = 1L;
    private JTextField quantityField;
    private JLabel lblAlbumTitle;
    private JLabel lblArtist;
    public WishlistCDPanel() {
        setLayout(new FormLayout(new ColumnSpec[] {
                FormFactory.RELATED_GAP_COLSPEC,
                ColumnSpec.decode("max(75dlu;default)"),
                FormFactory.RELATED_GAP_COLSPEC,
                FormFactory.DEFAULT_COLSPEC,
                FormFactory.RELATED_GAP_COLSPEC,
                FormFactory.DEFAULT_COLSPEC,
                FormFactory.RELATED_GAP_COLSPEC,
                FormFactory.DEFAULT_COLSPEC,},
            new RowSpec[] {
                FormFactory.RELATED_GAP_ROWSPEC,
                FormFactory.DEFAULT_ROWSPEC,
                FormFactory.RELATED_GAP_ROWSPEC,
                FormFactory.DEFAULT_ROWSPEC,
                FormFactory.RELATED_GAP_ROWSPEC,
                FormFactory.DEFAULT_ROWSPEC,}));
        
        lblAlbumTitle = new JLabel("Album Title");
        lblAlbumTitle.setFont(new Font("Tahoma", Font.BOLD, 14));
        add(lblAlbumTitle, "2, 2, 7, 1, left, default");
        
        lblArtist = new JLabel("Artist");
        add(lblArtist, "2, 4, left, default");
        
        JLabel lblQuantity = new JLabel("Quantity");
        add(lblQuantity, "2, 6, left, default");
        
        quantityField = new JTextField();
        quantityField.setText("0");
        add(quantityField, "6, 6, left, default");
        quantityField.setColumns(2);
        
        JButton btnPurchase = new JButton("Purchase");
        add(btnPurchase, "8, 6, left, default");
    }
    
    public WishlistCDPanel(CD cd) {
        this();
        lblAlbumTitle.setText(cd.getTitle());
        lblArtist.setText(cd.getArtist());
    }
    
}

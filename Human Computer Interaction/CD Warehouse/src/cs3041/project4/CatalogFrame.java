package cs3041.project4;

import java.awt.CardLayout;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

import java.awt.Dimension;

import javax.swing.JLabel;

import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.Insets;

import javax.swing.JTextField;
import javax.swing.JPasswordField;
import javax.swing.JButton;
import javax.swing.JSplitPane;
import javax.swing.JScrollPane;

import com.jgoodies.forms.layout.FormLayout;
import com.jgoodies.forms.layout.ColumnSpec;
import com.jgoodies.forms.factories.FormFactory;
import com.jgoodies.forms.layout.RowSpec;

import javax.swing.SwingConstants;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

import org.eclipse.wb.swing.FocusTraversalOnArray;

import java.awt.Component;

import javax.swing.BoxLayout;
import java.awt.Font;

public class CatalogFrame extends JFrame{
    Catalog catalog;
    User example;
    boolean loggedIn;
    static User loggedInUser;
    ArrayList<String> genres;
    
    public CatalogFrame(Catalog catalog, final User example) {
        this.catalog = catalog;
        this.example = example;
        this.loggedIn = false;
        CatalogFrame.loggedInUser = null;
        this.genres = new ArrayList<String>();
        for(CD cd:catalog.getCdList()){
            if(!genres.contains(cd.getGenre())){
                genres.add(cd.getGenre());
            }
        }
        Collections.sort(genres);
        setTitle("CD Warehouse Catalog");
        setSize(new Dimension(800, 600));
        setPreferredSize(new Dimension(800, 600));
        final CardLayout mainCard = new CardLayout(0, 0);
        getContentPane().setLayout(mainCard);
        
        JPanel logInPanel = new JPanel();
        getContentPane().add(logInPanel, "name_46115376950475");
        GridBagLayout gbl_logInPanel = new GridBagLayout();
        gbl_logInPanel.columnWidths = new int[]{50, 0, 335, 67, 50};
        gbl_logInPanel.rowHeights = new int[] {50, 44, 30, 30, 30, 0, 0, 0, 0};
        gbl_logInPanel.columnWeights = new double[]{1.0, 0.0, 0.0, 0.0, 1.0};
        gbl_logInPanel.rowWeights = new double[]{1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, Double.MIN_VALUE};
        logInPanel.setLayout(gbl_logInPanel);
        
        JLabel lblCatalog = new JLabel("CD Warehouse");
        lblCatalog.setFont(new Font("Tahoma", Font.PLAIN, 24));
        GridBagConstraints gbc_lblCatalog = new GridBagConstraints();
        gbc_lblCatalog.gridwidth = 5;
        gbc_lblCatalog.fill = GridBagConstraints.VERTICAL;
        gbc_lblCatalog.insets = new Insets(0, 0, 5, 0);
        gbc_lblCatalog.gridx = 0;
        gbc_lblCatalog.gridy = 1;
        logInPanel.add(lblCatalog, gbc_lblCatalog);
        
        JLabel lblEmail = new JLabel("E-mail:");
        GridBagConstraints gbc_lblEmail = new GridBagConstraints();
        gbc_lblEmail.anchor = GridBagConstraints.EAST;
        gbc_lblEmail.fill = GridBagConstraints.VERTICAL;
        gbc_lblEmail.insets = new Insets(0, 0, 5, 5);
        gbc_lblEmail.gridx = 1;
        gbc_lblEmail.gridy = 2;
        logInPanel.add(lblEmail, gbc_lblEmail);
        
        emailField = new JTextField();
        GridBagConstraints gbc_emailField = new GridBagConstraints();
        gbc_emailField.insets = new Insets(0, 0, 5, 5);
        gbc_emailField.fill = GridBagConstraints.HORIZONTAL;
        gbc_emailField.gridx = 2;
        gbc_emailField.gridy = 2;
        logInPanel.add(emailField, gbc_emailField);
        emailField.setColumns(10);
        
        JLabel lblPassword = new JLabel("Password:");
        GridBagConstraints gbc_lblPassword = new GridBagConstraints();
        gbc_lblPassword.anchor = GridBagConstraints.EAST;
        gbc_lblPassword.fill = GridBagConstraints.VERTICAL;
        gbc_lblPassword.insets = new Insets(0, 0, 5, 5);
        gbc_lblPassword.gridx = 1;
        gbc_lblPassword.gridy = 3;
        logInPanel.add(lblPassword, gbc_lblPassword);
        
        passField = new JPasswordField();
        GridBagConstraints gbc_passField = new GridBagConstraints();
        gbc_passField.insets = new Insets(0, 0, 5, 5);
        gbc_passField.fill = GridBagConstraints.HORIZONTAL;
        gbc_passField.gridx = 2;
        gbc_passField.gridy = 3;
        logInPanel.add(passField, gbc_passField);
        
        JButton btnLogIn = new JButton("Log In");
        btnLogIn.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if(emailField.getText().equals(example.getEmail()) && Arrays.equals(passField.getPassword(), example.getPassword().toCharArray())){
                    loggedIn = true;
                    loggedInUser = example;
                    populateWishlist();
                    mainCard.next(getContentPane());
                }
            }
        });
        GridBagConstraints gbc_btnLogIn = new GridBagConstraints();
        gbc_btnLogIn.insets = new Insets(0, 0, 5, 5);
        gbc_btnLogIn.gridx = 3;
        gbc_btnLogIn.gridy = 3;
        logInPanel.add(btnLogIn, gbc_btnLogIn);
        
        JLabel lblDontHaveAn = new JLabel("Don't have an account?");
        GridBagConstraints gbc_lblDontHaveAn = new GridBagConstraints();
        gbc_lblDontHaveAn.anchor = GridBagConstraints.EAST;
        gbc_lblDontHaveAn.insets = new Insets(0, 0, 5, 5);
        gbc_lblDontHaveAn.gridx = 1;
        gbc_lblDontHaveAn.gridy = 5;
        logInPanel.add(lblDontHaveAn, gbc_lblDontHaveAn);
        
        JButton btnRegister = new JButton("Register");
        btnRegister.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                populateWishlist();
                mainCard.next(getContentPane());
                tabbedPane.setSelectedComponent(registerTab);
            }
        });
        GridBagConstraints gbc_btnRegister = new GridBagConstraints();
        gbc_btnRegister.anchor = GridBagConstraints.WEST;
        gbc_btnRegister.insets = new Insets(0, 0, 5, 5);
        gbc_btnRegister.gridx = 2;
        gbc_btnRegister.gridy = 5;
        logInPanel.add(btnRegister, gbc_btnRegister);
        
        JLabel lblOrContinueAs = new JLabel("Or, continue as a ");
        GridBagConstraints gbc_lblOrContinueAs = new GridBagConstraints();
        gbc_lblOrContinueAs.anchor = GridBagConstraints.EAST;
        gbc_lblOrContinueAs.insets = new Insets(0, 0, 5, 5);
        gbc_lblOrContinueAs.gridx = 1;
        gbc_lblOrContinueAs.gridy = 6;
        logInPanel.add(lblOrContinueAs, gbc_lblOrContinueAs);
        
        JButton btnGuest = new JButton("Guest");
        btnGuest.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                populateWishlist();
                mainCard.next(getContentPane());
            }
        });
        GridBagConstraints gbc_btnGuest = new GridBagConstraints();
        gbc_btnGuest.anchor = GridBagConstraints.WEST;
        gbc_btnGuest.insets = new Insets(0, 0, 5, 5);
        gbc_btnGuest.gridx = 2;
        gbc_btnGuest.gridy = 6;
        logInPanel.add(btnGuest, gbc_btnGuest);
        logInPanel.setFocusTraversalPolicy(new FocusTraversalOnArray(new Component[]{emailField, passField, btnRegister, btnGuest}));
        
        tabbedPane = new JTabbedPane(JTabbedPane.TOP);
        tabbedPane.setFont(new Font("Tahoma", Font.PLAIN, 16));
        getContentPane().add(tabbedPane, "name_46118966034563");
        
        final JSplitPane browseTab = new JSplitPane();
        tabbedPane.addTab("Browse", null, browseTab, null);
        
        genreLevelCard = new JPanel();
        browseTab.setRightComponent(genreLevelCard);
        genreLevelCard.setLayout(new CardLayout(0, 0));
        
        JPanel genreHelp = new JPanel();
        genreLevelCard.add(genreHelp, "name_49271656923177");
        genreHelp.setLayout(new FormLayout(new ColumnSpec[] {
                ColumnSpec.decode("25px"),
                ColumnSpec.decode("158px:grow"),
                FormFactory.LABEL_COMPONENT_GAP_COLSPEC,
                ColumnSpec.decode("121px"),
                FormFactory.LABEL_COMPONENT_GAP_COLSPEC,
                ColumnSpec.decode("128px:grow"),},
                new RowSpec[] {
                FormFactory.RELATED_GAP_ROWSPEC,
                FormFactory.DEFAULT_ROWSPEC,
                FormFactory.RELATED_GAP_ROWSPEC,
                RowSpec.decode("default:grow"),
                FormFactory.RELATED_GAP_ROWSPEC,
                FormFactory.DEFAULT_ROWSPEC,
                FormFactory.RELATED_GAP_ROWSPEC,
                RowSpec.decode("default:grow"),}));
        
        JLabel lblWelcome = new JLabel("Welcome!");
        lblWelcome.setFont(new Font("Tahoma", Font.PLAIN, 24));
        genreHelp.add(lblWelcome, "4, 2, center, top");
        
        JLabel lblToFindA = new JLabel("To find a CD, pick a genre.");
        lblToFindA.setFont(new Font("Tahoma", Font.PLAIN, 16));
        genreHelp.add(lblToFindA, "2, 6, left, top");
        
        JSplitPane artistCardSplit = new JSplitPane();
        genreLevelCard.add(artistCardSplit, "name_49289801183148");
        
        artistLevelCard = new JPanel();
        artistCardSplit.setRightComponent(artistLevelCard);
        artistLevelCard.setLayout(new CardLayout(0, 0));
        
        artistHelp = new JPanel();
        artistLevelCard.add(artistHelp, "name_49473276051370");
        artistHelp.setLayout(new FormLayout(new ColumnSpec[] {
                ColumnSpec.decode("25px"),
                ColumnSpec.decode("194px:grow"),},
                new RowSpec[] {
                FormFactory.LINE_GAP_ROWSPEC,
                RowSpec.decode("14px:grow"),}));
        
        JLabel lblNowYouCan = new JLabel("Now, you can select an artist.");
        lblNowYouCan.setFont(new Font("Tahoma", Font.PLAIN, 16));
        artistHelp.add(lblNowYouCan, "2, 2, left, center");
        
        JScrollPane albums = new JScrollPane();
        artistLevelCard.add(albums, "name_51087521067910");
        
        albumContents = new JPanel();
        albums.setViewportView(albumContents);
        albumContents.setLayout(new BoxLayout(albumContents, BoxLayout.Y_AXIS));
        
        JScrollPane artistColumn = new JScrollPane();
        artistColumn.setMinimumSize(new Dimension(150, 23));
        artistColumn.setPreferredSize(new Dimension(150, 2));
        artistCardSplit.setLeftComponent(artistColumn);
        
        JLabel lblArtist = new JLabel("Artist");
        lblArtist.setFont(new Font("Tahoma", Font.PLAIN, 16));
        lblArtist.setHorizontalAlignment(SwingConstants.CENTER);
        artistColumn.setColumnHeaderView(lblArtist);
        
        artistContents = new JPanel();
        artistColumn.setViewportView(artistContents);
        artistContents.setLayout(new BoxLayout(artistContents, BoxLayout.Y_AXIS));
        
        JScrollPane genreColumn = new JScrollPane();
        genreColumn.setMinimumSize(new Dimension(150, 23));
        genreColumn.setPreferredSize(new Dimension(150, 2));
        browseTab.setLeftComponent(genreColumn);
        
        JLabel lblGenre = new JLabel("Genre");
        lblGenre.setFont(new Font("Tahoma", Font.PLAIN, 16));
        lblGenre.setHorizontalAlignment(SwingConstants.CENTER);
        genreColumn.setColumnHeaderView(lblGenre);
        
        JPanel genreContents = new JPanel();
        for(final String g:genres){
            JButton genreButton = new JButton(g);
            genreButton.addActionListener(new ActionListener(){
                
                @Override
                public void actionPerformed(ActionEvent e) {
                    displayArtists(g);
                }
                
            });
            genreContents.add(genreButton);
        }
        genreColumn.setViewportView(genreContents);
        genreContents.setLayout(new BoxLayout(genreContents, BoxLayout.Y_AXIS));
        
        JPanel wishlistTab = new JPanel();
        tabbedPane.addTab("Wishlist", null, wishlistTab, null);
        wishlistTab.setLayout(new FormLayout(new ColumnSpec[] {
                FormFactory.DEFAULT_COLSPEC,
                FormFactory.RELATED_GAP_COLSPEC,
                FormFactory.DEFAULT_COLSPEC,
                FormFactory.RELATED_GAP_COLSPEC,
                ColumnSpec.decode("388px:grow"),
                ColumnSpec.decode("2px"),},
                new RowSpec[] {
                FormFactory.LINE_GAP_ROWSPEC,
                RowSpec.decode("30px:grow"),
                FormFactory.RELATED_GAP_ROWSPEC,
                FormFactory.DEFAULT_ROWSPEC,
                FormFactory.RELATED_GAP_ROWSPEC,
                FormFactory.DEFAULT_ROWSPEC,}));
        
        JScrollPane wishlistPane = new JScrollPane();
        wishlistTab.add(wishlistPane, "1, 2, 5, 1, fill, fill");
        
        wishlistContents = new JPanel();
        wishlistPane.setViewportView(wishlistContents);
        wishlistContents.setLayout(new BoxLayout(wishlistContents, BoxLayout.Y_AXIS));
        
        JButton btnBack = new JButton("Back");
        btnBack.addActionListener(new ActionListener(){
            
            @Override
            public void actionPerformed(ActionEvent e) {
                tabbedPane.setSelectedComponent(browseTab);
            }
            
        });
        wishlistTab.add(btnBack, "3, 4");
        
        registerTab = new JPanel();
        tabbedPane.addTab("Register", null, registerTab, null);
        registerTab.setLayout(new FormLayout(new ColumnSpec[] {
                ColumnSpec.decode("90px:grow"),
                ColumnSpec.decode("125px"),
                FormFactory.LABEL_COMPONENT_GAP_COLSPEC,
                ColumnSpec.decode("54px:grow"),
                FormFactory.LABEL_COMPONENT_GAP_COLSPEC,
                ColumnSpec.decode("90px:grow"),},
                new RowSpec[] {
                FormFactory.RELATED_GAP_ROWSPEC,
                RowSpec.decode("default:grow"),
                FormFactory.LINE_GAP_ROWSPEC,
                RowSpec.decode("30px"),
                FormFactory.RELATED_GAP_ROWSPEC,
                RowSpec.decode("30px"),
                FormFactory.RELATED_GAP_ROWSPEC,
                RowSpec.decode("30px"),
                FormFactory.RELATED_GAP_ROWSPEC,
                RowSpec.decode("30px"),
                FormFactory.RELATED_GAP_ROWSPEC,
                RowSpec.decode("30px"),
                FormFactory.RELATED_GAP_ROWSPEC,
                RowSpec.decode("default:grow"),}));
        
        JLabel lblRegisterANew = new JLabel("Register a new account");
        lblRegisterANew.setFont(new Font("Tahoma", Font.PLAIN, 16));
        registerTab.add(lblRegisterANew, "4, 2, left, default");
        
        JLabel lblFirstName = new JLabel("First Name:");
        registerTab.add(lblFirstName, "2, 4, right, center");
        
        regFirNameField = new JTextField();
        registerTab.add(regFirNameField, "4, 4, fill, default");
        regFirNameField.setColumns(10);
        
        JLabel lblLastName = new JLabel("Last Name:");
        registerTab.add(lblLastName, "2, 6, right, center");
        
        regLasNameField = new JTextField();
        registerTab.add(regLasNameField, "4, 6, fill, default");
        regLasNameField.setColumns(10);
        
        JLabel lblEmail_1 = new JLabel("E-mail:");
        registerTab.add(lblEmail_1, "2, 8, right, center");
        
        regEmailField = new JTextField();
        registerTab.add(regEmailField, "4, 8, fill, default");
        regEmailField.setColumns(10);
        
        JLabel lblPassword_1 = new JLabel("Password:");
        registerTab.add(lblPassword_1, "2, 10, right, center");
        
        JButton btnRegister_1 = new JButton("Register");
        btnRegister_1.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                loggedIn = true;
                loggedInUser = new User();
                loggedInUser.setFirstName(regFirNameField.getText());
                loggedInUser.setLastName(regLasNameField.getText());
                loggedInUser.setEmail(regEmailField.getText());
                loggedInUser.setPassword(regPassField.getPassword().toString());
                loggedInUser.setWishlist(new ArrayList<CD>());
                populateWishlist();
                tabbedPane.setSelectedComponent(browseTab);
            }
        });
        
        regPassField = new JPasswordField();
        registerTab.add(regPassField, "4, 10, fill, default");
        registerTab.add(btnRegister_1, "4, 12, left, default");
        
        this.setVisible(true);
    }
    
    private void displayArtists(String genre) {
        artistContents.removeAll();
        ((CardLayout)artistLevelCard.getLayout()).first(artistLevelCard);
        ((CardLayout)genreLevelCard.getLayout()).last(genreLevelCard);
        ArrayList<String> artists = new ArrayList<String>();
        for(CD cd:catalog.getCdList()){
            if(cd.getGenre().equals(genre) && !artists.contains(cd.getArtist())){
                artists.add(cd.getArtist());
            }
        }
        Collections.sort(artists);
        for(final String a:artists){
            JButton artistButton = new JButton(a);
            artistButton.addActionListener(new ActionListener(){
                
                @Override
                public void actionPerformed(ActionEvent e) {
                    displayAlbums(a);
                }
                
            });
            artistContents.add(artistButton);
        }
    }
    
    private void displayAlbums(String artist) {
        albumContents.removeAll();
        ((CardLayout)artistLevelCard.getLayout()).last(artistLevelCard);
        ArrayList<CD> toDisplay = new ArrayList<CD>();
        for(CD cd:catalog.getCdList()){
            if(cd.getArtist().equals(artist) && !toDisplay.contains(cd)){
                toDisplay.add(cd);
            }
        }
        for(int i = 0; i < toDisplay.size(); i++){
            if(i == 0){
                albumContents.add(new BrowseCDPanel(toDisplay.get(i), loggedIn, true));
            }
            else{
                albumContents.add(new BrowseCDPanel(toDisplay.get(i), loggedIn, false));
            }
        }
    }
    
    static public void populateWishlist() {
        wishlistContents.removeAll();
        if(loggedInUser == null){
            wishlistContents.add(new JLabel("Register to create a wishlist!"));
        }
        else if(loggedInUser.getWishlist().isEmpty()){
            wishlistContents.add(new JLabel("Your wishlist is empty! Browse for CDs to add!"));
        }
        else{
            for(CD cd:loggedInUser.getWishlist()){
                wishlistContents.add(new WishlistCDPanel(cd));
            }
        }
    }
    /**
     * 
     */
    private static final long serialVersionUID = 1L;
    private JTextField emailField;
    private JPasswordField passField;
    private JTextField regFirNameField;
    private JTextField regLasNameField;
    private JTextField regEmailField;
    private JPanel registerTab;
    private JTabbedPane tabbedPane;
    private JPanel artistContents;
    private JPanel artistLevelCard;
    private JPanel artistHelp;
    private JPanel genreLevelCard;
    private JPanel albumContents;
    private static JPanel wishlistContents;
    private JPasswordField regPassField;
}

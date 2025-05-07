import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;

public class SolitaireClient extends JFrame {
    private JPanel cardPanel;
    private JTextField commandField;
    private Socket socket;
    private PrintWriter out;
    private BufferedReader in;
    private JPanel statusPanel;
    String returnString = "";
    boolean playphase = false;

    //Gui setup
    public SolitaireClient() {
        setTitle("Solitaire Client");
        setSize(1000, 700);
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        //Cards in GUI
        cardPanel = new JPanel(new GridBagLayout());
        cardPanel.setBackground(new Color(0, 76, 153));
        JScrollPane scrollPane = new JScrollPane(cardPanel);

        //Inputcommand field in GUI
        commandField = new JTextField(20);
        commandField.addActionListener(e -> sendCommand());

        // Show (Last command and message) in GUI
        statusPanel = new JPanel();
        statusPanel.setBackground(new Color(0, 76, 153));
        statusPanel.setLayout(new BoxLayout(statusPanel, BoxLayout.Y_AXIS));

        //Box for both commandfield and status panel
        JPanel bottomPanel = new JPanel();
        bottomPanel.setLayout(new BorderLayout());
        bottomPanel.setBackground(new Color(0, 76, 153));
        bottomPanel.add(statusPanel, BorderLayout.CENTER);
        bottomPanel.add(commandField, BorderLayout.SOUTH);

        //Combine into layout
        setLayout(new BorderLayout());
        add(scrollPane, BorderLayout.CENTER);
        add(bottomPanel, BorderLayout.SOUTH);

        connectToServer();
    }




    //Socket - connect
    private void connectToServer() {
        new Thread(() -> {
            try {
                socket = new Socket("localhost", 12345);
                out = new PrintWriter(socket.getOutputStream(), true);
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                SwingUtilities.invokeLater(() -> displayMessage("Connected to server..."));
            } catch (IOException e) {
                SwingUtilities.invokeLater(() -> displayMessage("Failed to connect: " + e.getMessage()));
            }
        }).start();
    }

    //Socket send command to C-program
    private void sendCommand() {
        String command;

        if (returnString.length() >= 8) { //If we click cards we send command in returnString
            command = returnString;
            returnString = "";
        } else { //if command typed in inputfield
        command = commandField.getText().trim();
        returnString = "";

        //We should only be able to click on cards during playphase, so these ensure we are in correct phase.
        if (command.toLowerCase().equals("p")) {
            playphase = true;
        } else if (command.toLowerCase().equals("q")) {
            playphase = false;
        }

        if (command.isEmpty()) return;
        commandField.setText("");
        }

        new Thread(() -> {
            try {
                out.print(command); //Send command
                out.flush();

                StringBuilder response = new StringBuilder();
                String line;
                while ((line = in.readLine()) != null && !line.equals("END")) {
                    response.append(line).append("\n");
                }

                displayBoard(response.toString());
            } catch (IOException e) {
                SwingUtilities.invokeLater(() -> displayMessage("Error: " + e.getMessage()));
            }
        }).start();
    }

    //Display basic message
    private void displayMessage(String msg) {
        cardPanel.removeAll();
        JLabel label = new JLabel(msg);
        label.setForeground(Color.WHITE);
        label.setFont(new Font("Monospaced", Font.BOLD, 16));
        cardPanel.add(label);
        cardPanel.revalidate();
        cardPanel.repaint();
    }

    //GUI display to show cards
    private void displayBoard(String boardData) {
        SwingUtilities.invokeLater(() -> {
            //Clear status and current shown cards
            cardPanel.removeAll();
            statusPanel.removeAll();
            
            // Remove newlines from input 
            String[] lines = boardData.split("\n");
            GridBagConstraints gbc = new GridBagConstraints();
            gbc.insets = new Insets(0, 2, 2, 2); //padding around each card

            // Get LastCommand and msg and put in statusPanel box in gui
            String lastCommand = lines[lines.length - 2];
            String message = lines[lines.length - 1];
            JLabel lastCommandLabel = new JLabel(lastCommand);
            lastCommandLabel.setForeground(Color.WHITE);
            statusPanel.add(lastCommandLabel);
            JLabel messageLabel = new JLabel(message);
            messageLabel.setForeground(Color.WHITE);
            statusPanel.add(messageLabel);
            statusPanel.revalidate();
            statusPanel.repaint();

            // Print cards in gui (CardPanel)
            int row = 0;
            for (int i = 0; i < lines.length - 2; i++) {
                if (lines[i].trim().isEmpty()) continue; 

                String[] tokens = lines[i].split("\t"); // Split at every tab

                //Iterate all elements (cards, columnnumbers, foundationnumbers)
                for (int col = 0; col < tokens.length; col++) {
                    String token = tokens[col].trim();

                    gbc.gridx = col;
                    gbc.gridy = row;

                    // if foundation
                    if (col == 7) {
                        cardPanel.add(Box.createRigidArea(new Dimension(40, 0)), gbc); // Add space between column7 and foundation
                    }

                    if (token.isEmpty() && i ==2 && col < 7) {
                        addCardImage(cardPanel, "Cards/noCard.png", gbc, token,col); //Link cardid to empty image card
                    }
                    //If facedowncard or foundation
                    if (token.equals("[]")) { 
                        //Foundation
                        if (col > 7) { 
                            addCardImage(cardPanel, "Cards/noCard.png", gbc, token,col); //Link cardid to empty image card
                        } else { //Facedowncard
                            addCardImage(cardPanel, "Cards/EMPTY.png", gbc, token,col); //Link cardid to empty image card
                        }
                    // Any other card (use regular expression)
                    } else if (token.matches("[2-9TJQKA][CDHS]")) { 
                        String imagePath = "Cards/" + token + ".png";
                        addCardImage(cardPanel, imagePath, gbc, token, col); //Link cardid to empty image card, and give mouseclick eventhandler

                        //to print Columnnumbers and foundationnumbers like "c1", "f1"...
                    } else if (!token.isEmpty()) {
                        JLabel label = new JLabel(token);
                        label.setForeground(Color.WHITE);
                        label.setFont(new Font("SansSerif", Font.BOLD, 14));
                        cardPanel.add(label, gbc);
                    }
                }

                row++;
            }

            cardPanel.revalidate();
            cardPanel.repaint();
        });
    }




    //Link cardid to a Cardimage.  Make each card a object so we can give each card an eventhandler when mouse clicked.
    private void addCardImage(JPanel panel, String imagePath, GridBagConstraints gbc, String token, int colnr) {
        try {
            ImageIcon icon = new ImageIcon(imagePath); // ImageIcon used to load img from file path
            if (icon.getIconWidth() == -1) throw new IOException("Image not found");
            Image scaled = icon.getImage().getScaledInstance(60, 70, Image.SCALE_SMOOTH);

            //Make the card an object (cardLabel)
            CardLabel cardLabel = new CardLabel(token, new ImageIcon(scaled), colnr);

           
            if (playphase) {
                 //Eventhandling if click on card (cardLabel)
                cardLabel.addMouseListener(new MouseAdapter() {
                    @Override
                    public void mouseClicked(MouseEvent e) {
                        //This Func prepares returnstring, which is a input command of moving cards that is sent back to C program.
                        updateReturnString(cardLabel.getCardID(), cardLabel.getCol()); 

                        //We wont mark the card with a border if we click on Facedown/nocard
                        if (!cardLabel.getCardID().equals("[]") && !cardLabel.getCardID().equals("noCard") )
                        cardLabel.setBorder(BorderFactory.createLineBorder(Color.RED, 2)); // Mark the card with a border
                        cardPanel.repaint();
                    }
                });
            }
            panel.add(cardLabel, gbc); //Add the card to gui
        } catch (Exception e) { //Error handling
            JLabel fallback = new JLabel("[?]");
            fallback.setForeground(Color.RED);
            panel.add(fallback, gbc);
        }
    }

    //Class of cardlabel (cards)
    private static class CardLabel extends JLabel {
        private final String cardID; //Cardid like "KH", "6C" ETC.
        private String col; //columnnr of card like "F1" or "C2"

        public CardLabel(String token, Icon icon, int colnr) {
            super(icon);
            if (colnr < 7) { //Column
                col = "C" + (colnr+1);
            } else if (colnr >= 8 ) { //foundations (Workaround to iniate Foundationnr since we dont bother bringing rows into this class)
                col = "F" + (colnr-7);
            }
            this.cardID = token;
        }

        //Getters
        public String getCardID() { 
            return cardID;
        }

        public String getCol() {
            return col;
        }

    }

    //Prepares input command when Clicked on cards (NOT TYPED IN TEXTFIELD)
    //Inputcommand is ready if two cards are clicked on.
    void updateReturnString(String cardID, String col) {
        //moving card. (We also make sure we cant move a facedowncard (empty) or noCard.) 
        if (returnString.equals("") && !cardID.equals("noCard") && !cardID.equals("[]")) { 
            returnString += col + ":" + cardID + "->"; //converts cards field to inputcommand
            
            //Column, moved card is moved too (runs the second time this function is called)
        } else if (!returnString.equals("")) { 
            returnString += col;
            sendCommand(); //Send command now
        }

    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new SolitaireClient().setVisible(true));
    }
}
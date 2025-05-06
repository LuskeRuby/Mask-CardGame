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

    // Gui setup
    public SolitaireClient() {
        setTitle("Solitaire Client");
        setSize(1000, 700);
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        // Card panel setup
        cardPanel = new JPanel(new GridBagLayout());
        cardPanel.setBackground(new Color(0, 76, 153));
        JScrollPane scrollPane = new JScrollPane(cardPanel);

        // Command field
        commandField = new JTextField(20);
        commandField.addActionListener(e -> sendCommand());

        // Statuspanel (Last command and message)
        statusPanel = new JPanel();
        statusPanel.setBackground(new Color(0, 76, 153));
        statusPanel.setLayout(new BoxLayout(statusPanel, BoxLayout.Y_AXIS));

        // Box for both command field and status panel
        JPanel bottomPanel = new JPanel();
        bottomPanel.setLayout(new BorderLayout());
        bottomPanel.setBackground(new Color(0, 76, 153));
        bottomPanel.add(statusPanel, BorderLayout.CENTER);
        bottomPanel.add(commandField, BorderLayout.SOUTH);

        // Layout
        setLayout(new BorderLayout());
        add(scrollPane, BorderLayout.CENTER);
        add(bottomPanel, BorderLayout.SOUTH);

        connectToServer();
    }

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

    private void sendCommand() {
        String command;
        if (returnString.length() >= 8) { //If two card is clicked
            command = returnString;
            returnString = "";
        } else { //if typed
        command = commandField.getText().trim();
        returnString = "";
        if (command.isEmpty()) return;
        commandField.setText("");
        }

        new Thread(() -> {
            try {
                out.print(command);
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

    private void displayMessage(String msg) {
        cardPanel.removeAll();
        JLabel label = new JLabel(msg);
        label.setForeground(Color.WHITE);
        label.setFont(new Font("Monospaced", Font.BOLD, 16));
        cardPanel.add(label);
        cardPanel.revalidate();
        cardPanel.repaint();
    }

    void updateReturnString(String cardID, String col) {
        if (returnString.equals("")) { //This means the first card that is clicked on 
            returnString += col + ":" + cardID + "->";
        } else { //not empty, second card that is clicked on, so we must now also return this msg.
            returnString += col;
            sendCommand();
        }

    }


    private void displayBoard(String boardData) {
        SwingUtilities.invokeLater(() -> {
            cardPanel.removeAll();
            statusPanel.removeAll();
            // Remove newlines from input
            String[] lines = boardData.split("\n");

            GridBagConstraints gbc = new GridBagConstraints();
            gbc.insets = new Insets(0, 2, 2, 2);

            // Get LastCommand and msg and put in statusPanel
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

            // CardPanel
            int row = 0;
            for (int i = 0; i < lines.length - 2; i++) {
                if (lines[i].trim().isEmpty()) continue; // If empty
                String[] tokens = lines[i].split("\t"); // Split at every tab

                for (int col = 0; col < tokens.length; col++) {
                    String token = tokens[col].trim();

                    gbc.gridx = col;
                    gbc.gridy = row;

                    // if foundation
                    if (col == 7) {
                        cardPanel.add(Box.createRigidArea(new Dimension(40, 0)), gbc); // Add space between column7 and foundation
                    }

                    if (token.equals("[]")) { //Facedown
                        addCardImage(cardPanel, "Cards/EMPTY.png", gbc, token,col);

                    } else if (token.matches("[2-9TJQKA][CDHS]")) { // Any other card (use regular expression)
                        String imagePath = "Cards/" + token + ".png";
                        
                        addCardImage(cardPanel, imagePath, gbc, token, col);

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





    private void addCardImage(JPanel panel, String imagePath, GridBagConstraints gbc, String token, int colnr) {
        try {
            ImageIcon icon = new ImageIcon(imagePath); // ImageIcon used to load img from file path
            if (icon.getIconWidth() == -1) throw new IOException("Image not found");

            Image scaled = icon.getImage().getScaledInstance(60, 70, Image.SCALE_SMOOTH);


            //Make card object (cardLabel)
            CardLabel cardLabel = new CardLabel(token, new ImageIcon(scaled), colnr);
            //Eventhandling if click on card (cardLabel)
            cardLabel.addMouseListener(new MouseAdapter() {
                @Override
                public void mouseClicked(MouseEvent e) {
                    updateReturnString(cardLabel.getCardID(), cardLabel.getCol()); //This Func prepares the string sent back to C program.
                    cardLabel.setBorder(BorderFactory.createLineBorder(Color.RED, 2)); // Mark the card with a border
                    cardPanel.repaint();
                }
            });
            panel.add(cardLabel, gbc);
        } catch (Exception e) {
            JLabel fallback = new JLabel("[?]");
            fallback.setForeground(Color.RED);
            panel.add(fallback, gbc);
        }
    }

    private static class CardLabel extends JLabel {
        private final String cardID;
        private String col;

        public CardLabel(String token, Icon icon, int colnr) {
            super(icon);
            if (colnr < 7) { //Column
                col = "C" + (colnr+1);
            } else if (colnr >= 8 ) { //foundations (Workaround to iniate Foundationnr since we dont bother bringing rows into this class)
                col = "F" + (colnr-7);
            }

            this.cardID = token;
        }

        public String getCardID() {
            return cardID;
        }

        public String getCol() {
            return col;
        }

    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new SolitaireClient().setVisible(true));
    }
}
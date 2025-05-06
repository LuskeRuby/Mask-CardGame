import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.net.*;

public class SolitaireClient extends JFrame {
    private JPanel cardPanel;
    private JTextField commandField;
    private Socket socket;
    private PrintWriter out;
    private BufferedReader in;

    public SolitaireClient() {
        setTitle("Solitaire Client");
        setSize(1000, 700);
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        // Card panel setup
        cardPanel = new JPanel(new GridBagLayout());
        cardPanel.setBackground(new Color(0, 100, 0)); // Dark green felt
        JScrollPane scrollPane = new JScrollPane(cardPanel);

        // Command field
        commandField = new JTextField(20);
        commandField.addActionListener(e -> sendCommand());

        // Layout
        setLayout(new BorderLayout());
        add(scrollPane, BorderLayout.CENTER);
        add(commandField, BorderLayout.SOUTH);

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
        String command = commandField.getText().trim();
        if (command.isEmpty()) return;
        commandField.setText("");

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

    private void displayBoard(String boardData) {
        SwingUtilities.invokeLater(() -> {
            cardPanel.removeAll();
            String[] lines = boardData.split("\n");
    
            GridBagConstraints gbc = new GridBagConstraints();
            gbc.insets = new Insets(2, 2, 2, 2);
    
            int row = 0;
            for (String line : lines) {
                if (line.trim().isEmpty()) continue;
    
                String[] tokens = line.split("\t");
    
                for (int col = 0; col < tokens.length; col++) {
                    String token = tokens[col].trim();
                    gbc.gridx = col;
                    gbc.gridy = row;
    
                    if (token.equals("[]")) {
                        addCardImage(cardPanel, "Cards/EMPTY.PNG", gbc);  // Assuming it's back.png
                    } else if (token.matches("[2-9TJQKA][CDHS]")) {
                        String imagePath = "Cards/" + token + ".PNG";
                        addCardImage(cardPanel, imagePath, gbc);
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

    private void addCardImage(JPanel panel, String imagePath, GridBagConstraints gbc) {
        try {
            ImageIcon icon = new ImageIcon(imagePath); // Load from file path
            if (icon.getIconWidth() == -1) throw new IOException("Image not found");
    
            Image scaled = icon.getImage().getScaledInstance(50, 80, Image.SCALE_SMOOTH);
            JLabel cardLabel = new JLabel(new ImageIcon(scaled));
            panel.add(cardLabel, gbc);
        } catch (Exception e) {
            JLabel fallback = new JLabel("[?]");
            fallback.setForeground(Color.RED);
            panel.add(fallback, gbc);
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new SolitaireClient().setVisible(true));
    }
}

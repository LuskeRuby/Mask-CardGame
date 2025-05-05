import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.net.*;

public class SolitaireClient extends JFrame {
    private JTextArea boardArea;
    private JTextField commandField;
    private Socket socket;
    private PrintWriter out;
    private BufferedReader in;

    public SolitaireClient() {
        setTitle("Solitaire Client");
        setSize(600, 500);
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        // Create board area (display area)
        boardArea = new JTextArea();
        boardArea.setEditable(false);
        boardArea.setFont(new Font("Monospaced", Font.PLAIN, 14)); // Use monospaced font
        JScrollPane scrollPane = new JScrollPane(boardArea);

        // Create command field (input)
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
                boardArea.append("Connected to server...\n");
            } catch (IOException e) {
                boardArea.setText("Failed to connect: " + e.getMessage());
            }
        }).start();
    }

    private void sendCommand() {
        String command = commandField.getText().trim();
        if (command.isEmpty()) return;

        commandField.setText("");

        new Thread(() -> {
            try {
                out.print(command); // Send command to server
                out.flush();        // send immediately

                StringBuilder response = new StringBuilder();
                String line;
                while ((line = in.readLine()) != null && !line.equals("END")) {
                    response.append(line).append("\n");
                }

                boardArea.setText(response.toString()); // Show response
            } catch (IOException e) {
                boardArea.setText("Error: " + e.getMessage());
            }
        }).start();
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new SolitaireClient().setVisible(true));
    }
}
// Author: Maxime Pol Marcet.
// I build a Java Swing application to manage and visualize employee work sessions.

import model.Employee;
import model.WorkSession;
import service.WorkTracker;

import javax.swing.*;
import java.awt.*;
import java.time.LocalDateTime;
import java.util.List;

public class Main {
    // I initialize application components.
    private static WorkTracker tracker = new WorkTracker();
    private static JTextField idField = new JTextField(5);
    private static JTextField nameField = new JTextField(10);
    private static JCheckBox remoteCheck = new JCheckBox("Remote");
    private static JTextArea reportArea = new JTextArea(20, 40);

    public static void main(String[] args) {
        // I preload example data.
        tracker.registerEmployee("E001", "Laura Martínez", false);
        tracker.registerEmployee("E002", "Carlos Gómez", true);

        tracker.logSession("E001", LocalDateTime.of(2025, 5, 1, 9, 0), LocalDateTime.of(2025, 5, 1, 14, 30));
        tracker.logSession("E002", LocalDateTime.of(2025, 5, 1, 8, 0), LocalDateTime.of(2025, 5, 1, 17, 0));

        SwingUtilities.invokeLater(Main::createAndShowGUI);
    }

    // I build the GUI for employee tracking.
    private static void createAndShowGUI() {
        JFrame frame = new JFrame("Work Time Tracker");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new BorderLayout());

        JPanel topPanel = new JPanel();
        topPanel.add(new JLabel("ID:"));
        topPanel.add(idField);
        topPanel.add(new JLabel("Name:"));
        topPanel.add(nameField);
        topPanel.add(remoteCheck);

        JButton addBtn = new JButton("Add Employee");
        addBtn.addActionListener(e -> {
            String id = idField.getText();
            String name = nameField.getText();
            boolean isRemote = remoteCheck.isSelected();
            tracker.registerEmployee(id, name, isRemote);
            JOptionPane.showMessageDialog(frame, "Employee added.");
        });

        JButton reportBtn = new JButton("Show Report");
        reportBtn.addActionListener(e -> reportArea.setText(tracker.getGuiReport()));

        topPanel.add(addBtn);
        topPanel.add(reportBtn);

        frame.add(topPanel, BorderLayout.NORTH);
        reportArea.setEditable(false);
        frame.add(new JScrollPane(reportArea), BorderLayout.CENTER);

        frame.pack();
        frame.setVisible(true);
    }
}

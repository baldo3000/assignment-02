package andreabaldazzi.smart_waste_disposal;

import java.util.Date;
import java.awt.*;
import javax.swing.*;
import javax.swing.border.EmptyBorder;

class LogView extends JFrame {

	private JTextArea log;

	public LogView() {
		super("Log ");
		setSize(600, 600);
		setResizable(false);

		JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BorderLayout());

		JLabel titleLabel = new JLabel("Log Messages", JLabel.CENTER);
		titleLabel.setFont(new Font("Arial", Font.BOLD, 16));
		mainPanel.add(titleLabel, BorderLayout.NORTH);

		log = new JTextArea(30, 40);
		log.setEditable(false);
		log.setFont(new Font("Monospaced", Font.PLAIN, 12));
		log.setBorder(new EmptyBorder(10, 10, 10, 10));
		JScrollPane scroll = new JScrollPane(log);
		scroll.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
		scroll.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
		scroll.getVerticalScrollBar().setUI(new javax.swing.plaf.basic.BasicScrollBarUI() {
			@Override
			protected void configureScrollBarColors() {
				this.thumbColor = Color.GRAY;
			}
		});
		mainPanel.add(scroll, BorderLayout.CENTER);

		this.getContentPane().add(mainPanel);
	}

	public void log(String msg) {
		SwingUtilities.invokeLater(() -> {
			String date = new Date().toString();
			log.append("[" + date + "] " + msg + "\n");
		});
	}
}

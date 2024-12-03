package andreabaldazzi.smart_waste_disposal;

import java.util.Date;

import javax.swing.*;

class LogView extends JFrame {

	private JTextArea log;

	public LogView() {
		super("Log ");
		setSize(600, 600);
		setResizable(false);

		JPanel mainPanel = new JPanel();
		log = new JTextArea(30, 40);
		log.setEditable(false);
		JScrollPane scroll = new JScrollPane(log);
		scroll.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
		mainPanel.add(scroll);
		this.getContentPane().add(mainPanel);
	}

	public void log(String msg) {
		SwingUtilities.invokeLater(() -> {
			String date = new Date().toString();
			log.append("[" + date + "] " + msg + "\n");
		});
	}
}

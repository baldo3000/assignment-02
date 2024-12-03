package andreabaldazzi.smart_waste_disposal;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.*;

class DashboardView extends JFrame implements ActionListener {

	private JButton emptyButton;
	private JButton resetButton;
	private JTextField currentFullness;
	private JTextField currentTemperature;

	private JTextField systemState;
	private SmartWasteDisposalSystemDashboardController controller;

	public DashboardView() {
		super(".:: Smart Disposal System ::.");
		setSize(600, 150);

		JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
		mainPanel.add(Box.createRigidArea(new Dimension(0, 20)));

		JPanel infoLine = new JPanel();
		infoLine.setLayout(new BoxLayout(infoLine, BoxLayout.LINE_AXIS));
		systemState = new JTextField("--");
		systemState.setEditable(false);
		systemState.setPreferredSize(new Dimension(200, 15));
		infoLine.add(new JLabel("System State:"));
		infoLine.add(systemState);
		currentFullness = new JTextField("--");
		currentFullness.setEditable(false);
		currentFullness.setPreferredSize(new Dimension(100, 15));
		infoLine.add(new JLabel("Fullness:"));
		infoLine.add(currentFullness);
		currentTemperature = new JTextField("--");
		currentTemperature.setEditable(false);
		currentTemperature.setPreferredSize(new Dimension(200, 15));
		infoLine.add(new JLabel("Current Temperature:"));
		infoLine.add(currentTemperature);

		mainPanel.add(infoLine);
		mainPanel.add(Box.createRigidArea(new Dimension(0, 20)));
		mainPanel.setPreferredSize(new Dimension(200, 20));

		JPanel buttonPanel = new JPanel();
		emptyButton = new JButton("Empty container");
		emptyButton.setEnabled(false);
		emptyButton.addActionListener(this);
		resetButton = new JButton("Reset");
		resetButton.setEnabled(false);
		resetButton.addActionListener(this);
		buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));
		buttonPanel.add(emptyButton);
		buttonPanel.add(resetButton);

		mainPanel.add(buttonPanel);
		mainPanel.add(Box.createRigidArea(new Dimension(0, 20)));
		setContentPane(mainPanel);

		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent ev) {
				System.exit(-1);
			}
		});
	}

	public void registerController(final SmartWasteDisposalSystemDashboardController contr) {
		this.controller = contr;
	}

	public void setSystemState(final String msg) {
		SwingUtilities.invokeLater(() -> {
			systemState.setText(msg);
		});
	}

	public void setFullness(final double fullness) {
		SwingUtilities.invokeLater(() -> {
			currentFullness.setText("" + fullness + "%");
		});
	}

	public void setCurrentTemperature(final double temp) {
		SwingUtilities.invokeLater(() -> {
			currentTemperature.setText("" + temp + "°C");
		});
	}

	public void enableEmpty() {
		SwingUtilities.invokeLater(() -> {
			emptyButton.setEnabled(true);
		});
	}

	public void enableReset() {
		SwingUtilities.invokeLater(() -> {
			resetButton.setEnabled(true);
		});
	}

	public void actionPerformed(final ActionEvent ev) {
		try {
			if (ev.getSource() == emptyButton) {
				controller.notifyEmptied();
				emptyButton.setEnabled(false);
			} else if (ev.getSource() == resetButton) {
				controller.notifyReset();
				resetButton.setEnabled(false);
			}
		} catch (Exception ex) {
			ex.printStackTrace();

		}
	}
}

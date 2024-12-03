package andreabaldazzi.smart_waste_disposal;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.*;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

class DashboardView extends JFrame implements ActionListener {

	private JButton emptyButton;
	private JButton resetButton;
	private JTextField currentFullness;
	private JTextField currentTemperature;

	private JTextField systemState;
	private SmartWasteDisposalSystemDashboardController controller;

	private XYSeries temperatureSeries;
	private XYSeries fullnessSeries;

	static long startTime;

	public DashboardView() {
		super(".:: Smart Disposal System ::.");
		setSize(600, 550);
		setResizable(false);
		setLocationRelativeTo(null);

		JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
		mainPanel.add(Box.createRigidArea(new Dimension(0, 20)));

		JPanel infoLine = new JPanel();
		infoLine.setLayout(new BoxLayout(infoLine, BoxLayout.LINE_AXIS));
		systemState = new JTextField("--");
		systemState.setEditable(false);
		systemState.setPreferredSize(new Dimension(200, 15));
		systemState.setBackground(Color.WHITE);
		infoLine.add(new JLabel("System State:"));
		infoLine.add(systemState);
		currentFullness = new JTextField("--");
		currentFullness.setEditable(false);
		currentFullness.setPreferredSize(new Dimension(100, 15));
		currentFullness.setBackground(Color.WHITE);
		infoLine.add(new JLabel("Fullness:"));
		infoLine.add(currentFullness);
		currentTemperature = new JTextField("--");
		currentTemperature.setEditable(false);
		currentTemperature.setPreferredSize(new Dimension(200, 15));
		currentTemperature.setBackground(Color.WHITE);
		infoLine.add(new JLabel("Current Temperature:"));
		infoLine.add(currentTemperature);

		mainPanel.add(infoLine);

		// Create the temperature history graph
		temperatureSeries = new XYSeries("Temperature");
		XYSeriesCollection datasetTemperature = new XYSeriesCollection(temperatureSeries);
		JFreeChart chartTemperature = ChartFactory.createXYLineChart(
				"Temperature History",
				"Time (s)",
				"Temperature",
				datasetTemperature,
				PlotOrientation.VERTICAL,
				true,
				true,
				false);
		XYPlot plotTemperature = chartTemperature.getXYPlot();
		XYLineAndShapeRenderer rendererTemperature = new XYLineAndShapeRenderer();
		rendererTemperature.setSeriesPaint(0, Color.RED);
		rendererTemperature.setSeriesShapesVisible(0, false);
		plotTemperature.setRenderer(rendererTemperature);
		ChartPanel chartPanelTemperature = new ChartPanel(chartTemperature);
		chartPanelTemperature.setPreferredSize(new Dimension(600, 200));
		mainPanel.add(chartPanelTemperature);

		// Create the fullness history graph
		fullnessSeries = new XYSeries("Fill level");
		XYSeriesCollection datasetFullness = new XYSeriesCollection(fullnessSeries);
		JFreeChart chartFullness = ChartFactory.createXYLineChart(
				"Fullness History",
				"Time (s)",
				"Fill Level",
				datasetFullness,
				PlotOrientation.VERTICAL,
				true,
				true,
				false);
		XYPlot plotFullness = chartFullness.getXYPlot();
		XYLineAndShapeRenderer rendererFullness = new XYLineAndShapeRenderer();
		rendererFullness.setSeriesPaint(0, Color.BLUE);
		rendererFullness.setSeriesShapesVisible(0, false);
		plotFullness.setRenderer(rendererFullness);
		ChartPanel chartPanelFullness = new ChartPanel(chartFullness);
		chartPanelFullness.setPreferredSize(new Dimension(600, 200));
		mainPanel.add(chartPanelFullness);

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

		startTime = System.currentTimeMillis();

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
			if (fullness == 100) {
				currentFullness.setBackground(Color.RED);
			} else if (fullness > 75) {
				currentFullness.setBackground(Color.ORANGE);
			} else if (fullness > 50) {
				currentFullness.setBackground(Color.YELLOW);
			} else {
				currentFullness.setBackground(Color.WHITE);
			}
			fullnessSeries.add(getElapsedSeconds(), fullness);
		});
	}

	public void setCurrentTemperature(final double temp) {
		SwingUtilities.invokeLater(() -> {
			currentTemperature.setText("" + temp + "°C");
			if (temp > 40) {
				currentTemperature.setBackground(Color.RED);
			} else if (temp > 35) {
				currentTemperature.setBackground(Color.ORANGE);
			} else if (temp > 30) {
				currentTemperature.setBackground(Color.YELLOW);
			} else {
				currentTemperature.setBackground(Color.WHITE);
			}
			temperatureSeries.add(getElapsedSeconds(), temp);
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

	// Method to get elapsed seconds since program start
	public long getElapsedSeconds() {
		long currentTime = System.currentTimeMillis();
		return (currentTime - startTime) / 1000;
	}

}
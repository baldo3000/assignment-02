package andreabaldazzi.smart_waste_disposal;

import javax.swing.SwingUtilities;

class SmartWasteDisposalSystemDashboard {

	static DashboardView view = null;
	static LogView log = null;

	public static void main(String[] args) throws Exception {
		SwingUtilities.invokeAndWait(() -> {
			view = new DashboardView();
			log = new LogView();
		});
		String portName = "COM9";
		SmartWasteDisposalSystemDashboardController contr = new SmartWasteDisposalSystemDashboardController(portName,
				view, log);
		view.registerController(contr);
		SwingUtilities.invokeLater(() -> {
			view.setVisible(true);
			log.setVisible(true);
		});
	}
}
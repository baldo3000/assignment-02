package andreabaldazzi.smart_waste_disposal;

public class SmartWasteDisposalSystemDashboardController {

	static final String MSG_EMPTIED = "emptied";
	static final String MSG_RESET = "reset";

	SerialCommChannel channel;
	DashboardView view;
	LogView logger;

	public SmartWasteDisposalSystemDashboardController(String port, DashboardView view, LogView logger)
			throws Exception {
		this.view = view;
		this.logger = logger;

		channel = new SerialCommChannel(port, 9600);
		new MonitoringAgent(channel, view, logger).start();

		/* attesa necessaria per fare in modo che Arduino completi il reboot */

		System.out.println("Waiting Arduino for rebooting...");
		Thread.sleep(4000);
		System.out.println("Ready.");

	}

	public void notifyEmptied() {
		channel.sendMsg(MSG_EMPTIED);
	}

	public void notifyReset() {
		channel.sendMsg(MSG_RESET);
	}

}

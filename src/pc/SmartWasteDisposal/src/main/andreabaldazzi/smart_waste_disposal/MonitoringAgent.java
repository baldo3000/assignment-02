package andreabaldazzi.smart_waste_disposal;

public class MonitoringAgent extends Thread {

	SerialCommChannel channel;
	DashboardView view;
	LogView logger;

	static final String SYSTEM_PREFIX = "wd:";
	static final String LOG_PREFIX = "lo:";
	static final String MSG_STATE = "st:";

	static final String[] stateNames = { "Idle", "Waiting for user", "User Detected", "Disposing", "Full", "Overheated"};

	public MonitoringAgent(final SerialCommChannel channel, final DashboardView view, final LogView log)
			throws Exception {
		this.view = view;
		this.logger = log;
		this.channel = channel;
	}

	public void run() {
		boolean full = true;
		boolean overheated = true;
		while (true) {
			try {
				String msg = channel.receiveMsg();
				if (msg.startsWith(SYSTEM_PREFIX)) {
					String cmd = msg.substring(SYSTEM_PREFIX.length());
					// logger.log("new command: "+cmd);
					if (cmd.startsWith(MSG_STATE)) {
						try {
							String args = cmd.substring(MSG_STATE.length());
							String[] elems = args.split(":");
							if (elems.length >= 3) {
								final int stateCode = Integer.parseInt(elems[0]);
								final double fullness = Double.parseDouble(elems[1]);
								final double temp = Double.parseDouble(elems[2]);

								view.setFullness(fullness);
								view.setCurrentTemperature(temp);
								view.setSystemState(stateNames[stateCode]);

								if (stateCode == 4 && !full) { // full
									full = true;
									view.enableEmpty();
								} else if (stateCode != 4 && full) {
									full = false;
								}
								if (stateCode == 5 && !overheated) { // overheated
									overheated = true;
									view.enableReset();
								} else if (stateCode != 5 && overheated) {
									overheated = false;
								}
							}
						} catch (Exception ex) {
							ex.printStackTrace();
							System.err.println("Error in msg: " + cmd);
						}
					}
				} else if (msg.startsWith(LOG_PREFIX)) {
					this.logger.log(msg.substring(LOG_PREFIX.length()));
				}
			} catch (Exception ex) {
				ex.printStackTrace();
			}
		}
	}

}

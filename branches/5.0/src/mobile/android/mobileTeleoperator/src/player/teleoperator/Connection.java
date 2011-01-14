package player.teleoperator;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import jderobot.LaserPrx;
import jderobot.MotorsPrx;

public final class Connection {
	
	public static void disconnect() {
		if(communicator != null)
			communicator.destroy();
		
		if(communicatorLaser != null)
			communicator.destroy();
	}
	
	public static void setCommunicator(Ice.Communicator c) {
		communicator = c;
	}
	
	public static void setTeleoperator(MotorsPrx t) {
		mprx = t;
	}
	
	public static MotorsPrx getTeleoperator() {
		return mprx;
	}
	
	public static void setConnected(boolean status) {
		connected = status;
	}
	
	public static boolean isConnected() {
		return connected;
	}
	
	//Laser functions	
	public static void setLaserAvailable(boolean status) {
		laserAvailable = status;
	}
	
	public static boolean isLaserAvailable() {
		return laserAvailable;
	}
	public static void setLaser(LaserPrx l) {
		lprx = l;
	}
	
	public static LaserPrx getLaser() {
		return lprx;
	}
	
	public static void setCommunicatorLaser(Ice.Communicator c) {
		communicatorLaser = c;
	}
	
	public static void setV(float v)  {
		Connection.lock.lock();
		Connection.getTeleoperator().setV(v);
		Connection.v = v;
		ltime = System.currentTimeMillis();
		Connection.lock.unlock();
	}
	
	public static void setW(float w) {
		Connection.lock.lock();
		Connection.getTeleoperator().setW(w);
		Connection.w = w;
		ltime = System.currentTimeMillis();
		Connection.lock.unlock();
	}
	
	public static void setL(float l) {
		Connection.lock.lock();
		Connection.getTeleoperator().setL(l);
		Connection.l = l;
		ltime = System.currentTimeMillis();
		Connection.lock.unlock();
	}
	
	public static void sendThread() {
		// Create a thread to send the signal
		thread = new Thread(new Runnable() {
			public void run() {
				long ctime;
				boolean sendv = true;

				try {
					while (true) {
						
						if(Connection.isConnected()) {
							Connection.lock.lock();
							ctime = System.currentTimeMillis();
							
							if((ctime - ltime) > nsegs*1000) {
								if(sendv)
									Connection.getTeleoperator().setV(v);
								else{
									Connection.getTeleoperator().setW(w);
									Connection.getTeleoperator().setL(l);
								}
					
								

								sendv = !sendv;
								ltime = System.currentTimeMillis();
							}	
							Connection.lock.unlock();
						}

						Thread.sleep(500);
					}
				} catch (InterruptedException e) {
				}
			}
		});

		thread.start();
		
		
	}
	
	private static float v;
	private static float w;
	private static float l;
	private static long ltime=0;
	private static long nsegs = 2;
	private static Thread thread;
	private static Lock lock = new ReentrantLock();

	private static Ice.Communicator communicator;
	private static MotorsPrx mprx;
	private static boolean connected;
	
	private static Ice.Communicator communicatorLaser;
	private static LaserPrx lprx;
	private static boolean laserAvailable;
}

package player.teleoperator;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import jderobot.LaserPrx;
import jderobot.MotorsPrx;
import bica.*;

public final class Connection {
	
	public static void disconnect() {
		if(communicator != null)
			communicator.destroy();
	}
	
	public static void setCommunicator(Ice.Communicator c) {
		communicator = c;
	}
	
	public static void setScheduler(SchedulerManagerPrx s) {
		sprx = s;
	}
	
	public static SchedulerManagerPrx getScheduler() {
		return sprx;
	}
	
	public static void setBody(BodyMotionManagerPrx b) {
		bprx = b;
	}
	
	public static BodyMotionManagerPrx getBody() {
		return bprx;
	}
	
	public static void setHead(HeadMotionManagerPrx h) {
		hprx = h;
	}
	
	public static HeadMotionManagerPrx getHead() {
		return hprx;
	}
	
	public static void setMotors(MotorsPrx m) {
		mprx = m;
	}
	
	public static MotorsPrx getMotors() {
		return mprx;
	}	
	
	public static void setConnected(boolean status) {
		connected = status;
		
		if(use_nao) {
			if(status == true) {
				Connection.runBica();
			} else {
				Connection.stopBica();
			}
		}
	}
	
	public static void setUseNao(boolean status) {
		use_nao = status;
	}
	
	public static boolean isConnected() {
		return connected;
	}
	
	public static boolean isLaserAvailable() {
		return false;
	}
	
	public static void setV(float v)  {
		Connection.lock.lock();
		if(use_nao)
			Connection.getBody().setVel(v, 0.0f, 0.0f);
		else
			Connection.getMotors().setV(v);
		Connection.lock.unlock();
	}
	
	public static void setW(float w) {
		Connection.lock.lock();
		if(use_nao)
			Connection.getBody().setVel(0.0f, w, 0.0f);
		else
			Connection.getMotors().setW(w);
		Connection.lock.unlock();
	}
	
	public static void setL(float l) {
		Connection.lock.lock();
		if(use_nao)
			Connection.getBody().setVel(0.0f, 0.0f, l);
		else
			Connection.getMotors().setL(l);
		Connection.lock.unlock();
	}
	
	public static void setPan(float pan)  {
		Connection.lock.lock();
		if(use_nao)
			Connection.getHead().setPanPos(pan, 1.0f);
		Connection.lock.unlock();
	}
	
	public static void setTilt(float tilt)  {
		Connection.lock.lock();
		if(use_nao)
			Connection.getHead().setTiltPos(tilt, 1.0f);
		Connection.lock.unlock();
	}
	
	public static void runBica() {	
		if(!bicarunning) {
			Connection.getScheduler().run("Body");
			Connection.getScheduler().run("Head");
			bicarunning = true;
		}
	}
	
	public static void stopBica() {
		if(bicarunning) {
			Connection.getScheduler().stop("Body");
			Connection.getScheduler().stop("Head");
			bicarunning = false;
		}		
	}
	

	/*public static void sendThread() {
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
	private static Thread thread;*/
		
	private static Lock lock = new ReentrantLock();
	private static Ice.Communicator communicator;
	private static MotorsPrx mprx;
	private static boolean connected;

	private static HeadMotionManagerPrx hprx;
	private static BodyMotionManagerPrx bprx;
	private static SchedulerManagerPrx sprx;
	private static boolean bicarunning = false;
	private static boolean use_nao = false;
}

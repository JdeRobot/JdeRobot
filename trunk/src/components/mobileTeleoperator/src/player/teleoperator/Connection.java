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
	
	public static void setMotors(MotorsPrx m) {
		mprx = m;
	}
	
	public static MotorsPrx getMotors() {
		return mprx;
	}	
	
	public static void setConnected(boolean status) {
		connected = status;
		
		if(use_nao) {
			if(status == true)
				Connection.runBody();
			else
				Connection.stopBody();
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
	
	public static void kickLeft() {
		if(use_nao)
			Connection.getBody().doMove("LFOOT");
	}
	
	public static void kickRight() {
		if(use_nao)
			Connection.getBody().doMove("RFOOT");
	}
	
	public static void runBody() {	
		if(!bodyrunning) {
			Connection.getScheduler().run("Body");
			bodyrunning = true;
		}
	}
	
	public static void stopBody() {
		if(bodyrunning) {
			Connection.getScheduler().stop("Body");
			bodyrunning = false;
		}		
	}
		
	private static Lock lock = new ReentrantLock();
	private static Ice.Communicator communicator;
	private static MotorsPrx mprx;
	private static boolean connected;

	private static BodyMotionManagerPrx bprx;
	private static SchedulerManagerPrx sprx;
	private static boolean bodyrunning = false;
	private static boolean use_nao = false;
}

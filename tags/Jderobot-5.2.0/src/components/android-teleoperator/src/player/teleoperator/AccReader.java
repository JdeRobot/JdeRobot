package player.teleoperator;

import android.app.Activity;
import android.content.Context;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.hardware.Sensor;

public class AccReader implements SensorEventListener {

	private SensorManager sensorManager;
	private float accX, accY, accZ;

	public AccReader(Activity act) {

		// Create the sensor manager
		this.sensorManager = (SensorManager) act.getSystemService(Context.SENSOR_SERVICE);

	}
	
	public float getX() {
		return this.accX;
	}
	
	public float getY() {
		return this.accY;
	}

	
	public float getZ() {
		return this.accZ;
	}

	public void init() {
		// Register accelerometer
		sensorManager.registerListener(this, sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),
				SensorManager.SENSOR_DELAY_FASTEST);
	}

	public void destroy() {
		// Unregister accelerometer
		sensorManager.unregisterListener(this, sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER));
	}
	
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// TODO Auto-generated method stub

	}

	public void onSensorChanged(SensorEvent event) {
		//Save values
		synchronized (this) {
			switch (event.sensor.getType()) {
			case Sensor.TYPE_ACCELEROMETER:
				this.accX = event.values[0];
				this.accY = event.values[1];
				this.accZ = event.values[2];
				break;
			default:
				break;

			}
		}

	}

}

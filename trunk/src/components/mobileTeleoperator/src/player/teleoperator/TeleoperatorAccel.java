package player.teleoperator;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageButton;

public class TeleoperatorAccel extends Activity {
	
	private static final int SHOW_LASER = 0;

	private static final int MENU_BACK = Menu.FIRST;
	private static final int MENU_LASER = Menu.FIRST + 1;

	private ImageButton buttonUp, buttonDown;
	private ImageButton buttonLeft, buttonRight;
	private ImageButton buttonStop;
	private ImageButton buttonPress;

	private static final float V_VEL = 0.2f;
	private static final float W_VEL = 0.4f;

	private static final int CHANGE_V = 0;
	private static final int CHANGE_W = 1;
	private static final int CHANGE_BOTH = 2;

	private float velv, velw;
	private float lastv, lastw;
	private float savedX, savedY;
	private boolean pressed;

	private AccReader accReader;

	private Thread thread;
	private boolean endThread;
	private Handler mHandler;
	private Runnable mUpdateResults;

	private Lock lock;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.accelerometers);

		this.pressed = false;
		this.endThread = false;

		this.buttonUp = (ImageButton) findViewById(R.id.button_up);
		this.buttonDown = (ImageButton) findViewById(R.id.button_down);
		this.buttonLeft = (ImageButton) findViewById(R.id.button_left);
		this.buttonRight = (ImageButton) findViewById(R.id.button_right);
		this.buttonStop = (ImageButton) findViewById(R.id.button_stop);
		this.buttonPress = (ImageButton) findViewById(R.id.button_press);

		this.buttonUp.setEnabled(false);
		this.buttonDown.setEnabled(false);
		this.buttonLeft.setEnabled(false);
		this.buttonRight.setEnabled(false);
		this.buttonStop.setEnabled(false);

		this.lastv = 0.0f;
		this.lastw = 0.0f;

		this.lock = new ReentrantLock();

		// add a touch listener to the buttons
		this.buttonPress.setOnTouchListener(new View.OnTouchListener() {

			public boolean onTouch(View v, MotionEvent event) {
				int i = event.getAction();

				if (i == MotionEvent.ACTION_DOWN) {
					lock.lock();
					pressed = true;
					buttonPress.setImageResource(R.drawable.bar_down);
					buttonStop.setEnabled(true);

					// Save values
					savedX = accReader.getX();
					savedY = accReader.getY();
					lock.unlock();
				}
				if (i == MotionEvent.ACTION_UP) {
					lock.lock();
					pressed = false;
					buttonPress.setImageResource(R.drawable.bar_up);
					buttonStop.setEnabled(false);

					// Stop robot
					velv = 0.0f;
					velw = 0.0f;
					if (lastv != velv || lastw != velw) {
						if (velv != lastv)
							updateVel(CHANGE_V);
						if (velw != lastw)
							updateVel(CHANGE_W);
						lastv = 0.0f;
						lastw = 0.0f;
					}
					
					// Update buttons
					mHandler.post(mUpdateResults);
					
					lock.unlock();
				}
				return false;
			}
		});

		// Create a handler to manage buttons
		mHandler = new Handler();
		mUpdateResults = new Runnable() {
			public void run() {
				updateButtons();
			}
		};
	}

	@Override
	public void onResume() {
		super.onResume();

		// Create a thread to control if the button is pressed
		this.thread = new Thread(new Runnable() {
			public void run() {
				float diffx, diffy;
				float thresholdx = 1.0f;
				float thresholdy = 1.0f;

				try {
					while (!endThread) {
						lock.lock();
						if (pressed) {
							// Check difference between values
							diffx = accReader.getX() - savedX;
							diffy = accReader.getY() - savedY;

							// Compare V, only when w is 0
							if(velw == 0.0) {
								if (diffx > thresholdx)
									velv = -diffx*1.3f;
								else if (diffx < -thresholdx)
									velv = -diffx*1.3f;
								else
									velv = 0.0f;
							}

							// Compare W, only when v is 0
							if(velv == 0.0) {
								if (diffy > thresholdy)
									velw = -diffy*4;
								else if (diffy < -thresholdy)
									velw = -diffy*4;
								else
									velw = 0.0f;
							}

							// Update vel
							if (velv != lastv || velw != lastw) {
								if (velv != lastv)
									updateVel(CHANGE_V);
								if (velw != lastw)
									updateVel(CHANGE_W);
								lastv = velv;
								lastw = velw;
							}

							// Update buttons
							mHandler.post(mUpdateResults);
						}
						lock.unlock();

						Thread.sleep(250);
					}
				} catch (InterruptedException e) {
				}
			}
		});

		this.thread.start();

		// Create an accelerometer reader
		this.accReader = new AccReader(this);
		this.accReader.init();
	}

	@Override
	protected void onPause() {
		super.onPause();

		this.endThread = true;
		this.accReader.destroy();
		this.stopRobot();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
    	super.onCreateOptionsMenu(menu);
    	
    	//Create and add menu items
    	MenuItem itemBack = menu.add(0, MENU_BACK, Menu.NONE, R.string.menu_back);
    	//MenuItem itemLaser = menu.add(0, MENU_LASER, Menu.NONE, R.string.menu_laser);
    	
    	//Assign icons
    	itemBack.setIcon(R.drawable.menu_back);
    	//itemLaser.setIcon(R.drawable.menu_laser);
    	
    	//Allocate shortcuts to each of them
    	itemBack.setShortcut('0', 'b');
    	//itemLaser.setShortcut('1', 'l');
    	return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		super.onOptionsItemSelected(item);

		switch (item.getItemId()) {
		case (MENU_BACK): {
			setResult(RESULT_OK, null);
			finish();
			return true;
		}
		case (MENU_LASER): {
			init_activity_laser();
			return true;
		}
		}
		return false;
	}

	private void stopRobot() {
		this.velv = 0.0f;
		this.velw = 0.0f;

		this.updateVel(CHANGE_BOTH);
	}

	private void updateVel(int type) {
		if (type == CHANGE_V || type == CHANGE_BOTH)
			Connection.setV(this.velv);
		if (type == CHANGE_W || type == CHANGE_BOTH)
			Connection.setW(this.velw);
	}

	private void updateButtons() {
		if (this.velv > 0)
			this.buttonUp.setEnabled(true);
		else
			this.buttonUp.setEnabled(false);
		if (this.velv < 0)
			this.buttonDown.setEnabled(true);
		else
			this.buttonDown.setEnabled(false);
		if (this.velw > 0)
			this.buttonLeft.setEnabled(true);
		else
			this.buttonLeft.setEnabled(false);
		if (this.velw < 0)
			this.buttonRight.setEnabled(true);
		else
			this.buttonRight.setEnabled(false);
	}
	
	private void init_activity_laser() {
		Intent intent = new Intent(TeleoperatorAccel.this, Simulation.class);
		startActivityForResult(intent, SHOW_LASER);		
	}
}
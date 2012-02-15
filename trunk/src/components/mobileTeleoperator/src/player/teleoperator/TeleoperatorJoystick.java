package player.teleoperator;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageButton;

public class TeleoperatorJoystick extends Activity {
	
	private static final int SHOW_LASER = 0;

	private static final int MENU_BACK = Menu.FIRST;
	private static final int MENU_LASER = Menu.FIRST + 1;

	private ImageButton buttonUp, buttonDown;
	private ImageButton buttonLeft, buttonRight;
	private ImageButton buttonStop;

	private static final float V_VEL = 0.2f;
	private static final float W_VEL = 0.4f;

	private static final int CHANGE_V = 0;
	private static final int CHANGE_W = 1;
	private static final int CHANGE_BOTH = 2;
	
	private static final int PRESSED_NONE = 0;
	private static final int PRESSED_STOP = 1;
	private static final int PRESSED_UP = 2;
	private static final int PRESSED_DOWN = 3;
	private static final int PRESSED_LEFT = 4;
	private static final int PRESSED_RIGHT = 5;

	private float velv, velw;
	private int pressed;
	
	private Lock lock;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.joystick);
		
		this.pressed = PRESSED_NONE;
		
		this.lock = new ReentrantLock();

		this.buttonUp = (ImageButton) findViewById(R.id.button_up);
		this.buttonDown = (ImageButton) findViewById(R.id.button_down);
		this.buttonLeft = (ImageButton) findViewById(R.id.button_left);
		this.buttonRight = (ImageButton) findViewById(R.id.button_right);
		this.buttonStop = (ImageButton) findViewById(R.id.button_stop);
		
		/*Activate the stop*/
		//this.buttonStop.getBackground().setColorFilter(new LightingColorFilter(0xFF000000, 0xFF0000FF));
		//buttonStop.setPressed(true);

		// add a touch listener to the buttons
		this.buttonStop.setOnTouchListener(new View.OnTouchListener() {

			public boolean onTouch(View v, MotionEvent event) {
				int i = event.getAction();

				if (i == MotionEvent.ACTION_DOWN) {
					lock.lock();
					
					if(pressed != PRESSED_NONE) {
						lock.unlock();
						return false;
					}
					
					//Stop robot
					stopRobot();
					
					pressed = PRESSED_STOP;
					lock.unlock();
				} else if (i == MotionEvent.ACTION_UP) {
					lock.lock();
					
					//Stop robot
					stopRobot();
					pressed = PRESSED_NONE;
					
					lock.unlock();
				}
				return false;
			}
		});
		
		this.buttonUp.setOnTouchListener(new View.OnTouchListener() {

			public boolean onTouch(View v, MotionEvent event) {
				int i = event.getAction();

				if (i == MotionEvent.ACTION_DOWN) {
					lock.lock();
					
					if(pressed != PRESSED_NONE) {
						lock.unlock();
						return false;
					}
								
					//Move robot
					velv = V_VEL;
					updateVel(CHANGE_V);
					
					pressed = PRESSED_UP;
					lock.unlock();
				} else if (i == MotionEvent.ACTION_UP) {
					lock.lock();
					
					//Stop robot
					stopRobot();
					pressed = PRESSED_NONE;
					
					lock.unlock();
				}
				return false;
			}
		});
		
		this.buttonDown.setOnTouchListener(new View.OnTouchListener() {

			public boolean onTouch(View v, MotionEvent event) {
				int i = event.getAction();

				if (i == MotionEvent.ACTION_DOWN) {
					lock.lock();
					
					if(pressed != PRESSED_NONE) {
						lock.unlock();
						return false;
					}
					
					//Move robot
					velv = -V_VEL;
					updateVel(CHANGE_V);
					
					pressed = PRESSED_DOWN;
					lock.unlock();
				} else if (i == MotionEvent.ACTION_UP) {
					lock.lock();
					
					//Stop robot
					stopRobot();
					pressed = PRESSED_NONE;
					
					lock.unlock();
				}
				return false;
			}
		});
		
		this.buttonLeft.setOnTouchListener(new View.OnTouchListener() {

			public boolean onTouch(View v, MotionEvent event) {
				int i = event.getAction();

				if (i == MotionEvent.ACTION_DOWN) {
					lock.lock();
					
					if(pressed != PRESSED_NONE) {
						lock.unlock();
						return false;
					}
									
					//Move robot
					velw = W_VEL;
					updateVel(CHANGE_W);
					
					pressed = PRESSED_LEFT;
					lock.unlock();
				} else if (i == MotionEvent.ACTION_UP) {
					lock.lock();
					
					//Stop robot
					stopRobot();
					pressed = PRESSED_NONE;
					
					lock.unlock();
				}
				return false;
			}
		});
		
		this.buttonRight.setOnTouchListener(new View.OnTouchListener() {

			public boolean onTouch(View v, MotionEvent event) {
				int i = event.getAction();

				if (i == MotionEvent.ACTION_DOWN) {
					lock.lock();
					
					if(pressed != PRESSED_NONE) {
						lock.unlock();
						return false;
					}
					
					//Move robot
					velw = -W_VEL;
					updateVel(CHANGE_W);
					
					pressed = PRESSED_RIGHT;
					lock.unlock();
				} else if (i == MotionEvent.ACTION_UP) {
					lock.lock();
					
					//Stop robot
					stopRobot();
					pressed = PRESSED_NONE;
					
					lock.unlock();
				}
				return false;
			}
		});
	}

	@Override
	protected void onPause() {
		super.onPause();

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
	
	private void init_activity_laser() {
		Intent intent = new Intent(TeleoperatorJoystick.this, Simulation.class);
		startActivityForResult(intent, SHOW_LASER);		
	}
}
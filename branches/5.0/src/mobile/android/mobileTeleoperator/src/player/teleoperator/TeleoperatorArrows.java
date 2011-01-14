package player.teleoperator;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageButton;

public class TeleoperatorArrows extends Activity {
	
	private static final int SHOW_LASER = 0;

	private static final int MENU_BACK = Menu.FIRST;
	private static final int MENU_LASER = Menu.FIRST + 1;

	private ImageButton buttonUp, buttonDown;
	private ImageButton buttonLeft, buttonRight;
	private ImageButton buttonSideLeft, buttonSideRight;
	private ImageButton buttonStop;
	
	private static final float V_VEL = 0.2f;
	private static final float W_VEL = 0.4f;
	private static final float L_VEL = 1; 
	
	private static final int CHANGE_V = 0;
	private static final int CHANGE_W = 1;
	private static final int CHANGE_BOTH = 2;

	private float velv, velw, vels;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.arrows);

		this.buttonUp = (ImageButton) findViewById(R.id.button_up);
		this.buttonDown = (ImageButton) findViewById(R.id.button_down);
		this.buttonLeft = (ImageButton) findViewById(R.id.button_left);
		this.buttonRight = (ImageButton) findViewById(R.id.button_right);
		this.buttonSideLeft = (ImageButton) findViewById(R.id.button_side_left);
		this.buttonSideRight = (ImageButton) findViewById(R.id.button_side_right);
		
		this.buttonStop = (ImageButton) findViewById(R.id.button_stop);

		// add a click listener to the buttons
		this.buttonUp.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				handleButtonUp();
			}
		});
		this.buttonDown.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				handleButtonDown();
			}
		});
		this.buttonLeft.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				handleButtonLeft();
			}
		});
		this.buttonRight.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				handleButtonRight();
			}
		});
		
		this.buttonSideLeft.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v){
					hadleButtonSideLeft();
			}
		});
		
		this.buttonSideRight.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v){
					hadleButtonSideRight();
			}
		});
		
		this.buttonStop.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				handleButtonStop();
			}
		});
	}
	
	@Override
	protected void onPause() {
		super.onPause();
		
		this.handleButtonStop();
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
    	
    	switch(item.getItemId()) {
    		case(MENU_BACK): {
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

	/* Handle button */
	private void handleButtonUp() {
		this.velv += V_VEL;

		//if (this.velv > V_VEL)
		//	this.velv = V_VEL;

		this.updateVel(CHANGE_V);
	}

	private void handleButtonDown() {
		this.velv -= V_VEL;

		//if (this.velv < -V_VEL)
		//	this.velv = -V_VEL;

		this.updateVel(CHANGE_V);
	}

	private void handleButtonLeft() {
		this.velw += W_VEL;

		//if (this.velw > W_VEL)
		//	this.velw = W_VEL;

		this.updateVel(CHANGE_W);
	}

	private void handleButtonRight() {
		this.velw -= W_VEL;

		//if (this.velw < -W_VEL)
		//	this.velw = -W_VEL;

		this.updateVel(CHANGE_W);
	}
	
	private void hadleButtonSideLeft(){
		this.vels -= L_VEL;
		this.updateSide();
	}
		
	private void hadleButtonSideRight(){
		this.vels += L_VEL;
		this.updateSide();
	}

	private void handleButtonStop() {
		this.velv = 0.0f;
		this.velw = 0.0f;
		this.vels = 0.0f;

		this.updateVel(CHANGE_BOTH);
		this.updateSide();
	}

	private void updateVel(int type) {
		if(type == CHANGE_V || type == CHANGE_BOTH)
			Connection.setV(this.velv);
		if(type == CHANGE_W || type == CHANGE_BOTH)
			Connection.setW(this.velw);
	}
	
	private void updateSide(){
		Connection.setL(this.vels);
	}
	
	private void init_activity_laser() {
		Intent intent = new Intent(TeleoperatorArrows.this, Simulation.class);
		startActivityForResult(intent, SHOW_LASER);		
	}
}
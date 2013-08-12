package player.teleoperator;

import android.app.Activity;
import android.app.Dialog;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TextView;

public class Teleoperator extends Activity {
	
	private static final int SHOW_CONNECT = 0;
	private static final int SHOW_ARROWS = 1;
	private static final int SHOW_ACCEL = 2;
	private static final int SHOW_JOYSTICK = 2;
	
	private static final int OK_CONNECT = 0;
	private static final int OK_DISCONNECT = 1;
	
	private static final int MENU_EXIT = Menu.FIRST;
	private static final int MENU_CONNECT = Menu.FIRST + 1;
	private static final int MENU_DISCONNECT = Menu.FIRST + 2;
	
	private ImageButton buttonConnect;
	private ImageButton buttonArrows, buttonAccel, buttonJoystick;
	
	private TextView textConnect;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        this.buttonArrows = (ImageButton) findViewById(R.id.button_arrows);
        this.buttonAccel = (ImageButton) findViewById(R.id.button_accel);
        this.buttonJoystick = (ImageButton) findViewById(R.id.button_joystick);
        this.buttonConnect = (ImageButton) findViewById(R.id.button_connect);
        
        this.textConnect = (TextView) findViewById(R.id.text_button_connect);
        
		// add a click listener to the buttons
		this.buttonArrows.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				init_activity_arrows();
			}
		});
		this.buttonAccel.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				init_activity_accel();
			}
		});
		this.buttonJoystick.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				init_activity_joystick();
			}
		});
		this.buttonConnect.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				if(!Connection.isConnected())
					init_activity_connect();
				else {
					setDisconnected();
					Connection.disconnect();
					showDialog(OK_DISCONNECT);
				}
			}
		});
		
	    this.setDisconnected();
	    
	    //Init send thread
	    //Connection.sendThread();
    }
    
	@Override
    public boolean onCreateOptionsMenu(Menu menu) {
    	super.onCreateOptionsMenu(menu);
    	
    	//Create and add menu items
    	MenuItem itemConnect = menu.add(0, MENU_CONNECT, Menu.NONE, R.string.menu_connect);
    	MenuItem itemDisconnect = menu.add(0, MENU_DISCONNECT, Menu.NONE, R.string.menu_disconnect);
    	MenuItem itemExit = menu.add(0, MENU_EXIT, Menu.NONE, R.string.menu_exit);
    	
    	//Assign icons
    	itemConnect.setIcon(R.drawable.connect_icon);
    	itemDisconnect.setIcon(R.drawable.disconnect_icon);
    	itemExit.setIcon(R.drawable.menu_exit);
    	
    	//Allocate shortcuts to each of them
    	itemConnect.setShortcut('0', 'c');
    	itemDisconnect.setShortcut('1', 'd');
    	itemExit.setShortcut('2', 'e');
    	
    	return true;
    }
    
    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
    	super.onPrepareOptionsMenu(menu);
    	
    	if(Connection.isConnected()) {
    		menu.getItem(0).setVisible(false);
    		menu.getItem(1).setVisible(true);
    	} else {
    		menu.getItem(0).setVisible(true);
    		menu.getItem(1).setVisible(false);   		
    	}

    	return true;
    }
    
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		super.onOptionsItemSelected(item);

		switch (item.getItemId()) {
		case (MENU_CONNECT): {
			init_activity_connect();
			return true;
		}
		case (MENU_DISCONNECT): {
			this.setDisconnected();
			Connection.disconnect();
			showDialog(OK_DISCONNECT);
			return true;
		}
		case (MENU_EXIT): {
			Connection.disconnect();
			Connection.setConnected(false);
			setResult(RESULT_OK, null);
			finish();
			return true;
		}
		}
		return false;
	}
	
	@Override
	protected Dialog onCreateDialog(int id) {
		switch (id) {
		case OK_CONNECT:
			return new OkDialog(this, getString(R.string.ok_msg_connect));
		case OK_DISCONNECT:
			return new OkDialog(this, getString(R.string.ok_msg_disconnect));	
		}
		return null;
	}
	
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		switch (requestCode) {
		case (SHOW_CONNECT): {
			if (resultCode == Activity.RESULT_OK) {
				this.setConnected();
				showDialog(OK_CONNECT);
			}
			break;
		}
		}
	}
	 
    private void setConnected() {
    	//Set connection to true
		Connection.setConnected(true);
		this.buttonConnect.setImageResource(R.drawable.disconnect_icon);
		this.textConnect.setText(R.string.main_button_disconnect);
		
		//Enable buttons
		this.buttonArrows.setEnabled(true);
		this.buttonAccel.setEnabled(true);
		this.buttonJoystick.setEnabled(true);
    }
    
    private void setDisconnected() {
    	//Set connection to false
    	Connection.setConnected(false);	
		this.buttonConnect.setImageResource(R.drawable.connect_icon);
		this.textConnect.setText(R.string.main_button_connect);
    	
    	//Disable buttons
    	this.buttonArrows.setEnabled(false);
    	this.buttonAccel.setEnabled(false); 
    	this.buttonJoystick.setEnabled(false); 
    }
     
    private void init_activity_connect() {
		Intent intent = new Intent(Teleoperator.this, Connect.class);
		startActivityForResult(intent, SHOW_CONNECT);
    }
    
    private void init_activity_accel() {
		Intent intent = new Intent(Teleoperator.this, TeleoperatorAccel.class);
		startActivityForResult(intent, SHOW_ACCEL);			
	}

	private void init_activity_arrows() {
		Intent intent = new Intent(Teleoperator.this, TeleoperatorArrows.class);
		startActivityForResult(intent, SHOW_ARROWS);		
	}
	
	private void init_activity_joystick() {
		Intent intent = new Intent(Teleoperator.this, TeleoperatorJoystick.class);
		startActivityForResult(intent, SHOW_JOYSTICK);		
	}
}
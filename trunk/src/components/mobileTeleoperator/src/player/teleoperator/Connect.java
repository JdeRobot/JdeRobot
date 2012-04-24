package player.teleoperator;

import jderobot.MotorsPrx;
import jderobot.MotorsPrxHelper;
import jderobot.Pose3DMotorsPrx;
import jderobot.Pose3DMotorsPrxHelper;
import android.app.Activity;
import android.app.Dialog;
import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageButton;
import android.view.Menu;

public class Connect extends Activity {

    private static final int ERROR_IP = 1;
    private static final int ERROR_PORT = 2;
    private static final int ERROR_ICE = 3;
    private static final int MENU_BACK = Menu.FIRST;
    
    private static final boolean USE_NAO_ROBOT = false;
	
	private EditText text_ip;
	private EditText text_port;
	private ImageButton button_connect;
	
	private static String my_ip = "";
	private static String my_port = "";
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.connect);
        
        // capture our View elements
        this.text_ip = (EditText) findViewById(R.id.text_ip);
        this.text_port = (EditText) findViewById(R.id.text_port);
        this.button_connect = (ImageButton) findViewById(R.id.button_connect);
        
        this.button_connect.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                tryConnection();
            }
        });
        
        //Change values
        if(!my_ip.equals(""))
        	this.text_ip.setText(my_ip);
        if(!my_port.equals(""))
        	this.text_port.setText(my_port);

    }
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
    	super.onCreateOptionsMenu(menu);
    	
    	//Create and add menu items
    	MenuItem itemBack = menu.add(0, MENU_BACK, Menu.NONE, R.string.menu_back);
    	
    	//Assign icons
    	itemBack.setIcon(R.drawable.menu_back);
    	
    	//Allocate shortcuts to each of them
    	itemBack.setShortcut('0', 'b');
    	return true;
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
    	super.onOptionsItemSelected(item);
    	
    	switch(item.getItemId()) {
    		case(MENU_BACK): {
    			setResult(RESULT_CANCELED, null);
    			finish();
    			return true;
    		}
    	}
    	return false;
    }
    
    @Override
    protected Dialog onCreateDialog(int id) {
        switch (id) {
	    case ERROR_IP:
	    	return new ErrorDialog(this, getString(R.string.error_msg_ip));
	    case ERROR_PORT:
	    	return new ErrorDialog(this, getString(R.string.error_msg_port));
	    case ERROR_ICE:
	    	return new ErrorDialog(this, getString(R.string.error_msg_ice));
	    }
        return null;
    }
    
    public void tryConnection() {
    	String ip, sport;
    	int port;
    	String proxy_motors_nao, proxy_head_nao, proxy_motors;
    	
    	ip = this.text_ip.getText().toString();
    	sport = this.text_port.getText().toString();
    	
    	if(ip.equals("")) {
    		Log.e("Connect", "Ip is empty");
    		showDialog(ERROR_IP);
    		return;
    	} else
    		my_ip = ip;
    	
    	if(sport.equals("")) {
    		Log.e("Connect", "Port is empty");
    		showDialog(ERROR_PORT);
    		return;
    	} else
    		my_port = sport;
    	
    	//Connect to player
    	try {
    		port = Integer.parseInt(sport);
    		
     		proxy_motors = "motors1:tcp -h " + ip + " -p " + port;
    		proxy_motors_nao = "Motors:tcp -h " + ip + " -p " + port;
    		proxy_head_nao = "Pose3DMotors:tcp -h " + ip + " -p " + port;
    		
    		//Initialize ICE with the IP and port selected
    		Ice.Communicator communicator = Ice.Util.initialize();
    		
    		/*Configure nao robot*/
    		if(USE_NAO_ROBOT) {
	    		Log.d("Motors Nao", "Connecting to: " + proxy_motors_nao);
	    		Ice.ObjectPrx base = communicator.stringToProxy(proxy_motors_nao);
		        if (base == null){
		        	Log.e("Motors Nao", "No Motors Nao available");
		            return;
		        } 
		        
	        	MotorsPrx mprx = MotorsPrxHelper.checkedCast(base);
		        if (mprx == null){ 
		        	Log.e("Motors Nao", "No Motors Nao available");
		            return;
		        }

		        /*Log.d("Head", "Connecting to: " + proxy_head_nao);
		   		base = communicator.stringToProxy(proxy_head_nao);
		        if (base == null){
		        	Log.e("Head", "No head available");
		            return;
		        } 
		        
	        	Pose3DMotorsPrx hprx = Pose3DMotorsPrxHelper.checkedCast(base);
		        if (hprx == null){ 
		        	Log.e("Head", "No head available");
		            return;
		        }*/
		        
		        /*Save proxy*/
		        Connection.setMotors(mprx);
		        //Connection.setHead(hprx);
		        Connection.setUseNao(true);
		        
    		} else { /*Configure player*/
    		
	    		Log.d("Motors", "Connecting to: " + proxy_motors);
	    		Ice.ObjectPrx base = communicator.stringToProxy(proxy_motors);
		        if (base == null){
		        	Log.e("Motors", "No motors available");
		            return;
		        } 
		        
	        	MotorsPrx mprx = MotorsPrxHelper.checkedCast(base);
		        if (mprx == null){ 
		        	Log.e("Motors", "No motors available");
		            return;
		        }
		        
		        /*Save proxy*/
		        Connection.setMotors(mprx);
		        Connection.setUseNao(false);
    		}
	                  
	        //Save connection
	        Connection.setCommunicator(communicator);
	        Connection.setConnected(true);
    	} catch(NumberFormatException e) {
    		Log.e("Connect", "Ip is not a valid number");
    		showDialog(ERROR_PORT);
    		return;
    	} catch(Exception e) {
    		Log.e("Connect", "Ice error: unknown");
    		showDialog(ERROR_ICE);
    		return;
    	}
    	
		setResult(RESULT_OK, null);
		finish();
    }
}

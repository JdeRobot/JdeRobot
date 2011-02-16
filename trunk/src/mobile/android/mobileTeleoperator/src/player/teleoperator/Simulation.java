package player.teleoperator;

import jderobot.LaserData;
import android.app.Activity;
import android.app.Dialog;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

public class Simulation extends Activity {
	
	private static final int MENU_BACK = Menu.FIRST;
	private static final int MENU_REFRESH = Menu.FIRST + 1;
	
	private static final int ERROR_LASER = 0;
		
	private GLSurfaceView mGLSurfaceView;
	private Renderer renderer;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        this.renderer = new Renderer();

        this.mGLSurfaceView = new GLSurfaceView(this);
        this.mGLSurfaceView.setRenderer(this.renderer);
        setContentView(this.mGLSurfaceView);
        
        if(!Connection.isLaserAvailable()) {
        	showDialog(ERROR_LASER);
        	this.renderer.setLaserData(null);
        	return;
        }
        this.refresh_data();
    }
    
    @Override
    protected void onResume() {
        super.onResume();
        mGLSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mGLSurfaceView.onPause();
    }
    
	@Override
    public boolean onCreateOptionsMenu(Menu menu) {
    	super.onCreateOptionsMenu(menu);
    	
    	//Create and add menu items
    	MenuItem itemBack = menu.add(0, MENU_BACK, Menu.NONE, R.string.menu_back);
    	MenuItem itemRefresh = menu.add(0, MENU_REFRESH, Menu.NONE, R.string.menu_refresh);
    	
    	//Assign icons
    	itemBack.setIcon(R.drawable.menu_back);
    	itemRefresh.setIcon(R.drawable.menu_refresh);
    	
    	//Allocate shortcuts to each of them
    	itemBack.setShortcut('0', 'b');
    	itemRefresh.setShortcut('1', 'r');
    	
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
		case (MENU_REFRESH): {
			refresh_data();
			return true;
		}
		}
		return false;
	}
    
    @Override
    protected Dialog onCreateDialog(int id) {
        switch (id) {
	    case ERROR_LASER:
	    	return new ErrorDialog(this, getString(R.string.error_laser));
	    }
        return null;
    }
    
    private void refresh_data() {
    	if(!Connection.isLaserAvailable()) {
        	showDialog(ERROR_LASER);
        	return;
        }
    	
        LaserData ld = Connection.getLaser().getLaserData();
        if(this.renderer != null)
        	this.renderer.setLaserData(ld);
    }
 }
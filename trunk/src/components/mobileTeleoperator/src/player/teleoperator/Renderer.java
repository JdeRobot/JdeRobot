package player.teleoperator;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import jderobot.LaserData;

import android.opengl.GLSurfaceView;
import android.opengl.GLU;
import android.util.Log;

class Renderer implements GLSurfaceView.Renderer {
	
	private static float DEG2RAD = 0.017453292f;
	
	private boolean mTranslucentBackground;
	private LaserData laserData;

	public Renderer() {
		mTranslucentBackground = false;
	}

	public void onDrawFrame(GL10 gl) {
		gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
		gl.glMatrixMode(GL10.GL_MODELVIEW);
		gl.glLoadIdentity();

		gl.glColor4f(0.0f, 0.0f, 0.0f, 0.0f);

		// Draw robot
		this.drawRobot(gl);
		
		//Draw laser
		this.drawLaser(gl);
	}

	public void onSurfaceChanged(GL10 gl, int width, int height) {
		gl.glViewport(0, 0, width, height);

		float ratio = (float) width / height;

		/* Virtual camera setting */
		float positionX = -500.f;
		float positionY = 0.f;
		float positionZ = 11000.f;
		float foaX = 0.f;
		float foaY = 0.f;
		float foaZ = 0.f;

		gl.glMatrixMode(GL10.GL_PROJECTION);
		gl.glLoadIdentity();

		// perspective projection. intrinsic parameters + frustrum
		GLU.gluPerspective(gl, 45.f, ratio, 1.0f, 50000.0f);
		// extrinsic parameters
		GLU.gluLookAt(gl, positionX, positionY, positionZ, foaX, foaY, foaZ,
				0.f, 0.f, 1.f);
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {

		gl.glDisable(GL10.GL_DITHER);
		gl.glHint(GL10.GL_PERSPECTIVE_CORRECTION_HINT, GL10.GL_FASTEST);

		if (mTranslucentBackground) {
			gl.glClearColor(0, 0, 0, 0);
		} else {
			gl.glClearColor(1, 1, 1, 1);
		}
		gl.glEnable(GL10.GL_CULL_FACE);
		gl.glShadeModel(GL10.GL_SMOOTH);
		gl.glEnable(GL10.GL_DEPTH_TEST);
	}
	
	public void setLaserData(LaserData ld) {
		this.laserData = ld;
	}

	private void drawRobot(GL10 gl) {
		
		FloatBuffer vertexBuffer;
		ShortBuffer indexBuffer;
		
		float robot1[] = {
			      -500.0f,  400.0f, 0.5f,
			      -500.0f, -400.0f, 0.5f,
			       500.0f, -400.0f, 0.5f,
			       500.0f,  400.0f, 0.5f,
			};
		
		short[] indices = { 0, 1, 2, 0, 2, 3 };

		gl.glColor4f(1.0f, 0.0f, 0.0f, 0.0f);

		// a float is 4 bytes, therefore we multiply the number if
		// vertices with 4.
		ByteBuffer vbb = ByteBuffer.allocateDirect(robot1.length * 4);
		vbb.order(ByteOrder.nativeOrder());
		vertexBuffer = vbb.asFloatBuffer();
		vertexBuffer.put(robot1);
		vertexBuffer.position(0);

		// short is 2 bytes, therefore we multiply the number if
		// vertices with 2.
		ByteBuffer ibb = ByteBuffer.allocateDirect(indices.length * 2);
		ibb.order(ByteOrder.nativeOrder());
		indexBuffer = ibb.asShortBuffer();
		indexBuffer.put(indices);
		indexBuffer.position(0);

		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glVertexPointer(3, GL10.GL_FLOAT, 0, vertexBuffer);
		gl.glDrawElements(GL10.GL_TRIANGLES, indices.length, GL10.GL_UNSIGNED_SHORT, indexBuffer);
		gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glFlush();
	}
	
	private void drawLaser(GL10 gl) {
		
		FloatBuffer vertexBuffer;
		ShortBuffer indexBuffer;
		
		//this.laserData = Connection.getLaser().getLaserData();
		
		if(this.laserData==null)
			return;
		
		int [] laser = this.laserData.distanceData;
		
		float[] values = new float[(this.laserData.numLaser+1)*3];
		short[] indices = new short[(this.laserData.numLaser+1)*3];
		
		values[0] = values[1] = values[2] = 0.0f;
		indices[0] = 0;
			
		for(int i=0;i<this.laserData.numLaser;i++) {
			values[(i+1)*3] = (float) Math.sin(i*DEG2RAD)*(float)laser[i];
			values[(i+1)*3+1] = (float) -Math.cos(i*DEG2RAD)*(float)laser[i];
			values[(i+1)*3+2] = 0.0f;
			
			indices[i+1] = (short) (i+1);
		}
		
		gl.glColor4f(0.0f, 0.0f, 1.0f, 0.0f);
		
		// a float is 4 bytes, therefore we multiply the number if
		// vertices with 4.
		ByteBuffer vbb = ByteBuffer.allocateDirect(values.length * 4);
		vbb.order(ByteOrder.nativeOrder());
		vertexBuffer = vbb.asFloatBuffer();
		vertexBuffer.put(values);
		vertexBuffer.position(0);

		// short is 2 bytes, therefore we multiply the number if
		// vertices with 2.
		ByteBuffer ibb = ByteBuffer.allocateDirect(indices.length * 2);
		ibb.order(ByteOrder.nativeOrder());
		indexBuffer = ibb.asShortBuffer();
		indexBuffer.put(indices);
		indexBuffer.position(0);

		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glVertexPointer(3, GL10.GL_FLOAT, 0, vertexBuffer);
		gl.glDrawElements(GL10.GL_TRIANGLE_FAN, indices.length, GL10.GL_UNSIGNED_SHORT, indexBuffer);
		gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glFlush();
	}
}
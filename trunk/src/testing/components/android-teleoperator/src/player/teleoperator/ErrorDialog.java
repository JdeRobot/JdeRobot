package player.teleoperator;

import android.app.Dialog;

import android.content.Context;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class ErrorDialog extends Dialog {
	
	private TextView text;
	private Button button;

	public ErrorDialog(Context context, String msg) {
		super(context);
		this.setContentView(R.layout.errordialog);
		this.setTitle(R.string.error_title);
		
		this.text = (TextView) this.findViewById(R.id.text);
		this.button = (Button) this.findViewById(R.id.button);
		
		this.text.setText(msg);
		this.button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
               closeDialog();
            }
        });
	}
	
	private void closeDialog() {
		this.dismiss();
	}
}

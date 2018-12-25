package com.example.panzq.hello;

import android.app.Activity;
import android.os.Bundle;
import android.os.IHelloService;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends Activity implements View.OnClickListener {
    private final static String LOG_TAG = "panzqww";

    private IHelloService helloService = null;

    private EditText valueText = null;
    private Button readButton = null;
    private Button writeButton = null;
    private Button clearButton = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        helloService = IHelloService.Stub.asInterface(
                ServiceManager.getService("hello"));

        valueText = (EditText)findViewById(R.id.edit_value);
        readButton = (Button)findViewById(R.id.button_read);
        writeButton = (Button)findViewById(R.id.button_write);
        clearButton = (Button)findViewById(R.id.button_clear);

        readButton.setOnClickListener(this);
        writeButton.setOnClickListener(this);
        clearButton.setOnClickListener(this);


    }

    @Override
    public void onClick(View v) {
        if (v.equals(readButton)) {
            try {
                int val = helloService.getVal();
                String text = String.valueOf(val);
                valueText.setText(text);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Remote Exception while reading value from device.");
            }
        } else if (v.equals(writeButton)) {
            try {
                String text = valueText.getText().toString();
                int val = Integer.parseInt(text);
                helloService.setVal(val);
            } catch (RemoteException e) {
                Log.e(LOG_TAG, "Remote Exception while writing value to device.");
            }
        } else if (v.equals(clearButton)) {
            String text = "";
            valueText.setText(text);
        }

    }
}

package blog.claybailey.seniorproject;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.MotionEvent;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */

    //declare native functions HERE

    public native String stringFromJNI();//example

    private native long startEngine();
    private native void stopEngine(long engineHandle);
    private native void tap(long engineHandle, boolean b);

    //member data
    private static long mEngineHandle = 0;


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());//changes text from "Hello World" to "Hello from C++"

        //start the audio engine
        mEngineHandle = startEngine();
    }

    //logic to handle screen taps.
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if(event.getAction() == MotionEvent.ACTION_DOWN){
            tap(mEngineHandle, true);
        } else if (event.getAction() == MotionEvent.ACTION_UP){
            tap(mEngineHandle, false);
        }
        return super.onTouchEvent(event);
    }

    @Override
    protected void onPause() {
        stopEngine(mEngineHandle);
        super.onPause();
    }
}

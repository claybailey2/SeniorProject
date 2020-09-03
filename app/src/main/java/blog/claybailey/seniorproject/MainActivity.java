package blog.claybailey.seniorproject;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.media.AudioManager;
import android.os.Build;
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

    private native void startEngine();
    private native void stopEngine();
    private native void tap(boolean b);

    private static native void native_setDefaultStreamValues(int sampleRate, int framesPerBurst);

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    //Display content and configure audio when app is started
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());//changes text from "Hello World" to "Hello from C++"

        //Configure app to hardware-specific audio output settings
        setDefaultStreamValues(this);
    }

    //Start the audio engine when app in focus
    @Override
    protected void onResume() {
        startEngine();
        super.onResume();
    }

    //Stop the audio when app is out of focus
    @Override
    protected void onPause() {
        stopEngine();
        super.onPause();
    }

    //logic to handle screen taps.
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if(event.getAction() == MotionEvent.ACTION_DOWN){
            tap(true);
        } else if (event.getAction() == MotionEvent.ACTION_UP){
            tap(false);
        }
        return super.onTouchEvent(event);
    }

    //Asks hardware for audio stream settings
    //code block from MegaDrone Sample
    static void setDefaultStreamValues(Context context) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1){
            AudioManager myAudioMgr = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
            String sampleRateStr = myAudioMgr.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE);
            int defaultSampleRate = Integer.parseInt(sampleRateStr);
            String framesPerBurstStr = myAudioMgr.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER);
            int defaultFramesPerBurst = Integer.parseInt(framesPerBurstStr);

            native_setDefaultStreamValues(defaultSampleRate, defaultFramesPerBurst);
        }
    }
}

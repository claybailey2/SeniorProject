package blog.claybailey.seniorproject;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.pm.ActivityInfo;
import android.media.AudioManager;
import android.os.Build;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;

import java.util.logging.Level;
import java.util.logging.Logger;

public class MainActivity extends AppCompatActivity {
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */

    //declare native functions HERE
    private static long mEngineHandle = 0;

    private native long startEngine();
    private native void stopEngine(long engineHandle);
    private native void resumeEngine(long engineHandle);
    private native void tap(long engineHandle);

    private static native void native_setDefaultStreamValues(int sampleRate, int framesPerBurst);

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    //Display content and configure audio when app is started
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        setContentView(R.layout.activity_main);
        Button kickButton = (Button)findViewById(R.id.button_kick);
        if(kickButton != null) {
            kickButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if (mEngineHandle == 0) {
                        mEngineHandle = startEngine();
                    }
                    tap(mEngineHandle);
                }
            });
        } else {
            Logger.getAnonymousLogger().log(Level.FINER, "button not found");
        }

        //Configure app to hardware-specific audio output settings
        setDefaultStreamValues(this);
    }

    //Start the audio engine when app in focus
    @Override
    protected void onResume() {
        if (mEngineHandle != 0) resumeEngine(mEngineHandle);
        super.onResume();
    }

    //Stop the audio when app is out of focus
    @Override
    protected void onPause() {
        stopEngine(mEngineHandle);
        super.onPause();
    }

//    //logic to handle screen taps.
//    @Override
//    public boolean onTouchEvent(MotionEvent event) {
//        Logger.getAnonymousLogger().log(Level.FINER, "Touch event");
//        if(mEngineHandle == 0) {
//            mEngineHandle = startEngine();
//        }
//
//        if (event.getAction() == MotionEvent.ACTION_DOWN) {
//            tap(mEngineHandle, true);
//        } else if (event.getAction() == MotionEvent.ACTION_UP) {
//            tap(mEngineHandle, false);
//        }
//
//        return super.onTouchEvent(event);
//
//    }

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

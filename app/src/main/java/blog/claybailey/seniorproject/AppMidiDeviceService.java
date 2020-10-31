package blog.claybailey.seniorproject;

import android.media.midi.MidiDeviceService;
import android.media.midi.MidiReceiver;
import android.util.Log;

import java.io.IOException;

public class AppMidiDeviceService extends MidiDeviceService {
    private MidiReceiver mInput = new VirtualDevice();
    //private MidiDeviceServer mOutput = new VirtualServer();

    private static final String TAG = "AppMidiDeviceService";

    @Override
    public MidiReceiver[] onGetInputPortReceivers() {
        return new MidiReceiver[]{ mInput };
    }

    class VirtualDevice extends MidiReceiver {
        @Override
        public void onSend(byte[] msg, int offset, int count, long timestamp) throws IOException {
            //Log.d(TAG,"Virtual device send!");
            //send to AppMidiDeviceService's output port
            MidiReceiver[] outputPortReceivers = getOutputPortReceivers();
            //Log.d(TAG, "There are " + outputPortReceivers.length + " output ports");
            outputPortReceivers[0].send(msg, offset, count, timestamp);
        }
    }

    //class VirtualServer extends MidiDeviceServer {

    //}
}

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import java.util.concurrent.LinkedBlockingQueue;

class StreamConsumer extends Thread {
    private InputStream source;
    private StreamType type;
    private boolean broadcast;
    private LinkedBlockingQueue<Message> queue;
    private int id;

    StreamConsumer(InputStream source, StreamType type, boolean broadcast,
                   LinkedBlockingQueue<Message> queue, int id) {
        this.source = source;
        this.type = type;
        this.broadcast = broadcast;
        this.queue = queue;
        this.id = id;
    }

    public void run()
    {
        try
        {
            BufferedReader br = new BufferedReader(new InputStreamReader(source));
            String message = null;
            while ((message = br.readLine()) != null)
                queue.add(new Message(type, broadcast, message, id));
        } catch (IOException e) {
            System.err.println("IO Exception while reading on ID " + id);
        }
    }
}

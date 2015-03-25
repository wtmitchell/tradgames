public class Message
{
    public StreamType type;
    public boolean broadcast;
    public String message;
    public int id;

    public Message() {
        type = StreamType.stdin;
        broadcast = false;
        message = "";
        id = -1;
    }

    public Message(String message) {
        type = StreamType.stdin;
        broadcast = true;
        this.message = message;
        this.id = -1;
    }

    public Message(StreamType type, boolean broadcast, String message, int id) {
        this.type = type;
        this.broadcast = broadcast;
        this.message = message;
        this.id = id;
    }

    public String toString() {
        if (broadcast && type == StreamType.stdout)
            return id + ":" + type + " all: " + message;
        return id + ":" + type + ": " + message;
    }
}

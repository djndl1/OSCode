import java.util.ArrayList;
import java.util.List;

public class SimpleThread {
    public static void main(String[] args) {

        List<Thread> threads = new ArrayList<>(10);
        for (int i = 1; i < 10; i++) {
            var thread = new Thread(new SimpleRunnable(i));
            thread.start();
            threads.add(thread);
        }

        threads.stream().forEach(t -> {
            try {
                t.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        });
    }
}

class SimpleRunnable implements Runnable {

    private int id;

    public SimpleRunnable(int id) {
        this.id = id;
    }

    @Override
    public void run() {
        System.out.println("Thread " + this.id + " running");
    }
}

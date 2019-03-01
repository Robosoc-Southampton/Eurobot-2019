package com.robosoc.eurobot2018;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Main class.
 * @author billy
 *
 */
public class App {
	
	/**
	 * Main method. Starts listening on the specified port and accepts one
	 * connection at a time from the main python program / any TCP connection.
	 * @param args Arg 1: port to use. Arg 2: camera, -1 is pi cam, otherwise webcam no.
	 */
    public static void main( String[] args ) {
    	int port = Integer.parseInt(args[0]);
    	int device = args.length > 1 ? Integer.parseInt(args[1]) : 0;
    	
    	try (ServerSocket serverSocket = new ServerSocket(port)) {
    		
    		while(true) {
    			new Connection(serverSocket.accept(), device);
    		}
    		
    	} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
}

package com.robosoc.eurobot2018;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class App {
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

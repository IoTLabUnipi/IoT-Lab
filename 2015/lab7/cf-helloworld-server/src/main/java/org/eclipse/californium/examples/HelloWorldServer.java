/*******************************************************************************
 * Copyright (c) 2014 Institute for Pervasive Computing, ETH Zurich and others.
 * 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 * 
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.html.
 * 
 * Contributors:
 *    Matthias Kovatsch - creator and main architect
 ******************************************************************************/
package org.eclipse.californium.examples;

import java.net.SocketException;

import org.eclipse.californium.core.CoapServer;
import org.eclipse.californium.examples.resources.HelloWorldResource;
import org.eclipse.californium.examples.resources.MirrorResource;
import org.eclipse.californium.examples.resources.SquareResource;


public class HelloWorldServer extends CoapServer {
    
    /*
     * Application entry point.
     */
    public static void main(String[] args) {
        
        try {
            
            // create server
            HelloWorldServer server = new HelloWorldServer();
            server.start();
            
        } catch (SocketException e) {
            
            System.err.println("Failed to initialize server: " + e.getMessage());
        }
    }
    
    /*
     * Constructor for a new Hello-World server. Here, the resources
     * of the server are initialized.
     */
    public HelloWorldServer() throws SocketException {
        
        // provide an instance of a Hello-World resource
        add(new HelloWorldResource());
        add(new MirrorResource("Mirror"));
        add(new SquareResource("Square"));
    }
    
    /*
     * Definition of the Hello-World Resource
     */
    
}

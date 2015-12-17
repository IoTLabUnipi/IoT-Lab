package org.eclipse.californium.examples.resources;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.server.resources.CoapExchange;

public class HelloWorldResource extends CoapResource {
        
    public HelloWorldResource() {
        
        // set resource identifier
        super("helloWorld");
        
        // set display name
        getAttributes().setTitle("Hello-World Resource");
    }
    
    @Override
    public void handleGET(CoapExchange exchange) {
        
        // respond to the request
        exchange.respond("Hello World!");
    }
}


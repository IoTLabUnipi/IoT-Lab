package org.eclipse.californium.examples.resources;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.coap.Request;
import org.eclipse.californium.core.server.resources.CoapExchange;
import org.eclipse.californium.core.server.resources.Resource;

public class MirrorResource extends CoapResource {

	public MirrorResource(String name) {
		super(name);
	}
	
	@Override
	public Resource getChild(String name) {
		return this;
	}
	
	@Override
	public void handleGET(CoapExchange exchange) {
		
		Request request = exchange.advanced().getCurrentRequest();
		StringBuilder buffer = new StringBuilder();
		buffer.append("resource ").append(getURI()).append(" received request")
			.append("\n").append("Code: ").append(request.getCode())
			.append("\n").append("Source: ").append(request.getSource()).append(":").append(request.getSourcePort())
			.append("\n").append("Type: ").append(request.getType())
			.append("\n").append("MID: ").append(request.getMID())
			.append("\n").append("Token: ").append(request.getTokenString())
			.append("\n").append(request.getOptions());
		
		exchange.respond(buffer.toString());
	}
}

package org.eclipse.californium.examples.resources;

import java.util.List;
import java.util.regex.Pattern;

import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.coap.CoAP.ResponseCode;
import org.eclipse.californium.core.coap.Request;
import org.eclipse.californium.core.server.resources.CoapExchange;

public class SquareResource extends CoapResource {
	private Pattern pattern;
	
	public SquareResource(String name) {
		super(name);
		this.pattern = Pattern.compile("number=\\d*");
	}

	@Override
	public void handleGET(CoapExchange exchange) {
		Request request = exchange.advanced().getCurrentRequest();
		int n = 2000;
		if (request.getOptions().getURIQueryCount() > 0) {
			try {
				List<String> queries = request.getOptions().getUriQuery();
				for (String query:queries) {
					if (pattern.matcher(query).matches()) {
						n = Integer.parseInt(query.split("=")[1]);
						System.out.println("number="+n);
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
				exchange.respond(ResponseCode.BAD_REQUEST, e.getMessage());
				return;
			}
		}
		
		int res = n * n;
		exchange.respond(n+"^2 = "+res);
	}
}

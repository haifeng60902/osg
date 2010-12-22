//
//  HttpManager.m
//  iMARTClientDemo
//
//  Created by Injun Song on 1/20/10.
//  Copyright 2010 Korea Institute of Science and Technology. All rights reserved.
//

#import "HttpManager.h"


@implementation HttpManager

@synthesize delegate;
@synthesize selector;
@synthesize uri;
@synthesize method;
@synthesize body;

- (BOOL)openHttpConnection {
	
	NSMutableURLRequest *req;
	
	req = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:uri]];
	
	if ([method caseInsensitiveCompare:@"post"] == NSOrderedSame) {
		[req setHTTPMethod:@"POST"];
		[req setHTTPBody:body];
	}
	
	conn = [[NSURLConnection alloc] initWithRequest:req delegate:self];
	if (conn == nil) {
		return NO;
	}
	
	recvData = [[NSMutableData alloc] init];
	return YES;
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
	[recvData appendData:data];
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response {
	[recvData setLength:0];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection {
	if (delegate) {
		[delegate performSelector:selector withObject:recvData];
	}
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
	if (delegate) {
		[delegate performSelector:selector withObject:recvData];
	}
	
}

- (void)dealloc {
	[uri release];
	[method release];
	[body release];
	[conn dealloc];
	[recvData dealloc];
	[super dealloc];
}

@end

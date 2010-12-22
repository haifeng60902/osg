//
//  HttpManager.h
//  iMARTClientDemo
//
//  Created by Injun Song on 1/20/10.
//  Copyright 2010 Korea Institute of Science and Technology. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface HttpManager : NSObject {
	
	id delegate;
	SEL selector;
	
	NSString *uri;
	NSString *method;
	NSData *body;
	
	NSMutableData *recvData;
	NSURLConnection *conn;
}

@property (nonatomic, assign) id delegate;
@property (nonatomic, assign) SEL selector;
@property (nonatomic, retain) NSString *uri;
@property (nonatomic, retain) NSString *method;
@property (nonatomic, retain) NSData *body;


- (BOOL)openHttpConnection;


@end

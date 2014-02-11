#import "ObjectiveDDP.h"

@protocol DDPAuthDelegate;
@protocol DDPSubscriptionDelegate;
@protocol DDPConnectionDelegate;

extern NSString * const MeteorClientDidConnectNotification;
extern NSString * const MeteorClientDidDisconnectNotification;

/** Errors due to transport (connection) problems will have this domain. For errors being reported
    from the backend, they will have the "errorType" key as their error domain. */
extern NSString * const MeteorClientTransportErrorDomain;

// xxx:
NS_ENUM(NSUInteger, MeteorClientError) {
    MeteorClientErrorNotConnected,
    MeteorClientErrorDisconnectedBeforeCallbackComplete,
    MeteorClientErrorLogonRejected
};

typedef NS_ENUM(NSUInteger, AuthState) {
    AuthStateNoAuth,
    AuthStateLoggingIn,
    AuthStateLoggedIn,
    /* implies using auth but not currently authorized */
    AuthStateLoggedOut
};

typedef void(^MeteorClientMethodCallback)(NSDictionary *response, NSError *error);

@interface MeteorClient : NSObject<ObjectiveDDPDelegate>

@property (nonatomic, strong) ObjectiveDDP *ddp;
@property (nonatomic, weak) id<DDPConnectionDelegate> connectionDelegate;
@property (nonatomic, weak) id<DDPSubscriptionDelegate> subscriptionDelegate;
@property (nonatomic, weak) id<DDPAuthDelegate> authDelegate;
@property (nonatomic, strong, readonly) NSMutableDictionary *collections;
@property (nonatomic, copy, readonly) NSString *userId;
@property (nonatomic, assign, readonly) BOOL websocketReady;
@property (nonatomic, assign, readonly) BOOL connected;
@property (nonatomic, assign, readonly) AuthState authState;

#pragma mark - Methods

- (NSString *)callMethodName:(NSString *)methodName parameters:(NSArray *)parameters responseCallback:(MeteorClientMethodCallback)asyncCallback;
- (void)logonWithUsername:(NSString *)username password:(NSString *)password responseCallback:(MeteorClientMethodCallback)responseCallback;
- (void)logonWithUserParameters:(NSDictionary *)userParameters username:(NSString *)username password:(NSString *)password responseCallback:(MeteorClientMethodCallback)responseCallback;
- (void)addSubscription:(NSString *)subscriptionName;
- (void)addSubscription:(NSString *)subscriptionName withParameters:(NSArray *)parameters;
- (void)removeSubscription:(NSString *)subscriptionName;
- (void)logout;
- (void)disconnect;

// Deprecated methods

- (void)logonWithUsername:(NSString *)username password:(NSString *)password __attribute__((deprecated("use logonWithUsername:password:responseCallback: instead")));
- (NSString *)sendWithMethodName:(NSString *)methodName parameters:(NSArray *)parameters notifyOnResponse:(BOOL)notify __attribute__((deprecated("use callMethodName:parameters:responseCallback: instead")));
- (void)sendWithMethodName:(NSString *)methodName parameters:(NSArray *)parameters __attribute__((deprecated("use callMethodName:parameters:responseCallback: instead")));

@end

#pragma mark - <DDPAuthDelegate>

@protocol DDPAuthDelegate <NSObject>

- (void)authenticationWasSuccessful;
- (void)authenticationFailed:(NSString *)reason;

@end

/*
 * Delegate Method for callbacks regarding Connection status
 */

#pragma mark - <DDPConnectionDelegate>

@protocol DDPConnectionDelegate <NSObject>

- (void) clientConnected;
- (void) clientDisconnected;
- (void) ddpConnected;

@end

/*
 * Delegate Method for callbacks regarding Subscription updates
 */

#pragma mark - <DDPSubscriptionDelegate>

@protocol DDPSubscriptionDelegate <NSObject>

- (void) readyMessage:(NSString *) name;
- (void) addedMessage:(NSString *) name data:(NSDictionary *) data;
- (void) removedMessage:(NSString *) name;
- (void) changedMessage:(NSString *) name data:(NSDictionary *) data;

@end

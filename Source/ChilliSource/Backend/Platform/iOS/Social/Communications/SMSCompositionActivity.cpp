/*
 *  SMSCompositionActivity.cpp
 *  moFlow
 *
 *  Created by Stuart McGaw on 09/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/iOS/Social/Communications/SMSCompositionActivity.h>

#include <ChilliSource/Backend/Platform/iOS/Core/Base/EAGLView.h>
#include <ChilliSource/Backend/Platform/iOS/Core/String/NSStringUtils.h>
#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource{
	namespace iOS {
		bool SMSCompositionActivity::IsA(Core::InterfaceIDType inID) const{
			return inID == SMSCompositionActivity::InterfaceID;
		}
		bool SMSCompositionActivity::SupportedByDevice(){
			
			NSString *reqSysVer = @"4.0";
			NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
			bool osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
			if (osVersionSupported){
				
				return [MFMessageComposeViewController canSendText];
				
			}
			
			return false;
		}
		
		SMSCompositionActivity::SMSCompositionActivity()
		:mpDelegate(nil)
		{
			
		}
		SMSCompositionActivity::~SMSCompositionActivity(){
			[mpDelegate release];
			mpDelegate = nil;
		}
		void SMSCompositionActivity::Present(const std::vector<Core::UTF8String> & inastrRecipientNumbers, const Core::UTF8String & instrContents, const SMSCompositionActivity::SendResultDelegate & inCallback){
			
			mpDelegate = [[SMSDelegate alloc] initWithCallback:inCallback];
						
			UIViewController * pRootVC = [EAGLView sharedInstance].viewController;
			
			mpVC = [[MFMessageComposeViewController alloc] init];
			mpVC.messageComposeDelegate = mpDelegate;
			
			NSMutableArray * pNamesArray = [[NSMutableArray alloc] initWithCapacity:inastrRecipientNumbers.size()];
			
			for (u32 nRecipient = 0; nRecipient < inastrRecipientNumbers.size(); nRecipient++)
            {
                NSString* recipientNumber = [NSStringUtils newNSStringWithUTF8String:inastrRecipientNumbers[nRecipient]];
				[pNamesArray addObject:recipientNumber];
                [recipientNumber release];
			}
			mpVC.recipients = pNamesArray;
			
            NSString* body = [NSStringUtils newNSStringWithUTF8String:instrContents];
 			mpVC.body = body;
            [body release];
            
			[pRootVC presentModalViewController:mpVC animated:YES];
			
			[mpVC release];
			[pNamesArray release];

		}
		void SMSCompositionActivity::Dismiss()
        {
			[mpDelegate release];
			mpVC.messageComposeDelegate = nil;
            
            if(mpVC.parentViewController)
            {
                [mpVC.parentViewController dismissModalViewControllerAnimated:YES];
            }
            else if([mpVC presentingViewController])
            {
                [[mpVC presentingViewController] dismissModalViewControllerAnimated:YES];
            }
		}

	}
}

@implementation SMSDelegate

-(SMSDelegate*) initWithCallback:(ChilliSource::Social::SMSCompositionActivity::SendResultDelegate)callback{
	
	if ((self = [super init])){
	
		mCallback = callback;
		
	}
	
	return self;
}

- (void)messageComposeViewController:(MFMessageComposeViewController *)controller didFinishWithResult:(MessageComposeResult)result 
{
	using namespace ChilliSource::Social;
    
    if(controller.parentViewController)
    {
        [controller.parentViewController dismissModalViewControllerAnimated:YES];
    }
    else if([controller presentingViewController])
    {
        [[controller presentingViewController] dismissModalViewControllerAnimated:YES];
    }
    
	SMSCompositionActivity::SendResult eResult;
	switch (result) {
		case MessageComposeResultSent:
			eResult = SMSCompositionActivity::SendResult::k_succeed;
			break;
		case MessageComposeResultCancelled:
			eResult = SMSCompositionActivity::SendResult::k_cancelled;
			break;
		case MessageComposeResultFailed:
			eResult = SMSCompositionActivity::SendResult::k_failed;
	}
	
	if (mCallback){
		mCallback(eResult);
	}
}

@end
//
//  ApplicationEvents.h
//  moFloTest
//
//  Created by Scott Downie on 13/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_CORE_APPLICATION_EVENTS_H_
#define _MO_FLO_CORE_APPLICATION_EVENTS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/GenericEvent.h>
#include <ChilliSource/Core/Base/Screen.h>

namespace ChilliSource
{
    namespace Core
    {
        typedef std::function<void()> ApplicationSystemDelegate;
        typedef std::function<void(ScreenOrientation)> ApplicationScreenOrientationDelegate;
		typedef std::function<void(u32, u32)> ApplicationScreenResizeDelegate;
        typedef std::function<void(const std::string&)> ApplicationReceivesURLDelegate;
		
        class CApplicationEvents
        {
        public:
            
            //-----------------------------------------------------------------
            /// Get Resume Event
            ///
            /// @return Event that is triggered when the application resumes
            //-----------------------------------------------------------------
            static CEvent0<ApplicationSystemDelegate>& GetResumeEvent();
            //-----------------------------------------------------------------
            /// Get Suspend Event
            ///
            /// @return Event that is triggered on application suspension
            //-----------------------------------------------------------------
            static CEvent0<ApplicationSystemDelegate>& GetSuspendEvent();
			//-----------------------------------------------------------------
            /// Get Late Suspend Event
            ///
            /// @return Event that is triggered on application suspension after GetSuspendEvent
            //-----------------------------------------------------------------
            static CEvent0<ApplicationSystemDelegate>& GetLateSuspendEvent();
            //-----------------------------------------------------------------
            /// Get Low Memory Event
            ///
            /// @return Event that is triggered when the application 
            /// receives a memory warning
            //-----------------------------------------------------------------
            static CEvent0<ApplicationSystemDelegate>& GetLowMemoryEvent();
            //-----------------------------------------------------------------
            /// Get Screen Orientation Changed Event
            ///
            /// @return Event that is triggered when screen rotates
            //-----------------------------------------------------------------
            static CEvent1<ApplicationScreenOrientationDelegate>& GetScreenOrientationChangedEvent();
            //-----------------------------------------------------------------
            /// Get Back Event
            ///
            /// @return Event that is triggered when back is pressed
            //-----------------------------------------------------------------
            static CEvent0<ApplicationSystemDelegate>& GetGoBackEvent();
			//-----------------------------------------------------------------
            /// Get Screen Resized Event
            ///
            /// @return Event that is triggered when screen resizes
            //-----------------------------------------------------------------
            static CEvent2<ApplicationScreenResizeDelegate>& GetScreenResizedEvent();
			//-----------------------------------------------------------------
            /// GetReceivesURLEvent
            ///
            /// @return Event that is triggered when the app receives a URL string
            //-----------------------------------------------------------------
            static CEvent1<ApplicationReceivesURLDelegate>& GetReceivesURLEvent();  
        private:
            
            static CEvent0<ApplicationSystemDelegate> mResumeEvent;
            static CEvent0<ApplicationSystemDelegate> mSuspendEvent;
			static CEvent0<ApplicationSystemDelegate> mLateSuspendEvent;
            static CEvent0<ApplicationSystemDelegate> mLowMemoryEvent;
            
            static CEvent1<ApplicationScreenOrientationDelegate> mScreenOrientationChangedEvent;
            static CEvent0<ApplicationSystemDelegate> mGoBackEvent;
			static CEvent2<ApplicationScreenResizeDelegate> mScreenResizedEvent;
			static CEvent1<ApplicationReceivesURLDelegate> mReceivesURLEvent;
        };
    }
}

#endif
